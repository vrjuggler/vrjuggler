/*********** <VRJ VNC auto-copyright.pl BEGIN do not edit this line> **********
 *
 * VRJ VNC is (C) Copyright 2003-2005 by Iowa State University
 *
 * Original Authors:
 *   Patrick Hartling, Allen Bierbaum
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU General Public
 * License along with this application; if not, write to the Free
 * Software Foundation, Inc., 59 Temple Place - Suite 330, Boston,
 * MA 02111-1307, USA.
 *
 * -----------------------------------------------------------------
 * File:          $RCSfile$
 * Date modified: $Date$
 * Version:       $Revision$
 * -----------------------------------------------------------------
 *
 ************ <VRJ VNC auto-copyright.pl END do not edit this line> **********/

#include <stdlib.h>
#include <string.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <X11/Xmd.h>
#include <rfbproto.h>
#include <gmtl/Point.h>
#include <gmtl/Vec.h>
#include <gmtl/Ray.h>
#include <gmtl/Generate.h>
#include <gmtl/Containment.h>
#include <gmtl/Intersection.h>
#include <gmtl/Output.h>

#include <Rectangle.h>
#include <VNCDebug.h>
#include <VNCDesktop.h>


namespace vrjvnc
{

static int getNearestMultipleOfTwo(int value)
{
    int bits;
    value--;
    for (bits = 0; value; bits++, value >>= 1);
    return (1 << bits);
}

VNCDesktop::VNCDesktop(const std::string& hostname, const vpr::Uint16& port,
                       const std::string& password,
                       const float& desktopSideLength)
   : mVncIf(hostname, port, password)
   , mVncThreadFunctor(NULL)
   , mVncThread(NULL)
   , mHaveKeyboard(false)
   , mDesktopWandIsect(false)
   , mDesktopGrabbed(false)
   , mTextureData(NULL)
{
   mSelectState = Nothing;
   mActiveState = Normal;
   mHasRectUpdate = false;

   mVncWidth = mVncIf.getWidth();                                 // Get real size of the desktop
   mVncHeight = mVncIf.getHeight();

   mTexWidth  = getNearestMultipleOfTwo(mVncIf.getWidth());       // Create a texture of multiple
   mTexHeight = getNearestMultipleOfTwo(mVncIf.getHeight());      // of two sized

   mMaxTexCoordX = float(mVncWidth) / float(mTexWidth);           // Compute the tex coords for the desktop
   mMaxTexCoordY = float(mVncHeight) / float(mTexHeight);

   // Set initial desktop size based on aspect ratio of the vnc desktop
   float aspect_ratio = float(mVncWidth) / float(mVncHeight);
   mDesktopWidth = desktopSideLength;
   mDesktopHeight = desktopSideLength/aspect_ratio;

   vprASSERT((mVncWidth <= mTexWidth) && (mVncHeight <= mTexHeight));   // Make sure tex is large enough

   mMaxSize = mDesktopWidth * 1.75f;
   mMinSize = mDesktopWidth * 0.50f;
   mIncSize = 0.02f;

   std::cout << "max size: " << mMaxSize << std::endl;
   std::cout << "min size: " << mMinSize << std::endl;

   updateDesktopParameters();       // Initial update of desktop parameters

   // Allocate texture data
   vprASSERT( NULL == mTextureData );
   const int bytes_per_pixel(mVncIf.getPixelSize() / 8);
   const size_t tex_size(mTexWidth*mTexHeight*bytes_per_pixel);
   mTextureData = (char*) malloc(tex_size);
   memset(mTextureData, 0, tex_size);     // Zero out the texture data.

   // Set initial transform
   gmtl::setTrans( m_vworld_M_desktop, gmtl::Vec3f(-3.0f, -1.0f, -4.0f));
   m_vworld_M_desktop *= gmtl::makeRot<gmtl::Matrix44f>(gmtl::EulerAngleXYZf(-0.4f, -0.05f, 0.0f));

   // Request the first update.
   mVncIf.sendFramebufferUpdateRequest(0, 0, mVncIf.getWidth(), mVncIf.getHeight());

   // Spawn off a thread to keep updating the data vnc connection
   mVncThreadFunctor = new vpr::ThreadRunFunctor<VNCInterface>(&mVncIf);
   mVncThread        = new vpr::Thread(mVncThreadFunctor);
   vprASSERT(mVncThread->valid() && "VNC sample thread failed to start");
   // And we're off!
}

VNCDesktop::~VNCDesktop()
{
   if ( NULL != mVncThreadFunctor )
   {
      mVncIf.stop();
//      mVncThread->join();
      delete mVncThreadFunctor;
      delete mVncThread;
   }

   if ( NULL != mTextureData )
   {
      free(mTextureData);
   }
}

void VNCDesktop::init(const std::string& wandName,
                      const std::string& leftButtonName,
                      const std::string& middleButtonName,
                      const std::string& rightButtonName,
                      const std::string& keyboardName)
{
   mWand.init(wandName);
   mLeftButton.init(leftButtonName);
   mMiddleButton.init(middleButtonName);
   mRightButton.init(rightButtonName);

   if ( keyboardName != std::string("") )
   {
      mKeyboard.init(keyboardName);
      mHaveKeyboard = true;
   }

   // Allocate a new quadric that will be used to render the sphere.
   mSphereQuad = gluNewQuadric();

   mCylinderQuad = gluNewQuadric();
   gluQuadricDrawStyle(mCylinderQuad,GLU_FILL);
   gluQuadricNormals(mCylinderQuad,GLU_SMOOTH);
   gluQuadricTexture(mCylinderQuad,GL_FALSE);
}

/** Updates the desktop parameters
* @pre mDesktopWidth, mDesktopHeight, and pos matrix are update
* @post Everything else is updated correspondingly
*/
void VNCDesktop::updateDesktopParameters()
{
   vprDEBUG(vrjDBG_VNC, vprDBG_STATE_LVL)
       << "VNCDesktop::updateDesktopParams() entered\n" << vprDEBUG_FLUSH;

   const float BorderSize(0.50f);
   const float BorderDepth(BorderSize/2.0f);
   const float CornerSize(BorderSize+0.2f);
   const float CornderDepth(CornerSize/2.0f);

   // --- Update scales and bounds. --- //
   mDesktopToVncWidthScale  = mVncWidth / mDesktopWidth;
   mDesktopToVncHeightScale = mVncHeight / mDesktopHeight;

   // Configure the desktop bounding box
   mDesktopBox.setMin(gmtl::Point3f(0, 0, -0.50f));
   mDesktopBox.setMax(gmtl::Point3f(mDesktopWidth, mDesktopHeight, 0.50f));
   mDesktopBox.setEmpty(false);

   float far_left = -CornerSize;
   float far_right = mDesktopWidth+CornerSize;
   float far_bottom = -CornerSize;
   float far_top = mDesktopHeight+CornerSize;

   // --- Update corners --- //
   // LL
   mLLCorner.setMin(gmtl::Point3f(far_left, far_bottom,-CornderDepth));
   mLLCorner.setMax(gmtl::Point3f(0.0f, 0.0f, CornderDepth));
   mLLCorner.setEmpty(false);
   // LR
   mLRCorner.setMin(gmtl::Point3f(mDesktopWidth, far_bottom,-CornderDepth));
   mLRCorner.setMax(gmtl::Point3f(far_right, 0.0f, CornderDepth));
   mLRCorner.setEmpty(false);
   // UR
   mURCorner.setMin(gmtl::Point3f(mDesktopWidth, mDesktopHeight, -CornderDepth));
   mURCorner.setMax(gmtl::Point3f(far_right, far_top, CornderDepth));
   mURCorner.setEmpty(false);
   // UL
   mULCorner.setMin(gmtl::Point3f(far_left, mDesktopHeight,-CornderDepth));
   mULCorner.setMax(gmtl::Point3f(0.0f, far_top, CornderDepth));
   mULCorner.setEmpty(false);

   far_left = -BorderSize;
   far_right = mDesktopWidth+BorderSize;
   far_bottom = -BorderSize;
   far_top = mDesktopHeight+BorderSize;

   // Update borders -- Goto the far's so they puncture the corners
   mLeftBorder.setMin(gmtl::Point3f(far_left,far_bottom,-BorderDepth));
   mLeftBorder.setMax(gmtl::Point3f(0.0f,far_top,BorderDepth));
   mLeftBorder.setEmpty(false);
   mRightBorder.setMin(gmtl::Point3f(mDesktopWidth,far_bottom,-BorderDepth));
   mRightBorder.setMax(gmtl::Point3f(far_right,far_top,BorderDepth));
   mRightBorder.setEmpty(false);
   mBottomBorder.setMin(gmtl::Point3f(far_left,far_bottom,-BorderDepth));
   mBottomBorder.setMax(gmtl::Point3f(far_right,0.0f,BorderDepth));
   mBottomBorder.setEmpty(false);
   mTopBorder.setMin(gmtl::Point3f(far_left,mDesktopHeight,-BorderDepth));
   mTopBorder.setMax(gmtl::Point3f(far_right,far_top,BorderDepth));
   mTopBorder.setEmpty(false);

   // Set the translation point to be the middle of the desktop polygon.
   //gmtl::setTrans(mDesktopMatrix, gmtl::Vec3f(0.0f, 5.0f, -5.0f));

   vprDEBUG(vrjDBG_VNC, vprDBG_STATE_LVL)
       << "VNCDesktop::updateDesktopParams() exited\n" << vprDEBUG_FLUSH;
}

VNCDesktop::Focus VNCDesktop::update(const gmtl::Matrix44f& navMatrix)
{
   vpr::DebugOutputGuard guard(vrjDBG_VNC, vprDBG_STATE_LVL,
                               "VNCDesktop::update()\n",
                               "VNCDesktop::update() done.\n");

   // --------- UPDATE NAV AND DESKTOP MATRICES ----------------------- //
   // Do all intersection testing and stuff in the local coordinate frame so
   // we don't have to deal with rotations and translations of the desktop.
   // Just transform the wand into the local frame and we are set to go with
   // minimal effort.

   // Get the point position of the wand and construct a ray that shoots out
   // of the wand.
   const float max_ray_length(100.0f);

   // Compute the wand xform in the local desktop coordinate frame
   //   wand_vnc = vnc_M_vw * vw_M_world
   gmtl::Matrix44f desktop_M_world;       // inv(wMvw*vwMvnc) -- nav*desktop
   const gmtl::Matrix44f wand_mat_world(mWand->getData());
   gmtl::Matrix44f world_M_desktop = navMatrix*m_vworld_M_desktop;
   gmtl::invertFull_orig(desktop_M_world, world_M_desktop);
   gmtl::Matrix44f wand_mat = desktop_M_world * wand_mat_world;

   const gmtl::Point3f wand_point(gmtl::makeTrans<gmtl::Point3f>(wand_mat));
   const gmtl::Vec3f ray_vector(0.0f, 0.0f, -max_ray_length);

   // For now set it long.  Clip later.
   mWandRay.setOrigin(gmtl::makeTrans<gmtl::Point3f>(wand_mat));
   mWandRay.setDir(wand_mat*ray_vector);

   // Find ray intersection on the z=0 plane
   const gmtl::Vec3f pos_z_norm(0.0f, 0.0f, 1.0f);
   const gmtl::Point3f origin_point(0.0f, 0.0f, 0.0f);
   const gmtl::Planef z_plane(pos_z_norm, origin_point);

   // Find the point on the desktop plane that the ray intersects it
   // this is our intersection in desktop coordinates (polygon).
   // It will have to be scaled to get back into vnc coords.
   float t_isect;
   bool ray_intersects(false);
   ray_intersects = gmtl::intersect(z_plane, mWandRay, t_isect);

   if ( ray_intersects )
   {
      mIsectPoint = (mWandRay.mOrigin + (mWandRay.mDir*t_isect));
      vprASSERT(gmtl::Math::isEqual(mIsectPoint[2], 0.0f, 0.01f) &&
                "Point should be on z=0 plane");

      // Compute drawing objects
      mWandRay.setDir(mWandRay.getDir()*t_isect);     // Scale it back

      vprDEBUG(vrjDBG_VNC, vprDBG_VERB_LVL)
         << "VNC: Isect point: " << mIsectPoint << std::endl << vprDEBUG_FLUSH;
   }

   // Get button states
   bool select_button_state = mLeftButton->getData();
   bool things_grabbed = (mSelectState > GrabBegin && mSelectState < GrabEnd);

   // Check for transition states
   // - If have some, then update them and early exit from this method
   const float roll_inc(0.05f);
   if ( RollingUp == mActiveState )
   {
      mRollUpPercent += roll_inc;
      if ( mRollUpPercent >= 1.0f )      // If finished rolling up
      {
         mActiveState = RolledUp;
         mDesktopHeight = 0.0f;
      }
      else
      {
         float delta_h = -(roll_inc*mOriginalHeight);
         mDesktopHeight = mOriginalHeight*(1.0f-mRollUpPercent);
         m_vworld_M_desktop *= gmtl::makeTrans<gmtl::Matrix44f>(gmtl::Vec3f(0.0f, -delta_h, 0.0f));
      }

      updateDesktopParameters();
   }
   else if ( RollingDown == mActiveState )
   {
      mRollUpPercent -= roll_inc;
      if ( mRollUpPercent <= 0.0f )    // Finished unrolling
      {
         mActiveState = Normal;
         mDesktopHeight = mOriginalHeight;
      }
      else
      {
         float delta_h = (roll_inc*mOriginalHeight);
         mDesktopHeight = mOriginalHeight*(1.0f-mRollUpPercent);
         m_vworld_M_desktop *= gmtl::makeTrans<gmtl::Matrix44f>(gmtl::Vec3f(0.0f, -delta_h, 0.0f));
      }
      updateDesktopParameters();
   }
   // --- CHECK SELECTIONS --- //
   // If nothing grabbed then check for input
   else if ( ! things_grabbed && ray_intersects )
   {
      // Check for selecting the main desktop box
      if ( gmtl::isInVolume(mDesktopBox, mIsectPoint) )
      {
         // Compute VNC button masks
         int button_mask(0);

         if ( mLeftButton->getData() )
         {
            button_mask |= rfbButton1Mask;
         }
         if ( mMiddleButton->getData())
         {
            button_mask |= rfbButton2Mask;
         }
         if ( mRightButton->getData() )
         {
            button_mask |= rfbButton3Mask;
         }

         // Translate that point into the coordinates VNC wants to see.
         //
         // vnc x,y desktop point just like x desktop.  origin upper left,
         // y increases going down
         //     The valid range is [0,mVncWidth or mVncHeight]
         // Scale
         float vnc_x = mIsectPoint[gmtl::Xelt] * mDesktopToVncWidthScale;
         // Flip and scale
         float vnc_y = -(mIsectPoint[gmtl::Yelt] - mDesktopHeight) *
                          mDesktopToVncHeightScale;

         mVncIf.pointerEvent(int(vnc_x), int(vnc_y), button_mask);

         if ( mHaveKeyboard )
         {
            // Handle keyboard input.
         }
         mSelectState = Desktop;
      }
      // ---- Check corner selection --- //
      else if ( gmtl::isInVolume(mURCorner, mIsectPoint) )
      {
         if ( select_button_state )
         {
            if ( mActiveState == Normal )
            {
               std::cout << "Rolling up\n";
               mActiveState = RollingUp;
               mRollUpPercent = 0.0f;
               mOriginalHeight = mDesktopHeight;
            }
            else if ( RolledUp == mActiveState )
            {
               std::cout << "Rolling down\n";
               mActiveState = RollingDown;
               mRollUpPercent = 1.0f;
            }
         }
         else     // Just select it
         {
            if ( URCornerSelect != mSelectState )
            {
               std:: cout << "State: URCornerSelect" << std::endl;
            }
            mSelectState = URCornerSelect;
         }
      }
      else if ( gmtl::isInVolume(mULCorner, mIsectPoint) )
      {
         if ( select_button_state )
         {
         }
         else     // Just select it
         {
            if ( ULCornerSelect != mSelectState )
            {
               std:: cout << "State: ULCornerSelect" << std::endl;
            }
            mSelectState = ULCornerSelect;
         }
      }
      else if ( gmtl::isInVolume(mLLCorner, mIsectPoint) )
      {
         if ( select_button_state )
         {
            mSelectState = LLCornerGrab;
            mCornerGrabPoint = mIsectPoint;
         }
         else     // Just select it
         {
            if ( LLCornerSelect != mSelectState )
            {
               std:: cout << "State: LLCornerSelect" << std::endl;
            }
            mSelectState = LLCornerSelect;
         }
      }
      else if ( gmtl::isInVolume(mLRCorner, mIsectPoint))
      {
         if ( select_button_state )
         {
            mSelectState = LRCornerGrab;        // Set to grab state
            mCornerGrabPoint = mIsectPoint;
         }
         else     // Just select it
         {
            if ( LRCornerSelect != mSelectState )
            {
               std:: cout << "State: LRCornerSelect" << std::endl;
            }
            mSelectState = LRCornerSelect;
         }
      }
      // ---- Check border selection --- //
      else if ( gmtl::isInVolume(mTopBorder, mIsectPoint) )
      {
         if ( select_button_state )
         {
            mSelectState = TopBorderGrab;
            m_wandMdesktop_grab = wand_mat;
            gmtl::invertFull_orig(m_wandMdesktop_grab, m_wandMdesktop_grab);
         }
         else     // Just select it
         {
            if ( TopBorderSelect != mSelectState )
            {
               std:: cout << "State: TopBorderSelect" << std::endl;
            }
            mSelectState = TopBorderSelect;
         }
      }
      else if ( gmtl::isInVolume(mBottomBorder, mIsectPoint) )
      {
         if ( select_button_state )
         {
            mSelectState = BottomBorderGrab;
            m_wandMdesktop_grab = wand_mat;
            gmtl::invertFull_orig(m_wandMdesktop_grab, m_wandMdesktop_grab);
         }
         else     // Just select it
         {
            if ( BottomBorderSelect != mSelectState )
            {
               std:: cout << "State: BottomBorderSelect" << std::endl;
            }
            mSelectState = BottomBorderSelect;
         }
      }
      else if ( gmtl::isInVolume(mLeftBorder, mIsectPoint) )
      {
         if ( select_button_state )
         {
            mSelectState = LeftBorderGrab;
            m_wandMdesktop_grab = wand_mat;
            gmtl::invertFull_orig(m_wandMdesktop_grab, m_wandMdesktop_grab);
         }
         else     // Just select it
         {
            if ( LeftBorderSelect != mSelectState )
            {
               std:: cout << "State: LeftBorderSelect" << std::endl;
            }
            mSelectState = LeftBorderSelect;
         }
      }
      else if ( gmtl::isInVolume(mRightBorder, mIsectPoint) )
      {
         if ( select_button_state )
         {
            mSelectState = RightBorderGrab;
            m_wandMdesktop_grab = wand_mat;
            gmtl::invertFull_orig(m_wandMdesktop_grab, m_wandMdesktop_grab);
         }
         else     // Just select it
         {
            if ( RightBorderSelect != mSelectState )
            {
               std:: cout << "State: RightBorderSelect" << std::endl;
            }
            mSelectState = RightBorderSelect;
         }
      }
      else     // Default to resetting to nothing
      {
         mSelectState = Nothing;
      }
   }
   // --- Resizing
   else if ( things_grabbed ) // Grab state is active
   {
      if ( LRCornerGrab == mSelectState )
      {
         // Compute the desired change in height and width
         float delta_w = mIsectPoint[0] - mCornerGrabPoint[0];       // now - grab
         float delta_h =  mCornerGrabPoint[1] - mIsectPoint[1];      // Grab - now

         // Transform opposite of change in height to make it look right
         m_vworld_M_desktop *=
            gmtl::makeTrans<gmtl::Matrix44f>(gmtl::Vec3f(0.0f, -delta_h, 0.0f));
         mDesktopWidth += delta_w;
         mDesktopHeight += delta_h;

         // Reset grab point. x becomes new isect. y stays same.
         mCornerGrabPoint[0] = mIsectPoint[0];
      }
      else if ( LLCornerGrab == mSelectState )
      {
         // Compute the desired change in height and width
         float delta_w =  mCornerGrabPoint[0] - mIsectPoint[0];      // grab - now
         float delta_h =  mCornerGrabPoint[1] - mIsectPoint[1];      // Grab - now

         // Transform opposite of change in height and width to look right
         m_vworld_M_desktop *= gmtl::makeTrans<gmtl::Matrix44f>(gmtl::Vec3f(-delta_w, -delta_h, 0.0f));
         mDesktopWidth += delta_w;
         mDesktopHeight += delta_h;
         // Don't reset grab point. Both stay same since we transformed to get
         // them to be the same
         //mCornerGrabPoint[0] = mIsectPoint[0];
      }
      // --- Moving - Rotation and translation --- //
      else if ( LeftBorderGrab == mSelectState ||
                RightBorderGrab == mSelectState ||
                TopBorderGrab == mSelectState ||
                BottomBorderGrab == mSelectState )
      {
         vprDEBUG(vprDBG_ALL, 0) << "Moving or something\n" << vprDEBUG_FLUSH;
         // Compute desired pos and then figure out how to get the desktop to
         // there
         gmtl::Matrix44f desired_pos = (wand_mat * m_wandMdesktop_grab);
         m_vworld_M_desktop *= desired_pos;
      }

      // Get out of grab states if possible
      if(!select_button_state)
      {
         mSelectState = Nothing;
      }
   } // grab states

   updateDesktopParameters();

   // - While there are rectangle updates to process
   //    - Combine the retangles
   mHasRectUpdate = false;
   if ( mVncIf.getFramebufferUpdate(mUpdateRect) )
   {
      mHasRectUpdate = true;
      Rectangle temp_rect;
      while ( mVncIf.getFramebufferUpdate(temp_rect) )
      {
         mUpdateRect.merge(temp_rect);
      }

      // Compute texture upload stats
      const double one_mb(1024.0 * 1024.0);
      double tex_size_mb =
         (mUpdateRect.width * mUpdateRect.height * 8.0 * 1.0) / one_mb;
      mTextureUploadRate.addSample(tex_size_mb);
      mTextureUpdateCount.addSample(tex_size_mb);
   }

   // Check status of focus
   enum Focus focus_val(NOT_IN_FOCUS);
   if ( mSelectState != Nothing )
   {
      focus_val = IN_FOCUS;
   }

   return focus_val;
}

/** Update the desktop texture
* @pre The texture is currently bound
*/
void VNCDesktop::updateDesktopTexture()
{
#if 1
   vprDEBUG(vrjDBG_VNC, vprDBG_STATE_LVL)
      << "VNCDesktop::updateDesktopTexture() entered\n" << vprDEBUG_FLUSH;

   const int bytes_per_pixel(mVncIf.getPixelSize() / 8);
   // - If have update
   // - Get the source buffer pointer and compute any other params
   // - Set the correct pixel transfer params
   // - Load the texture
   // - Add to texture stats
   if ( mHasRectUpdate )
   {
      // Start of source buffer
      const char* src = mVncIf.getFramebuffer() +
                        (((mUpdateRect.y * mVncWidth) + mUpdateRect.x) *
                        bytes_per_pixel);

      // Set the OpenGL row length.  This is used to skip data after each line
      // of pixels is read.
      //glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
      glPixelStorei(GL_UNPACK_ROW_LENGTH, mVncWidth);

      // Subload the texture
      glTexSubImage2D(GL_TEXTURE_2D, 0,
                      (GLint)mUpdateRect.x, (GLint)mUpdateRect.y,
                      (GLsizei)mUpdateRect.width, (GLsizei)mUpdateRect.height,
                      GL_RGBA, GL_UNSIGNED_BYTE, src);

      // Reset to default since this is a "strange" param
      glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
   }

#else

   // If there are updates, consume them and then update the entire buffer
   Rectangle bogus_rect;
   if ( mVncIf.getFramebufferUpdate(bogus_rect) )
   {
      while( mVncIf.getFramebufferUpdate(bogus_rect) )
      {
         /* Nothing */;
      }

      // Initial texture load
      // XXX: I don't think GL_RGBA should be hard-coded since VNC may not
      // actually use 8 bytes per pixel.
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, (GLsizei) mTexWidth,
            (GLsizei) mTexHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE,
            (GLubyte*) mTextureData);

      // Add stats
      // Compute texture stats
      const double one_mb(1024.0*1024.0);
      double tex_size_mb = (mTexWidth*mTexHeight*8.0*1.0)/one_mb;
      mTextureUploadRate.addSample(tex_size_mb);
      mTextureUpdateCount.addSample(tex_size_mb);
   }

#endif

   GLenum err = glGetError();

   if ( err != GL_NO_ERROR )
   {
      vprDEBUG(vprDBG_ERROR, vprDBG_CRITICAL_LVL)
         << clrOutNORM(clrRED, "OpenGL ERROR") << ": "
         << gluErrorString(err) << std::endl << vprDEBUG_FLUSH;
   }

   vprDEBUG(vrjDBG_VNC, vprDBG_STATE_LVL)
      << "VNCDesktop::updateDesktopTexture() exited\n" << vprDEBUG_FLUSH;
}


void VNCDesktop::contextPreDraw()
{
   vprDEBUG(vrjDBG_VNC, vprDBG_STATE_LVL)
      << "VNCDesktop::contextPreDraw() entered\n" << vprDEBUG_FLUSH;

   glPushAttrib(GL_TEXTURE_BIT);

   // Draw the desktop surface
   glEnable(GL_TEXTURE_2D);

   // Check if we need to allocate a texture object
   // XXX: This should really move to contextInit or somewhere like that
   if ( 0 == (*mTexInfo).id )
   {
      // Allocate texture data
      vprASSERT( NULL != mTextureData );     // Already allocated

      // Create and bind texture object
      glGenTextures(1, &((*mTexInfo).id));
      glBindTexture(GL_TEXTURE_2D, mTexInfo->id);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
      vprASSERT(glIsTexture(mTexInfo->id) && "Invalid texture");

      // Initial texture load
      // XXX: I don't think GL_RGBA should be hard-coded since VNC may not
      // actually use 8 bytes per pixel.
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, (GLsizei) mTexWidth,
                   (GLsizei) mTexHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE,
                   (GLubyte*) mTextureData);
   }
   else
   {
      vprASSERT(glIsTexture(mTexInfo->id) && "Invalid texture");
      glBindTexture(GL_TEXTURE_2D, mTexInfo->id);
   }

   // Update the texture
   updateDesktopTexture();

   glPopAttrib();

   vprDEBUG(vrjDBG_VNC, vprDBG_STATE_LVL)
      << "VNCDesktop::contextPreDraw() exited\n" << vprDEBUG_FLUSH;
}

void VNCDesktop::draw()
{
   vprDEBUG(vrjDBG_VNC, vprDBG_STATE_LVL)
      << "VNCDesktop::draw() entered\n" << vprDEBUG_FLUSH;

   // -- Const colors to choose from --//
   // Put them here so we don't have to change a bunch of code below to change
   // the visuals
   const gmtl::Vec3f micro_gui_blue(0.39f, 0.51f, 0.77f);
   const gmtl::Vec3f micro_gui_blue_selected(0.39f, 0.71f, 0.97f);
   const gmtl::Vec3f micro_gui_yellow(0.97f, 0.92f, 0.22f);
   const gmtl::Vec3f micro_gui_yellow_selected(0.97f, 0.4f, 0.22f);
   const gmtl::Vec3f ximian_orange(0.98f, 0.70f, 0.098f);

   const gmtl::Vec3f corner_color(micro_gui_yellow);
   const gmtl::Vec3f corner_color_selected(micro_gui_yellow_selected);
   const gmtl::Vec3f border_color(micro_gui_blue);
   const gmtl::Vec3f border_color_selected(micro_gui_blue_selected);

   glPushMatrix();
   {
      // Get into the desktop coordinate frame
      glMultMatrixf(m_vworld_M_desktop.mData);

      // Save attributes
      glPushAttrib(GL_ALL_ATTRIB_BITS);
      glDisable(GL_BLEND);
      //glEnable(GL_LIGHTING);      // Only use lighting if it is already on
      glEnable(GL_NORMALIZE);

      // -- Draw the desktop "objects" -- //
      // Draw isect point
      //**//drawSphere(0.25f, mIsectPoint);

      // Draw the desktop corners and borders
      setColorIfState(corner_color_selected, corner_color, LLCornerSelect,
                      LLCornerGrab);
      drawBox(mLLCorner);
      setColorIfState(corner_color_selected, corner_color, LRCornerSelect,
                      LRCornerGrab);
      drawBox(mLRCorner);
      setColorIfState(corner_color_selected, corner_color, URCornerSelect,
                      URCornerGrab);
      drawBox(mURCorner);
      setColorIfState(corner_color_selected, corner_color, ULCornerSelect,
                      ULCornerGrab);
      drawBox(mULCorner);

      setColorIfState(border_color_selected, border_color, LeftBorderSelect,
                      LeftBorderSelect);
      drawCylinder(mLeftBorder,1);
      setColorIfState(border_color_selected, border_color, RightBorderSelect,
                      RightBorderSelect);
      drawCylinder(mRightBorder,1);
      setColorIfState(border_color_selected, border_color, TopBorderSelect,
                      TopBorderSelect);
      drawCylinder(mTopBorder,0);
      setColorIfState(border_color_selected, border_color, BottomBorderSelect,
                      BottomBorderSelect);
      drawCylinder(mBottomBorder,0);

      glDisable(GL_LIGHTING);    // The stuff below doesn't like the light...

      // Draw the ray (if we are pointing at the desktop. ie. it is in focus)
      if ( Nothing != mSelectState )
      {
         glPushMatrix();
            gmtl::Vec3f ray_start = mWandRay.getOrigin();
            gmtl::Vec3f ray_end = mIsectPoint;

            glPolygonMode(GL_FRONT, GL_LINE);
            glLineWidth(1);
            glColor3f(1.0f, 0.0f, 0.0f);

            glBegin(GL_LINES);
               glVertex3fv(ray_start.mData);
               glVertex3fv(ray_end.mData);
            glEnd();

            glPolygonMode(GL_FRONT, GL_FILL);
         glPopMatrix();
      }

      // --- DRAW DESKTOP --- //
      glEnable(GL_TEXTURE_2D);
      glBindTexture(GL_TEXTURE_2D, mTexInfo->id);
      glColor3f(1.0f, 1.0f, 1.0f);

      // --- COMPUTE TEX Coords --- //
      // Rolling texcoord should vary between 0.0 and mMaxTexCoordY
      float min_tex_coordY = 0.0f;            // Default to full
      if ( RollingDown == mActiveState || RollingUp == mActiveState )
      {
         min_tex_coordY += mRollUpPercent*mMaxTexCoordY;  // [0.0,mMaxTexCoordY]
      }

      glBegin(GL_QUADS);
         // Draw quad, counter counter clockwise from bottom left hand corner
         // (0,0)
         glTexCoord2f(0.0f, mMaxTexCoordY);           // LL
         glVertex3f(0.0f, 0.0f, 0.0f);

         glTexCoord2f(mMaxTexCoordX, mMaxTexCoordY);           // LR
         glVertex3f(mDesktopWidth, 0.0f, 0.0f);

         glTexCoord2f(mMaxTexCoordX, min_tex_coordY);           // UR
         glVertex3f(mDesktopWidth, mDesktopHeight, 0.0f);

         glTexCoord2f(0.0f, min_tex_coordY);           // UL
         glVertex3f(0.0f, mDesktopHeight, 0.0f);
      glEnd();

      // Check for errors
      GLenum err = glGetError();
      if ( err != GL_NO_ERROR )
      {
         vprDEBUG(vprDBG_ERROR, vprDBG_CRITICAL_LVL)
            << clrOutNORM(clrRED, "OpenGL ERROR") << ": "
            << gluErrorString(err) << std::endl << vprDEBUG_FLUSH;
      }

      // Set back to default texture
      glBindTexture(GL_TEXTURE_2D, 0);
      glDisable(GL_TEXTURE_2D);

      // Set back to original attribute state
      glPopAttrib();
   }
   glPopMatrix();

   vprDEBUG(vrjDBG_VNC, vprDBG_STATE_LVL)
      << "VNCDesktop::draw() exited\n" << vprDEBUG_FLUSH;
}


void VNCDesktop::printStats()
{
   std::cout << "Texture upload/sec: mean: " << mTextureUploadRate.getMean()
             // << "  sta: " << mTextureUploadRate.getSTA()
             << std::endl;
   std::cout << "Texture counts:     mean: " << mTextureUpdateCount.getMean()
             //<< "  sta: " << mTextureUpdateCount.getSTA()
             << std::endl;
}

void VNCDesktop::drawSphere(float radius, gmtl::Point3f offset, int parts)
{
   glPushMatrix();
      glTranslatef(offset[0], offset[1], offset[2]);
      gluSphere(mSphereQuad, radius, parts, parts);
   glPopMatrix();
}

void VNCDesktop::drawCylinder(const gmtl::AABoxf& cyBox, unsigned majorAxis,
                              unsigned slices)
{
   // Find the axis to use for non major axis stuff (like radius)
   unsigned non_major = majorAxis+1;
   if ( 3 == non_major )
   {
      non_major = 0;
   }
   const float radius((cyBox.mMax[non_major] - cyBox.mMin[non_major])/2.0f);

   // Compute the point at the center of the base of this cylinder
   gmtl::Point3f temp_point(cyBox.mMax + cyBox.mMin);
   gmtl::Point3f center_base(temp_point / 2.0f);     // Average 2 points
   center_base[majorAxis] = cyBox.mMin[majorAxis];             // Project down to the min side of major axis

   //std::cout << " cy: box: " << cyBox << "   major:" << majorAxis
   //          << "   center:" << center_base << std::endl;
   float height = cyBox.mMax[majorAxis] - cyBox.mMin[majorAxis];

   glPushMatrix();
      glTranslatef(center_base.mData[0], center_base.mData[1],
                   center_base.mData[2]);
      if ( 0 == majorAxis )
      {
         glRotatef(90.0f, 0.0f, 1.0f, 0.0f);    // Rotate to x-axis
      }
      else if ( 1 == majorAxis )
      {
         glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);   // Rotate to y-axis
      }

      gluCylinder(mCylinderQuad, radius, radius, height, slices, 1);
   glPopMatrix();
}

void VNCDesktop::drawBox(const gmtl::AABoxf& box)
{
   // Define the normals for the box faces.
   // left, top, right, bottom, front, back
   static GLfloat normals[6][3] =
   {
      { -1.0, 0.0, 0.0 }, { 0.0, 1.0, 0.0 }, { 1.0, 0.0, 0.0 },
      { 0.0, -1.0, 0.0 }, { 0.0, 0.0, 1.0 }, { 0.0, 0.0, -1.0 }
   };

   // Define the array indices for the cube faces.  These will be used to
   // access values in the v array declared below.
   // left,
   static GLint faces[6][4] =
   {
      { 0, 1, 2, 3 }, { 3, 2, 6, 7 }, { 7, 6, 5, 4 },
      { 4, 5, 1, 0 }, { 5, 6, 2, 1 }, { 7, 4, 0, 3 }
   };

   gmtl::Point3f box_min = box.getMin();
   gmtl::Point3f box_max = box.getMax();

   // Define the vertices based on the min and max points on the cube.
   // 0,1,2,3 left
   // 4,5,6,7 right
   GLfloat v[8][3];
   v[0][0] = v[1][0] = v[2][0] = v[3][0] = box_min[0];   // Left x
   v[4][0] = v[5][0] = v[6][0] = v[7][0] = box_max[0];   // Right x

   v[0][1] = v[1][1] = v[4][1] = v[5][1] = box_min[1];   // bottom y
   v[2][1] = v[3][1] = v[6][1] = v[7][1] = box_max[1];   // top y

   v[0][2] = v[3][2] = v[4][2] = v[7][2] = box_min[2];   // back
   v[1][2] = v[2][2] = v[5][2] = v[6][2] = box_max[2];   // front

   // Draw the box.
   for ( GLint i = 0; i < 6; ++i )
   {
      glBegin(GL_QUADS);
         glNormal3fv(&normals[i][0]);
         glVertex3fv(&v[faces[i][0]][0]);
         glVertex3fv(&v[faces[i][1]][0]);
         glVertex3fv(&v[faces[i][2]][0]);
         glVertex3fv(&v[faces[i][3]][0]);
      glEnd();
   }
}

} // End of vrjvnc namespace
