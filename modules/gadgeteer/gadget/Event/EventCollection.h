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

#ifndef _GADGET_EVENT_COLLECTION_H_
#define _GADGET_EVENT_COLLECTION_H_

#include <gadget/gadgetConfig.h>

#include <vector>
#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/identity.hpp>
#include <boost/type_traits/is_same.hpp>


namespace gadget
{

namespace event
{

template<typename DataType>
class EventCollector
{
public:
   /**
    * @since 2.1.16
    */
   std::vector<DataType>& getEvents()
   {
      return mPendingEvents;
   }

   /**
    * @since 2.1.16
    */
   const std::vector<DataType>& getEvents() const
   {
      return mPendingEvents;
   }

protected:
   EventCollector()
   {
      /* Do nothing. */ ;
   }

   std::vector<DataType> mPendingEvents;
};

template<typename DataType>
class AllEventsCollector : public EventCollector<DataType>
{
protected:
   AllEventsCollector()
   {
      /* Do nothing. */ ;
   }

   void addEvent(const DataType& eventData)
   {
      this->mPendingEvents.push_back(eventData);
   }
};

template<typename DataType>
class LastEventCollector : public EventCollector<DataType>
{
protected:
   LastEventCollector()
   {
      /* Do nothing. */ ;
   }

   void addEvent(const DataType& eventData)
   {
      this->mPendingEvents.resize(1);
      this->mPendingEvents[0] = eventData;
   }
};

}

template<typename CollectionTag, typename DataType>
struct CollectionTypeChooser
{
   typedef typename boost::mpl::eval_if<
         boost::is_same<CollectionTag, event::all_events_tag>,
         boost::mpl::identity<event::AllEventsCollector<DataType> >,
         boost::mpl::identity<event::LastEventCollector<DataType> >
      >::type type;
};

}


#endif /* _GADGET_EVENT_COLLECTION_H_ */
