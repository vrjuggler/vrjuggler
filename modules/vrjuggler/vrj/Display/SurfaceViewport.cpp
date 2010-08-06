/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2010 by Iowa State University
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
#include <gmtl/Vec.h>
#include <gmtl/Matrix.h>
#include <gmtl/MatrixOps.h>
#include <gmtl/Generate.h>
#include <gmtl/Xforms.h>

#include <jccl/Config/ConfigElement.h>
#include <gadget/Type/Position/PositionUnitConversion.h>

#include <vrj/Kernel/User.h>
#include <vrj/Util/Debug.h>
#include <vrj/Display/SurfaceProjection.h>
#include <vrj/Display/TrackedSurfaceProjection.h>
#include <vrj/Display/DisplayExceptions.h>
#include <vrj/Display/SurfaceViewport.h>
#include <vrj/Display/Display.h>

namespace vrj
{

SurfaceViewport::SurfaceViewport()
   : Viewport()
   , mTracked(false)
{
   /* Do nothing. */ ;
}

ViewportPtr SurfaceViewport::create()
{
   return ViewportPtr(new SurfaceViewport());
}

SurfaceViewport::~SurfaceViewport()
{
   /* Do nothing. */ ;
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
   
   mCornerUpdateMode = static_cast<CornerUpdateMode>(element->getProperty<int>("auto_corner_update", 0));

   // Calculate the rotation and the pts
//   calculateSurfaceRotation();
//   calculateCornersInBaseFrame();

   // Get info about being tracked
   mTracked = element->getProperty<bool>("tracked");
   if(mTracked)
   {
      mTrackerProxyName = element->getProperty<std::string>("tracker_proxy");
   }

   // Create Projection objects
   // NOTE: The -'s are because we are measuring distance to
   //  the left(bottom) which is opposite the normal axis direction
   //vjMatrix rot_inv;
   //rot_inv.invert(mSurfaceRotation);
   SurfaceProjectionPtr left_proj;
   SurfaceProjectionPtr right_proj;

   if(!mTracked)
   {
      left_proj = SurfaceProjection::create(mLLCorner, mLRCorner, mURCorner,
                                            mULCorner);
      right_proj = SurfaceProjection::create(mLLCorner, mLRCorner, mURCorner,
                                             mULCorner);
   }
   else
   {
      left_proj = TrackedSurfaceProjection::create(mLLCorner, mLRCorner,
                                                   mURCorner, mULCorner,
                                                   mTrackerProxyName);
      right_proj = TrackedSurfaceProjection::create(mLLCorner, mLRCorner,
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
      mLeftProj->setViewport(shared_from_this());

      mRightProj->config(element);
      mRightProj->setEye(Projection::RIGHT);
      mRightProj->setViewport(shared_from_this());
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

      result = false;
   }

   return result;
}

void SurfaceViewport::updateProjections(const float positionScale)
{
   // -- Calculate Eye Positions -- //
   const gmtl::Matrix44f cur_head_pos(
      mUser->getHeadPosProxy()->getData(positionScale)
   );

   // Compute location of left and right eyes
   float interocular_dist = mUser->getInterocularDistance();
   interocular_dist *= positionScale;              // Scale eye separation

   // Distance to move eye.
   const float eye_offset(interocular_dist * 0.5f);

   // NOTE: Eye coord system is -z forward, x-right, y-up

   if (Viewport::LEFT_EYE == mView || Viewport::STEREO == mView)
   {
      const gmtl::Point3f left_eye_pos(
         cur_head_pos * gmtl::Point3f(-eye_offset, 0, 0)
      );
      mLeftProj->calcViewMatrix(left_eye_pos, positionScale);
   }

   if (Viewport::RIGHT_EYE == mView || Viewport::STEREO == mView)
   {
      const gmtl::Point3f right_eye_pos(
         cur_head_pos * gmtl::Point3f(eye_offset, 0, 0)
      );
      mRightProj->calcViewMatrix(right_eye_pos, positionScale);
   }
}

void SurfaceViewport::computePixelTransforms()
{
   // Update viewport location & dimensions in pixels
   computePixelOriginAndSize();
   
   // Get horizontal and vertical vectors using corner points
   mHDirection = mLRCorner - mLLCorner;
   mVDirection = mULCorner - mLLCorner;
  
   // Use width & height to compute scale
   mPixelsPerUnitX = mVpWidth / gmtl::length(mHDirection);
   mPixelsPerUnitY = mVpHeight / gmtl::length(mVDirection);
   
   // Normalize the direction vectors
   gmtl::normalize(mHDirection);
   gmtl::normalize(mVDirection);
   
   // Compute the 3Space location of the screen origin
   mIntercept = 
      mLLCorner - 
         mHDirection * ( mVpOriginX / mPixelsPerUnitX ) -
         mVDirection * ( mVpOriginY / mPixelsPerUnitY );
}

void SurfaceViewport::updateCorners()
{
   switch (mCornerUpdateMode)
   {
      case NO_UPDATE:
         return;
      case RESIZE_ONLY:
      {
         float vp_pixel_width, vp_pixel_height;

         // Save previous width & height for computing delta
         vp_pixel_width = mVpWidth;
         vp_pixel_height = mVpHeight;

         // Update viewport location & dimensions in pixels
         computePixelOriginAndSize();
      
         // Compute expansion amounts
         gmtl::Vec3f h_expand = 
            mHDirection * 0.5f * 
            (mVpWidth - vp_pixel_width) / mPixelsPerUnitX;
         gmtl::Vec3f v_expand = 
            mVDirection * 0.5f * 
            (mVpHeight - vp_pixel_height) / mPixelsPerUnitY;

         // Expand or contract about the center
         mLLCorner = mLLCorner - h_expand - v_expand;
         mLRCorner = mLRCorner + h_expand - v_expand;
         mULCorner = mULCorner - h_expand + v_expand;
         mURCorner = mURCorner + h_expand + v_expand;

         break;
      }
      case RESIZE_WIDTH_ONLY:
      {
         float vp_pixel_width, vp_pixel_height;

         // Save previous width & height for computing delta
         vp_pixel_width = mVpWidth;
         vp_pixel_height = mVpHeight;

         // Update viewport location & dimensions in pixels
         computePixelOriginAndSize();

         // Correct width for change in Height.
         vp_pixel_width = mVpHeight / vp_pixel_height * vp_pixel_width;
         gmtl::Vec3f vec = mLRCorner - mLLCorner;
         mPixelsPerUnitX = vp_pixel_width / gmtl::length(vec);

         // Compute expansion amounts
         gmtl::Vec3f h_expand =
            mHDirection * 0.5f *
            (mVpWidth - vp_pixel_width) / mPixelsPerUnitX;

         // Expand or contract about the center
         mLLCorner = mLLCorner - h_expand;
         mLRCorner = mLRCorner + h_expand;
         mULCorner = mULCorner - h_expand;
         mURCorner = mURCorner + h_expand;

         break;
      }
      case RESIZE_AND_MOVE:
         // Update viewport location & dimensions in pixels
         computePixelOriginAndSize();

         // Use slope/intercept to move the corners
         mLLCorner = mIntercept + 
                     mHDirection * (mVpOriginX / mPixelsPerUnitX) + 
                     mVDirection * (mVpOriginY / mPixelsPerUnitY);
         mLRCorner = mLLCorner + mHDirection * (mVpWidth / mPixelsPerUnitX);
         mULCorner = mLLCorner + mVDirection * (mVpHeight / mPixelsPerUnitY);
         mURCorner = mLRCorner + mVDirection * (mVpHeight / mPixelsPerUnitY);
         
         break;
      default:
        return;
   }

   // Apply this update to the projections
   SurfaceProjectionPtr lproj = 
     boost::dynamic_pointer_cast<SurfaceProjection>(mLeftProj);
   vprASSERT(NULL != lproj.get());
   if (NULL != lproj.get())
   {
      lproj->updateCorners(mLLCorner, mLRCorner, mURCorner, mULCorner);
   }

   SurfaceProjectionPtr rproj = 
     boost::dynamic_pointer_cast<SurfaceProjection>(mRightProj);
   vprASSERT(NULL != rproj.get());
   if (NULL != rproj.get())
   {
      rproj->updateCorners(mLLCorner, mLRCorner, mURCorner, mULCorner);
   }
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

void SurfaceViewport::computePixelOriginAndSize()
{
   // Get window location & dimensions
   int win_origin_x, win_origin_y, win_width, win_height;
   getDisplay()->getOriginAndSize(win_origin_x, win_origin_y,
                                  win_width, win_height);

   // Compute viewport location & dimensions in pixels
   mVpOriginX = static_cast<float>(win_origin_x) + 
     mXorigin * static_cast<float>(win_width);
   mVpOriginY = static_cast<float>(win_origin_y) +
     mYorigin * static_cast<float>(win_height);
   mVpWidth = static_cast<float>(win_width) * mXsize;
   mVpHeight = static_cast<float>(win_height) * mYsize;
}

}
