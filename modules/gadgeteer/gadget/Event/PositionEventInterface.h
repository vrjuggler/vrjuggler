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
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

#ifndef _GADGET_POSITION_EVENT_INTERFACE_H_
#define _GADGET_POSITION_EVENT_INTERFACE_H_

#include <gadget/gadgetConfig.h>

#include <boost/static_assert.hpp>
#include <boost/type_traits/is_same.hpp>

#include <gadget/Event/EventInterface.h>
#include <gadget/Event/BasicEventGenerator.h>
#include <gadget/Type/PositionProxy.h>


namespace gadget
{

/** \class PositionEventGenerator PositionEventInterface.h gadget/Event/PositionEventInterface.h
 *
 * @tparam CollectionTag A tag specifyiing which event(s) will be collected by
 *                       the event generator created by this object. This must
 *                       be a valid collection tag in order for the code to
 *                       compile.
 * @tparam GenerationTag A tag specifying how events will be emitted by the
 *                       event generator created by this object. This must be
 *                       a valid generation tag in order for the code to
 *                       compile.
 *
 * @since 2.1.4
 */
template<typename CollectionTag, typename GenerationTag>
class PositionEventGenerator
   : public BasicEventGenerator<PositionProxy, CollectionTag, GenerationTag>
{
public:
   typedef BasicEventGenerator<PositionProxy, CollectionTag, GenerationTag>
      base_type;

protected:
   PositionEventGenerator()
      : mScaleFactor(PositionUnitConversion::ConvertToFeet)
   {
      /* Do nothing. */ ;
   }

public:
   static boost::shared_ptr<PositionEventGenerator> create()
   {
      return boost::shared_ptr<PositionEventGenerator>(
                new PositionEventGenerator()
             );
   }

   virtual ~PositionEventGenerator()
   {
      /* Do nothing. */ ;
   }

   void setScaleFactor(const float scaleFactor)
   {
      mScaleFactor = scaleFactor;
   }

protected:
   BOOST_STATIC_ASSERT(
      (boost::is_same<typename base_type::raw_data_type, gmtl::Matrix44f>::value)
   );

   void onDataAdded(const gmtl::Matrix44f& data)
   {
      // Apply the proxy's position filters to the given data.
      PositionData pos_data(data);
      const PositionProxyPtr& proxy(this->getProxy());
      pos_data = proxy->applyFilters(pos_data);

      // Apply our scale factor to our data.
      gmtl::Matrix44f result;
      proxy->applyScaleFactor(pos_data.getValue(), mScaleFactor, result);

      // Store or emit the result.
      this->base_type::onDataAdded(result);
   }

private:
   float mScaleFactor;
};

/** \class PositionEventInterface PositionEventInterface.h gadget/Event/PositionEventInterface.h
 *
 * The event interface for gadget::PositionProxy objects.
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
 * @since 2.1.4
 */
template<typename CollectionTag = event::last_event_tag
       , typename GenerationTag = event::synchronized_tag>
class PositionEventInterface
   : public EventInterface<PositionProxy
                         , PositionEventGenerator<CollectionTag, GenerationTag>
                         >
{
public:
   typedef EventInterface<PositionProxy
                        , PositionEventGenerator<CollectionTag, GenerationTag>
                        >
      base_type;

   typedef typename base_type::proxy_ptr_type proxy_ptr_type;

   PositionEventInterface(
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

protected:
   EventGeneratorPtr createEventGenerator(const proxy_ptr_type& proxy)
   {
      typedef typename base_type::generator_type generator_type;

      EventGeneratorPtr base_generator(
         this->base_type::createEventGenerator(proxy)
      );

      // Downcast to our specific generator type so that we can se the scale
      // factor that the event generator will use.
      boost::shared_ptr<generator_type> generator(
         boost::dynamic_pointer_cast<generator_type>(base_generator)
      );
      generator->setScaleFactor(mScaleFactor);

      return base_generator;
   }

private:
   const float mScaleFactor;
};

}


#endif /* _GADGET_POSITION_EVENT_INTERFACE_H_ */
