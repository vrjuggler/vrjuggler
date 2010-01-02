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

#include <gadget/Devices/DriverConfig.h>

#include <boost/bind.hpp>

#include <vpr/vprConfig.h>
#include <vpr/System.h>

#include <gmtl/Matrix.h>
#include <gmtl/Vec.h>
#include <gmtl/MatrixOps.h>
#include <gmtl/Generate.h>
#include <gmtl/Quat.h>

#include <jccl/Config/ConfigElement.h>
#include <gadget/Type/DeviceConstructor.h>
#include <gadget/Util/Debug.h>
#include <gadget/gadgetParam.h>

#include <drivers/NaturalPoint/OptiTrack/OptiTrack.h>

extern "C"
{

GADGET_DRIVER_EXPORT(vpr::Uint32) getGadgeteerVersion()
{
   return __GADGET_version;
}

GADGET_DRIVER_EXPORT(void) initDevice(gadget::InputManager* inputMgr)
{
   new gadget::DeviceConstructor<gadget::OptiTrack>(inputMgr);
}

}

namespace gadget
{

/**
* Constructor.
*/
OptiTrack::OptiTrack()
 : mDone(false)
{;}

/**
* Destructor.
*/
OptiTrack::~OptiTrack()
{
   this->stopSampling();
}

std::string OptiTrack::getElementType()
{
   return "optitrack";
}

bool OptiTrack::config(jccl::ConfigElementPtr e)
{
   unsigned int i;

   if (!(Input::config(e) && Position::config(e)))
   {
      return false;
   }

   // Get configuration attribute(s).
   mServer = e->getProperty<std::string>("server");

   // Fill vector with IDs of rigid bodies.
   mRigidBodyIDs.resize(e->getNum("rigid_body"));

   for ( i = 0; i < mRigidBodyIDs.size(); ++i )
   {
      jccl::ConfigElementPtr rb_config =
         e->getProperty<jccl::ConfigElementPtr>("rigid_body", i);

      mRigidBodyIDs[i] = rb_config->getProperty<int>("body_index");
   }

   // Fill vector with IDs of individual markers.
   mMarkerIDs.resize(e->getNum("marker"));

   for (i = 0; i < mMarkerIDs.size(); ++i)
   {
      jccl::ConfigElementPtr marker_config =
         e->getProperty<jccl::ConfigElementPtr>("marker", i);

      mMarkerIDs[i] = marker_config->getProperty<int>("marker_index");
   }

   vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_CONFIG_LVL)
      << "[gadget::OptiTrack::config()] Configured for " << mRigidBodyIDs.size()
      << " rigid bodies and " << mMarkerIDs.size() << " markers.\n"
      << vprDEBUG_FLUSH;

   return true;
}

/** Main thread of control for this active object. */
void OptiTrack::controlLoop()
{  
   // Loop through and keep sampling until stopSampling is called.
   while (!mDone)
   {
      this->sample();

      // Note: might need a sleep here if CPU is high
      //vpr::System::msleep(10);
   }
}

bool OptiTrack::startSampling()
{
   // Ensure that we have not already started sampling.
   if (this->isActive())
   {
      vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_WARNING_LVL)
         << "OptiTrack driver was already started." << std::endl
         << vprDEBUG_FLUSH;
      return false;
   }

   // Ensure that the thread has not already started.
   if (mThread != NULL)
   {
      vprDEBUG(vprDBG_ERROR,vprDBG_CRITICAL_LVL)
         << clrOutNORM(clrRED,"ERROR:")
         << " gadget::OptiTrack::startSampling() called, when already "
         << "sampling.\n" << vprDEBUG_FLUSH;
      return false;
   }

   // Open the connection to the tracker
   mTracker.open(mServer);
   if (!this->isActive())
   {
      vprDEBUG(vprDBG_ERROR,vprDBG_CRITICAL_LVL)
         << clrOutNORM(clrRED,"ERROR:")
         << " [gadget::OptiTrack::startSampling()] mTracker.open() "
         << "failed to connect to tracker.\n" << vprDEBUG_FLUSH;
      return false;
   }

   // Set flag that will later allow us to stop the control loop.
   mDone = false;

   bool started(true);

   // Create a new thread to handle the control
   try
   {
      mThread = new vpr::Thread(boost::bind(&OptiTrack::controlLoop, this));
   }
   catch (vpr::Exception& ex)
   {
      vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_CRITICAL_LVL)
         << clrOutBOLD(clrRED, "ERROR")
         << ": Failed to spawn thread for OptiTrack driver!\n"
         << vprDEBUG_FLUSH;
      vprDEBUG_NEXT(gadgetDBG_INPUT_MGR, vprDBG_CRITICAL_LVL)
         << ex.what() << std::endl << vprDEBUG_FLUSH;
      started = false;
   }

   return started;
}

bool OptiTrack::sample()
{
   unsigned int i;

   // If we are not active, then don't try to sample.
   if (!isActive())
   {
      return false;
   }

   // Check to see if we have new data to pull
   if (!mTracker.updateData())
   {
      vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_CRITICAL_LVL)
         << clrOutBOLD(clrRED, "[gadget::OptiTrack::sample()]")
         << ": Could not read data from OptiTrack driver!\n"
         << vprDEBUG_FLUSH;
      return false;
   }

   // Create the data buffers to put the new data into.
   std::vector<gadget::PositionData> current_samples(mRigidBodyIDs.size() + mMarkerIDs.size());

   // Get an initial timestamp for rigid bodies.
   if (!current_samples.empty())
   {
      current_samples[0].setTime();
   }

   // Fill rigid body data.
   for (i = 0 ; i < mRigidBodyIDs.size(); ++i)
   {
      // Get the station index for the given station.
      int index = mRigidBodyIDs[i];

      // Set the time of each PositionData to match the first.
      current_samples[i].setTime(current_samples[0].getTime());

      gmtl::identity(current_samples[i].mPosData);

      gmtl::Quatf quat(mTracker.xRBQuat(index),
                       mTracker.yRBQuat(index),
                       mTracker.zRBQuat(index),
                       mTracker.wRBQuat(index));
      
      current_samples[i].mPosData = gmtl::makeRot<gmtl::Matrix44f>(quat);

      gmtl::setTrans(current_samples[i].mPosData,
                     gmtl::Vec3f(mTracker.xRBPos(index),
                                 mTracker.yRBPos(index),
                                 mTracker.zRBPos(index)));
   }
    
   // Fill in marker data.  Note: this assumes that the IDs do not overlap with rigid bodies.  
   // Numbering for markers should start at 1000 at the configuration level.
   for (i = mRigidBodyIDs.size(); i < (mMarkerIDs.size() + mRigidBodyIDs.size()); ++i)
   {
      // Get the station index for the given station.
      int index = mMarkerIDs[i];

      // Set the time of each PositionData to match the first.
      current_samples[i].setTime( current_samples[0].getTime() );

      gmtl::identity(current_samples[i].mPosData);

      gmtl::setTrans( current_samples[i].mPosData,
                      gmtl::Vec3f(mTracker.xMarkerPos( index ),
                                  mTracker.yMarkerPos( index ),
                                  mTracker.zMarkerPos( index )) );
   }

   // Lock and then swap the buffers.
   addPositionSample(current_samples);
   
   return true;
}

bool OptiTrack::stopSampling()
{  
   // Make sure that we are sampling in the first place.
   if (!isActive())
   {
     mDone = true; // just to be safe 
     return false;
   }

   if (mThread != NULL)
   {
      vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_CONFIG_LVL)
         << "[gadget::OptiTrack::stopSampling()] Stopping the OptiTrack "
         << "thread...\n" << vprDEBUG_FLUSH;

      // Set the done flag to attempt to cause the control loop to exit
      // naturally.
      mDone = true;

      mThread->join();
      delete mThread;
      mThread = NULL;

      // Close the connection to the tracker
      mTracker.close();

      // If the tracker failed to stop
      if (isActive() == true)
      {
         vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_CRITICAL_LVL)
            << clrOutNORM(clrRED,"\nERROR:")
            << " [gadget::OptiTrack::stopSampling()] OptiTrack tracker "
            << "failed to stop.\n" << vprDEBUG_FLUSH;
         return false;
      }

      vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_CONFIG_LVL)
         << "*** OptiTrack has been shutdown. ***" << std::endl
         << vprDEBUG_FLUSH;
   }
   
   return true;
}

void OptiTrack::updateData()
{  
   if (isActive())
   {
      swapPositionBuffers();
   }
}

} // End of gadget namespace
