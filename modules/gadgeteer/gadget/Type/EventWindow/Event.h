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

#ifndef _GADGET_EVENT_H_
#define _GADGET_EVENT_H_

#include <vpr/IO/SerializableObject.h>

namespace gadget
{
/// Possible event types.
enum EventType
{
   NoEvent                 = 0,  /**< No event */
   KeyPressEvent           = 1,  /**< Key press event */
   KeyReleaseEvent         = 2,  /**< Key release event */
   MouseButtonPressEvent   = 3,  /**< Mouse button press event */
   MouseButtonReleaseEvent = 4,  /**< Mouse button release event */
   MouseMoveEvent          = 5   /**< Mouse move event */
};

/**
 * Base event type that an event source may generate.  This class cannot be
 * instantiated directly, and instead, subclasses must be used.
 */
class Event : public vpr::SerializableObject
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
    * Set the type of this event. This is needed because while using an
    * Object Reader to de-serialize an Event we can not set the type 
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

   vpr::ReturnStatus writeObject(vpr::ObjectWriter* writer)
   {
      writer->writeUint8(mType);
      writer->writeUint64(mTime);
      return vpr::ReturnStatus::Succeed;
   }

   vpr::ReturnStatus readObject(vpr::ObjectReader* reader)
   {
      mType = (EventType)reader->readUint8();
      mTime = reader->readUint64();
      return vpr::ReturnStatus::Succeed;
   }

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
