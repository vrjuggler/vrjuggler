#ifndef _GADGET_DEVICE_CONSTRUCTOR_H_
#define _GADGET_DEVICE_CONSTRUCTOR_H_

#include <gadget/gadgetConfig.h>

#include <string>
#include <vpr/Util/Assert.h>
#include <jccl/Config/ConfigChunk.h>

#include <gadget/InputManager.h>
#include <gadget/Type/DeviceFactory.h>
#include <gadget/Type/DeviceConstructorBase.h>


namespace gadget
{

template <class DEV>
class DeviceConstructor : public DeviceConstructorBase
{
public:
   DeviceConstructor(gadget::InputManager* inputMgr)
   {
      vprASSERT(DeviceFactory::instance() != NULL);
      vprASSERT(inputMgr != NULL);
      inputMgr->getDeviceFactory()->registerDevice(this);
   }

   Input* createDevice(jccl::ConfigChunkPtr chunk)
   {
      DEV* new_dev = new DEV;
      bool success = new_dev->config(chunk);
      if(success)
      {
         return new_dev;
      }
      else
      {
         delete new_dev;
         return NULL;
      }
   }

   virtual std::string getChunkType()
   {
      return DEV::getChunkType();
   }
};

} // End of gadget namespace


#endif
