/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998, 1999, 2000 by Iowa State University
 *
 * Original Authors:
 *   Allen Bierbaum, Christopher Just,
 *   Patrick Hartling, Kevin Meinert,
 *   Carolina Cruz-Neira, Albert Baker
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 *
 * -----------------------------------------------------------------
 * File:          $RCSfile$
 * Date modified: $Date$
 * Version:       $Revision$
 * -----------------------------------------------------------------
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/


#include <vjConfig.h>

#include <Input/InputManager/vjInputManager.h>
#include <Kernel/vjDebug.h>

// Sims
//#include <Input/vjSim/vjSimAnalog.h>
//#include <Input/vjSim/vjSimDigital.h>
//#include <Input/vjSim/vjSimPosition.h>
//#include <Input/vjSim/vjSimGloveGesture.h>

// Physical devices
#ifndef WIN32
//#include <Input/vjPosition/vjFlock.h>
//#include <Input/vjInput/vjIbox.h>
//#include <Input/vjGlove/vjCyberGlove.h>
//#include <Input/vjKeyboard/vjXWinKeyboard.h>
//#include <Input/vjPosition/logiclass.h>
#else
//#include <Input/vjKeyboard/vjKeyboardWin32.h>
#endif

#include <Input/InputManager/vjDeviceFactory.h>
#include <Input/InputManager/vjProxyFactory.h>

// Local helpers
bool recognizeProxyAlias(vjConfigChunk* chunk);

/**********************************************************
  vjInputManager::vjInputManager()

  vjInputManager Constructor

*********************************************** ahimberg */
vjInputManager::vjInputManager()
:  m_dummyPos(),
   m_dummyDig(),
   m_dummyAna(),
   m_dummyGlove(),
   m_dummyKeyboard(),
   m_dummyGesture(),
   m_dummyPosProxy(NULL),
   m_dummyDigitalProxy(NULL),
   m_dummyAnalogProxy(NULL),
   m_dummyGloveProxy(NULL),
   m_dummyKeyboardProxy(NULL),
   m_dummyGestureProxy(NULL)
{
   // Setup all the dummies and their proxies
   // XXX: This should really be moved to the dev interfaces when the new input system is written
   m_dummyPosProxy = new vjPosProxy();      // Create dummy proxy
   m_dummyPosProxy->set(&m_dummyPos,0);
   m_dummyGlove.setGlovePosProxy(m_dummyPosProxy);       // So dummy glove has a location
   m_dummyDigitalProxy = new vjDigitalProxy();
   m_dummyDigitalProxy->set(&m_dummyDig,0);
   m_dummyAnalogProxy = new vjAnalogProxy();
   m_dummyAnalogProxy->set(&m_dummyAna,0);
   m_dummyGloveProxy = new vjGloveProxy();
   m_dummyGloveProxy->set(&m_dummyGlove,0);
   m_dummyKeyboardProxy = new vjKeyboardProxy();
   m_dummyKeyboardProxy->set(&m_dummyKeyboard);
   m_dummyGestureProxy = new vjGestureProxy();
   m_dummyGestureProxy->set(&m_dummyGesture);
}


/**********************************************************
  vjInputManager::~vjInputManager()

  vjInputManager Destructor

*********************************************** ahimberg */
vjInputManager::~vjInputManager()
{
   for (unsigned int a = 0; a < m_devVector.size(); a++)    // Stop all devices
      if (m_devVector[a] != NULL)
         m_devVector[a]->stopSampling();
   for (unsigned int i = 0; i < m_devVector.size(); i++)    // Delete all devices
      if (m_devVector[i] != NULL)
         delete m_devVector[i];
   for (unsigned int j = 0; j < m_posProxyVector.size(); j++)  // Delete all pos proxies
      if (m_posProxyVector[j] != NULL)
         delete m_posProxyVector[j];
   for (unsigned int k = 0; k < m_digProxyVector.size(); k++)  // Delete all digital proxies
      if (m_digProxyVector[k] != NULL)
         delete m_digProxyVector[k];
   for (unsigned int l = 0; l < m_anaProxyVector.size(); l++)  // Delete all analog proxies
      if (m_anaProxyVector[l] != NULL)
         delete m_anaProxyVector[l];
   for (unsigned int m = 0; m < m_gloveProxyVector.size(); m++)  // Delete all glove proxies
      if (m_gloveProxyVector[m] != NULL)
         delete m_gloveProxyVector[m];
   for (unsigned int n = 0; n < m_gestureProxyVector.size(); n++)  // Delete all gesture proxies
      if (m_gestureProxyVector[n] != NULL)
         delete m_gestureProxyVector[n];
}

//: Set the initial configuration
// Finds the InputManager chunks in the db
// uses these to configure dummyPos
/*
void vjInputManager::configureInitial(vjConfigChunkDB *cdb)
{
   vjDEBUG(vjDBG_INPUT_MGR,6) << "vjInputManager::ConfigureInitial" << std::endl << vjDEBUG_FLUSH;

   if(cdb == NULL)
   {
      vjDEBUG(vjDBG_INPUT_MGR,0) << "vjInputManager::ConfigureInitial: NULL config file.  Can configure.\n" << vjDEBUG_FLUSH;
      return;
   }

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
*/

//: Add the given config chunk to the input system
bool vjInputManager::configAdd(vjConfigChunk* chunk)
{
   vjDEBUG_BEGIN(vjDBG_INPUT_MGR,vjDBG_STATE_LVL) << "\nvjInputManager: Adding pending config chunk... " << std::endl << vjDEBUG_FLUSH;
   vjASSERT(configCanHandle(chunk));

   bool ret_val = false;      // Flag to return success

   if(vjDeviceFactory::instance()->recognizeDevice(chunk))
      ret_val = configureDevice(chunk);
   else if(vjProxyFactory::instance()->recognizeProxy(chunk))
      ret_val = configureProxy(chunk);
   else if(recognizeProxyAlias(chunk))
      ret_val = configureProxyAlias(chunk);

   //DumpStatus();                      // Dump the status
   vjDEBUG_BEGIN(vjDBG_INPUT_MGR,vjDBG_VERB_LVL) << "New input manager state:\n" << vjDEBUG_FLUSH;
   vjDEBUG(vjDBG_INPUT_MGR,vjDBG_VERB_LVL) << (*this) << vjDEBUG_FLUSH;
   vjDEBUG_END(vjDBG_INPUT_MGR,vjDBG_VERB_LVL) << std::endl << vjDEBUG_FLUSH;

   if(ret_val)
   {
      updateAllData();                             // Update all the input data
      vjDeviceInterface::refreshAllDevices();      // Refresh all the device interface handles
      vjDEBUG(vjDBG_INPUT_MGR,vjDBG_STATE_LVL) << "Updated all devices" << std::endl << vjDEBUG_FLUSH;
   }

   vjDEBUG_END(vjDBG_INPUT_MGR,vjDBG_STATE_LVL) << std::endl << vjDEBUG_FLUSH;
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
   vjDEBUG_BEGIN(vjDBG_INPUT_MGR,vjDBG_STATE_LVL) << "\nvjInputManager: Removing config... " << std::endl << vjDEBUG_FLUSH;
   vjASSERT(configCanHandle(chunk));

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
      vjDEBUG(vjDBG_INPUT_MGR,vjDBG_VERB_LVL) << "InputManager::configRemove(): Updated all data" << std::endl << vjDEBUG_FLUSH;
   }

   vjDEBUG_END(vjDBG_INPUT_MGR,vjDBG_STATE_LVL) << std::endl << vjDEBUG_FLUSH;
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
   vjDEBUG_BEGIN(vjDBG_INPUT_MGR,vjDBG_STATE_LVL) << "ConfigureDevice: Named: " << dev_name.c_str() << std::endl << vjDEBUG_FLUSH;

   vjInput* new_device;
   new_device = vjDeviceFactory::instance()->loadDevice(chunk);

   if ((new_device != NULL) && (new_device->startSampling()))
   {
      int dev_index = fAddDevice(new_device);
      ret_val = true;
      vjDEBUG(vjDBG_INPUT_MGR,vjDBG_STATE_LVL) << "   Successfully added dev: " << dev_name.c_str() << "  at index:" << dev_index << std::endl << vjDEBUG_FLUSH;
   }
   else
   {
      vjDEBUG(vjDBG_ERROR,vjDBG_CRITICAL_LVL) << clrOutNORM(clrRED,"ERROR:")
                  << "new dev " << clrSetBOLD(clrCYAN) << dev_name.c_str() << clrRESET << " failed to start.. deleting instance" << std::endl << vjDEBUG_FLUSH;
      delete new_device;
      ret_val = false;
   }

   vjDEBUG_END(vjDBG_INPUT_MGR,vjDBG_STATE_LVL) << std::endl << vjDEBUG_FLUSH;
   return ret_val;
}

// Check if the device factory or proxy factory can handle the chunk
bool vjInputManager::configureProxy(vjConfigChunk* chunk)
{
   std::string proxy_name = chunk->getProperty("name");
   vjDEBUG_BEGIN(vjDBG_INPUT_MGR,vjDBG_STATE_LVL) << "vjInputManager::configureProxy: Named: " << proxy_name.c_str() << std::endl << vjDEBUG_FLUSH;

   vjProxy* new_proxy;

   // Tell the factory to load the proxy
   // NOTE: The constructor for the proxy registers it with the input manager
   new_proxy = vjProxyFactory::instance()->loadProxy(chunk);

   if ( new_proxy != NULL)
   {
      vjDEBUG_END(vjDBG_INPUT_MGR,vjDBG_STATE_LVL) << "   Proxy is set" << std::endl << vjDEBUG_FLUSH;
      return true;
   }
   else
   {
      vjDEBUG(vjDBG_ERROR,vjDBG_CRITICAL_LVL) << clrOutNORM(clrRED,"ERROR:") << "vjInputManager::configureProxy: Proxy construction failed:" << proxy_name.c_str() << std::endl << vjDEBUG_FLUSH;
      vjDEBUG_END(vjDBG_INPUT_MGR,vjDBG_STATE_LVL) << std::endl << vjDEBUG_FLUSH;
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
std::ostream& operator<<(std::ostream& out, vjInputManager& iMgr)
{
  unsigned int i;
  int d;
  out << "\n=============== vjInputManager Status: ===============================" << std::endl;

  out << "Devices:\n";

  for (i = 0; i < iMgr.m_devVector.size(); i++)      // Dump DEVICES
     if (iMgr.m_devVector[i] != NULL)
       out << std::setw(2) << std::setfill(' ') << i << ":"
           << "  name:" << std::setw(30) << iMgr.m_devVector[i]->getInstanceName()
           << "  type:" << std::setw(12) << iMgr.m_devVector[i]->getDeviceName()
           << "  port:" << std::setw(10) << iMgr.m_devVector[i]->getPort()
           << "  baud:" << iMgr.m_devVector[i]->getBaudRate() << std::endl;

  out << "\nPos Proxies:\n";
  for (i = 0; i < iMgr.m_posProxyVector.size(); i++)    // DUMP PosProxies
  {
     d = iMgr.findDeviceNum( (iMgr.m_posProxyVector[i]->getPositionPtr())->getInstanceName() );
     if (d != -1)
       out << std::setw(2) << i << ":"
           << "   Proxies:"
           << std::setw(10)
           << (iMgr.m_posProxyVector[i]->getPositionPtr())->getInstanceName()
           << "  unit:" <<
                iMgr.m_posProxyVector[i]->getUnit() << std::endl;
  }

  out << "\nDigital Proxies\n";
  for (i = 0; i < iMgr.m_digProxyVector.size(); i++)                // Dump DigitalProxies
  {
     d = iMgr.findDeviceNum( (iMgr.m_digProxyVector[i]->getDigitalPtr())->getInstanceName() );
     if (d != -1)
       out << std::setw(2) << i << ":"
            << "   Proxies:"
            << std::setw(10)
            << (iMgr.m_digProxyVector[i]->getDigitalPtr())->getInstanceName()
            << "  unit:" <<
                iMgr.m_digProxyVector[i]->getUnit() << std::endl;
  }

  out << "\nAnalog Proxies\n";
  for (i = 0; i < iMgr.m_anaProxyVector.size(); i++)                 // Dump Analog Proxies
  {
     d = iMgr.findDeviceNum( (iMgr.m_anaProxyVector[i]->getAnalogPtr())->getInstanceName() );
     if (d != -1)
       out << std::setw(2) << i << ":"
            << "   Proxies:"
            << std::setw(10)
            << (iMgr.m_anaProxyVector[i]->getAnalogPtr())->getInstanceName()
            << "  unit:" <<
                iMgr.m_anaProxyVector[i]->getUnit() << std::endl;
  }

  out << "\nGlove Proxies:\n";
  for (i = 0; i < iMgr.m_gloveProxyVector.size(); i++)                 // Dump Glove Proxies
  {
     d = iMgr.findDeviceNum( (iMgr.m_gloveProxyVector[i]->getGlovePtr())->getInstanceName() );
     if (d != -1)
       out << std::setw(2) << i << ":"
            << "   Proxies:"
            << std::setw(10)
            << (iMgr.m_gloveProxyVector[i]->getGlovePtr())->getInstanceName()
        << "  unit:" <<
                iMgr.m_gloveProxyVector[i]->getUnit() << std::endl;
  }

  out << "\nKeyboard Proxies\n";
  for (i = 0; i < iMgr.m_keyboardProxyVector.size(); i++)                 // Dump Keyboard Proxies
  {
     d = iMgr.findDeviceNum( (iMgr.m_keyboardProxyVector[i]->getKeyboardPtr())->getInstanceName() );
     if (d != -1)
       out << std::setw(2) << i << ":"
            << "   Proxies:"
            << std::setw(10)
            << (iMgr.m_keyboardProxyVector[i]->getKeyboardPtr())->getInstanceName()
            << std::endl;
  }

  out << "\nGesture Proxies\n";
  for (i = 0; i < iMgr.m_gestureProxyVector.size(); i++)                 // Dump Gesture Proxies
  {
     d = iMgr.findDeviceNum( (iMgr.m_gestureProxyVector[i]->getGesturePtr())->getInstanceName() );
     if (d != -1)
       out << std::setw(2) << i << ":"
            << "   Proxies:"
            << std::setw(10)
            << (iMgr.m_gestureProxyVector[i]->getGesturePtr())->getInstanceName()
            << std::endl;
  }

  out << std::endl;

      // Dump Alias list
  out << "Alias List:" << std::endl;
  for(std::map<std::string, int>::iterator cur_alias = iMgr.proxyAliases.begin(); cur_alias != iMgr.proxyAliases.end(); cur_alias++)
     out << "    " << (*cur_alias).first.c_str() << "  index:" << (*cur_alias).second << std::endl;

  out << "=============== vjInputManager Status =========================" << std::endl;
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
   unsigned int j;
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
   for (unsigned int i = 0; i < m_devVector.size(); i++)
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
vjInput* vjInputManager::getDevice(unsigned int devNum)
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
   unsigned int i;
   if (m_devVector[devNum] == NULL)    // Check for valid device
      return false;

   // XXX: We should actually just un-config the proxies

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
   {
      vjDEBUG(vjDBG_INPUT_MGR,vjDBG_STATE_LVL) << "vjInputManager::addPosProxy: Could not find device: " << devName << std::endl << vjDEBUG_FLUSH;
      return -1;
   }

   vjDEBUG(vjDBG_INPUT_MGR,vjDBG_STATE_LVL)      << "adding a posProxy : " << proxyName.c_str() << std::endl << vjDEBUG_FLUSH;
   vjDEBUG_CONT(vjDBG_INPUT_MGR,vjDBG_STATE_LVL) << "   attaching to device named: " << devName.c_str() << std::endl << vjDEBUG_FLUSH;
   vjDEBUG_CONT(vjDBG_INPUT_MGR,vjDBG_STATE_LVL) << "   at unit number: " << subNum << std::endl << vjDEBUG_FLUSH;
   vjDEBUG_CONT(vjDBG_INPUT_MGR,vjDBG_STATE_LVL) << "  Found the device at devNum: " << dev_num << std::endl << vjDEBUG_FLUSH;

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
   {
      vjDEBUG(vjDBG_INPUT_MGR,vjDBG_STATE_LVL) << "vjInputManager::addDigProxy: Could not find device: " << devName << std::endl << vjDEBUG_FLUSH;
      vjDEBUG(vjDBG_INPUT_MGR,vjDBG_STATE_LVL) << "   Current state:\n" << (*this) << std::endl << vjDEBUG_FLUSH;
      return -1;
   }

   vjDEBUG(vjDBG_INPUT_MGR,vjDBG_STATE_LVL)      << "adding a digProxy : " << proxyName.c_str() << std::endl << vjDEBUG_FLUSH;
   vjDEBUG_NEXT(vjDBG_INPUT_MGR,vjDBG_STATE_LVL) << "   attaching to device named: " << devName.c_str() << std::endl << vjDEBUG_FLUSH;
   vjDEBUG_NEXT(vjDBG_INPUT_MGR,vjDBG_STATE_LVL) << "   at unit number: " << subNum << std::endl << vjDEBUG_FLUSH;
   vjDEBUG_NEXT(vjDBG_INPUT_MGR,vjDBG_STATE_LVL) << "  Found the device at devNum: " << dev_num << std::endl << vjDEBUG_FLUSH;

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
   {
      vjDEBUG(vjDBG_INPUT_MGR,vjDBG_STATE_LVL) << "vjInputManager::addAnaProxy: Could not find device: " << devName << std::endl << vjDEBUG_FLUSH;
      return -1;
   }

   vjDEBUG(vjDBG_INPUT_MGR,vjDBG_STATE_LVL)      << "adding a anaProxy : " << proxyName.c_str() << std::endl << vjDEBUG_FLUSH;
   vjDEBUG_CONT(vjDBG_INPUT_MGR,vjDBG_STATE_LVL) << "   attaching to device named: " << devName.c_str() << std::endl << vjDEBUG_FLUSH;
   vjDEBUG_CONT(vjDBG_INPUT_MGR,vjDBG_STATE_LVL) << "   at unit number: " << subNum << std::endl << vjDEBUG_FLUSH;
   vjDEBUG_CONT(vjDBG_INPUT_MGR,vjDBG_STATE_LVL) << "  Found the device at devNum: " << dev_num << std::endl << vjDEBUG_FLUSH;

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
   {
      vjDEBUG(vjDBG_INPUT_MGR,vjDBG_STATE_LVL) << "vjInputManager::addGloveProxy: Could not find device: " << devName << std::endl << vjDEBUG_FLUSH;
      return -1;
   }

   vjDEBUG(vjDBG_INPUT_MGR,vjDBG_STATE_LVL)      << "adding a gloveProxy : " << proxyName.c_str() << std::endl << vjDEBUG_FLUSH;
   vjDEBUG_CONT(vjDBG_INPUT_MGR,vjDBG_STATE_LVL) << "   attaching to device named: " << devName.c_str() << std::endl << vjDEBUG_FLUSH;
   vjDEBUG_CONT(vjDBG_INPUT_MGR,vjDBG_STATE_LVL) << "   at unit number: " << subNum << std::endl << vjDEBUG_FLUSH;
   vjDEBUG_CONT(vjDBG_INPUT_MGR,vjDBG_STATE_LVL) << "  Found the device at devNum: " << dev_num << std::endl << vjDEBUG_FLUSH;

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
   {
      vjDEBUG(vjDBG_INPUT_MGR,vjDBG_STATE_LVL) << "vjInputManager::addKeyboardProxy: Could not find device: " << devName << std::endl << vjDEBUG_FLUSH;
      return -1;
   }

   vjDEBUG(vjDBG_INPUT_MGR,vjDBG_STATE_LVL)      << "adding a keyboard Proxy : " << proxyName.c_str() << std::endl << vjDEBUG_FLUSH;
   vjDEBUG_CONT(vjDBG_INPUT_MGR,vjDBG_STATE_LVL) << "   attaching to device named: " << devName.c_str() << std::endl << vjDEBUG_FLUSH;
   vjDEBUG_CONT(vjDBG_INPUT_MGR,vjDBG_STATE_LVL) << "   at unit number: " << subNum << std::endl << vjDEBUG_FLUSH;
   vjDEBUG_CONT(vjDBG_INPUT_MGR,vjDBG_STATE_LVL) << "  Found the device at devNum: " << dev_num << std::endl << vjDEBUG_FLUSH;

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
   {
      vjDEBUG(vjDBG_INPUT_MGR,vjDBG_STATE_LVL) << "vjInputManager::addGesterProxy: Could not find device: " << devName << std::endl << vjDEBUG_FLUSH;
      return -1;
   }

   vjDEBUG(vjDBG_INPUT_MGR,vjDBG_STATE_LVL)      << "adding a anaProxy : " << proxyName.c_str() << std::endl << vjDEBUG_FLUSH;
   vjDEBUG_CONT(vjDBG_INPUT_MGR,vjDBG_STATE_LVL) << "   attaching to device named: " << devName.c_str() << std::endl << vjDEBUG_FLUSH;
   vjDEBUG_CONT(vjDBG_INPUT_MGR,vjDBG_STATE_LVL) << "   at unit number: " << subNum << std::endl << vjDEBUG_FLUSH;
   vjDEBUG_CONT(vjDBG_INPUT_MGR,vjDBG_STATE_LVL) << "  Found the device at devNum: " << dev_num << std::endl << vjDEBUG_FLUSH;

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
   return (((std::string)chunk->getType()) == std::string("proxyAlias"));
}

// Configures proxy aliases in config database
// PRE: none
// POST: (alias not already in list) ==> Alias is added to proxyAlias list
//+      (alias was already is list) ==> Alias is set to point to the new proxy instead
bool vjInputManager::configureProxyAlias(vjConfigChunk* chunk)
{
   vjDEBUG_BEGIN(vjDBG_INPUT_MGR,vjDBG_STATE_LVL) << "vjInputManager::Configuring proxy alias" << std::endl << vjDEBUG_FLUSH;
   vjASSERT(((std::string)chunk->getType()) == "proxyAlias");

   std::string alias_name, proxy_name;  // The string of the alias, name of proxy to pt to

   alias_name = (std::string)chunk->getProperty("aliasName");
   proxy_name = (std::string)chunk->getProperty("proxyPtr");

   if(proxyAliases.end() == proxyAliases.find(proxy_name))
   {
      vjDEBUG(vjDBG_ERROR,vjDBG_CRITICAL_LVL)
                  << clrOutNORM(clrRED,"ERROR:") << "vjInputManager::ConfigureProxyAliases: Alias: " << alias_name.c_str()
                 << "  cannot find proxy: " << proxy_name.c_str() << std::endl << vjDEBUG_FLUSH;
      return false;
   } else {
         // Since all proxies are already in the alias list, we just have to find the
         // one to point to and use it's index
      proxyAliases[alias_name] = proxyAliases[proxy_name];
      vjDEBUG(vjDBG_INPUT_MGR,vjDBG_CONFIG_LVL) << "   alias:" << alias_name.c_str() << "   index:"
                 << proxyAliases[proxy_name] << std::endl << vjDEBUG_FLUSH;
   }
   vjDEBUG_END(vjDBG_INPUT_MGR,vjDBG_STATE_LVL) << std::endl << vjDEBUG_FLUSH;
   return true;
}

// Remove a proxy aliases in config database
// PRE: none
// POST: (alias not in list) ==> returns = false
//+      (alias is in list) ==> (alias is removed from list) returns true
bool vjInputManager::removeProxyAlias(vjConfigChunk* chunk)
{
   vjDEBUG_BEGIN(vjDBG_INPUT_MGR,vjDBG_STATE_LVL) << "vjInputManager::RemoveProxyAlias" << std::endl << vjDEBUG_FLUSH;
   vjASSERT(((std::string)chunk->getType()) == "proxyAlias");

   std::string alias_name, proxy_name;  // The string of the alias, name of proxy to pt to

   alias_name = (std::string)chunk->getProperty("aliasName");

   if(proxyAliases.end() == proxyAliases.find(alias_name))
   {
      vjDEBUG(vjDBG_ERROR,vjDBG_CRITICAL_LVL)
                 << clrOutNORM(clrRED,"ERROR:") << "vjInputManager::RemoveProxyAlias: Alias: " << alias_name.c_str()
                 << "  cannot find proxy: " << proxy_name.c_str() << std::endl << vjDEBUG_FLUSH;
      return false;
   } else {
      proxyAliases.erase(alias_name);
      vjDEBUG(vjDBG_INPUT_MGR,vjDBG_CONFIG_LVL) << "   alias:" << alias_name.c_str() << "   index:"
                 << proxyAliases[proxy_name] << "  has been removed." << std::endl << vjDEBUG_FLUSH;
   }
   vjDEBUG_END(vjDBG_INPUT_MGR,vjDBG_STATE_LVL) << std::endl << vjDEBUG_FLUSH;
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
   vjDEBUG(vjDBG_INPUT_MGR,vjDBG_STATE_LVL) << "Proxy alias [" << aliasStr.c_str() << "] added.\n" << vjDEBUG_FLUSH;
   vjDEBUG(vjDBG_INPUT_MGR,vjDBG_VERB_LVL)   << "   index:" << proxyIndex << std::endl << vjDEBUG_FLUSH;
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
