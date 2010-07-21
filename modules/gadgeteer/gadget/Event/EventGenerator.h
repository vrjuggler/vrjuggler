#ifndef _GADGET_EVENT_GENERATOR_H_
#define _GADGET_EVENT_GENERATOR_H_

#include <gadget/gadgetConfig.h>

#include <boost/enable_shared_from_this.hpp>
#include <boost/noncopyable.hpp>

#include <gadget/Event/EventGeneratorPtr.h>


namespace gadget
{

class GADGET_CLASS_API EventGenerator
   : public boost::enable_shared_from_this<EventGenerator>
   , private boost::noncopyable
{
protected:
   EventGenerator();

public:
   virtual ~EventGenerator();

   virtual void emitEvents() = 0;
};

}


#endif /* _GADGET_EVENT_GENERATOR_H_ */
