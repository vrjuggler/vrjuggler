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

#ifndef _GADGET_KEYBOARD_MOUSE_PROXY_H_
#define _GADGET_KEYBOARD_MOUSE_PROXY_H_

#include <gadget/gadgetConfig.h>
#include <gadget/Type/Proxy.h>
#include <gadget/Type/KeyboardMouse.h>
//#include <gadget/Devices/KeyboardMouseDevice/KeyboardMouseDevice.h>
#include <gadget/Event/Event.h>
#include <gadget/Type/KeyboardMouseProxyPtr.h>

namespace gadget
{

/** \class KeyboardMouseProxy KeyboardMouseProxy.h gadget/Type/KeyboardMouseProxy.h
 *
 * A proxy class to keyboard/mouse devices, used by the Input Manager.
 *
 * A keyboard/mouse proxy always points to a keyboard/mouse device and unit
 * number within that device.  The Input Manager can therefore keep an array
 * of these around and treat them as digital devices that only return a single
 * sub-device's amount of data (an event queue and individual keys).
 *
 * @see gagdet::KeyboardMouse
 */
class GADGET_CLASS_API KeyboardMouseProxy : public TypedProxy<KeyboardMouse>
{
public:
   /** @since 2.1.1 */
   typedef TypedProxy<KeyboardMouse> base_type;

protected:
   KeyboardMouseProxy();

public:
   /**
    * Creates a KeyboardMouseProxy instance and returns it wrapped in a
    * KeyboardMouseProxyPtr object.
    *
    * @since 1.3.7
    */
   static KeyboardMouseProxyPtr create();

   virtual ~KeyboardMouseProxy();

   /** Returns the time of the last update. */
   const vpr::Interval& getTimeStamp() const;

   /**
    * Determines if the given modifier key is the only modifier pressed.
    *
    * @pre The given key identifier is one of gadget::NONE, gadget::KEY_ALT,
    *      gadget::KEY_CTRL, or gadget::KEY_SHIFT.
    *
    * @param modKey A gadget::Keys value that must be one of gadget::NONE,
    *               gadget::KEY_ALT, gadget::KEY_CTRL, or gadget::KEY_SHIFT.
    *
    * @return true if the given modifier key is the only modifier key pressed.
    */
   bool modifierOnly(gadget::Keys modKey) const
   {
      if ( isStupefied() || (NULL == mTypedDevice.get()) )
      {
         return false;
      }
      else
      {
         return mTypedDevice->modifierOnly(modKey);
      }
   }

   /**
    * Returns the number of times the given key was pressed during the last
    * frame.  The value returned can be used in a conditional expression to
    * determine if the key was pressed at all.
    *
    * @param keyId The identifier for the key whose state will be queried.
    *
    * @return The number of times the key was pressed since last update.
    */
   int keyPressed(gadget::Keys keyId) const
   {
      if ( isStupefied() || (NULL == mTypedDevice.get()) )
      {
         return 0;
      }
      else
      {
         return mTypedDevice->keyPressed(keyId);
      }
   }

   /**
    * Returns a copy of the current queue of events for the proxied device.
    *
    * @note As of Gadgeteer 2.1.1, this is an alias for getData().
    */
   const KeyboardMouse::EventQueue getEventQueue() const
   {
      return getData();
   }

   static std::string getElementType();

   bool config(jccl::ConfigElementPtr element);
};


} // End of gadget namespace

#endif
