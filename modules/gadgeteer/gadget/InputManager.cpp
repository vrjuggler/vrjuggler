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
   m_dummyPosProxy->set(&m_dummyPos,0);
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
         m_devVector[a]->stopSampling();
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
void vjInputManager::configureInitial(vjConfigChunkDB *cdb)
{
   vjDEBUG(vjDBG_INPUT_MGR,6) << "vjInputManager::ConfigureInitial" << endl << vjDEBUG_FLUSH;
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
   vjDEBUG_BEGIN(vjDBG_INPUT_MGR,3) << "\nvjInputManager: Adding config info... " << endl << vjDEBUG_FLUSH;

   bool ret_val = false;      // Flag to return success

   if(vjDeviceFactory::instance()->recognizeDevice(chunk))
      ret_val = configureDevice(chunk);
   else if(vjProxyFactory::instance()->recognizeProxy(chunk))
      ret_val = configureProxy(chunk);
   else if(recognizeProxyAlias(chunk))
      ret_val = configureProxyAlias(chunk);

   //DumpStatus();                      // Dump the status
   if(ret_val)
   {
      updateAllData();                             // Update all the input data
      vjDeviceInterface::refreshAllDevices();      // Refresh all the device interface handles
      vjDEBUG(vjDBG_INPUT_MGR,4) << "Updated all devices" << endl << vjDEBUG_FLUSH;
   }

   vjDEBUG_END(vjDBG_INPUT_MGR,3) << endl << vjDEBUG_FLUSH;
   return ret_val;         // Return the success flag if we added at all
}


//: Remove the chunk from the current configuration
//! PRE: configCanHandle(chunk) == true
//! POST: (chunk is proxy) ==> (returns == false)
//+       (chunk is device) ==> (devices is removed && proxies are stupified)
//+       (chunk is proxyAlias) ==> (proxyAlias is removed && devInterfaces.refreshAll())
//!RETURNS: success
bool vjInputManager::configRemove(vjConfigChunk* chunk)
{
   vjDEBUG_BEGIN(vjDBG_INPUT_MGR,3) << "\nvjInputManager: Removing config... " << endl << vjDEBUG_FLUSH;

   bool ret_val = false;      // Flag to return success

   if(vjDeviceFactory::instance()->recognizeDevice(chunk))
      ret_val = removeDevice(chunk);
   else if(recognizeProxyAlias(chunk))
      ret_val = removeProxyAlias(chunk);
   else     //if(vjProxyFactory::instance()->recognizeProxy(chunk))
      ret_val = false;     // Chunk that is unrecognized (or a proxy)

   if(ret_val)
   {
      updateAllData();                             // Update all the input data
      vjDeviceInterface::refreshAllDevices();      // Refresh all the device interface handles
      vjDEBUG(vjDBG_INPUT_MGR,3) << "InputManager::configRemove(): Updated all data" << endl << vjDEBUG_FLUSH;
   }

   vjDEBUG_END(vjDBG_INPUT_MGR,3) << endl << vjDEBUG_FLUSH;
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
bool vjInputManager::configureDevice(vjConfigChunk* chunk)
{
   bool ret_val;
   std::string dev_name = chunk->getProperty("name");
   vjDEBUG_BEGIN(vjDBG_INPUT_MGR,4) << "ConfigureDevice: Named: " << dev_name << endl << vjDEBUG_FLUSH;


   vjInput* new_device;
   new_device = vjDeviceFactory::instance()->loadDevice(chunk);

   if ((new_device != NULL) && (new_device->startSampling()))
   {
      int dev_index = fAddDevice(new_device);
      ret_val = true;
      vjDEBUG(vjDBG_INPUT_MGR,4) << "   Successfully added dev: " << dev_name << "  at index:" << dev_index << endl << vjDEBUG_FLUSH;
   }
   else
   {
      vjDEBUG(vjDBG_ERROR,0) << "ERROR: new dev failed to start.. deleting instance" << endl << vjDEBUG_FLUSH;
      delete new_device;
      ret_val = false;
   }

   vjDEBUG_END(vjDBG_INPUT_MGR,4) << endl << vjDEBUG_FLUSH;
   return ret_val;
}

// Check if the device factory or proxy factory can handle the chunk
bool vjInputManager::configureProxy(vjConfigChunk* chunk)
{
   std::string proxy_name = chunk->getProperty("name");
   vjDEBUG_BEGIN(vjDBG_INPUT_MGR,4) << "ConfigureProxy: Named: " << proxy_name << endl << vjDEBUG_FLUSH;

   vjProxy* new_proxy;

   // Tell the factory to load the proxy
   // NOTE: The constructor for the proxy registers it with the input manager
   new_proxy = vjProxyFactory::instance()->loadProxy(chunk);

   if ( new_proxy != NULL)
   {
      vjDEBUG_END(vjDBG_INPUT_MGR,4) << "   Proxy is set" << endl << vjDEBUG_FLUSH;
      return true;
   }
   else
   {
      vjDEBUG(vjDBG_ERROR,0) << "ERROR: Could not configure proxy:" << proxy_name << endl << vjDEBUG_FLUSH;
      vjDEBUG_END(vjDBG_INPUT_MGR,4) << endl << vjDEBUG_FLUSH;
      return false;
   }
}


//: Remove the device associated with the given chunk
//!RETURNS: true - Device was removed
bool vjInputManager::removeDevice(vjConfigChunk* chunk)
{
   char* dev_name = chunk->getProperty("name").cstring();      // Get the name of the device
   return removeDevice(dev_name);
}


/**********************************************************
  operator<<()

  Dump the current Status of the vjInputManager, listing all
  the devices, proxies and internal settings

*********************************************** ahimberg */
ostream& operator<<(ostream& out, vjInputManager& iMgr)
{
  int i,d;
  out << "\n=============== vjInputManager Status: ===============================" << endl;

  out << "Devices:\n";

  for (i = 0; i < iMgr.m_devVector.size(); i++)      // Dump DEVICES
     if (iMgr.m_devVector[i] != NULL)
       out << setw(2) << i << ":"
           << "  name:" << setw(30) << iMgr.m_devVector[i]->getInstanceName()
	        << "  type:" << setw(12) << iMgr.m_devVector[i]->getDeviceName()
	        << "  port:" << setw(10) << iMgr.m_devVector[i]->getPort()
	        << "  baud:" << iMgr.m_devVector[i]->getBaudRate() << endl;

  out << "\nPos Proxies:\n";
  for (i = 0; i < iMgr.m_posProxyVector.size(); i++)    // DUMP PosProxies
  {
     d = iMgr.findDeviceNum( (iMgr.m_posProxyVector[i]->getPositionPtr())->getInstanceName() );
     if (d != -1)
       out << setw(2) << i << ":"
           << "   Proxies:"
	        << setw(10)
	        << (iMgr.m_posProxyVector[i]->getPositionPtr())->getInstanceName()
	        << "  unit:" <<
	             iMgr.m_posProxyVector[i]->getUnit() << endl;
  }

  out << "\nDigital Proxies\n";
  for (i = 0; i < iMgr.m_digProxyVector.size(); i++)                // Dump DigitalProxies
  {
     d = iMgr.findDeviceNum( (iMgr.m_digProxyVector[i]->getDigitalPtr())->getInstanceName() );
     if (d != -1)
       out << setw(2) << i << ":"
            << "   Proxies:"
	         << setw(10)
	         << (iMgr.m_digProxyVector[i]->getDigitalPtr())->getInstanceName()
            << "  unit:" <<
	             iMgr.m_digProxyVector[i]->getUnit() << endl;
  }

  out << "\nAnalog Proxies\n";
  for (i = 0; i < iMgr.m_anaProxyVector.size(); i++)                 // Dump Analog Proxies
  {
     d = iMgr.findDeviceNum( (iMgr.m_anaProxyVector[i]->getAnalogPtr())->getInstanceName() );
     if (d != -1)
       out << setw(2) << i << ":"
            << "   Proxies:"
	         << setw(10)
	         << (iMgr.m_anaProxyVector[i]->getAnalogPtr())->getInstanceName()
            << "  unit:" <<
	             iMgr.m_anaProxyVector[i]->getUnit() << endl;
  }

  out << "\nGlove Proxies:\n";
  for (i = 0; i < iMgr.m_gloveProxyVector.size(); i++)                 // Dump Glove Proxies
  {
     d = iMgr.findDeviceNum( (iMgr.m_gloveProxyVector[i]->getGlovePtr())->getInstanceName() );
     if (d != -1)
       out << setw(2) << i << ":"
            << "   Proxies:"
	         << setw(10)
	         << (iMgr.m_gloveProxyVector[i]->getGlovePtr())->getInstanceName()
        << "  unit:" <<
	             iMgr.m_gloveProxyVector[i]->getUnit() << endl;
  }

  out << "\nKeyboard Proxies\n";
  for (i = 0; i < iMgr.m_keyboardProxyVector.size(); i++)                 // Dump Keyboard Proxies
  {
     d = iMgr.findDeviceNum( (iMgr.m_keyboardProxyVector[i]->getKeyboardPtr())->getInstanceName() );
     if (d != -1)
       out << setw(2) << i << ":"
            << "   Proxies:"
	         << setw(10)
	         << (iMgr.m_keyboardProxyVector[i]->getKeyboardPtr())->getInstanceName()
	         << endl;
  }

  out << "\nGesture Proxies\n";
  for (i = 0; i < iMgr.m_gestureProxyVector.size(); i++)                 // Dump Gesture Proxies
  {
     d = iMgr.findDeviceNum( (iMgr.m_gestureProxyVector[i]->getGesturePtr())->getInstanceName() );
     if (d != -1)
       out << setw(2) << i << ":"
            << "   Proxies:"
	         << setw(10)
	         << (iMgr.m_gestureProxyVector[i]->getGesturePtr())->getInstanceName()
	         << endl;
  }

  out << endl;

      // Dump Alias list
  out << "Alias List:" << endl;
  for(std::map<std::string, int>::iterator cur_alias = iMgr.proxyAliases.begin(); cur_alias != iMgr.proxyAliases.end(); cur_alias++)
     out << "    " << (*cur_alias).first << "  index:" << (*cur_alias).second << endl;
	
  out << "=============== vjInputManager Status =========================" << endl;
  return out;
}

/**********************************************************
  vjInputManager::fAddDevice(vjInput* devPtr)

  Add a device to the vjInputManager, returns the index
  where the device was placed

*********************************************** ahimberg */
int vjInputManager::fAddDevice(vjInput* devPtr)
{
   m_devVector.push_back(devPtr);
   return (m_devVector.size()-1);   // ASSERT: This is the index we placed
}



/**********************************************************
  vjInputManager::updateAllData()

  Call UpdateData() on all the devices and transform proxies

*********************************************** ahimberg */
void vjInputManager::updateAllData()
{
   int j;
   for (j = 0; j < m_devVector.size(); j++)
      if (m_devVector[j] != NULL)
         m_devVector[j]->updateData();

      // The posProxies copy the data locally and may do a transform, so
      // need to call their UpdateData function, too
      // NOTE: Glove prxies don't need UpdateData()
   for (j = 0; j < m_posProxyVector.size(); j++)
      m_posProxyVector[j]->updateData();
   for (j = 0; j < m_digProxyVector.size(); j++)
      m_digProxyVector[j]->updateData();
   for (j = 0; j < m_anaProxyVector.size(); j++)
      m_anaProxyVector[j]->updateData();

}


/**********************************************************
  vjInputManager::findDeviceNum(char* instName)

  Find a devive Number from the instance name of a device,
  if the instance name is not in the device array returns -1

*********************************************** ahimberg */
int vjInputManager::findDeviceNum(const char* instName)
{
   for (int i = 0; i < m_devVector.size(); i++)
   {
      if (m_devVector[i] != NULL)
         if ((strcasecmp(m_devVector[i]->getInstanceName(),instName) == 0)
             && (strcasecmp(m_devVector[i]->getInstanceName(),"Undefined") != 0))
            return i;
   }
   return -1;
}


/**********************************************************
  vjInputManager::getDevice(int devNum)

  Get the vjInput pointer at a device,
  NOTE: this can return a null pointer if there is not a
	device at devNum

*********************************************** ahimberg */
vjInput* vjInputManager::getDevice(int devNum)
{
    assert (devNum < (m_devVector.size() -1));
    //assert (m_devVector[devNum] != NULL);

    return m_devVector[devNum];
}


/**********************************************************
  vjInputManager::removeDevice(int devNum)

  vjInputManager remove devNum from the vjInputManager,
  currently stupifies all the proxies connected to it.

*********************************************** ahimberg */
bool vjInputManager::removeDevice(int devNum)
{
   int i;
   if (m_devVector[devNum] == NULL)    // Check for valid device
      return false;

   // Find any proxies connected to the device
   // Stupify any proxies connected to device
   for (i = 0; i < m_posProxyVector.size(); i++)         // Pos devices
      if (m_posProxyVector[i]->getProxiedInputDevice() == m_devVector[devNum])
         stupifyPos(i);
   for (i = 0; i < m_digProxyVector.size(); i++)         // Digital devices
      if (m_digProxyVector[i]->getProxiedInputDevice() == m_devVector[devNum])
         stupifyDig(i);
   for (i = 0; i < m_anaProxyVector.size(); i++)         // Analog devices
      if (m_anaProxyVector[i]->getProxiedInputDevice() == m_devVector[devNum])
         stupifyAna(i);
   for (i = 0; i < m_gloveProxyVector.size(); i++)       // Glove devices
      if (m_gloveProxyVector[i]->getProxiedInputDevice() == m_devVector[devNum])
         stupifyGlove(i);
   for (i = 0; i < m_keyboardProxyVector.size(); i++)       // Glove devices
      if (m_keyboardProxyVector[i]->getProxiedInputDevice() == m_devVector[devNum])
         stupifyKeyboard(i);
   for (i = 0; i < m_gestureProxyVector.size(); i++)       // Glove devices
      if (m_gestureProxyVector[i]->getProxiedInputDevice() == m_devVector[devNum])
         stupifyGesture(i);

      // stop the device, delete it, set pointer to NULL
   m_devVector[devNum]->stopSampling();
   delete m_devVector[devNum];
   m_devVector[devNum] = NULL;

   return 1;
}

/**********************************************************
  vjInputManager::removeDevice(char* instName)

  vjInputManager remove instName from the vjInputManager,
  currently stupifies all the proxies connected to it.

*********************************************** ahimberg */
bool vjInputManager::removeDevice(char* instName)
{
   // find the device
   int i = findDeviceNum(instName);

   // -1 means not found, so fail
   if (i == -1)
      return false;
   else // Let removeDevice(int devNum) takeover
      return (removeDevice(i) );

}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * *
  * * * * * * * * * * * * * * * * * * * * * * * * * * * *
		POSITION PROXY FUNCTIONS
* * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * */


/**********************************************************
  vjInputManager::setPosProxy(int ProxyNum, int DevNum, int subNum)

  Set the position proxy #ProxyNum to be hooked to
                  device #DevNum at sub-unit #subNum

*********************************************** ahimberg */	
int vjInputManager::setPosProxy(int ProxyNum, int DevNum, int subNum)
{
   m_posProxyVector[ProxyNum]->set(dynamic_cast<vjPosition*>(m_devVector[DevNum]),subNum);
   return 1;
}

//: Add the pos proxy
//! POST: pos proxy has been added
//+   proxy alias has been set
//! RETURNS: -1: failure, >0: proxy_num
int vjInputManager::addPosProxy(std::string devName, int subNum, std::string proxyName, vjPosProxy* posProxy)
{
   int dev_num = findDeviceNum(devName.c_str());              // Get device number
   if(-1 == dev_num)       // Not found, ERROR
      return -1;

   vjDEBUG(vjDBG_INPUT_MGR,1) << "adding a posProxy : " << proxyName << endl << vjDEBUG_FLUSH;
   vjDEBUG(vjDBG_INPUT_MGR,1) << "   attaching to device named: " << devName << endl << vjDEBUG_FLUSH;
   vjDEBUG(vjDBG_INPUT_MGR,1) << "   at unit number: " << subNum << endl << vjDEBUG_FLUSH;
   vjDEBUG(vjDBG_INPUT_MGR,1) << "	Found the device at devNum: " << dev_num << endl << vjDEBUG_FLUSH;

   posProxy->set(dynamic_cast<vjPosition*>(m_devVector[dev_num]),subNum);
   m_posProxyVector.push_back(posProxy);

   int proxy_num = (m_posProxyVector.size() -1);         // Get proxy num
   addProxyAlias(proxyName, proxy_num);                  // Add alias

   return proxy_num;
}

/**********************************************************
  vjInputManager::stupifyPos(int ProxyNum)

  Make Position Proxy #ProxyNum point back to the dummyPos

*********************************************** ahimberg */
void vjInputManager::stupifyPos(int ProxyNum)
{
   m_posProxyVector[ProxyNum]->set(&m_dummyPos,0);
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * *
  * * * * * * * * * * * * * * * * * * * * * * * * * * * *
		DIGITAL PROXY FUNCTIONS
* * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * */


/**********************************************************
  vjInputManager::setDigProxy(int ProxyNum, int DevNum, int subNum)

  Set the digital proxy number #ProxyNum to be hooked up to
            device number #devNum at sub-unit #subNum

*********************************************** ahimberg */
int vjInputManager::setDigProxy(int ProxyNum, int DevNum, int subNum)
{
   m_digProxyVector[ProxyNum]->set(dynamic_cast<vjDigital*>(m_devVector[DevNum]),subNum);
   return 1;
}

//: Add the digital proxy
//! POST: dig proxy has been added
//+   proxy alias has been set
//! RETURNS: -1: failure, >0: proxy_num
int vjInputManager::addDigProxy(std::string devName, int subNum, std::string proxyName, vjDigitalProxy* digitalProxy)
{
   int dev_num = findDeviceNum(devName.c_str());              // Get device number
   if(-1 == dev_num)       // Not found, ERROR
      return -1;

   vjDEBUG(vjDBG_INPUT_MGR,1) << "adding a digProxy : " << proxyName << endl << vjDEBUG_FLUSH;
   vjDEBUG(vjDBG_INPUT_MGR,1) << "   attaching to device named: " << devName << endl << vjDEBUG_FLUSH;
   vjDEBUG(vjDBG_INPUT_MGR,1) << "   at unit number: " << subNum << endl << vjDEBUG_FLUSH;
   vjDEBUG(vjDBG_INPUT_MGR,1) << "	Found the device at devNum: " << dev_num << endl << vjDEBUG_FLUSH;

   digitalProxy->set(dynamic_cast<vjDigital*>(m_devVector[dev_num]),subNum);      // Set the proxy
   m_digProxyVector.push_back(digitalProxy);                                     // Add to proxy array

   int proxy_num = (m_digProxyVector.size() - 1);        // Get proxy num
   addProxyAlias(proxyName, proxy_num);                  // Add the alias

   return proxy_num;
}

/**********************************************************
  vjInputManager::stupifyDig(int ProxyNum)

  Point Digital Proxy #ProxyNum to the dummyDigital of the
  vjInputManager

*********************************************** ahimberg */
void vjInputManager::stupifyDig(int ProxyNum)
{
   m_digProxyVector[ProxyNum]->set(&m_dummyDig,0);
}

	

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * *
  * * * * * * * * * * * * * * * * * * * * * * * * * * * *
		ANALOG PROXY FUNCTIONS
* * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * */


/**********************************************************
  vjInputManager::setAnaProxy(int ProxyNum, int DevNum, int subNum)

 Set the analog proxy #ProxyNum to be hooked to
                  device #DevNum at sub-unit #subNum

*********************************************** ahimberg */
int vjInputManager::setAnaProxy(int ProxyNum, int DevNum, int subNum)
{
   m_anaProxyVector[ProxyNum]->set(dynamic_cast<vjAnalog*>(m_devVector[DevNum]),subNum);
   return 1;
}

//: Add the analog proxy
//! POST: analog proxy has been added
//+   proxy alias has been set
//! RETURNS: -1: failure, >0: proxy_num
int vjInputManager::addAnaProxy(std::string devName, int subNum, std::string proxyName, vjAnalogProxy* anaProxy)
{
   int dev_num = findDeviceNum(devName.c_str());              // Get device number
   if(-1 == dev_num)       // Not found, ERROR
      return -1;

   vjDEBUG(vjDBG_INPUT_MGR,1) << "adding a anaProxy : " << proxyName << endl << vjDEBUG_FLUSH;
   vjDEBUG(vjDBG_INPUT_MGR,1) << "   attaching to device named: " << devName << endl << vjDEBUG_FLUSH;
   vjDEBUG(vjDBG_INPUT_MGR,1) << "   at unit number: " << subNum << endl << vjDEBUG_FLUSH;
   vjDEBUG(vjDBG_INPUT_MGR,1) << "	Found the device at devNum: " << dev_num << endl << vjDEBUG_FLUSH;

   anaProxy->set(dynamic_cast<vjAnalog*>(m_devVector[dev_num]),subNum);    // Set the proxy
   m_anaProxyVector.push_back(anaProxy);                                  // Add to proxy array

   int proxy_num = (m_anaProxyVector.size() -1);            // Get proxy num
   addProxyAlias(proxyName, proxy_num);                     // Add the alias

   return proxy_num;
}

/**********************************************************
  vjInputManager::stupifyAna(int ProxyNum)

  Point Analog Proxy #ProxyNum to the dummyAnalog of the
  vjInputManager

*********************************************** ahimberg */
void vjInputManager::stupifyAna(int ProxyNum)
{
   m_anaProxyVector[ProxyNum]->set(&m_dummyAna,0);
}



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * *
  * * * * * * * * * * * * * * * * * * * * * * * * * * * *
		GLOVE PROXY FUNCTIONS
* * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * */

//: Set the glove proxy #ProxyNum to be hooked to
//+ device #DevNum at sub-unit #subNum
int vjInputManager::setGloveProxy(int ProxyNum, int DevNum, int subNum)
{
   m_gloveProxyVector[ProxyNum]->set(dynamic_cast<vjGlove*>(m_devVector[DevNum]),subNum);
   return 1;
}

//: Add the glove proxy
//! POST: glove proxy has been added
//+   proxy alias has been set
//! RETURNS: -1: failure, >0: proxy_num
int vjInputManager::addGloveProxy(std::string devName, int subNum, std::string proxyName, vjGloveProxy* gloveProxy)
{
   int dev_num = findDeviceNum(devName.c_str());              // Get device number
   if(-1 == dev_num)       // Not found, ERROR
      return -1;

   vjDEBUG(vjDBG_INPUT_MGR,1) << "adding a gloveProxy : " << proxyName << endl << vjDEBUG_FLUSH;
   vjDEBUG(vjDBG_INPUT_MGR,1) << "   attaching to device named: " << devName << endl << vjDEBUG_FLUSH;
   vjDEBUG(vjDBG_INPUT_MGR,1) << "   at unit number: " << subNum << endl << vjDEBUG_FLUSH;
   vjDEBUG(vjDBG_INPUT_MGR,1) << "	Found the device at devNum: " << dev_num << endl << vjDEBUG_FLUSH;

   gloveProxy->set(dynamic_cast<vjGlove*>(m_devVector[dev_num]),subNum);       // Set the proxy
   m_gloveProxyVector.push_back(gloveProxy);                                  // Add to proxy array

   int proxy_num = (m_gloveProxyVector.size() -1);       // Get proxy num
   addProxyAlias(proxyName, proxy_num);                  // Add the alias

   return proxy_num;
}


//: Make glove Proxy #ProxyNum point back to the dummyGlove
void vjInputManager::stupifyGlove(int ProxyNum)
{
   m_gloveProxyVector[ProxyNum]->set(&m_dummyGlove,0);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * *
  * * * * * * * * * * * * * * * * * * * * * * * * * * * *
		KEYBOARD PROXY FUNCTIONS
* * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * */
//: Set the keyboard proxy #ProxyNum to be hooked to
//+ device #DevNum
int vjInputManager::setKeyboardProxy(int ProxyNum, int DevNum)
{
   m_keyboardProxyVector[ProxyNum]->set(dynamic_cast<vjKeyboard*>(m_devVector[DevNum]));
   return 1;
}

//: Add the keyboard proxy
//! POST: keyboard proxy has been added
//+   proxy alias has been set
//! RETURNS: -1: failure, >0: proxy_num
int vjInputManager::addKeyboardProxy(std::string devName, int subNum, std::string proxyName, vjKeyboardProxy* kbProxy)
{
   int dev_num = findDeviceNum(devName.c_str());              // Get device number
   if(-1 == dev_num)       // Not found, ERROR
      return -1;

   vjDEBUG(vjDBG_INPUT_MGR,1) << "adding a keyboard Proxy : " << proxyName << endl << vjDEBUG_FLUSH;
   vjDEBUG(vjDBG_INPUT_MGR,1) << "   attaching to device named: " << devName << endl << vjDEBUG_FLUSH;
   vjDEBUG(vjDBG_INPUT_MGR,1) << "   at unit number: " << subNum << endl << vjDEBUG_FLUSH;
   vjDEBUG(vjDBG_INPUT_MGR,1) << "	Found the device at devNum: " << dev_num << endl << vjDEBUG_FLUSH;

   vjKeyboard* theKB = dynamic_cast<vjKeyboard*>(m_devVector[dev_num]);
   vjASSERT(theKB != NULL);
   kbProxy->set(theKB);                                  // Set the proxy
   m_keyboardProxyVector.push_back(kbProxy);        // Add to proxy array

   int proxy_num = (m_keyboardProxyVector.size() -1);    // Get proxy num
   addProxyAlias(proxyName, proxy_num);                  // Add the alias

   return proxy_num;
}

//: Make keyboard Proxy #ProxyNum point back to the dummyKeyboard
void vjInputManager::stupifyKeyboard(int ProxyNum)
{
   m_keyboardProxyVector[ProxyNum]->set(&m_dummyKeyboard);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * *
  * * * * * * * * * * * * * * * * * * * * * * * * * * * *
		GESTURE PROXY FUNCTIONS
* * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * */
//: Set the gesture proxy
// Hook devNum to proxyNum
int vjInputManager::setGestureProxy(int ProxyNum, int DevNum)
{
   m_gestureProxyVector[ProxyNum]->set(dynamic_cast<vjGesture*>(m_devVector[DevNum]));
   return 1;
}

//: Add the gesture proxy
//! POST: gesture proxy has been added
//+   proxy alias has been set
//! RETURNS: -1: failure, >0: proxy_num
int vjInputManager::addGestureProxy(std::string devName, int subNum, std::string proxyName, vjGestureProxy* gestureProxy)
{
   int dev_num = findDeviceNum(devName.c_str());              // Get device number
   if(-1 == dev_num)       // Not found, ERROR
      return -1;

   vjDEBUG(vjDBG_INPUT_MGR,1) << "adding a anaProxy : " << proxyName << endl << vjDEBUG_FLUSH;
   vjDEBUG(vjDBG_INPUT_MGR,1) << "   attaching to device named: " << devName << endl << vjDEBUG_FLUSH;
   vjDEBUG(vjDBG_INPUT_MGR,1) << "   at unit number: " << subNum << endl << vjDEBUG_FLUSH;
   vjDEBUG(vjDBG_INPUT_MGR,1) << "	Found the device at devNum: " << dev_num << endl << vjDEBUG_FLUSH;

   gestureProxy->set(dynamic_cast<vjGesture*>(m_devVector[dev_num]));
   m_gestureProxyVector.push_back(gestureProxy);

   int proxy_num = (m_gestureProxyVector.size() -1);        // Get proxy num
   addProxyAlias(proxyName, proxy_num);                     // Add the alias

   return proxy_num;
}

//: Turn the gesture proxy at index ProxyNum to point back
// to the default dummy gesture proxy. <br>
// <br>
//! MODIFIES: self <br>
//! POST:  m_gestureProxyArray[proxyNum]' = m_gestureKeyboard
void vjInputManager::stupifyGesture(int ProxyNum)
{
   m_gestureProxyVector[ProxyNum]->set(&m_dummyGesture);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * *
  * * * * * * * * * * * * * * * * * * * * * * * * * * * *
	PRIVATE FUNCTIONS TO INPUTGROUP
* * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * */


// Is it a proxy alias
bool recognizeProxyAlias(vjConfigChunk* chunk)
{
   return (((std::string)chunk->getType()) == "proxyAlias");
}

// Configures proxy aliases in config database
// PRE: none
// POST: (alias not already in list) ==> Alias is added to proxyAlias list
//+      (alias was already is list) ==> Alias is set to point to the new proxy instead
bool vjInputManager::configureProxyAlias(vjConfigChunk* chunk)
{
   vjDEBUG_BEGIN(vjDBG_INPUT_MGR,3) << "vjInputManager::Configuring proxy alias" << endl << vjDEBUG_FLUSH;
   vjASSERT(((std::string)chunk->getType()) == "proxyAlias");

   std::string alias_name, proxy_name;  // The string of the alias, name of proxy to pt to

   alias_name = (std::string)chunk->getProperty("aliasName");
   proxy_name = (std::string)chunk->getProperty("proxyPtr");

   if(proxyAliases.end() == proxyAliases.find(proxy_name))
   {
      vjDEBUG(vjDBG_ERROR,0) << "vjInputManager::ConfigureProxyAliases: Alias: " << alias_name
                 << "  cannot find proxy: " << proxy_name << endl << vjDEBUG_FLUSH;
      return false;
   } else {
         // Since all proxies are already in the alias list, we just have to find the
         // one to point to and use it's index
      proxyAliases[alias_name] = proxyAliases[proxy_name];
      vjDEBUG(vjDBG_INPUT_MGR,1) << "   alias:" << alias_name << "   index:"
                 << proxyAliases[proxy_name] << endl << vjDEBUG_FLUSH;
   }
   vjDEBUG_END(vjDBG_INPUT_MGR,3) << endl << vjDEBUG_FLUSH;
   return true;
}

// Remove a proxy aliases in config database
// PRE: none
// POST: (alias not in list) ==> returns = false
//+      (alias is in list) ==> (alias is removed from list) returns true
bool vjInputManager::removeProxyAlias(vjConfigChunk* chunk)
{
   vjDEBUG_BEGIN(vjDBG_INPUT_MGR,3) << "vjInputManager::RemoveProxyAlias" << endl << vjDEBUG_FLUSH;
   vjASSERT(((std::string)chunk->getType()) == "proxyAlias");

   std::string alias_name, proxy_name;  // The string of the alias, name of proxy to pt to

   alias_name = (std::string)chunk->getProperty("aliasName");

   if(proxyAliases.end() == proxyAliases.find(alias_name))
   {
      vjDEBUG(vjDBG_ERROR,0) << "vjInputManager::RemoveProxyAlias: Alias: " << alias_name
                 << "  cannot find proxy: " << proxy_name << endl << vjDEBUG_FLUSH;
      return false;
   } else {
      proxyAliases.erase(alias_name);
      vjDEBUG(vjDBG_INPUT_MGR,1) << "   alias:" << alias_name << "   index:"
                 << proxyAliases[proxy_name] << "  has been removed." << endl << vjDEBUG_FLUSH;
   }
   vjDEBUG_END(vjDBG_INPUT_MGR,3) << endl << vjDEBUG_FLUSH;
   return true;
}


//: Adds a Proxy alias to the alias list
// PRE: none - BUT: Preferable if proxyIndex  is for a valid proxy
// POST: Alias list has alias str refering to proxyIndex
// ARG: str: The string descriptor for the alias
//       NOTE: You can pass a char*, std::string has a copy contructor for
//             char*.
void vjInputManager::addProxyAlias(std::string aliasStr, int proxyIndex)
{
   vjDEBUG(vjDBG_INPUT_MGR,1) << "AddProxyAlias: alias:" << aliasStr << "   index:" << proxyIndex << endl << vjDEBUG_FLUSH;
   proxyAliases[aliasStr] = proxyIndex;
}

//: Gets index to the proxy/alias named by str
// PRE: none
// POST: none
// RETVAL: -1 if str not found
//         proxy index if found
int vjInputManager::getProxyIndex(std::string proxyName)
{
   if(proxyAliases.end() == proxyAliases.find(proxyName))
      return -1;
   else
      return proxyAliases[proxyName];
}

