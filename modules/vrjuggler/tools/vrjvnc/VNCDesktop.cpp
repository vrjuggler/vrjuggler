/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VRJ VNC is (C) Copyright 2003 by Iowa State University
 *
 * Original Author:
 *   Patrick Hartling
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
 *************** <auto-copyright.pl END do not edit this line> ***************/

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
   : mVncIf(hostname, port, password), mVncThreadFunctor(NULL),
     mVncThread(NULL), mHaveKeyboard(false), mRectSize(desktopSideLength,
     desktopSideLength), mDesktopWandIsect(false), mDesktopGrabbed(false),
     mTextureData(NULL)
{
   mTexWidth  = getNearestMultipleOfTwo(mVncIf.getWidth());
   mTexHeight = getNearestMultipleOfTwo(mVncIf.getHeight());

   mWidthScale  = mTexWidth / mRectSize[0];
   mHeightScale = mTexHeight / mRectSize[1];

   const float half_width(mRectSize[0] / 2.0f);
   const float half_height(mRectSize[1] / 2.0f);

   const gmtl::Point3f desktop_center(0.0f, 0.0f, 0.0f);

   // Define the triangle for the upper-left half of the desktop square.
   // 1---0
   // |  /
   // | /
   // |/
   // 2
   mDesktopUL[0] = gmtl::Point3f(desktop_center[0] + half_width,
                                 desktop_center[1] + half_height,
                                 desktop_center[2]);
   mDesktopUL[1] = gmtl::Point3f(desktop_center[0] - half_width,
                                 desktop_center[1] + half_height,
                                 desktop_center[2]);
   mDesktopUL[2] = gmtl::Point3f(desktop_center[0] - half_width,
                                 desktop_center[1] - half_height,
                                 desktop_center[2]);

   // Define the triangle for the lower-right half of the desktop square.
   //     0
   //    /|
   //   / |
   //  /  |
   // 1---2
   mDesktopLR[0] = mDesktopUL[0];
   mDesktopLR[1] = mDesktopUL[2];
   mDesktopLR[2] = gmtl::Point3f(desktop_center[0] + half_width,
                                 desktop_center[1] - half_width,
                                 desktop_center[2]);

   vprDEBUG(vprDBG_ALL, vprDBG_CONFIG_LVL)
      << "mDesktopUL: " << mDesktopUL << std::endl << vprDEBUG_FLUSH;
   vprDEBUG(vprDBG_ALL, vprDBG_CONFIG_LVL)
      << "mDesktopLR: " << mDesktopLR << std::endl << vprDEBUG_FLUSH;

   mDesktopBox.setMin(gmtl::Point3f(mDesktopUL[2][0], mDesktopUL[2][1],
                                    mDesktopUL[2][2] - 0.25f));
   mDesktopBox.setMax(gmtl::Point3f(mDesktopUL[0][0], mDesktopUL[0][1],
                                    mDesktopUL[0][2] + 0.5f));
   mDesktopBox.setEmpty(false);

   // Set the translation point to be the middle of the desktop polygon.
   gmtl::setTrans(mDesktopMatrix, gmtl::Vec3f(0.0f, 5.0f, -5.0f));

   // Request the first update.
   mVncIf.updateFramebuffer(0, 0, mVncIf.getWidth(), mVncIf.getHeight());

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
}

VNCDesktop::Focus VNCDesktop::update(const gmtl::Matrix44f& navMatrix)
{
   vpr::DebugOutputGuard guard(vprDBG_ALL, vprDBG_STATE_LVL,
                               "VNCDesktop::update()\n",
                               "VNCDesktop::update() done.\n");

   enum Focus focus_val(NOT_IN_FOCUS);

   Rectangle r;

   r.x      = 0;
   r.y      = 0;
   r.width  = mTexWidth;
   r.height = mTexHeight;

   const int bytes_per_pixel(mVncIf.getPixelSize() / 8);
   const size_t update_size(r.width * r.height * bytes_per_pixel);

   vprDEBUG(vprDBG_ALL, vprDBG_HVERB_LVL)
      << "bytes_per_pixel == " << bytes_per_pixel << std::endl
      << vprDEBUG_FLUSH;
   vprDEBUG(vprDBG_ALL, vprDBG_HVERB_LVL)
      << "update_size == " << update_size << std::endl << vprDEBUG_FLUSH;

   // Allocate mTextureData if it has not already been created.  It is only
   // allocated once under the assumption that the VNC frame buffer size does
   // not change.
   if ( NULL == mTextureData )
   {
      mTextureData = (char*) malloc(update_size);
   }

   // Zero out the texture data.
   memset(mTextureData, 0, update_size);

   // Scary math ...
   const char* src = mVncIf.getFramebuffer() + r.y * mVncIf.getWidth() + r.x;
   char* dest = mTextureData;

   const int copy_width  = std::min(mVncIf.getWidth() - r.x, r.width);
   const int copy_height = std::min(mVncIf.getHeight() - r.y, r.height);

   vprDEBUG(vprDBG_ALL, vprDBG_HVERB_LVL)
      << "copy_width == " << copy_width << std::endl << vprDEBUG_FLUSH;
   vprDEBUG(vprDBG_ALL, vprDBG_HVERB_LVL)
      << "copy_height == " << copy_height << std::endl << vprDEBUG_FLUSH;

   // Copy the frame buffer into our texture data buffer.
   for ( int i = 0; i < copy_height; ++i )
   {
      memcpy(dest, src, copy_width * bytes_per_pixel);
      src  += mVncIf.getWidth() * bytes_per_pixel;
      dest += r.width * bytes_per_pixel;
   }

/*
   for ( int h = 0; h < mTexHeight; ++h )
   {
      for ( int w = 0; w < mTexWidth; ++w )
      {
         std::cerr << std::setfill('0') << std::setw(2) << std::hex
                   << (unsigned int) mTextureData[mTexWidth * h + w];
      }

      std::cerr << std::endl;
   }
*/

   gmtl::Matrix44f desktop_nav = navMatrix * mDesktopMatrix;

   // Get the point position of the wand and construct a ray that shoots out
   // of the wand.
   const gmtl::Matrix44f wand_mat(*(mWand->getData()));
   const gmtl::Point3f wand_point(gmtl::makeTrans<gmtl::Point3f>(wand_mat));
   const gmtl::Vec3f ray_vector(0.0f, 0.0f, -100.0f);
   const gmtl::Rayf wand_ray(gmtl::makeTrans<gmtl::Point3f>(wand_mat),
                             wand_mat * ray_vector);

   // Construct a transformed triangle for the upper-left half of the desktop.
   const gmtl::Point3f desktop_ul_pts[] =
      {
         gmtl::Point3f(desktop_nav * mDesktopUL[0]),
         gmtl::Point3f(desktop_nav * mDesktopUL[1]),
         gmtl::Point3f(desktop_nav * mDesktopUL[2])
      };
   const gmtl::Tri<float> desktop_ul_trans(desktop_ul_pts[0],
                                           desktop_ul_pts[1],
                                           desktop_ul_pts[2]);

   // Construct a transformed triangle for the lower-right half of the desktop.
   const gmtl::Point3f desktop_lr_pts[] =
      {
         gmtl::Point3f(desktop_nav * mDesktopLR[0]),
         gmtl::Point3f(desktop_nav * mDesktopLR[1]),
         gmtl::Point3f(desktop_nav * mDesktopLR[2])
      };
   const gmtl::Tri<float> desktop_lr_trans(desktop_lr_pts[0],
                                           desktop_lr_pts[1],
                                           desktop_lr_pts[2]);

   // Transform the bounding box.
   const gmtl::AABoxf desktop_box_trans(desktop_nav * mDesktopBox.getMin(),
                                        desktop_nav * mDesktopBox.getMax());

   // Storage for information returned by gmtl::intersect().
   float u, v, t;

   mDesktopWandIsect = gmtl::isInVolume(desktop_box_trans, wand_point);

   // If the wand intersects the desktop, button operations will affect
   // the position rather than perform desktop interaction.
   if ( mDesktopWandIsect )
   {
      vprDEBUG(vprDBG_ALL, vprDBG_STATE_LVL)
         << "Wand and desktop intersection\n" << vprDEBUG_FLUSH;

      focus_val = IN_FOCUS;

      mDesktopWandIsect = true;

      if ( mLeftButton->getData() )
      {
         vprDEBUG(vprDBG_ALL, vprDBG_STATE_LVL) << "Desktop grabbed!\n"
                                                << vprDEBUG_FLUSH;

         mDesktopGrabbed = true;
      }
   }
   // If the wand does not intersect the desktop, test to see if the wand
   // ray intersects it.  If it does, then we are going to be moving the
   // mouse and maybe pressing buttons.
   else if ( gmtl::intersect(desktop_ul_trans, wand_ray, u, v, t) || 
             gmtl::intersect(desktop_lr_trans, wand_ray, u, v, t) )
   {
      focus_val = IN_FOCUS;

      // Get the point of intersection.
      gmtl::Point3f isect = wand_ray.mDir * t + wand_ray.mOrigin;

      vprDEBUG(vprDBG_ALL, vprDBG_VERB_LVL)
         << "Got an intersection at " << isect << std::endl << vprDEBUG_FLUSH;

      // Translate that point into the coordinates VNC wants to see.
      gmtl::Point3f vnc_isect = isect - desktop_ul_trans[1];
      float x(isect[0] - desktop_ul_trans[2][0]);
      float y(mRectSize[1] - isect[1] - desktop_ul_trans[2][1]);

      int button_mask(0);

      if ( mLeftButton->getData() == gadget::Digital::ON )
      {
         button_mask |= rfbButton1Mask;
      }

      if ( mMiddleButton->getData() == gadget::Digital::TOGGLE_ON )
      {
         button_mask |= rfbButton2Mask;
      }

      if ( mRightButton->getData() == gadget::Digital::TOGGLE_ON )
      {
         button_mask |= rfbButton3Mask;
      }

      mVncIf.pointerEvent(int(x * mWidthScale), int(y * mHeightScale),
                          button_mask);

      if ( mHaveKeyboard )
      {
         // Handle keyboard input.
      }
   }

   return focus_val;
}

void VNCDesktop::draw()
{
   // XXX: Should probably use an attribute stack or something here.
   glDisable(GL_LIGHTING);
   glDisable(GL_BLEND);

   glPushMatrix();
      glMultMatrixf(mWand->getData()->mData);

      glPolygonMode(GL_FRONT, GL_LINE);
      glLineWidth(2);
      glColor3f(1.0f, 0.0f, 0.0f);

      glBegin(GL_LINES);
         glVertex3f(0.0f, 0.0f, 0.0f);
         glVertex3f(0.0f, 0.0f, -100.0f);
      glEnd();

      glPolygonMode(GL_FRONT, GL_FILL);
   glPopMatrix();

   glPushMatrix();
      glEnable(GL_TEXTURE_2D);

      GLuint tex_name;
      glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
      glGenTextures(1, &tex_name);
      glBindTexture(GL_TEXTURE_2D, tex_name);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);

      // XXX: I don't think GL_RGBA should be hard-coded since VNC may not
      // actually use 8 bytes per pixel.
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, (GLsizei) mTexWidth,
                  (GLsizei) mTexHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE,
                  (GLubyte*) mTextureData);

      vprASSERT(glIsTexture(tex_name) && "Invalid texture");
      GLenum err = glGetError();

      if ( err != GL_NO_ERROR )
      {
         vprDEBUG(vprDBG_ERROR, vprDBG_CRITICAL_LVL)
            << clrOutNORM(clrRED, "OpenGL ERROR") << ": "
            << gluErrorString(err) << std::endl << vprDEBUG_FLUSH;
      }
      else
      {
         glMultMatrixf(mDesktopMatrix.mData);

         glColor3f(1.0f, 1.0f, 1.0f);
         glBindTexture(GL_TEXTURE_2D, tex_name);
         glBegin(GL_QUADS);
/*
            glTexCoord2f(0.0f, 0.0f);
            glVertex3f(mDesktopUL[0][0], mDesktopUL[0][1], mDesktopUL[0][2]);
            glTexCoord2f(1.0f, 0.0f);
            glVertex3f(mDesktopUL[1][0], mDesktopUL[1][1], mDesktopUL[1][2]);
            glTexCoord2f(1.0f, 1.0f);
            glVertex3f(mDesktopUL[2][0], mDesktopUL[2][1], mDesktopUL[2][2]);
            glTexCoord2f(0.0f, 1.0f);
            glVertex3f(mDesktopLR[2][0], mDesktopLR[2][1], mDesktopLR[2][2]);
*/
            // XXX: This specifies the texture coordinates in clockwise order.
            // I had to do this to get the image to show up correctly, though
            // I am not sure why ...
            glTexCoord2f(1.0f, 0.0f);
            glVertex3f(mDesktopUL[0][0], mDesktopUL[0][1], mDesktopUL[0][2]);
            glTexCoord2f(0.0f, 0.0f);
            glVertex3f(mDesktopUL[1][0], mDesktopUL[1][1], mDesktopUL[1][2]);
            glTexCoord2f(0.0f, 1.0f);
            glVertex3f(mDesktopUL[2][0], mDesktopUL[2][1], mDesktopUL[2][2]);
            glTexCoord2f(1.0f, 1.0f);
            glVertex3f(mDesktopLR[2][0], mDesktopLR[2][1], mDesktopLR[2][2]);
         glEnd();

         // XXX: It would probably be good to render nothing on the back
         // side of the desktop polygon.

         err = glGetError();

         if ( err != GL_NO_ERROR )
         {
            vprDEBUG(vprDBG_ERROR, vprDBG_CRITICAL_LVL)
               << clrOutNORM(clrRED, "OpenGL ERROR") << ": "
               << gluErrorString(err) << std::endl << vprDEBUG_FLUSH;
         }

         glBindTexture(GL_TEXTURE_2D, 0);
      }

      glDeleteTextures(1, &tex_name);

      glDisable(GL_TEXTURE_2D);
   glPopMatrix();

   glEnable(GL_LIGHTING);
}

} // End of vrjvnc namespace
