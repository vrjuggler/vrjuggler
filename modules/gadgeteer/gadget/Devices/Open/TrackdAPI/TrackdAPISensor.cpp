/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2002 by Iowa State University
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
 * File:          TrackdSensor.cpp,v
 * Date modified: 2002/06/05 22:30:48
 * Version:       1.22
 * -----------------------------------------------------------------
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

#include <gadget/gadgetConfig.h>
#include <gadget/Devices/Open/Trackd/TrackdSensorStandalone.h>
#include <gadget/Devices/Open/Trackd/TrackdSensor.h>
#include <jccl/Config/ConfigChunk.h>

#include <vpr/Util/Debug.h>


namespace gadget
{

 // ------------------------------------------------------------------------
 //: Constructor.
 // ------------------------------------------------------------------------
 TrackdAPISensor::TrackdAPISensor(): mTrackerReader(NULL)
 {;}

 // ------------------------------------------------------------------------
 //: Destructor.
 //
 //! PRE: None.
 //! POST: Shared memory is released
 // ------------------------------------------------------------------------
 TrackdAPISensor::~TrackdAPISensor()
 {
    // This may cause problems on windows
    delete mTrackerReader;
 }

 // ------------------------------------------------------------------------
 //: Configure the trackd sensor with the given config chunk.
 //
 // -Create the trackdSensor based on config info
 // -set to active
 // -grow the vector to however many values we need
 bool TrackdAPISensor::config(jccl::ConfigChunkPtr c)
 {
    if(! (Input::config(c) && Position::config(c)))
       return false;

    // Create sensor
    int shm_key = c->getProperty<int>("shm_key");
    if(shm_key == 0)
    {
       vprDEBUG(vprDBG_ALL,vprDBG_CONFIG_LVL) << "vjTrackdAPISensor::config: Bad shm_key sent: Had value of 0.\n" << vprDEBUG_FLUSH;
       return false;
    }
    mTrackerReader = new TrackerReader(shm_key);

    // set to active
    mActive = true;

    // grow vector
    mCurSensorValues.resize(mTrackerReader->trackdGetNumberOfSensors());
    
    return true;
 }


 // ------------------------------------------------------------------------
 //: Update to the sampled data.
 //
 //! PRE: None.
 //! POST: Most recent value is copied over to temp area
 // ------------------------------------------------------------------------
 void TrackdAPISensor::updateData()
 {
    vprASSERT(mTrackerReader != NULL && "Make sure that trackd sensors has been initialized");
    vprASSERT((unsigned)mTrackerReader->trackdGetNumberOfSensors() <= mCurSensorValues.size());

    for(int i=0;i<mTrackerReader->trackdGetNumberOfSensors();i++)
    {
       *(mCurSensorValues[i].getPosition())
           = getSensorPos(i);
       mCurSensorValues[i].setTime();
    }

    // Update the data buffer
    mPosSamples.lock();
    mPosSamples.addSample(mCurSensorValues);
    mPosSamples.unlock();

    // Swap it
    mPosSamples.swapBuffers();
 }

 // Return the position of the given sensor
gmtl::Matrix44f TrackdAPISensor::getSensorPos(int sensorNum)
{
   assert(mMem != NULL && "We don't have a valid trackd memory area");
   assert(sensorNum < numSensors() && "Out of bounds request for a sensor");

   gmtl::Matrix44f ret_val;
   float tmp[4][4]; 
   
   mMem->trackdGetMatrix(sensorNum, tmp);

   // AJS - some obvious set/get functions seem to be missing in GMTL
   ret_val[0]=tmp[0][0];
   ret_val[1]=tmp[0][1];
   ret_val[2]=tmp[0][2];
   ret_val[3]=tmp[0][3];

   ret_val[4]=tmp[1][0];
   ret_val[5]=tmp[1][1];
   ret_val[6]=tmp[1][2];
   ret_val[7]=tmp[1][3];

   ret_val[8]=tmp[2][0];
   ret_val[9]=tmp[2][1];
   ret_val[10]=tmp[2][2];
   ret_val[11]=tmp[2][3];

   ret_val[12]=tmp[3][0];
   ret_val[13]=tmp[3][1];
   ret_val[14]=tmp[3][2];
   ret_val[15]=tmp[3][3];


   return ret_val;
}




};
