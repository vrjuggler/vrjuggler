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

#ifndef _GADGET_MOUSE_EVENT_H_
#define _GADGET_MOUSE_EVENT_H_

#include <gadget/gadgetConfig.h>

#include <gadget/Event/Event.h>
#include <gadget/Event/KeyboardMouse/MouseEventPtr.h>
#include <gadget/Type/KeyboardMouse/Keys.h>
#include <gadget/Type/KeyboardMouse/EventFactory.h>


namespace gadget
{
GADGET_REGISTER_EVENT_CREATOR( MouseEvent, MouseButtonPressEvent );
GADGET_REGISTER_EVENT_CREATOR( MouseEvent, MouseButtonReleaseEvent );
GADGET_REGISTER_EVENT_CREATOR( MouseEvent, MouseMoveEvent );
GADGET_REGISTER_EVENT_CREATOR( MouseEvent, MouseScrollEvent );

/** \class MouseEvent MouseEvent.h gadget/Type/KeyboardMouse/MouseEvent.h
 *
 * Mouse event class.  This captures the state of the mouse when any mouse
 * event occurs.  This includes button presses, button releases, and mouse
 * motion.
 */
class GADGET_API MouseEvent
   : public gadget::Event
{
public:
   /**
    * Initializes data members.
    *
    * @param type          The type of this event (a mouse button press, a
    *                      mouse button release, or a mouse move).
    * @param button        The button that generated the event.
    * @param x             The X coordinate of the mouse within the window.
    * @param y             The Y coordinate of the mouse within the window.
    * @param globalX       The X coordiante of the mouse relative to the root
    *                      window (i.e., the desktop).
    * @param globalY       The Y coordiante of the mouse relative to the root
    *                      window (i.e., the desktop).
    * @param scrollDeltaX  The scroll delta in the X axis (left/right). A
    *                      negative value indicates scrolling to the left, and
    *                      a positive value indicates scrolling to the right.
    * @param scrollDeltaY  The scroll delta in the Y axis (up/down). A
    *                      negative value indicates scrolling down, and a
    *                      positive value indicates scrolling up.
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
   MouseEvent(const gadget::EventType type, const gadget::Keys button,
              const int x, const int y, const int globalX, const int globalY,
              const float scrollDeltaX, const float scrollDeltaY,
              const int state, const unsigned long time, InputArea* source);

   /**
    * Default constructor needed in order to use the templated EventFactory 
    * without modification. The EventFactory was needed to allow the 
    * correct subtype of Event, MouseEvent in this case, to be created 
    * during de-serialization.
    */
   MouseEvent();

   ~MouseEvent()
   {
      /* Do nothing. */ ;
   }

   /**
    * Get the mouse button that was pressed while generating this event.
    */
   gadget::Keys getButton() const
   {
      return mButton;
   }

   /** @name Mouse Position Information */
   //@{
   /**
    * Returns the X coordinate of the mouse pointer relative to the window.
    */
   int getX() const
   {
      return mRelativeX;
   }

   /**
    * Returns the Y coordinate of the mouse pointer relative to the window.
    */
   int getY() const
   {
      return mRelativeY;
   }

   /**
    * Returns the X coordinate of the mouse pointer relative to the root
    * window (i.e., the desktop).
    */
   int getGlobalX() const
   {
      return mGlobalX;
   }

   /**
    * Returns the Y coordinate of the mouse pointer relative to the root
    * window (i.e., the desktop).
    */
   int getGlobalY() const
   {
      return mGlobalY;
   }
   //@}

   /** @name Scroll Deltas */
   //@{
   /**
    * Returns the scroll delta in the X axis (right/left scrolling). A
    * positive value indicates scrolling right, and a negative value indicates
    * scrolling left. This value is only valid if type() returns
    * gadget::MouseScrollEvent.
    *
    * @since 1.3.22
    */
   float getScrollDeltaX() const
   {
      return mScrollDeltaX;
   }

   /**
    * Returns the scroll delta in the Y axis (up/down scrolling). A positive
    * value indicates scrolling up, and a negative value indicates scrolling
    * down. This value is only valid if type() returns
    * gadget::MouseScrollEvent.
    *
    * @since 1.3.22
    */
   float getScrollDeltaY() const
   {
      return mScrollDeltaY;
   }
   //@}

   /**
    * Returns the state of the mouse buttons and keyboard modifier keys
    * (CTRL, ALT, and SHIFT).
    *
    * @see gadget::ModifierMask, gadget::ButtonMask
    */
   int getState() const
   {
      return mState;
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

private:
   gadget::Keys mButton;        /**< The button associated with this event. */
   int          mRelativeX;     /**< Relative X position. */
   int          mRelativeY;     /**< Relative Y position. */
   int          mGlobalX;       /**< Global X position. */
   int          mGlobalY;       /**< Global Y position. */
   float        mScrollDeltaX;  /**< Scroll delta on X axis. */
   float        mScrollDeltaY;  /**< Scroll delta on Y axis. */
   int          mState;         /**< Mouse button and modifier key state. */
};

} // End of gadget namespace


#endif /* _GADGET_MOUSE_EVENT_H_ */
