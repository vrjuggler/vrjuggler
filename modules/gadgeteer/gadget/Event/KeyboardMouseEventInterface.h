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

#ifndef _GADGET_KEYBOARD_MOUSE_EVENT_INTERFACE_H_
#define _GADGET_KEYBOARD_MOUSE_EVENT_INTERFACE_H_

#include <gadget/gadgetConfig.h>

#include <gadget/Event/EventInterface.h>
#include <gadget/Event/BasicEventGenerator.h>
#include <gadget/Event/KeyboardMouse/SampleHandler.h>
#include <gadget/Type/KeyboardMouseProxy.h>


namespace gadget
{

/** \class KeyboardMouseEventInterface KeyboardMouseEventInterface.h gadget/Event/KeyboardMouseEventInterface.h
 *
 * The event interface for gadget::KeyboardMouseProxy objects.
 *
 * @tparam CollectionTag A tag specifyiing which event(s) will be collected by
 *                       the event generator created by this object. This must
 *                       be a valid collection tag in order for the code to
 *                       compile. This template paramter is optional, and it
 *                       defaults to gadget::event::all_events_tag.
 * @tparam GenerationTag A tag specifying how events will be emitted by the
 *                       event generator created by this object. This must be
 *                       a valid generation tag in order for the code to
 *                       compile. This template paramter is optional, and it
 *                       defaults to gadget::event::immediate_tag.
 *
 * @since 2.1.8
 */
template<typename CollectionTag = event::all_events_tag
       , typename GenerationTag = event::immediate_tag>
class KeyboardMouseEventInterface
   : public EventInterface<KeyboardMouseProxy
                         , BasicEventGenerator<KeyboardMouseProxy
                                             , CollectionTag
                                             , GenerationTag
                                             , event::kbd::SampleHandler>
                         >
{
};

}


#endif /* _GADGET_KEYBOARD_MOUSE_EVENT_INTERFACE_H_ */
