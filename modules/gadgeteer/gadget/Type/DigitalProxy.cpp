#include <vjConfig.h>
#include <Kernel/vjKernel.h>
#include <Input/InputManager/vjDigitalProxy.h>

bool vjDigitalProxy::config(vjConfigChunk* chunk)
{
   vjDEBUG_BEGIN(1) << "----------------- DIG PROXY ----------------------\n"
                 << "vjInputManager::ConfigureDigProxy" << endl << vjDEBUG_FLUSH;
   vjASSERT(((string)(char*)chunk->getType()) == "DigProxy");


   int unitNum = chunk->getProperty("unit");
   string proxy_name = (char*)chunk->getProperty("name");
   string dev_name = (char*)chunk->getProperty("device");

   int proxy_num = vjKernel::instance()->getInputManager()->AddDigProxy(dev_name,unitNum,proxy_name,this);

   if ( proxy_num != -1)
   {
      vjDEBUG_END(1) << "   DigProxy config()'ed" << endl << vjDEBUG_FLUSH;
      return true;
   }
   else
   {
      vjDEBUG_END(1) << "   DigProxy config() failed" << endl << vjDEBUG_FLUSH;
      return false;
   }
}


