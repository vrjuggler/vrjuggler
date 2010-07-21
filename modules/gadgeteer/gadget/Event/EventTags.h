#ifndef _GADGET_EVENT_TAGS_H_
#define _GADGET_EVENT_TAGS_H_


namespace gadget
{

namespace event
{

/** @name Event Behavior Tags */
//@{
struct generator_tag {};
struct immediate_tag : generator_tag {};
struct periodic_tag : generator_tag {};
struct synchronized_tag : generator_tag {};
//@}

struct collector_tag {};
struct all_events_tag : collector_tag {};
struct last_event_tag : collector_tag {};

}

}

#endif /* _GADGET_EVENT_TAGS_H_ */
