/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2007 by Iowa State University
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

#ifndef _GADGET_KEY_EVENT_H_
#define _GADGET_KEY_EVENT_H_

#include <gadget/gadgetConfig.h>

#include <gadget/Type/KeyboardMouse/Event.h>
#include <gadget/Type/KeyboardMouse/KeyEventPtr.h>
#include <gadget/Type/KeyboardMouse/Keys.h>
#include <gadget/Type/KeyboardMouse/EventFactory.h>


namespace gadget
{

GADGET_REGISTER_EVENT_CREATOR( KeyEvent, KeyPressEvent );
GADGET_REGISTER_EVENT_CREATOR( KeyEvent, KeyReleaseEvent );

/** \class KeyEvent KeyEvent.h gadget/Type/KeyboardMouse/KeyEvent.h
 *
 * Key press or release event class.
 */
class GADGET_CLASS_API KeyEvent : public gadget::Event
{
public:
   /**
    * Initializes data members.
    *
    * @param type       The type of this event (either key press or key
    *                   release).
    * @param key        The platform-independent value of the key that was
    *                   pressed or released.
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
    * @param unicodeKey The Unicode value of the key associated with this
    *                   event. This parameter is optional, and it defaults to
    *                   0 if not specified. This parameter was added in version
    *                   1.1.24.
    *
    * @see gadget::ModifierMask
    */
   KeyEvent(const gadget::EventType type, const gadget::Keys key,
            int mask, unsigned long time, char asciiKey = 0,
            wchar_t unicodeKey = 0);

   /**
    * Default constructor needed in order to use the templated EventFactory 
    * without modification.  The EventFactory was needed to allow the 
    * correct subtype of Event (KeyEvent in this case) to be created 
    * during de-serialization.
    */
   KeyEvent();

   virtual ~KeyEvent()
   {
      /* Do nothing. */ ;
   }

   /**
    * Get the key that was pressed while generating this event.
    */
   const gadget::Keys getKey() const
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
   const int getModifierMask() const
   {
      return mModifierMask;
   }

   /**
    * Returns the ASCII character corresponding to the key that was pressed.
    */
   const char getKeyChar() const
   {
      return mAsciiKey;
   }

   /**
    * Returns the Unicode character corresponding to the key that was pressed.
    *
    * @since 1.1.24
    */
   wchar_t getKeyUnicode() const
   {
      return mUnicodeKey;
   }

   /**
    * Serializes this object into the given object writer.
    *
    * @param writer The object writer to which this object will be serialized.
    *
    * @throw vpr::IOException is thrown if serialization fails.
    */
   virtual void writeObject(vpr::ObjectWriter* writer);

   /**
    * De-serializes this object.
    *
    * @param reader The object reader from which this object will be
    *               de-serialized.
    *
    * @throw vpr::IOException is thrown if de-serialization fails.
    */
   virtual void readObject(vpr::ObjectReader* reader);

protected:
   gadget::Keys mKey;          /**< The actual key pressed. */
   int          mModifierMask; /**< The mask of any modifiers in addition to mKey. */
   char         mAsciiKey;     /**< ASCII representation of key pressed. */
   wchar_t      mUnicodeKey;   /**< Unicode represencation of key pressed. */
};

} // End of gadget namespace


#endif /* _GADGET_KEY_EVENT_H_ */
