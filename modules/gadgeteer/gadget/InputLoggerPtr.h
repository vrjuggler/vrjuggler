#ifndef _GADGET_INPUT_LOGGER_PTR_H_
#define _GADGET_INPUT_LOGGER_PTR_H_

#include <boost/smart_ptr.hpp>

namespace gadget
{
   class InputLogger;
   typedef boost::shared_ptr<ChunkDesc> InputLoggerPtr;
}

#endif

