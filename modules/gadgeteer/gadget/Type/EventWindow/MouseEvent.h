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

#ifndef _GADGET_MOUSE_EVENT_H_
#define _GADGET_MOUSE_EVENT_H_

#include <gadget/gadgetConfig.h>

#include <gadget/Type/EventWindow/Event.h>
#include <gadget/Type/EventWindow/MouseEventPtr.h>
#include <gadget/Type/EventWindow/Keys.h>
#include <gadget/Type/EventWindow/EventFactory.h>

#include <vpr/IO/SerializableObject.h>


namespace gadget
{
GADGET_REGISTER_EVENT_CREATOR( MouseEvent, MouseButtonPressEvent );
GADGET_REGISTER_EVENT_CREATOR( MouseEvent, MouseButtonReleaseEvent );
GADGET_REGISTER_EVENT_CREATOR( MouseEvent, MouseMoveEvent );

/**
 * Mouse event class.  This captures the state of the mouse when any mouse
 * event occurs.  This includes button presses, button releases, and mouse
 * motion.
 */
class MouseEvent : public gadget::Event
{
public:
   /**
    * Initializes data members.
    *
    * @param button        The button that generated the event.
    * @param x             The X coordinate of the mouse within the window.
    * @param y             The Y coordinate of the mouse within the window.
    * @param globalX       The X coordiante of the mouse relative to the root
    *                      window (i.e., the desktop).
    * @param globalY       The Y coordiante of the mouse relative to the root
    *                      window (i.e., the desktop).
    * @param state         The mask of mouse buttons and any modifiers being
    *                      pressed.  This should be constructed using the
    *                      bitwise OR of gadget::ModifierMask and
    *                      gadget::ButtonMask values.
    * @param time          The time at which this event occurred.  This should
    *                      be as accurate as possible,  preferabbly acquired
    *                      from the operating system or windowing system event
    *                      data structure.  The time at which the event was
    *                      processed is not an acceptable value.
    *
    * @see gadget::ModifierMask, gadget::ButtonMask
    */
   MouseEvent(const gadget::EventType& type, const gadget::Keys& button,
              const int& x, const int& y, const int& globalX,
              const int& globalY, const int& state, const unsigned long& time)
      : gadget::Event(type, time)
      , mButton(button)
      , mRelativeX(x)
      , mRelativeY(y)
      , mGlobalX(globalX)
      , mGlobalY(globalY)
      , mState(state)
   {
   }
   
   /**
    * Default constructor needed in order to use the templated EventFactory 
    * without modification. The EventFactory was needed to allow the 
    * correct subtype of Event, MouseEvent in this case, to be created 
    * during de-serialization.
    */
   MouseEvent()
      : gadget::Event(NoEvent, 0),
      mButton(gadget::NO_MBUTTON), 
      mRelativeX(0), 
      mRelativeY(0), 
      mGlobalX(0), 
      mGlobalY(0), 
      mState(0)
   {
   }

   /**
    * Get the mouse button that was pressed while generating this event.
    */
   const gadget::Keys& getButton() const
   {
      return mButton;
   }

   /**
    * Returns the X corrdinate of the mouse pointer relative to the window.
    */
   const int& getX() const
   {
      return mRelativeX;
   }

   /**
    * Returns the Y corrdinate of the mouse pointer relative to the window.
    */
   const int& getY() const
   {
      return mRelativeY;
   }

   /**
    * Returns the X corrdinate of the mouse pointer relative to the root
    * window (i.e., the desktop).
    */
   const int& getGlobalX() const
   {
      return mGlobalX;
   }

   /**
    * Returns the Y corrdinate of the mouse pointer relative to the root
    * window (i.e., the desktop).
    */
   const int& getGlobalY() const
   {
      return mGlobalY;
   }

   /**
    * Returns the state of the mouse buttons and keyboard modifier keys
    * (CTRL, ALT, and SHIFT).
    *
    * @see gadget::ModifierMask, gadget::ButtonMask
    */
   const int& getState() const
   {
      return mState;
   }

   /**
    * Serializes this event using the given ObjectWriter.
    */
   vpr::ReturnStatus writeObject(vpr::ObjectWriter* writer)
   {
      writer->writeUint16(mType);

      // Serialize all member variables
      writer->writeUint32(mButton);
      writer->writeUint32(mRelativeX);
      writer->writeUint32(mRelativeY);
      writer->writeUint32(mGlobalX);
      writer->writeUint32(mGlobalY);
      writer->writeUint32(mState);
      
      return vpr::ReturnStatus::Succeed;
   }
   
   /**
    * De-serializes this event using the given ObjectReader.
    */
   vpr::ReturnStatus readObject(vpr::ObjectReader* reader)
   {
      // We have already read the type in EventWindoe to decide
      // if we should construct a KeyEvent or a MouseEvent
      //mType = reader->readUint16();

      // De-Serialize all member variables
      mButton = (gadget::Keys)reader->readUint32();
      mRelativeX = reader->readUint32();
      mRelativeY = reader->readUint32();
      mGlobalX = reader->readUint32();
      mGlobalY = reader->readUint32();
      mState = reader->readUint32();
      return vpr::ReturnStatus::Succeed;
   }

private:
   gadget::Keys mButton;        /**< The button associated with this event. */
   int          mRelativeX;     /**< Relative X position. */
   int          mRelativeY;     /**< Relative Y position. */
   int          mGlobalX;       /**< Global X position. */
   int          mGlobalY;       /**< Global Y position. */
   int          mState;         /**< Mouse button and modifier key state. */
};

} // End of gadget namespace


#endif /* _GADGET_MOUSE_EVENT_H_ */
