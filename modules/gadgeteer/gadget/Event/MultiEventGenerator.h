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

#ifndef _GADGET_MULTI_EVENT_GENERATOR_H_
#define _GADGET_MULTI_EVENT_GENERATOR_H_

#include <gadget/gadgetConfig.h>

#include <algorithm>
#include <vector>
#include <boost/function.hpp>
#include <boost/fusion/include/map.hpp>
#include <boost/fusion/include/at_key.hpp>
#include <boost/fusion/include/pair.hpp>
#include <boost/fusion/include/as_map.hpp>
#include <boost/fusion/include/mpl.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/mpl/transform.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/for_each.hpp>
#include <boost/bind.hpp>
#include <boost/bind/apply.hpp>
#include <boost/signals/connection.hpp>

#include <gadget/Type/ProxyTraits.h>
#include <gadget/Event/EventGenerator.h>
#include <gadget/Event/EventCollection.h>
#include <gadget/Event/SampleHandler.h>
#include <gadget/Event/DataExaminer.h>
#include <gadget/Event/EventTags.h>


namespace gadget
{

namespace event
{

/**
 * @since 2.1.16
 */
template<typename CollectionTag, typename DataType>
class MultiEventCollector
   : public CollectionTypeChooser<CollectionTag, DataType>::type
{
public:
   typedef typename
      CollectionTypeChooser<CollectionTag, DataType>::type
   base_type;

   // Make addEvent() public.
   using base_type::addEvent;

   /**
    * @since 2.1.16
    */
   std::vector<DataType>& getEvents()
   {
      return this->mPendingEvents;
   }
};

}

/** \class MultiEventGenerator MultiEventGenerator.h gadget/Event/MultiEventGenerator.h
 *
 * In this design, the data examiner has the role of determining whether a
 * sample (or group of samples) represents an event. Event collection is done
 * thorugh instantiations of gadget::event::MultiEventCollector managed
 * through tag-specific associations.
 *
 * @tparam ProxyType     The type of proxy to be used by this multi-event
 *                       generator type instantiation.
 * @tparam EventTags     The tags for the events that may be generated. Event
 *                       tags are specified using a type sequence supporting
 *                       forward iteration. Generally, this would be an
 *                       instantiation of \c boost::mpl::vector containing
 *                       all the event tags.
 * @tparam CollectionTag A tag specifyiing which event(s) will be collected by
 *                       this event generator. This must be a valid collection
 *                       tag in order for the code to compile.
 * @tparam GenerationTag A tag specifying how events will be emitted by this
 *                       event generator. This must be a valid generation tag
 *                       in order for the code to compile.
 * @tparam SampleHandler The handler for samples. This type is used to
 *                       decouple the specifics of the data type used by
 *                       \c ProxyType from this multi-event generator type
 *                       instantiation. This template parameter is optional,
 *                       and it defaults to DefaultSampleHandler<ProxyType>.
 *
 * @see gadget::event::DataExaminer
 *
 * @since 2.1.16
 */
template<
     typename ProxyType
   , typename EventTags
   , typename CollectionTag
   , typename GenerationTag
   , typename SampleHandler = DefaultSampleHandler<ProxyType>
>
class MultiEventGenerator
   : public EventGenerator
{
public:
   /** @name Type Declarations */
   //@{
   typedef GenerationTag                                tag;
   typedef ProxyType                                    proxy_type;
   typedef EventTags                                    event_tags;
   typedef ProxyTraits<ProxyType>                       proxy_traits_type;
   typedef typename proxy_traits_type::proxy_ptr_type   proxy_ptr_type;
   typedef typename proxy_traits_type::device_type      device_type;
   typedef typename proxy_traits_type::device_ptr_type  device_ptr_type;
   typedef typename SampleHandler::sample_type          sample_type;
   typedef typename SampleHandler::raw_data_type        raw_data_type;

   typedef boost::function<void (const raw_data_type&)> callback_type;

   /**
    * Produces an MPL sequence type containing an instantiation of
    * boost::fusion::pair<K,V> for each \c K in event_tags where \c V is
    * gadget::event::DataExaminer<K,raw_data_type>.
    */
   typedef typename
      boost::mpl::transform<
           event_tags
         , boost::fusion::pair<
                boost::mpl::_1
              , event::DataExaminer<
                     boost::mpl::_1
                   , raw_data_type
                >
           >
      >::type
   examiner_pairs_type;

   /**
    * Produces a boost::fusion::map instantiation using the
    * boost::fusion::pair instantiations from examiner_pairs_type.
    */
   typedef typename
      boost::fusion::result_of::as_map<examiner_pairs_type>::type
   examiner_map_type;
   //@}

protected:
   MultiEventGenerator()
   {
      mCallbackInvoker.owner = this;
   }

public:
   static boost::shared_ptr<MultiEventGenerator> create()
   {
      return boost::shared_ptr<MultiEventGenerator>(new MultiEventGenerator());
   }

   virtual ~MultiEventGenerator()
   {
      mDevConn.disconnect();
      mRefreshConn.disconnect();
   }

   boost::shared_ptr<MultiEventGenerator> init(const proxy_ptr_type& proxy)
   {
      mProxy = proxy;

      device_ptr_type device(mProxy->getTypedInputDevice());

      // If the proxy is not currently stupefied, then we can connect to its
      // input device.
      if (device.get() != NULL)
      {
         onProxyRefreshed(device);
      }

      // This is not connected using a shared pointer to this object in order
      // to prevent a circlar reference between this object and the proxy.
      mRefreshConn =
         mProxy->proxyRefreshed().connect(
            boost::bind(&MultiEventGenerator::onProxyRefreshed, this, _1)
         );

      EventRegistrator reg(this);
      boost::mpl::for_each<EventTags>(reg);

      EventGeneratorPtr self(shared_from_this());
      return boost::dynamic_pointer_cast<MultiEventGenerator>(self);
   }

   /** @name gadget::EventGenerator Interface Implementation */
   //@{
   void emitEvents()
   {
      // Invoke the callback for every pending event.
      boost::mpl::for_each<event_tags>(mCallbackInvoker);
   }
   //@}

   /**
    * Defines the callback for the identified event tag.
    */
   template<typename EventTag>
   void setCallback(const callback_type& callback)
   {
      boost::fusion::at_key<EventTag>(mCallbackMap) = callback;
   }

   /** @name Data Examiner Access */
   //@{
   template<typename EventTag>
   typename
   boost::fusion::result_of::at_key<examiner_map_type, EventTag>::type
   getExaminer()
   {
      return boost::fusion::at_key<EventTag>(mExaminers);
   }

   template<typename EventTag>
   typename
   boost::fusion::result_of::at_key<const examiner_map_type, EventTag>::type
   getExaminer() const
   {
      return boost::fusion::at_key<EventTag>(mExaminers);
   }

   examiner_map_type& getExaminers()
   {
      return mExaminers;
   }

   const examiner_map_type& getExaminers() const
   {
      return mExaminers;
   }
   //@}

private:
   static const bool sEmitsImmediately =
      boost::is_same<GenerationTag, event::immediate_tag>::value;

   /**
    * Slot connected to the dataAdded signal on the device proxy used by this
    * event generator. When the device adds a sample, this method is invoked.
    */
   void onSamplesAdded(const sample_type& sample)
   {
      InvokeExaminer invoker(
         *this, mSampleHandler.getData(sample, mProxy->getUnit())
      );
      boost::mpl::for_each<EventTags>(invoker);
   }

   /**
    * Callback invoked by the gadget::DataExaminer<EventTag,raw_data_type>
    * object when it determines that an event has been added.
    */
   template<typename EventTag>
   void onEventAdded(const raw_data_type& eventData)
   {
      using namespace boost::fusion;

      // If this generator emits events immediately, then the callback
      // registered for EventTag is invoked.
      if (sEmitsImmediately)
      {
         const callback_type& callback(at_key<EventTag>(mCallbackMap));

         if (! callback.empty())
         {
            callback(eventData);
         }
      }
      // Otherwise, the event is added to the event collector for EventTag for
      // emission when emitEvents() is invoked.
      else
      {
         at_key<EventTag>(mCollectors).addEvent(eventData);
      }
   }

   const proxy_ptr_type& getProxy() const
   {
      return mProxy;
   }

   /**
    * @since 2.1.17
    */
   void onProxyRefreshed(device_ptr_type newDev)
   {
      mDevConn.disconnect();

      if (newDev.get() != NULL)
      {
         // This is not connected using a shared pointer to this object in
         // order to prevent a circlar reference between this object and the
         // proxied device.
         mDevConn =
            newDev->dataAdded().connect(
               boost::bind(&MultiEventGenerator::onSamplesAdded, this, _1)
            );
      }
   }

private:
   /** @name boost::mpl::for_each Functors */
   //@{
   struct EventRegistrator
   {
      EventRegistrator(MultiEventGenerator* owner)
         : owner(owner)
      {
         /* Do nothing. */ ;
      }

      template<typename EventTag>
      void operator()(const EventTag&) const
      {
         using namespace boost::fusion;

         // NOTE: This binder is created using a raw MultiEventGenerator
         // pointer. This is to prevent a circular reference between the data
         // examiners and the event generator that owns them.
         at_key<EventTag>(owner->mExaminers).setEventCallback(
            boost::bind(&MultiEventGenerator::template onEventAdded<EventTag>,
                        owner, _1)
         );
      }

      MultiEventGenerator* owner;
   };

   /**
    * The critical thing that this type provides is a means to get a sample
    * from a device to the data examiners without copying it. For some device
    * types (e.g., gadget::Position), a copy would be particularly expensive.
    */
   struct InvokeExaminer
   {
      InvokeExaminer(MultiEventGenerator& owner, const raw_data_type& value)
         : owner(owner)
         , value(&value)
      {
         /* Do nothing. */ ;
      }

      template<typename EventTag>
      void operator()(const EventTag&) const
      {
         boost::fusion::at_key<EventTag>(owner.mExaminers).examine(*value);
      }

      MultiEventGenerator& owner;
      const raw_data_type* const value;
   };

   struct CallbackInvoker
   {
      CallbackInvoker()
         : owner(NULL)
      {
         /* Do nothing. */ ;
      }

      /**
       * @post The pending event vector for the event collector associated
       *       with event tag EventTag is empty.
       */
      template<typename EventTag>
      void operator()(const EventTag&) const
      {
         using namespace boost::fusion;

         std::vector<raw_data_type>& events(
            at_key<EventTag>(owner->mCollectors).getEvents()
         );
         const callback_type& callback(at_key<EventTag>(owner->mCallbackMap));

         if (! callback.empty())
         {
            std::for_each(events.begin(), events.end(),
                          boost::bind(boost::apply<void>(), callback, _1));
         }

         events.clear();
      }

      MultiEventGenerator* owner;
   };
   //@}

   /** @name Data Examiners */
   //@{
   examiner_map_type mExaminers;
   //@}

   /** @name Event Collection */
   //@{
   typedef typename
      boost::mpl::transform<
           EventTags
         , boost::fusion::pair<
                boost::mpl::_1
              , event::MultiEventCollector<CollectionTag, raw_data_type>
           >
      >::type
   collection_pairs_type;

   /**
    * Produces a boost::fusion::map instantiation using the
    * boost::fusion::pair instantiations from collection_pairs_type.
    */
   typedef typename
      boost::fusion::result_of::as_map<collection_pairs_type>::type
   collection_map_type;

   /**
    * The event generator needs to be able to store events in the case of
    * delayed event emission.
    */
   collection_map_type mCollectors;
   //@}

   /** @name Event Interface Callbacks */
   //@{
   typedef typename
      boost::mpl::transform<
           EventTags
         , boost::fusion::pair<boost::mpl::_1, callback_type>
      >::type
   callback_pairs_type;

   /**
    * Produces a boost::fusion::map instantiation using the
    * boost::fusion::pair instantiations from callback_pairs_type.
    */
   typedef typename
      boost::fusion::result_of::as_map<callback_pairs_type>::type
   callback_map_type;

   callback_map_type mCallbackMap;
   //@}

   CallbackInvoker mCallbackInvoker;

   SampleHandler mSampleHandler;
   proxy_ptr_type mProxy;
   boost::signals::connection mDevConn;
   boost::signals::connection mRefreshConn;
};

}


#endif /* _GADGET_MULTI_EVENT_GENERATOR_H_ */
