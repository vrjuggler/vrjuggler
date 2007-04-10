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

#ifndef _RECONFIG_APP
#define _RECONFIG_APP

#include <vrj/vrjConfig.h>

#if defined(__APPLE__)
#  include <OpenGL/gl.h>
#  include <OpenGL/glu.h>
#else
#  include <GL/gl.h>
#  include <GL/glu.h>
#endif

#include <jccl/Config/ConfigElementPtr.h>
#include <jccl/Config/Configuration.h>

#include <gadget/Type/PositionInterface.h>
#include <gadget/Type/AnalogInterface.h>
#include <gadget/Type/DigitalInterface.h>

#include <vrj/Draw/OGL/GlApp.h>
#include <vrj/Kernel/Kernel.h>


class reconfigApp : public vrj::GlApp
{
public:
   reconfigApp(vrj::Kernel* kern)
      : vrj::GlApp(kern)
      , mNewCfg(NULL)
   {
      /* Do nothing. */ ;
   }

   virtual ~reconfigApp()
   {
      /* Do nothing. */ ;
   }

   // Execute any initialization needed before the API is started.  Put device
   // initialization here.
   virtual void init();

   // Called immediately upon opening a new OpenGL context.  This is called
   // once for every display window that is opened.  Put OpenGL resource
   // allocation here.
   virtual void contextInit();

   // Function called after tracker update but before start of drawing.  Do
   // calculations and state modifications here.
   virtual void preFrame();

   virtual void bufferPreDraw();

   // Function to draw the scene.  Put OpenGL draw functions here.
   //
   // PRE: OpenGL state has correct transformation and buffer selected
   // POST: The current scene has been drawn
   virtual void draw();

//UTILITIES

   void setPath(const std::string& path);

private:
   bool addElementFile(const std::string& filename);
   bool removeElementFile(const std::string& filename);
   bool swapElementFiles(const std::string& removeFile,
                         const std::string& addFile);
   bool removeRecentConfiguration();

   bool checkTime();

   bool verifyProxy(const std::string& proxyName,
                    const std::string& deviceName);
   bool verifyViewport(vrj::Viewport* viewport,
                       jccl::ConfigElementPtr viewportElement);
   bool verifyAllViewports(vrj::DisplayPtr display,
                           jccl::ConfigElementPtr viewportElement);

   vrj::DisplayPtr getDisplay(const std::string& name);
   bool verifyDisplayFile(const std::string& filename);
   bool verifyDisplayProps(vrj::DisplayPtr disp, const std::string& name,
                           const int xOrigin, const int yOrigin,
                           const int xSize, const int ySize,
                           const int pipeNum, const bool stereo,
                           const bool border, const bool active);

//TEST SUITE FUNCS
private:
   bool addMachineSpecific_exec();
   bool addMachineSpecific_check();

   bool removeMachineSpecific_exec();
   bool removeMachineSpecific_check();

   bool addGFXWindow_exec();
   bool addGFXWindow_check();

   bool removeGFXWindow_exec();
   bool removeGFXWindow_check();

   bool readdGFXWindow_exec();
   bool readdGFXWindow_check();

   bool resizeGFXWindow_exec();
   bool resizeGFXWindow_check();

   bool moveGFXWindow_exec();
   bool moveGFXWindow_check();

   bool addViewport_exec();
   bool addViewport_check();

   bool removeViewport_exec();
   bool removeViewport_check();

   bool resizeViewport_exec();
   bool resizeViewport_check();

   bool moveViewport_exec();
   bool moveViewport_check();

   bool enableStereoSurface_exec();
   bool enableStereoSurface_check();

   bool disableStereoSurface_exec();
   bool disableStereoSurface_check();

   bool removeKeyboardWin_exec();
   bool removeKeyboardWin_check();

   bool readdKeyboardWin_exec();
   bool readdKeyboardWin_check();

   bool addSimPos_exec();
   bool addSimPos_check();

   bool removeSimPos_exec();
   bool removeSimPos_check();

   bool readdSimPos_exec();
   bool readdSimPos_check();

   bool repointProxy_exec();
   bool repointProxy_check();

   bool reconfigSimPos_exec();
   bool reconfigSimPos_check();

   bool reconfigSimDigital_exec();
   bool reconfigSimDigital_check();

   bool reconfigSimAnalog_exec();
   bool reconfigSimAnalog_check();

   bool addSimDigital_exec();
   bool addSimDigital_check();

   bool removeSimDigital_exec();
   bool removeSimDigital_check();

   bool readdSimDigital_exec();
   bool readdSimDigital_check();

   bool addSimAnalog_exec();
   bool addSimAnalog_check();

   bool removeSimAnalog_exec();
   bool removeSimAnalog_check();

   bool readdSimAnalog_exec();
   bool readdSimAnalog_check();

   bool addPosProxy_exec();
   bool addPosProxy_check();

   bool reconfigPosProxy_exec();
   bool reconfigPosProxy_check();

   bool removePosProxy_exec();
   bool removePosProxy_check();

   bool addAnalogProxy_exec();
   bool addAnalogProxy_check();

   bool reconfigAnalogProxy_exec();
   bool reconfigAnalogProxy_check();

   bool removeAnalogProxy_exec();
   bool removeAnalogProxy_check();

   bool addDigitalProxy_exec();
   bool addDigitalProxy_check();

   bool reconfigDigitalProxy_exec();
   bool reconfigDigitalProxy_check();

   bool removeDigitalProxy_exec();
   bool removeDigitalProxy_check();

   bool addKeyboardProxy_exec();
   bool addKeyboardProxy_check();

   bool reconfigKeyboardProxy_exec();
   bool reconfigKeyboardProxy_check();

   bool removeKeyboardProxy_exec();
   bool removeKeyboardProxy_check();

   bool addStupefiedAnalogProxy_exec();
   bool addStupefiedAnalogProxy_check();

   bool removeStupefiedAnalogProxy_exec();
   bool removeStupefiedAnalogProxy_check();

private:

   //State machine items
   unsigned int mStartTime;
   unsigned int mFrameCount;
   unsigned int mTestState;
   unsigned int mWaitTime;
   bool mTesting;
   bool mFinished;

   //Element loading items
   std::string mPath;
   jccl::Configuration* mNewCfg;

   void initGLState();

   gadget::PositionInterface     mWand;
   gadget::PositionInterface     mHead;
   gadget::DigitalInterface   mButton0;
   gadget::DigitalInterface   mButton1;
   gadget::DigitalInterface   mButton2;
   gadget::DigitalInterface   mButton3;
   gadget::DigitalInterface   mButton4;
   gadget::DigitalInterface   mButton5;

};


#endif
