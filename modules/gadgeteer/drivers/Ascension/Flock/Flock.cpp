/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2003 by Iowa State University
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

#include <boost/concept_check.hpp>
#include <gmtl/Matrix.h>
#include <gmtl/MatrixOps.h>
#include <gmtl/Generate.h>
#include <gmtl/EulerAngle.h>

#include <vpr/vpr.h>
#include <vpr/System.h>
#include <vpr/Util/FileUtils.h>

#include <jccl/Config/ConfigElement.h>
#include <gadget/Util/Debug.h>
#include <gadget/Type/DeviceConstructor.h>
#include <drivers/Ascension/Flock/Flock.h>


extern "C"
{

GADGET_DRIVER_EXPORT(void) initDevice(gadget::InputManager* inputMgr)
{
   new gadget::DeviceConstructor<gadget::Flock>(inputMgr);
}

}

namespace gadget
{

/**
 * Configure constructor.
 *
 * @param port  such as "/dev/ttyd3"
 * @param baud  such as 38400, 19200, 9600, 14400, etc...
 * @param sync  sync type
 * @param block  blocking
 * @param numBrds  number of birds in flock (without transmitter)
 * @param transmit  transmitter unit number
 * @param hemi  hemisphere to track from
 * @param filt  filtering type
 * @param report  flock report rate
 * @param calfile  a calibration file, if "", then use none
 */
Flock::Flock(const char* const port, const int& baud,
             const int& numBrds, const int& transmit)
   : mThread(NULL),
     mFlockOfBirds(port, baud)

{
   ;
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

   std::string port_name = e->getProperty<std::string>("port");
   int baud_rate = e->getProperty<int>("baud");

   // keep FlockStandalone's port and baud members in sync with Input's port
   // and baud members.
   mFlockOfBirds.setPort( port_name );
   mFlockOfBirds.setBaudRate( baud_rate );

   // If these are non-negative, then override the auto-configuration abilities of the flock driver
   // XXX: These values are auto configured now.
   
   //int num_birds =  e->getProperty<int>("number_of_birds");
   //if(num_birds > 0)
   //{  mFlockOfBirds.setNumSensors(num_birds); }
   //int trans_addr =  e->getProperty<int>("transmitter_id");
   //if (trans_addr > 0)
   //{  mFlockOfBirds.setTransmitter(trans_addr); }
   //mFlockOfBirds.setSync( e->getProperty<int>("sync_style") );
   
   // Set mFlockOfBirds with the config info.
   mFlockOfBirds.setHemisphere( (BIRD_HEMI) e->getProperty<int>("hemisphere") ); //LOWER_HEMI
   mFlockOfBirds.setFilterType( (BIRD_FILT) e->getProperty<int>("filter") ); //
   mFlockOfBirds.setOutputFormat(::Flock::Output::PositionQuaternion);          // Default to pos quaternion

   return true;
}

Flock::~Flock()
{
   this->stopSampling();
}

void Flock::controlLoop(void* nullParam)
{
   boost::ignore_unused_variable_warning(nullParam);

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
      vpr::ThreadMemberFunctor<Flock>* memberFunctor =
          new vpr::ThreadMemberFunctor<Flock>(this, &Flock::controlLoop, NULL);
      mThread = new vpr::Thread(memberFunctor);

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
   std::vector< gadget::PositionData > cur_samples(mFlockOfBirds.getNumSensors());

   if ( !isActive() )
   {  return false; }

   mFlockOfBirds.sample();

   // get an initial timestamp for this entire sample. we'll copy it into
   // each PositionData for this sample.
   if (!cur_samples.empty())
   {  cur_samples[0].setTime(); }

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
