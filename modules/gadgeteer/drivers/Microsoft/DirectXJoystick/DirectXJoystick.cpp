/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2005 by Iowa State University
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dinput.h>

#include <vpr/Util/Debug.h>
#include <jccl/Config/ConfigElement.h>
#include <gadget/Type/DeviceConstructor.h>
#include <gadget/Util/Debug.h>
#include <gadget/gadgetParam.h>

#include <drivers/Microsoft/DirectXJoystick/DirectXJoystickExceptions.h>
#include <drivers/Microsoft/DirectXJoystick/DirectXJoystick.h>


extern "C"
{

GADGET_DRIVER_EXPORT(vpr::Uint32) getGadgeteerVersion()
{
   return __GADGET_version;
}

GADGET_DRIVER_EXPORT(void) initDevice(gadget::InputManager* inputMgr)
{
   new gadget::DeviceConstructor<gadget::DirectXJoystick>(inputMgr);
}

}

namespace gadget
{

/** Constructor. */
DirectXJoystick::DirectXJoystick()
   : mActive(false)
{
}

/**
 * Destructor.
 *
 * @pre None.
 * @post Shared memory is released.
 */
DirectXJoystick::~DirectXJoystick()
{
}

std::string DirectXJoystick::getElementType()
{
   return "directx_joystick";
}

/**
 * config 
 */
bool DirectXJoystick::config(jccl::ConfigElementPtr e)
{
   if(! (Input::config(e) && Digital::config(e) && Analog::config(e)))
   {
      return false;
   }

   mJsLabel = e->getName();

   unsigned num_axis_buttons = e->getNum("axis_buttons");
   for ( unsigned i = 0; i < num_axis_buttons; ++i )
   {
      unsigned idx = e->getProperty<int>("axis_buttons", i);
      mAxisButtonIndices.push_back(idx);
   }

   if ( getMin() >= getMax() )
   {
      setMin((float) DirectXJoystickStandalone::getAxisMin());
      setMax((float) DirectXJoystickStandalone::getAxisMax());
   }

   return true;
}

bool DirectXJoystick::startSampling()
{
   vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_CONFIG_STATUS_LVL)
      << "Opening Win Joystick driver " << std::endl << vprDEBUG_FLUSH;

   try
   {
      mInputDrv.init();
      mActive = true;
   }
   catch(DirectXJoystickException& ex)
   {
      vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_CRITICAL_LVL)
         << clrOutBOLD(clrRED, "ERROR:")
         << "[gadget::DirectXJoystick::startSampling()] Failed to open Direct Input joystick:"
         << vprDEBUG_FLUSH;
      vprDEBUG_NEXT(gadgetDBG_INPUT_MGR, vprDBG_CRITICAL_LVL)
         << ex.what() << std::endl << vprDEBUG_FLUSH;
      return false;
   }

   // FIXME: hardcoded number of axes and buttons
   int version = 0;
   mNumAxes = 9;       // Analog = 8   from DIJOYSTATE
   mNumButtons = 10;   // Digital = 32  from DIJOYSTATE

   // Output joystick description
   vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_CONFIG_LVL)
         << "  joystick label: " << mJsLabel << std::endl
         << "   joystick name: " << mInputDrv.getProductName() << std::endl
         << "            axes: " << mNumAxes << std::endl
         << "         buttons: " << mNumButtons << std::endl
         << "      driver ver: " << version << std::endl
         << "    axis buttons: ";

   for(unsigned i=0;i<mAxisButtonIndices.size(); ++i)
   { 
      vprDEBUG_CONTnl(gadgetDBG_INPUT_MGR, vprDBG_CONFIG_LVL)
         << mAxisButtonIndices[i] << " ";
   }
   vprDEBUG_CONTnl(gadgetDBG_INPUT_MGR, vprDBG_CONFIG_LVL)
      << std::endl << vprDEBUG_FLUSH;

   // Allocate initial device data
   // - By default this will clear them out
   mCurAxes.resize(mNumAxes);
   mCurAxesRanges.resize(mNumAxes, axis_range_t(-100.0f, 100.0f));             // Initialize ranges to 0,255
   mCurButtons.resize(mNumButtons + mAxisButtonIndices.size());

   // Setup axis as button stuff
   mAxisToButtonIndexLookup.clear();
   mAxisToButtonIndexLookup.resize(mNumAxes, -1);            // Default to -1, meaning no axis button
   for(unsigned i=0;i<mAxisButtonIndices.size(); ++i)       // For each configured axis index
   {
      unsigned virtual_btn_index = (mNumButtons+i);                     // Index of the virtual button from the axis
      vprASSERT(virtual_btn_index < mCurButtons.size() && "Virtual button index out of range");
      unsigned axis_index = mAxisButtonIndices[i];                      // Index of the axis we are mapping
      mAxisToButtonIndexLookup[axis_index] = int(virtual_btn_index);    // Setup the mapping
   }

   return true;
}

/**
 * Stops sampling.
 * Drop connection to joystick and clear everything.
 */
bool DirectXJoystick::stopSampling()
{
   if ( mActive )
   {
      mInputDrv.close();  // Close the joystick device
      mActive = false;
   }

   return true;
}

/**
 * Updates to the sampled data.
 *
 * @pre None.
 * @post Most recent value is copied over to temp area.
 */
void DirectXJoystick::updateData()
{
   if ( mInputDrv.poll() )
   {
      const DIJOYSTATE& mJsData = mInputDrv.getData(); 

      //FIXME: if & only if there is events happen, do setTime
      // for buttons, do update 
      for ( unsigned int i = 0; i < mCurButtons.size(); ++i )
      {
         mCurButtons[i].setDigital(mJsData.rgbButtons[i]);
         if( mCurButtons[i].getDigital() != 0)
         {
            mCurButtons[i].setTime();
         }
      }

      // for axes, do update
      float norm_value;
      normalizeMinToMax(mJsData.lX, norm_value);
      mCurAxes[0] = norm_value;
      normalizeMinToMax(mJsData.lY, norm_value);
      mCurAxes[1] = norm_value;
      normalizeMinToMax(mJsData.lZ, norm_value);
      mCurAxes[2] = norm_value;
      normalizeMinToMax(mJsData.lRx, norm_value); // x rotation
      mCurAxes[3] = norm_value;
      normalizeMinToMax(mJsData.lRy, norm_value); // y rotation
      mCurAxes[4] = norm_value;
      normalizeMinToMax(mJsData.lRz, norm_value); // z rotation
      mCurAxes[5] = norm_value;
      normalizeMinToMax(mJsData.rglSlider[0], norm_value); // u-axis
      mCurAxes[6] = norm_value;
      normalizeMinToMax(mJsData.rglSlider[1], norm_value); // v-axis
      mCurAxes[7] = norm_value;
      mCurAxes[8] = (float(mJsData.rgdwPOV[0])); //hat: -1, 0, 9,000, 18,000, or 27,000.

      // use axes as button, only first 3 are tested.
      for ( unsigned int axis_number = 0; axis_number < mNumAxes; ++axis_number )
      {
         // FIXME: don't know when it is changed
         mCurAxes[axis_number].setTime();

         // Check for axis buttons
         // - If we have a mapping
         // - If axis is gt 0.5, then btn is down
         if ( axis_number < 3 )
         {
            float norm_value(0.0f); // mCurAxes[axis_number].getAnalog();
            if ( mAxisToButtonIndexLookup[axis_number] != -1 ) // If we map to a virtual button
            {
               unsigned vir_btn_index = mAxisToButtonIndexLookup[axis_number];
               vprASSERT(vir_btn_index < mCurButtons.size() && "Virtual button index out of range");
               mCurButtons[vir_btn_index] = ((norm_value > 0.5f) ? 1 : 0);
               mCurButtons[vir_btn_index].setTime();
            }
         }
      }

      addDigitalSample(mCurButtons);
      swapDigitalBuffers();

      addAnalogSample(mCurAxes);
      swapAnalogBuffers();
   }
}

} // End of gadget namespace
