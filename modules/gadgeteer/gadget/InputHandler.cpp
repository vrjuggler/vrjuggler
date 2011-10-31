#include <gadget/gadgetConfig.h>

#include <vpr/Util/Assert.h>
#include <vpr/Sync/Guard.h>

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
   vpr::Guard<vpr::Mutex> sv_guard(mPeriodicIfacesLock);
   vprASSERT(iface->getEventGenerator().get() != NULL);
   mPeriodicIfaces[iface] = iface->getEventGenerator();
}

void InputHandler::unregisterPeriodicInterface(AbstractEventInterface* iface)
{
   vpr::Guard<vpr::Mutex> sv_guard(mPeriodicIfacesLock);
   mPeriodicIfaces.erase(iface);
}

void InputHandler::registerSynchronizedInterface(AbstractEventInterface* iface)
{
   vpr::Guard<vpr::Mutex> sv_guard(mSyncIfacesLock);
   vprASSERT(iface->getEventGenerator().get() != NULL);
   mSyncIfaces[iface] = iface->getEventGenerator();
}

void
InputHandler::unregisterSynchronizedInterface(AbstractEventInterface* iface)
{
   vpr::Guard<vpr::Mutex> sv_guard(mSyncIfacesLock);
   mSyncIfaces.erase(iface);
}

void InputHandler::sync()
{
   vpr::Guard<vpr::Mutex> sv_guard(mSyncIfacesLock);

   typedef iface_map_type::iterator iter_type;
   for (iter_type i = mSyncIfaces.begin(); i != mSyncIfaces.end(); ++i)
   {
      (*i).second->emitEvents();
   }
}

}
