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
 * -----------------------------------------------------------------
 * File:          $RCSfile$
 * Date modified: $Date$
 * Version:       $Revision$
 * -----------------------------------------------------------------
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

#include <gadget/Devices/DriverConfig.h>

#include <vector>
#include <boost/bind.hpp>

#include <gmtl/Matrix.h>
#include <gmtl/MatrixOps.h>
#include <gmtl/Generate.h>
#include <gmtl/EulerAngle.h>

#include <vpr/vpr.h>
#include <vpr/System.h>
#include <vpr/Util/FileUtils.h>

#include <jccl/Config/ConfigElement.h>
#include <gadget/Util/Debug.h>
#include <gadget/gadgetParam.h>
#include <gadget/Type/DeviceConstructor.h>
#include <drivers/Ascension/Flock/Flock.h>


extern "C"
{

GADGET_DRIVER_EXPORT(vpr::Uint32) getGadgeteerVersion()
{
   return __GADGET_version;
}

GADGET_DRIVER_EXPORT(void) initDevice(gadget::InputManager* inputMgr)
{
   new gadget::DeviceConstructor<gadget::Flock>(inputMgr);
}

}

namespace gadget
{

/**
 * Configure constructor.
 */
Flock::Flock(const char* port, const int baud, const int numBrds,
             const int transmit)
   : mThread(NULL)
   , mFlockOfBirds(port, baud)
{
   boost::ignore_unused_variable_warning(numBrds);
   boost::ignore_unused_variable_warning(transmit);
}

std::string Flock::getElementType()
{
   return "flock";
}

bool Flock::config(jccl::ConfigElementPtr e)
{
   vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_STATE_LVL)
      << "Flock::Flock(jccl::ConfigElementPtr)\n" << vprDEBUG_FLUSH;

   // read in Position's config stuff,
   // --> this will be the port and baud fields
   if(! (Input::config(e) && Position::config(e)))
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
   if ( e->getVersion() < cur_version )
   {
      vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_CRITICAL_LVL)
         << clrOutBOLD(clrRED, "ERROR") << " [Flock] Element named '"
         << e->getName() << "'" << std::endl << vprDEBUG_FLUSH;
      vprDEBUG_NEXT(gadgetDBG_INPUT_MGR, vprDBG_CRITICAL_LVL)
         << "is version " << e->getVersion()
         << ", but we require at least version " << cur_version << std::endl
         << vprDEBUG_FLUSH;
      vprDEBUG_NEXT(gadgetDBG_INPUT_MGR, vprDBG_CRITICAL_LVL)
         << "Ignoring this element and moving on." << std::endl
         << vprDEBUG_FLUSH;
      status = false;
   }
   else
   {
      // keep FlockStandalone's port and baud members in sync with Input's port
      // and baud members.
      mFlockOfBirds.setPort( e->getProperty<std::string>("port") );
      mFlockOfBirds.setBaudRate( e->getProperty<int>("baud") );

      // Set mFlockOfBirds with the config info.
      mFlockOfBirds.setHemisphere( (BIRD_HEMI) e->getProperty<int>("hemisphere") );
      mFlockOfBirds.setMasterAddress( e->getProperty<unsigned>("master_address") );
      mFlockOfBirds.setAddressingMode( (::Flock::AddressingMode) e->getProperty<int>("addressing_mode") );
      mFlockOfBirds.setFilterType( (BIRD_FILT) e->getProperty<int>("filter") ); //
      mFlockOfBirds.setOutputFormat(::Flock::Output::PositionQuaternion);          // Default to pos quaternion
   }

   return status;
}

Flock::~Flock() throw ()
{
   this->stopSampling();
}

void Flock::controlLoop()
{
   // vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_STATE_LVL)
   //    << "gadget::Flock: Spawned SampleBirds starting"
   //    << std::endl << vprDEBUG_FLUSH;

   // Loop will flag is set
   while  ( !mExitFlag )
   {
      this->sample();
   }
}

bool Flock::startSampling()
{
      // make sure birds aren't already started
   if (this->isActive() == true)
   {
      vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_WARNING_LVL)
         << "gadget::Flock was already started." << std::endl << vprDEBUG_FLUSH;
      return false;
   }

   if (mThread == NULL)
   {
      vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_CONFIG_LVL)
         << "    Opening flock....\n" << vprDEBUG_FLUSH;
      mFlockOfBirds.open();
      mFlockOfBirds.configure();
      mFlockOfBirds.startStreaming();

      //sanity check.. make sure birds actually started
      if (mFlockOfBirds.getStatus() == FlockStandalone::CLOSED)
      {
         vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_CRITICAL_LVL)
            << "gadget::Flock failed to start.." << std::endl << vprDEBUG_FLUSH;
         return false;
      }

      vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_CONFIG_LVL)
         << "gadget::Flock ready to go..\n" << vprDEBUG_FLUSH;

      // Create a new thread to handle the control
      mExitFlag = false;
      mThread = new vpr::Thread(boost::bind(&Flock::controlLoop, this));

      if ( ! mThread->valid() )
      {
         return false;  // Fail
      }
      else
      {
         return true;   // success
      }
   }
   else
   {
      return false; // already sampling
   }
}

bool Flock::sample()
{
   std::vector<gadget::PositionData> cur_samples(mFlockOfBirds.getNumSensors());

   if ( !isActive() )
   {
      return false;
   }

   mFlockOfBirds.sample();

   // get an initial timestamp for this entire sample. we'll copy it into
   // each PositionData for this sample.
   if (!cur_samples.empty())
   {
      cur_samples[0].setTime();
   }

   vpr::Thread::yield();

   // For each bird
   for (unsigned i=0; i < mFlockOfBirds.getNumSensors(); ++i)
   {
      // Transforms between the cord frames
      gmtl::Matrix44f transmitter_T_reciever = mFlockOfBirds.getSensorPosition(i);

      // Set timestamp & Store the corrected xform back into buffer.
      cur_samples[i].mPosData = transmitter_T_reciever;
      cur_samples[i].setTime (cur_samples[0].getTime());
   }

   // Add data sample
   addPositionSample(cur_samples);

   return true;
}

bool Flock::stopSampling()
{
   if (this->isActive() == false)
   {
      return false;
   }

   if (mThread != NULL)
   {
      vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_CONFIG_LVL)
         << "Stopping the flock thread..." << vprDEBUG_FLUSH;

      mExitFlag = true;
      mThread->join();
      delete mThread;
      mThread = NULL;

      vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_CONFIG_LVL)
         << "  Stopping the flock..." << vprDEBUG_FLUSH;

      mFlockOfBirds.close();

      // sanity check: did the flock actually stop?
      if (this->isActive() == true)
      {
         vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_CRITICAL_LVL)
            << "Flock didn't stop." << std::endl << vprDEBUG_FLUSH;
         return 0;
      }

      vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_CONFIG_LVL)
         << "stopped." << std::endl << vprDEBUG_FLUSH;
   }

   return true;
}

void Flock::updateData()
{
   if ( isActive() )
   {
      swapPositionBuffers();
   }
}

} // End of gadget namespace
