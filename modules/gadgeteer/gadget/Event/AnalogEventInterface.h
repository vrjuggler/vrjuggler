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

#ifndef _GADGET_ANALOG_EVENT_INTERFACE_H_
#define _GADGET_ANALOG_EVENT_INTERFACE_H_

#include <gadget/gadgetConfig.h>

#include <gadget/Event/MultiEventInterface.h>
#include <gadget/Event/MultiEventGenerator.h>
#include <gadget/Event/ProxySetter.h>
#include <gadget/Type/AnalogProxy.h>


namespace gadget
{

namespace event
{

/** \struct normalized_analog_event_tag AnalogEventInterface.h gadget/Event/AnalogEventInterface.h
 *
 * The event tag for analog events delivering normalized data as emitted by
 * instances of gadget::AnalogEventInterface.
 *
 * @since 2.1.21
 */
struct normalized_analog_event_tag : base_event_tag {};

/** \struct raw_analog_event_tag AnalogEventInterface.h gadget/Event/AnalogEventInterface.h
 *
 * The event tag for analog events delivering raw (non-normalized) data as
 * emitted by instances of gadget::AnalogEventInterface.
 *
 * @since 2.1.21
 */
struct raw_analog_event_tag : base_event_tag {};

/**
 * A specialization of gadget::event::DataExaminer for use by
 * gadget::AnalogEventInterface. This class is used to add analog events with
 * normalized device data.
 *
 * @since 2.1.21
 */
template<>
class DataExaminer<normalized_analog_event_tag, AnalogProxy::raw_data_type>
   : public BaseExaminer<AnalogProxy::raw_data_type>
{
public:
   typedef AnalogProxy::raw_data_type data_type;

   void examine(const data_type data, bool& consumed)
   {
      this->addEvent(mProxy->normalizeData(data), consumed);
   }

   void setProxy(const AnalogProxyPtr& proxy)
   {
      mProxy = proxy;
   }

private:
   AnalogProxyPtr mProxy;
};

/**
 * A specialization of gadget::event::DataExaminer for use by
 * gadget::AnalogEventInterface. This class is used to add analog events with
 * raw (non-normalized) device data.
 *
 * @since 2.1.21
 */
template<>
class DataExaminer<raw_analog_event_tag, AnalogProxy::raw_data_type>
   : public BaseExaminer<AnalogProxy::raw_data_type>
{
public:
   typedef AnalogProxy::raw_data_type data_type;

   void examine(const data_type data, bool& consumed)
   {
      this->addEvent(data, consumed);
   }

   void setProxy(const AnalogProxyPtr&)
   {
      /* Do nothing. */ ;
   }
};

}

/** \class AnalogEventInterface AnalogEventInterface.h gadget/Event/AnalogEventInterface.h
 *
 * The event interface for gadget::AnalogProxy objects. This is a multi-event
 * interface where callbacks can be registered to receive normalized analog
 * data and raw analog data. The callbacks are added as follows:
 *
 * \code
 * void normalizedDataCallback(const float data)
 * {
 *    // Handle normalized analog device data...
 * }
 *
 * void rawDataCallback(const float data)
 * {
 *    // Handle raw (non-normalized) analog device data...
 * }
 *
 * void MyApp::init()
 * {
 *    mAnalogIface.init("VJAnalog0");
 *
 *    using namespace gadget::event;
 *    mAnalogIface.addCallback<normalized_analog_event_tag>(
 *       normalizedDataCallback
 *    );
 *    mAnalogIface.addCallback<raw_analog_event_tag>(rawDataCallback);
 * }
 * \endcode
 *
 * @tparam CollectionTag A tag specifyiing which event(s) will be collected by
 *                       the event generator created by this object. This must
 *                       be a valid collection tag in order for the code to
 *                       compile. This template paramter is optional, and it
 *                       defaults to gadget::event::last_event_tag.
 * @tparam GenerationTag A tag specifying how events will be emitted by the
 *                       event generator created by this object. This must be
 *                       a valid generation tag in order for the code to
 *                       compile. This template paramter is optional, and it
 *                       defaults to gadget::event::synchronized_tag.
 *
 * @note This class was rewritten in 2.1.21 to be a multi-event generator.
 *       The interface is compatible with the version added in 2.1.7, although
 *       the events that are generated now deliver normalized data by default
 *       instead of raw data.
 *
 * @since 2.1.7
 */
template<typename CollectionTag = event::last_event_tag
       , typename GenerationTag = event::synchronized_tag>
class AnalogEventInterface
   : public MultiEventInterface<AnalogProxy
                              , MultiEventGenerator<
                                     AnalogProxy
                                   , boost::mpl::vector<
                                          event::normalized_analog_event_tag
                                        , event::raw_analog_event_tag
                                     >
                                   , CollectionTag
                                   , GenerationTag
                                >
                              >
{
public:
   typedef typename AnalogEventInterface::event_interface_ base_type;
   typedef typename base_type::proxy_ptr_type proxy_ptr_type;

   virtual ~AnalogEventInterface()
   {
      /* Do nothing. */ ;
   }

   using base_type::addCallback;

   /**
    * Adds a callback for the gadget::event::normalized_analog_event_tag event
    * tag. This is mainly for convenience as most user-level code will want
    * to get normalized analog data.
    */
   void addCallback(const typename base_type::callback_type& callback)
   {
      base_type::template addCallback<event::normalized_analog_event_tag>(callback);
   }

protected:
   typedef typename base_type::generator_type     generator_type;
   typedef typename base_type::generator_ptr_type generator_ptr_type;
   typedef typename base_type::event_tags         event_tags;

   void onProxyChanged(const proxy_ptr_type&, const proxy_ptr_type& newProxy)
   {
      // If newProxy is NULL, then we don't have an event generator with any
      // data examiners to update.
      if (newProxy)
      {
         generator_ptr_type generator(
            boost::dynamic_pointer_cast<generator_type>(
               this->getEventGenerator()
            )
         );
         typedef
            event::detail::ProxySetter<generator_ptr_type, proxy_ptr_type>
         proxy_setter_type;
         proxy_setter_type setter(generator, newProxy);
         boost::mpl::for_each<event_tags>(setter);
      }
   }
};

}


#endif /* _GADGET_ANALOG_EVENT_INTERFACE_H_ */
