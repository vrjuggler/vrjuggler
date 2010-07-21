#ifndef _GADGET_INPUT_HANDLER_H_
#define _GADGET_INPUT_HANDLER_H_

#include <gadget/gadgetConfig.h>

#include <boost/enable_shared_from_this.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/noncopyable.hpp>

#include <gadget/Event/AbstractEventInterface.h>
#include <gadget/Event/EventGenerator.h>
#include <gadget/InputHandlerPtr.h>


namespace gadget
{

class GADGET_CLASS_API InputHandler
   : public boost::enable_shared_from_this<InputHandler>
   , private boost::noncopyable
{
private:
   InputHandler();

public:
   static InputHandlerPtr create();

   ~InputHandler();
/*
   void registerImmediateInterface(AbstractEventInterface* iface);

   void unregisterImmediateInterface(AbstractEventInterface* iface);
*/
   void registerPeriodicInterface(AbstractEventInterface* iface);

   void unregisterPeriodicInterface(AbstractEventInterface* iface);

   void registerSynchronizedInterface(AbstractEventInterface* iface);

   void unregisterSynchronizedInterface(AbstractEventInterface* iface);
};

}


#endif /* _GADGET_INPUT_HANDLER_H_ */
