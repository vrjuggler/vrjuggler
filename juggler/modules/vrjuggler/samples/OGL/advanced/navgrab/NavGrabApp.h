/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2002 by Iowa State University
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

#ifndef _INTERACT_APP_H_
#define _INTERACT_APP_H_

#include <stdlib.h>
#include <gmtl/Sphere.h>
#include <gmtl/AABox.h>

#include <gadget/Type/PositionInterface.h>
#include <gadget/Type/DigitalInterface.h>
#include <vrj/Draw/OGL/GlApp.h>


class NavGrabApp : public vrj::GlApp
{
public:
   NavGrabApp()
      : vrj::GlApp(), mSphereQuad(NULL), mSphereIsect(false),
        mSphereSelected(false), mCubeIsect(false), mCubeSelected(false)
   {
      initShapes();
   }

   NavGrabApp(vrj::Kernel* kern)
      : vrj::GlApp(kern), mSphereQuad(NULL), mSphereIsect(false),
        mSphereSelected(false), mCubeIsect(false), mCubeSelected(false)
   {
      initShapes();
   }

   virtual ~NavGrabApp()
   {
      gluDeleteQuadric(mSphereQuad);
   }

   virtual void init();

   virtual void contextInit();

   virtual void preFrame();

   virtual void bufferPreDraw();

   virtual void draw();

private:
   void initShapes();

   void initGLState();

   void drawSphere(const gmtl::Spheref& sphere, const bool& intersected,
                   const bool& selected);

   void drawCube(const gmtl::AABoxf& cube, const bool& intersected,
                 const bool& selected);

   void drawFloor();

   gadget::PositionInterface mHead;
   gadget::PositionInterface mWand;
   gadget::DigitalInterface  mButton0;

   gmtl::Spheref mSphere;
   GLUquadric*   mSphereQuad;
   bool          mSphereIsect;
   bool          mSphereSelected;

   gmtl::AABoxf  mCube;
   bool          mCubeIsect;
   bool          mCubeSelected;
};


#endif /* _INTERACT_APP_H_ */
