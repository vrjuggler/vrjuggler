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

#ifndef GADGET_INPUT_AREA_H
#define GADGET_INPUT_AREA_H

#include <gadget/gadgetConfig.h>

#include <vpr/Sync/Mutex.h>

#include <gadget/Type/KeyboardMouse/KeyEvent.h>
#include <gadget/Type/KeyboardMouse/MouseEvent.h>
#include <gadget/Type/KeyboardMouse/Keys.h>
#include <gadget/Type/KeyboardMouse/Event.h>
#include <gadget/Devices/KeyboardMouseDevice/KeyboardMouseDevice.h>
#include <gadget/Util/Debug.h>

#include <jccl/Config/ConfigElement.h>
#include <jccl/Config/ConfigElementPtr.h>

namespace gadget
{
class GADGET_CLASS_API InputArea
{
public:
   /** Enum to keep track of current lock state for state machine. */
   enum lockState
   {
      Unlocked,     /**< The mouse is free. */
      Lock_LockKey, /**< The mouse is locked due to lock toggle key press. */
      Lock_KeyDown  /**< The mouse is locked due to a key being held down. */
   };

   InputArea()
      : mLockState(Unlocked), mLockStoredKey(-1), mLockToggleKey(-1),
        mMouseSensitivity(1.0f), mSleepTimeMS(0),
        mKeyboardMouseDevice(NULL), mKeyboardMouseDeviceName("")
   {;}
   ~InputArea()
   {;}
   
   bool config(jccl::ConfigElementPtr e)
   {
      // Get the lock information
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

      KeyboardMouseDevice::KeyboardMouseDeviceRegistry::KeyboardMouseDeviceInfo event_source_info;
      bool found_window = KeyboardMouseDevice::KeyboardMouseDeviceRegistry::instance()->getKeyboardMouseDevice(mKeyboardMouseDeviceName, event_source_info);
      if(!found_window)
      {
         vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_WARNING_LVL)
            << "WARNING: Could not find KeyboardMouse named: '"
            << mKeyboardMouseDeviceName << "'. Failed to create InputWindowXWin.\n"
            << vprDEBUG_FLUSH;
         return false;
      }
      else
      {
         mKeyboardMouseDeviceInfo = event_source_info;
         mKeyboardMouseDevice = mKeyboardMouseDeviceInfo.mKeyboardMouseDevice;
      }

      return true;
   }

protected:
   lockState    mLockState;       /**< The current state of locking. */
   int          mLockStoredKey;   /**< The key that was pressed down. */
   int          mLockToggleKey;   /**< The key that toggles the locking. */
   float        mMouseSensitivity;
   int          mSleepTimeMS;
   
   KeyboardMouseDevice* mKeyboardMouseDevice;
   
   std::string                mKeyboardMouseDeviceName; /**< Name of the remote display window (index in registry). */
   KeyboardMouseDevice::KeyboardMouseDeviceRegistry::KeyboardMouseDeviceInfo mKeyboardMouseDeviceInfo;     /**< Info structure for remote window. */
};

} // end namespace gadget

#endif /*GADGET_INPUT_AREA_H*/
