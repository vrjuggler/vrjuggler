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

#ifndef _VRJVNC_VNC_DESKTOP_H_
#define _VRJVNC_VNC_DESKTOP_H_

#include <stdlib.h>
#include <string>
#include <gmtl/Matrix.h>
#include <gmtl/AABox.h>
#include <gmtl/Tri.h>
#include <vpr/vpr.h>
#include <gadget/Type/PositionInterface.h>
#include <gadget/Type/DigitalInterface.h>
#include <gadget/Type/KeyboardInterface.h>

#include <VNCInterface.h>


namespace vrjvnc
{

class VNCDesktop
{
public:
   enum Focus
   {
      IN_FOCUS,     /**< */
      NOT_IN_FOCUS  /**< */
   };

   VNCDesktop(const std::string& hostname, const vpr::Uint16& port,
              const std::string& password, const float& desktopSideLength);

   ~VNCDesktop();

   void init(const std::string& wandName, const std::string& leftButtonName,
             const std::string& middleButtonName,
             const std::string& rightButtonName,
             const std::string& keyboardName = std::string(""));

   Focus update(const gmtl::Matrix44f& navMatrix);

   /**
    * Renders the desktop.
    *
    * @pre The navigation matrix is on the stack.
    */
   void draw();

protected:     // Helpers

   /** Updates the desktop parameters
   * @pre mDesktopWidth, mDesktopHeight, and pos matrix are update
   * @post Everything else is updated correspondingly
   */
   void updateDesktopParameters();


   void drawSphere(float radius, gmtl::Point3f offset, int parts=4);

private:
   VNCInterface                         mVncIf;
   vpr::ThreadRunFunctor<VNCInterface>* mVncThreadFunctor;
   vpr::Thread*                         mVncThread;

   gadget::PositionInterface mWand;
   gadget::DigitalInterface  mLeftButton;
   gadget::DigitalInterface  mMiddleButton;
   gadget::DigitalInterface  mRightButton;
   gadget::KeyboardInterface mKeyboard;

   bool mHaveKeyboard;

private:
   /** @name Desktop parameters
   * The desktop window (ie. the texture of the desktop) is assumed to be
   * centered on 0,0,0 in the local coordinate frame with
   */
   //@{
   float mDesktopWidth, mDesktopHeight;      /**< Width and height of the virtual desktop. VRJ units */
   int   mTexWidth, mTexHeight;             /**< Width and height of the texture in pixels */
   int   mVncWidth, mVncHeight;              /**< The width and height of the vnc/X desktop */

   /** Scale value for converting from virt desktop size to VNC coords
   *    ex. vnc.x = desktoppoly.x * mDesktopToVncWidthScale;
   */
   float          mDesktopToVncWidthScale;
   float          mDesktopToVncHeightScale;

   gmtl::Matrix44f  mDesktopMatrix;  /**< vnc_T_world */
   gmtl::AABoxf     mDesktopBox;
   //@}

   bool mDesktopWandIsect;
   bool mDesktopGrabbed;

   char* mTextureData;

   // Debugging stuff
   GLUquadric*   mSphereQuad;       /**< Sphere rep for draw "points" */
   gmtl::Point3f mDebug_IsectPoint; /**< Isect point to draw */

};

}


#endif /*_VRJVNC_VNC_DESKTOP_H_ */
