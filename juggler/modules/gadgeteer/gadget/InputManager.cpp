#include <vjConfig.h>

#include <Input/InputManager/vjInputManager.h>
#include <Kernel/vjDebug.h>

// Sims
#include <Input/vjSim/vjSimAnalog.h>
#include <Input/vjSim/vjSimDigital.h>
#include <Input/vjSim/vjSimPosition.h>
#include <Input/vjSim/vjSimGloveGesture.h>

// Physical devices
#ifndef WIN32
#include <Input/vjPosition/vjFlock.h>
#include <Input/vjInput/vjIbox.h>
#include <Input/vjGlove/vjCyberGlove.h>
#include <Input/vjKeyboard/vjXWinKeyboard.h>
#include <Input/vjPosition/logiclass.h>
#else
#include <Input/vjInput/vjKeyboardWin32.h>
#endif

#include <Input/InputManager/vjDeviceFactory.h>
#include <Input/InputManager/vjProxyFactory.h>

// Local helpers
bool recognizeProxyAlias(vjConfigChunk* chunk);

/**********************************************************
  vjInputManager::vjInputManager()

  vjInputManager Constructor

*********************************************** ahimberg */
vjInputManager::vjInputManager() : m_dummyPos(), m_dummyDig(), m_dummyAna(), m_dummyGlove()
{
   m_dummyPosProxy = new vjPosProxy();      // Create dummy proxy
   m_dummyPosProxy->Set(&m_dummyPos,0);
   m_dummyGlove.setGlovePosProxy(m_dummyPosProxy);       // So dummy glove has a location
}


/**********************************************************
  vjInputManager::~vjInputManager()

  vjInputManager Destructor

*********************************************** ahimberg */
vjInputManager::~vjInputManager()
{
   for (int a = 0; a < m_devVector.size(); a++)    // Stop all devices
      if (m_devVector[a] != NULL)
         m_devVector[a]->StopSampling();
   for (int i = 0; i < m_devVector.size(); i++)    // Delete all devices
      if (m_devVector[i] != NULL)
         delete m_devVector[i];
   for (int j = 0; j < m_posProxyVector.size(); j++)  // Delete all pos proxies
      if (m_posProxyVector[j] != NULL)
         delete m_posProxyVector[j];
   for (int k = 0; k < m_digProxyVector.size(); k++)  // Delete all digital proxies
      if (m_digProxyVector[k] != NULL)
         delete m_digProxyVector[k];
   for (int l = 0; l < m_anaProxyVector.size(); l++)  // Delete all analog proxies
      if (m_anaProxyVector[l] != NULL)
         delete m_anaProxyVector[l];
   for (int m = 0; m < m_gloveProxyVector.size(); m++)  // Delete all glove proxies
      if (m_gloveProxyVector[m] != NULL)
         delete m_gloveProxyVector[m];
   for (int n = 0; n < m_gestureProxyVector.size(); n++)  // Delete all gesture proxies
      if (m_gestureProxyVector[n] != NULL)
         delete m_gestureProxyVector[n];
}

//: Set the initial configuration
// Finds the InputManager chunks in the db
// uses these to configure dummyPos
void vjInputManager::ConfigureInitial(vjConfigChunkDB *cdb)
{
   vjDEBUG(1) << "vjInputManager::ConfigureInputManager" << endl << vjDEBUG_FLUSH;
   vjConfigChunk *c;// = cdb->getChunk("vjInputManager");
   std::vector<vjConfigChunk*> *vChunks;
   vChunks = cdb->getMatching("InputManager");

   if (vChunks->size() > 0)
   {
      c = (*vChunks)[0];

      m_dummyPos.posData().makeXYZEuler(c->getProperty("dummyOr",0) ,
                                         c->getProperty("dummyOr",1) ,
                                         c->getProperty("dummyOr",2));

      m_dummyPos.posData().setTrans( c->getProperty("dummyPos",0) ,
                                      c->getProperty("dummyPos",1) ,
                                      c->getProperty("dummyPos",2));
   }
   delete vChunks;
}

//: Add the given config chunk to the input system
bool vjInputManager::configAdd(vjConfigChunk* chunk)
{
   vjDEBUG_BEGIN(1) << "\nvjInputManager::configAdd() " << endl << vjDEBUG_FLUSH;

   bool ret_val = false;      // Flag to return success

   if(vjDeviceFactory::instance()->recognizeDevice(chunk))
      ret_val = ConfigureDevice(chunk);
   else if(vjProxyFactory::instance()->recognizeProxy(chunk))
      ret_val = ConfigureProxy(chunk);
   else if(recognizeProxyAlias(chunk))
      ret_val = ConfigureProxyAlias(chunk);

   //DumpStatus();                      // Dump the status
   if(ret_val)
   {
      UpdateAllData();
      vjDEBUG(1) << "InputManager::configAdd(): Updated all data" << endl << vjDEBUG_FLUSH;
   }

   vjDEBUG_END(1) << endl << vjDEBUG_FLUSH;
   return ret_val;         // Return the success flag if we added at all
}

// Return true if:
//  It is recognized device, proxy, or alias.
bool vjInputManager::configCanHandle(vjConfigChunk* chunk)
{
   return ( vjDeviceFactory::instance()->recognizeDevice(chunk) ||
            vjProxyFactory::instance()->recognizeProxy(chunk) ||
            recognizeProxyAlias(chunk));
}

// Check if the device factory or proxy factory can handle the chunk
bool vjInputManager::ConfigureDevice(vjConfigChunk* chunk)
{
   bool ret_val;
   std::string dev_name = (std::string)(char*)chunk->getProperty("name");
   vjDEBUG_BEGIN(1) << "ConfigureDevice: Named: " << dev_name << endl << vjDEBUG_FLUSH;

   vjInput* new_device;
   new_device = vjDeviceFactory::instance()->loadDevice(chunk);

   if ((new_device != NULL) && (new_device->StartSampling()))
   {
      int dev_index = FAddDevice(new_device);
      ret_val = true;
      vjDEBUG(1) << "   Successfully added dev: " << dev_name << "  at index:" << dev_index << endl << vjDEBUG_FLUSH;
   }
   else
   {
      vjDEBUG(0) << "ERROR: new dev failed to start.. deleting instance" << endl << vjDEBUG_FLUSH;
      delete new_device;
      ret_val = false;
   }

   vjDEBUG_END(1) << endl << vjDEBUG_FLUSH;
   return ret_val;
}

// Check if the device factory or proxy factory can handle the chunk
bool vjInputManager::ConfigureProxy(vjConfigChunk* chunk)
{
   std::string proxy_name = (std::string)(char*)chunk->getProperty("name");
   vjDEBUG_BEGIN(1) << "ConfigureProxy: Named: " << proxy_name << endl << vjDEBUG_FLUSH;

   vjProxy* new_proxy;
   new_proxy = vjProxyFactory::instance()->loadProxy(chunk);

   if ( new_proxy != NULL)
   {
      vjDEBUG_END(1) << "   Proxy is set" << endl << vjDEBUG_FLUSH;
      return true;
   }
   else
   {
      vjDEBUG(0) << "ERROR: Could not configure proxy:" << proxy_name << endl << vjDEBUG_FLUSH;
      vjDEBUG_END(1) << endl << vjDEBUG_FLUSH;
      return false;
   }
}


/**********************************************************
  vjInputManager::DumpStatus()

  Dump the current Status of the vjInputManager, listing all
  the devices, proxies and internal settings

*********************************************** ahimberg */
void vjInputManager::DumpStatus()
{
  int i;
  cout << "**vjInputManager Status: " << endl;

  for (i = 0; i < m_devVector.size(); i++)      // Dump DEVICES
     if (m_devVector[i] != NULL)
       cout << "  Device #" << setw(2) << i << ":"
            << "  Instance Name: " << setw(10) << m_devVector[i]->GetInstanceName()
	    << " "
	    << "   Device Type: " << m_devVector[i]->GetDeviceName() << endl
	    << "      on Port : " << setw(10) << m_devVector[i]->GetPort()
	    << "         baud : " << m_devVector[i]->GetBaudRate() << endl;

  cout << endl;

  int d;
  for (i = 0; i < m_posProxyVector.size(); i++)    // DUMP PosProxies
  {
     d = FindDeviceNum( (m_posProxyVector[i]->GetPositionPtr())->GetInstanceName() );
     if (d != -1)
       cout << "  PosProxy#" << setw(2) << i << ":"
            << "   Proxies Device: "
	    << setw(10)
	    << (m_posProxyVector[i]->GetPositionPtr())->GetInstanceName()
	    << "  sub-unit number: " <<
	             m_posProxyVector[i]->GetUnit() << endl;
  }

  cout << endl;

  for (i = 0; i < m_digProxyVector.size(); i++)                // Dump DigitalProxies
  {
     d = FindDeviceNum( (m_digProxyVector[i]->GetDigitalPtr())->GetInstanceName() );
     if (d != -1)
       cout << "  DigProxy#" << setw(2) << i << ":"
            << "   Proxies Device: "
	    << setw(10)
	    << (m_digProxyVector[i]->GetDigitalPtr())->GetInstanceName()
	    << "  sub-unit number: " <<
	             m_digProxyVector[i]->GetUnit() << endl;
  }

  cout << endl;

  for (i = 0; i < m_anaProxyVector.size(); i++)                 // Dump Analog Proxies
  {
     d = FindDeviceNum( (m_anaProxyVector[i]->GetAnalogPtr())->GetInstanceName() );
     if (d != -1)
       cout << "  AnaProxy#" << setw(2) << i << ":"
            << "   Proxies Device: "
	    << setw(10)
	    << (m_anaProxyVector[i]->GetAnalogPtr())->GetInstanceName()
	    << "  sub-unit number: " <<
	             m_anaProxyVector[i]->GetUnit() << endl;
  }

  for (i = 0; i < m_gloveProxyVector.size(); i++)                 // Dump Glove Proxies
  {
     d = FindDeviceNum( (m_gloveProxyVector[i]->getGlovePtr())->GetInstanceName() );
     if (d != -1)
       cout << "  GloveProxy#" << setw(2) << i << ":"
            << "   Proxies Device: "
	         << setw(10)
	         << (m_gloveProxyVector[i]->getGlovePtr())->GetInstanceName()
	         << "  sub-unit number: " <<
	             m_gloveProxyVector[i]->getUnit() << endl;
  }

  for (i = 0; i < m_keyboardProxyVector.size(); i++)                 // Dump Keyboard Proxies
  {
     d = FindDeviceNum( (m_keyboardProxyVector[i]->getKeyboardPtr())->GetInstanceName() );
     if (d != -1)
       cout << "  KeyboardProxy#" << setw(2) << i << ":"
            << "   Proxies Device: "
	         << setw(10)
	         << (m_keyboardProxyVector[i]->getKeyboardPtr())->GetInstanceName()
	         << endl;
  }

  for (i = 0; i < m_gestureProxyVector.size(); i++)                 // Dump Gesture Proxies
  {
     d = FindDeviceNum( (m_gestureProxyVector[i]->getGesturePtr())->GetInstanceName() );
     if (d != -1)
       cout << "  GestureProxy#" << setw(2) << i << ":"
            << "   Proxies Device: "
	         << setw(10)
	         << (m_gestureProxyVector[i]->getGesturePtr())->GetInstanceName()
	         << endl;
  }

  cout << endl;

      // Dump Alias list
  cout << "   Alias List:" << endl;
  for(std::map<std::string, int>::iterator cur_alias = proxyAliases.begin(); cur_alias != proxyAliases.end(); cur_alias++)
     cout << "      Alias:" << (*cur_alias).first << "  index:" << (*cur_alias).second << endl;
	
  cout << "  vjInputManager Status**" << endl << flush;
}

/**********************************************************
  vjInputManager::FAddDevice(vjInput* devPtr)

  Add a device to the vjInputManager, returns the index
  where the device was placed

*********************************************** ahimberg */
int vjInputManager::FAddDevice(vjInput* devPtr)
{
   m_devVector.push_back(devPtr);
   return (m_devVector.size()-1);   // ASSERT: This is the index we placed
}



/**********************************************************
  vjInputManager::UpdateAllData()

  Call UpdateData() on all the devices and transform proxies

*********************************************** ahimberg */
void vjInputManager::UpdateAllData()
{
   int j;
   for (j = 0; j < m_devVector.size(); j++)
      if (m_devVector[j] != NULL)
         m_devVector[j]->UpdateData();

      // The posProxies copy the data locally and may do a transform, so
      // need to call their UpdateData function, too
      // NOTE: Glove prxies don't need UpdateData()
   for (j = 0; j < m_posProxyVector.size(); j++)
      m_posProxyVector[j]->UpdateData();
   for (j = 0; j < m_digProxyVector.size(); j++)
      m_digProxyVector[j]->UpdateData();
   for (j = 0; j < m_anaProxyVector.size(); j++)
      m_anaProxyVector[j]->UpdateData();

}


/**********************************************************
  vjInputManager::FindDeviceNum(char* instName)

  Find a devive Number from the instance name of a device,
  if the instance name is not in the device array returns -1

*********************************************** ahimberg */
int vjInputManager::FindDeviceNum(const char* instName)
{
   for (int i = 0; i < m_devVector.size(); i++)
   {
      if (m_devVector[i] != NULL)
         if ((strcasecmp(m_devVector[i]->GetInstanceName(),instName) == 0)
             && (strcasecmp(m_devVector[i]->GetInstanceName(),"Undefined") != 0))
            return i;
   }
   return -1;
}


/**********************************************************
  vjInputManager::GetDevice(int devNum)

  Get the vjInput pointer at a device,
  NOTE: this can return a null pointer if there is not a
	device at devNum

*********************************************** ahimberg */
vjInput* vjInputManager::GetDevice(int devNum)
{
    assert (devNum < (m_devVector.size() -1));
    //assert (m_devVector[devNum] != NULL);

    return m_devVector[devNum];
}


/**********************************************************
  vjInputManager::RemoveDevice(int devNum)

  vjInputManager remove devNum from the vjInputManager,
  currently stupifies all the proxies connected to it.

*********************************************** ahimberg */
int vjInputManager::RemoveDevice(int devNum)
{
   int i;
   if (m_devVector[devNum] == NULL)    // Check for valid device
      return 0;

      // Stupify any proxies connected to device
   for (i = 0; i < m_posProxyVector.size(); i++)
      if ((vjInput*)(m_posProxyVector[i]) == m_devVector[devNum])
         StupifyPos(i);
   for (i = 0; i < m_digProxyVector.size(); i++)
      if ((vjInput*)(m_digProxyVector[i]) == m_devVector[devNum])
         StupifyDig(i);
   for (i = 0; i < m_anaProxyVector.size(); i++)
      if ((vjInput*)(m_anaProxyVector[i]) == m_devVector[devNum])
         StupifyAna(i);
   for (i = 0; i < m_gloveProxyVector.size(); i++)
      if ((vjInput*)(m_gloveProxyVector[i]) == m_devVector[devNum])
         StupifyGlove(i);

      // stop the device, delete it, set pointer to NULL
   m_devVector[devNum]->StopSampling();
   delete m_devVector[devNum];
   m_devVector[devNum] = NULL;

   return 1;
}

/**********************************************************
  vjInputManager::RemoveDevice(char* instName)

  vjInputManager remove instName from the vjInputManager,
  currently stupifies all the proxies connected to it.

*********************************************** ahimberg */
int vjInputManager::RemoveDevice(char* instName)
{
   // find the device
   int i = FindDeviceNum(instName);

   // -1 means not found, so fail
   if (i == -1)
      return 0;
   else // Let RemoveDevice(int devNum) takeover
      return (RemoveDevice(i) );

}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * *
  * * * * * * * * * * * * * * * * * * * * * * * * * * * *
		POSITION PROXY FUNCTIONS
* * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * */


/**********************************************************
  vjInputManager::SetPosProxy(int ProxyNum, int DevNum, int subNum)

  Set the position proxy #ProxyNum to be hooked to
                  device #DevNum at sub-unit #subNum

*********************************************** ahimberg */	
int vjInputManager::SetPosProxy(int ProxyNum, int DevNum, int subNum)
{
   m_posProxyVector[ProxyNum]->Set(dynamic_cast<vjPosition*>(m_devVector[DevNum]),subNum);
   return 1;
}

//: Add the pos proxy
//! POST: pos proxy has been added
//+   proxy alias has been set
//! RETURNS: -1: failure, >0: proxy_num
int vjInputManager::AddPosProxy(std::string devName, int subNum, std::string proxyName, vjPosProxy* posProxy)
{
   int dev_num = FindDeviceNum(devName.c_str());              // Get device number
   if(-1 == dev_num)       // Not found, ERROR
      return -1;

   vjDEBUG(1) << "adding a posProxy : " << proxyName << endl << vjDEBUG_FLUSH;
   vjDEBUG(1) << "   attaching to device named: " << devName << endl << vjDEBUG_FLUSH;
   vjDEBUG(1) << "   at unit number: " << subNum << endl << vjDEBUG_FLUSH;
   vjDEBUG(1) << "	Found the device at devNum: " << dev_num << endl << vjDEBUG_FLUSH;

   posProxy->Set(dynamic_cast<vjPosition*>(m_devVector[dev_num]),subNum);
   m_posProxyVector.push_back(posProxy);

   int proxy_num = (m_posProxyVector.size() -1);         // Get proxy num
   AddProxyAlias(proxyName, proxy_num);                  // Add alias

   return proxy_num;
}

/**********************************************************
  vjInputManager::StupifyPos(int ProxyNum)

  Make Position Proxy #ProxyNum point back to the dummyPos

*********************************************** ahimberg */
void vjInputManager::StupifyPos(int ProxyNum)
{
   m_posProxyVector[ProxyNum]->Set(&m_dummyPos,0);
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * *
  * * * * * * * * * * * * * * * * * * * * * * * * * * * *
		DIGITAL PROXY FUNCTIONS
* * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * */


/**********************************************************
  vjInputManager::SetDigProxy(int ProxyNum, int DevNum, int subNum)

  Set the digital proxy number #ProxyNum to be hooked up to
            device number #devNum at sub-unit #subNum

*********************************************** ahimberg */
int vjInputManager::SetDigProxy(int ProxyNum, int DevNum, int subNum)
{
   m_digProxyVector[ProxyNum]->Set(dynamic_cast<vjDigital*>(m_devVector[DevNum]),subNum);
   return 1;
}

//: Add the digital proxy
//! POST: dig proxy has been added
//+   proxy alias has been set
//! RETURNS: -1: failure, >0: proxy_num
int vjInputManager::AddDigProxy(std::string devName, int subNum, std::string proxyName, vjDigitalProxy* digitalProxy)
{
   int dev_num = FindDeviceNum(devName.c_str());              // Get device number
   if(-1 == dev_num)       // Not found, ERROR
      return -1;

   vjDEBUG(1) << "adding a digProxy : " << proxyName << endl << vjDEBUG_FLUSH;
   vjDEBUG(1) << "   attaching to device named: " << devName << endl << vjDEBUG_FLUSH;
   vjDEBUG(1) << "   at unit number: " << subNum << endl << vjDEBUG_FLUSH;
   vjDEBUG(1) << "	Found the device at devNum: " << dev_num << endl << vjDEBUG_FLUSH;

   digitalProxy->Set(dynamic_cast<vjDigital*>(m_devVector[dev_num]),subNum);      // Set the proxy
   m_digProxyVector.push_back(digitalProxy);                                     // Add to proxy array

   int proxy_num = (m_digProxyVector.size() - 1);        // Get proxy num
   AddProxyAlias(proxyName, proxy_num);                  // Add the alias

   return proxy_num;
}

/**********************************************************
  vjInputManager::StupifyDig(int ProxyNum)

  Point Digital Proxy #ProxyNum to the dummyDigital of the
  vjInputManager

*********************************************** ahimberg */
void vjInputManager::StupifyDig(int ProxyNum)
{
   m_digProxyVector[ProxyNum]->Set(&m_dummyDig,0);
}

	

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * *
  * * * * * * * * * * * * * * * * * * * * * * * * * * * *
		ANALOG PROXY FUNCTIONS
* * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * */


/**********************************************************
  vjInputManager::SetAnaProxy(int ProxyNum, int DevNum, int subNum)

 Set the analog proxy #ProxyNum to be hooked to
                  device #DevNum at sub-unit #subNum

*********************************************** ahimberg */
int vjInputManager::SetAnaProxy(int ProxyNum, int DevNum, int subNum)
{
   m_anaProxyVector[ProxyNum]->Set(dynamic_cast<vjAnalog*>(m_devVector[DevNum]),subNum);
   return 1;
}

//: Add the analog proxy
//! POST: analog proxy has been added
//+   proxy alias has been set
//! RETURNS: -1: failure, >0: proxy_num
int vjInputManager::AddAnaProxy(std::string devName, int subNum, std::string proxyName, vjAnalogProxy* anaProxy)
{
   int dev_num = FindDeviceNum(devName.c_str());              // Get device number
   if(-1 == dev_num)       // Not found, ERROR
      return -1;

   vjDEBUG(1) << "adding a anaProxy : " << proxyName << endl << vjDEBUG_FLUSH;
   vjDEBUG(1) << "   attaching to device named: " << devName << endl << vjDEBUG_FLUSH;
   vjDEBUG(1) << "   at unit number: " << subNum << endl << vjDEBUG_FLUSH;
   vjDEBUG(1) << "	Found the device at devNum: " << dev_num << endl << vjDEBUG_FLUSH;

   anaProxy->Set(dynamic_cast<vjAnalog*>(m_devVector[dev_num]),subNum);    // Set the proxy
   m_anaProxyVector.push_back(anaProxy);                                  // Add to proxy array

   int proxy_num = (m_anaProxyVector.size() -1);            // Get proxy num
   AddProxyAlias(proxyName, proxy_num);                     // Add the alias

   return proxy_num;
}

/**********************************************************
  vjInputManager::StupifyAna(int ProxyNum)

  Point Analog Proxy #ProxyNum to the dummyAnalog of the
  vjInputManager

*********************************************** ahimberg */
void vjInputManager::StupifyAna(int ProxyNum)
{
   m_anaProxyVector[ProxyNum]->Set(&m_dummyAna,0);
}



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * *
  * * * * * * * * * * * * * * * * * * * * * * * * * * * *
		GLOVE PROXY FUNCTIONS
* * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * */

//: Set the glove proxy #ProxyNum to be hooked to
//+ device #DevNum at sub-unit #subNum
int vjInputManager::SetGloveProxy(int ProxyNum, int DevNum, int subNum)
{
   m_gloveProxyVector[ProxyNum]->Set(dynamic_cast<vjGlove*>(m_devVector[DevNum]),subNum);
   return 1;
}

//: Add the glove proxy
//! POST: glove proxy has been added
//+   proxy alias has been set
//! RETURNS: -1: failure, >0: proxy_num
int vjInputManager::AddGloveProxy(std::string devName, int subNum, std::string proxyName, vjGloveProxy* gloveProxy)
{
   int dev_num = FindDeviceNum(devName.c_str());              // Get device number
   if(-1 == dev_num)       // Not found, ERROR
      return -1;

   vjDEBUG(1) << "adding a gloveProxy : " << proxyName << endl << vjDEBUG_FLUSH;
   vjDEBUG(1) << "   attaching to device named: " << devName << endl << vjDEBUG_FLUSH;
   vjDEBUG(1) << "   at unit number: " << subNum << endl << vjDEBUG_FLUSH;
   vjDEBUG(1) << "	Found the device at devNum: " << dev_num << endl << vjDEBUG_FLUSH;

   gloveProxy->Set(dynamic_cast<vjGlove*>(m_devVector[dev_num]),subNum);       // Set the proxy
   m_gloveProxyVector.push_back(gloveProxy);                                  // Add to proxy array

   int proxy_num = (m_gloveProxyVector.size() -1);       // Get proxy num
   AddProxyAlias(proxyName, proxy_num);                  // Add the alias

   return proxy_num;
}


//: Make glove Proxy #ProxyNum point back to the dummyGlove
void vjInputManager::StupifyGlove(int ProxyNum)
{
   m_gloveProxyVector[ProxyNum]->Set(&m_dummyGlove,0);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * *
  * * * * * * * * * * * * * * * * * * * * * * * * * * * *
		KEYBOARD PROXY FUNCTIONS
* * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * */
//: Set the keyboard proxy #ProxyNum to be hooked to
//+ device #DevNum
int vjInputManager::SetKeyboardProxy(int ProxyNum, int DevNum)
{
   m_keyboardProxyVector[ProxyNum]->Set(dynamic_cast<vjKeyboard*>(m_devVector[DevNum]));
   return 1;
}

//: Add the keyboard proxy
//! POST: keyboard proxy has been added
//+   proxy alias has been set
//! RETURNS: -1: failure, >0: proxy_num
int vjInputManager::AddKeyboardProxy(std::string devName, int subNum, std::string proxyName, vjKeyboardProxy* kbProxy)
{
   int dev_num = FindDeviceNum(devName.c_str());              // Get device number
   if(-1 == dev_num)       // Not found, ERROR
      return -1;

   vjDEBUG(1) << "adding a anaProxy : " << proxyName << endl << vjDEBUG_FLUSH;
   vjDEBUG(1) << "   attaching to device named: " << devName << endl << vjDEBUG_FLUSH;
   vjDEBUG(1) << "   at unit number: " << subNum << endl << vjDEBUG_FLUSH;
   vjDEBUG(1) << "	Found the device at devNum: " << dev_num << endl << vjDEBUG_FLUSH;

   vjKeyboard* theKB = dynamic_cast<vjKeyboard*>(m_devVector[dev_num]);
   vjASSERT(theKB != NULL);
   kbProxy->Set(theKB);                                  // Set the proxy
   m_keyboardProxyVector.push_back(kbProxy);        // Add to proxy array

   int proxy_num = (m_keyboardProxyVector.size() -1);    // Get proxy num
   AddProxyAlias(proxyName, proxy_num);                  // Add the alias

   return proxy_num;
}

//: Make keyboard Proxy #ProxyNum point back to the dummyKeyboard
void vjInputManager::StupifyKeyboard(int ProxyNum)
{
   m_keyboardProxyVector[ProxyNum]->Set(&m_dummyKeyboard);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * *
  * * * * * * * * * * * * * * * * * * * * * * * * * * * *
		GESTURE PROXY FUNCTIONS
* * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * */
//: Set the gesture proxy
// Hook devNum to proxyNum
int vjInputManager::SetGestureProxy(int ProxyNum, int DevNum)
{
   m_gestureProxyVector[ProxyNum]->Set(dynamic_cast<vjGesture*>(m_devVector[DevNum]));
   return 1;
}

//: Add the gesture proxy
//! POST: gesture proxy has been added
//+   proxy alias has been set
//! RETURNS: -1: failure, >0: proxy_num
int vjInputManager::AddGestureProxy(std::string devName, int subNum, std::string proxyName, vjGestureProxy* gestureProxy)
{
   int dev_num = FindDeviceNum(devName.c_str());              // Get device number
   if(-1 == dev_num)       // Not found, ERROR
      return -1;

   vjDEBUG(1) << "adding a anaProxy : " << proxyName << endl << vjDEBUG_FLUSH;
   vjDEBUG(1) << "   attaching to device named: " << devName << endl << vjDEBUG_FLUSH;
   vjDEBUG(1) << "   at unit number: " << subNum << endl << vjDEBUG_FLUSH;
   vjDEBUG(1) << "	Found the device at devNum: " << dev_num << endl << vjDEBUG_FLUSH;

   gestureProxy->Set(dynamic_cast<vjGesture*>(m_devVector[dev_num]));
   m_gestureProxyVector.push_back(gestureProxy);

   int proxy_num = (m_gestureProxyVector.size() -1);        // Get proxy num
   AddProxyAlias(proxyName, proxy_num);                     // Add the alias

   return proxy_num;
}

//: Turn the gesture proxy at index ProxyNum to point back
// to the default dummy gesture proxy. <br>
// <br>
//! MODIFIES: self <br>
//! POST:  m_gestureProxyArray[proxyNum]' = m_gestureKeyboard
void vjInputManager::StupifyGesture(int ProxyNum)
{
   m_gestureProxyVector[ProxyNum]->Set(&m_dummyGesture);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * *
  * * * * * * * * * * * * * * * * * * * * * * * * * * * *
	PRIVATE FUNCTIONS TO INPUTGROUP
* * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * */


// Is it a proxy alias
bool recognizeProxyAlias(vjConfigChunk* chunk)
{
   return (((std::string)(char*)chunk->getType()) == "proxyAlias");
}

// Configures proxy aliases in config database
// PRE: none
// POST: Alias is added to proxyAlias list
bool vjInputManager::ConfigureProxyAlias(vjConfigChunk* chunk)
{
   vjDEBUG_BEGIN(1) << "vjInputManager::ConfigureProxyAlias" << endl << vjDEBUG_FLUSH;
   vjASSERT(((std::string)(char*)chunk->getType()) == "proxyAlias");

   std::string alias_name, proxy_ptr;  // The string of the alias, name of proxy to pt to

   alias_name = (std::string)(char*)chunk->getProperty("aliasName");
   proxy_ptr = (std::string)(char*)chunk->getProperty("proxyPtr");

   if(proxyAliases.end() == proxyAliases.find(proxy_ptr))
   {
      vjDEBUG(1) << "vjInputManager::ConfigureProxyAliases: Alias: " << alias_name
                 << "  cannot find proxy: " << proxy_ptr << endl << vjDEBUG_FLUSH;
      return false;
   } else {
         // Since all proxies are already in the alias list, we just have to find the
         // one to point to and use it's index
      proxyAliases[alias_name] = proxyAliases[proxy_ptr];
      vjDEBUG(0) << "   alias:" << alias_name << "   index:"
                 << proxyAliases[proxy_ptr] << endl << vjDEBUG_FLUSH;
   }
   vjDEBUG_END(1) << endl << vjDEBUG_FLUSH;
   return true;
}

//: Adds a Proxy alias to the alias list
// PRE: none - BUT: Preferable if proxyIndex  is for a valid proxy
// POST: Alias list has alias str refering to proxyIndex
// ARG: str: The string descriptor for the alias
//       NOTE: You can pass a char*, std::string has a copy contructor for
//             char*.
void vjInputManager::AddProxyAlias(std::string aliasStr, int proxyIndex)
{
   vjDEBUG(1) << "AddProxyAlias: alias:" << aliasStr << "   index:" << proxyIndex << endl << vjDEBUG_FLUSH;
   proxyAliases[aliasStr] = proxyIndex;
}

//: Gets index to the proxy/alias named by str
// PRE: none
// POST: none
// RETVAL: -1 if str not found
//         proxy index if found
int vjInputManager::GetProxyIndex(std::string proxyName)
{
   if(proxyAliases.end() == proxyAliases.find(proxyName))
      return -1;
   else
      return proxyAliases[proxyName];
}

