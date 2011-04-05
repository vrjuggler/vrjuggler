/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2011 by Iowa State University
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
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

#ifndef _APPLICATION_BARRIER_APP_
#define _APPLICATION_BARRIER_APP_

#include <vrj/vrjConfig.h>
#include <vpr/vpr.h>

#ifdef VPR_OS_Darwin
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#endif

#include <vector>

#include <vrj/Kernel/Kernel.h>
#include <vrj/Draw/OpenGL/App.h>
#include <vrj/Draw/OpenGL/ContextData.h>

#include <gmtl/Matrix.h>
#include <gmtl/MatrixOps.h>
#include <vrj/Util/Debug.h>

#include <gadget/Type/PositionInterface.h>
#include <gadget/Type/AnalogInterface.h>
#include <gadget/Type/DigitalInterface.h>
#include <plugins/ApplicationBarrierManager/ApplicationBarrier.h>

#include <vrj/Kernel/UserPtr.h>

#include <vpr/Util/DurationStatCollector.h>
#include <vpr/Perf/ProfileManager.h>
#include <vpr/Thread/Thread.h>


// Class to hold all context specific data
class ContextData
{
public:
   ContextData()
   {
      dlIndex  = -1;
      maxIndex = -1;
   }

public:
   int   dlIndex;
   int   maxIndex;     // For debugging purposes only!
};

class ContextTimingData
{
public:
   ContextTimingData()
      : dlist_wait(vpr::Interval::Usec, 400000)
   {;}

   vpr::DurationStatCollector dlist_wait;
};

// Class to hold all data for a specific user
class UserData
{
public:
   enum ShapeSetting
   {
      CUBE,
      CONE
   };

   // Constructor
   // Takes the string names of the devices to use
   // NOTE: This means that we cannot construct a user until the input manager is loaded
   //       Ex. The Init function
   UserData(vrj::UserPtr user, std::string wandName, std::string incButton,
            std::string decButton, std::string stopButton,
            std::string toggleButton)
      : mShapeSetting(CUBE)
   {
      mCurVelocity = 0.0;
      gmtl::identity(mNavMatrix);

      mUser = user;
      // Initialize devices
      mWand.init(wandName);
      mIncVelocityButton.init(incButton);
      mDecVelocityButton.init(decButton);
      mStopButton.init(stopButton);
      mToggleButton.init(toggleButton);
   }

   // Update the navigation matrix
   //
   // Uses a quaternion to do rotation in the environment
   void updateNavigation();

   void updateShapeSetting();

   ShapeSetting getShapeSetting()
   {
      return mShapeSetting;
   }

public:
      // Devices to use for the given user
   gadget::PositionInterface mWand;              /**< The wand */
   gadget::DigitalInterface  mIncVelocityButton; /**< Button for velocity */
   gadget::DigitalInterface  mDecVelocityButton;
   gadget::DigitalInterface  mStopButton;        /**< Button to stop */
   gadget::DigitalInterface  mToggleButton;      /**< Button to toggle shape */

   // Navigation info for the user
   float                mCurVelocity;  // The current velocity
   gmtl::Matrix44f      mNavMatrix;    // Matrix for navigation in the application

   vrj::UserPtr         mUser;         // The user we hold data for

   ShapeSetting mShapeSetting; // Flag identifying the shape to render
};

//--------------------------------------------------
// Demonstration OpenGL application class
//
// This application renders a field of cubes and tests the ApplicationBarrier.
//---------------------------------------------------
class applicationBarrierApp : public vrj::opengl::App
{
public:
   applicationBarrierApp(vrj::Kernel* kern)
      : vrj::opengl::App(kern)
      , mCurFrameNum(0)
   {
      /* Do nothing. */ ;
   }

   virtual ~applicationBarrierApp()
   {
      mWorkThread->kill();
   }

   // Execute any initialization needed before the API is started.  Put device
   // initialization here.
   virtual void init();

   // Execute any initialization needed <b>after</b> API is started
   //  but before the drawManager starts the drawing loops.
   virtual void apiInit()
   {
      vprDEBUG(vprDBG_ALL, vprDBG_VERB_LVL)
         << "---- applicationBarrierApp::apiInit() ----\n" << vprDEBUG_FLUSH;
   }

   // Called immediately upon opening a new OpenGL context.  This is called
   // once for every display window that is opened.  Put OpenGL resource
   // allocation here.
   virtual void contextInit();

   // Called immediately upon closing an OpenGL context
   // (called for every window that is closed)
   // put your opengl deallocation here...
   virtual void contextClose();

   /**   name Drawing Loop Functions
    *
    *  The drawing loop will look similar to this:
    *
    *  while (drawing)
    *  {
    *        preFrame();
    *       draw();
    *        intraFrame();      // Drawing is happening while here
    *       sync();
    *        postFrame();      // Drawing is now done
    *
    *       UpdateTrackers();
    *  }
    *
    */

   // Function called after tracker update but before start of drawing.  Do
   // calculations and state modifications here.
   virtual void preFrame()
   {
      VPR_PROFILE_GUARD("applicationBarrierApp::preFrame");
       vprDEBUG(vprDBG_ALL, vprDBG_HVERB_LVL) << "applicationBarrierApp::preFrame()"
                                              << std::endl << vprDEBUG_FLUSH;

       for ( unsigned int i = 0; i < mUserData.size(); ++i )
       {
          mUserData[i]->updateShapeSetting();
          mUserData[i]->updateNavigation();     // Update the navigation matrix
       }

       ++mCurFrameNum;     // Goto next frame
   }

   virtual void pipePreDraw()
   {
      if((mCurFrameNum % 50) == 0)
      { 
         ContextTimingData* timing_data = &(*mContextTiming);
         double mean = timing_data->dlist_wait.getMean();
         vprDEBUG(vprDBG_ALL, vprDBG_VERB_LVL) << "dlist wait: " << mean
                                               << std::endl << vprDEBUG_FLUSH;
      }
   }

   // Function to draw the scene.  Put OpenGL draw functions here.
   //
   // PRE: OpenGL state has correct transformation and buffer selected
   // POST: The current scene has been drawn
   virtual void draw()
   {
      VPR_PROFILE_GUARD("applicationBarrierApp::draw");
      glClear(GL_DEPTH_BUFFER_BIT);
      initGLState();    // This should really be in another function

      myDraw(vrj::opengl::DrawManager::instance()->currentUserData()->getUser());
   }

   // Clear the buffer each frame.
   virtual void bufferPreDraw()
   {
      glClearColor(0.0, 0.0, 0.0, 0.0);
      glClear(GL_COLOR_BUFFER_BIT);
   }

   /// Function called after drawing has been triggered but BEFORE it completes.
   virtual void intraFrame()
   {
      VPR_PROFILE_GUARD("applicationBarrierApp::intraFrame");
      vprDEBUG(vprDBG_ALL, vprDBG_HVERB_LVL) << "applicationBarrierApp::intraFrame()" << std::endl
                           << vprDEBUG_FLUSH;
   }

   // Function called before updating trackers but after the frame is drawn.
   // Do calculations here.
   virtual void postFrame()
   {
      vprDEBUG(vprDBG_ALL, vprDBG_HVERB_LVL)
         << "applicationBarrierApp::postFrame" << std::endl << vprDEBUG_FLUSH;
   }

   // Make sure that all our dependencies are satisfied.
   // Make sure that there are Users registered with the system.
   virtual bool depSatisfied()
   {
      // We can't start until there are users registered wth the system
      // We rely upon users to keep track of the multi-user data structure
      size_t num_users = vrj::Kernel::instance()->getUsers().size();
      return (num_users > 0);
   }

   virtual void exit();

private:

   //----------------------------------------------
   //  Draw the scene.  A bunch of boxes of differing color and stuff
   //----------------------------------------------
   void myDraw(vrj::UserPtr user);
   void initGLState();

   void drawCube()
   {
      glCallList(mDlCubeData->dlIndex);
      //drawbox(-1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f, GL_QUADS);
   }

   void drawbox(GLdouble x0, GLdouble x1, GLdouble y0, GLdouble y1,
                GLdouble z0, GLdouble z1, GLenum type);

   void drawCone()
   {
      glCallList(mDlConeData->dlIndex);
//      drawCone(1.5f, 2.0f, 20, 10);
   }

   /**
    * @pre This is invoked while an OpenGL context is active.
    */
   void drawCone(GLdouble base, GLdouble height, GLint slices, GLint stacks)
   {
      gluCylinder(*mConeQuad, base, 0.0f, height, slices, stacks);
      gluDisk(*mBaseQuad, 0.0f, base, slices, 1);
   }

public:
   vrj::opengl::ContextData<ContextData> mDlCubeData;  // Data for cube display lists
   vrj::opengl::ContextData<ContextData> mDlConeData;  // Data for cone display lists
   vrj::opengl::ContextData<ContextData> mDlDebugData; // Data for debugging display lists
   vrj::opengl::ContextData<GLUquadric*> mConeQuad;
   vrj::opengl::ContextData<GLUquadric*> mBaseQuad;
   std::vector<UserData*>           mUserData;    // All the users in the program
   vpr::TSObjectProxy<ContextTimingData>  mContextTiming;

   long                             mCurFrameNum;     // Current frame number count

private:
   vpr::Thread *mWorkThread;
};


#endif
