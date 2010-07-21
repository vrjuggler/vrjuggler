/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2010 by Iowa State University
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
#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/identity.hpp>
#include <boost/type_traits/is_same.hpp>

#include <vpr/Util/Debug.h>

#include <gadget/InputManager.h>
#include <gadget/InputHandler.h>
#include <gadget/Util/Debug.h>
#include <gadget/Type/ProxyPtr.h>
#include <gadget/Type/ProxyTraits.h>
#include <gadget/Event/AbstractEventInterface.h>
#include <gadget/Event/EventTags.h>


namespace gadget
{

namespace event
{

struct null_data_type {};

class ImmediateRegistrator
{
protected:
   void registerInterface(AbstractEventInterface*)
   {
   }

   void unregisterInterface(AbstractEventInterface*)
   {
   }
};

class PeriodicRegistrator
{
protected:
   void registerInterface(AbstractEventInterface* iface)
   {
      InputHandlerPtr handler(InputManager::instance()->getInputHandler());
      handler->registerPeriodicInterface(iface);
   }

   void unregisterInterface(AbstractEventInterface* iface)
   {
      InputHandlerPtr handler(InputManager::instance()->getInputHandler());
      handler->unregisterPeriodicInterface(iface);
   }
};

class SynchronizedRegistrator
{
protected:
   void registerInterface(AbstractEventInterface* iface)
   {
      InputHandlerPtr handler(InputManager::instance()->getInputHandler());
      handler->registerSynchronizedInterface(iface);
   }

   void unregisterInterface(AbstractEventInterface* iface)
   {
      InputHandlerPtr handler(InputManager::instance()->getInputHandler());
      handler->unregisterSynchronizedInterface(iface);
   }
};

}

/** \struct GenerationTypeChooser GenerationTypeChooser.h gadget/Event/GenerationTypeChooser.h
 *
 * A metafunction for choosing the base generation type for an
 * instantiation of gadget::EventInterface<P,G,D>.
 *
 * @since 2.1.2
 */
template<typename GeneratorType>
struct RegistrationTypeChooser
{
   typedef typename GeneratorType::tag generator_tag;
   typedef typename boost::mpl::eval_if<
      boost::is_same<generator_tag, event::immediate_tag>,
         boost::mpl::identity<event::ImmediateRegistrator>,
         boost::mpl::eval_if<
            boost::is_same<generator_tag, event::periodic_tag>,
            boost::mpl::identity<event::PeriodicRegistrator>,
            boost::mpl::identity<event::SynchronizedRegistrator>
         >
      >::type type;
};

/** \class EventInterface EventInterface.h gadget/Event/EventInterface.h
 *
 * @since 2.1.2
 */
template<typename ProxyType
       , typename GeneratorType
       , typename DataType = typename ProxyTraits<ProxyType>::raw_data_type>
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
   typedef DataType                                     raw_data_type;
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

   EventInterface& addCallback(const callback_type& callback)
   {
      mCallbacks.push_back(callback);
      return *this;
   }

   EventInterface& operator&&(const callback_type& callback)
   {
      return addCallback(callback);
   }

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
         boost::bind(&EventInterface::onEventAdded, this, _1)
      );

      return generator;
   }

   /**
    * Invokes every registered callback using the given device data as the
    * callback argument.
    */
   void onEventAdded(const raw_data_type& data)
   {
      std::for_each(mCallbacks.begin(), mCallbacks.end(),
                    boost::bind(boost::apply<void>(), _1, data));
   }

private:
   std::vector<callback_type> mCallbacks;

   proxy_ptr_type mProxy;
   EventGeneratorPtr mEventGenerator;
};

}


#endif /* _GADGET_EVENT_INTERFACE_H_ */
