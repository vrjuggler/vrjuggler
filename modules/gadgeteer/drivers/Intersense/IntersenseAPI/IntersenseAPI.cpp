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

#include <gadget/Devices/DriverConfig.h>

#include <boost/bind.hpp>

#include <vpr/vprConfig.h>
#include <vpr/System.h>

#include <gmtl/Matrix.h>
#include <gmtl/Vec.h>
#include <gmtl/MatrixOps.h>
#include <gmtl/Generate.h>
#include <gmtl/EulerAngle.h>
#include <gmtl/Quat.h>

#include <jccl/Config/ConfigElement.h>
#include <gadget/Type/DeviceConstructor.h>
#include <gadget/Util/Debug.h>
#include <gadget/gadgetParam.h>

#include <drivers/Intersense/IntersenseAPI/IntersenseAPI.h>


extern "C"
{

GADGET_DRIVER_EXPORT(vpr::Uint32) getGadgeteerVersion()
{
   return __GADGET_version;
}

GADGET_DRIVER_EXPORT(void) initDevice(gadget::InputManager* inputMgr)
{
   new gadget::DeviceConstructor<gadget::IntersenseAPI>(inputMgr);
}

}

namespace gadget
{

IntersenseAPI::IntersenseAPI()
   : mDone(false)
{
   vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_STATE_LVL)
      << "*** IntersenseAPI::IntersenseAPI() ***\n" << vprDEBUG_FLUSH;
}

std::string IntersenseAPI::getElementType()
{
   return "intersense_api";
}

bool IntersenseAPI::config(jccl::ConfigElementPtr e)
{
   vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_STATE_LVL)
      << "*** IntersenseAPI::config() ***" << std::endl << vprDEBUG_FLUSH;

   // Configure the subclasses
   if (!(Input::config(e) && Position::config(e) &&
         Digital::config(e) && Analog::config(e)))
   {
      return false;
   }

   // Set various attributes of the Intersense driver
   mTracker.setPort( e->getProperty<std::string>("port") );
   mTracker.setVerbose(e->getProperty<bool>("verbose"));

   // Get the location of the Intersense DSO
   mISenseDriverLocation = e->getProperty<std::string>("driver");

   // Create a new array of StationConfigs
   mStations.resize(e->getNum("stations"));

   // Configure each station
   for( unsigned int i = 0; i < mStations.size(); ++i )
   {
      jccl::ConfigElementPtr station_config =
         e->getProperty<jccl::ConfigElementPtr>("stations", i);

      mStations[i].enabled = station_config->getProperty<bool>("enabled");
      mStations[i].stationIndex =
         station_config->getProperty<int>("station_index");
      mStations[i].useDigital =
         station_config->getProperty<bool>("use_digital");
      mStations[i].useAnalog = station_config->getProperty<bool>("use_analog");

      mStations[i].dig_num = station_config->getProperty<int>("digital_count");
      mStations[i].ana_num = station_config->getProperty<int>("analog_count");
   }

   return true;
}

IntersenseAPI::~IntersenseAPI()
{
   this->stopSampling();
}

// Main thread of control for this active object
void IntersenseAPI::controlLoop()
{
   // Loop through and keep sampling until stopSampleing is called.
   while(!mDone)
   {
      this->sample();
      //TODO: Find a way to eliminate this sleep. If I rememeber correctly it
      //      was added because the CPU was getting pegged too fast with samples
      //      since there was no I/O wait because we are simply querying the
      //      ISense library.
      vpr::System::msleep(50);
   }
}

bool IntersenseAPI::startSampling()
{
   // Configure the stations used by the configuration
   for( unsigned int i = 0; i < mStations.size(); ++i )
   {
      int station_index = mStations[i].stationIndex;

      // Load the config state from the physical tracker
      mTracker.loadConfigState(station_index);
      mTracker.setState(station_index, mStations[i].enabled);
      mTracker.setAngleFormat(station_index, ISD_EULER);
      mTracker.setInputs(station_index,
                         mStations[i].useDigital || mStations[i].useAnalog);
      // Save the config state to the physical tracker.
      mTracker.saveConfigState(station_index);
   }

   // Ensure that we have not already started sampling.
   if (this->isActive() == true)
   {
      vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_WARNING_LVL)
         << "IntersenseAPI driver was already started." << std::endl
         << vprDEBUG_FLUSH;
      return false;
   }

   // Ensure that the thread has not already started.
   if(mThread != NULL)
   {
      vprDEBUG(vprDBG_ERROR,vprDBG_CRITICAL_LVL)
         << clrOutNORM(clrRED,"ERROR:")
         << " gadget::IntersenseAPI::startSampling() called, when already "
         << "sampling.\n" << vprDEBUG_FLUSH;
      return false;
   }

   // Open the connection to the tracker
   mTracker.open(mISenseDriverLocation);
   if (this->isActive() == false)
   {
      vprDEBUG(vprDBG_ERROR,vprDBG_CRITICAL_LVL)
         << clrOutNORM(clrRED,"ERROR:")
         << " [gadget::IntersenseAPI::startSampling()] mTracker.open() "
         << "failed to connect to tracker.\n" << vprDEBUG_FLUSH;
      return false;
   }

   // Set flag that will later allow us to stop the control loop.
   mDone = false;

   // Create a new thread to handle the control
   mThread = new vpr::Thread(boost::bind(&IntersenseAPI::controlLoop, this));

   if ( ! mThread->valid() )
   {
      return false;   // Failed
   }
   else
   {
      return true;   // Succeeded
   }
}

bool IntersenseAPI::sample()
{
   // If we are not active, then don't try to sample.
   if (!isActive())
   {
      return false;
   }

   // Create the data buffers to put the new data into.
   std::vector<gadget::PositionData> cur_pos_samples(mStations.size());
   std::vector<gadget::DigitalData>  cur_digital_samples;
   std::vector<gadget::AnalogData>   cur_analog_samples;


   // get an initial timestamp for this entire sample. we'll copy it into
   // each PositionData for this sample.
   if ( ! cur_pos_samples.empty() )
   {
      cur_pos_samples[0].setTime();
   }

   mTracker.updateData();

   vpr::Thread::yield();

   for ( unsigned int i = 0 ; i < mStations.size() ; ++i )
   {
      // Get the station index for the given station.
      int stationIndex = mStations[i].stationIndex;

      // Set the time of each PositionData to match the first.
      cur_pos_samples[i].setTime( cur_pos_samples[0].getTime() );

      // If the Intersense is returning data in Euler format. Otherwise we
      // assume that it is returning data in quaternion format.
      if ( mTracker.getAngleFormat(stationIndex) == ISD_EULER )
      {
         gmtl::identity(cur_pos_samples[i].mPosData);
         gmtl::EulerAngleZYXf euler( gmtl::Math::deg2Rad( mTracker.zRot( stationIndex ) ),
                                     gmtl::Math::deg2Rad( mTracker.yRot( stationIndex ) ),
                                     gmtl::Math::deg2Rad( mTracker.xRot( stationIndex ) ) );
         gmtl::setRot( cur_pos_samples[i].mPosData, euler );
         gmtl::setTrans( cur_pos_samples[i].mPosData,
                         gmtl::Vec3f(mTracker.xPos( stationIndex ),
                                     mTracker.yPos( stationIndex ),
                                     mTracker.zPos( stationIndex )) );
      }
      else
      {
         gmtl::Quatf quatValue(mTracker.xQuat( stationIndex ),
                               mTracker.yQuat( stationIndex ),
                               mTracker.zQuat( stationIndex ),
                               mTracker.wQuat( stationIndex ));
         gmtl::set( cur_pos_samples[i].mPosData, quatValue );
      }

      // We start at the index of the first digital item (set in the config
      // files) and we copy the digital data from this station to the
      // InterSense device for range (min -> min+count-1).

      if (mStations[i].useDigital)
      {
         for ( int j = 0; j < mStations[i].dig_num; ++j )
         {
            DigitalData new_digital(mTracker.buttonState(stationIndex, j));
            new_digital.setTime();
            cur_digital_samples.push_back(new_digital);
         }
      }

      // Analog works the same as the digital
      if (mStations[i].useAnalog)
      {
         float f;
         for ( int j = 0; j < mStations[i].ana_num; ++j )
         {
            Analog::normalizeMinToMax(mTracker.analogData(stationIndex, j), f);
            AnalogData new_analog(f);
            new_analog.setTime();
            cur_analog_samples.push_back(new_analog);
         }
      }
   }

   // Lock and then swap the buffers.
   addAnalogSample(cur_analog_samples);
   addDigitalSample(cur_digital_samples);
   addPositionSample(cur_pos_samples);

   return true;
}

bool IntersenseAPI::stopSampling()
{
   // Make sure that we are sampling in the first place.
   if (this->isActive() == false)
   {
      return false;
   }

   if (mThread != NULL)
   {
      vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_CONFIG_LVL)
         << "[gadget::IntersenseAPI::stopSampling()] Stopping the InterSense "
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
            << " [gadget::IntersenseAPI::stopSampling()] InterSense tracker "
            << "failed to stop.\n" << vprDEBUG_FLUSH;
         return false;
      }

      vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_CONFIG_LVL)
         << "*** IntersenseAPI has been shutdown. ***" << std::endl
         << vprDEBUG_FLUSH;
   }

   return true;
}

void IntersenseAPI::updateData()
{
   if (isActive())
   {
      swapDigitalBuffers();
      swapAnalogBuffers();
      swapPositionBuffers();
   }
}

} // End of gadget namespace
