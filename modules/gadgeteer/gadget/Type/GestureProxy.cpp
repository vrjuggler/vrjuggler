#include <vjConfig.h>
#include <Kernel/vjKernel.h>
#include <Input/InputManager/vjGestureProxy.h>

bool vjGestureProxy::config(vjConfigChunk* chunk)
{
   vjDEBUG_BEGIN(vjDBG_ALL,1) << "------------------ GESTURE PROXY -----------------\n"
                    << "   config()" << endl << vjDEBUG_FLUSH;
   vjASSERT(((std::string)chunk->getType()) == "GestureProxy");

   int unitNum = chunk->getProperty("unit");
   std::string proxy_name = chunk->getProperty("name");
   std::string dev_name = chunk->getProperty("device");

   int proxy_num = vjKernel::instance()->getInputManager()->addGestureProxy(dev_name,unitNum,proxy_name,this);

   if ( proxy_num != -1)
   {
      vjDEBUG_END(vjDBG_ALL,1) << "   GestureProxy config()'ed" << endl << vjDEBUG_FLUSH;
      return true;
   }
   else
   {
      vjDEBUG_END(vjDBG_ALL,1) << "   GestureProxy config() failed" << endl << vjDEBUG_FLUSH;
      return false;
   }
}

