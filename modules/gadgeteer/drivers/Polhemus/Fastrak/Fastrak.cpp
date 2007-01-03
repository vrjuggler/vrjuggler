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

// ----------------------------------------------------------------------------
// Author:
//     Nicolas Tarrin <nicolas.tarrin@inria.fr>
//     February 2001
// ----------------------------------------------------------------------------

#include <gadget/Devices/DriverConfig.h>

#include <vector>
#include <boost/bind.hpp>

#include <gmtl/EulerAngle.h>
#include <gmtl/Vec.h>
#include <gmtl/Generate.h>

#include <vpr/vpr.h>
#include <vpr/System.h>

#include <gadget/Type/DeviceConstructor.h>
#include <gadget/gadgetParam.h>
#include <gadget/Util/Debug.h>

#include <drivers/Polhemus/Fastrak/Fastrak.h>

extern "C"
{

GADGET_DRIVER_EXPORT(vpr::Uint32) getGadgeteerVersion()
{
   return __GADGET_version;
}

GADGET_DRIVER_EXPORT(void) initDevice(gadget::InputManager* inputMgr)
{
   new gadget::DeviceConstructor<gadget::Fastrak>(inputMgr);
}

}

namespace gadget
{

Fastrak::Fastrak(const char* port, const int baud)
   : mSampleThread(NULL)
   , mFastrak(port, baud)
{
   ;
}

Fastrak::~Fastrak()
{
   this->stopSampling();
}

std::string Fastrak::getElementType()
{
   return "fastrak";
}

bool Fastrak::config(jccl::ConfigElementPtr elm)
{
   vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_STATE_LVL)
      << "Fastrak::Fastrak(jccl::ConfigElementPtr)\n" << vprDEBUG_FLUSH;

   // read in Position's config stuff,
   // --> this will be the port and baud fields
   if(!Input::config(elm) || !Position::config(elm))
   {
      return false;
   }

   // Keep this up to date with the version of the element definition we're
   // expecting to handle.
   const unsigned int cur_version(2);
   bool status(true);

   // If the element version is less than cur_version, we will not try to
   // proceed.  Instead, we'll print an error message and return false so
   // that the Config Manager knows this element wasn't consumed.
   if ( elm->getVersion() < cur_version )
   {
      vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_CRITICAL_LVL)
         << clrOutBOLD(clrRED, "ERROR") << " [Fastrak] Element named '"
         << elm->getName() << "'" << std::endl << vprDEBUG_FLUSH;
      vprDEBUG_NEXT(gadgetDBG_INPUT_MGR, vprDBG_CRITICAL_LVL)
         << "is version " << elm->getVersion()
         << ", but we require at least version " << cur_version << std::endl
         << vprDEBUG_FLUSH;
      vprDEBUG_NEXT(gadgetDBG_INPUT_MGR, vprDBG_CRITICAL_LVL)
         << "Ignoring this element and moving on." << std::endl
         << vprDEBUG_FLUSH;
      status = false;
   }
   else
   {
      // Configure FastrakStandalone.
      mFastrak.setPort( elm->getProperty<std::string>("port") );
      mFastrak.setBaudRate( elm->getProperty<int>("baud") );
      mFastrak.setUnits(::Fastrak::CENTIMETERS);
      mFastrak.init();

      for (unsigned int i = 1; i < 5; i++)
      {
         mFastrak.setHemisphere(i, (::Fastrak::HEMISPHERE) elm->getProperty<int>("hemisphere") );
         mFastrak.setStylusButtonEnabled(1, false);
      }
   }

   return true;
}

bool Fastrak::startSampling()
{
   vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_CRITICAL_LVL)
      << clrOutBOLD(clrRED, "FASTRAK")
      << "::startSampling()!\n"
      << vprDEBUG_FLUSH;
   mFastrak.open();
   
   mExitFlag = false;
   try
   {
      mSampleThread = new vpr::Thread(boost::bind(&Fastrak::controlLoop,
                                                  this));
      return true;
   }
   catch (vpr::Exception& ex)
   {
      vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_CRITICAL_LVL)
         << clrOutBOLD(clrRED, "ERROR")
         << ": Failed to spawn thread for Fastrak driver!\n"
         << vprDEBUG_FLUSH;
      vprDEBUG_NEXT(gadgetDBG_INPUT_MGR, vprDBG_CRITICAL_LVL)
         << ex.what() << std::endl << vprDEBUG_FLUSH;
   }

   return false;
}

// Record (or sample) the current data
// this is called repeatedly by the sample thread created by startSampling()
bool Fastrak::sample()
{
   bool status(true);

   // NOTE: Currently we are returning data for all stations, including
   //       disabled stations.
   std::vector<gadget::PositionData> cur_pos_samples(4);

   // get an initial timestamp for this entire sample. we'll copy it into
   // each PositionData for this sample.
   cur_pos_samples[0].setTime();
   try
   {
      mFastrak.readData();

      for (unsigned int i = 0; i < 4; ++i)
      {
         // Transforms between the cord frames
         gmtl::Matrix44f transmitter_T_reciever = mFastrak.getStationPosition(i+1);
         cur_pos_samples[i].setTime(cur_pos_samples[0].getTime());
         cur_pos_samples[i].mPosData = transmitter_T_reciever;
      }

      addPositionSample(cur_pos_samples);
   }
   catch (vpr::Exception& ex)
   {
      vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_CRITICAL_LVL)
         << clrOutBOLD(clrRED, "ERROR")
         << ": Failed reading data from Fastrak driver!\n"
         << vprDEBUG_FLUSH;
      vprDEBUG_NEXT(gadgetDBG_INPUT_MGR, vprDBG_CRITICAL_LVL)
         << ex.what() << std::endl << vprDEBUG_FLUSH;
      return false;
   }

   return true;
}

void Fastrak::updateData()
{
   // swap the buffered sample data
   swapPositionBuffers();
}

// kill sample thread
bool Fastrak::stopSampling()
{
   if ( mSampleThread != NULL )
   {
      mExitFlag = true;
      mSampleThread->join();
      delete mSampleThread;
      mSampleThread = NULL;
   }
   return true;
}

void Fastrak::controlLoop()
{
   // Exit when flag is set...
   while ( !mExitFlag )
   {
      sample();
      vpr::System::msleep(10);
   }
}

} // End of gadget namespace
