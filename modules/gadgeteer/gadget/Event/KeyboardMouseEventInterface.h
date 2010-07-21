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

#ifndef _GADGET_KEYBOARD_MOUSE_EVENT_INTERFACE_H_
#define _GADGET_KEYBOARD_MOUSE_EVENT_INTERFACE_H_

#include <gadget/gadgetConfig.h>

#include <gadget/Event/EventInterface.h>
#include <gadget/Event/BasicEventGenerator.h>
#include <gadget/Type/KeyboardMouseProxy.h>


namespace gadget
{

// This should probably be something like MouseEventInterface,
// KeyEventInterface, etc. Having a DoubleClickEventInterface is the goal.
// Perhaps there should be a MouseClickEventInterface?

typedef EventInterface<KeyboardMouseProxy
                     , BasicEventGenerator<KeyboardMouseProxy
                                         , event::all_events_tag
                                         , event::immediate_tag
                                         , gadget::EventPtr>
                     , gadget::EventPtr> KeyboardMouseEventInterface;

typedef boost::shared_ptr<class DoubleClickEventGenerator>
   DoubleClickEventGeneratorPtr;

class GADGET_CLASS_API DoubleClickEventGenerator
   : public BasicEventGenerator<KeyboardMouseProxy
                              , event::last_event_tag
                              , event::periodic_tag
                              , gadget::EventPtr>
{
protected:
   DoubleClickEventGenerator();

public:
   static DoubleClickEventGeneratorPtr create();

protected:
   virtual void onEventAdded(const raw_data_type& data);
};

typedef EventInterface<KeyboardMouseProxy
                     , DoubleClickEventGenerator
                     , gadget::EventPtr> DoubleClickEventInterface;

/*
class GADGET_CLASS_API DoubleClickEventInterface
   : public EventInterface<KeyboardMouseProxy
                         , DoubleClickEventGenerator
                         , gadget::EventPtr>
{
public:
   DoubleClickEventInterface();

   virtual ~DoubleClickEventInterface();
};
*/
}


#endif /* _GADGET_KEYBOARD_MOUSE_EVENT_INTERFACE_H_ */
