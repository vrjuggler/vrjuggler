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
