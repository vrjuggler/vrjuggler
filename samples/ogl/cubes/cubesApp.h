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

#include <vjConfig.h>

#include <GL/gl.h>
#include <GL/glu.h>
#include <vector>

#include <Kernel/GL/vjGlApp.h>
#include <Kernel/GL/vjGlContextData.h>
#include <Math/vjMatrix.h>
#include <Math/vjVec3.h>
#include <Kernel/vjDebug.h>

#include <Input/InputManager/vjPosInterface.h>
#include <Input/InputManager/vjAnalogInterface.h>
#include <Input/InputManager/vjDigitalInterface.h>

#include <Kernel/vjUser.h>



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
   UserData(vjUser* user, std::string wandName, std::string incButton,
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
   vjPosInterface       mWand;                  // the Wand
   vjDigitalInterface   mIncVelocityButton;     // Button for velocity
   vjDigitalInterface   mDecVelocityButton;
   vjDigitalInterface   mStopButton;            // Button to stop

      // Navigation info for the user
   float                mCurVelocity;  // The current velocity
   vjMatrix             mNavMatrix;    // Matrix for navigation in the application

   vjUser*              mUser;         // The user we hold data for
};

//--------------------------------------------------
// Demonstration OpenGL application class
//
// This application simply renders a field of cubes.
//---------------------------------------------------
class cubesApp : public vjGlApp
{
public:
   cubesApp(vjKernel* kern) : vjGlApp(kern)
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
      vjDEBUG(vjDBG_ALL,0) << "---- cubesApp::apiInit() ----\n" << vjDEBUG_FLUSH;
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
       vjDEBUG(vjDBG_ALL,5) << "cubesApp::preFrame()" << std::endl
                            << vjDEBUG_FLUSH;

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

      myDraw(vjGlDrawManager::instance()->currentUserData()->getUser());
   }

   /// Function called after drawing has been triggered but BEFORE it completes
   virtual void intraFrame()
   {
      vjDEBUG(vjDBG_ALL,5) << "cubesApp::intraFrame()" << std::endl
                           << vjDEBUG_FLUSH;
   }

   // Function called before updating trackers but after the frame is drawn.
   // Do calculations here.
   virtual void postFrame()
   {
      vjDEBUG(vjDBG_ALL,5) << "cubesApp::postFrame" << std::endl
                           << vjDEBUG_FLUSH;
   }

   //: Make sure that all our dependencies are satisfied
   // Make sure that there are vjUsers registered with the system 
   virtual bool depSatisfied()
   {
      // We can't start until there are users registered wth the system
      // We rely upon users to keep track of the multi-user data structure
      int num_users = vjKernel::instance()->getUsers().size();
      return (num_users > 0);
   }

private:

   //----------------------------------------------
   //  Draw the scene.  A bunch of boxes of differing color and stuff
   //----------------------------------------------
   void myDraw(vjUser* user);
   void initGLState();

   void drawCube()
   {
       glCallList(mDlData->dlIndex);
       //drawbox(-1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f, GL_QUADS);
   }

   void drawbox(GLdouble x0, GLdouble x1, GLdouble y0, GLdouble y1,
                GLdouble z0, GLdouble z1, GLenum type);

public:
   vjGlContextData<ContextData>  mDlData;       // Data for display lists
   vjGlContextData<ContextData>  mDlDebugData;  // Data for debugging display lists
   std::vector<UserData*>        mUserData;     // All the users in the program
};


#endif
