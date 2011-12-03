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

#ifndef _GADGET_COMMAND_EVENT_INTERFACE_H_
#define _GADGET_COMMAND_EVENT_INTERFACE_H_

#include <gadget/gadgetConfig.h>

#include <gadget/Event/MultiEventInterface.h>
#include <gadget/Event/MultiEventGenerator.h>
#include <gadget/Type/CommandProxy.h>


namespace gadget
{

namespace event
{

/** \struct command_event_tag CommandEventInterface.h gadget/Event/CommandEventInterface.h
 *
 * The event tag for command events emitted by instances of
 * gadget::CommandEventInterface.
 *
 * @since 2.1.27
 */
struct command_event_tag : base_event_tag {};

}

/** \class CommandEventInterface CommandEventInterface.h gadget/Event/CommandEventInterface.h
 *
 * The multi-event interface for gadget::CommandProxy objects. While this is
 * a multi-event interface, there is only only event tag supported:
 * gadget::event::command_event_tag.
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
 * @note This was converted to a multi-event interface in 2.1.27.
 * 
 * @since 2.1.7
 */
template<typename CollectionTag = event::last_event_tag
       , typename GenerationTag = event::synchronized_tag>
class CommandEventInterface
   : public MultiEventInterface<CommandProxy
                              , MultiEventGenerator<
                                     CommandProxy
                                   , boost::mpl::vector<
                                        event::command_event_tag
                                     >
                                   , CollectionTag
                                   , GenerationTag
                                >
                              >
{
public:
   typedef typename CommandEventInterface::event_interface_ base_type;

   using base_type::addCallback;

   /**
    * Adds a callback for the gadget::event::command_event_tag event tag.
    * This is for convenience since this multi-event interface supports
    * just one event tag.
    */
   void addCallback(const typename base_type::callback_type& callback)
   {
      base_type::template addCallback<event::command_event_tag>(callback);
   }
};

}


#endif /* _GADGET_COMMAND_EVENT_INTERFACE_H_ */
