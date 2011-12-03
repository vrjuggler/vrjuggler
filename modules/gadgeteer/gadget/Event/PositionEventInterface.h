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

#ifndef _GADGET_POSITION_EVENT_INTERFACE_H_
#define _GADGET_POSITION_EVENT_INTERFACE_H_

#include <gadget/gadgetConfig.h>

#include <boost/static_assert.hpp>
#include <boost/type_traits/is_same.hpp>

#include <gadget/Event/MultiEventInterface.h>
#include <gadget/Event/MultiEventGenerator.h>
#include <gadget/Event/ProxySetter.h>
#include <gadget/Type/PositionProxy.h>


namespace gadget
{

namespace event
{

/** \struct position_event_tag PositionEventInterface.h gadget/Event/PositionEventInterface.h
 *
 * The event tag for position events emitted by instances of
 * gadget::PositionEventInterface.
 *
 * @since 2.1.16
 */
struct position_event_tag : base_event_tag {};

/**
 * A specialization of gadget::event::DataExaminer for use by
 * gadget::PositionEventInterface.
 *
 * @since 2.1.16
 */
template<>
class DataExaminer<position_event_tag, PositionProxy::raw_data_type>
   : public BaseExaminer<PositionProxy::raw_data_type>
{
public:
   typedef PositionProxy::raw_data_type data_type;

   BOOST_STATIC_ASSERT((boost::is_same<data_type, gmtl::Matrix44f>::value));

   DataExaminer()
      : mScaleFactor(PositionUnitConversion::ConvertToFeet)
   {
      /* Do nothing. */ ;
   }

   void examine(const data_type& data, bool& consumed)
   {
      // Apply the proxy's position filters to the given data.
      PositionData pos_data(data);
      pos_data = mProxy->applyFilters(pos_data);

      // Apply our scale factor to our data.
      gmtl::Matrix44f result;
      mProxy->applyScaleFactor(pos_data.getValue(), mScaleFactor, result);

      // Store or emit the result.
      this->addEvent(result, consumed);
   }

   void setScaleFactor(const float scaleFactor)
   {
      mScaleFactor = scaleFactor;
   }

   void setProxy(const PositionProxyPtr& proxy)
   {
      mProxy = proxy;
   }

private:
   float mScaleFactor;
   PositionProxyPtr mProxy;
};

}

/** \class PositionEventInterface PositionEventInterface.h gadget/Event/PositionEventInterface.h
 *
 * The multi-event interface for gadget::PositionProxy objects. While this is
 * a multi-event interface, there is only only event tag supported:
 * gadget::event::position_event_tag.
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
 * @note This was converted to a multi-event interface in 2.1.26.
 *
 * @since 2.1.4
 */
template<typename CollectionTag = event::last_event_tag
       , typename GenerationTag = event::synchronized_tag>
class PositionEventInterface
   : public MultiEventInterface<PositionProxy
                              , MultiEventGenerator<
                                     PositionProxy
                                   , boost::mpl::vector<
                                        event::position_event_tag
                                     >
                                   , CollectionTag
                                   , GenerationTag
                                >
                              >
{
public:
   typedef typename PositionEventInterface::event_interface_ base_type;
   typedef typename base_type::proxy_ptr_type proxy_ptr_type;

   /**
    * Constructor.
    *
    * @param scaleFactor The scale factor to apply to position data received
    *                    from the gadget::PositionProxy associated with this
    *                    multi-event interface object. This parameter is
    *                    optional, and it defaults to
    *                    gadge::PositionUnitConversion::ConvertToFeet.
    */
   explicit PositionEventInterface(
      const float scaleFactor = PositionUnitConversion::ConvertToFeet
   )
      : mScaleFactor(scaleFactor)
   {
      /* Do nothing. */ ;
   }

   virtual ~PositionEventInterface()
   {
      /* Do nothing. */ ;
   }

   using base_type::addCallback;

   /**
    * Adds a callback for the gadget::event::position_event_tag event tag.
    * This is for convenience since this multi-event interface supports
    * just one event tag.
    */
   void addCallback(const typename base_type::callback_type& callback)
   {
      base_type::template addCallback<event::position_event_tag>(callback);
   }

protected:
   typedef typename base_type::generator_type     generator_type;
   typedef typename base_type::generator_ptr_type generator_ptr_type;
   typedef typename base_type::event_tags         event_tags;

   EventGeneratorPtr createEventGenerator(const proxy_ptr_type& proxy)
   {
      EventGeneratorPtr base_generator(
         this->base_type::createEventGenerator(proxy)
      );

      // Downcast to our specific generator type so that we can se the scale
      // factor that the event generator will use.
      generator_ptr_type generator(
         boost::dynamic_pointer_cast<generator_type>(base_generator)
      );
      ScaleFactorSetter setter(generator, mScaleFactor);
      boost::mpl::for_each<event_tags>(setter);

      return base_generator;
   }

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

   struct ScaleFactorSetter
   {
      ScaleFactorSetter(const generator_ptr_type& generator,
                        const float scaleFactor)
         : generator(generator)
         , scaleFactor(scaleFactor)
      {
         /* Do nothing. */ ;
      }

      template<typename EventTag>
      void operator()(const EventTag&)
      {
         generator->template getExaminer<EventTag>().setScaleFactor(scaleFactor);
      }

      generator_ptr_type generator;
      const float        scaleFactor;
   };

private:
   const float mScaleFactor;
};

}


#endif /* _GADGET_POSITION_EVENT_INTERFACE_H_ */
