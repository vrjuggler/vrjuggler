/*
 * VRJuggler
 *   Copyright (C) 1997,1998,1999,2000
 *   Iowa State University Research Foundation, Inc.
 *   All Rights Reserved
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
 */

#ifndef _NULL_APP_
#define _NULL_APP_

#include <math.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <iostream.h>

#include <Kernel/GL/vjGlApp.h>
#include <Kernel/GL/vjGlContextData.h>
#include <Kernel/vjDebug.h>

#include <Math/vjMatrix.h>
#include <Math/vjVec3.h>
#include <Math/vjQuat.h>
#include <Kernel/vjDebug.h>

#include <Input/InputManager/vjPosInterface.h>
#include <Input/InputManager/vjAnalogInterface.h>
#include <Input/InputManager/vjDigitalInterface.h>

class UserData
{
public:
   UserData()
   {
      firstTime = true;
      cubeDLIndex = -1;
   }

public:
   bool  firstTime;
   int   cubeDLIndex;
};


//--------------------------------------------------
// Demonstration OpenGL application class
//
// This application simply renders a field of cubes.
//---------------------------------------------------
class nullApp : public vjGlApp
{
public:
   nullApp(vjKernel* kern) : vjGlApp(kern)
   {
      mNavMatrix.makeIdent();
   }

   // Execute any initialization needed before the API is started
   virtual void init()
   {
      cout << "---------- App:init() ---------------" << endl;
         // Initialize devices
      mWand.init("VJWand");
      mIncVelocityButton.init("VJButton0");
      mDecVelocityButton.init("VJButton1");
      mStopButton.init("VJButton2");
   }

   // Execute any initialization needed <b>after</b> API is started
   //  but before the drawManager starts the drawing loops.
   virtual void apiInit()
   {
      ;
   }

   // Called immediately upon opening a new OpenGL context
   virtual void contextInit()
   {
      // Create display list
      vjASSERT(mDlData->firstTime == true);   // We should not have been here yet
      mDlData->firstTime = false;

      glGenLists(rand()%25);        // Generate some random lists.  NOTE: Needed for testing only
      mDlData->cubeDLIndex = glGenLists(1);


   }

   /** Function to draw the scene
    * PRE: OpenGL state has correct transformation and buffer selected
    * POST: The current scene has been drawn
    */
   virtual void draw()
   {
       ;
   }

   /**   name Drawing Loop Functions
    *
    *  The drawing loop will look similar to this:
    *
    *  while (drawing)
    *  {
    *        preDraw();
    *	      draw();
    *	       postDraw();      // Drawing is happening while here
    *       sync();
    *        postSync();      // Drawing is now done
    *
    *	      UpdateTrackers();
    *  }
    *
    */

   /// Function called after tracker update but before start of drawing
   virtual void preDraw()
   {
       ;
   }

   /// Function called after drawing has been triggered but BEFORE it completes
   virtual void postDraw()
   {
       ;
   }

   /// Function called before updating trackers but after the frame is drawn
    virtual void postSync() {
	;
    }


private:

   void initGLState()
   {
       ;
   }


public:
   vjPosInterface       mWand;      // the Wand
   vjPosInterface       mWandEnd;   // The index to the end of the wand
   vjDigitalInterface   mIncVelocityButton;   // Button for velocity
   vjDigitalInterface   mDecVelocityButton;
   vjDigitalInterface   mStopButton;         // Button to stop

   float                      mCurVelocity;  // The current velocity
   vjMatrix                   mNavMatrix;    // Matrix for navigation in the application
   vjGlContextData<UserData>  mDlData;       // Data for display lists

};

#endif
