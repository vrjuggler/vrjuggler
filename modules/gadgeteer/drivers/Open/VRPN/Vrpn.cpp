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

//=====================================================
// Vrpn
//
// Purpose:
//  VR Juggler VRPN
//
// Author:
//  Jason Jerald   8-26-02
// Revisions:
//      Ported to to 1.1 (DR2) and sgi platform Anthony Steed, 10-4-02.
//      Simplified to one class to deal with threading issues.
//
// ToDo:
//   Should we fetch the room->tracker_base and tracker->hand transforms
//    from the VRPN config system, or always set them in VR Juggler. Or just
//    set room->juggler in VR Juggler?
//
//   Other VRPN devices.
//
//   Tidy up variables, there is some redundancy
//
//   Consider doing pre-rotate in posiiton proxy tracker offset. Though
//    note there are no handedness changes in position proxy.
//======================================================

#include <gadget/Devices/DriverConfig.h>

#include <boost/bind.hpp>

#include <gmtl/Matrix.h>
#include <gmtl/MatrixOps.h>
#include <gmtl/Generate.h>
#include <gmtl/Quat.h>
#include <gmtl/Output.h>

#include <vpr/vpr.h>
#include <vpr/Sync/Guard.h>
#include <vpr/Util/Debug.h>
#include <vpr/Util/Assert.h>
#include <vpr/System.h>
#include <jccl/Config/ConfigElement.h>
#include <gadget/InputManager.h>
#include <gadget/gadgetParam.h>
#include <gadget/Type/DeviceConstructor.h>
#include <gadget/Util/Debug.h>

#include <drivers/Open/VRPN/Vrpn.h>
#include "VrpnRumbleEffect.h"

extern "C"
{

GADGET_DRIVER_EXPORT(vpr::Uint32) getGadgeteerVersion()
{
   return __GADGET_version;
}

GADGET_DRIVER_EXPORT(void) initDevice(gadget::InputManager* inputMgr)
{
   new gadget::DeviceConstructor<gadget::Vrpn>(inputMgr);
}

}

namespace gadget
{

void VRPN_CALLBACK handleTrackerChange(void* userdata, vrpn_TRACKERCB t)
{
   vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_STATE_LVL)
      << "VRPN driver handleTrackerChange() called\n" << vprDEBUG_FLUSH;

   gadget::Vrpn* this_ptr = static_cast<gadget::Vrpn*>(userdata);
   this_ptr->trackerChange(t);
}

void VRPN_CALLBACK handleButtonChange(void* userdata, vrpn_BUTTONCB b)
{
   vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_STATE_LVL)
      << "VRPN driver handleButtonChange() called\n" << vprDEBUG_FLUSH;

   gadget::Vrpn* this_ptr = static_cast<gadget::Vrpn*>(userdata);
   this_ptr->buttonChange(b);
}

void VRPN_CALLBACK handleAnalogChange(void* userdata, vrpn_ANALOGCB b)
{
   vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_STATE_LVL)
      << "VRPN driver handleAnalogChange() called\n" << vprDEBUG_FLUSH;

   gadget::Vrpn* this_ptr = static_cast<gadget::Vrpn*>(userdata);
   this_ptr->analogChange(b);
}

Vrpn::Vrpn()
   : mExitFlag(false)
   , mTrackerNumber(0)
   , mButtonNumber(0)
   , mAnalogNumber(0)
{
   /* Do nothing. */ ;
}

Vrpn::~Vrpn()
{
   stopSampling();
}

bool Vrpn::config(jccl::ConfigElementPtr e)
{
   const unsigned int min_def_version(2);

   if ( e->getVersion() < min_def_version )
   {
      vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_WARNING_LVL)
         << clrOutBOLD(clrYELLOW, "WARNING:") << " Element named '"
         << e->getName() << "'" << std::endl;
      vprDEBUG_NEXTnl(gadgetDBG_INPUT_MGR, vprDBG_WARNING_LVL)
         << "is version " << e->getVersion()
         << ", but we expect at least version " << min_def_version
         << ".\n";
      vprDEBUG_NEXTnl(gadgetDBG_INPUT_MGR, vprDBG_WARNING_LVL)
         << "Default values will be used for some settings.\n"
         << vprDEBUG_FLUSH;
   }

   if ( ! (Input::config(e) && Position::config(e) && Digital::config(e) &&
           Analog::config(e)) )
   {
      return false;
   }

   // Get the name of the VRPN tracker server.
   mTrackerServer = e->getProperty<std::string>("tracker_server");
   // Get the number of tracked objects.
   mTrackerNumber = e->getProperty<int>("tracker_count");

   if ( mTrackerNumber > 0 )
   {
      if ( mTrackerServer.empty() )
      {
         vprDEBUG(vprDBG_ALL, vprDBG_WARNING_LVL)
            << "[Vrpn::config()] VRPN tracker server name not set!\n"
            << vprDEBUG_FLUSH;
         mTrackerNumber = 0;
      }
      else
      {
         vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_CONFIG_LVL)
            << "VRPN driver set to read " << mTrackerNumber
            << " tracker values from '" << mTrackerServer << "'\n"
            << vprDEBUG_FLUSH;
      }

      mPositions.resize(mTrackerNumber);
      mQuats.resize(mTrackerNumber);
   }

   // Get the name of the VRPN button server.
   mButtonServer = e->getProperty<std::string>("button_server");
   // Get the number of buttons.
   mButtonNumber = e->getProperty<int>("button_count");

   if ( mButtonNumber > 0 )
   {
      if ( mButtonServer.empty() )
      {
         vprDEBUG(vprDBG_ALL, vprDBG_WARNING_LVL)
            << "[Vrpn::config()] VRPN button server name not set!\n"
            << vprDEBUG_FLUSH;
         mButtonNumber = 0;
      }
      else
      {
         vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_CONFIG_LVL)
            << "VRPN driver set to read " << mButtonNumber
            << " button values from '" << mButtonServer << "'\n"
            << vprDEBUG_FLUSH;
      }

      mButtons.resize(mButtonNumber);
   }

   // Get the name of the VRPN analog server.
   mAnalogServer = e->getProperty<std::string>("analog_server");
   // Get the number of analogs.
   mAnalogNumber = e->getProperty<int>("analog_count");

   if ( mAnalogNumber > 0 )
   {
      if ( mAnalogServer.empty() )
      {
         vprDEBUG(vprDBG_ALL, vprDBG_WARNING_LVL)
            << "[Vrpn::config()] VRPN analog server name not set!\n"
            << vprDEBUG_FLUSH;
         mAnalogNumber = 0;
      }
      else
      {
         vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_CONFIG_LVL)
            << "VRPN driver set to read " << mAnalogNumber
            << " analog values from '" << mAnalogServer << "'\n"
            << vprDEBUG_FLUSH;
      }

      mAnalogs.resize(mAnalogNumber);
   }

   return true;
}

bool Vrpn::startSampling()
{
   bool started(false);

   if ( NULL == mThread )
   {
      // Set flags and spawn sample thread
      mExitFlag = false;

      vprDEBUG(vprDBG_ALL, vprDBG_CONFIG_LVL) 
         << "[VRPN] Spawning control thread." << std::endl  << vprDEBUG_FLUSH;
      
      try
      {
         mThread = new vpr::Thread(boost::bind(&Vrpn::readLoop, this));
         started = true;

         vprDEBUG(vprDBG_ALL, vprDBG_CONFIG_LVL)
            << "[VRPN] VRPN driver is active." << std::endl
            << vprDEBUG_FLUSH;
      }
      catch (vpr::Exception& ex)
      {
         vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_CRITICAL_LVL)
            << clrOutBOLD(clrRED, "ERROR")
            << ": Failed to spawn thread for VRPN driver!\n" << vprDEBUG_FLUSH;
         vprDEBUG_NEXT(gadgetDBG_INPUT_MGR, vprDBG_CRITICAL_LVL)
            << ex.what() << std::endl << vprDEBUG_FLUSH;
      }
   }

   return started;
}

bool Vrpn::stopSampling()
{
   if ( NULL != mThread )
   {
      vprDEBUG(vprDBG_ALL, vprDBG_CRITICAL_LVL)
         << "[Vrpn::stopSampling()] Stopping sample thread" << std::endl
         << vprDEBUG_FLUSH;

      mExitFlag = true;

      // Wait for thread to exit..
      mThread->join();
      vprDEBUG(vprDBG_ALL, vprDBG_CRITICAL_LVL)
         << "[Vrpn::stopSampling()] Sample thread stopped" << std::endl
         << vprDEBUG_FLUSH;
      delete mThread;
      mThread = NULL;
   }
   return true;
}

bool Vrpn::sample()
{
   if ( mTrackerNumber > 0 )
   {
      std::vector<PositionData> positions(mTrackerNumber);
      vpr::Guard<vpr::Mutex> g(mTrackerMutex);

      for ( int i = 0; i < mTrackerNumber; ++i )
      {
         gmtl::Matrix44f pos;
         gmtl::setRot(pos, mQuats[i]);
         gmtl::setTrans(pos, mPositions[i]);

         positions[i].setValue(pos);
         positions[i].setTime();
      }

      addPositionSample(positions);
   }

   if ( mButtonNumber > 0 )
   {
      std::vector<DigitalData> buttons(mButtonNumber);
      vpr::Guard<vpr::Mutex> g(mButtonMutex);

      for ( int i = 0; i < mButtonNumber; ++i )
      {
         buttons[i] = mButtons[i];
         buttons[i].setTime();
      }

      addDigitalSample(buttons);
   }

   if ( mAnalogNumber > 0 )
   {
      std::vector<AnalogData> analogs(mAnalogNumber);
      vpr::Guard<vpr::Mutex> g(mAnalogMutex);

      for ( int i = 0; i < mAnalogNumber; ++i )
      {
         analogs[i] = mAnalogs[i];
         analogs[i].setTime();
      }

      addAnalogSample(analogs);
   }

   return true;
}

void Vrpn::updateData()
{
   // Sample the data from the device.
   // NOTE: This is done here because the readLoop is asyncronous and
   //       if we place it there we get very large buffer sizes.
   sample();
   
   // Swap it
   swapPositionBuffers();
   swapDigitalBuffers();
   swapAnalogBuffers();
}

std::string Vrpn::getElementType()
{
   return "vrpn";
}

unsigned int Vrpn::getCapabilities()
{
   return gadget::RumbleEffect::CONSTANT;
}

void Vrpn::startRumble()
{
   vprASSERT(mAnalogOutHandle);
   vpr::Guard<vpr::Mutex> g(mVrpnConnectionMutex);

   // Send 1.0 to channel 0 to turn on rumble on WiiMote
   mAnalogOutHandle->request_change_channel_value(0, 1.0);
}

void Vrpn::stopRumble()
{
   vprASSERT(mAnalogOutHandle);
   vpr::Guard<vpr::Mutex> g(mVrpnConnectionMutex);
   
   // Send 0.0 to channel 0 to turn off rumble on WiiMote
   mAnalogOutHandle->request_change_channel_value(0, 0.0);
}

RumbleEffectPtr Vrpn::createEffectImp(RumbleEffect::RumbleType type)
{
   boost::ignore_unused_variable_warning(type);
   RumbleEffectPtr ret;

   switch (type)
   {
   case RumbleEffect::CONSTANT:
      ret.reset(new VrpnRumbleEffect(this, RumbleEffect::CONSTANT));
      break;
   case RumbleEffect::SQUARE:
      ret.reset(new VrpnRumbleEffect(this, RumbleEffect::SQUARE));
      break;
   }

   return ret;
}

void Vrpn::readLoop()
{
   boost::shared_ptr<vrpn_Tracker_Remote> tracker_handle;
   boost::shared_ptr<vrpn_Button_Remote> button_handle;
   boost::shared_ptr<vrpn_Analog_Remote> analog_handle;
   if ( mTrackerNumber > 0 )
   {
      tracker_handle.reset(new vrpn_Tracker_Remote(mTrackerServer.c_str()));
      tracker_handle->register_change_handler(static_cast<void*>(this),
                                              &handleTrackerChange);
   }

   if ( mButtonNumber > 0 )
   {
      button_handle.reset(new vrpn_Button_Remote(mButtonServer.c_str()));
      button_handle->register_change_handler(static_cast<void*>(this),
                                             &handleButtonChange);
   }

   if ( mAnalogNumber > 0 )
   {
      analog_handle.reset(new vrpn_Analog_Remote(mAnalogServer.c_str()));
      mAnalogOutHandle.reset(new vrpn_Analog_Output_Remote(mAnalogServer.c_str()));
      analog_handle->register_change_handler(static_cast<void*>(this),
                                             &handleAnalogChange);
   }

   // If there is nothing from which to read data, there is no point
   // in continuing to run this thread.
   if ( !tracker_handle && !button_handle && !analog_handle)
   {
      return;
   }

   // loop through  and keep sampling
   while ( ! mExitFlag )
   {
      vpr::Guard<vpr::Mutex> g(mVrpnConnectionMutex);

      if (tracker_handle)
      {
         tracker_handle->mainloop();
      }

      if (button_handle)
      {
         button_handle->mainloop();
      }

      if (analog_handle)
      {
         analog_handle->mainloop();
      }
      if (mAnalogOutHandle)
      {
         mAnalogOutHandle->mainloop();
      }

      vpr::Thread::yield();
      // yield and take a brief sleep
      vpr::System::msleep(1);
   }

   mTrackerNumber = 0;
   if (tracker_handle)
   {
      tracker_handle->unregister_change_handler(static_cast<void*>(this),
                                                &handleTrackerChange);
   }

   mButtonNumber = 0;
   if (button_handle)
   {
      button_handle->unregister_change_handler(static_cast<void*>(this),
                                               &handleButtonChange);
   }

   mAnalogNumber = 0;
   if (analog_handle)
   {
      analog_handle->unregister_change_handler(static_cast<void *>(this),
                                               &handleAnalogChange);
   }

   mAnalogOutHandle.reset();
}

void Vrpn::trackerChange(const vrpn_TRACKERCB& t)
{
   vpr::Guard<vpr::Mutex> g(mTrackerMutex);

   if ( t.sensor > mTrackerNumber )
   {
      vprDEBUG(vprDBG_ALL, vprDBG_CONFIG_LVL)
         << "Vrpn: tracker " << t.sensor
         << " out of declared range (" << mPositions.size() << ")" << std::endl
         << vprDEBUG_FLUSH;
      mPositions.resize(t.sensor + 1);
      mQuats.resize(t.sensor + 1);
   }

   vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_VERB_LVL)
      << "Tracker #" << t.sensor << " quat " << mQuats[t.sensor] << std::endl
      << vprDEBUG_FLUSH;

   mQuats[t.sensor][0] = t.quat[0];
   mQuats[t.sensor][1] = t.quat[1];
   mQuats[t.sensor][2] = t.quat[2];
   mQuats[t.sensor][3] = t.quat[3];

   mPositions[t.sensor][0] = t.pos[0];
   mPositions[t.sensor][1] = t.pos[1];
   mPositions[t.sensor][2] = t.pos[2];
}

void Vrpn::buttonChange(const vrpn_BUTTONCB& b)
{
   vpr::Guard<vpr::Mutex> g(mButtonMutex);

   if ( b.button > mButtonNumber )
   {
      vprDEBUG(vprDBG_ALL, vprDBG_CONFIG_LVL)
         << "Vrpn: button " << b.button
         << " out of declared range ("<<mButtons.size()<<")"<<std::endl
         << vprDEBUG_FLUSH;
      mButtons.resize(b.button + 1);
   }

   vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_VERB_LVL)
      << "Button #" << b.button << " state " << b.state << std::endl
      << vprDEBUG_FLUSH;

   mButtons[b.button] = static_cast<DigitalState::State>(b.state);
}

void Vrpn::analogChange(const vrpn_ANALOGCB& b)
{
   vpr::Guard<vpr::Mutex> g(mAnalogMutex);

   if ( b.num_channel > mAnalogNumber )
   {
      vprDEBUG(vprDBG_ALL, vprDBG_CONFIG_LVL)
         << "Vrpn: analog channel size " << b.num_channel
         << " out of declared range (" << mAnalogs.size() << ")" << std::endl
         << vprDEBUG_FLUSH;
      mAnalogs.resize(b.num_channel + 1);
   }

   for ( int i = 0; i < b.num_channel; ++i )
   {
      vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_VERB_LVL)
         << "Analog #" << i << " value " << b.channel[i] << std::endl
         << vprDEBUG_FLUSH;

      mAnalogs[i] = b.channel[i];
   }
}
} // End of gadget namespace
