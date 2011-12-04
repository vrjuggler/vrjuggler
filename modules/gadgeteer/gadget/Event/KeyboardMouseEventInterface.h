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

#ifndef _GADGET_KEYBOARD_MOUSE_EVENT_INTERFACE_H_
#define _GADGET_KEYBOARD_MOUSE_EVENT_INTERFACE_H_

#include <gadget/gadgetConfig.h>

#include <gadget/Event/MultiEventInterface.h>
#include <gadget/Event/MultiEventGenerator.h>
#include <gadget/Event/KeyboardMouse/SampleHandler.h>
#include <gadget/Type/KeyboardMouseProxy.h>


namespace gadget
{

namespace event
{

/** \struct kbd_mouse_event_tag KeyboardMouseEventInterface.h gadget/Event/KeyboardMouseEventInterface.h
 *
 * The event tag for keyboard/mouse events emitted by instances of
 * gadget::KeyboardMouseEventInterface.
 *
 * @since 2.1.27
 */
struct kbd_mouse_event_tag : base_event_tag {};

}

/** \class KeyboardMouseEventInterface KeyboardMouseEventInterface.h gadget/Event/KeyboardMouseEventInterface.h
 *
 * The multi-event interface for gadget::KeyboardMouseProxy objects. While
 * this is a multi-event interface, there is only only event tag supported:
 * gadget::event::kbd_mouse_event_tag.
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
 * @note This was converted to a multi-event interface in 2.1.27.
 *
 * @since 2.1.8
 */
template<typename CollectionTag = event::all_events_tag
       , typename GenerationTag = event::immediate_tag>
class KeyboardMouseEventInterface
   : public MultiEventInterface<KeyboardMouseProxy
                              , MultiEventGenerator<
                                     KeyboardMouseProxy
                                   , boost::mpl::vector<
                                        event::kbd_mouse_event_tag
                                     >
                                   , CollectionTag
                                   , GenerationTag
                                   , event::kbd::SampleHandler
                                >
                              >
{
public:
   typedef typename KeyboardMouseEventInterface::event_interface_ base_type;

   using base_type::addCallback;

   /**
    * Adds a callback for the gadget::event::kbd_mouse_event_tag event tag.
    * This is for convenience since this multi-event interface supports
    * just one event tag.
    */
   void addCallback(const typename base_type::callback_type& callback)
   {
      base_type::template addCallback<event::kbd_mouse_event_tag>(callback);
   }
};

}


#endif /* _GADGET_KEYBOARD_MOUSE_EVENT_INTERFACE_H_ */
