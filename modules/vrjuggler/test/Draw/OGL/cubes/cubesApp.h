/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998, 1999, 2000 by Iowa State University
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

#ifndef _CUBES_APP_
#define _CUBES_APP_

#include <vrj/vrjConfig.h>

#ifdef VJ_OS_Darwin
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#endif

#include <vector>

#include <vrj/Kernel/Kernel.h>
#include <vrj/Draw/OGL/GlApp.h>
#include <vrj/Draw/OGL/GlContextData.h>

#include <vrj/Math/Matrix.h>
#include <vrj/Math/Vec3.h>
#include <vrj/Util/Debug.h>

#include <vrj/Input/Type/PosInterface.h>
#include <vrj/Input/Type/AnalogInterface.h>
#include <vrj/Input/Type/DigitalInterface.h>

#include <vrj/Kernel/User.h>



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

// Class to hold all data for a specific user
class UserData
{
public:
   // Constructor
   // Takes the string names of the devices to use
   // NOTE: This means that we cannot construct a user until the input manager is loaded
   //       Ex. The Init function
   UserData(vrj::User* user, std::string wandName, std::string incButton,
            std::string decButton, std::string stopButton)
   {
      mCurVelocity = 0.0;
      mNavMatrix.makeIdent();

      mUser = user;
      // Initialize devices
      mWand.init(wandName);
      mIncVelocityButton.init(incButton);
      mDecVelocityButton.init(decButton);
      mStopButton.init(stopButton);
   }

   // Update the navigation matrix
   //
   // Uses a quaternion to do rotation in the environment
   void updateNavigation();

public:
      // Devices to use for the given user
   vrj::PosInterface       mWand;                  // the Wand
   vrj::DigitalInterface   mIncVelocityButton;     // Button for velocity
   vrj::DigitalInterface   mDecVelocityButton;
   vrj::DigitalInterface   mStopButton;            // Button to stop

      // Navigation info for the user
   float                mCurVelocity;  // The current velocity
   vrj::Matrix          mNavMatrix;    // Matrix for navigation in the application

   vrj::User*           mUser;         // The user we hold data for
};

//--------------------------------------------------
// Demonstration OpenGL application class
//
// This application simply renders a field of cubes.
//---------------------------------------------------
class cubesApp : public vrj::GlApp
{
public:
   cubesApp(vrj::Kernel* kern) : vrj::GlApp(kern)
   {
      ;
   }

   virtual ~cubesApp() {}

   // Execute any initialization needed before the API is started.  Put device
   // initialization here.
   virtual void init();

   // Execute any initialization needed <b>after</b> API is started
   //  but before the drawManager starts the drawing loops.
   virtual void apiInit()
   {
      vprDEBUG(vrjDBG_ALL,0) << "---- cubesApp::apiInit() ----\n" << vprDEBUG_FLUSH;
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
       vprDEBUG(vrjDBG_ALL,5) << "cubesApp::preFrame()" << std::endl
                            << vprDEBUG_FLUSH;

       for(unsigned int i=0;i<mUserData.size();i++)
          mUserData[i]->updateNavigation();       // Update the navigation matrix
   }

   // Function to draw the scene.  Put OpenGL draw functions here.
   //
   // PRE: OpenGL state has correct transformation and buffer selected
   // POST: The current scene has been drawn
   virtual void draw()
   {
      initGLState();    // This should really be in another function

      myDraw(vrj::GlDrawManager::instance()->currentUserData()->getUser());
   }

   // Clear the buffer each frame
   virtual void bufferPreDraw()
   {
      glClearColor(0.0, 0.0, 0.0, 0.0);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   }

   /// Function called after drawing has been triggered but BEFORE it completes
   virtual void intraFrame()
   {
      vprDEBUG(vrjDBG_ALL,5) << "cubesApp::intraFrame()" << std::endl
                           << vprDEBUG_FLUSH;
   }

   // Function called before updating trackers but after the frame is drawn.
   // Do calculations here.
   virtual void postFrame()
   {
      vprDEBUG(vrjDBG_ALL,5) << "cubesApp::postFrame" << std::endl
                           << vprDEBUG_FLUSH;
   }

   //: Make sure that all our dependencies are satisfied
   // Make sure that there are Users registered with the system
   virtual bool depSatisfied()
   {
      // We can't start until there are users registered wth the system
      // We rely upon users to keep track of the multi-user data structure
      int num_users = vrj::Kernel::instance()->getUsers().size();
      return (num_users > 0);
   }

private:

   //----------------------------------------------
   //  Draw the scene.  A bunch of boxes of differing color and stuff
   //----------------------------------------------
   void myDraw(vrj::User* user);
   void initGLState();

   void drawCube()
   {
       glCallList(mDlData->dlIndex);
       //drawbox(-1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f, GL_QUADS);
   }

   void drawbox(GLdouble x0, GLdouble x1, GLdouble y0, GLdouble y1,
                GLdouble z0, GLdouble z1, GLenum type);

public:
   vrj::GlContextData<ContextData>  mDlData;      // Data for display lists
   vrj::GlContextData<ContextData>  mDlDebugData; // Data for debugging display lists
   std::vector<UserData*>        mUserData;     // All the users in the program
};


#endif
