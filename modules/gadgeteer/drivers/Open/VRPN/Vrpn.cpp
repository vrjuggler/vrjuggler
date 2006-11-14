/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2006 by Iowa State University
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
//   Analog and other VRPN devices.
//
//   Tidy up variables, there is some redundancy
//
//   Consider doing pre-rotate in posiiton proxy tracker offset. Though
//    note there are no handedness changes in position proxy.
//======================================================

#include <gadget/Devices/DriverConfig.h>

#include <boost/bind.hpp>

#include <gmtl/Matrix.h>
#include <gmtl/Vec.h>
#include <gmtl/MatrixOps.h>
#include <gmtl/Generate.h>
#include <gmtl/EulerAngle.h>
#include <gmtl/Quat.h>
#include <gmtl/AxisAngle.h>

#include <vpr/vpr.h>
#include <vpr/Util/Debug.h>
#include <jccl/Config/ConfigElement.h>
#include <gadget/InputManager.h>
#include <gadget/gadgetParam.h>
#include <gadget/Type/DeviceConstructor.h>
#include <gadget/Util/Debug.h>

#include <drivers/Open/VRPN/Vrpn.h>


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

void VRPN_CALLBACK staticHandleTracker(void* userdata, vrpn_TRACKERCB t)
{
   vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_STATE_LVL)
      << "VRPN driver staticHanldeTracker() called\n" << vprDEBUG_FLUSH;

   gadget::Vrpn* this_ptr = static_cast<gadget::Vrpn*>(userdata);
   this_ptr->handleTracker(t);
}

void VRPN_CALLBACK staticHandleButton(void* userdata, vrpn_BUTTONCB b)
{
   vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_STATE_LVL)
      << "VRPN driver staticHanldeButton() called\n" << vprDEBUG_FLUSH;

   gadget::Vrpn* this_ptr = static_cast<gadget::Vrpn*>(userdata);
   this_ptr->handleButton(b);
}

void VRPN_CALLBACK staticHandleAnalog(void* userdata, vrpn_ANALOGCB b)
{
   vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_STATE_LVL)
      << "VRPN driver staticHanldeAnalog() called\n" << vprDEBUG_FLUSH;

   gadget::Vrpn* this_ptr = static_cast<gadget::Vrpn*>(userdata);
   this_ptr->handleAnalog(b);
}

std::string Vrpn::getElementType()
{
   return "vrpn";
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

Vrpn::~Vrpn()
{
   stopSampling();
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

void Vrpn::readLoop()
{
   vrpn_Tracker_Remote* tracker(NULL);
   vrpn_Button_Remote* button(NULL);
   vrpn_Analog_Remote* analog(NULL);

   if ( mTrackerNumber > 0 )
   {
      tracker = new vrpn_Tracker_Remote(mTrackerServer.c_str());
      tracker->register_change_handler((void*) this, staticHandleTracker);
   }

   if ( mButtonNumber > 0 )
   {
      button = new vrpn_Button_Remote(mButtonServer.c_str());
      button->register_change_handler((void*) this, staticHandleButton);
   }

   if ( mAnalogNumber > 0 )
   {
      analog = new vrpn_Analog_Remote(mAnalogServer.c_str());
      analog->register_change_handler((void*) this, staticHandleAnalog);
   }

   // loop through  and keep sampling
   while ( ! mExitFlag )
   {
      if ( mTrackerNumber > 0 )
      {
         tracker->mainloop();
      }

      if ( mButtonNumber > 0 )
      {
         button->mainloop();
      }

      if ( mAnalogNumber > 0 )
      {
         analog->mainloop();
      }

      vpr::Thread::yield();
   }
}

void Vrpn::handleTracker(const vrpn_TRACKERCB& t)
{
   if ( t.sensor > mTrackerNumber )
   {
      vprDEBUG(vprDBG_ALL, vprDBG_CONFIG_LVL)
         << "Vrpn: tracker " << t.sensor
         << " out of declared range (" << mPositions.size() << ")" << std::endl
         << vprDEBUG_FLUSH;
      mPositions.resize(t.sensor);
      mQuats.resize(t.sensor);
   }

   vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_VERB_LVL)
      << "Tracker #"<< t.sensor << " quat "
      << mQuats[t.sensor][0] << " "
      << mQuats[t.sensor][1] << " "
      << mQuats[t.sensor][2] << " "
      << mQuats[t.sensor][3] << std::endl << vprDEBUG_FLUSH;

   mQuats[t.sensor][0] = t.quat[0];
   mQuats[t.sensor][1] = t.quat[1];
   mQuats[t.sensor][2] = t.quat[2];
   mQuats[t.sensor][3] = t.quat[3];

   mPositions[t.sensor][0] = t.pos[0];
   mPositions[t.sensor][1] = t.pos[1];
   mPositions[t.sensor][2] = t.pos[2];
}

void Vrpn::handleButton(const vrpn_BUTTONCB& b)
{
   if ( b.button > mButtonNumber )
   {
      vprDEBUG(vprDBG_ALL, vprDBG_CONFIG_LVL)
         << "Vrpn: button " << b.button
         << " out of declared range ("<<mButtons.size()<<")"<<std::endl
         << vprDEBUG_FLUSH;
      mButtons.resize(b.button);
   }

   vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_VERB_LVL)
      << "Button #" << b.button << " state " << b.state << std::endl
      << vprDEBUG_FLUSH;

   mButtons[b.button] = b.state;
}

void Vrpn::handleAnalog(const vrpn_ANALOGCB& b)
{
   if ( b.num_channel > mAnalogNumber )
   {
      vprDEBUG(vprDBG_ALL, vprDBG_CONFIG_LVL)
         << "Vrpn: analog channel size " << b.num_channel
         << " out of declared range (" << mAnalogs.size() << ")" << std::endl
         << vprDEBUG_FLUSH;
      mAnalogs.resize(b.num_channel);
   }

   for ( int i = 0; i < b.num_channel; ++i )
   {
      vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_VERB_LVL)
	 << "Analog #" << i << " value " << b.channel[i] << std::endl
	 << vprDEBUG_FLUSH;

      float value(0.0f);
      normalizeMinToMax(b.channel[i], value);
      mAnalogs[i] = value;
   }
}

bool Vrpn::sample()
{
   if ( mTrackerNumber > 0 )
   {
      std::vector<PositionData> positions(mTrackerNumber);

      for ( int i = 0; i < mTrackerNumber; ++i )
      {
         positions[i].setPosition(getSensorPos(i));
         positions[i].setTime();
      }

      addPositionSample(positions);
   }

   if ( mButtonNumber > 0 )
   {
      std::vector<DigitalData> buttons(mButtonNumber);

      for ( int i = 0; i < mButtonNumber; ++i )
      {
         buttons[i] = getDigitalData(i);
         buttons[i].setTime();
      }

      addDigitalSample(buttons);
   }

   if ( mAnalogNumber > 0 )
   {
      std::vector<AnalogData> analogs(mAnalogNumber);

      for ( int i = 0; i < mAnalogNumber; ++i )
      {
         analogs[i] = getAnalogData(i);
         analogs[i].setTime();
      }

      addAnalogSample(analogs);
   }

   return true;
}

bool Vrpn::stopSampling()
{
   if ( mThread != NULL )
   {
      mExitFlag = true;

      // Wait for thread to exit..
      mThread->join();
      delete mThread;
      mThread = NULL;
      vprDEBUG(vprDBG_ALL, vprDBG_CONFIG_LVL)
         << "[vrpn] stopping driver..." << std::endl << vprDEBUG_FLUSH;
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

gmtl::Matrix44f Vrpn::getSensorPos(const unsigned int i)
{
   gmtl::Matrix44f ret_val;
   gmtl::setRot(ret_val, mQuats[i]);
   gmtl::setTrans(ret_val, mPositions[i]);
   return ret_val;
}

gadget::DigitalData Vrpn::getDigitalData(const unsigned int i)
{
   return mButtons[i];
}

gadget::AnalogData Vrpn::getAnalogData(const unsigned int i)
{
   return mAnalogs[i];
}

} // End of gadget namespace
