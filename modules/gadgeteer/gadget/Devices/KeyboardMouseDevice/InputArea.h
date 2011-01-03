/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2011 by Iowa State University
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
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

#ifndef GADGET_INPUT_AREA_H
#define GADGET_INPUT_AREA_H

#include <gadget/gadgetConfig.h>

#include <string>
#include <boost/noncopyable.hpp>

#include <jccl/Config/ConfigElementPtr.h>
#include <gadget/Devices/KeyboardMouseDevice/KeyboardMouseDevice.h>


namespace gadget
{

/** \class InputArea InputArea.h gadget/Devices/KeyboardMouseDevice/InputArea.h
 *
 * Base class for window types that accept keyboard and mouse input.
 */
class GADGET_CLASS_API InputArea : private boost::noncopyable
{
public:
   /** Enum to keep track of current lock state for state machine. */
   enum lockState
   {
      Unlocked,     /**< The mouse is free. */
      Lock_LockKey, /**< The mouse is locked due to lock toggle key press. */
      Lock_KeyDown, /**< The mouse is locked due to a key being held down. */
      Lock_API      /**< The mouse is locked due to an API call. */
   };

protected:
   InputArea();

public:
   virtual ~InputArea();

public:
   /** @name Mouse Pointer Access */
   //@{
   /**
    * Locks the mouse to the center of this input area.
    *
    * @post \c mLockState is \c Lock_API
    *
    * @see unlockMouse()
    *
    * @since 1.3.25
    */
   void lockMouse();

   /**
    * Unlocks the mouse so that it can move again.
    *
    * @post \c mLockState is \c Unlocked
    *
    * @since 1.3.25
    */
   void unlockMouse();
   //@}

protected:
   bool config(jccl::ConfigElementPtr e);

   /** @name Mouse Pointer Locking Template Methods */
   //@{
   /**
    * The template method that performs the real mouse locking specific to
    * the host window system.
    *
    * @since 1.3.27
    */
   virtual void lockMouseInternal() = 0;

   /**
    * The template method that performs the real mouse unlocking specific to
    * the host window system.
    *
    * @since 1.3.27
    */
   virtual void unlockMouseInternal() = 0;
   //@}

   /** Shortened form of the keyboard/mouse device registry type name. */
   typedef KeyboardMouseDevice::KeyboardMouseDeviceRegistry km_registry_t;

   bool      mAllowMouseLocking; /**< Flag allowing mouse locking. */
   lockState mLockState;         /**< The current state of locking. */
   int       mLockStoredKey;     /**< The key that was pressed down. */
   int       mLockToggleKey;     /**< The key that toggles the locking. */
   int       mSleepTimeMS;

   unsigned int mLockXCenter;   /**< The mouse X location used for locking. */
   unsigned int mLockYCenter;   /**< The mouse Y location used for locking. */
   
   KeyboardMouseDevice* mKeyboardMouseDevice;

   /** Name of the remote display window (index in registry). */
   std::string mKeyboardMouseDeviceName;

   /** Info structure for remote window. */
   km_registry_t::KeyboardMouseDeviceInfo mKeyboardMouseDeviceInfo;
};

} // end namespace gadget

#endif /*GADGET_INPUT_AREA_H*/
