#include <vjConfig.h>
#include <Kernel/vjKernel.h>
#include <Input/InputManager/vjGestureProxy.h>

bool vjGestureProxy::config(vjConfigChunk* chunk)
{
   vjDEBUG_BEGIN(1) << "------------------ GESTURE PROXY -----------------\n"
                    << "   config()" << endl << vjDEBUG_FLUSH;
   vjASSERT(((string)(char*)chunk->getType()) == "GestureProxy");

   int unitNum = chunk->getProperty("unit");
   string proxy_name = (char*)chunk->getProperty("name");
   string dev_name = (char*)chunk->getProperty("device");

   int proxy_num = vjKernel::instance()->getInputManager()->AddGestureProxy(dev_name,unitNum,proxy_name,this);

   if ( proxy_num != -1)
   {
      vjDEBUG_END(1) << "   GestureProxy config()'ed" << endl << vjDEBUG_FLUSH;
      return true;
   }
   else
   {
      vjDEBUG_END(1) << "   GestureProxy config() failed" << endl << vjDEBUG_FLUSH;
      return false;
   }
}

