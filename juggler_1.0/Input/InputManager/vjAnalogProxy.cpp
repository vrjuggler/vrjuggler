#include <vjConfig.h>
#include <Kernel/vjKernel.h>
#include <Input/InputManager/vjAnalogProxy.h>

bool vjAnalogProxy::config(vjConfigChunk* chunk)
{
   vjDEBUG_BEGIN(1) << "------------------ ANALOG PROXY -----------------\n"
                    << "   config()" << endl << vjDEBUG_FLUSH;
   vjASSERT(((std::string)chunk->getType()) == "AnaProxy");

   int unitNum = chunk->getProperty("unit");
   std::string proxy_name = chunk->getProperty("name");
   std::string dev_name = chunk->getProperty("device");

   int proxy_num = vjKernel::instance()->getInputManager()->AddAnaProxy(dev_name,unitNum,proxy_name,this);

   if ( proxy_num != -1)
   {
      vjDEBUG_END(1) << "   AnaProxy config()'ed" << endl << vjDEBUG_FLUSH;
      return true;
   }
   else
   {
      vjDEBUG_END(1) << "   AnaProxy config() failed" << endl << vjDEBUG_FLUSH;
      return false;
   }
}

