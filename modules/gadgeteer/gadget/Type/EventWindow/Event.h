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


namespace gadget
{

/**
 * Base event type that an event source may generate.  This class cannot be
 * instantiated directly, and instead, subclasses must be used.
 */
class Event
{
public:
   /// Possible event types.
   enum Type
   {
      None       = 0,  /**< No event */
      KeyEvent   = 1,  /**< Keyboard event */
      MouseEvent = 2   /**< Mouse event */
   };

   /**
    * Returns the type of this event.  This can be used for dynamic casting
    * to more specific event types.
    */
   const Type& type() const
   {
      return mType;
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
   Event(const Type& type, const unsigned long& time)
      : mType(type), mTime(time)
   {
   }

   Type          mType; /**< The event type. */
   unsigned long mTime; /**< Time at which the event occurred. */
};

}


#endif /* _GADGET_EVENT_H_ */
