#include <gadget/Event/KeyboardMouseEventInterface.h>


namespace gadget
{

DoubleClickEventGenerator::DoubleClickEventGenerator()
{
}

DoubleClickEventGeneratorPtr DoubleClickEventGenerator::create()
{
   return DoubleClickEventGeneratorPtr(new DoubleClickEventGenerator());
}

void DoubleClickEventGenerator::onEventAdded(const raw_data_type& data)
{
}
/*
DoubleClickEventInterface::DoubleClickEventInterface()
{
}

DoubleClickEventInterface::~DoubleClickEventInterface()
{
}
*/
}
