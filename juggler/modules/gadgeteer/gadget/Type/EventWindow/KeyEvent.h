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
#include <gadget/Type/EventWindow/EventFactory.h>

#include <vpr/IO/SerializableObject.h>


namespace gadget
{

GADGET_REGISTER_EVENT_CREATOR( KeyEvent, KeyPressEvent );
GADGET_REGISTER_EVENT_CREATOR( KeyEvent, KeyReleaseEvent );

/**
 * Key press or release event class.
 */
class KeyEvent : public gadget::Event
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
    * @param asciiKey   The ASCII value of the key associated with this event.
    *                   This parameter is optional, and it defaults to 0 if not
    *                   specified.
    *
    * @see gadget::ModifierMask
    */
   KeyEvent(const gadget::EventType& type, const gadget::Keys& key,
            const int& mask, const unsigned long& time, char asciiKey = 0)
      : gadget::Event(type, time), mKey(key), mModifierMask(mask),
        mAsciiKey(asciiKey)
   {
   }

   /**
    * Default constructor needed in order to use the templated EventFactory 
    * without modification. The EventFactory was needed to allow the 
    * correct subtype of Event, KeyEvent in this case, to be created 
    * during de-serialization.
    */
   KeyEvent() 
      : gadget::Event(NoEvent, 0)
   {
      mKey           = gadget::KEY_SPACE;
      mModifierMask  = 0;
      mAsciiKey      = ' ';
   }

   /**
    * Get the key that was pressed while generating this event.
    */
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
   const char& getKeyChar() const
   {
      return mAsciiKey;
   }

   /**
    * Serializes this event using the given ObjectWriter.
    */
   vpr::ReturnStatus writeObject(vpr::ObjectWriter* writer)
   {
      writer->writeUint16(mType);

      // Serialize all member variables
      writer->writeUint32(mKey);
      writer->writeUint32(mModifierMask);
      writer->writeUint64(mTime);
      writer->writeUint8(mAsciiKey);
      
      return vpr::ReturnStatus::Succeed;
   }
   
   /**
    * De-serializes this event using the given ObjectReader.
    */
   vpr::ReturnStatus readObject(vpr::ObjectReader* reader)
   {
      // We have already read the type in EventWindow to decide
      // if we should construct a KeyEvent or a MouseEvent
      //mType = reader->readUint16();

      // De-Serialize all member variables
      mKey = (gadget::Keys)reader->readUint32();
      mModifierMask = reader->readUint32();
      mTime = reader->readUint64();
      mAsciiKey = reader->readUint8();
      return vpr::ReturnStatus::Succeed;
   }

protected:
   gadget::Keys mKey;          /**< The actual key pressed. */
   int          mModifierMask; /**< The mask of any modifiers in addition to mKey. */
   char         mAsciiKey;     /**< ASCCI representation of key pressed. */
};

} // End of gadget namespace


#endif /* _GADGET_KEY_EVENT_H_ */
