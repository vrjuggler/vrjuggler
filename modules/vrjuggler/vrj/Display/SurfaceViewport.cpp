/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2007 by Iowa State University
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
 *************** <auto-copyright.pl END do not edit this line> ***************/

#include <vrj/vrjConfig.h>

#include <string>
#include <gmtl/Math.h>
//#include <vrj/Math/Coord.h>
#include <gmtl/Vec.h>
#include <jccl/Config/ConfigElement.h>
#include <vrj/Kernel/User.h>

#include <gadget/Type/Position/PositionUnitConversion.h>

#include <gmtl/Matrix.h>
#include <gmtl/MatrixOps.h>
#include <gmtl/Generate.h>
#include <gmtl/Xforms.h>

#include <vrj/Display/SurfaceProjection.h>
#include <vrj/Display/TrackedSurfaceProjection.h>
#include <vrj/Display/DisplayExceptions.h>
#include <vrj/Display/SurfaceViewport.h>

namespace vrj
{

SurfaceViewport::SurfaceViewport()
   : Viewport()
   , mTracked(false)
{
   /* Do nothing. */ ;
}

SurfaceViewport::~SurfaceViewport()
{
   if ( NULL != mLeftProj )
   {
      delete mLeftProj;
      mLeftProj = NULL;
   }

   if ( NULL != mRightProj )
   {
      delete mRightProj;
      mRightProj = NULL;
   }
}

bool SurfaceViewport::config(jccl::ConfigElementPtr element)
{
   vprASSERT(element.get() != NULL);
   vprASSERT(element->getID() == "surface_viewport");

   // Call base class config
   if ( ! Viewport::config(element) )
   {
      return false;
   }

   bool result(true);

   mType = SURFACE;

   // Read in the corners
   mLLCorner.set(element->getProperty<float>("lower_left_corner", 0),
                 element->getProperty<float>("lower_left_corner", 1),
                 element->getProperty<float>("lower_left_corner", 2));
   mLRCorner.set(element->getProperty<float>("lower_right_corner", 0),
                 element->getProperty<float>("lower_right_corner", 1),
                 element->getProperty<float>("lower_right_corner", 2));
   mURCorner.set(element->getProperty<float>("upper_right_corner", 0),
                 element->getProperty<float>("upper_right_corner", 1),
                 element->getProperty<float>("upper_right_corner", 2));
   mULCorner.set(element->getProperty<float>("upper_left_corner", 0),
                 element->getProperty<float>("upper_left_corner", 1),
                 element->getProperty<float>("upper_left_corner", 2));

   // Calculate the rotation and the pts
//   calculateSurfaceRotation();
//   calculateCornersInBaseFrame();

   // Get info about being tracked
   mTracked = element->getProperty<bool>("tracked");
   if(mTracked)
   {
      mTrackerProxyName = element->getProperty<std::string>("tracker_proxy");
   }

   if ( NULL != mLeftProj )
   {
      delete mLeftProj;
   }

   if ( NULL != mRightProj )
   {
      delete mRightProj;
   }

   // Create Projection objects
   // NOTE: The -'s are because we are measuring distance to
   //  the left(bottom) which is opposite the normal axis direction
   //vjMatrix rot_inv;
   //rot_inv.invert(mSurfaceRotation);
   SurfaceProjection* left_proj(NULL);
   SurfaceProjection* right_proj(NULL);

   if(!mTracked)
   {
      left_proj = new SurfaceProjection(mLLCorner, mLRCorner, mURCorner,
                                        mULCorner);
      right_proj = new SurfaceProjection(mLLCorner, mLRCorner, mURCorner,
                                         mULCorner);
   }
   else
   {
      left_proj = new TrackedSurfaceProjection(mLLCorner, mLRCorner,
                                               mURCorner, mULCorner,
                                               mTrackerProxyName);
      right_proj = new TrackedSurfaceProjection(mLLCorner, mLRCorner,
                                                mURCorner, mULCorner,
                                                mTrackerProxyName);
   }

   try
   {
      left_proj->validateCorners();
      right_proj->validateCorners();

      // NOTE: Even if the corner validation above failed, we still proceed with
      // setting up mLeftProj and mRightProj. This is because other code is not
      // written to handle the case of a viewport having no projections. This
      // could definitely be improved.
      mLeftProj  = left_proj;
      mRightProj = right_proj;

      // Configure the projections
      mLeftProj->config(element);
      mLeftProj->setEye(Projection::LEFT);
      mLeftProj->setViewport(this);

      mRightProj->config(element);
      mRightProj->setEye(Projection::RIGHT);
      mRightProj->setViewport(this);
   }
   catch (InvalidSurfaceException& ex)
   {
      vprDEBUG(vrjDBG_DISP_MGR, vprDBG_CRITICAL_LVL)
         << clrOutBOLD(clrRED, "ERROR")
         << ": The surface defined by the viewport named\n" << vprDEBUG_FLUSH;
      vprDEBUG_NEXT(vrjDBG_DISP_MGR, vprDBG_CRITICAL_LVL)
         << "       '" << element->getName() << "' is invalid!\n"
         << vprDEBUG_FLUSH;
      vprDEBUG_NEXT(vrjDBG_DISP_MGR, vprDBG_CRITICAL_LVL)
         << ex.what() << std::endl << vprDEBUG_FLUSH;

      delete left_proj;
      delete right_proj;

      result = false;
   }

   return result;
}

void SurfaceViewport::updateProjections(const float positionScale)
{
   gmtl::Matrix44f left_eye_pos, right_eye_pos;     // NOTE: Eye coord system is -z forward, x-right, y-up

   // -- Calculate Eye Positions -- //
   gmtl::Matrix44f cur_head_pos = mUser->getHeadPosProxy()->getData(positionScale);
   /*
   Coord  head_coord(cur_head_pos);       // Create a user readable version

   vprDEBUG(vprDBG_ALL, vprDBG_HVERB_LVL)
      << "vjDisplay::updateProjections: Getting head position" << std::endl
      << vprDEBUG_FLUSH;
   vprDEBUG(vprDBG_ALL, vprDBG_HVERB_LVL) << "\tHeadPos:" << head_coord.pos << "\tHeadOr:"
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
   if ( mView == vrj::Viewport::LEFT_EYE || mView == vrj::Viewport::STEREO )
   {
      out << indent_text << "Left projection:\n";
      mLeftProj->outStream(out, indentLevel + 2);
      out << std::endl;
   }
   if ( mView == vrj::Viewport::RIGHT_EYE || mView == vrj::Viewport::STEREO )
   {
      out << indent_text << "Right projection:\n";
      mRightProj->outStream(out, indentLevel + 2);
      out << std::endl;
   }

   return out;
}

}
