/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2009 by Iowa State University
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

#include <gadget/gadgetConfig.h>

#include <stdlib.h>

#include <jccl/Config/ConfigElement.h>

#include <gadget/Util/Debug.h>
#include <gadget/Devices/KeyboardMouseDevice/InputArea.h>


namespace gadget
{

InputArea::InputArea()
   : mAllowMouseLocking(true)
   , mLockState(Unlocked)
   , mLockStoredKey(-1)
   , mLockToggleKey(-1)
   , mSleepTimeMS(0)
   , mKeyboardMouseDevice(NULL)
   , mKeyboardMouseDeviceName("")
{
   /* Do nothing. */ ;
}

bool InputArea::config(jccl::ConfigElementPtr e)
{
   // Get the lock information
   mAllowMouseLocking = e->getProperty<bool>("allow_mouse_locking");
   mLockToggleKey = e->getProperty<int>("lock_key");
   bool start_locked = e->getProperty<bool>("start_locked");

   if (start_locked)
   {
      mLockState = Lock_LockKey;      // Initialize to the locked state
   }

   mSleepTimeMS = e->getProperty<int>("sleep_time");

   // Sanity check.
   if (mSleepTimeMS == 0)
   {
      mSleepTimeMS = 50;
   }

   // If we have a pointer to a window in registry, then use that
   mKeyboardMouseDeviceName = e->getProperty<std::string>("keyboard_mouse_device_name");

   km_registry_t::KeyboardMouseDeviceInfo event_source_info;
   bool found_window =
      km_registry_t::instance()->getKeyboardMouseDevice(mKeyboardMouseDeviceName,
                                                        event_source_info);

   if(!found_window)
   {
      // Use vprDBG_WARNING_LVL for input_window config elements as they are
      // almost certainly mis-configured without a valid value for
      // keyboard_mouse_device_name.
      int level = e->getID() == std::string("input_window") ? vprDBG_WARNING_LVL
                                                            : vprDBG_STATE_LVL;

      vprDEBUG(gadgetDBG_INPUT_MGR, level)
         << "WARNING: Could not find keyboard/mouse device named '"
         << mKeyboardMouseDeviceName << "'\n" << vprDEBUG_FLUSH;
      vprDEBUG_NEXT(gadgetDBG_INPUT_MGR, level)
         << "No InputArea will be created for window named '"
         << e->getName() << "'.\n" << vprDEBUG_FLUSH;
      return false;
   }
   else
   {
      mKeyboardMouseDeviceInfo = event_source_info;
      mKeyboardMouseDevice = mKeyboardMouseDeviceInfo.mKeyboardMouseDevice;
   }

   return true;
}

}
