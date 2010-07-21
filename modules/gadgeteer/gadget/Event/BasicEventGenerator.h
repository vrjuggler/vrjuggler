#ifndef _GADGET_BASIC_EVENT_GENERATOR_H_
#define _GADGET_BASIC_EVENT_GENERATOR_H_

#include <gadget/gadgetConfig.h>

#include <algorithm>
#include <vector>
#include <boost/function.hpp>
#include <boost/bind.hpp>
#include <boost/bind/apply.hpp>
#include <boost/signals/connection.hpp>
#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/identity.hpp>
#include <boost/type_traits/is_same.hpp>

#include <gadget/Type/ProxyTraits.h>
#include <gadget/Event/EventGenerator.h>
#include <gadget/Event/EventTags.h>


namespace gadget
{

namespace event
{

template<typename DataType>
class EventCollector
{
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

template<typename CollectorTag, typename DataType>
struct CollectionTypeChooser
{
   typedef typename boost::mpl::eval_if<
         boost::is_same<CollectorTag, event::all_events_tag>,
         boost::mpl::identity<event::AllEventsCollector<DataType> >,
         boost::mpl::identity<event::LastEventCollector<DataType> >
      >::type type;
};

/** \class BasicEventGenerator BasicEventGenerator.h gadget/Event/BasicEventGenerator.h
 *
 * @since 2.1.2
 */
template<typename ProxyType
       , typename CollectorTag
       , typename GenerationTag
       , typename DataType = typename ProxyTraits<ProxyType>::raw_data_type>
class BasicEventGenerator
   : public EventGenerator
   , protected CollectionTypeChooser<CollectorTag, DataType>::type
{
public:
   /** @name Type Declarations */
   //@{
   typedef GenerationTag                              tag;
   typedef ProxyType                                  proxy_type;
   typedef ProxyTraits<ProxyType>                     proxy_traits_type;
   typedef typename proxy_traits_type::proxy_ptr_type proxy_ptr_type;
   typedef typename proxy_traits_type::device_type    device_type;
   typedef boost::shared_ptr<device_type>             device_ptr_type;
   typedef DataType                                   raw_data_type;

   typedef boost::function<void (const raw_data_type&)> callback_type;
   //@}

protected:
   BasicEventGenerator()
   {
      /* Do nothing. */ ;
   }

public:
   static boost::shared_ptr<BasicEventGenerator> create()
   {
      return boost::shared_ptr<BasicEventGenerator>(new BasicEventGenerator());
   }

   virtual ~BasicEventGenerator()
   {
      mDevConn.disconnect();
   }

   boost::shared_ptr<BasicEventGenerator> init(const proxy_ptr_type& proxy)
   {
      mProxy = proxy;

      device_ptr_type device(mProxy->getTypedInputDevice());

      // This is not connected using a shared pointer to this object in order
      // to prevent a circlar reference between this object and the proxied
      // device.
      mDevConn =
         device->eventAdded().connect(
            boost::bind(&BasicEventGenerator::onEventAdded, this, _1)
         );

      EventGeneratorPtr self(shared_from_this());
      return boost::dynamic_pointer_cast<BasicEventGenerator>(self);
   }

   /** #name gadget::EventGenerator Interface Implementation */
   //@{
   void emitEvents()
   {
      if (! mCallback.empty())
      {
         // Invoke the callback for every pending event.
         std::for_each(this->mPendingEvents.begin(),
                       this->mPendingEvents.end(),
                       boost::bind(boost::apply<void>(), mCallback, _1));
      }

      // We are done with the pending events.
      this->mPendingEvents.clear();
   }
   //@}

   void setCallback(const callback_type& callback)
   {
      mCallback = callback;
   }

protected:
   static const bool sEmitsImmediately =
      boost::is_same<GenerationTag, event::immediate_tag>::value;

   virtual void onEventAdded(const raw_data_type& data)
   {
      if (sEmitsImmediately)
      {
         if (! mCallback.empty())
         {
            mCallback(data);
         }
      }
      else
      {
         addEvent(data);
      }
   }

private:
   proxy_ptr_type mProxy;
   boost::signals::connection mDevConn;

   callback_type mCallback;
};

}


#endif /* _GADGET_BASIC_EVENT_GENERATOR_H_ */
