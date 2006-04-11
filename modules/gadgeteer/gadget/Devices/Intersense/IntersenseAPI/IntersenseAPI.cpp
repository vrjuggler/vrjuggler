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

#include <vpr/vprConfig.h>
#include <vpr/System.h>

#include <gmtl/Matrix.h>
#include <gmtl/Vec.h>
#include <gmtl/MatrixOps.h>
#include <gmtl/Generate.h>
#include <gmtl/EulerAngle.h>
#include <gmtl/Quat.h>

#include <jccl/Config/ConfigChunk.h>
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
    vprASSERT((ret_val >= 0) && (ret_val < ((mTracker.NumStations()+1)*3)));
    return ret_val;
}



IntersenseAPI::IntersenseAPI()
{
    vprDEBUG(gadgetDBG_INPUT_MGR,1)
       << "*** Intersense::Intersense() ***\n" << vprDEBUG_FLUSH;
    //vprDEBUG(gadgetDBG_INPUT_MGR,1) << "*** Intersense::deviceAbilities = " << deviceAbilities << " ***\n" << vprDEBUG_FLUSH;
}

bool IntersenseAPI::config(jccl::ConfigChunkPtr c)
{
    vprDEBUG(gadgetDBG_INPUT_MGR,1)
       << "         Intersense::config(jccl::ConfigChunkPtr)"
       << std::endl << vprDEBUG_FLUSH;

// read in Position's, Digital's, and Analog's config stuff,
// --> this will be the port and baud fields
   if(! (Input::config(c) && Position::config(c) && Digital::config(c) && Analog::config(c) ))
      return false;

   mISenseDriverLocation = c->getProperty<std::string>("driver");


   mPortName = c->getProperty<std::string>("port");
   mBaudRate = c->getProperty<int>("baud");

// keep IntersenseStandalone's port and baud members in sync with Input's port
// and baud members.
   mTracker.setPortName( mPortName );
   mTracker.setBaudRate( mBaudRate );
   mTracker.rNumStations() = c->getNum("stations");

   if(stations != NULL) delete [] stations;
   stations = new ISStationConfig[mTracker.rNumStations()];
   jccl::ConfigChunkPtr stationConfig;
   for( int i = 0; i < mTracker.rNumStations(); i++)
   {
      stationConfig = c->getProperty<jccl::ConfigChunkPtr>("stations", i);
      stations[i].enabled = stationConfig->getProperty<bool>("enabled");
      stations[i].stationIndex = stationConfig->getProperty<int>("stationIndex");
      stations[i].useDigital = stationConfig->getProperty<bool>("useDigital");
      stations[i].useAnalog = stationConfig->getProperty<bool>("useAnalog");

      stations[i].dig_min = stationConfig->getProperty<int>("digitalFirst");
      stations[i].dig_num = stationConfig->getProperty<int>("digitalNum");
      stations[i].ana_min = stationConfig->getProperty<int>("analogFirst");
      stations[i].ana_num = stationConfig->getProperty<int>("analogNum");
   }

   // load an init script for the tracker and then pass it to mTracker
   const char* filename = c->getProperty<std::string>("script").c_str();
   std::stringstream script;
   std::ifstream scriptFile;
   scriptFile.open(filename);
   script<<scriptFile.rdbuf();
   mTracker.setScript(script.str().c_str());
   scriptFile.close();
   mTracker.rVerbose() = c->getProperty<bool>("verbose");

   return true;
}

IntersenseAPI::~IntersenseAPI()
{
    this->stopSampling();
    if (stations != NULL)
        delete [] stations;
}

// Main thread of control for this active object
void IntersenseAPI::controlLoop(void* nullParam)
{

    // Configure the stations used by the configuration
    int j = 0;
    for( int i = 0; i < mTracker.NumStations(); i++ )
    {
        j = stations[i].stationIndex;

        mTracker.getConfigState(j);
        mTracker.rState(j) = stations[i].enabled;
        mTracker.rAngleFormat(j) = ISD_EULER;
        mTracker.rButtons(j) = stations[i].useDigital;
        mTracker.rAnalogData(j) = stations[i].useAnalog;
        mTracker.setConfigState(j);
    }

// Loop through and keep sampling
    for (;;)
    {
        this->sample();
        vpr::System::msleep(50);
    }
}

int IntersenseAPI::startSampling()
{
// make sure inertia cubes aren't already started
    if (this->isActive() == true)
    {
        vprDEBUG(gadgetDBG_INPUT_MGR,2) << "vjIntersense was already started."
                                        << std::endl << vprDEBUG_FLUSH;
        return 0;
    }

// Has the thread actually started already
    if(mThread != NULL)
    {
        vprDEBUG(vprDBG_ERROR,vprDBG_CRITICAL_LVL) << clrOutNORM(clrRED,"ERROR:")
                                                << "vjIntersense: startSampling called, when already sampling.\n"
                                                << vprDEBUG_FLUSH;
        vprASSERT(false);
    } else {

// open the tracker connection
   mTracker.open(mISenseDriverLocation);
      if (this->isActive() == false) {
      vprDEBUG(vprDBG_ERROR,vprDBG_CRITICAL_LVL) << clrOutNORM(clrRED,"ERROR:")
                  << "vjIntersense: mTracker.open() failed to connect to tracker.\n"
                  << vprDEBUG_FLUSH;
      return 0;
      }

// Create a new thread to handle the control
        vpr::ThreadMemberFunctor<IntersenseAPI>* memberFunctor =
            new vpr::ThreadMemberFunctor<IntersenseAPI>(this, &IntersenseAPI::controlLoop, NULL);
        mThread = new vpr::Thread(memberFunctor);

        if ( ! mThread->valid() )
        {
            return 0;  // Fail
        }
        else
        {
            return 1;   // success
        }
    }

    return 0;
}

int IntersenseAPI::sample()
{
    if (this->isActive() == false)
        return 0;

    int i,  j;

    IsenseData cur_sample;
    std::vector< gadget::PositionData > cur_pos_samples( mTracker.NumStations() );

    mTracker.updateData();

    // get an initial timestamp for this entire sample. we'll copy it into
    // each PositionData for this sample.
    //int firstindex;
    if (!cur_pos_samples.empty())
    {
        //firstindex = getStationIndex (0, progress);
        cur_pos_samples[0].setTime();
    }

    int k;
    int stationIndex;
    int min, num;

    vpr::Thread::yield();

   for (i = 0 ; i < (mTracker.NumStations()); i++)
   {
      //int index = getStationIndex(i,progress);

      stationIndex = stations[i].stationIndex;

      if ( mTracker.rAngleFormat(stationIndex) == ISD_EULER )
      {
         gmtl::identity(cur_pos_samples[i].mPosData);
         gmtl::EulerAngleZYXf euler( gmtl::Math::deg2Rad( mTracker.zRot( stationIndex ) ),
                                     gmtl::Math::deg2Rad( mTracker.yRot( stationIndex ) ),
                                     gmtl::Math::deg2Rad( mTracker.xRot( stationIndex ) ) );
         gmtl::setRot( cur_pos_samples[i].mPosData, euler );
         gmtl::setTrans( cur_pos_samples[i].mPosData,
                            gmtl::Vec3f(mTracker.xPos( stationIndex )*3.2808,
                                        mTracker.yPos( stationIndex )*3.2808,
                                        mTracker.zPos( stationIndex )*3.2808) );
      }
      else
      {
         gmtl::Quatf quatValue(mTracker.xQuat( stationIndex ),
                             mTracker.yQuat( stationIndex ),
                             mTracker.zQuat( stationIndex ),
                             mTracker.wQuat( stationIndex ));
         gmtl::set( cur_pos_samples[i].mPosData, quatValue );
      }

      cur_pos_samples[i].setTime (cur_pos_samples[0].getTime());

      // We start at the index of the first digital item (set in the config files)
      // and we copy the digital data from this station to the Intersense device for range (min -> min+count-1)
      min = stations[i].dig_min;
      num = min + stations[i].dig_num;
      if (stations[i].useDigital)
      {
         for ( j = 0, k = min; (j < MAX_NUM_BUTTONS) && (k < IS_BUTTON_NUM) && (k < num); j++, k++)
         {
            //mInput[progress].digital[k] = mTracker.buttonState(stationIndex, j);
            cur_sample.digital[k] = mTracker.buttonState(stationIndex, j);
         }
      }
      // Analog works the same as the digital
      min = stations[i].ana_min;
      num = min + stations[i].ana_num;
      if (stations[i].useAnalog)
      {
         float f;
         for ( j = 0, k = min; (j < MAX_ANALOG_CHANNELS) && (k < IS_ANALOG_NUM) && (k < num); j++, k++)
         {
            Analog::normalizeMinToMax(mTracker.analogData(stationIndex, j), f);
            //mInput[progress].analog[k] = f;
            cur_sample.analog[k] = f;
         }
      }

   }

   // Locks and then swaps the indices.
   addAnalogSample(cur_sample.analog);
   addDigitalSample(cur_sample.digital);
   addPositionSample(cur_pos_samples);

    return 1;
}

int IntersenseAPI::stopSampling()
{
   if (this->isActive() == false)
      return 0;

   if (mThread != NULL)
   {
      vprDEBUG(gadgetDBG_INPUT_MGR,1)
         << "vjIntersense::stopSampling(): Stopping the intersense thread... "
         << vprDEBUG_FLUSH;

      mThread->kill();
      delete mThread;
      mThread = NULL;

      mTracker.close();

      if (this->isActive() == true)
      {
         vprDEBUG(gadgetDBG_INPUT_MGR,0)
            << clrOutNORM(clrRED,"\nERROR:")
            << "vjIntersense::stopSampling(): Intersense tracker failed to stop.\n"
            << vprDEBUG_FLUSH;
         return 0;
      }

      vprDEBUG(gadgetDBG_INPUT_MGR,1) << "stopped." << std::endl
                                      << vprDEBUG_FLUSH;
   }

   return 1;
}


void IntersenseAPI::updateData()
{
   if (!isActive())
      return;

   swapDigitalBuffers();
   swapAnalogBuffers();
   swapPositionBuffers();
}

} // End of gadget namespace
