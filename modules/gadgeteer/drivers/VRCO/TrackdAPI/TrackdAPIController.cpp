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
#include <drivers/VRCO/TrackdAPI/TrackdAPIController.h>
#include <drivers/VRCO/TrackdAPI/TrackdAPISensor.h>
#include <jccl/Config/ConfigElement.h>
#include <vpr/Util/Debug.h>
#include <gadget/gadgetParam.h>

#include <gadget/Type/DeviceConstructor.h>


extern "C"
{

GADGET_DRIVER_EXPORT(vpr::Uint32) getGadgeteerVersion()
{
   return __GADGET_version;
}

GADGET_DRIVER_EXPORT(void) initDevice(gadget::InputManager* inputMgr)
{
   new gadget::DeviceConstructor<gadget::TrackdAPIController>(inputMgr);
   new gadget::DeviceConstructor<gadget::TrackdAPISensor>(inputMgr);
}

}

namespace gadget
{


/**
 * Constructor.
 */
TrackdAPIController::TrackdAPIController(): mControllerReader(NULL)
{;}

/**
 * Destructor.
 *
 * @pre None.
 * @post Shared memory is released.
 */
TrackdAPIController::~TrackdAPIController()
{
   delete mControllerReader;
}

std::string TrackdAPIController::getElementType()
{
   return "trackd_api_controller";
}

/**
 * Configures the trackd sensor with the given config element.
 *
 * -Create the trackdSensor based on config info.
 * -set to active.
 * -grow the vector to however many values we need.
 */
bool TrackdAPIController::config(jccl::ConfigElementPtr e)
{
   if(! (Input::config(e) && Digital::config(e) && Analog::config(e)))
   {
      return false;
   }

   // Create sensor
   int shm_key = e->getProperty<int>("shared_memory_key");
   vprDEBUG(vprDBG_ALL, vprDBG_CONFIG_LVL) << "TrackdAPIController::config: shared mem key:" << shm_key << std::endl << vprDEBUG_FLUSH;
   if (shm_key == 0)
   {
      vprDEBUG(vprDBG_ALL,vprDBG_CONFIG_LVL) << "TrackdAPIController::config: Bad shm_key sent: Had value of 0.\n" << vprDEBUG_FLUSH;
      return false;
   }
   mControllerReader = new ControllerReader(shm_key);

   // set to active
   mActive = true;

   // grow vector
   mCurValuators.resize(mControllerReader->trackdGetNumberOfValuators());
   mCurButtons.resize(mControllerReader->trackdGetNumberOfButtons());

   vprDEBUG(vprDBG_ALL, vprDBG_CONFIG_LVL) << "Num valuators:" << mCurValuators.size() << std::endl << vprDEBUG_FLUSH;
   vprDEBUG(vprDBG_ALL, vprDBG_CONFIG_LVL) << "Num buttons:" << mCurButtons.size() << std::endl << vprDEBUG_FLUSH;

   return true;
}


/**
 * Updates to the sampled data.
 *
 * @pre None.
 * @post Most recent value is copied over to temp area.
 */
void TrackdAPIController::updateData()
{
   vprASSERT(mControllerReader != NULL && "Make sure that trackd controller has been initialized");
   vprASSERT((unsigned)mControllerReader->trackdGetNumberOfButtons() <= mCurButtons.size());
   vprASSERT((unsigned)mControllerReader->trackdGetNumberOfValuators() <= mCurValuators.size() );

   for (int i=0;i<mControllerReader->trackdGetNumberOfButtons();i++)
   {
      mCurButtons[i] =
         static_cast<DigitalState::State>(mControllerReader->trackdGetButton(i));
      mCurButtons[i].setTime();
   }

   for (int j=0;j<mControllerReader->trackdGetNumberOfValuators();j++)
   {
       mCurValuators[j] = mControllerReader->trackdGetValuator(j);
       mCurValuators[j].setTime();
   }

   addDigitalSample(mCurButtons);
   swapDigitalBuffers();

   addAnalogSample(mCurValuators);
   swapAnalogBuffers();
}

} // End of gadget namespace
