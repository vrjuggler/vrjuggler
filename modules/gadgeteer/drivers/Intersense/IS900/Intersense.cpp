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

#include <gadget/Devices/DriverConfig.h>

#include <sstream>
#include <fstream>
#include <boost/bind.hpp>

#include <vpr/vpr.h>
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

#include <drivers/Intersense/IS900/Intersense.h>


extern "C"
{

GADGET_DRIVER_EXPORT(vpr::Uint32) getGadgeteerVersion()
{
   return __GADGET_version;
}

GADGET_DRIVER_EXPORT(void) initDevice(gadget::InputManager* inputMgr)
{
   new gadget::DeviceConstructor<gadget::Intersense>(inputMgr);
}

}

namespace gadget
{

// Helper to return the index for theData array
// given the stationNum we are dealing with and the bufferIndex
// to get
//! ARGS:stationNum - The number of the cube we care about
//! ARGS:bufferIndex - the value of current, progress, or valid (it is an offset in the array)
// XXX: We are going to say the cubes are 0 based


int Intersense::getStationIndex(int stationNum, int bufferIndex)
{
   int ret_val = (stationNum*3)+bufferIndex;
   vprASSERT((ret_val >= 0) && (ret_val < ((mTracker.NumStations()+1)*3)));
   return ret_val;
}

Intersense::Intersense()
   : stations(NULL)
{
   vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_STATE_LVL)
      << "*** Intersense::Intersense() ***\n" << vprDEBUG_FLUSH;
//   vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_CONFIG_LVL)
//      << "*** Intersense::deviceAbilities = " << deviceAbilities << " ***\n"
//      << vprDEBUG_FLUSH;
}

std::string Intersense::getElementType()
{
   return "intersense";
}

bool Intersense::config(jccl::ConfigElementPtr e)
{
   vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_CONFIG_LVL)
      << "         Intersense::config(jccl::ConfigElementPtr)"
      << std::endl << vprDEBUG_FLUSH;

   // read in Position's, Digital's, and Analog's config stuff,
   // --> this will be the port and baud fields
   if ( ! (Input::config(e) && Position::config(e) && Digital::config(e) &&
           Analog::config(e)) )
   {
      return false;
   }

   mPortName = e->getProperty<std::string>("port");
   mBaudRate = e->getProperty<int>("baud");

   // keep IntersenseStandalone's port and baud members in sync with Input's
   // port and baud members.
   mTracker.setPortName( mPortName );
   mTracker.setBaudRate( mBaudRate );
   mTracker.rNumStations() = e->getNum("stations");

   if ( stations != NULL )
   {
      delete [] stations;
   }

   stations = new ISStationConfig[mTracker.rNumStations()];
   jccl::ConfigElementPtr stationConfig;
   for ( int i = 0; i < mTracker.rNumStations(); ++i )
   {
      stationConfig = e->getProperty<jccl::ConfigElementPtr>("stations", i);
      stations[i].enabled = stationConfig->getProperty<bool>("enabled");
      stations[i].stationIndex =
         stationConfig->getProperty<int>("station_index");
      stations[i].useDigital = stationConfig->getProperty<bool>("use_digital");
      stations[i].useAnalog = stationConfig->getProperty<bool>("use_analog");

      stations[i].digMin = stationConfig->getProperty<int>("digital_first");
      stations[i].digNum = stationConfig->getProperty<int>("digital_count");
      stations[i].anaMin = stationConfig->getProperty<int>("analog_first");
      stations[i].anaNum = stationConfig->getProperty<int>("analog_count");
   }

   // load an init script for the tracker and then pass it to mTracker
   std::string script_file = e->getProperty<std::string>("script");

   if ( script_file.length() > 0 )
   {
      std::stringstream script;
      std::ifstream script_stream;
      script_stream.open(script_file.c_str());
      script << script_stream.rdbuf();
      mTracker.setScript(script.str().c_str());
      script_stream.close();
   }

   mTracker.rVerbose() = e->getProperty<bool>("verbose");

   return true;
}

Intersense::~Intersense()
{
   this->stopSampling();
   if (stations != NULL)
   {
      delete [] stations;
   }
}

// Main thread of control for this active object
void Intersense::controlLoop()
{
   // Configure the stations used by the configuration.
   for ( int i = 0; i < mTracker.NumStations(); ++i )
   {
      const int j(stations[i].stationIndex);

      mTracker.getConfigState(j);
      mTracker.rState(j) = stations[i].enabled;
      mTracker.rAngleFormat(j) = ISD_EULER;
      mTracker.rButtons(j) = stations[i].useDigital;
      mTracker.rAnalogData(j) = stations[i].useAnalog;
      mTracker.setConfigState(j);
   }

   // Loop through and keep sampling
   while ( ! mExitFlag )
   {
      this->sample();
      //TODO: Find a way to eliminate this sleep. This was added because
      //      the CPU was getting pegged too fast with samples.
      vpr::System::msleep(50);
   }
}

bool Intersense::startSampling()
{
   // make sure inertia cubes aren't already started
   if (this->isActive() == true)
   {
      vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_WARNING_LVL)
         << "gadget::Intersense was already started." << std::endl
         << vprDEBUG_FLUSH;
      return false;
   }

   // Has the thread actually started already
   if ( mThread != NULL )
   {
      vprDEBUG(vprDBG_ERROR,vprDBG_CRITICAL_LVL)
         << clrOutNORM(clrRED,"ERROR:")
         << " gadget::Intersense: startSampling called, when already sampling."
         << std::endl << vprDEBUG_FLUSH;
      vprASSERT(false);
   }
   else
   {
      // open the tracker connection
      mTracker.open();

      if ( this->isActive() == false )
      {
         vprDEBUG(vprDBG_ERROR,vprDBG_CRITICAL_LVL)
            << clrOutNORM(clrRED,"ERROR:")
            << " gadget::Intersense: mTracker.open() failed to connect to "
            << "tracker.\n" << vprDEBUG_FLUSH;
         return false;
      }

      // Create a new thread to handle the control and set flag to loop
      mExitFlag = false;

      try
      {
         mThread = new vpr::Thread(boost::bind(&Intersense::controlLoop,
                                               this));
         return true;
      }
      catch (vpr::Exception& ex)
      {
         vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_CRITICAL_LVL)
            << clrOutBOLD(clrRED, "ERROR")
            << ": Failed to spawn thread for IS-900 driver!\n"
            << vprDEBUG_FLUSH;
         vprDEBUG_NEXT(gadgetDBG_INPUT_MGR, vprDBG_CRITICAL_LVL)
            << ex.what() << std::endl << vprDEBUG_FLUSH;

         return false;
      }
   }

   return false;
}

bool Intersense::sample()
{
   if ( this->isActive() == false )
   {
       return false;
   }

   IsenseData cur_sample;
   std::vector<gadget::PositionData> cur_pos_samples(mTracker.NumStations());

   mTracker.updateData();

   // get an initial timestamp for this entire sample. we'll copy it into
   // each PositionData for this sample.
   //int firstindex;
   if ( ! cur_pos_samples.empty() )
   {
      //firstindex = getStationIndex (0, progress);
      cur_pos_samples[0].setTime();
   }

   vpr::Thread::yield();

   for ( int i = 0 ; i < (mTracker.NumStations()); ++i )
   {
      //int index = getStationIndex(i,progress);

      const int station_index(stations[i].stationIndex);
      gmtl::Matrix44f& pos_data(cur_pos_samples[i].editValue());

      if ( mTracker.rAngleFormat(station_index) == ISD_EULER )
      {
         gmtl::identity(pos_data);
         gmtl::EulerAngleZYXf euler(
            gmtl::Math::deg2Rad(mTracker.zRot(station_index)),
            gmtl::Math::deg2Rad(mTracker.yRot(station_index)),
            gmtl::Math::deg2Rad(mTracker.xRot(station_index))
         );
         gmtl::setRot(pos_data, euler);
         gmtl::setTrans(pos_data,
                        gmtl::Vec3f(mTracker.xPos(station_index),
                                    mTracker.yPos(station_index),
                                    mTracker.zPos(station_index)));
      }
      else
      {
         gmtl::Quatf quatValue(mTracker.xQuat(station_index),
                               mTracker.yQuat(station_index),
                               mTracker.zQuat(station_index),
                               mTracker.wQuat(station_index));
         gmtl::set(pos_data, quatValue);
      }

      cur_pos_samples[i].setTime(cur_pos_samples[0].getTime());

      // We start at the index of the first digital item (set in the config
      // files) and we copy the digital data from this station to the
      // Intersense device for range (min -> min+count-1)
      int min = stations[i].digMin;
      int num = min + stations[i].digNum;
      if ( stations[i].useDigital )
      {
         for ( int j = 0, k = min;
               j < MAX_NUM_BUTTONS && k < IS_BUTTON_NUM && k < num;
               ++j, ++k )
         {
            //mInput[progress].digital[k] = mTracker.buttonState(station_index, j);
			 cur_sample.digital[k] =
                static_cast<DigitalState::State>(mTracker.buttonState(station_index, j));
         }
      }

      // Analog works the same as the digital
      min = stations[i].anaMin;
      num = min + stations[i].anaNum;

      if ( stations[i].useAnalog )
      {
         for ( int j = 0, k = min;
               j < MAX_ANALOG_CHANNELS && k < IS_ANALOG_NUM && k < num;
               ++j, ++k )
         {
            //mInput[progress].analog[k] = mTracker.analogData(station_index, j);
            cur_sample.analog[k] = mTracker.analogData(station_index, j);
         }
      }

    }

    // Locks and then swaps the indices
    addAnalogSample(cur_sample.analog);
    addDigitalSample(cur_sample.digital);
    addPositionSample(cur_pos_samples);

    return true;
}

bool Intersense::stopSampling()
{
   if ( this->isActive() == false )
   {
      return false;
   }

   if ( mThread != NULL )
   {
      // Signal thread to stop looping
      mExitFlag = true;
      vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_CONFIG_LVL)
         << "gadget::Intersense::stopSampling(): Stopping the intersense thread... "
         << vprDEBUG_FLUSH;

      mThread->join();
      delete mThread;
      mThread = NULL;

      mTracker.close();

      if ( this->isActive() == true )
      {
         vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_CRITICAL_LVL)
            << clrOutNORM(clrRED,"\nERROR:")
            << " gadget::Intersense::stopSampling(): Intersense tracker failed to stop.\n"
            << vprDEBUG_FLUSH;
         return false;
      }

      vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_CONFIG_LVL)
         << "stopped." << std::endl << vprDEBUG_FLUSH;
   }

   return true;
}


void Intersense::updateData()
{
   if ( ! isActive() )
   {
      return;
   }

   swapDigitalBuffers();
   swapAnalogBuffers();
   swapPositionBuffers();
}

} // End of gadget namespace
