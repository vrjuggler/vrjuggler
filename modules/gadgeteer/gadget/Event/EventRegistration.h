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

#ifndef _GADGET_EVENT_REGISTRATION_H_
#define _GADGET_EVENT_REGISTRATION_H_

#include <gadget/gadgetConfig.h>

#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/identity.hpp>
#include <boost/type_traits/is_same.hpp>

#include <gadget/InputManager.h>
#include <gadget/EventEmitter.h>


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
      EventEmitterPtr handler(InputManager::instance()->getEventEmitter());
      handler->registerPeriodicInterface(iface);
   }

   void unregisterInterface(AbstractEventInterface* iface)
   {
      EventEmitterPtr handler(InputManager::instance()->getEventEmitter());
      handler->unregisterPeriodicInterface(iface);
   }
};

class SynchronizedRegistrator
{
protected:
   void registerInterface(AbstractEventInterface* iface)
   {
      EventEmitterPtr handler(InputManager::instance()->getEventEmitter());
      handler->registerSynchronizedInterface(iface);
   }

   void unregisterInterface(AbstractEventInterface* iface)
   {
      EventEmitterPtr handler(InputManager::instance()->getEventEmitter());
      handler->unregisterSynchronizedInterface(iface);
   }
};

}

/** \struct RegistrationTypeChooser RegistrationTypeChooser.h gadget/Event/EventRegistration.h
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

}


#endif /* _GADGET_EVENT_REGISTRATION_H_ */
