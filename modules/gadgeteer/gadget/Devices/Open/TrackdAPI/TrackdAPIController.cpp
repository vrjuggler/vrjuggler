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
 * File:          TrackdController.cpp,v
 * Date modified: 2002/06/05 22:30:48
 * Version:       1.19
 * -----------------------------------------------------------------
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

#include <gadget/gadgetConfig.h>
#include <gadget/Devices/Open/Trackd/TrackdControllerStandalone.h>
#include <gadget/Devices/Open/Trackd/TrackdController.h>
#include <jccl/Config/ConfigChunk.h>

#include <vpr/Util/Debug.h>

namespace gadget
{


 // ------------------------------------------------------------------------
 //: Constructor.
 // ------------------------------------------------------------------------
 TrackdAPIController::TrackdAPIController(): mControllerReader(NULL)
 {;}

 // ------------------------------------------------------------------------
 //: Destructor.
 //
 //! PRE: None.
 //! POST: Shared memory is released
 // ------------------------------------------------------------------------
 TrackdAPIController::~TrackdAPIController()
 {
    delete mControllerReader;
 }

// ------------------------------------------------------------------------
//: Configure the trackd sensor with the given config chunk.
//
// -Create the trackdSensor based on config info
// -set to active
// -grow the vector to however many values we need
bool TrackdAPIController::config(jccl::ConfigChunkPtr c)
{
   if(! (Input::config(c) && Digital::config(c) && Analog::config(c)))
      return false;

   // Create sensor
   int shm_key = c->getProperty<int>("shm_key");
   if (shm_key == 0)
   {
      vprDEBUG(vprDBG_ALL,vprDBG_CONFIG_LVL) << "vjTrackdSensor::config: Bad shm_key sent: Had value of 0.\n" << vprDEBUG_FLUSH;
      return false;
   }
   mControllerReader = new ControllerReader(shm_key);

   // set to active
   mActive = true;

   // grow vector
   mCurValuators.resize(mControllerReader->trackdGetNumberOfValuators());
   mCurButtons.resize(mControllerReader->trackdGetNumberOfButtons());

   return true;
}


// ------------------------------------------------------------------------
//: Update to the sampled data.
//
//! PRE: None.
//! POST: Most recent value is copied over to temp area
// ------------------------------------------------------------------------
void TrackdAPIController::updateData()
{
   vprASSERT(mTrackdController != NULL && "Make sure that trackd controller has been initialized");
   vprASSERT((unsigned)mControllerReader->trackdGetNumberOfButtons() <= mCurButtons.size());
   vprASSERT((unsigned)mControllerReader->trackdGetNumberOfValuators() <= mCurValuators.size() );

   for (int i=0;i<mControllerReader->trackdGetNumberOfButtons();i++)
   {
      mCurButtons[i] = mControllerReader->trackdGetButton(i);
      mCurButtons[i].setTime();
   }

   for (int j=0;j<mControllerReader->trackdGetNumberOfValuators();j++)
   {
       // TrackdController doesn't have a sample, so we do
       // normalization here...
       float f;
       this->normalizeMinToMax (mControllerReader->trackdGetValuator(j), f);
       mCurValuators[j] = f;
       mCurValuators[j].setTime();
   }
   
   mDigitalSamples.lock();
   mDigitalSamples.addSample(mCurButtons);
   mDigitalSamples.unlock();
   mDigitalSamples.swapBuffers();


   mAnalogSamples.lock();
   mAnalogSamples.addSample(mCurValuators);
   mAnalogSamples.unlock();
   mAnalogSamples.swapBuffers();
  
}


};
