/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2005 by Iowa State University
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

#ifndef _SPEECH_RECOG_APP
#define _SPEECH_RECOG_APP

#include <vrj/vrjConfig.h>

#include <iostream>
#include <iomanip>
#include <GL/gl.h>

#include <vrj/Draw/OGL/GlApp.h>
#include <vpr/Util/Interval.h>

#include <gadget/Type/PositionInterface.h>
#include <gadget/Type/DigitalInterface.h>
#include <gadget/Type/CommandInterface.h>
#include <gadget/Type/StringInterface.h>

using namespace vrj;

// Utility function to draw a cube
void drawbox( GLdouble x0, GLdouble x1, GLdouble y0, GLdouble y1,
              GLdouble z0, GLdouble z1, GLenum type );

/**
 * Simple OGL Demonstration application.
 *
 * This application intialized and recieves positional
 * and digital intput from the wand.
 *
 * It also has basic code that draws a box centered on
 * the origin.
 */
class SpeechRecogApp : public GlApp
{
public:
   SpeechRecogApp() : mLastRecString(""), mLastRecDigital(0)
   {;}

   virtual ~SpeechRecogApp (void) {;}

public: // ---- INITIALIZATION FUNCTIONS ---- //
   /**
    * Executes any initialization needed before the API is started.
    *
    * @post Device interfaces are initialized with the device names
    *       we want to use.
    * @note This is called once before OpenGL is initialized.
    */
   virtual void init()
   {
      std::cout << "---------- App:init() ---------------" << std::endl;
      // Initialize devices
      mWand.init("VJWand");
      mHead.init("VJHead");
      mButton0.init("VJButton0");
      mButton1.init("VJButton1");
      mSpRecDigital.init("MSFTSpeechRecogDigital");
      mSpRecString.init("MSFTSpeechRecogString");

      //Setup Recognition Responses
      mRecogComputer.mMaxTime = mRecogHello.mMaxTime = mRecogGoodbye.mMaxTime = 0.5;
      mRecogComputer.mBoxColor.set(1.0,0.0,0.0);
      mRecogHello.mBoxColor.set(0.0,1.0,0.0);
      mRecogGoodbye.mBoxColor.set(0.0,0.0,1.0);
      mRecogComputer.mRecogString = "Computer";
      mRecogHello.mRecogInt = 1;
      mRecogGoodbye.mRecogInt = 2;
   }

   /**
    * Executes any initialization needed after API is started but before the
    * drawManager starts the drawing loops.
    *
    * This is called once after OGL is initialized.
    */
   virtual void apiInit()
   {;}

public:
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
   //       UpdateDevices();
   //  }
   //------------------------------------

   /**
    * Function that is called upon entry into a buffer of an OpenGL context
    * (window).
    *
    * @note This function is designed to be used when you want to do something
    *       only once per buffer (ie.once for left buffer, once for right
    *       buffer).
    */
   virtual void bufferPreDraw();

   /**
    * Called before start of frame.
    *
    * @note Function called after device updates but before start of drawing.
    */
   virtual void preFrame()
   {
      //Get the elapsed time since last preframe
      vpr::Interval now = mButton0->getTimeStamp();
      float elps( (now - mLastFrameTime).secf() );
      mLastFrameTime = now;

      //Check for recognitions
      std::string tempStr(mSpRecString->getData());
      if(tempStr != "")
      {
         std::cout << "Recognized String " << tempStr << std::endl;
         if(tempStr == mRecogComputer.mRecogString){mRecogComputer.mIsRecognized = true; mRecogComputer.mElpsTime = 0.0;}
      }
      int tempInt(mSpRecDigital->getData());
      if(tempInt != 0)
      {
         std::cout << "Recognized Digital " << tempInt << std::endl;
         if(tempInt == mRecogHello.mRecogInt){mRecogHello.mIsRecognized = true; mRecogHello.mElpsTime = 0.0;}
         else if(tempInt == mRecogGoodbye.mRecogInt){mRecogGoodbye.mIsRecognized = true; mRecogGoodbye.mElpsTime = 0.0;}
      }

      //Update elapsed time
      if(mRecogComputer.mIsRecognized)
      {
         mRecogComputer.mElpsTime += elps;
         if(mRecogComputer.mElpsTime >= mRecogComputer.mMaxTime) { mRecogComputer.mIsRecognized = false; }
      }
      if(mRecogHello.mIsRecognized)
      {
         mRecogHello.mElpsTime += elps;
         if(mRecogHello.mElpsTime >= mRecogHello.mMaxTime) { mRecogHello.mIsRecognized = false; }
      }
      if(mRecogGoodbye.mIsRecognized)
      {
         mRecogGoodbye.mElpsTime += elps;
         if(mRecogGoodbye.mElpsTime >= mRecogGoodbye.mMaxTime) { mRecogGoodbye.mIsRecognized = false; }
      }
   }

   /**
    * Called during the Frame.
    *
    * @note Function called after drawing has been triggered but BEFORE it
    *       completes.
    */
   virtual void intraFrame() {;}

   /**
    * Called at end of frame.
    *
    * @note Function called before updating trackers but after the frame is
    *       drawn.
    */
   virtual void postFrame() {;}

public: // ----- OpenGL FUNCTIONS ---- //
   /**
    * Function that is called immediately after a new OGL context is created.
    * Initialize GL state here. Also used to create context specific
    * information.
    *
    * This is called once for each context.
    */
   virtual void contextInit()
   {
      initGLState();       // Initialize the GL state information. (lights, shading, etc)
   }

   /**
    * Function to draw the scene.
    *
    * @pre OpenGL state has correct transformation and buffer selected.
    * @post The current scene has been drawn.
    *
    * @note Called 1 or more times per frame.
    */
   virtual void draw();

private:
   void initGLState();

   void drawCube()
   {
      drawbox(-0.5, 0.5, -0.5, 0.5, -0.5, 0.5, GL_QUADS);
   }

private:
   gadget::PositionInterface  mWand;    /**< Positional interface for Wand position */
   gadget::PositionInterface  mHead;    /**< Positional interface for Head position */
   gadget::DigitalInterface   mButton0; /**< Digital interface for button 0 */
   gadget::DigitalInterface   mButton1; /**< Digital interface for button 1 */
   gadget::CommandInterface   mSpRecDigital;
   gadget::StringInterface    mSpRecString;

   std::string mLastRecString;
   int mLastRecDigital;

   struct RecogResponse
   {
      bool mIsRecognized;
      int mRecogInt;
      std::string mRecogString;
      gmtl::Vec3f mBoxColor;
      float mElpsTime;
      float mMaxTime;
      
      RecogResponse() : mIsRecognized(false),
                        mRecogInt(0),
                        mRecogString(""),
                        mBoxColor(0.0,0.0,0.0),
                        mElpsTime(0.0),
                        mMaxTime(0.0)
      {}
   };
   RecogResponse mRecogComputer;
   RecogResponse mRecogHello;
   RecogResponse mRecogGoodbye;
   
   vpr::Interval mLastFrameTime;

};


#endif
