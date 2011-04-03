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

#ifndef _GADGET_EVENT_INTERFACE_H_
#define _GADGET_EVENT_INTERFACE_H_

#include <gadget/gadgetConfig.h>

#include <algorithm>
#include <typeinfo>
#include <vector>
#include <boost/shared_ptr.hpp>
#include <boost/function.hpp>
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

/** \class EventInterface EventInterface.h gadget/Event/EventInterface.h
 *
 * @tparam ProxyType     The type of proxy to be used by this EventInterface
 *                       type instantiation.
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
 * @tparam DataType      The "raw" data type returned by the associated device
 *                       proxy type. For example, it is the return type of
 *                       gadget::TypedProxy<ProxyType>::getData(). This
 *                       template paramter is optional, and it defaults to
 *                       gadget::ProxyTraits<ProxyType>::raw_data_type.
 *
 * @since 2.1.2
 */
template<typename ProxyType, typename GeneratorType>
class EventInterface
   : public AbstractEventInterface
   , protected RegistrationTypeChooser<GeneratorType>::type
{
public:
   /** @name Type Declarations */
   //@{
   typedef ProxyType                                    proxy_type;
   typedef boost::shared_ptr<proxy_type>                proxy_ptr_type;
   typedef ProxyTraits<ProxyType>                       proxy_traits_type;
   typedef typename proxy_traits_type::device_type      device_type;
   typedef typename proxy_traits_type::device_data_type device_data_type;
   typedef typename GeneratorType::raw_data_type        raw_data_type;
   typedef GeneratorType                                generator_type;
   //@}

   typedef boost::function<void (const raw_data_type&)> callback_type;

public:
   EventInterface()
   {
      /* Do nothing. */ ;
   }

   virtual ~EventInterface()
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
   EventInterface& addCallback(const callback_type& callback)
   {
      mCallbacks.push_back(callback);
      return *this;
   }

   EventInterface& operator&&(const callback_type& callback)
   {
      return addCallback(callback);
   }
   //@}

protected:
   void setProxy(const proxy_ptr_type& proxy)
   {
      // If mProxy is not NULL and is not the same as proxy, then that means
      // that this event interface is already registered and needs to be
      // removed from the input handler.
      if (mProxy.get() != NULL && mProxy != proxy)
      {
         unregisterInterface(this);
      }

      mProxy = proxy;

      // If we now have a non-NULL value for mProxy, then we need to create
      // our event generator and register with the input handler.
      if (mProxy.get() != NULL)
      {
         mEventGenerator = createEventGenerator(mProxy);
         registerInterface(this);
      }
   }

   const proxy_ptr_type& getProxy() const
   {
      return mProxy;
   }

   virtual EventGeneratorPtr createEventGenerator(const proxy_ptr_type& proxy)
   {
      boost::shared_ptr<generator_type> generator(generator_type::create());
      generator->init(proxy);
      generator->setCallback(
         boost::bind(&EventInterface::onDataAdded, this, _1)
      );

      return generator;
   }

   /**
    * Invokes every registered callback using the given device data as the
    * callback argument.
    */
   void onDataAdded(const raw_data_type& data)
   {
      std::for_each(mCallbacks.begin(), mCallbacks.end(),
                    boost::bind(boost::apply<void>(), _1, boost::ref(data)));
   }

private:
   std::vector<callback_type> mCallbacks;

   proxy_ptr_type mProxy;
   EventGeneratorPtr mEventGenerator;
};

}


#endif /* _GADGET_EVENT_INTERFACE_H_ */
