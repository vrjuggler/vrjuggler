/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2003 by Iowa State University
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

#include <vrj/vrjConfig.h>

#include <string>
#include <gmtl/Math.h>
//#include <vrj/Math/Coord.h>
#include <gmtl/Vec.h>
#include <jccl/Config/ConfigChunk.h>
#include <vrj/Kernel/User.h>

#include <gadget/Type/Position/PositionUnitConversion.h>

#include <gmtl/Matrix.h>
#include <gmtl/MatrixOps.h>
#include <gmtl/Generate.h>
#include <gmtl/Xforms.h>

#include <vrj/Display/SurfaceProjection.h>
#include <vrj/Display/TrackedSurfaceProjection.h>
#include <vrj/Display/SurfaceViewport.h>

namespace vrj
{

void SurfaceViewport::config(jccl::ConfigChunkPtr chunk)
{
   vprASSERT(chunk.get() != NULL);
   vprASSERT(chunk->getDescToken() == std::string("surfaceViewport"));

   Viewport::config(chunk);     // Call base class config

   mType = SURFACE;

   // Read in the corners
   jccl::ConfigChunkPtr ll_corner_chunk = chunk->getProperty<jccl::ConfigChunkPtr>("corners",0);
   jccl::ConfigChunkPtr lr_corner_chunk = chunk->getProperty<jccl::ConfigChunkPtr>("corners",1);
   jccl::ConfigChunkPtr ur_corner_chunk = chunk->getProperty<jccl::ConfigChunkPtr>("corners",2);
   jccl::ConfigChunkPtr ul_corner_chunk = chunk->getProperty<jccl::ConfigChunkPtr>("corners",3);
   mLLCorner.set(ll_corner_chunk->getProperty<float>("x"),
                 ll_corner_chunk->getProperty<float>("y"),
                 ll_corner_chunk->getProperty<float>("z"));
   mLRCorner.set(lr_corner_chunk->getProperty<float>("x"),
                 lr_corner_chunk->getProperty<float>("y"),
                 lr_corner_chunk->getProperty<float>("z"));
   mURCorner.set(ur_corner_chunk->getProperty<float>("x"),
                 ur_corner_chunk->getProperty<float>("y"),
                 ur_corner_chunk->getProperty<float>("z"));
   mULCorner.set(ul_corner_chunk->getProperty<float>("x"),
                 ul_corner_chunk->getProperty<float>("y"),
                 ul_corner_chunk->getProperty<float>("z"));

   // Calculate the rotation and the pts
   calculateSurfaceRotation();
   calculateCornersInBaseFrame();

   // Get info about being tracked
   mTracked = chunk->getProperty<bool>("tracked");
   if(mTracked)
   {
      mTrackerProxyName = chunk->getProperty<std::string>("trackerproxy");
   }

   // Create Projection objects
   // NOTE: The -'s are because we are measuring distance to
   //  the left(bottom) which is opposite the normal axis direction
   //vjMatrix rot_inv;
   //rot_inv.invert(mSurfaceRotation);
   if(!mTracked)
   {
      mLeftProj = new SurfaceProjection(mSurfaceRotation,-mxLLCorner[gmtl::Zelt],
                                    mxLRCorner[gmtl::Xelt],-mxLLCorner[gmtl::Xelt],
                                    mxURCorner[gmtl::Yelt],-mxLRCorner[gmtl::Yelt]);
      mRightProj = new SurfaceProjection(mSurfaceRotation,-mxLLCorner[gmtl::Zelt],
                                    mxLRCorner[gmtl::Xelt],-mxLLCorner[gmtl::Xelt],
                                    mxURCorner[gmtl::Yelt],-mxLRCorner[gmtl::Yelt]);
   }
   else
   {
      mLeftProj = new TrackedSurfaceProjection(mSurfaceRotation,-mxLLCorner[gmtl::Zelt],
                                    mxLRCorner[gmtl::Xelt],-mxLLCorner[gmtl::Xelt],
                                    mxURCorner[gmtl::Yelt],-mxLRCorner[gmtl::Yelt],
                                              mTrackerProxyName);
      mRightProj = new TrackedSurfaceProjection(mSurfaceRotation,-mxLLCorner[gmtl::Zelt],
                                    mxLRCorner[gmtl::Xelt],-mxLLCorner[gmtl::Xelt],
                                    mxURCorner[gmtl::Yelt],-mxLRCorner[gmtl::Yelt],
                                               mTrackerProxyName);
   }
   // Configure the projections
   mLeftProj->config(chunk);
   mLeftProj->setEye(Projection::LEFT);
   mLeftProj->setViewport(this);

   mRightProj->config(chunk);
   mRightProj->setEye(Projection::RIGHT);
   mRightProj->setViewport(this);
}

void SurfaceViewport::updateProjections(const float positionScale)
{
   gmtl::Matrix44f left_eye_pos, right_eye_pos;     // NOTE: Eye coord system is -z forward, x-right, y-up

   // -- Calculate Eye Positions -- //
   gmtl::Matrix44f cur_head_pos = mUser->getHeadPosProxy()->getData(positionScale);
   /*
   Coord  head_coord(cur_head_pos);       // Create a user readable version

   vprDEBUG(vprDBG_ALL,5)
      << "vjDisplay::updateProjections: Getting head position" << std::endl
      << vprDEBUG_FLUSH;
   vprDEBUG(vprDBG_ALL,5) << "\tHeadPos:" << head_coord.pos << "\tHeadOr:"
                        << head_coord.orient << std::endl << vprDEBUG_FLUSH;
                        */

   // Compute location of left and right eyes
   //float interocularDist = 2.75f/12.0f;
   float interocular_dist = mUser->getInterocularDistance();
   interocular_dist *= positionScale;              // Scale eye separation
   float eye_offset = interocular_dist/2.0f;      // Distance to move eye

   left_eye_pos = cur_head_pos * gmtl::makeTrans<gmtl::Matrix44f>(gmtl::Vec3f( -eye_offset, 0, 0));
   right_eye_pos = cur_head_pos * gmtl::makeTrans<gmtl::Matrix44f>(gmtl::Vec3f(eye_offset, 0, 0));

   mLeftProj->calcViewMatrix(left_eye_pos, positionScale);
   mRightProj->calcViewMatrix(right_eye_pos, positionScale);
}

std::ostream& SurfaceViewport::outStream(std::ostream& out,
                                         const unsigned int indentLevel)
{
   Viewport::outStream(out, indentLevel);
   out << std::endl;

   const std::string indent_text(indentLevel, ' ');

   /*
   out << "LL: " << mLLCorner << ", LR: " << mLRCorner
       << ", UR: " << mURCorner << ", UL:" << mULCorner << std::endl;
   out << "surfRot: \n" << mSurfaceRotation << std::endl;
   */
   out << indent_text << "Left projection:\n";
   mLeftProj->outStream(out, indentLevel + 2);
   out << std::endl;
   out << indent_text << "Right projection:\n";
   mRightProj->outStream(out, indentLevel + 2);

   return out;
}

void SurfaceViewport::calculateSurfaceRotation()
{
   assertPtsLegal();

   // Find the base vectors for the surface axiis (in terms of the base coord system)
   // With z out, x to the right, and y up
   gmtl::Vec3f x_base, y_base, z_base;
   x_base = (mLRCorner-mLLCorner);
   y_base = (mURCorner-mLRCorner);
   gmtl::cross( z_base, x_base, y_base);

   // They must be normalized
   gmtl::normalize(x_base); gmtl::normalize(y_base); gmtl::normalize(z_base);

   // Calculate the surfaceRotMat using law of cosines
   mSurfaceRotation = gmtl::makeDirCos<gmtl::Matrix44f>(x_base, y_base, z_base );
}

void SurfaceViewport::calculateCornersInBaseFrame()
{
   // Convert the coordinates over to surface coordinate system
   gmtl::Matrix44f surf_M_base;
   gmtl::invert(surf_M_base, mSurfaceRotation);

   mxLLCorner = surf_M_base * mLLCorner;
   mxLRCorner = surf_M_base * mLRCorner;
   mxURCorner = surf_M_base * mURCorner;
   mxULCorner = surf_M_base * mULCorner;

   // Verify that they are all in the same x,y plane
   vprDEBUG(vprDBG_ALL, vprDBG_HVERB_LVL) << std::setprecision(10)
                                          << mxLLCorner[gmtl::Zelt] << " "
                                          << mxLRCorner[gmtl::Zelt] << " "
                                          << mxURCorner[gmtl::Zelt] << " "
                                          << mxULCorner[gmtl::Zelt] << "\n"
                                          << vprDEBUG_FLUSH;
#ifdef VJ_DEBUG
   const float epsilon = 1e-6;
   vprASSERT(gmtl::Math::isEqual(mxLLCorner[gmtl::Zelt], mxLRCorner[gmtl::Zelt], epsilon) &&
             gmtl::Math::isEqual(mxURCorner[gmtl::Zelt], mxULCorner[gmtl::Zelt], epsilon) &&
             gmtl::Math::isEqual(mxLLCorner[gmtl::Zelt], mxULCorner[gmtl::Zelt], epsilon));
#endif
}

}
