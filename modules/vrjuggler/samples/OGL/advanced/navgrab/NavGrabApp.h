/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2008 by Iowa State University
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

#ifndef _INTERACT_APP_H_
#define _INTERACT_APP_H_

#include <stdlib.h>

#if defined(__APPLE__)
#  include <OpenGL/gl.h>
#  include <OpenGL/glu.h>
#else
#  include <GL/gl.h>
#  include <GL/glu.h>
#endif

#include <gmtl/Point.h>
#include <gmtl/Sphere.h>
#include <gmtl/AABox.h>

#include <gadget/Type/PositionInterface.h>
#include <gadget/Type/DigitalInterface.h>
#include <vrj/Draw/OpenGL/App.h>

#include <vrj/Test/TestRunner.h>


class NavGrabApp : public vrj::opengl::App
{
public:
   NavGrabApp(vrj::Kernel* kern = NULL)
      : vrj::opengl::App(kern)
      , mSphereQuad(NULL)
      , mSphereIsect(false)
      , mSphereSelected(false)
      , mCubeIsect(false)
      , mCubeSelected(false)
      , mFramesToSleep(25)
      , mTestRunner(NULL)
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

protected:
   /** Update any selection and grabbing information */
   void updateGrabbing();

   /** Update the state of the current navigation */
   void updateNavigation();

   /** Update any logger playback that is being used */
   void updateLogger();

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
   
   gadget::DigitalInterface  mForwardButton;       /**< Button to go forward */
   gadget::DigitalInterface  mRotateButton;        /**< Button to rotate */

   gadget::DigitalInterface  mGrabButton;          /**< Button to grab an object */
   gadget::DigitalInterface  mDumpStateButton;     /**< Button to dump the current state */
      
   // Sphere information
   gmtl::Spheref mSphere;                          /**< State of the sphere */
   GLUquadric*   mSphereQuad;                      /**< GL Quadric for drawing the sphere */
   bool          mSphereIsect;                     /**< If true, the wand is isecting with the sphere */
   bool          mSphereSelected;                  /**< If true, then the user is grabbing the sphere */

   float         mCubeEdgeLength;                  /**< Length of an edge on the cube */
   gmtl::AABoxf  mCube;                            /**< State of the cube */
   bool          mCubeIsect;                       /**< If true, then the wand is isecting with the cube */
   bool          mCubeSelected;                    /**< If true, then the cube is selected */

   gmtl::Matrix44f   mNavMatrix;                   /**< Navigation matrix: world_M_virtualworld */

   gadget::DigitalInterface     mLoggerPlayButton;   // Playback log file when pressed

   unsigned                mFramesToSleep;         /**< Number of frames to sleep at start up */
   vrj::test::TestRunner*  mTestRunner;            /**< Test runner for this appliation */
};


#endif /* _INTERACT_APP_H_ */
