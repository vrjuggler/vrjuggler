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

DirectXJoystick::DirectXJoystick()
   : mActive(false)
{
}

DirectXJoystick::~DirectXJoystick() throw ()
{
   stopSampling();
}

std::string DirectXJoystick::getElementType()
{
   return "directx_joystick";
}

bool DirectXJoystick::config(jccl::ConfigElementPtr e)
{
   if(! (Input::config(e) && Digital::config(e) && Analog::config(e)))
   {
      return false;
   }

   mJsLabel = e->getName();

   unsigned int num_axis_buttons = e->getNum("axis_buttons");
   for ( unsigned int i = 0; i < num_axis_buttons; ++i )
   {
      unsigned idx = e->getProperty<int>("axis_buttons", i);
      mAxisButtonIndices.push_back(idx);
   }

   // Override whatever the user configured with the minimum and maximum values
   // used by the standalone Direct Input device.  User configuration input is
   // useless in this case.
   setMin((float) DirectXJoystickStandalone::getAxisMin());
   setMax((float) DirectXJoystickStandalone::getAxisMax());

   return true;
}

bool DirectXJoystick::startSampling()
{
   vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_STATE_LVL)
      << "Initializing Direct Input joystick device" << std::endl
      << vprDEBUG_FLUSH;

   try
   {
      mInputDrv.init();
      mActive = true;
   }
   catch(DirectXJoystickException& ex)
   {
      vprDEBUG(vprDBG_ERROR, vprDBG_CRITICAL_LVL)
         << clrOutBOLD(clrRED, "ERROR:")
         << " [gadget::DirectXJoystick::startSampling()] "
         << "Failed to initialize Direct Input joystick:" << std::endl
         << vprDEBUG_FLUSH;
      vprDEBUG_NEXT(vprDBG_ERROR, vprDBG_CRITICAL_LVL)
         << ex.what() << std::endl << vprDEBUG_FLUSH;
      return false;
   }

   // Output joystick description
   vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_CONFIG_LVL)
      << std::endl
      << "  joystick label: " << mJsLabel << std::endl
      << "   joystick name: " << mInputDrv.getProductName() << std::endl
      << "            axes: " << mInputDrv.getNumAxes() << std::endl
      << "         buttons: " << mInputDrv.getNumButtons() << std::endl
      << "    firmware rev: " << mInputDrv.getFirmwareRev() << std::endl
      << "    hardware rev: " << mInputDrv.getHardwareRev() << std::endl
      << "    axis buttons: ";

   for ( unsigned int i = 0; i < mAxisButtonIndices.size(); ++i )
   { 
      vprDEBUG_CONTnl(gadgetDBG_INPUT_MGR, vprDBG_CONFIG_LVL)
         << mAxisButtonIndices[i] << " ";
   }
   vprDEBUG_CONTnl(gadgetDBG_INPUT_MGR, vprDBG_CONFIG_LVL)
      << std::endl << vprDEBUG_FLUSH;

   // Allocate initial device data.  By default this will clear them out.
   const DWORD num_axes(mInputDrv.getNumAxes()),
               num_buttons(mInputDrv.getNumButtons());
   mCurAxes.resize(num_axes);
   mCurAxesRanges.resize(num_axes,
                         axis_range_t((float) DirectXJoystickStandalone::getAxisMin(),
                                      (float) DirectXJoystickStandalone::getAxisMax()));
   mCurButtons.resize(num_buttons + mAxisButtonIndices.size());

   // Set up axis-as-button stuff.
   // Default to -1, meaning no axis button.
   mAxisToButtonIndexLookup.clear();
   mAxisToButtonIndexLookup.resize(num_axes, -1);

   for ( unsigned int i = 0; i < mAxisButtonIndices.size(); ++i )
   {
      // Index of the virtual button from the axis.
      unsigned int virtual_btn_index = num_buttons + i;
      vprASSERT(virtual_btn_index < mCurButtons.size() &&
                "Virtual button index out of range");
      // Index of the axis we are mapping.
      unsigned int axis_index = mAxisButtonIndices[i];
      // Set up the mapping.
      mAxisToButtonIndexLookup[axis_index] = int(virtual_btn_index);
   }

   return true;
}

// Stops sampling.
// Drop connection to joystick and clear everything.
bool DirectXJoystick::stopSampling()
{
   if ( mActive )
   {
      mInputDrv.close();  // Close the joystick device
      mActive = false;
   }

   return true;
}

// Updates to the sampled data.
void DirectXJoystick::updateData()
{
   if ( mInputDrv.poll() )
   {
      // Get button values.  We cannot use mCurButtons.size() here because that
      // value includes any axis buttons that were configured.
      for ( unsigned int i = 0; i < mInputDrv.getNumButtons(); ++i )
      {
         mCurButtons[i].setDigital(mInputDrv.getButtonValue(i));
         mCurButtons[i].setTime();
      }

      for ( unsigned int i = 0; i < mCurAxes.size(); ++i )
      {
         mCurAxes[i].setTime();

         float norm_value;
         normalizeMinToMax(mInputDrv.getAxisValue(i), norm_value);
         mCurAxes[i].setAnalog(norm_value);

         // If the current axis maps to a virtual button, update the correct
         // digital sample in mCurButtons.
         if ( mAxisToButtonIndexLookup[i] != -1 )
         {
            unsigned int vir_btn_index = mAxisToButtonIndexLookup[i];
            vprASSERT(vir_btn_index < mCurButtons.size() &&
                      "Virtual button index out of range");
            mCurButtons[vir_btn_index] = ((norm_value > 0.5f) ? 1 : 0);
            mCurButtons[vir_btn_index].setTime();
         }
      }


      addDigitalSample(mCurButtons);
      swapDigitalBuffers();

      addAnalogSample(mCurAxes);
      swapAnalogBuffers();
   }
}

} // End of gadget namespace
