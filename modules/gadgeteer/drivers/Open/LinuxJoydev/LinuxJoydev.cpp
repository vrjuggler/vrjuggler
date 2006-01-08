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
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <fcntl.h>

#include <linux/joystick.h>      // Get the joystick abilities

#include <vpr/Util/Debug.h>
#include <jccl/Config/ConfigElement.h>
#include <gadget/Type/DeviceConstructor.h>
#include <gadget/Util/Debug.h>
#include <gadget/gadgetParam.h>

#include <drivers/Open/LinuxJoydev/LinuxJoydev.h>


extern "C"
{

GADGET_DRIVER_EXPORT(vpr::Uint32) getGadgeteerVersion()
{
   return __GADGET_version;
}

GADGET_DRIVER_EXPORT(void) initDevice(gadget::InputManager* inputMgr)
{
   new gadget::DeviceConstructor<gadget::LinuxJoydev>(inputMgr);
}

}

namespace gadget
{

// Constructor.
LinuxJoydev::LinuxJoydev()
{
   /* Do nothing. */ ;
}

// Destructor.
LinuxJoydev::~LinuxJoydev() throw ()
{
   /* Do nothing. */ ;
}

std::string LinuxJoydev::getElementType()
{
   return "linux_joydev";
}

bool LinuxJoydev::config(jccl::ConfigElementPtr e)
{
   if(! (Input::config(e) && Digital::config(e) && Analog::config(e)))
   {
      return false;
   }

   // The joydev driver returns axis values in the range [-32767, 32767].
   // Hence, we ignore whatever the user provided through the config element.
   setMin(-32767.0f);
   setMax(32767.0f);

   mJsLabel = e->getName();
   mPortName = e->getProperty<std::string>("port_name");

   unsigned int num_axis_buttons = e->getNum("axis_buttons");
   for ( unsigned int i = 0; i < num_axis_buttons; ++i )
   {
      unsigned int idx = e->getProperty<unsigned int>("axis_buttons", i);
      mAxisButtonIndices.push_back(idx);
   }

   return true;
}

// Begins sampling.  Connects to the joystick and prepares to read.
bool LinuxJoydev::startSampling()
{
   vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_CONFIG_STATUS_LVL)
      << "Opening Linux Joystick driver on port: " << mPortName << std::endl
      << vprDEBUG_FLUSH;

   // Open the joystick non-blocking.
   mJsFD = open(mPortName.c_str(), O_RDONLY | O_NONBLOCK);

   if(mJsFD < 0)
   {
      vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_CRITICAL_LVL)
         << "ERROR: Failed to open Linux Joystick: " << mPortName << std::endl
         << vprDEBUG_FLUSH;
      return false;
   }

   // Read in the settings
   int version;
   unsigned char num_axes, num_buttons;
   char js_name[255];

   ioctl(mJsFD, JSIOCGVERSION, &version);

   const unsigned int version_major(version >> 16);
   const unsigned int version_minor((version >> 8) & 0xff);
   const unsigned int version_patch(version & 0xff);
   char version_str[256];
   snprintf(version_str, sizeof(version_str), "%d.%d.%d", version_major,
            version_minor, version_patch);

   // Verify that the version is new enough.
   if ( version_major < 1 )
   {
      vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_CRITICAL_LVL)
         << "ERROR: Linux Joystick API Version is too old.  The LinuxJoydev "
         << "driver requires version 1.0 or greater, but found version "
         << version_str << std::endl << vprDEBUG_FLUSH;
      return false;
   }

   ioctl(mJsFD, JSIOCGAXES, &num_axes);
   ioctl(mJsFD, JSIOCGBUTTONS, &num_buttons);

   mNumAxes = num_axes;
   mNumButtons = num_buttons;

   // Get joystick name
   if ( ioctl(mJsFD, JSIOCGNAME(sizeof(js_name)), js_name) < 0 )
   {
      mPhysicalJsName = std::string("Unknown");
   }
   else
   {
      mPhysicalJsName = js_name;
   }

   // Output joystick description
   vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_CONFIG_LVL)
         << "  Joystick Label: " << mJsLabel << std::endl
         << "   Joystick Name: " << mPhysicalJsName << std::endl
         << "            Axes: " << mNumAxes << std::endl
         << "         Buttons: " << mNumButtons << std::endl
         << "  Driver version: " << version_str << std::endl
         << "    Axis buttons: ";

   for ( unsigned int i = 0; i < mAxisButtonIndices.size(); ++i )
   {
      vprDEBUG_CONTnl(gadgetDBG_INPUT_MGR, vprDBG_CONFIG_LVL)
         << mAxisButtonIndices[i] << " ";
   }
   vprDEBUG_CONTnl(gadgetDBG_INPUT_MGR, vprDBG_CONFIG_LVL) << std::endl
                                                           << vprDEBUG_FLUSH;

   // Allocate initial device data
   // - By default this will clear them out
   mCurAxes.resize(mNumAxes);
   mCurAxesRanges.resize(mNumAxes, axis_range_t(getMin(), getMax()));
   // We use double the number of axis button indices because each axis has
   // to map to two buttons (high and low).
   mCurButtons.resize(mNumButtons + mAxisButtonIndices.size() * 2);

   // Setup axis as button stuff
   mAxisToButtonIndexLookup.clear();
   mAxisToButtonIndexLookup.resize(mNumAxes, -1);            // Default to -1, meaning no axis button
   for ( unsigned int i = 0; i < mAxisButtonIndices.size(); ++i )       // For each configured axis index
   {
      // Define the index of the virtual button from the axis. Each axis
      // provides two virtual buttons: one for its low value and one for its
      // high value. These virtual buttons are paired up in the digital
      // sample buffers as [low0, high0, low1, high1, ..., lowN, highN] for
      // axes 0 through N.
      const unsigned int virtual_btn_index = mNumButtons + i * 2;
      vprASSERT(virtual_btn_index < mCurButtons.size() &&
                "Virtual button index out of range");
      unsigned int axis_index = mAxisButtonIndices[i];                      // Index of the axis we are mapping
      mAxisToButtonIndexLookup[axis_index] = int(virtual_btn_index);    // Setup the mapping
   }

   return true;
}

// Stops sampling.  Drops the connection to joystick and clears everything.
bool LinuxJoydev::stopSampling()
{
   if(mJsFD > 0)
   {
      close(mJsFD);     // Close the joystick device
   }

   return true;
}


// Updates to the sampled data.
void LinuxJoydev::updateData()
{
   // -- Read in any new pending events
   js_event cur_event;

   // While events pending
   while (read(mJsFD, &cur_event, sizeof(struct js_event)) > 0)
   {
      if(cur_event.type & JS_EVENT_BUTTON)
      {
         //std::cout << "ljs: btn: " << unsigned(cur_event.number) << " val:"
         //          << cur_event.value << std::endl;
         const unsigned int btn_number(cur_event.number);
         vprASSERT(btn_number < mCurButtons.size() && "Button out of range");
         mCurButtons[btn_number] = cur_event.value;         // Assign the new button value (0,1)
         mCurButtons[btn_number].setTime();                 // Set timestamp to now
      }
      else if(cur_event.type & JS_EVENT_AXIS)
      {
         //std::cout << "ljs: axis: " << unsigned(cur_event.number) << " val:"
         //          << cur_event.value << std::endl;
         const unsigned int axis_number(cur_event.number);
         vprASSERT(axis_number < mCurAxes.size() && "Axis out of range");
         vprASSERT(axis_number < mCurAxesRanges.size() && "Axis out of range");

         float norm_value(0.0f);
         normalizeMinToMax(cur_event.value, norm_value);
         mCurAxes[axis_number] = norm_value;
         mCurAxes[axis_number].setTime();

         // Check for axis buttons. If we have a mapping for axis_number,
         // then we map the value of the analog axis to two buttons (high and
         // low). If the analog value is greater than 0.5, then we map the
         // high button to 1 and the low button to 0. If the analog value is
         // less than 0.5, then we map the high button to 0 and the low button
         // to 0. Otherwise, both buttons are 0.
         if ( mAxisToButtonIndexLookup[axis_number] != -1 )    // If we map to a virtual button
         {
            const unsigned int low_btn_index =
               mAxisToButtonIndexLookup[axis_number];
            const unsigned int high_btn_index = low_btn_index + 1;
            vprASSERT(high_btn_index < mCurButtons.size() &&
                      "Virtual high button index out of range");
            vprASSERT(low_btn_index < mCurButtons.size() &&
                      "Virtual low button index out of range");

            // Record the high button as pressed and the low button as not
            // pressed.
            if ( norm_value > 0.5f )
            {
               mCurButtons[low_btn_index]  = 0;
               mCurButtons[high_btn_index] = 1;
            }
            // Record the high button as not pressed and the low button as
            // pressed.
            else if ( norm_value < 0.5f )
            {
               mCurButtons[low_btn_index]  = 1;
               mCurButtons[high_btn_index] = 0;
            }
            // Record both buttons as not pressed.
            else
            {
               mCurButtons[low_btn_index]  = 0;
               mCurButtons[high_btn_index] = 0;
            }

            mCurButtons[low_btn_index].setTime();
            mCurButtons[high_btn_index].setTime();
         }
      }
   }

   // Check to make sure error was just no-pending events
   if(errno != EAGAIN)
   {
      vprDEBUG(vprDBG_ALL, vprDBG_CRITICAL_LVL)
         << "ERROR: Error reading linux joystick.\n" << vprDEBUG_FLUSH;
      return;
   }

   addDigitalSample(mCurButtons);
   swapDigitalBuffers();

   addAnalogSample(mCurAxes);
   swapAnalogBuffers();
}

} // End of gadget namespace
