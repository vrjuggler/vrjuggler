/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2005 by Iowa State University
 *
 * Original Authors:
 *   Allen Bierbaum, Christopher Just,
 *   Patrick Hartling, Kevin Meinert,
 *   Carolina Cruz-Neira, Albert Baker
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 *
 * -----------------------------------------------------------------
 * File:          $RCSfile$
 * Date modified: $Date$
 * Version:       $Revision$
 * -----------------------------------------------------------------
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

#include <gadget/gadgetConfig.h>

#include <sstream>
#include <fstream>

#include <cppdom/cppdom.h>
#include <gmtl/Math.h>
#include <gmtl/Matrix.h>
#include <gmtl/MatrixOps.h>
#include <gmtl/Output.h>
#include <gmtl/Vec.h>
#include <gmtl/VecOps.h>

#include <vpr/Util/Debug.h>
#include <vpr/Util/Assert.h>
#include <jccl/Config/ParseUtil.h>
#include <jccl/Config/ConfigElement.h>
#include <gadget/Filter/Position/PositionCalibrationFilter.h>
#include <gadget/Filter/Position/PositionFilterFactory.h>

namespace gadget
{

   GADGET_REGISTER_POSFILTER_CREATOR( PositionCalibrationFilter );
   
   PositionCalibrationFilter::~PositionCalibrationFilter()
   {
      for (size_t i = 0; i < mTable.size(); ++i)
      {
         delete[] mWMatrix[i];
      }
      delete[] mWMatrix;
   }
   
   std::string
   PositionCalibrationFilter::getElementType()
   {
      return std::string("position_calibration_filter");
   }
   
   bool
   PositionCalibrationFilter::config(jccl::ConfigElementPtr e)
   {
      vprASSERT(e->getID() == PositionCalibrationFilter::getElementType());
      vprDEBUG_OutputGuard(vprDBG_ALL, vprDBG_VERB_LVL,
                           std::string("PositionCalibrationFilter::config:  ") +
                           e->getFullName() + std::string(":") + e->getID() + 
                           std::string("\n"),
                    std::string("PositionCalibrationFilter::config: done.\n") );
      std::string file_name = e->getProperty<std::string>("calibration_file",0);
      mFileName = jccl::ParseUtil::expandFileName(file_name, std::string("") );

      // Parse the calibration file.
      vprDEBUG(vprDBG_ALL, vprDBG_VERB_LVL)
         << "[PositionCalibrationFilter::config] Parsing " << mFileName << "\n"
         << vprDEBUG_FLUSH;

      cppdom::ContextPtr context( new cppdom::Context() );
      cppdom::DocumentPtr document( new cppdom::Document(context) );
      
      try
      {
         document->loadFile(mFileName);
      }
      catch (cppdom::Error e)
      {
         vprDEBUG(vprDBG_ERROR, vprDBG_CONFIG_LVL)
            << "[PositionCalibrationFilter::config] Unable to load calibration "
            << "file " << mFileName << ".  " << e.getString() << " at "
            << e.getInfo() << vprDEBUG_FLUSH;
         return false;
      }

      cppdom::NodePtr root = document->getChild("CalibrationTable");
      if (NULL == root.get())
      {
         vprDEBUG(vprDBG_ERROR, vprDBG_CONFIG_LVL)
            << "[PositionCalibrationFilter::config] Bad calibration file "
            << mFileName << "; could not retrieve the root node "
            << "'CalibrationTable'.  " << vprDEBUG_FLUSH;
         return false;
      }
      
      cppdom::NodeList offset_list;
      offset_list = root->getChildren("Offset");
      cppdom::NodeListIterator itr;
      
      for (itr = offset_list.begin(); itr != offset_list.end(); ++itr)
      {
         gmtl::Vec3d real_position;
         gmtl::Vec3d dev_position;
         
         if ( (*itr)->hasAttribute("X") && 
              (*itr)->hasAttribute("Y") && 
              (*itr)->hasAttribute("Z") )
         {
            real_position.set( (*itr)->getAttribute("X").getValue<double>(),
                               (*itr)->getAttribute("Y").getValue<double>(),
                               (*itr)->getAttribute("Z").getValue<double>() );
            std::istringstream offset_stream((*itr)->getCdata());
            //XXX:  Error checking on the Cdata needs to be more robust.
            offset_stream >> dev_position[gmtl::Xelt];
            offset_stream >> dev_position[gmtl::Yelt];
            offset_stream >> dev_position[gmtl::Zelt];

            vprDEBUG(vprDBG_ALL, vprDBG_VERB_LVL)
               << "[PositionCalibrationFilter::config()] Added "
               << real_position << " --> " << dev_position << " "
               << "to the table.\n"
               << vprDEBUG_FLUSH;
            
            mTable.push_back( std::make_pair(real_position, dev_position) );
         }
         else
         {
            vprDEBUG(vprDBG_ERROR, vprDBG_CONFIG_LVL)
               << "[PositionCalibrationFilter::config()] Malformed Offset "
               << "Element.  The element does not contain X, Y, and Z "
               << "attributes that represent a position; skipping.\n"
               << vprDEBUG_FLUSH;
         }
      }

      // Now that we have the offsets, loop through the alpha elements to
      // obtain the coefficients.
      mAlphaVec.reserve(mTable.size());
      cppdom::NodeList alpha_list = root->getChildren("Alpha");
      if (alpha_list.empty())
      {
         vprDEBUG(vprDBG_ERROR, vprDBG_CONFIG_LVL)
            << "[PositionCalibrationFilter::config()] Bad calibration file "
            << "'" << mFileName << "'\n" << "This file contains no Alpha "
            << " elements; these coefficients are necessary for calibration!\n"
            << vprDEBUG_FLUSH;
         return false;
      }
      for (itr = alpha_list.begin(); itr != alpha_list.end(); ++itr)
      {
         if ( (*itr)->hasAttribute("X") && 
              (*itr)->hasAttribute("Y") && 
              (*itr)->hasAttribute("Z") )
         {
            gmtl::Vec3d alpha_value(
                  (*itr)->getAttribute("X").getValue<double>(),
                  (*itr)->getAttribute("Y").getValue<double>(),
                  (*itr)->getAttribute("Z").getValue<double>() );
            vprDEBUG(vprDBG_ALL, vprDBG_VERB_LVL)
               << "[PositionCalibrationFilter::config()] Added "
               << alpha_value 
               << " to the alpha vector.\n"
               << vprDEBUG_FLUSH;
            mAlphaVec.push_back(alpha_value);
         }
         else
         {
            vprDEBUG(vprDBG_ERROR, vprDBG_CONFIG_LVL)
               << "[PositionCalibrationFilter::config()] "
               << "Malformed Alpha Element; this element does not contain "
               << "X="", Y="", Z="" attributes.\n"
               << vprDEBUG_FLUSH;
         }
      }

      // Now that we have the calibration table, compute the W Matrix and then
      // solve for the Alpha Vector.

      vprDEBUG(vprDBG_ALL, vprDBG_VERB_LVL)
         << "[PositionCalibrationFilter::config()] Calibration table parsed; "
         << "preparing W Matrix...\n" << vprDEBUG_FLUSH;

      // The "W" Matrix is an NxN matrix where N = mTable.size()
      // It is composed of elements computed using the w(p) function:
      // w[j](p) = sqrt( length(p - p[j])^2 + R^2 )
      // where 10 <= R^2 <= 1000.
      // The matrix looks like:
      // ( w[1](p[1]) w[2](p[1]) w[3](p[1]) ... w[N](p[1]) )
      // ( w[1](p[2]) w[2](p[2]) w[3](p[2]) ... w[N](p[2]) )
      // ( w[1](p[3]) w[2](p[3]) w[3](p[3]) ... w[N](p[3]) )
      // (                      .                          )
      // (                      .                          )
      // (                      .                          )
      // ( w[1](p[N]) w[2](p[N]) w[3](p[N]) ... w[N](p[N]) )

      vprDEBUG(vprDBG_ALL, vprDBG_DETAILED_LVL)
         << "[PositionCalibrationFilter::config()] The W matrix is " 
         << mTable.size() << "x" << mTable.size() << ".\n" 
         << vprDEBUG_FLUSH;
      
      double r_squared(0.4f);
      double r(0.63245553203367588f);
      mWMatrix = new double*[mTable.size()];
      for (unsigned int i = 0; i < mTable.size(); ++i)
      {
         mWMatrix[i] = new double[mTable.size()];
         for (unsigned int j = 0; j < mTable.size(); ++j)
         {
            if (i == j)
            {
               // Shortcut; the diagonal will always be equal to R.
               mWMatrix[i][j] = r;
            }
            else
            {
               // XXX: This is a workaround for GMTL CVS Head which supports 
               //      template meta-programming for vectors; unfortunately, 
               //      gmtl::length(v1 - v2) will not compile since the types 
               //      are NOT gmtl::Vec.
               gmtl::Vec3d difference = mTable[i].second - mTable[j].second;
               //double length = gmtl::length( difference );
               mWMatrix[i][j] = gmtl::Math::sqrt( 
                                gmtl::dot(difference, difference) + 
                                r_squared );
            }
            vprDEBUG(vprDBG_ALL, vprDBG_HEX_LVL)
               << "[PositionCalibrationFilter::config()] Assigning " 
               << mWMatrix[i][j]
               << " to mWMatrix( " << i << ", " << j << ").\n"
               << vprDEBUG_FLUSH;
         }
      }

      return true;  
   }

   void
   PositionCalibrationFilter::apply(std::vector< PositionData >& posSample)
   {
      vprDEBUG(vprDBG_ALL, vprDBG_DETAILED_LVL)
         << "[PositionCalibrationFilter::apply()] Received " << posSample.size()
         << " samples.\n"
         << vprDEBUG_FLUSH;
      std::vector< PositionData >::iterator itr;
      for (itr = posSample.begin(); itr != posSample.end(); ++itr)
      {
         // Prepare the position matrix for Hardy's multi-quadric method.
         // 
         // Right now, we only calibrate the position, not the orientation, so
         // first, we need to pull out the translation matrix of posSample.
         // We do this using the standard method:  
         // pull the rotation matrix R out
         // of the transformation matrix Tr and multiply R-inverse by Tr, ie,
         // inverse(R) * Tr = T
         // and since rotation matrices are orthogonal,
         // inverse(R) = transpose(R), so...
         // transpose(R) * Tr = T
         gmtl::Matrix44f rotation(itr->getPosition());
         rotation[0][3] = 0;
         rotation[1][3] = 0;
         rotation[2][3] = 0;
         rotation[3][3] = 1;
         
         gmtl::Matrix44f translation = gmtl::transpose(rotation) * itr->getPosition();
         vprDEBUG(vprDBG_ALL, vprDBG_VERB_LVL)
            << "[PositionCalibrationFilter::apply()] Received tracked "
            << "position\n" << translation << "\n"
            << vprDEBUG_FLUSH;
         gmtl::Vec3d tracked_pos( translation[0][3], 
                                  translation[1][3], 
                                  translation[2][3] );

         // Now, we apply the following to the tracked position:
         // real_pos = alpha[j] * w[j](tracked_pos) + ... + alpha[N] * 
         // w[N](tracked_pos)
         // where w[j](p) = sqrt( length( p - p[j] )^2 + R^2 )
         // where 10 <= R^2 <= 1000.
         gmtl::Vec3d real_pos(0.0f, 0.0f, 0.0f);
         double r_squared = 40.0f;
         vprDEBUG(vprDBG_ALL, vprDBG_DETAILED_LVL)
            << "[PositionCalibrationFilter::apply()] Summing real position...\n"
            << vprDEBUG_FLUSH;
         for (unsigned int i = 0; i < mTable.size(); ++i)
         {
            // XXX: This is a workaround for GMTL CVS Head which supports 
            //      template meta-programming for vectors; unfortunately, 
            //      gmtl::length(v1 - v2) will not compile since the types are 
            //      NOT gmtl::Vec.
            gmtl::Vec3d difference = tracked_pos - mTable[i].second;
            //double length = gmtl::length(difference);
            real_pos += mAlphaVec[i] * 
                       gmtl::Math::sqrt( gmtl::dot(difference, difference) + 
                                         r_squared ); 
            vprDEBUG(vprDBG_ALL, vprDBG_DETAILED_LVL)
               << "[PositionCalibrationFilter::apply()] real_pos: "
               << real_pos << "\n" << vprDEBUG_FLUSH;
         }

         
         // Now we clobber the old transformation and replace it with a 
         // translation to our real position.
         gmtl::Matrix44f new_translation;
         new_translation[0][3] = static_cast<float>(real_pos[0]);
         new_translation[1][3] = static_cast<float>(real_pos[1]);
         new_translation[2][3] = static_cast<float>(real_pos[2]);
         
         vprDEBUG(vprDBG_ALL, vprDBG_VERB_LVL)
            << "[PositionCalibrationFilter::apply()] Replaced " 
            << tracked_pos << " with \n"
            << real_pos << "\n"
            << vprDEBUG_FLUSH;
         vprDEBUG(vprDBG_ALL, vprDBG_VERB_LVL)
            << "[PositionCalibrationFilter::apply()] Replaced \n" << translation
            << "\nwith " << new_translation
            << vprDEBUG_FLUSH;

         // Rebuild the position sample (transformation matrix).
         itr->setPosition(rotation * new_translation);
      }
   }
}
