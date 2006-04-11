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
#include <gadget/Devices/Open/Trackd/TrackdSensorStandalone.h>
#include <gadget/Devices/Open/Trackd/TrackdSensor.h>
#include <jccl/Config/ConfigChunk.h>

#include <vpr/Util/Debug.h>


namespace gadget
{

/**
 * Constructor.
 */
TrackdSensor::TrackdSensor(): mTrackdSensors(NULL)
{;}

/**
 * Destructor.
 *
 * @pre None.
 * @post Shared memory is released
 */
TrackdSensor::~TrackdSensor()
{
   delete mTrackdSensors;
}

 /**
  * Configures the trackd sensor with the given configuration element.
  *
  * -Create the trackdSensor based on config info<br>
  * -set to active<br>
  * -grow the vector to however many values we need
  */
 bool TrackdSensor::config(jccl::ConfigChunkPtr c)
 {
    if(! (Input::config(c) && Position::config(c)))
       return false;

    // Create sensor
    int shm_key = c->getProperty<int>("shm_key");
    if(shm_key == 0)
    {
       vprDEBUG(vprDBG_ALL,vprDBG_CONFIG_LVL) << "vjTrackdSensor::config: Bad shm_key sent: Had value of 0.\n" << vprDEBUG_FLUSH;
       return false;
    }
    mTrackdSensors = new TrackdSensorStandalone(shm_key);

    // set to active
    mActive = true;

    // grow vector
    mCurSensorValues.resize(mTrackdSensors->numSensors());

    return true;
 }


 /**
  * Updates to the sampled data.
  *
  * @pre None.
  * @post Most recent value is copied over to temp area
  */
 void TrackdSensor::updateData()
 {
    vprASSERT(mTrackdSensors != NULL && "Make sure that trackd sensors has been initialized");
    vprASSERT((unsigned)mTrackdSensors->numSensors() <= mCurSensorValues.size());

    for(int i=0;i<mTrackdSensors->numSensors();i++)
    {
       mCurSensorValues[i].mPosData = mTrackdSensors->getSensorPos(i);
       mCurSensorValues[i].setTime();
    }

    // Update the data buffer
    addPositionSample(mCurSensorValues);
    swapPositionBuffers();
 }



} // End of gadget namespace
