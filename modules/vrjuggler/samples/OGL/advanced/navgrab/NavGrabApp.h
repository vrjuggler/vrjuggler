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
#include <gmtl/Point.h>
#include <gmtl/Sphere.h>
#include <gmtl/AABox.h>

#include <gadget/Type/PositionInterface.h>
#include <gadget/Type/DigitalInterface.h>
#include <vrj/Draw/OGL/GlApp.h>

#include <vrj/Test/TestRunner.h>


class NavGrabApp : public vrj::GlApp
{
public:
   NavGrabApp()
      : vrj::GlApp(), mSphereQuad(NULL), mSphereIsect(false),
        mSphereSelected(false), mCubeIsect(false), mCubeSelected(false),
        mFramesToSleep(25), mTestRunner(NULL)
   {
      initShapes();
   }

   NavGrabApp(vrj::Kernel* kern)
      : vrj::GlApp(kern), mSphereQuad(NULL), mSphereIsect(false),
        mSphereSelected(false), mCubeIsect(false), mCubeSelected(false),
        mFramesToSleep(25), mTestRunner(NULL)
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

   virtual void reset();

   void dumpState();

   /** Initialize the test runner and the associated tests.
   * This also sets up the runner to start processing tests
   */
   void initTesting();

   friend class SphereTestCase;     // Make friends with our test case so it can poke our internals

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
   gadget::DigitalInterface  mGrabButton;
   gadget::DigitalInterface  mResetButton;
   gadget::DigitalInterface  mDumpStateButton;

   gadget::DigitalInterface     mLoggerPlayButton;   // Playback log file when pressed

   gmtl::Point3f mSphereCenter;
   float         mSphereRadius;
   gmtl::Spheref mSphere;
   GLUquadric*   mSphereQuad;
   bool          mSphereIsect;
   bool          mSphereSelected;

   gmtl::Point3f mCubeMin;
   gmtl::Point3f mCubeMax;
   gmtl::AABoxf  mCube;
   bool          mCubeIsect;
   bool          mCubeSelected;

   unsigned                mFramesToSleep;         /**< Number of frames to sleep at start up */
   vrj::test::TestRunner*  mTestRunner;            /**< Test runner for this appliation */
};


#endif /* _INTERACT_APP_H_ */
