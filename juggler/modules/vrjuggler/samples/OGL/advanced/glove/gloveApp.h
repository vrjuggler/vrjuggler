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
      // Initialize devices
      mGlove.init("VJGlove");
      mGesture.init("VJGesture");
      //mGloveTracker.init("GlovePos Proxy");
   }

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
   //: VR Juggler devices
   vjGloveInterface    mGlove;
   vjGestureInterface  mGesture;
   //vjPosInterface      mGloveTracker;

   //: Object selection
   bool                mCubeSelected;
   bool                mSphereSelected;
   bool                mConeSelected;

   //: Object positions
   vjVec3               mCubePos;
   vjVec3               mConePos;
   vjVec3               mSpherePos;

   vjMatrix    mNavigation;

   vjGlContextData<Scene> mScene;
};


#endif
