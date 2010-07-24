#include <gadget/gadgetConfig.h>

#include <vpr/Util/Assert.h>

#include <gadget/Event/AbstractEventInterface.h>
#include <gadget/Event/EventGenerator.h>
#include <gadget/InputHandler.h>


namespace gadget
{

InputHandler::InputHandler()
{
   /* Do nothing. */ ;
}

InputHandler::~InputHandler()
{
   /* Do nothing. */ ;
}

InputHandlerPtr InputHandler::create()
{
   return InputHandlerPtr(new InputHandler());
}

void InputHandler::registerPeriodicInterface(AbstractEventInterface* iface)
{
   vprASSERT(iface->getEventGenerator().get() != NULL);
   mPeriodicIfaces[iface] = iface->getEventGenerator();
}

void InputHandler::unregisterPeriodicInterface(AbstractEventInterface* iface)
{
   mPeriodicIfaces.erase(iface);
}

void InputHandler::registerSynchronizedInterface(AbstractEventInterface* iface)
{
   vprASSERT(iface->getEventGenerator().get() != NULL);
   mSyncIfaces[iface] = iface->getEventGenerator();
}

void
InputHandler::unregisterSynchronizedInterface(AbstractEventInterface* iface)
{
   mSyncIfaces.erase(iface);
}

void InputHandler::sync()
{
   typedef iface_map_type::iterator iter_type;
   for (iter_type i = mSyncIfaces.begin(); i != mSyncIfaces.end(); ++i)
   {
      (*i).second->emitEvents();
   }
}

}
