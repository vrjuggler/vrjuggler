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

#ifndef _GADGET_EVENT_H_
#define _GADGET_EVENT_H_

#include <gadget/gadgetConfig.h>
#include <vpr/IO/SerializableObject.h>

#include <gadget/Type/KeyboardMouse/EventPtr.h>

namespace gadget
{

/** Possible event types. */
enum EventType
{
   NoEvent                 = 0,  /**< No event */
   KeyPressEvent           = 1,  /**< Key press event */
   KeyReleaseEvent         = 2,  /**< Key release event */
   MouseButtonPressEvent   = 3,  /**< Mouse button press event */
   MouseButtonReleaseEvent = 4,  /**< Mouse button release event */
   MouseMoveEvent          = 5   /**< Mouse move event */
};

/** \class Event Event.h gadget/Type/KeyboardMouse/Event.h
 *
 * Base event type that an event source may generate.  This class cannot be
 * instantiated directly, and instead, subclasses must be used.
 */
class GADGET_CLASS_API Event : public vpr::SerializableObject
{
public:
   /**
    * Returns the type of this event.  This can be used for dynamic casting
    * to more specific event types.
    */
   const EventType& type() const
   {
      return mType;
   }

   /**
    * Sets the type of this event.  This is needed because, while using an
    * object reader to de-serialize an Event object, we cannot set the type 
    * during construction. We must set the event type after creating
    * this event using the EventFactory. This could later be removed if
    * the EventFactory is chaged to take care of this.
    */
   void setType(const EventType& type)
   {
      mType = type;
   }

   /**
    * Returns the time at which the event occurred.
    */
   const unsigned long& time() const
   {
      return mTime;
   }

   virtual ~Event()
   {
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
   /**
    * Initializes data members.
    *
    * @param type The type of this event from the Event::Type enumeration.
    * @param time The time at which this event occurred.  This should be as
    *             accurate as possible,  preferabbly acquired from the
    *             operating system or windowing system event data structure.
    *             The time at which the event was processed is not an
    *             acceptable value.
    */
   Event(const EventType& type, const unsigned long& time)
      : mType(type), mTime(time)
   {
   }

   EventType     mType; /**< The event type. */
   unsigned long mTime; /**< Time at which the event occurred. */
};

}


#endif /* _GADGET_EVENT_H_ */
