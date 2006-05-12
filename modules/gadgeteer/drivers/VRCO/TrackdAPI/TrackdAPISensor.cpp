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
#include <drivers/VRCO/TrackdAPI/TrackdAPISensor.h>
#include <jccl/Config/ConfigElement.h>

#include <vpr/Util/Debug.h>


namespace gadget
{

 /**
  * Constructor.
  */
 TrackdAPISensor::TrackdAPISensor(): mTrackerReader(NULL)
 {;}

 /**
  * Destructor.
  *
  * @pre None.
  * @post Shared memory is released.
  */
 TrackdAPISensor::~TrackdAPISensor()
 {
    // This may cause problems on windows
    delete mTrackerReader;
 }

 std::string TrackdAPISensor::getElementType()
 {
    return "trackd_api_sensor";
 }

 /**
  * Configures the trackd sensor with the given config element.
  *
  * -Create the trackdSensor based on config info<br>
  * -set to active<br>
  * -grow the vector to however many values we need<br>
  */
 bool TrackdAPISensor::config(jccl::ConfigElementPtr e)
 {
    if(! (Input::config(e) && Position::config(e)))
    {
       return false;
    }

    // Create sensor
    int shm_key = e->getProperty<int>("shared_memory_key");
    if(shm_key == 0)
    {
       vprDEBUG(vprDBG_ALL,vprDBG_CONFIG_LVL)
         << "gadget::TrackdAPISensor::config: Bad shm_key sent: Had value of 0."
         << std::endl << vprDEBUG_FLUSH;
       return false;
    }
    mTrackerReader = new TrackerReader(shm_key);

    // set to active
    mActive = true;

    return true;
 }


 /**
  * Updates the sampled data.
  *
  * @pre None.
  * @post Most recent value is copied over to temp area.
  */
 void TrackdAPISensor::updateData()
 {
    vprASSERT(mTrackerReader != NULL && "Make sure that trackd sensors has been initialized");
    std::vector<gadget::PositionData> cur_samples(mTrackerReader->trackdGetNumberOfSensors());

    if ( ! cur_samples.empty() )
    {
       cur_samples[0].setTime();
    }

    for(int i=0;i<mTrackerReader->trackdGetNumberOfSensors();i++)
    {
       cur_samples[i].setTime(cur_samples[0].getTime());
       cur_samples[i].mPosData = getSensorPos(i);
    }

    // Update the data buffer
    addPositionSample(cur_samples);
        
    // Swap it
    swapPositionBuffers();
 }

/** Returns the position of the given sensor. */
gmtl::Matrix44f TrackdAPISensor::getSensorPos(int sensorNum)
{
   assert(mTrackerReader != NULL && "We don't have a valid trackd memory area");
   assert(sensorNum < mTrackerReader->trackdGetNumberOfSensors() && "Out of bounds request for a sensor");

   gmtl::Matrix44f ret_val;
   float mat_data[4][4];

   mTrackerReader->trackdGetMatrix(sensorNum, mat_data);

   // AJS - some obvious set/get functions seem to be missing in GMTL
   ret_val.mData[0] = mat_data[0][0];
   ret_val.mData[1] = mat_data[0][1];
   ret_val.mData[2] = mat_data[0][2];
   ret_val.mData[3] = mat_data[0][3];

   ret_val.mData[4] = mat_data[1][0];
   ret_val.mData[5] = mat_data[1][1];
   ret_val.mData[6] = mat_data[1][2];
   ret_val.mData[7] = mat_data[1][3];

   ret_val.mData[8]  = mat_data[2][0];
   ret_val.mData[9]  = mat_data[2][1];
   ret_val.mData[10] = mat_data[2][2];
   ret_val.mData[11] = mat_data[2][3];

   ret_val.mData[12] = mat_data[3][0];
   ret_val.mData[13] = mat_data[3][1];
   ret_val.mData[14] = mat_data[3][2];
   ret_val.mData[15] = mat_data[3][3];

   return ret_val;
}

} // End of gadget namespace
