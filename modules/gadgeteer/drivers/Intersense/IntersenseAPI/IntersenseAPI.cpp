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

#include <sstream>
#include <fstream>
#include <boost/concept_check.hpp>

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

#include <gadget/Devices/Intersense/IntersenseAPI/IntersenseAPI.h>

void initDevice(gadget::InputManager* inputMgr)
{
   new gadget::DeviceConstructor<gadget::IntersenseAPI>(inputMgr);
}

namespace gadget
{

// Helper to return the index for theData array
// given the stationNum we are dealing with and the bufferIndex
// to get
//! ARGS:stationNum - The number of the cube we care about
//! ARGS:bufferIndex - the value of current, progress, or valid (it is an offset in the array)
// XXX: We are going to say the cubes are 0 based

int IntersenseAPI::getStationIndex(int stationNum, int bufferIndex)
{
   int ret_val = (stationNum*3)+bufferIndex;
   vprASSERT((ret_val >= 0) && (ret_val < ((mTracker.getNumStations()+1)*3)));
   return ret_val;
}



IntersenseAPI::IntersenseAPI() : stations(NULL),
                                 mDone(false)
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
   << "*** Intersense::config() ***"
   << std::endl << vprDEBUG_FLUSH;

   // Configure the subclasses
   if( ! (Input::config(e) && Position::config(e)
            && Digital::config(e) && Analog::config(e)) )
   {
      return false;
   }

   // Set various attributes of the Intersense driver
   mTracker.setPort( e->getProperty<std::string>("port") );
   mTracker.setBaudRate( e->getProperty<int>("baud") );
   mTracker.setNumStations(e->getNum("stations"));
   mTracker.setVerbose(e->getProperty<bool>("verbose"));

   // Get the location of the Intersense DSO
   mISenseDriverLocation = e->getProperty<std::string>("driver");

   // Clear all data in the stations array
   if(stations != NULL)
   {
      delete [] stations;
   }

   // Create a new array of StationConfigs
   stations = new ISStationConfig[mTracker.getNumStations()];

   vprASSERT(mTracker.getNumStations() == e->getNum("stations")
   && "ERROR: IntersenseAPI is configured incorrectly. "
   && "The number of stations specified does not match the number of embedded elements.");

   // Still print out an error, even if the above ASSERT gets compiled out for
   // an optimied build.
   if(mTracker.getNumStations() != e->getNum("stations"))
   {
      vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_CRITICAL_LVL)
      << "ERROR: IntersenseAPI is configured incorrectly. "
      << "The number of stations specified does not match the number of embedded elements."
      << std::endl << vprDEBUG_FLUSH;
   }

   // Configure each station
   for( int i = 0; i < mTracker.getNumStations(); i++)
   {
      jccl::ConfigElementPtr station_config = e->getProperty<jccl::ConfigElementPtr>("stations", i);

      stations[i].enabled = station_config->getProperty<bool>("enabled");
      stations[i].stationIndex = station_config->getProperty<int>("station_index");
      stations[i].useDigital = station_config->getProperty<bool>("use_digital");
      stations[i].useAnalog = station_config->getProperty<bool>("use_analog");

      stations[i].dig_min = station_config->getProperty<int>("digital_first");
      stations[i].dig_num = station_config->getProperty<int>("digital_count");
      stations[i].ana_min = station_config->getProperty<int>("analog_first");
      stations[i].ana_num = station_config->getProperty<int>("analog_count");
   }

   //XXX: We currentl do not support sending a script file to the tracker since
   //     it is so easy to screw up the whole tracker configuration/constillation
   //     this way.
   //const char* filename = e->getProperty<std::string>("script").c_str();
   //std::stringstream script;
   //std::ifstream scriptFile;
   //scriptFile.open(filename);
   //script<<scriptFile.rdbuf();
   //mTracker.setScript(script.str().c_str());
   //scriptFile.close();

   return true;
}

IntersenseAPI::~IntersenseAPI()
{
   this->stopSampling();
   if (stations != NULL)
   {
      delete [] stations;
   }
}

// Main thread of control for this active object
void IntersenseAPI::controlLoop(void* nullParam)
{
   boost::ignore_unused_variable_warning(nullParam);

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
   for( int i = 0; i < mTracker.getNumStations(); i++ )
   {
      int station_index = stations[i].stationIndex;

      // Load the config state from the physical tracker
      mTracker.loadConfigState(station_index);
      mTracker.setState(station_index, stations[i].enabled);
      mTracker.setAngleFormat(station_index, ISD_EULER);
      mTracker.setButtons(station_index, stations[i].useDigital);
      mTracker.setAnalogData(station_index, stations[i].useAnalog);
      // Save the config state to the physical tracker.
      mTracker.saveConfigState(station_index);
   }

   // Ensure that we have not already started sampling.
   if (this->isActive() == true)
   {
      vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_WARNING_LVL)
      << "gadget::IntersenseAPI was already started." << std::endl
      << vprDEBUG_FLUSH;
      return 0;
   }

   // Ensure that the thread has not already started.
   if(mThread != NULL)
   {
      vprDEBUG(vprDBG_ERROR,vprDBG_CRITICAL_LVL)
      << clrOutNORM(clrRED,"ERROR:")
      << "gadget::IntersenseAPI: startSampling called, when already sampling.\n"
      << vprDEBUG_FLUSH;
      return 0;
   }

   // Open the connection to the tracker
   mTracker.open(mISenseDriverLocation);
   if (this->isActive() == false)
   {
      vprDEBUG(vprDBG_ERROR,vprDBG_CRITICAL_LVL)
      << clrOutNORM(clrRED,"ERROR:")
      << "gadget::IntersenseAPI: mTracker.open() failed to connect to tracker.\n"
      << vprDEBUG_FLUSH;
      return 0;
   }

   // Set flag that will later allow us to stop the control loop.
   mDone = false;

   // Create a new thread to handle the control
   vpr::ThreadMemberFunctor<IntersenseAPI>* memberFunctor =
   new vpr::ThreadMemberFunctor<IntersenseAPI>(this, &IntersenseAPI::controlLoop, NULL);
   mThread = new vpr::Thread(memberFunctor);

   if ( ! mThread->valid() )
   {
      return 0;   // Failed
   }
   else
   {
      return 1;   // Succeeded
   }
}

bool IntersenseAPI::sample()
{
   // If we are not active, then don't try to sample.
   if (!isActive())
   {
      return 0;
   }

   // Create the data buffers to put the new data into.
   std::vector<gadget::PositionData>   cur_pos_samples( mTracker.getNumStations() );
   std::vector<gadget::DigitalData>    cur_digital_samples;
   std::vector<gadget::AnalogData>     cur_analog_samples;


   // get an initial timestamp for this entire sample. we'll copy it into
   // each PositionData for this sample.
   if ( ! cur_pos_samples.empty() )
   {
      cur_pos_samples[0].setTime();
   }

   mTracker.updateData();

   vpr::Thread::yield();


   for (int i = 0 ; i < (mTracker.getNumStations()); i++)
   {
      // Get the station index for the given station.
      int stationIndex = stations[i].stationIndex;

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
         gmtl::setTrans( cur_pos_samples[i].mPosData, gmtl::Vec3f(mTracker.xPos( stationIndex ),
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

      // We start at the index of the first digital item (set in the config files)
      // and we copy the digital data from this station to the Intersense device for range (min -> min+count-1)

      // Get the miniumum and maximum index that we should write data into.
      int min_digital = stations[i].dig_min;
      int max_digital = min_digital + stations[i].dig_num;
      if (stations[i].useDigital)
      {
         int j, k;
         for ( j = 0, k = min_digital; (j < MAX_NUM_BUTTONS) && (k < IS_BUTTON_NUM) && (k < max_digital); j++, k++)
         {
            DigitalData new_digital(mTracker.buttonState(stationIndex, j));
            new_digital.setTime();
            cur_digital_samples.push_back(new_digital);
         }
      }

      // Analog works the same as the digital
      int min_analog = stations[i].ana_min;
      int max_analog = min_analog + stations[i].ana_num;
      if (stations[i].useAnalog)
      {
         float f;
         int j, k;
         for ( j = 0, k = min_analog; (j < MAX_ANALOG_CHANNELS) && (k < IS_ANALOG_NUM) && (k < max_analog); j++, k++)
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

   return 1;
}

bool IntersenseAPI::stopSampling()
{
   // Make sure that we are sampling in the first place.
   if (this->isActive() == false)
   {
      return 0;
   }

   if (mThread != NULL)
   {
      vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_CONFIG_LVL)
         << "gadget::IntersenseAPI::stopSampling(): Stopping the intersense thread... "
         << vprDEBUG_FLUSH;

      // Set the done flag to attempt to cause the control loop to exit naturally.
      mDone = true;

      // XXX: This is not a valid way to kill a thread, we must allow it to die
      // naturally.
      // mThread->kill();
      //delete mThread;
      //mThread = NULL;

      // Close the connection to the tracker
      mTracker.close();

      // If the tracker failed to stop
      if (isActive() == true)
      {
         vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_CRITICAL_LVL)
            << clrOutNORM(clrRED,"\nERROR:")
            << "gadget::IntersenseAPI::stopSampling(): Intersense tracker failed to stop.\n"
            << vprDEBUG_FLUSH;
         return 0;
      }

      vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_CONFIG_LVL)
         << "*** IntersenseAPI has been shutdown. ***" << std::endl << vprDEBUG_FLUSH;
   }
   return 1;
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
