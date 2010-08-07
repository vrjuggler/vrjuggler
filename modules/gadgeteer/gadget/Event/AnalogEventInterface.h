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

#ifndef _GADGET_ANALOG_EVENT_INTERFACE_H_
#define _GADGET_ANALOG_EVENT_INTERFACE_H_

#include <gadget/gadgetConfig.h>

#include <gadget/Event/EventInterface.h>
#include <gadget/Event/BasicEventGenerator.h>
#include <gadget/Type/AnalogProxy.h>


namespace gadget
{

/** \class AnalogEventInterface AnalogEventInterface.h gadget/Event/AnalogEventInterface.h
 *
 * The event interface for gadget::AnalogProxy objects.
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
 * @since 2.1.7
 */
template<typename CollectionTag = event::last_event_tag
       , typename GenerationTag = event::synchronized_tag>
class AnalogEventInterface
   : public EventInterface<AnalogProxy
                         , BasicEventGenerator<AnalogProxy
                                             , CollectionTag
                                             , GenerationTag>
                         >
{
};

}


#endif /* _GADGET_ANALOG_EVENT_INTERFACE_H_ */
