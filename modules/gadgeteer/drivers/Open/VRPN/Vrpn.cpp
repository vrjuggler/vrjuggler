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

#include <boost/concept_check.hpp>

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

#include <drivers/Open/VRPN/Vrpn.h>

#define VRPN_DEBUG 0
// 1 == reporting
// 2 == misc

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

void staticHandleTracker(void *userdata, vrpn_TRACKERCB t)
{

#if (VRPN_DEBUG&1)
   std::cout << "HandleTracker" << std::endl;
#endif

   gadget::Vrpn *this_ptr = static_cast<gadget::Vrpn*>(userdata);
   this_ptr->handleTracker(t);
}

void staticHandleButton(void *userdata, vrpn_BUTTONCB b)
{
#if (VRPN_DEBUG&1)
   std::cout << "HandleButton" << std::endl;
#endif
   gadget::Vrpn *this_ptr = static_cast<gadget::Vrpn*>(userdata);
   this_ptr->handleButton(b);
}

std::string Vrpn::getElementType()
{
   return "vrpn";
}

bool Vrpn::config(jccl::ConfigElementPtr e)
{
   if ( ! (Input::config(e) && Position::config(e)) )
   {
      return false;
   }

   // Get the name of the VRPN tracker server.
   mTrackerServer = e->getProperty<std::string>("tracker_server");
   if ( mTrackerServer == std::string("") )
   {
      vprDEBUG(vprDBG_ALL, vprDBG_CRITICAL_LVL)
         << "Vrpn::config() VRPN tracker server name not set!\n" << vprDEBUG_FLUSH;
   }
   else
   {
      vprDEBUG(vprDBG_ALL, vprDBG_CONFIG_STATUS_LVL)
         << "Vrpn::config() VRPN tracker server name set to: " << mTrackerServer
         << std::endl << vprDEBUG_FLUSH;
   }

   // Get the number of tracked objects.
   mTrackerNumber = e->getProperty<int>("tracker_count");

   // Get the name of the VRPN button server.
   mButtonServer = e->getProperty<std::string >("button_server");
   if ( mButtonServer == std::string("") )
   {
      vprDEBUG(vprDBG_ALL, vprDBG_CRITICAL_LVL)
         << "Vrpn::config() VRPN button server name not set!\n" << vprDEBUG_FLUSH;
   }
   else
   {
      vprDEBUG(vprDBG_ALL, vprDBG_CONFIG_STATUS_LVL)
         << "Vrpn::config() VRPN button server name set to: " << mButtonServer
         << std::endl << vprDEBUG_FLUSH;
   }

   // Get the number of buttons.
   mButtonNumber = e->getProperty<int>("button_count");

   // Resize vectors to hold the right amount of data.
   mPositions.resize(mTrackerNumber);
   mQuats.resize(mTrackerNumber);
   mButtons.resize(mButtonNumber);

   mCurPositions.resize(mTrackerNumber);
   mCurButtons.resize(mButtonNumber);

   return true;
}

Vrpn::~Vrpn()
{
   stopSampling();
}

bool Vrpn::startSampling()
{
   if (NULL != mThread)
   {
      // Already sampling
      return 0; 
   }
   else
   {
      // Set flags and spawn sample thread
      mExitFlag = false;

      vprDEBUG(vprDBG_ALL, vprDBG_CONFIG_LVL) 
         << "[VRPN] Spawning control thread." << std::endl  << vprDEBUG_FLUSH;
      
      vpr::ThreadMemberFunctor<Vrpn>* read_func =
         new vpr::ThreadMemberFunctor<Vrpn>(this, &Vrpn::readLoop, NULL);
      mReadThread = new vpr::Thread(read_func);
      if ( !mReadThread->valid() )
      {
         return 0;
      }
      else
      {
         vprDEBUG(vprDBG_ALL, vprDBG_CONFIG_LVL)
            << "[VRPN] VRPN driver is active." << std::endl
            << vprDEBUG_FLUSH;

         return 1;
      }
   }
}

void Vrpn::readLoop(void *nullParam)
{
   boost::ignore_unused_variable_warning(nullParam);

   vrpn_Tracker_Remote *tracker;
   vrpn_Button_Remote *button;

   if ( mTrackerNumber > 0 )
   {
      tracker = new vrpn_Tracker_Remote(mTrackerServer.c_str());
      tracker->register_change_handler((void *) this, staticHandleTracker);
   }

   if ( mButtonNumber > 0 )
   {
      button = new vrpn_Button_Remote(mButtonServer.c_str());
      button->register_change_handler((void *) this, staticHandleButton);
   }

   // loop through  and keep sampling
   while ( !mExitFlag )
   {
      if ( mTrackerNumber > 0 )
      {
         tracker->mainloop();
      }

      if ( mButtonNumber > 0 )
      {
         button->mainloop();
      }
      vpr::Thread::yield();
   }
}

void Vrpn::handleTracker(vrpn_TRACKERCB t)
{
   if ( t.sensor > mTrackerNumber )
   {
      vprDEBUG(vprDBG_ALL,vprDBG_CONFIG_LVL)
         << "Vrpn: tracker " << t.sensor
         << " out of declared range ("<<mPositions.size()<<")"<<std::endl
         << vprDEBUG_FLUSH;
      mPositions.resize(t.sensor);
      mQuats.resize(t.sensor);
   }

#if (VRPN_DEBUG&1)
   std::cout << "Tracker #"<<t.sensor<< " quat " <<
      mQuats[t.sensor][0] << " " <<
      mQuats[t.sensor][1] << " " <<
      mQuats[t.sensor][2] << " " <<
      mQuats[t.sensor][3] << " " << std::endl;

#endif

   mQuats[t.sensor][0] = t.quat[0];
   mQuats[t.sensor][1] = t.quat[1];
   mQuats[t.sensor][2] = t.quat[2];
   mQuats[t.sensor][3] = t.quat[3];

   mPositions[t.sensor][0] = t.pos[0];
   mPositions[t.sensor][1] = t.pos[1];
   mPositions[t.sensor][2] = t.pos[2];
}

void Vrpn::handleButton(vrpn_BUTTONCB b)
{
   if ( b.button > mButtonNumber )
   {
      vprDEBUG(vprDBG_ALL,vprDBG_CONFIG_LVL)
         << "Vrpn: button " << b.button
         << " out of declared range ("<<mButtons.size()<<")"<<std::endl
         << vprDEBUG_FLUSH;
      mButtons.resize(b.button);
   }
#if (VRPN_DEBUG&1)
   std::cout << "Button #"<<b.button<< " state " 
      << b.state << " " << std::endl;

#endif
   mButtons[b.button] = b.state;
}

bool Vrpn::sample()
{
   for ( int i=0;i<mTrackerNumber;i++ )
   {
      mCurPositions[i].setPosition(getSensorPos(i));
      mCurPositions[i].setTime();
   }

   for ( int i=0;i<mButtonNumber;i++ )
   {
      mCurButtons[i] = getDigitalData(i);
      mCurButtons[i].setTime();
   }

   // Update the data buffer
   addPositionSample(mCurPositions);
   addDigitalSample(mCurButtons);

   return 1;
}

bool Vrpn::stopSampling()
{
   if( mReadThread != NULL )
   {
      mExitFlag = true;

      // Wait for thread to exit..
      mReadThread->join();
      delete mReadThread;
      mReadThread = NULL;
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
}

gmtl::Matrix44f Vrpn::getSensorPos(int d)
{
   gmtl::Matrix44f ret_val;
   gmtl::setRot( ret_val, mQuats[d]);
   gmtl::setTrans( ret_val, mPositions[d]);
   return ret_val;
}

gadget::DigitalData Vrpn::getDigitalData(int d)
{
   return mButtons[d];
}

} // End of gadget namespace
