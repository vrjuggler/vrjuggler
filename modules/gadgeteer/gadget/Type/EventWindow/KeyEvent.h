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

#ifndef _GADGET_KEY_EVENT_H_
#define _GADGET_KEY_EVENT_H_

#include <gadget/gadgetConfig.h>

#include <gadget/Type/EventWindow/Event.h>
#include <gadget/Type/EventWindow/KeyEventPtr.h>
#include <gadget/Type/EventWindow/Keys.h>


namespace gadget
{

/**
 * Key press or release event class.
 */
class GADGET_CLASS_API KeyEvent : public gadget::Event
{
public:
   /**
    * Initializes data members.
    *
    * @param key        The platform-independent value of the key that was
    *                   pressed or released.
    * @param isKeyPress Flag indicating whether this is a key press event
    *                   (true) or a key release event (false).
    * @param mask       The mask of modifiers pressed in addition to key.
    *                   This should be constructed using the bitwise OR of
    *                   gadget::ModifierMask values.
    * @param time       The time at which this event occurred.  This should be
    *                   as accurate as possible,  preferabbly acquired from the
    *                   operating system or windowing system event data
    *                   structure.  The time at which the event was processed
    *                   is not an acceptable value.
    *
    * @see gadget::ModifierMask
    */
   KeyEvent(const gadget::EventType& type, const gadget::Keys& key,
            const int& mask, const unsigned long& time)
      : gadget::Event(type, time), mKey(key), mModifierMask(mask)
   {
   }

   const gadget::Keys& getKey() const
   {
      return mKey;
   }

   /**
    * Returns the modifier mask for this event.  This tells which modifier
    * keys (if any) were pressed at the same time as the key that was pressed
    * or released.  The mask is the bitwise OR of values in the enumeration
    * gadget::ModifierMask.
    *
    * @see gadget::ModifierMask
    */
   const int& getModifierMask() const
   {
      return mModifierMask;
   }

   /**
    * Returns the ASCII character corresponding to the key that was pressed.
    */
   char getKeyChar() const;

protected:
   gadget::Keys mKey;          /**< The actual key pressed. */
   int          mModifierMask; /**< The mask of any modifiers in addition to mKey. */
};

} // End of gadget namespace


#endif /* _GADGET_KEY_EVENT_H_ */
