#include <vjConfig.h>
#include <Kernel/vjKernel.h>
#include <Input/InputManager/vjDigitalProxy.h>

bool vjDigitalProxy::config(vjConfigChunk* chunk)
{
   vjDEBUG_BEGIN(1) << "----------------- DIG PROXY ----------------------\n"
                 << "vjInputManager::ConfigureDigProxy" << endl << vjDEBUG_FLUSH;
   vjASSERT(((std::string)(char*)chunk->getType()) == "DigProxy");


   int unitNum = chunk->getProperty("unit");
   std::string proxy_name = (char*)chunk->getProperty("name");
   std::string dev_name = (char*)chunk->getProperty("device");

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


void vjDigitalProxy::UpdateData()
{
    int new_state = m_digPtr->GetDigitalData(m_unitNum);
    int old_state = m_data;
    if(vjDigital::OFF == old_state)
	{
	    if(new_state)     // Button now pressed
		m_data = vjDigital::TOGGLE_ON;
	    else              // Button now released
		m_data = vjDigital::OFF;
	}
    else if(vjDigital::ON == old_state)
	{
	    if(new_state)     // Button now pressed
		m_data = vjDigital::ON;
	    else              // Button now released
		m_data = vjDigital::TOGGLE_OFF;
	}
    else if(vjDigital::TOGGLE_ON == old_state)
	{
	    if(new_state)     // Button now pressed
		m_data = vjDigital::ON;
	    else              // Button now released
		m_data = vjDigital::TOGGLE_OFF;
	}
    else if(vjDigital::TOGGLE_OFF == old_state)
	{
	    if(new_state)     // Button now pressed
		m_data = vjDigital::TOGGLE_ON;
	    else              // Button now released
		m_data = vjDigital::OFF;
	}
}
