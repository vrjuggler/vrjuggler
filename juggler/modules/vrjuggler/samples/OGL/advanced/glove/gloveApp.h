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

#ifndef _GLOVE_APP_
#define _GLOVE_APP_

#include <vjConfig.h>

#include <Kernel/GL/vjGlApp.h>
#include <Math/vjMatrix.h>
#include <Math/vjVec3.h>

#include <Kernel/GL/vjGlContextData.h>
#include <Input/InputManager/vjGloveInterface.h>
#include <Input/InputManager/vjGestureInterface.h>
#include <Input/InputManager/vjPosInterface.h>
#include <Input/InputManager/vjAnalogInterface.h>
#include <Input/InputManager/vjDigitalInterface.h>

#include <navigator.h>
#include <collidor.h>
#include <vjStdCaveNavigator.h>

#include "defines.h"
#include "Scene.h"
//#include "navigation.h"


//: GloveApp - A Demonstration OpenGL application class
// This application has a table with objects to grab
//
// ----- Drawing Loop Functions ------
//
//  The drawing loop will look similar to this:
//
//  while (drawing)
//  {
//        preFrame();
//       draw();
//        intraFrame();      // Drawing is happening while here
//       sync();
//        postFrame();      // Drawing is now done
//
//       UpdateTrackers();
//  }
class gloveApp : public vjGlApp
{
// utility functions
protected:
    //friend vjVec3 operator*( const vjMatrix& m, const vjVec3& v );

// Application Functions:
public:
   gloveApp(vjKernel* kern) : vjGlApp(kern),
                                mCubeSelected(false),
                                mSphereSelected(false),
                                mConeSelected(false),
            mCubePos(0.0f, 3.5f, -20.0f),
            mConePos(-2.5f, 3.5f, -20.0f),
            mSpherePos(2.5f, 3.5f, -20.0f)
   {
      /* Do nothing */ ;
   }

   //: Initialize
   // Executes any initialization needed before the API is started
   virtual void init()
   {
      // for the glove position
      mGlove.init("VJGlove");
      
      // for the glove fingers.
      mPinchLeftThumb.init("PinchLeftThumb");
      mPinchLeftIndex.init("PinchLeftIndex");
      mPinchLeftMiddle.init("PinchLeftMiddle");
      mPinchLeftRing.init("PinchLeftRing");
      mPinchLeftPinky.init("PinchLeftPinky");
      mPinchRightThumb.init("PinchRightThumb");
      mPinchRightIndex.init("PinchRightIndex");
      mPinchRightMiddle.init("PinchRightMiddle");
      mPinchRightRing.init("PinchRightRing");
      mPinchRightPinky.init("PinchRightPinky");

      //mGloveTracker.init("GlovePos Proxy");
   }
  
   //: is the glove pointing?
   bool LeftPointing();

   //: is the the glove open?
   bool LeftOpen();

   bool RightPointing();

   bool RightOpen();

   bool LeftFist();

   bool RightFist();

   //: API Init
   //  Executes any initialization needed after API is started
   //  but before the drawManager starts the drawing loops.
   virtual void apiInit()
   {
      /* Do nothing. */ ;
   }

   //: Function to draw the scene
   //! PRE: OpenGL state has correct transformation and buffer selected
   //! POST: The current scene has been drawn
   virtual void draw()
   {
      myDraw();
   }

   /// Function called before updating trackers but after the frame is drawn
   virtual void postFrame()
   {
      /* Do nothing. */ ;
   }

   //: Function called after tracker update but before start of drawing
   //  In the glove application, this function does the logic for picking the objects.
   virtual void preFrame();

   /// Function called after drawing has been triggered but BEFORE it completes
   virtual void intraFrame()
   {
      /* Do nothing. */
   }

   virtual void contextInit()
   {
      // Init the scene's displaylists for this context.
      mScene->init();
   }

private:
    void initGlState();
    void renderLightsAndMaterials();
    void myDraw();

protected:
   // for the glove position
   vjGloveInterface    mGlove;

   // for the glove fingers
   vjDigitalInterface  mPinchLeftThumb;
   vjDigitalInterface  mPinchLeftIndex;
   vjDigitalInterface  mPinchLeftMiddle;
   vjDigitalInterface  mPinchLeftRing;
   vjDigitalInterface  mPinchLeftPinky;
   vjDigitalInterface  mPinchRightThumb;
   vjDigitalInterface  mPinchRightIndex;
   vjDigitalInterface  mPinchRightMiddle;
   vjDigitalInterface  mPinchRightRing;
   vjDigitalInterface  mPinchRightPinky;
   
   
   //: Object selection
   bool                mCubeSelected;
   bool                mSphereSelected;
   bool                mConeSelected;

   //: Object positions
   vjVec3               mCubePos;
   vjVec3               mConePos;
   vjVec3               mSpherePos;

   vjStdCaveNavigator    mNavigation;

   vjGlContextData<Scene> mScene;
};


#endif
