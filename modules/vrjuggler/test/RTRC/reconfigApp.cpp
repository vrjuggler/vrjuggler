/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2010 by Iowa State University
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

#include <iostream>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <fstream>
#include <unistd.h>
#include <boost/concept_check.hpp>

#include <gmtl/Matrix.h>
#include <gmtl/Vec.h>
#include <gmtl/Generate.h>

#include <jccl/RTRC/ConfigManager.h>
#include <gadget/InputManager.h>
#include <gadget/Type/AnalogProxy.h>
#include <gadget/Type/KeyboardMouse.h>
#include <gadget/Type/KeyboardMouseProxy.h>
#include <gadget/Devices/Sim/SimAnalog.h>
#include <gadget/Devices/Sim/SimDigital.h>

#include <vrj/Display/Display.h>
#include <vrj/Display/DisplayManager.h>
#include <vrj/Display/Viewport.h>
#include <vrj/Kernel/User.h>
#include <vrj/Util/Debug.h>
#include <reconfigApp.h>

void reconfigApp::init()
{
   mWand.init("VJWand");
   mHead.init("VJHead");
   mButton0.init("VJButton0");
   mButton1.init("VJButton1");
   mButton2.init("VJButton2");
   mButton3.init("VJButton3");
   mButton4.init("VJButton4");
   mButton5.init("VJButton5");

   mFrameCount = 0;
   mStartTime = 1000;
   mWaitTime = 100;
   mTestState = 0;
   mTesting = false;
   mFinished = false;
}

void reconfigApp::contextInit()
{
   initGLState();
}

//Do some pre draw calculations
void reconfigApp::preFrame()
{
   if ( mFinished )
   {
      return;
   }

   //Don't start tests until a certain frame number
   if ( mStartTime > 1 )
   {
      --mStartTime;
      return;
   }

   bool status = false;

   //If we are not currently doing any testing, start a new test state
   if ( ! mTesting )
   {
      mTesting = true;

      //Switch on the test state
      switch (mTestState)
      {
         case 0:
            status = addGFXWindow_exec();
            break;
         case 1:
            status = removeGFXWindow_exec();
            break;
         case 2:
            status = readdGFXWindow_exec();
            break;
         case 3:
            status = resizeGFXWindow_exec();
            break;
         case 4:
            status = moveGFXWindow_exec();
            break;
         case 5:
            status = addViewport_exec();
            break;
         case 6:
            status = removeViewport_exec();
            break;
         case 7:
            status = resizeViewport_exec();
            break;
         case 8:
            status = moveViewport_exec();
            break;
         case 9:
            status = enableStereoSurface_exec();
            break;
         case 10:
            status = disableStereoSurface_exec();
            break;
         case 11:
            status = removeKeyboardWin_exec();
            break;
         case 12:  status =       readdKeyboardWin_exec();     break;

         case 13:  status =       addSimPos_exec();            break;

         case 14:  status =       removeSimPos_exec();         break;

         case 15:  status =       readdSimPos_exec();          break;

         case 16:  status =       repointProxy_exec();         break;

         case 17:  status =       reconfigSimPos_exec();       break;

         case 18:  status =       reconfigSimDigital_exec();   break;

         case 19:  status =       reconfigSimAnalog_exec();    break;

         case 20:  status =       addSimDigital_exec();        break;

         case 21:  status =       removeSimDigital_exec();     break;

         case 22:  status =       readdSimDigital_exec();      break;

         case 23:  status =       addSimAnalog_exec();         break;

         case 24:  status =       removeSimAnalog_exec();      break;

         case 25:  status =       readdSimAnalog_exec();       break;

         case 26:  status =       addPosProxy_exec();          break;

         case 27:  status =       reconfigPosProxy_exec();     break;

         case 28:  status =       removePosProxy_exec();       break;

         case 29:  status =       addAnalogProxy_exec();       break;

         case 30:  status =       reconfigAnalogProxy_exec();  break;

         case 31:  status =       removeAnalogProxy_exec();    break;

         case 32:  status =       addDigitalProxy_exec();      break;

         case 33:  status =       reconfigDigitalProxy_exec(); break;

         case 34:  status =       removeDigitalProxy_exec();   break;

         case 35:  status =       addKeyboardProxy_exec();     break;

         case 36:  status =       reconfigKeyboardProxy_exec();break;

         case 37:  status =       removeKeyboardProxy_exec();  break;

         case 38:  status =       addMachineSpecific_exec();  break;

         //case 39:  status =       reconfigMachineSpecific_exec();  break;

         case 39:  status =       removeMachineSpecific_exec();  break;

         case 40:  status =       addStupefiedAnalogProxy_exec();  break;

         case 41:
            status = removeStupefiedAnalogProxy_exec();
            break;
         case 42:
            status = true;
            mFinished = true;
            std::cout << "\n\n[Test battery completed]\n\n" << std::flush;
            mKernel->stop();
            break;
         default:
            status = true;
            break;
      }

      if (!status)
      {
         std::cout << "WARNING: Problem executing test. Subsequent results "
                   << "may be inaccurate\n\n" << std::flush;
      }
   }

   //If we have waited enough time after executing a test...
   //Then we need to run the
   if ( checkTime() )
   {
      switch ( mTestState )
      {
         case  0:  status =       addGFXWindow_check();         break;

         case  1:  status =       removeGFXWindow_check();      break;

         case  2:  status =       readdGFXWindow_check();       break;

         case  3:  status =       resizeGFXWindow_check();      break;

         case  4:  status =       moveGFXWindow_check();        break;

         case  5:  status =       addViewport_check();          break;

         case  6:  status =       removeViewport_check();       break;

         case  7:  status =       resizeViewport_check();       break;

         case  8:  status =       moveViewport_check();         break;

         case  9:  status =       enableStereoSurface_check();  break;

         case 10:  status =       disableStereoSurface_check(); break;

         case 11:  status =       removeKeyboardWin_check();    break;

         case 12:  status =       readdKeyboardWin_check();     break;

         case 13:  status =       addSimPos_check();            break;

         case 14:  status =       removeSimPos_check();         break;

         case 15:  status =       readdSimPos_check();          break;

         case 16:  status =       repointProxy_check();         break;

         case 17:  status =       reconfigSimPos_check();       break;

         case 18:  status =       reconfigSimDigital_check();   break;

         case 19:  status =       reconfigSimAnalog_check();    break;

         case 20:  status =       addSimDigital_check();        break;

         case 21:  status =       removeSimDigital_check();     break;

         case 22:  status =       readdSimDigital_check();      break;

         case 23:  status =       addSimAnalog_check();         break;

         case 24:  status =       removeSimAnalog_check();      break;

         case 25:  status =       readdSimAnalog_check();       break;

         case 26:  status =       addPosProxy_check();          break;

         case 27:  status =       reconfigPosProxy_check();     break;

         case 28:  status =       removePosProxy_check();       break;

         case 29:  status =       addAnalogProxy_check();       break;

         case 30:  status =       reconfigAnalogProxy_check();  break;

         case 31:  status =       removeAnalogProxy_check();    break;

         case 32:  status =       addDigitalProxy_check();      break;

         case 33:  status =       reconfigDigitalProxy_check(); break;

         case 34:  status =       removeDigitalProxy_check();   break;

         case 35:  status =       addKeyboardProxy_check();     break;

         case 36:  status =       reconfigKeyboardProxy_check();break;

         case 37:  status =       removeKeyboardProxy_check();  break;

         case 38:  status =       addMachineSpecific_check();   break;

         //case 39:  status =       reconfigMachineSpecific_check();break;

         case 39:  status =       removeMachineSpecific_check();break;

         case 40:  status =       addStupefiedAnalogProxy_check();break;

         case 41:  status =       removeStupefiedAnalogProxy_check();break;

         default:
            status = true;
            break;
      }

      ++mTestState;      //Next test state
      mTesting = false;  //No longer in the middle of a test

      if ( status )
      {
         std::cout << "[TEST PASSED]\n\n" << std::flush;
      }
      else
      {
         std::cout << "[ *** TEST FAILED *** ]\n\n" << std::flush;
      }
   }
}

void reconfigApp::bufferPreDraw()
{
   glClearColor(0.0, 0.0, 0.0, 0.0);
   glClear(GL_COLOR_BUFFER_BIT);
}

void reconfigApp::draw()
{
   glClear(GL_DEPTH_BUFFER_BIT);

   initGLState();

   glColor3f(0.3, 0.05, 0.6);
   glBegin(GL_POLYGON);
      glVertex3f(5.0, 0.0, -5.0);
      glVertex3f(-5.0, 0.0, -5.0);
      glVertex3f(0.0, 10.0, -5.0);
   glEnd();
}

void reconfigApp::initGLState()
{
   GLfloat light0_ambient[] = { 0.2f, 0.2f, 0.2f, 1.0f };
   GLfloat light0_diffuse[] = { 0.8f, 0.8f,  0.8f, 1.0f };
   GLfloat light0_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
   GLfloat light0_position[] = { 0.0f, 10.0f, 0.0f, 0.0f, 1.0f };

   glLightfv(GL_LIGHT0, GL_AMBIENT, light0_ambient);
   glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);
   glLightfv(GL_LIGHT0, GL_SPECULAR, light0_specular);
   glLightfv(GL_LIGHT0, GL_POSITION, light0_position);

   glEnable(GL_DEPTH_TEST);
   glEnable(GL_NORMALIZE);
   glEnable(GL_LIGHTING);
   glEnable(GL_LIGHT0);
   glEnable(GL_COLOR_MATERIAL);
   glShadeModel(GL_SMOOTH);

   glMatrixMode(GL_MODELVIEW);
}

/********************************************************************
UTILITY FUNCTIONS
********************************************************************/

void reconfigApp::setPath(const std::string& path)
{
   mPath = path;
}

bool reconfigApp::addElementFile(const std::string& filename)
{
   //Attempt to load a new config element
   if (NULL !=  mNewCfg )
   {
      delete mNewCfg;
   }

   mNewCfg = new jccl::Configuration;

   if ( mNewCfg->load(filename) )
   {
      jccl::ConfigManager::instance()->addConfigurationAdditions(mNewCfg);
      return true;
   }
   else
   {
      std::cout << "\tError: Failed to load config file: " << filename
                << std::endl << std::flush;
      delete mNewCfg;
      mNewCfg = NULL;
      return false;
   }
}

bool reconfigApp::removeElementFile(const std::string& filename)
{
   //Attempt to load a new config element
   if ( NULL != mNewCfg )
   {
      delete mNewCfg;
   }

   mNewCfg = new jccl::Configuration;

   if ( mNewCfg->load(filename) )
   {
      jccl::ConfigManager::instance()->addConfigurationRemovals(mNewCfg);
      return true;
   }
   else
   {
      std::cout << "\tError: Failed to load config file: " << filename
                << std::endl;
      delete mNewCfg;
      mNewCfg = NULL;
      return false;
   }
}

bool reconfigApp::swapElementFiles(const std::string& removeFile,
                                   const std::string& addFile)
{
   if ( ! removeElementFile(removeFile) )
   {
      std::cout << "\tError: Could not remove config file " << removeFile
                << std::endl;
      return false;
   }

   if ( ! addElementFile(addFile) )
   {
      std::cout << "\tError: Could not add config file " << addFile
                << std::endl;
      return false;
   }

   return true;
}

bool reconfigApp::removeRecentConfiguration()
{
   if ( NULL != mNewCfg )
   {
      jccl::ConfigManager::instance()->addConfigurationRemovals(mNewCfg);
      return true;
   }
   else
   {
      std::cout << "\tError: Cannot remove a NULL configuration" << std::endl;
      return false;
   }
}

bool reconfigApp::checkTime()
{
   if ( mFrameCount >= mWaitTime )
   {
      mFrameCount = 0;
      return true;
   }
   else
   {
      ++mFrameCount;
      return false;
   }
}

bool reconfigApp::verifyProxy(const std::string& proxyName,
                              const std::string& deviceName)
{
   //Try to get the new proxy and check its values
   gadget::ProxyPtr proxy =
      gadget::InputManager::instance()->getProxy(proxyName);

   if ( NULL == proxy )
   {
      std::cout << "\tError: Could not find the proxy" << std::endl;
      return false;
   }

   if ( proxy->isStupefied() )
   {
      std::cout << "\tError: Proxy is Stupefied" << std::endl;
      return false;
   }

   gadget::InputPtr inputDevice = proxy->getProxiedInputDevice();

   if ( NULL == inputDevice.get() )
   {
      std::cout << "\tError: Proxy does not point at a valid device"
                << std::endl;
      return false;
   }

   if ( inputDevice->getInstanceName() != deviceName )
   {
      std::cout << "\tError: Input device has wrong name: "
                << inputDevice->getInstanceName() << "   It should be "
                << deviceName << std::endl;
      return false;
   }

   return true;
}

vrj::DisplayPtr reconfigApp::getDisplay(const std::string& name)
{
   //Search the display manager for the display by name
   typedef std::vector<vrj::DisplayPtr>::iterator iter_type;
   std::vector<vrj::DisplayPtr> allDisplays =
      vrj::DisplayManager::instance()->getAllDisplays();

   for ( iter_type iter = allDisplays.begin(); iter != allDisplays.end(); ++iter )
   {
      if ( name == (*iter)->getName() )
      {
         return *iter;
      }
   }

   return vrj::DisplayPtr();
}

bool reconfigApp::verifyDisplayProps(vrj::DisplayPtr disp,
                                     const std::string& name,
                                     const int xOrigin, const int yOrigin,
                                     const int xSize, const int ySize,
                                     const int pipeNum, const bool stereo,
                                     const bool border, const bool active)
{
   bool ok(true);

   if ( disp->getName() != name )
   {
      ok = false;
   }

   int xo, yo, xs, ys;
   disp->getOriginAndSize(xo, yo, xs, ys);

   if ( xo != xOrigin )
   {
      ok = false;
   }

   if ( yo != yOrigin )
   {
      ok = false;
   }

   if ( xs != xSize )
   {
      ok = false;
   }

   if ( ys != ySize )
   {
      ok = false;
   }

   if ( disp->getPipe() != pipeNum )
   {
      ok = false;
   }

   if ( disp->isStereoRequested() != stereo )
   {
      ok = false;
   }

   if ( disp->shouldDrawBorder() != border )
   {
      ok = false;
   }

   if ( disp->isActive() != active )
   {
      ok = false;
   }

   return ok;
}

bool reconfigApp::verifyAllViewports(vrj::DisplayPtr display,
                                     jccl::ConfigElementPtr viewportElement)
{
   //Check to see if this viewport element matches with ANY of the display's
   //viewports
   bool status(false);
   for ( unsigned int i = 0; i < display->getNumViewports(); ++i )
   {
      vrj::ViewportPtr viewport = display->getViewport(i);
      if ( verifyViewport(viewport, viewportElement) )
      {
         status = true;
      }
   }

   return status;
}

bool reconfigApp::verifyViewport(vrj::ViewportPtr viewport,
                                 jccl::ConfigElementPtr viewportElement)
{
   //Verify that the display object has the specified viewport

   //First, get the properties from the config element
   const std::string viewport_name = viewportElement->getName();

   //Get origin values
   if ( viewportElement->getNum("origin") < 2 )
   {
      std::cout << "\tError: viewport element named " << viewport_name
                << " doesn't have origin properties" << std::endl;
      return false;
   }

   const float x_origin = viewportElement->getProperty<float>("origin", 0);
   const float y_origin = viewportElement->getProperty<float>("origin", 1);

   //Get size values
   if ( viewportElement->getNum("size") < 2 )
   {
      std::cout << "\tError: viewport element named " << viewport_name
                << " doesn't have size properties" << std::endl;
      return false;
   }

   const float x_size = viewportElement->getProperty<float>("size", 0);
   const float y_size = viewportElement->getProperty<float>("size", 1);

   //TEST size and origin
   float xo, xs, yo, ys;
   viewport->getOriginAndSize(xo, yo, xs, ys);
   if ( x_size != xs || y_size != ys || x_origin != xo || y_origin != yo )
   {
      std::cout << "\tError: viewport has incorrect size and/or origin"
                << std::endl;
      return false;
   }

   //Get view property
   if ( viewportElement->getNum("view") < 1 )
   {
      std::cout << "\tError: viewport element named " << viewport_name
                << " doesn't have view properties" << std::endl;
      return false;
   }

   const int view_num = viewportElement->getProperty<int>("view", 0);

   //TEST view prop
   if ( view_num != viewport->getView() )
   {
      std::cout << "\tError: viewport has incorrect view property"
                << std::endl;
      return false;
   }

   //Get cameraPos property
   if ( viewportElement->getNum("camera_pos") < 1 )
   {
      std::cout << "\tError: viewport element named " << viewport_name
                << " doesn't have cameraPos properties" << std::endl;
      return false;
   }

   const std::string camera_pos =
      viewportElement->getProperty<std::string>("camera_pos", 0);

   //TEST cameraPos property
   //There is no camera position property for the vrj::Viewport object

   //Get user property
   if ( viewportElement->getNum("user") < 1 )
   {
      std::cout << "\tError: viewport element named " << viewport_name
                << " doesn't have user properties" << std::endl;
      return false;
   }

   const std::string username =
      viewportElement->getProperty<std::string>("user", 0);

   if ( viewport->getUser()->getName() != username )
   {
      std::cout << "\tError: viewport has incorrect user name" << std::endl;
      return false;
   }

   //Get wandPos
   if ( viewportElement->getNum("wand_pos") < 1 )
   {
      std::cout << "\tError: viewport element named " << viewport_name
                << " doesn't have wandPos properties" << std::endl;
      return false;
   }

   const std::string wandPos =
      viewportElement->getProperty<std::string>("wand_pos", 0);
   //TEST wandPos property
   //There is no wand position property for the vrj::Viewport object

   //Get active
   if ( viewportElement->getNum("active") < 1 )
   {
      std::cout << "\tError: viewport element named " << viewport_name
                << " doesn't have cameraPos properties" << std::endl;
      return false;
   }

   const bool active = viewportElement->getProperty<bool>("active", 0);

   if ( viewport->isActive() != active )
   {
      std::cout << "\tError: viewport has incorrect active property"
                << std::endl;
      return false;
   }

   //drawProjections
   if ( viewportElement->getNum("draw_projections") < 1 )
   {
      std::cout << "\tError: viewport element named " << viewport_name
                << " doesn't have drawProjections properties" << std::endl;
      return false;
   }

   const bool draw_projections =
      viewportElement->getProperty<bool>("draw_projections");
   boost::ignore_unused_variable_warning(draw_projections);
   //TEST drawProjections property
   //There is no test for this property

   //surfaceColor x 3
   if ( viewportElement->getNum("surface_color") < 3 )
   {
      std::cout << "\tError: viewport element named " << viewport_name
                << " doesn't have surfaceColor properties" << std::endl;
      return false;
   }

   float color[3];
   color[0] = viewportElement->getProperty<float>("surface_color", 0);
   color[1] = viewportElement->getProperty<float>("surface_color", 1);
   color[2] = viewportElement->getProperty<float>("surface_color", 2);

   std::cout << "R:" << color[0] << "G:" << color[1] << "B:" << color[2]
             << std::endl;

   //Also no test for this

   //vert_fov
   if ( viewportElement->getNum("vertical_fov") < 1 )
   {
      std::cout << "\tError: viewport element named " << viewport_name
                << " doesn't have vert_fov properties" << std::endl;
      return false;
   }

   const float vert_fov =
      viewportElement->getProperty<float>("vertical_fov", 0);
   boost::ignore_unused_variable_warning(vert_fov);

   //Also no test for this

   return true;
}

bool reconfigApp::verifyDisplayFile(const std::string& filename)
{
   //Assume that the file given has the displayWindow element in it
   //Load up the given file
   jccl::Configuration cfg;
   cfg.load(filename);
   std::vector<jccl::ConfigElementPtr> windowElements;
   std::vector<jccl::ConfigElementPtr> machineElements;
   std::vector<jccl::ConfigElementPtr> machineSpecificElements;
   cfg.getByType("display_window", windowElements);
   cfg.getByType("machine_specific", machineElements);

   for ( unsigned int i = 0; i < machineElements.size(); ++i )
   {
      machineSpecificElements = windowElements[i]->getChildElements();
      for ( unsigned int j = 0; j < machineSpecificElements.size(); ++j )
      {
         if ( machineSpecificElements[j]->getID() == "display_window" )
         {
            std::cout << "Adding an embedded display to the list to test "
                      << "named: " << machineSpecificElements[j]->getName()
                      << std::endl;
            windowElements.push_back(machineSpecificElements[j]);
         }
      }
   }

   //Verify EACH display in the file
   for ( unsigned int i = 0; i < windowElements.size(); ++i )
   {
      //Get its attributes
      const std::string display_name = windowElements[i]->getName();

      //Get the display with the same name from the DisplayManager
      //and make sure it exists first
      vrj::DisplayPtr display = getDisplay(display_name);

      if ( display.get() == NULL )
      {
         std::cout << "\tError: display element named " << display_name
                   << " is not in the display manager" << std::endl;
         return false;
      }

      //Get origin values
      if ( windowElements[i]->getNum("origin") < 2 )
      {
         std::cout << "\tError: display element named " << display_name
                   << " doesn't have origin properties" << std::endl;
         return false;
      }

      const int x_origin = windowElements[i]->getProperty<int>("origin", 0);
      const int y_origin = windowElements[i]->getProperty<int>("origin", 1);

      //Get size values
      if ( windowElements[i]->getNum("size") < 2 )
      {
         std::cout << "\tError: display element named " << display_name
                   << " doesn't have size properties\n" << std::flush;
         return false;
      }

      const int x_size = windowElements[i]->getProperty<int>("size", 0);
      const int y_size = windowElements[i]->getProperty<int>("size", 1);

      //Get pipe number
      if ( windowElements[i]->getNum("pipe") < 1 )
      {
         std::cout << "\tError: display element named " << display_name
                   << " doesn't have pipe property" << std::endl;
         return false;
      }

      const int pipe_num = windowElements[i]->getProperty<int>("pipe");

      //Get stereo value
      if ( windowElements[i]->getNum("stereo") < 1 )
      {
         std::cout << "\tError: display element named " << display_name
                   << " doesn't have stereo property" << std::endl;
         return false;
      }

      const bool stereo = windowElements[i]->getProperty<bool>("stereo");

      //Get border value
      if ( windowElements[i]->getNum("border") < 1 )
      {
         std::cout << "\tError: display element named " << display_name
                   << " doesn't have border property" << std::endl;
         return false;
      }

      const bool border = windowElements[i]->getProperty<bool>("border");

      //Get active value
      if ( windowElements[i]->getNum("active") < 1 )
      {
         std::cout << "\tError: display element named " << display_name
                   << " doesn't have active property" << std::endl;
         return false;
      }

      const bool active = windowElements[i]->getProperty<bool>("active");

      //Verify the number of viewports
      if ( unsigned(windowElements[i]->getNum("simulator_viewports")) != unsigned(display->getNumViewports()) )
      {
         std::cout << "\tError: display element named " << display_name
                   << " has the wrong number of viewports" << std::endl;
         return false;
      }

      //Verify all of the viewports
      for ( unsigned int j = 0;
            j < windowElements[i]->getNum("simulator_viewports");
            ++j )
      {
         jccl::ConfigElementPtr viewlist =
            windowElements[i]->getProperty<jccl::ConfigElementPtr>("simulator_viewports", j);

         //Get the actual simViewport element and check if it really contains
         //any viewports
         if ( viewlist->getNum("simulator_viewport") > 0 )
         {
            jccl::ConfigElementPtr viewport =
               viewlist->getProperty<jccl::ConfigElementPtr>("simulator_viewport", 0);

            if ( ! verifyAllViewports(display, viewport) )
            {
               std::cout << "\tError: display element named " << display_name
                         << " has an incorrect viewport" << std::endl;
               return false;
            }
         }
      }

      //Run a verification check (verifyDisplayProps)
      return verifyDisplayProps(display, display_name, x_origin, y_origin,
                                x_size, y_size, pipe_num, stereo, border,
                                active);
   }

   return true;
}

/********************************************************************
TEST SUITE FUNCTIONS
********************************************************************/

//  - Adding machine specfic gfx window
bool reconfigApp::addMachineSpecific_exec()
{
   std::cout << "Beginning test for adding machine specific graphics windows..."
             << std::endl;
   return addElementFile(mPath + "sim.ms.01.jconf");
}

bool reconfigApp::addMachineSpecific_check()
{
   return verifyDisplayFile(mPath + "sim.ms.01.jconf");
}

//  - Remove machine specific gfx window
bool reconfigApp::removeMachineSpecific_exec()
{
   std::cout << "Beginning test for removing machine specific graphics windows..."
             << std::endl;
   return removeElementFile(mPath + "sim.ms.01.jconf");
}

bool reconfigApp::removeMachineSpecific_check()
{
   //Load up the given file
   jccl::Configuration cfg;
   cfg.load(mPath + "sim.extradisplay.01.jconf");
   std::vector<jccl::ConfigElementPtr> windowElements;
   cfg.getByType("display_window", windowElements);

   if ( windowElements.size() != 1 )
   {
      std::cout << "\tError: the display config element file contains "
                << windowElements.size() << " displays (should be 1)"
                << std::endl;
      return false;
   }

   vrj::DisplayPtr display = getDisplay(windowElements[0]->getName());
   if ( display.get() != NULL )
   {
      std::cout << "\tError: there is still a display in the system named "
                << windowElements[0]->getName() << std::endl;
      return false;
   }

   return true;
}

//  - Adding gfx window
bool reconfigApp::addGFXWindow_exec()
{
   std::cout << "Beginning test for adding a graphics window..." << std::endl;
   return addElementFile(mPath + "sim.extradisplay.01.jconf");
}

bool reconfigApp::addGFXWindow_check()
{
   return verifyDisplayFile(mPath + "sim.extradisplay.01.jconf");
}

//  - Remove gfx window
bool reconfigApp::removeGFXWindow_exec()
{
   std::cout << "Beginning test for removing a graphics window..."
             << std::endl;
   return removeElementFile(mPath + "sim.extradisplay.01.jconf");
}

bool reconfigApp::removeGFXWindow_check()
{
   //Load up the given file
   jccl::Configuration cfg;
   cfg.load(mPath + "sim.extradisplay.01.jconf");
   std::vector<jccl::ConfigElementPtr> windowElements;
   cfg.getByType("display_window", windowElements);

   if ( windowElements.size() != 1 )
   {
      std::cout << "\tError: the display config element file contains "
                << windowElements.size() << " displays (should be 1)"
                << std::endl;
      return false;
   }

   vrj::DisplayPtr display = getDisplay(windowElements[0]->getName());
   if ( display.get() != NULL )
   {
      std::cout << "\tError: there is still a display in the system named "
                << windowElements[0]->getName() << std::endl;
      return false;
   }

   return true;
}

bool reconfigApp::readdGFXWindow_exec()
{
   std::cout << "Beginning test for readding a graphics window..."
             << std::endl;

   return addElementFile(mPath + "sim.extradisplay.01.jconf") &&
          addElementFile(mPath + "sim.extradisplay.02.jconf");
}

bool reconfigApp::readdGFXWindow_check()
{
   return verifyDisplayFile(mPath + "sim.extradisplay.01.jconf") &&
          verifyDisplayFile(mPath + "sim.extradisplay.02.jconf");
}

bool reconfigApp::resizeGFXWindow_exec()
{
   std::cout << "Beginning test for resizing a graphics window..."
             << std::endl;

   return swapElementFiles(mPath + "sim.extradisplay.02.jconf",
                           mPath + "sim.extradisplay.02.resize.jconf");
}

bool reconfigApp::resizeGFXWindow_check()
{
   return verifyDisplayFile(mPath + "sim.extradisplay.02.resize.jconf");
}

bool reconfigApp::moveGFXWindow_exec()
{
   std::cout << "Beginning test for moving a graphics window..." << std::endl;

   return swapElementFiles(mPath + "sim.extradisplay.01.jconf",
                           mPath + "sim.extradisplay.01.move.jconf");
}

bool reconfigApp::moveGFXWindow_check()
{
   return verifyDisplayFile(mPath + "sim.extradisplay.01.move.jconf");
}

bool reconfigApp::addViewport_exec()
{
   std::cout << "Beginning test for adding a viewport to a display window..."
             << std::endl;

   return swapElementFiles(mPath + "sim.extradisplay.02.resize.jconf",
                           mPath + "sim.extradisplay.02.twoviews.jconf");
}

bool reconfigApp::addViewport_check()
{
   return verifyDisplayFile(mPath + "sim.extradisplay.02.twoviews.jconf");
}

bool reconfigApp::removeViewport_exec()
{
   std::cout << "Beginning test for removing a viewport from a graphics window..."
             << std::endl;

   return swapElementFiles(mPath + "sim.extradisplay.02.twoviews.jconf",
                           mPath + "sim.extradisplay.02.oneview.jconf");
}

bool reconfigApp::removeViewport_check()
{
   return verifyDisplayFile(mPath + "sim.extradisplay.02.oneview.jconf");
}

bool reconfigApp::resizeViewport_exec()
{
   std::cout << "Beginning test for resizing a viewport..." << std::endl;

   return swapElementFiles(mPath + "sim.extradisplay.02.oneview.jconf",
                           mPath + "sim.extradisplay.02.largerview.jconf");
}

bool reconfigApp::resizeViewport_check()
{
   return verifyDisplayFile(mPath + "sim.extradisplay.02.largerview.jconf");
}

bool reconfigApp::moveViewport_exec()
{
   std::cout << "Beginning test for moving a viewport..." << std::endl;

   return swapElementFiles(mPath + "sim.extradisplay.02.largerview.jconf",
                           mPath + "sim.extradisplay.02.moveview.jconf");
}

bool reconfigApp::moveViewport_check()
{
   return verifyDisplayFile(mPath + "sim.extradisplay.02.moveview.jconf");
}

bool reconfigApp::enableStereoSurface_exec()
{
   std::cout << "Beginning test for enabling stereo on a surface display..."
             << std::endl;

   return removeElementFile(mPath + "sim.extradisplay.02.moveview.jconf") &&
          removeElementFile(mPath + "sim.extradisplay.01.move.jconf") &&
          addElementFile(mPath + "sim.surfacedisplay.01.stereo.jconf");
}

bool reconfigApp::enableStereoSurface_check()
{
   //Load up the given file
   jccl::Configuration cfg;
   cfg.load(mPath + "sim.surfacedisplay.01.stereo.jconf");
   std::vector<jccl::ConfigElementPtr> windowElements;
   cfg.getByType("display_window", windowElements);

   if ( windowElements.size() < 1 )
   {
      std::cout << "\tError: There are no display window config elements in the file"
                << std::endl;
      return false;
   }

   //Get the SimSurfaceX01
   vrj::DisplayPtr surfaceDisplay = getDisplay(windowElements[0]->getName());

   //If we could not find the window...we have a problem to start with
   if ( surfaceDisplay.get() == NULL )
   {
      std::cout << "\tError: Could not find the display to check its viewport"
                << std::endl;
      return false;
   }

   //Make sure we have one viewport
   if ( surfaceDisplay->getNumViewports() != 1 )
   {
      std::cout << "\tError: " << windowElements[0]->getName()
                << " has an incorrect number of viewports" << std::endl;
      return false;
   }

   vrj::ViewportPtr surfaceViewport = surfaceDisplay->getViewport(0);

   if ( ! surfaceViewport->inStereo() )
   {
      std::cout << "\tError: the surface viewport is not in stereo"
                << std::endl;
      return false;
   }

   return true;
}

bool reconfigApp::disableStereoSurface_exec()
{
   std::cout << "Beginning test for disabling stereo on a surface display..."
             << std::endl;

   return swapElementFiles(mPath + "sim.surfacedisplay.01.stereo.jconf",
                           mPath + "sim.surfacedisplay.01.mono.jconf");
}

bool reconfigApp::disableStereoSurface_check()
{
   //Load up the given file
   jccl::Configuration cfg;
   cfg.load(mPath + "sim.surfacedisplay.01.mono.jconf");
   std::vector<jccl::ConfigElementPtr> windowElements;
   cfg.getByType("display_window", windowElements);

   if ( windowElements.size() < 1 )
   {
      std::cout << "\tError: There are no display window config elements in the file"
                << std::endl;
      return false;
   }

   //Get the SimSurfaceX01
   vrj::DisplayPtr surfaceDisplay = getDisplay(windowElements[0]->getName());

   //If we could not find the window...we have a problem to start with
   if ( surfaceDisplay.get() == NULL )
   {
      std::cout << "\tError: Could not find " << windowElements[0]->getName()
                << " to check its viewport" << std::endl;
      return false;
   }

   //Make sure we have one viewport
   if ( surfaceDisplay->getNumViewports() != 1 )
   {
      std::cout << "\tError: " << windowElements[0]->getName()
                << " has an incorrect number of viewports" << std::endl;
      return false;
   }

   vrj::ViewportPtr surfaceViewport = surfaceDisplay->getViewport(0);

   if ( surfaceViewport->inStereo() )
   {
      std::cout << "\tError: " << windowElements[0]->getName()
                << "'s viewport is in stereo" << std::endl;
      return false;
   }

   return true;
}

bool reconfigApp::removeKeyboardWin_exec()
{
   std::cout << "Beginning test for removing a keyboard window..."
             << std::endl;

   //Note that we are assuming ./Elements/startup/sim.wandkeyboardproxy.jconf
   //has already been loaded

   return removeElementFile(mPath + "startup/sim.wandkeyboard.jconf");
}

bool reconfigApp::removeKeyboardWin_check()
{
   //First check that the keyboard device no longer exists
   jccl::Configuration cfg;
   cfg.load(mPath + "startup/sim.wandkeyboard.jconf");
   std::vector<jccl::ConfigElementPtr> file_elements;
   cfg.getByType("input_window", file_elements);

   const std::string keyboard_name = file_elements[0]->getName();

   gadget::KeyboardMousePtr keyboard =
      boost::dynamic_pointer_cast<gadget::KeyboardMouse>(
         gadget::InputManager::instance()->getDevice(keyboard_name)
      );

   if ( keyboard != NULL )
   {
      std::cout << "\tError: the keyboard device named " << keyboard_name
                << " still exists" << std::endl;
      return false;
   }

   //Check the wand keyboard proxy that is supposed to point to it is Stupefied
   jccl::Configuration cfg2;
   cfg2.load(mPath + "startup/sim.wandkeyboardproxy.jconf");
   file_elements.clear();
   cfg2.getByType("input_window_proxy", file_elements);

   if ( file_elements.size() != 1 )
   {
      std::cout << "\tError: there are " << file_elements.size()
                << " keyboard proxies in the file (should be 1)" << std::endl;
      return false;
   }

   gadget::KeyboardMouseProxyPtr keyboard_proxy =
      boost::dynamic_pointer_cast<gadget::KeyboardMouseProxy>(
         gadget::InputManager::instance()->getProxy(file_elements[0]->getName())
      );

   if ( keyboard_proxy == NULL )
   {
      std::cout << "\tError: there is no keyboard proxy named "
                << file_elements[0]->getName() << std::endl;
      return false;
   }

   if (!keyboard_proxy->isStupefied())
   {
      std::cout << "\tError: keyboard proxy named "
                << file_elements[0]->getName() << " is not Stupefied"
                << std::endl;
      return false;
   }

   if ( keyboard_proxy->getDeviceName() != keyboard_name )
   {
      std::cout << "\tError: this keyboard proxy points at a keyboard named "
                << keyboard_proxy->getDeviceName() << std::endl;
      return false;
   }

   return true;
}

bool reconfigApp::readdKeyboardWin_exec()
{
   std::cout << "Beginning test for readding a keyboard window..."
             << std::endl;
   return addElementFile(mPath + "startup/sim.wandkeyboard.jconf");
}

bool reconfigApp::readdKeyboardWin_check()
{
   //Check the wand keyboard proxy that is supposed to point to it

   //First check that the keyboard device exists again
   jccl::Configuration cfg;
   cfg.load(mPath + "startup/sim.wandkeyboard.jconf");
   std::vector<jccl::ConfigElementPtr> file_elements;
   cfg.getByType("input_window", file_elements);

   const std::string keyboard_name = file_elements[0]->getName();

   gadget::KeyboardMousePtr keyboard =
      boost::dynamic_pointer_cast<gadget::KeyboardMouse>(
         gadget::InputManager::instance()->getDevice(keyboard_name)
      );

   if ( keyboard == NULL )
   {
      std::cout << "\tError: the keyboard device named " << keyboard_name
                << " doesn't exist" << std::endl;
      return false;
   }

   //Check the wand keyboard proxy that is supposed to point to it is Stupefied
   jccl::Configuration cfg2;
   cfg2.load(mPath + "startup/sim.wandkeyboardproxy.jconf");
   file_elements.clear();
   cfg2.getByType("input_window_proxy", file_elements);

   if ( file_elements.size() != 1 )
   {
      std::cout << "\tError: there are " << file_elements.size()
                << " keyboard proxies in the file (should be 1)" << std::endl;
      return false;
   }

   gadget::KeyboardMouseProxyPtr keyboard_proxy =
      boost::dynamic_pointer_cast<gadget::KeyboardMouseProxy>(
         gadget::InputManager::instance()->getProxy(file_elements[0]->getName())
      );

   if ( keyboard_proxy == NULL )
   {
      std::cout << "\tError: there is no keyboard proxy named "
                << file_elements[0]->getName() << std::endl;
      return false;
   }

   if ( keyboard_proxy->isStupefied() )
   {
      std::cout << "\tError: keyboard proxy named "
                << file_elements[0]->getName() << " is Stupefied"
                << std::endl;
      return false;
   }

   if ( keyboard_proxy->getDeviceName() != keyboard_name )
   {
      std::cout << "\tError: this keyboard proxy points at a keyboard named "
                << keyboard_proxy->getDeviceName() << std::endl;
      return false;
   }

   //Make sure the pointers match up
   gadget::KeyboardMousePtr dev =
      boost::dynamic_pointer_cast<gadget::KeyboardMouse>(
         keyboard_proxy->getProxiedInputDevice()
      );
   if ( dev != keyboard)
   {
      std::cout << "\tError: pointers don't match up" << std::endl;
      return false;
   }

   return true;
}

bool reconfigApp::addSimPos_exec()
{
   std::cout << "Beginning test for adding a sim position device and pointing proxies at it..."
             << std::endl;
   return addElementFile(mPath + "sim.positiondevice.jconf") &&
          addElementFile(mPath + "sim.positiondeviceproxy.jconf");
}

bool reconfigApp::addSimPos_check()
{
   //Check that the sim pos device exists and that the proxy points at it
   return verifyProxy("ExtraPositionProxy", "ExtraPositionDevice");
}

bool reconfigApp::removeSimPos_exec()
{
   std::cout << "Beginning test for removing a sim position device and checking its proxies..."
             << std::endl;
   return removeElementFile(mPath + "sim.positiondevice.jconf");
}

bool reconfigApp::removeSimPos_check()
{
   //Look at the button proxy and see what it points at
   gadget::ProxyPtr proxy =
      gadget::InputManager::instance()->getProxy("ExtraPositionProxy");

   if ( proxy == NULL )
   {
      std::cout << "\tError: Could not find the proxy" << std::endl;
      return false;
   }

   if ( ! proxy->isStupefied() )
   {
      std::cout << "\tError: Proxy is not Stupefied" << std::endl;
      return false;
   }

   return true;
}

bool reconfigApp::readdSimPos_exec()
{
   std::cout << "Beginning test for readding a sim position device and checking its proxies..."
             << std::endl;
   return addElementFile(mPath + "sim.positiondevice.jconf");
}

bool reconfigApp::readdSimPos_check()
{
   //Check that the sim pos device exists and that the proxy points at it
   return verifyProxy("ExtraPositionProxy", "ExtraPositionDevice");
}

bool reconfigApp::repointProxy_exec()
{
   std::cout << "Beginning test for repointing a proxy to something else..."
             << std::endl;

   return swapElementFiles(mPath + "sim.positiondeviceproxy.jconf",
                           mPath + "sim.positiondeviceproxy.repoint.jconf");
}

bool reconfigApp::repointProxy_check()
{
   //Load the repoint config file, get its device property, match that name
   //with the gadget::Proxy's instance name
   jccl::Configuration cfg;
   cfg.load(mPath + "sim.positiondeviceproxy.repoint.jconf");
   std::vector<jccl::ConfigElementPtr> file_elements;
   cfg.getByType("position_proxy", file_elements);


   if ( file_elements.size() != 1 )
   {
      std::cout << "\tError: there are " << file_elements.size()
                << " proxies in the config file (should be 1)" << std::endl;
      return false;
   }

   if ( file_elements[0]->getNum("device") != 1 )
   {
      std::cout << "\tError: there are " << file_elements[0]->getNum("device")
                << " device properties in the config file (should be 1)"
                << std::endl;
      return false;
   }

   //Get the proxy's name from the file
   const std::string proxyname = file_elements[0]->getName();

   gadget::PositionProxyPtr proxy =
      boost::dynamic_pointer_cast<gadget::PositionProxy>(
         gadget::InputManager::instance()->getProxy(proxyname)
      );

   if ( proxy == NULL )
   {
      std::cout << "\tError: Could not find proxy named " << proxyname
                << " in the Input Manager" << std::endl;
      return false;
   }

   if ( file_elements[0]->getProperty<std::string>("device", 0) != proxy->getDeviceName() )
   {
      std::cout << "\tError: the proxy points at device named "
                << proxy->getDeviceName() << ", but should point at "
                << file_elements[0]->getProperty<std::string>("device", 0)
                << std::endl;
      return false;
   }

   return true;
}

bool reconfigApp::reconfigSimPos_exec()
{
   std::cout << "Beginning test for reconfiguring a sim position device..."
             << std::endl;

   return swapElementFiles(mPath + "startup/sim.simheadpos.jconf",
                           mPath + "sim.simheadpos.reconfig.jconf");
}

bool reconfigApp::reconfigSimPos_check()
{
   //Get the sim position pointer
   gadget::PositionPtr device =
      boost::dynamic_pointer_cast<gadget::Position>(
         gadget::InputManager::instance()->getDevice("SimHeadPos")
      );

   if ( device.get() == NULL )
   {
      std::cout << "\tError: Could not find the sim position device"
                << std::endl;
      return false;
   }

   gmtl::Matrix44f mat = device->getPositionData().getPosition();

   gmtl::Vec3f pos = gmtl::makeTrans<gmtl::Vec3f>(mat);

   if ( pos[0] != 1.0f || pos[1] != 2.0f || pos[2] != 3.0f )
   {
      std::cout << "\tError: Sim position device has incorrect initial "
                << "position: " << pos << std::endl;
   }

   return true;
}


bool reconfigApp::reconfigSimDigital_exec()
{
   std::cout << "Beginning test for reconfiguring a sim digital device..."
             << std::endl;

   return swapElementFiles(mPath + "startup/sim.wandbuttonsdigital02.jconf",
                           mPath + "sim.wandbuttonsdigital02.reconfig.jconf");
}

bool reconfigApp::reconfigSimDigital_check()
{
   //Get the sim digital pointer
   boost::shared_ptr<gadget::SimDigital> device =
      boost::dynamic_pointer_cast<gadget::SimDigital>(
         gadget::InputManager::instance()->getDevice("SimWandButtons02")
      );

   if ( device.get() == NULL )
   {
      std::cout << "\tError: Could not find the sim digital device"
                << std::endl;
      return false;
   }

   //Get the vector of keymodpairs
   std::vector<gadget::SimInput::KeyModPair> keypairs = device->getKeys();

   //Load up the config file.
   jccl::Configuration cfg;
   cfg.load(mPath + "sim.wandbuttonsdigital02.reconfig.jconf");
   std::vector<jccl::ConfigElementPtr> file_elements;
   cfg.getByType("simulated_digital_device", file_elements);

   bool key_status;
   //For each keymodpair that was defined in the device...
   for ( unsigned int i = 0; i < keypairs.size(); ++i )
   {
      key_status = false;

      //Scan the sim digital config element to see if was defined there
      for ( unsigned int j = 0; j < file_elements[0]->getNum("key_pair"); ++j )
      {
         jccl::ConfigElementPtr key_mod_pair =
            file_elements[0]->getProperty<jccl::ConfigElementPtr>("key_pair", j);

         //If they are the same..
         if ( keypairs[i].mModifier == key_mod_pair->getProperty<int>("modifier_key", 0) &&
              keypairs[i].mKey == key_mod_pair->getProperty<int>("key", 0) )
         {
            key_status = true;
            continue;
         }
      }

      //If we have not found a key that matches...
      if ( key_status == false )
      {
         std::cout << "\tError: One or more of the key pair definitions are incorrect"
                   << std::endl;
         return false;
      }

   }

   return true;
}

bool reconfigApp::reconfigSimAnalog_exec()
{
   std::cout << "Beginning test for reconfiguring a sim analog device..."
             << std::endl;

   return swapElementFiles(mPath + "startup/sim.analogdevice1.jconf",
                           mPath + "sim.analogdevice1.reconfig.jconf");
}

bool reconfigApp::reconfigSimAnalog_check()
{
   //Get the sim analog pointer
   boost::shared_ptr<gadget::SimAnalog> device =
      boost::dynamic_pointer_cast<gadget::SimAnalog>(
         gadget::InputManager::instance()->getDevice("AnalogDevice1")
      );

   if ( device.get() == NULL )
   {
      std::cout << "\tError: Could not find the sim analog device"
                << std::endl;
      return false;
   }

   //Get the vector of keymodpairs
   std::vector<gadget::SimInput::KeyModPair> keypairsup = device->getUpKeys();
   std::vector<gadget::SimInput::KeyModPair> keypairsdown =
      device->getDownKeys();

   //Load up the config file.
   jccl::Configuration cfg;
   cfg.load(mPath + "sim.analogdevice1.reconfig.jconf");
   std::vector<jccl::ConfigElementPtr> file_elements;
   cfg.getByType("simulated_analog_device", file_elements);

   bool key_status;

   //Scan the sim digital config element to see if was defined there
   for ( unsigned int i=0; i < file_elements[0]->getNum("key_pair"); ++i )
   {
      key_status = false;

      jccl::ConfigElementPtr key_mod_pair =
         file_elements[0]->getProperty<jccl::ConfigElementPtr>("key_pair", i);

      //Scan the up keys
      for ( unsigned int j = 0; j < keypairsup.size(); ++j )
      {
         //If they are the same..
         if ( keypairsup[j].mModifier == key_mod_pair->getProperty<int>("modifier_key", 0) &&
              keypairsup[j].mKey == key_mod_pair->getProperty<int>("key", 0) )
         {
            key_status = true;
            continue;
         }
      }

      //Scan the down keys
      for ( unsigned int j = 0; j < keypairsdown.size(); ++j )
      {
         //If they are the same..
         if ( keypairsdown[j].mModifier == key_mod_pair->getProperty<int>("modifier_key", 0) &&
              keypairsdown[j].mKey == key_mod_pair->getProperty<int>("key", 0) )
         {
            key_status = true;
            continue;
         }
      }

      //If we have not found a key that matches...
      if ( key_status == false )
      {
         std::cout << "\tError: One or more of the key pair definitions are incorrect"
                   << std::endl;
         return false;
      }
   }

   return true;
}

bool reconfigApp::addSimDigital_exec()
{
   std::cout << "Beginning test for adding a sim digital device and pointing proxies at it..."
             << std::endl;

   return addElementFile(mPath + "sim.digitaldevice.jconf") &&
          addElementFile(mPath + "sim.digitalproxy.jconf");
}

bool reconfigApp::addSimDigital_check()
{
   return verifyProxy("DigitalProxy", "ExtraDigitalDevice");
}

bool reconfigApp::removeSimDigital_exec()
{
   std::cout << "Beginning test for removing a sim digital device..."
             << std::endl;
   return removeElementFile(mPath + "startup/sim.wandbuttonsdigital.jconf");
}

bool reconfigApp::removeSimDigital_check()
{
   //Look at the button proxy and see what it points at
   gadget::ProxyPtr proxy =
      gadget::InputManager::instance()->getProxy("Button0Proxy");

   if ( proxy == NULL )
   {
      std::cout << "\tError: Could not find the proxy" << std::endl;
      return false;
   }

   if ( ! proxy->isStupefied() )
   {
      std::cout << "\tError: Proxy is not Stupefied" << std::endl;
      return false;
   }

   return true;
}

bool reconfigApp::readdSimDigital_exec()
{
   std::cout << "Beginning test for readding a sim digital device and checking the proxies pointing at it..."
             << std::endl;

   return addElementFile(mPath + "startup/sim.wandbuttonsdigital.jconf");
}

bool reconfigApp::readdSimDigital_check()
{
   return verifyProxy("Button0Proxy", "SimWandButtons") &&
          verifyProxy("Button1Proxy", "SimWandButtons") &&
          verifyProxy("Button2Proxy", "SimWandButtons") &&
          verifyProxy("Button3Proxy", "SimWandButtons") &&
          verifyProxy("Button4Proxy", "SimWandButtons") &&
          verifyProxy("Button5Proxy", "SimWandButtons");
}

bool reconfigApp::addSimAnalog_exec()
{
   std::cout << "Beginning test for adding a sim analog device and pointing proxies at it..."
             << std::endl;

   return addElementFile(mPath + "sim.extraanalogdevice.jconf") &&
          addElementFile(mPath + "sim.extraanalogproxy.jconf");
}

bool reconfigApp::addSimAnalog_check()
{
   return verifyProxy("ExtraAnalogProxy", "ExtraAnalogDevice");
}

bool reconfigApp::removeSimAnalog_exec()
{
   std::cout << "Beginning test for removing a sim analog device and checking the proxies that point at it..."
             << std::endl;
   return removeElementFile(mPath + "sim.extraanalogdevice.jconf");
}

bool reconfigApp::removeSimAnalog_check()
{
   //Look at the button proxy and see what it points at
   gadget::ProxyPtr proxy =
      gadget::InputManager::instance()->getProxy("ExtraAnalogProxy");

   if (proxy == NULL)
   {
      std::cout << "\tError: Could not find the proxy" << std::endl;
      return false;
   }

   if (!proxy->isStupefied())
   {
      std::cout << "\tError: Proxy is not Stupefied" << std::endl;
      return false;
   }

   return true;
}

bool reconfigApp::readdSimAnalog_exec()
{
   std::cout << "Beginning test for readding a sim analog device and checking the old proxies..."
             << std::endl;
   return addElementFile(mPath + "sim.extraanalogdevice.jconf");
}

bool reconfigApp::readdSimAnalog_check()
{
   return verifyProxy("ExtraAnalogProxy", "ExtraAnalogDevice");
}

bool reconfigApp::addAnalogProxy_exec()
{
   std::cout << "Beginning test for adding an analog proxy..." << std::endl;
   return addElementFile(mPath + "sim.analogproxy01.jconf");
}

bool reconfigApp::addAnalogProxy_check()
{
   return verifyProxy("AnalogProxy01", "AnalogDevice1");
}

bool reconfigApp::reconfigAnalogProxy_exec()
{
   std::cout << "Beginning test for reconfiguring an analog proxy..."
             << std::endl;
   return swapElementFiles(mPath + "sim.analogproxy01.jconf",
                           mPath + "sim.analogproxy01.reconfig.jconf");
}

bool reconfigApp::reconfigAnalogProxy_check()
{
   return verifyProxy("AnalogProxy01", "AnalogDevice2");
}

bool reconfigApp::removeAnalogProxy_exec()
{
   std::cout << "Beginning test for removing an analog proxy..." << std::endl;
   return removeElementFile(mPath + "sim.analogproxy01.reconfig.jconf");
}

bool reconfigApp::removeAnalogProxy_check()
{
   //Try to get the new proxy and check its values
   if ( gadget::InputManager::instance()->getProxy("AnalogProxy01") != NULL )
   {
      std::cout << "\tError: AnalogProxy01 still exists" << std::endl;
      return false;
   }

   return true;
}

bool reconfigApp::addPosProxy_exec()
{
   std::cout << "Beginning test for adding a sim digital device..."
             << std::endl;
   return addElementFile(mPath + "sim.simcam1proxy02.jconf");
}

bool reconfigApp::addPosProxy_check()
{
   return verifyProxy("SimCam1 Proxy02", "SimCamera1");
}

bool reconfigApp::reconfigPosProxy_exec()
{
   std::cout << "Beginning test for reconfiguring a position proxy..."
             << std::endl;
   return swapElementFiles(mPath + "sim.simcam1proxy02.jconf",
                           mPath + "sim.simcam1proxy02.reconfig.jconf");
}

bool reconfigApp::reconfigPosProxy_check()
{
   return verifyProxy("SimCam1 Proxy02", "SimCamera2");
}

bool reconfigApp::removePosProxy_exec()
{
   std::cout << "Beginning test for removing a position proxy..."
             << std::endl;
   return removeElementFile(mPath + "sim.simcam1proxy02.reconfig.jconf");
}

bool reconfigApp::removePosProxy_check()
{
   //Try to get the new proxy and check its values
   if ( gadget::InputManager::instance()->getProxy("SimCam1 Proxy02") != NULL )
   {
      std::cout << "\tError: SimCam1 Proxy02 still exists" << std::endl;
      return false;
   }

   return true;
}

bool reconfigApp::addDigitalProxy_exec()
{
   std::cout << "Beginning test for adding a digital proxy..." << std::endl;
   return addElementFile(mPath + "sim.button0proxy02.jconf");
}

bool reconfigApp::addDigitalProxy_check()
{
   return verifyProxy("Button0Proxy02", "SimWandButtons");
}

bool reconfigApp::reconfigDigitalProxy_exec()
{
   std::cout << "Beginning test for reconfiguring a digital proxy..."
             << std::endl;
   return swapElementFiles(mPath + "sim.button0proxy02.jconf",
                           mPath + "sim.button0proxy02.reconfig.jconf");
}

bool reconfigApp::reconfigDigitalProxy_check()
{
   return verifyProxy("Button0Proxy02", "SimWandButtons02");
}

bool reconfigApp::removeDigitalProxy_exec()
{
   std::cout << "Beginning test for removing a digital proxy..." << std::endl;
   return removeElementFile(mPath + "sim.button0proxy02.reconfig.jconf");
}

bool reconfigApp::removeDigitalProxy_check()
{
   //Try to get the new proxy and check its values
   if ( gadget::InputManager::instance()->getProxy("Button0Proxy02") != NULL )
   {
      std::cout << "\tError: Button0 proxy 02 still exists" << std::endl;
      return false;
   }

   return true;
}

bool reconfigApp::addKeyboardProxy_exec()
{
   std::cout << "Beginning test for adding a keyboard proxy..." << std::endl;
   return addElementFile(mPath + "sim.wandkeyboardproxy02.jconf");
}

bool reconfigApp::addKeyboardProxy_check()
{
   return verifyProxy("WandKeyboardProxy02", "Wand Keyboard");
}

bool reconfigApp::reconfigKeyboardProxy_exec()
{
   std::cout << "Beginning test for reconfiguring a keyboard proxy..."
             << std::endl;
   return swapElementFiles(mPath + "sim.wandkeyboardproxy02.jconf",
                           mPath + "sim.wandkeyboardproxy02.reconfig.jconf");
}

bool reconfigApp::reconfigKeyboardProxy_check()
{
   return verifyProxy( "WandKeyboardProxy02", "Head Keyboard" );
}


bool reconfigApp::removeKeyboardProxy_exec()
{
   std::cout << "Beginning test for removing a keyboard proxy...\n" << std::flush;
   return removeElementFile( mPath + "sim.wandkeyboardproxy02.reconfig.jconf" );
}

bool reconfigApp::removeKeyboardProxy_check()
{
   //Try to get the new proxy and check its values
   if ( gadget::InputManager::instance()->getProxy("WandKeyboardProxy02") != NULL )
   {
      std::cout << "\tError: Keyboard proxy 02 still exists" << std::endl;
      return false;
   }

   return true;
}

bool reconfigApp::addStupefiedAnalogProxy_exec()
{
   std::cout << "Beginning test for adding a Stupefied analog proxy..."
             << std::endl;
   return addElementFile(mPath + "sim.stupidproxy01.jconf");
}

bool reconfigApp::addStupefiedAnalogProxy_check()
{
   //Try to get the new proxy and check its values
   gadget::ProxyPtr tempProxy =
      gadget::InputManager::instance()->getProxy("StupidAnalogProxy01");
   gadget::AnalogProxyPtr proxy =
      boost::dynamic_pointer_cast<gadget::AnalogProxy>(tempProxy);

   if ( proxy == NULL )
   {
      std::cout << "\tError: Could not find the proxy" << std::endl;
      return false;
   }

   if ( ! proxy->isStupefied() )
   {
      std::cout << "\tError: Proxy is not Stupefied" << std::endl;
      return false;
   }

   if ( proxy->getData() != 0.0f )
   {
   }

   if ( proxy->getProxiedInputDevice() != NULL )
   {
      std::cout << "\tError: Proxy points at a device" << std::endl;
      return false;
   }

   if ( proxy->getElementType() != gadget::AnalogProxy::getElementType() )
   {
      std::cout << "\tError: Proxy has the wrong element type: "
                << proxy->getElementType() << "   It should be "
                << gadget::AnalogProxy::getElementType() << "\n"
                << std::flush;
      return false;
   }

   return true;
}
bool reconfigApp::removeStupefiedAnalogProxy_exec()
{
   std::cout << "Beginning test for removing a Stupefied analog proxy..."
             << std::endl;
   return removeElementFile(mPath + "sim.stupidproxy01.jconf");
}

bool reconfigApp::removeStupefiedAnalogProxy_check()
{
   //Try to get the new proxy and check its values
   if ( gadget::InputManager::instance()->getProxy("StupidAnalogProxy01") != NULL )
   {
      std::cout << "\tError: Stupid Proxy 01 still exists" << std::endl;
      return false;
   }

   return true;
}
