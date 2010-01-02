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

#ifndef GADGET_INPUT_AREA_H
#define GADGET_INPUT_AREA_H

#include <gadget/gadgetConfig.h>

#include <string>

#include <jccl/Config/ConfigElementPtr.h>
#include <gadget/Devices/KeyboardMouseDevice/KeyboardMouseDevice.h>


namespace gadget
{

/** \class InputArea InputArea.h gadget/Devices/KeyboardMouseDevice/InputArea.h
 *
 * Base class for window types that accept keyboard and mouse input.
 */
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

   InputArea();

   virtual ~InputArea()
   {;}
   
   bool config(jccl::ConfigElementPtr e);

   /**
    * Locks the mouse to the center of this input area.
    *
    * @see unlockMouse()
    *
    * @since 1.3.25
    */
   virtual void lockMouse() = 0;

   /**
    * Unlocks the mouse so that it can move again.
    *
    * @since 1.3.25
    */
   virtual void unlockMouse() = 0;

protected:
   /** Shortened form of the keyboard/mouse device registry type name. */
   typedef KeyboardMouseDevice::KeyboardMouseDeviceRegistry km_registry_t;

   bool      mAllowMouseLocking; /**< Flag allowing mouse locking. */
   lockState mLockState;         /**< The current state of locking. */
   int       mLockStoredKey;     /**< The key that was pressed down. */
   int       mLockToggleKey;     /**< The key that toggles the locking. */
   int       mSleepTimeMS;
   
   KeyboardMouseDevice* mKeyboardMouseDevice;

   /** Name of the remote display window (index in registry). */
   std::string mKeyboardMouseDeviceName;

   /** Info structure for remote window. */
   km_registry_t::KeyboardMouseDeviceInfo mKeyboardMouseDeviceInfo;
};

} // end namespace gadget

#endif /*GADGET_INPUT_AREA_H*/
