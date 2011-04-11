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

#ifndef _GADGET_MULTI_EVENT_INTERFACE_H_
#define _GADGET_MULTI_EVENT_INTERFACE_H_

#include <gadget/gadgetConfig.h>

#include <algorithm>
#include <typeinfo>
#include <vector>
#include <boost/shared_ptr.hpp>
#include <boost/function.hpp>
#include <boost/fusion/include/map.hpp>
#include <boost/fusion/include/at_key.hpp>
#include <boost/fusion/include/pair.hpp>
#include <boost/fusion/include/as_map.hpp>
#include <boost/fusion/include/mpl.hpp>
#include <boost/mpl/transform.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/for_each.hpp>
#include <boost/bind.hpp>
#include <boost/bind/apply.hpp>
#include <boost/ref.hpp>

#include <vpr/Util/Debug.h>

#include <gadget/InputManager.h>
#include <gadget/Util/Debug.h>
#include <gadget/Type/ProxyPtr.h>
#include <gadget/Type/ProxyTraits.h>
#include <gadget/Event/AbstractEventInterface.h>
#include <gadget/Event/EventTags.h>
#include <gadget/Event/EventRegistration.h>


namespace gadget
{

/** \class MultiEventInterface MultiEventInterface.h gadget/Event/MultiEventInterface.h
 *
 * @tparam ProxyType     The type of proxy to be used by this EventInterface
 *                       type instantiation.
 * @tparam EventTags     A type sequence supporting forward iteration.
 * @tparam GeneratorType The event generator type. This is used to determine
 *                       how to register this interface with the central
 *                       input handler. This type must declare a type \c tag
 *                       that is one of gadget::event::immediate_tag (events
 *                       are generated as soon as the data added signal is
 *                       emitted by the device), gadget::event::periodic_tag
 *                       (events are generated once per iteration of the
 *                       input handler's thread), or
 *                       gadget::event::synchronized_tag (events are generated
 *                       once per iteration of the frame loop).
 *
 * @since 2.1.16
 */
template<typename ProxyType, typename EventTags, typename GeneratorType>
class MultiEventInterface
   : public AbstractEventInterface
   , protected RegistrationTypeChooser<GeneratorType>::type
{
public:
   /** @name Type Declarations */
   //@{
   typedef ProxyType                                    proxy_type;
   typedef EventTags                                    event_tags;
   typedef boost::shared_ptr<proxy_type>                proxy_ptr_type;
   typedef ProxyTraits<ProxyType>                       proxy_traits_type;
   typedef typename proxy_traits_type::device_type      device_type;
   typedef typename proxy_traits_type::device_data_type device_data_type;
   typedef typename GeneratorType::raw_data_type        raw_data_type;
   typedef GeneratorType                                generator_type;
   typedef boost::shared_ptr<generator_type>            generator_ptr_type;
   //@}

   typedef boost::function<void (const raw_data_type&)> callback_type;

protected:
   // For use by derived classes
   typedef
      MultiEventInterface<
           ProxyType
         , EventTags
         , GeneratorType
      >
   event_interface_;

public:
   MultiEventInterface()
   {
      /* Do nothing. */ ;
   }

   virtual ~MultiEventInterface()
   {
      if (mProxy.get() != NULL)
      {
         unregisterInterface(this);
      }
   }

   void init(const std::string& proxyName)
   {
      setProxyName(proxyName);
   }

   virtual bool isConnected() const
   {
      return NULL != mProxy.get();
   }

   virtual void refresh()
   {
      const std::string& proxy_name(getProxyName());

      if (! proxy_name.empty())
      {
         ProxyPtr proxy = InputManager::instance()->getProxy(proxy_name);

         if (mProxy != proxy)
         {
            proxy_ptr_type typed_proxy(
               boost::dynamic_pointer_cast<proxy_type>(proxy)
            );

            if (typed_proxy.get() == NULL)
            {
               vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_CONFIG_LVL)
                  << clrOutBOLD(clrYELLOW, "WARNING")
                  << ": Proxy '" << proxy_name << "' is not of type "
                  << typeid(proxy_type).name()
                  << ". This probably indicates an event interface/proxy "
                  << "configuration mismatch!\n" << vprDEBUG_FLUSH;
            }

            setProxy(typed_proxy);
         }
      }
   }

   virtual const EventGeneratorPtr& getEventGenerator() const
   {
      return mEventGenerator;
   }

   /** @name Callback Management */
   //@{
   template<typename EventTag>
   MultiEventInterface& addCallback(const EventTag&,
                                    const callback_type& callback)
   {
      return addCallback<EventTag>(callback);
   }

   template<typename EventTag>
   MultiEventInterface& addCallback(const callback_type& callback)
   {
      boost::fusion::at_key<EventTag>(mCallbackMap).push_back(callback);
      return *this;
   }
   //@}

protected:
   /**
    * Invokes the callbacks registered for the given event tag.
    *
    * @tparam EventTag The tag for the event that was generated.
    *
    * @param d The event data.
    */
   template<typename EventTag>
   void invoke(const raw_data_type& d) const
   {
      std::for_each(boost::fusion::at_key<EventTag>(mCallbackMap).begin(),
                    boost::fusion::at_key<EventTag>(mCallbackMap).end(),
                    boost::bind(boost::apply<void>(), _1, d));
   }

   struct CallbackRegistrar
   {
      CallbackRegistrar(MultiEventInterface* owner,
                        const generator_ptr_type& generator)
         : owner(owner)
         , generator(generator)
      {
         /* Do nothing. */ ;
      }

      template<typename U>
      void operator()(const U&) const
      {
         generator->template setCallback<U>(
            boost::bind(&MultiEventInterface::template invoke<U>, owner, _1)
         );
      }

      MultiEventInterface* owner;
      generator_ptr_type   generator;
   };

   void setProxy(const proxy_ptr_type& proxy)
   {
      // If mProxy is not NULL and is not the same as proxy, then that means
      // that this event interface is already registered and needs to be
      // removed from the input handler.
      if (mProxy.get() != NULL && mProxy != proxy)
      {
         unregisterInterface(this);
      }

      proxy_ptr_type old_proxy(mProxy);
      mProxy = proxy;

      // If we now have a non-NULL value for mProxy, then we need to create
      // our event generator and register with the input handler.
      if (mProxy.get() != NULL)
      {
         mEventGenerator = createEventGenerator(mProxy);
         registerInterface(this);
      }

      onProxyChanged(old_proxy, mProxy);
   }

   /**
    * Template method invoked when the proxy is changed. This is invoked
    * \em after the event generator for this event interface has been created.
    * In other words, if \p newProxy is not a NULL pointer, then this event
    * interface has a valid event generator. This default implementation does
    * nothing.
    *
    * @param oldProxy The device proxy in use prior to the change.
    * @param newProxy The new device proxy for this event interface.
    */
   virtual void onProxyChanged(const proxy_ptr_type& oldProxy,
                               const proxy_ptr_type& newProxy)
   {
      /* Do nothing. */ ;
   }

   const proxy_ptr_type& getProxy() const
   {
      return mProxy;
   }

   virtual EventGeneratorPtr createEventGenerator(const proxy_ptr_type& proxy)
   {
      generator_ptr_type generator(generator_type::create());
      generator->init(proxy);
      boost::mpl::for_each<EventTags>(CallbackRegistrar(this, generator));

      return generator;
   }

   /**
    * Invokes every registered callback using the given device data as the
    * callback argument.
    */
   void onDataAdded(const raw_data_type& data)
   {
      std::for_each(mCallbackMap.begin(), mCallbackMap.end(),
                    boost::bind(boost::apply<void>(), _1, boost::ref(data)));
   }

private:
   typedef typename
      boost::mpl::transform<
           EventTags
         , boost::fusion::pair<boost::mpl::_1, std::vector<callback_type> >
      >::type
   map_pairs_type;

   typedef typename
      boost::fusion::result_of::as_map<map_pairs_type>::type map_type;
   map_type mCallbackMap;

   proxy_ptr_type mProxy;
   EventGeneratorPtr mEventGenerator;
};

}


#endif /* _GADGET_MULTI_EVENT_INTERFACE_H_ */
