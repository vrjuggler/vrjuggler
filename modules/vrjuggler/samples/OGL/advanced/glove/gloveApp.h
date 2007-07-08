/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2007 by Iowa State University
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

#ifndef _GLOVE_APP_
#define _GLOVE_APP_

#include <vrj/vrjConfig.h>

#include <gmtl/Matrix.h>
#include <gmtl/Vec.h>

#include <vrj/Draw/OpenGL/App.h>
#include <vrj/Draw/OpenGL/ContextData.h>
#include <gadget/Type/GloveInterface.h>
#include <gadget/Type/PositionInterface.h>
#include <gadget/Type/DigitalInterface.h>

#include "defines.h"
#include "Scene.h"
#include "navigation.h"


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
//
// NOTE: this application has a really BAD example of navigation.
// the nav is a hack, and well, it works for this silly little app.
// be warned, it (navigation.h) won't be fun to extend

class gloveApp : public vrj::opengl::App
{
// utility functions
protected:
    //friend gmtl::Vec3f operator*( const Matrix& m, const gmtl::Vec3f& v );

// Application Functions:
public:
   gloveApp(vrj::Kernel* kern)
      : vrj::opengl::App(kern)
      , mCubeSelected(false)
      , mSphereSelected(false)
      , mConeSelected(false)
      , mCubePos(0.0f, 3.5f, -20.0f)
      , mConePos(-2.5f, 3.5f, -20.0f)
      , mSpherePos(2.5f, 3.5f, -20.0f)
   {
      // Do nothing
   }

   virtual ~gloveApp(){}

   //: Initialize
   // Executes any initialization needed before the API is started.
   // Initialize VR Juggler device interfaces here.
   virtual void init()
   {
      // for the glove position
      mGlove.init("VJGlove");

      // for the glove fingers.
      mPinchLeftThumb.init( "LeftThumb" );
      mPinchLeftIndex.init( "LeftIndex" );
      mPinchLeftMiddle.init( "LeftMiddle" );
      mPinchLeftRing.init( "LeftRing" );
      mPinchLeftPinky.init( "LeftPinky" );
      mPinchRightThumb.init( "RightThumb" );
      mPinchRightIndex.init( "RightIndex" );
      mPinchRightMiddle.init( "RightMiddle" );
      mPinchRightRing.init( "RightRing" );
      mPinchRightPinky.init( "RightPinky" );

      //mGloveTracker.init("GlovePos Proxy");
   }

   // Called immediately upon opening a new OpenGL context.  This is called
   // for every display window that is opened.  Put your OpenGL resource
   // allocation here.
   virtual void contextInit()
   {
      // Init the scene's displaylists for this context.
      mScene->init();
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
      // Do nothing
   }

   // Function called after tracker update but before start of drawing.  Do
   // calculations and state modifications here.
   // In the glove application, this function does the logic for picking the
   // objects.
   virtual void preFrame()
   {
      // Do nothing
   }

   // Function to draw the scene.  Put OpenGL draw functions here.
   //
   // PRE: OpenGL state has correct transformation and buffer selected
   // POST: The current scene has been drawn
   virtual void draw()
   {
      myDraw();
   }

   // Function called after drawing has been triggered but BEFORE it completes
   virtual void intraFrame()
   {
      // Do nothing
   }

   // Function called before updating trackers but after the frame is drawn.
   // Do calculations here.
   virtual void postFrame();

private:
   void initGlState();
   void renderLightsAndMaterials();
   void myDraw();

protected:
   // for the glove position
   gadget::GloveInterface    mGlove;

   // for the glove fingers
   gadget::DigitalInterface  mPinchLeftThumb;
   gadget::DigitalInterface  mPinchLeftIndex;
   gadget::DigitalInterface  mPinchLeftMiddle;
   gadget::DigitalInterface  mPinchLeftRing;
   gadget::DigitalInterface  mPinchLeftPinky;
   gadget::DigitalInterface  mPinchRightThumb;
   gadget::DigitalInterface  mPinchRightIndex;
   gadget::DigitalInterface  mPinchRightMiddle;
   gadget::DigitalInterface  mPinchRightRing;
   gadget::DigitalInterface  mPinchRightPinky;


   //: Object selection
   bool mCubeSelected;
   bool mSphereSelected;
   bool mConeSelected;

   //: Object positions
   gmtl::Vec3f     mCubePos;
   gmtl::Vec3f     mConePos;
   gmtl::Vec3f     mSpherePos;

   gmtl::Matrix44f mNavigation;

   vrj::opengl::ContextData<Scene> mScene;
};


#endif
