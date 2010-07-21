#include <algorithm>
#include <boost/bind.hpp>

#include <vpr/Util/Assert.h>

#include <gadget/Event/AbstractEventInterface.h>


namespace gadget
{

std::vector<AbstractEventInterface*> AbstractEventInterface::sAllocatedInterfaces;

AbstractEventInterface::AbstractEventInterface()
{
   sAllocatedInterfaces.push_back(this);
}

AbstractEventInterface::~AbstractEventInterface()
{
   // Attempt to find the device, if found, erase it, if not, then assert
   std::vector<AbstractEventInterface*>::iterator i =
      std::find(sAllocatedInterfaces.begin(), sAllocatedInterfaces.end(),
                this);
   vprASSERT(i != sAllocatedInterfaces.end());
   sAllocatedInterfaces.erase(i);
}

void AbstractEventInterface::refreshAllInterfaces()
{
   std::for_each(sAllocatedInterfaces.begin(), sAllocatedInterfaces.end(),
                 boost::bind(&AbstractEventInterface::refresh, _1));
}

}
