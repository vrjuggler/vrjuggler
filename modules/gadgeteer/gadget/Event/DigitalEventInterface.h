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

#ifndef _GADGET_DIGITAL_EVENT_INTERFACE_H_
#define _GADGET_DIGITAL_EVENT_INTERFACE_H_

#include <gadget/gadgetConfig.h>

#include <gadget/Event/EventInterface.h>
#include <gadget/Event/BasicEventGenerator.h>
#include <gadget/Type/DigitalProxy.h>


namespace gadget
{

namespace event
{

/** \class DigitalSampleHandler DigitalEventInterface.h gadget/Event/DigitalEventInterface.h
 *
 * This is a sample handler for gadget::DigitalProxy objects that tracks the
 * state of a digital device unit in order that events can be emitted for
 * the toggling of a digital device's state.
 *
 * @since 2.1.13
 */
class GADGET_CLASS_API DigitalSampleHandler
{
public:
   typedef ProxyTraits<DigitalProxy>     device_type;
   typedef device_type::device_data_type device_data_type;
   typedef std::vector<device_data_type> sample_type;
   typedef device_type::raw_data_type    raw_data_type;

   DigitalSampleHandler();

   DigitalState::State getData(const sample_type& samples,
                               const unsigned int unit);

private:
   DigitalState::State mCurState;
};

}

/** \class DigitalEventInterface DigitalEventInterface.h gadget/Event/DigitalEventInterface.h
 *
 * The event interface for gadget::DigitalProxy objects.
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
 * @since 2.1.6
 */
template<typename CollectionTag = event::last_event_tag
       , typename GenerationTag = event::synchronized_tag>
class DigitalEventInterface
   : public EventInterface<DigitalProxy
                         , BasicEventGenerator<DigitalProxy
                                             , CollectionTag
                                             , GenerationTag
                                             , event::DigitalSampleHandler>
                         >
{
};

}


#endif /* _GADGET_DIGITAL_EVENT_INTERFACE_H_ */
