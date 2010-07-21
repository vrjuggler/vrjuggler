#include <gadget/gadgetConfig.h>

#include <gadget/InputHandler.h>


namespace gadget
{

InputHandler::InputHandler()
{
}

InputHandler::~InputHandler()
{
}

InputHandlerPtr InputHandler::create()
{
   return InputHandlerPtr(new InputHandler());
}
/*
void InputHandler::registerImmediateInterface(AbstractEventInterface* iface)
{
}

void InputHandler::unregisterImmediateInterface(AbstractEventInterface* iface)
{
}
*/
void InputHandler::registerPeriodicInterface(AbstractEventInterface* iface)
{
}

void InputHandler::unregisterPeriodicInterface(AbstractEventInterface* iface)
{
}

void InputHandler::registerSynchronizedInterface(AbstractEventInterface* iface)
{
}

void
InputHandler::unregisterSynchronizedInterface(AbstractEventInterface* iface)
{
}

}
