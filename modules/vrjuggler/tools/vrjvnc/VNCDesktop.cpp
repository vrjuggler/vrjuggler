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
     mVncThread(NULL), mHaveKeyboard(false),
     mDesktopWidth(desktopSideLength), mDesktopHeight(desktopSideLength),
     mDesktopWandIsect(false), mDesktopGrabbed(false),
     mTextureData(NULL)
{
   mVncWidth = mVncIf.getWidth();
   mVncHeight = mVncIf.getHeight();

   mTexWidth  = getNearestMultipleOfTwo(mVncIf.getWidth());
   mTexHeight = getNearestMultipleOfTwo(mVncIf.getHeight());

   updateDesktopParameters();       // Initial update of desktop parameters

   // Request the first update.
   mVncIf.updateFramebuffer(0, 0, mVncIf.getWidth(), mVncIf.getHeight());

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
}

/** Updates the desktop parameters
* @pre mDesktopWidth, mDesktopHeight, and pos matrix are update
* @post Everything else is updated correspondingly
*/
void VNCDesktop::updateDesktopParameters()
{
   // --- Update scales and bounds. --- //
   mDesktopToVncWidthScale  = mVncWidth / mDesktopWidth;
   mDesktopToVncHeightScale = mVncHeight / mDesktopHeight;

   // Configure the desktop bounding box
   mDesktopBox.setMin(gmtl::Point3f(0, 0, -0.50f));
   mDesktopBox.setMax(gmtl::Point3f(mDesktopWidth, mDesktopHeight, 0.50f));
   mDesktopBox.setEmpty(false);

   // Set the translation point to be the middle of the desktop polygon.
   //gmtl::setTrans(mDesktopMatrix, gmtl::Vec3f(0.0f, 5.0f, -5.0f));
}

VNCDesktop::Focus VNCDesktop::update(const gmtl::Matrix44f& navMatrix)
{
   vpr::DebugOutputGuard guard(vprDBG_ALL, vprDBG_STATE_LVL,
                               "VNCDesktop::update()\n",
                               "VNCDesktop::update() done.\n");

   enum Focus focus_val(NOT_IN_FOCUS);

   // -------- COPY OVER TEXTURE DATA --------- //
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

   // --------- UPDATE NAV AND DESKTOP MATRICES ----------------------- //

   //gmtl::Matrix44f desktop_nav = navMatrix * mDesktopMatrix;

   // Do all intersection testing and stuff in the local coordinate frame so we don't have
   // to deal with rotations and translations of the desktop.
   // Just transform the wand into the local frame and we are set to go with minimal effort.

   // Get the point position of the wand and construct a ray that shoots out of the wand.
   const gmtl::Matrix44f wand_mat(*(mWand->getData()));
   const gmtl::Point3f wand_point(gmtl::makeTrans<gmtl::Point3f>(wand_mat));
   const gmtl::Vec3f ray_vector(0.0f, 0.0f, -100.0f);
   const gmtl::Rayf wand_ray(gmtl::makeTrans<gmtl::Point3f>(wand_mat), (wand_mat*ray_vector));

   // Find ray intersection on the z=0 plane
   const gmtl::Vec3f pos_z_norm(0.0f, 0.0f, 1.0f);
   const gmtl::Point3f origin_point(0.0f, 0.0f, 0.0f);
   const gmtl::Planef z_plane(pos_z_norm, origin_point);

   // Find the point on the desktop plane that the ray intersects it
   // this is our intersection in desktop coordinates (polygon).
   // It will have to be scaled to get back into vnc coords.
   float t_isect;
   gmtl::intersect(z_plane, wand_ray, t_isect);
   const gmtl::Point3f isect_point(wand_ray.mOrigin + (wand_ray.mDir*t_isect));
   vprASSERT( gmtl::Math::isEqual(isect_point[2], 0.0f, 0.01f) && "Point should be on z=0 plane");
   mDebug_IsectPoint = isect_point;

   vprDEBUG(vprDBG_ALL, vprDBG_VERB_LVL)
         << "VNC: Isect point: " << isect_point << std::endl << vprDEBUG_FLUSH;

   // Check for selecting the main desktop box
   if ( gmtl::isInVolume(mDesktopBox, isect_point) )
   {
      focus_val = IN_FOCUS;

      // Compute VNC button masks
      int button_mask(0);

      if ( mLeftButton->getData() )
      {  button_mask |= rfbButton1Mask; }
      if ( mMiddleButton->getData())
      {  button_mask |= rfbButton2Mask; }
      if ( mRightButton->getData() )
      {  button_mask |= rfbButton3Mask; }

      // Translate that point into the coordinates VNC wants to see.
      //
      // vnc x,y desktop point just like x desktop.  origin upper left, y increases going down
      //     The valid range is [0,mVncWidth or mVncHeight]
      float vnc_x = isect_point[gmtl::Xelt] * mDesktopToVncWidthScale;                          // Scale
      float vnc_y = -(isect_point[gmtl::Yelt] - mDesktopHeight) * mDesktopToVncHeightScale;     // Flip and scale

      mVncIf.pointerEvent(int(vnc_x), int(vnc_y), button_mask);

      if ( mHaveKeyboard )
      {
         // Handle keyboard input.
      }
   }

   /*    Grabbing
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
   */


   return focus_val;
}

void VNCDesktop::draw()
{
   // XXX: Should probably use an attribute stack or something here.
   glDisable(GL_LIGHTING);
   glDisable(GL_BLEND);

   // -- Draw the desktop "objects" -- //
   // Draw isect point
   drawSphere(0.25f, mDebug_IsectPoint);

   // Draw the ray
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
         glColor3f(1.0f, 1.0f, 1.0f);
         glBindTexture(GL_TEXTURE_2D, tex_name);
         glBegin(GL_QUADS);
           // Draw quad, counter counter clockwise from bottom left hand corner (0,0)
            glTexCoord2f(0.0f, 1.0f);           // LL
            glVertex3f(0.0f, 0.0f, 0.0f);

            glTexCoord2f(1.0f, 1.0f);           // LR
            glVertex3f(mDesktopWidth, 0.0f, 0.0f);

            glTexCoord2f(1.0f, 0.0f);           // UR
            glVertex3f(mDesktopWidth, mDesktopHeight, 0.0f);

            glTexCoord2f(0.0f, 0.0f);           // UL
            glVertex3f(0.0f, mDesktopHeight, 0.0f);
         glEnd();


         // Check for errors
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

void VNCDesktop::drawSphere(float radius, gmtl::Point3f offset, int parts)
{
   glPushMatrix();
      glTranslatef(offset[0], offset[1], offset[2]);
      gluSphere(mSphereQuad, radius, parts, parts);
   glPopMatrix();
}


} // End of vrjvnc namespace
