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


/**********************************************************
  vjInputManager::vjInputManager()

  vjInputManager Constructor

*********************************************** ahimberg */
vjInputManager::vjInputManager() : m_dummyPos(), m_dummyDig(), m_dummyAna(), m_dummyGlove()
{
   m_dummyPosProxy = new vjPosProxy(&m_dummyPos,0);      // Create dummy proxy
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


/**********************************************************
  vjInputManager::FNewInput(vjConfigChunkDB *cdb)

  To be called to initialize the input group, set up the
  input devices, and set up the proxies.

  The order of events is to:
    1 set up specific vjInputManager settings
    2 construct and StartSampling the devies
    3 set the proxies to physical devices and their sub-unit

*********************************************** ahimberg */
int vjInputManager::FNewInput(vjConfigChunkDB *cdb)
{
   vjDEBUG_BEGIN(1) << "\nvjInputManager::InitialConfig(vjConfigChunkDB*) " << endl << vjDEBUG_FLUSH;

   ConfigureInputManager(cdb);

   cdb->dependencySort();

#ifndef WIN32
   ConfigureFlock(cdb);
   ConfigureIbox(cdb);
   ConfigureDummyPos(cdb);
   Configure3DMouse(cdb);
#endif
   ConfigureKeyboard(cdb);
   ConfigureKeyboardProxy(cdb);

   ConfigureSimAnalog(cdb);
   ConfigureSimDigital(cdb);
   ConfigureSimPosition(cdb);
   ConfigureAnaProxy(cdb);
   ConfigureDigProxy(cdb);
   ConfigurePosProxy(cdb);

   ConfigureSimGloveGesture(cdb);
   ConfigureGestureProxy(cdb);

#ifndef WIN32
   ConfigureCyberGlove(cdb);     // Need other proxies set up
#endif
   ConfigureGloveProxy(cdb);

   ConfigureProxyAliases(cdb);         // Configure the default device array

   DumpStatus();                      // Dump the status

   vjDEBUG_END(1) << endl << vjDEBUG_FLUSH;
   return 1;
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
  for(map<string, int>::iterator cur_alias = proxyAliases.begin(); cur_alias != proxyAliases.end(); cur_alias++)
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

/**********************************************************
  vjInputManager::AddPosProxy(int DevNum, int subNum)

  Setup a new position proxy for
                  device #DevNum at sub-unit #subNum

*********************************************** allenb */	
int vjInputManager::AddPosProxy(int DevNum, int subNum)
{
   vjPosProxy* new_pos_proxy = new vjPosProxy(&m_dummyPos,0);
   new_pos_proxy->Set(dynamic_cast<vjPosition*>(m_devVector[DevNum]),subNum);
   m_posProxyVector.push_back(new_pos_proxy);
   return (m_posProxyVector.size() -1);
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

/**********************************************************
  vjInputManager::AddDigProxy(int DevNum, int subNum)

  add new digital proxy for
            device number #devNum at sub-unit #subNum

*********************************************** ahimberg */
int vjInputManager::AddDigProxy(int DevNum, int subNum)
{
   vjDigitalProxy* new_dig_proxy = new vjDigitalProxy(&m_dummyDig,0);
   new_dig_proxy->Set(dynamic_cast<vjDigital*>(m_devVector[DevNum]),subNum);
   m_digProxyVector.push_back(new_dig_proxy);
   return (m_digProxyVector.size() - 1);
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

	
/**********************************************************
  vjInputManager::GetDigData(int d)

  Get the digital data at Digital Proxy #d

*********************************************** ahimberg */
/*
int vjInputManager::GetDigData(int d)
{
   return m_digProxyVector[d]->GetData();
}
*/

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

/**********************************************************
  vjInputManager::AddAnaProxy(int DevNum, int subNum)

 Create a new analogProxy for
                  device #DevNum at sub-unit #subNum

*********************************************** ahimberg */
int vjInputManager::AddAnaProxy(int DevNum, int subNum)
{
   vjAnalogProxy* new_ana_proxy = new vjAnalogProxy(&m_dummyAna,0);
   new_ana_proxy->Set(dynamic_cast<vjAnalog*>(m_devVector[DevNum]),subNum);
   m_anaProxyVector.push_back(new_ana_proxy);
   return (m_anaProxyVector.size() -1);
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


/**********************************************************
  vjInputManager::GetAnaData(int d)

  Get the analog data at Analog Proxy #d

*********************************************** ahimberg */	
/*
int vjInputManager::GetAnaData(int d)
{
   return m_anaProxyVector[d]->GetData();
}
*/

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

//:  Setup a new glove proxy for
//+ device #DevNum at sub-unit #subNum
int vjInputManager::AddGloveProxy(int DevNum, int subNum)
{
   vjGloveProxy* new_glove_proxy = new vjGloveProxy(&m_dummyGlove,0);
   new_glove_proxy->Set(dynamic_cast<vjGlove*>(m_devVector[DevNum]),subNum);
   m_gloveProxyVector.push_back(new_glove_proxy);
   return (m_gloveProxyVector.size() -1);
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

//:  Setup a new keyboard proxy for
//+ device #DevNum
int vjInputManager::AddKeyboardProxy(int DevNum)
{
   vjKeyboardProxy* new_kb_proxy = new vjKeyboardProxy(&m_dummyKeyboard);
   vjKeyboard* theKB = dynamic_cast<vjKeyboard*>(m_devVector[DevNum]);
   vjASSERT(theKB != NULL);
   new_kb_proxy->Set(theKB);
   m_keyboardProxyVector.push_back(new_kb_proxy);
   return (m_keyboardProxyVector.size() -1);
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

// Setup a new gesture proxy for device at DevNum
//! RETURNS: = index of new gesture proxy
int vjInputManager::AddGestureProxy(int DevNum)
{
   vjGestureProxy* new_gest_proxy = new vjGestureProxy(&m_dummyGesture);
   new_gest_proxy->Set(dynamic_cast<vjGesture*>(m_devVector[DevNum]));
   m_gestureProxyVector.push_back(new_gest_proxy);
   return (m_gestureProxyVector.size() -1);
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
#ifndef WIN32
void vjInputManager::ConfigureFlock(vjConfigChunkDB *cdb)
{
   vjDEBUG_BEGIN(1) << "----------------- FLOCK ----------------------\n"
                    << "vjInputManager::ConfigureFlock" << endl << vjDEBUG_FLUSH;
   vector<vjConfigChunk*> *vChunks;
   vector<vjConfigChunk*>::iterator i;

   vChunks = cdb->getMatching("Flock");
   for (i = vChunks->begin(); i != vChunks->end(); i++)
   {
      vjDEBUG(1) << "found a flock named: "
                  << (char*)( (*i)->getProperty("name") )
                  << endl << vjDEBUG_FLUSH;

      vjFlock* aFlock = new (getMyMemPool()) vjFlock ( *i );

      if (aFlock->StartSampling())
      {
         FAddDevice(aFlock);
         vjDEBUG(1) << "   Flock started sampling and has been added." << endl << vjDEBUG_FLUSH;
      }
      else
      {
         vjDEBUG(0) << "   Flock failed to start, was not added to vjInputManager" << endl << vjDEBUG_FLUSH;
         delete aFlock;
      }
   }
   delete vChunks;

   vjDEBUG_END(1) << endl << vjDEBUG_FLUSH;

}

void vjInputManager::Configure3DMouse(vjConfigChunkDB *cdb)
{
   vjDEBUG(1) << "----------------- 3DMOUSE ----------------------\n"
              << "   vjInputManager::Configure3DMouse" << endl << vjDEBUG_FLUSH;
   vector<vjConfigChunk*> *vChunks;
   vector<vjConfigChunk*>::iterator i;

   vChunks = cdb->getMatching("ThreeDMouse");
   for (i = vChunks->begin(); i != vChunks->end(); i++)
   {
      vjDEBUG(1) << "	  found a 3dmouse named: "
                  << (char*)( (*i)->getProperty("name") )
                  << endl << vjDEBUG_FLUSH;

      vjThreeDMouse* aMouse = new (getMyMemPool()) vjThreeDMouse ( *i );

      if (aMouse->StartSampling())
      {
         FAddDevice(aMouse);
         vjDEBUG(2) << "   3DMouse started sampling and has been added." << endl << vjDEBUG_FLUSH;
      }
      else
      {
         vjDEBUG(2) << "   3DMouse failed to start, was not added to vjInputManager" << endl << vjDEBUG_FLUSH;
         delete aMouse;
      }
   }
   delete vChunks;

}

void vjInputManager::ConfigureDummyPos(vjConfigChunkDB *cdb)
{
   vjDEBUG_BEGIN(1) << "----------------- DUMMY POS ----------------------\n"
                    << "vjInputManager::DummyPos" << endl << vjDEBUG_FLUSH;
   vector<vjConfigChunk*> *vChunks;
   vector<vjConfigChunk*>::iterator i;

   vChunks = cdb->getMatching("DummyPosition");
   for (i = vChunks->begin(); i != vChunks->end(); i++)
   {
      vjDEBUG(1) << "found a dummyPosition named: "
      << (char*)( (*i)->getProperty("name") )
      << endl << vjDEBUG_FLUSH;

      vjDummyPosition* aDummy = new (getMyMemPool()) vjDummyPosition ( *i );
      FAddDevice(aDummy);
   }
   delete vChunks;

   vjDEBUG_END(1) << endl << vjDEBUG_FLUSH;
}

void vjInputManager::ConfigureIbox(vjConfigChunkDB *cdb)
{
   vjDEBUG_BEGIN(1) << "----------------- IBOX ----------------------\n"
                    << "   vjInputManager::ConfigureIbox" << endl << vjDEBUG_FLUSH;
   vector<vjConfigChunk*> *vChunks;
   vector<vjConfigChunk*>::iterator i;

   vChunks = cdb->getMatching("IBox");

   for (i = vChunks->begin(); i != vChunks->end(); i++)
   {
      vjDEBUG(1) << "found an ibox named: "
                  << (char*)( (*i)->getProperty("name") )
                  << endl << vjDEBUG_FLUSH;

      vjIBox* anIbox = new (getMyMemPool()) vjIBox ( *i );
      FAddDevice(anIbox);
      anIbox->StartSampling();
   }
   delete vChunks;

   vjDEBUG_END(1) << endl << vjDEBUG_FLUSH;
}

void vjInputManager::ConfigureCyberGlove(vjConfigChunkDB *cdb)
{
   vjDEBUG_BEGIN(1) << "----------------- CYBER GLOVE ----------------------\n"
                    << "vjInputManager::ConfigureCyberGlove" << endl << vjDEBUG_FLUSH;
   vector<vjConfigChunk*> *vChunks;
   vector<vjConfigChunk*>::iterator i;

   vChunks = cdb->getMatching("CyberGlove");

   for (i = vChunks->begin(); i != vChunks->end(); i++)
   {
      vjDEBUG(1) << "	  found a CyberGlove named: "
                 << (char*)( (*i)->getProperty("name") )
                 << endl << vjDEBUG_FLUSH;

      vjCyberGlove* aGlove = new vjCyberGlove(*i);
      if(aGlove->StartSampling())
         FAddDevice(aGlove);
      else
      {
         vjDEBUG(0) << "CyberGlove:" << (char*)( (*i)->getProperty("name") )
                    <<  "failed to start.  Deleting...." << endl << vjDEBUG_FLUSH;
         delete aGlove;
      }
   }
   delete vChunks;

   vjDEBUG_END(1) << endl << vjDEBUG_FLUSH;
}
#endif // ndef WIN32

void vjInputManager::ConfigureKeyboard(vjConfigChunkDB *cdb)
{
   vjDEBUG_BEGIN(1) << "----------------- KEYBOARD ----------------------\n"
                    << "vjInputManager::ConfigureKeyboard" << endl << vjDEBUG_FLUSH;
   vector<vjConfigChunk*> *vChunks;
   vector<vjConfigChunk*>::iterator i;

   vChunks = cdb->getMatching("Keyboard");

   for (i = vChunks->begin(); i != vChunks->end(); i++)
   {
      vjDEBUG(1) << "found a Keyboard named: "
                  << (char*)( (*i)->getProperty("name") )
                  << endl << vjDEBUG_FLUSH;

#ifndef WIN32
      vjKeyboard* aKeyboard = new (getMyMemPool()) vjXWinKeyboard ( *i );
#else
      vjKeyboard* aKeyboard = new (getMyMemPool()) vjWin32Keyboard (*i);
#endif
      if (aKeyboard->StartSampling())
      {
         int dev_num = FAddDevice(aKeyboard);
         vjDEBUG(1) << "Added Device: devnum: " << dev_num << endl << vjDEBUG_FLUSH;
      }
      else
      {
         vjDEBUG(0) << "ERROR: Keyboard failed to start.. deleting instance" << endl << vjDEBUG_FLUSH;
         delete aKeyboard;
      }
   }
   delete vChunks;

   vjDEBUG_END(1) << endl << vjDEBUG_FLUSH;
}

void vjInputManager::ConfigureSimAnalog(vjConfigChunkDB* cdb)
{
   vjDEBUG_BEGIN(1) << "----------------- SIM ANALOG ----------------------\n"
                    << "vjInputManager::ConfigureSimAnalog" << endl << vjDEBUG_FLUSH;
   vector<vjConfigChunk*> *vChunks;
   vector<vjConfigChunk*>::iterator i;

   vChunks = cdb->getMatching("SimAnalog");

   for (i = vChunks->begin(); i != vChunks->end(); i++)
   {
      char* dev_name = (char*)(*i)->getProperty("name");
      vjDEBUG(1) << "found a SimAnalog named: "
                 << dev_name << endl << vjDEBUG_FLUSH;

      vjSimAnalog* aSimAnalog = new (getMyMemPool()) vjSimAnalog (*i);

      if (aSimAnalog->StartSampling())
         FAddDevice(aSimAnalog);
      else
      {
         vjDEBUG(0) << "ERROR: SimAnalog failed to start.. deleting instance" << endl << vjDEBUG_FLUSH;
         delete aSimAnalog;
      }
   }
   delete vChunks;

   vjDEBUG_END(1) << endl << vjDEBUG_FLUSH;
}

void vjInputManager::ConfigureSimDigital(vjConfigChunkDB* cdb)
{
   vjDEBUG_BEGIN(1) << "----------------- SIM DIGITAL ----------------------\n"
                    << "vjInputManager::ConfigureSimDigital" << endl << vjDEBUG_FLUSH;
   vector<vjConfigChunk*> *vChunks;
   vector<vjConfigChunk*>::iterator i;

   vChunks = cdb->getMatching("SimDigital");

   for (i = vChunks->begin(); i != vChunks->end(); i++)
   {
      char* dev_name = (char*)(*i)->getProperty("name");
      vjDEBUG(1) << "found a SimDigital named: "
                 << dev_name << endl << vjDEBUG_FLUSH;

      vjSimDigital* aSimDigital = new (getMyMemPool()) vjSimDigital (*i);

      if (aSimDigital->StartSampling())
         FAddDevice(aSimDigital);
      else
      {
         vjDEBUG(0) << "ERROR: SimDigital failed to start.. deleting instance" << endl << vjDEBUG_FLUSH;
         delete aSimDigital;
      }
   }
   delete vChunks;

   vjDEBUG_END(1) << endl << vjDEBUG_FLUSH;
}


void vjInputManager::ConfigureSimPosition(vjConfigChunkDB* cdb)
{
   vjDEBUG_BEGIN(1) << "----------------- SIM POSITION ----------------------\n"
                    << "vjInputManager::ConfigureSimPosition" << endl << vjDEBUG_FLUSH;
   vector<vjConfigChunk*> *vChunks;
   vector<vjConfigChunk*>::iterator i;

   vChunks = cdb->getMatching("SimPosition");

   for (i = vChunks->begin(); i != vChunks->end(); i++)
   {
      char* dev_name = (char*)(*i)->getProperty("name");
      vjDEBUG(1) << "found a SimPosition named: "
                 << dev_name << endl << vjDEBUG_FLUSH;

      vjSimPosition* aSimPos = new (getMyMemPool()) vjSimPosition (*i);

      if (aSimPos->StartSampling())
         FAddDevice(aSimPos);
      else
      {
         vjDEBUG(0) << "ERROR: SimPosition failed to start.. deleting instance" << endl << vjDEBUG_FLUSH;
         delete aSimPos;
      }
   }
   delete vChunks;

   vjDEBUG_END(1) << endl << vjDEBUG_FLUSH;
}


void vjInputManager::ConfigureSimGloveGesture(vjConfigChunkDB* cdb)
{
   vjDEBUG_BEGIN(1) << "----------------- SIM GLOVE GESTURE ----------------------\n"
                    << "vjInputManager::ConfigureSimGloveGesture" << endl << vjDEBUG_FLUSH;
   vector<vjConfigChunk*> *vChunks;
   vector<vjConfigChunk*>::iterator i;

   vChunks = cdb->getMatching("SimGloveGesture");

   for (i = vChunks->begin(); i != vChunks->end(); i++)
   {
      char* dev_name = (char*)(*i)->getProperty("name");
      vjDEBUG(1) << "found a SimGloveGesture named: "
                 << dev_name
                 << endl << vjDEBUG_FLUSH;

      vjSimGloveGesture* aSimGloveGesture = new (getMyMemPool()) vjSimGloveGesture (*i);

      if (aSimGloveGesture->StartSampling())
         FAddDevice(aSimGloveGesture);
      else
      {
         vjDEBUG(0) << "ERROR: SimGloveGesture failed to start.. deleting instance" << endl << vjDEBUG_FLUSH;
         delete aSimGloveGesture;
      }
   }
   delete vChunks;

   vjDEBUG_END(1) << endl << vjDEBUG_FLUSH;
}

/******************************************************************
*******************************************************************
                     PROXIES
*******************************************************************
*******************************************************************/
void vjInputManager::ConfigurePosProxy(vjConfigChunkDB *cdb)
{
   vjDEBUG_BEGIN(1) << "----------------- POS PROXY ----------------------\n"
                    << "vjInputManager::ConfigurePosProxy" << endl << vjDEBUG_FLUSH;
   vector<vjConfigChunk*> *vChunks;
   vector<vjConfigChunk*>::iterator i;
   vChunks = cdb->getMatching("PosProxy");

   for (i = vChunks->begin(); i != vChunks->end(); i++)
   {
      int unitNum = (*i)->getProperty("unit");
      string proxy_name = (char*)(*i)->getProperty("name");

      vjDEBUG(1) << "found a posProxy:" << proxy_name << endl << vjDEBUG_FLUSH;
      vjDEBUG_BEGIN(1) << "   attaching to device named: "
                 << (char*) ( (*i)->getProperty("device") ) << endl << vjDEBUG_FLUSH;
      vjDEBUG(1) << "   at unit number: " << unitNum << endl << vjDEBUG_FLUSH;

      int devNum = FindDeviceNum((*i)->getProperty("device"));
      vjDEBUG(1) << "   Found the device at devNum: " << devNum << endl << vjDEBUG_FLUSH;

      if ( devNum != -1)
      {
         int proxyNum = AddPosProxy(devNum, unitNum);
         AddProxyAlias(proxy_name, proxyNum);                  // Set the alias for this proxy name
         vjDEBUG(1) << "   PosProxy is set" << endl << vjDEBUG_FLUSH;

         if (1 == (int)(*i)->getProperty("etrans") )
         {
            vjDEBUG(2) << "Position Transform enabled..." << endl << vjDEBUG_FLUSH;
            m_posProxyVector[proxyNum]->SetTransform
            ( (*i)->getProperty("translate",0) , // xtrans
              (*i)->getProperty("translate",1) , // ytrans
              (*i)->getProperty("translate",2) , // ztrans
              (*i)->getProperty("rotate",0) , // xrot
              (*i)->getProperty("rotate",1) , // yrot
              (*i)->getProperty("rotate",2) );// zrot
            vjDEBUG(2) << "Transform Matrix: " << endl
                       << m_posProxyVector[proxyNum]->GetTransform() << endl << vjDEBUG_FLUSH;
         }
      }
      else
         vjDEBUG(0) << "ERROR: Could not find device:" << (*i)->getProperty("device") << endl << vjDEBUG_FLUSH;

      vjDEBUG_END(1) << endl << vjDEBUG_FLUSH;
   }
   delete vChunks;
   vjDEBUG_END(1) << endl << vjDEBUG_FLUSH;
}

void vjInputManager::ConfigureDigProxy(vjConfigChunkDB *cdb)
{
   vjDEBUG_BEGIN(1) << "----------------- DIG PROXY ----------------------\n"
                    << "vjInputManager::ConfigureDigProxy" << endl << vjDEBUG_FLUSH;
   vector<vjConfigChunk*> *vChunks;
   vector<vjConfigChunk*>::iterator i;
   vChunks = cdb->getMatching("DigProxy");

   for (i = vChunks->begin(); i != vChunks->end(); i++)
   {
      int unitNum = (*i)->getProperty("unit");
      string proxy_name = (char*)(*i)->getProperty("name");

      vjDEBUG(1)  << "found a digProxy: " << proxy_name << endl
                  << "  attaching to device named: " << (char*) ( (*i)->getProperty("device") ) << endl
                  << "  at unit number: " << unitNum << endl << vjDEBUG_FLUSH;

      int devNum = FindDeviceNum((*i)->getProperty("device"));
      vjDEBUG(1) << "	Found the device at devNum: " << devNum << endl << vjDEBUG_FLUSH;

      if ( devNum != -1)
      {
         int proxy_num = AddDigProxy(devNum, unitNum);
         AddProxyAlias(proxy_name, proxy_num);                  // Set the alias for this proxy name
         vjDEBUG(1) << "   DigProxy is set" << endl << vjDEBUG_FLUSH;
      }
      else
         vjDEBUG(0) << "ERROR: Could not find device: " << (*i)->getProperty("device") << endl << vjDEBUG_FLUSH;
   }
   delete vChunks;
   vjDEBUG_END(1) << endl << vjDEBUG_FLUSH;
}

void vjInputManager::ConfigureAnaProxy(vjConfigChunkDB *cdb)
{
   vjDEBUG_BEGIN(1) << "------------------ ANALOG PROXY -----------------\n"
                    << "   vjInputManager::ConfigureAnaProxy" << endl << vjDEBUG_FLUSH;
   vector<vjConfigChunk*> *vChunks;
   vector<vjConfigChunk*>::iterator i;
   vChunks = cdb->getMatching("AnaProxy");

   for (i = vChunks->begin(); i != vChunks->end(); i++)
   {
      int unitNum = (*i)->getProperty("unit");
      string proxy_name = (char*)(*i)->getProperty("name");

      vjDEBUG(1) << "found a anaProxy : " << proxy_name << endl << vjDEBUG_FLUSH;
      vjDEBUG(1) << "   attaching to device named: " << (char*) ( (*i)->getProperty("device") ) << endl << vjDEBUG_FLUSH;
      vjDEBUG(1) << "   at unit number: " << unitNum << endl << vjDEBUG_FLUSH;

      int devNum = FindDeviceNum((*i)->getProperty("device"));
      vjDEBUG(1) << "	Found the device at devNum: " << devNum << endl << vjDEBUG_FLUSH;

      if ( devNum != -1)
      {
         int proxy_num = AddAnaProxy(devNum, unitNum);
         AddProxyAlias(proxy_name, proxy_num);
         vjDEBUG(1) << "   AnaProxy is set" << endl << vjDEBUG_FLUSH;
      }
      else
         vjDEBUG(0) << "ERROR: Could not find device:" << (*i)->getProperty("device") << endl << vjDEBUG_FLUSH;
   }
   delete vChunks;
   vjDEBUG_END(1) << endl << vjDEBUG_FLUSH;
}

void vjInputManager::ConfigureGloveProxy(vjConfigChunkDB* cdb)
{
   vjDEBUG_BEGIN(1) << "vjInputManager::ConfigureGloveProxy" << endl << vjDEBUG_FLUSH;
   vector<vjConfigChunk*> *vChunks;
   vector<vjConfigChunk*>::iterator i;
   vChunks = cdb->getMatching("GloveProxy");

   for (i = vChunks->begin(); i != vChunks->end(); i++)
   {
      int unitNum = (*i)->getProperty("unit");
      string proxy_name = (char*)(*i)->getProperty("name");
      string device_name = (char*)(*i)->getProperty("device");

      vjDEBUG_BEGIN(1) << "found a gloveProxy : " << proxy_name << endl << vjDEBUG_FLUSH;
      vjDEBUG(1)       << "   attaching to device named: " << device_name << endl << vjDEBUG_FLUSH;
      vjDEBUG(1)       << "   at unit number: " << unitNum << endl << vjDEBUG_FLUSH;

      int devNum = FindDeviceNum(device_name.c_str());
      vjDEBUG_END(1) << "  Found the device at devNum: " << devNum << endl << vjDEBUG_FLUSH;

      if ( devNum != -1)
      {
         int proxy_num = AddGloveProxy(devNum, unitNum);
         AddProxyAlias(proxy_name, proxy_num);
         vjDEBUG(1) << "   GloveProxy is set" << endl << vjDEBUG_FLUSH;
      }
      else
      {
         vjDEBUG(0) << "ERROR: ConfigureGloveProxy:: Could not create proxy: " << proxy_name
                    << "   Could not find device:" << device_name << endl << vjDEBUG_FLUSH;
      }
   }
   delete vChunks;
   vjDEBUG_END(1) << endl << vjDEBUG_FLUSH;
}

void vjInputManager::ConfigureKeyboardProxy(vjConfigChunkDB* cdb)
{
   vjDEBUG_BEGIN(1) << "vjInputManager::ConfigureKeyboardProxy" << endl << vjDEBUG_FLUSH;
   vector<vjConfigChunk*> *vChunks;
   vector<vjConfigChunk*>::iterator i;
   vChunks = cdb->getMatching("KeyboardProxy");

   for (i = vChunks->begin(); i != vChunks->end(); i++)
   {
      string proxy_name = (char*)(*i)->getProperty("name");
      string dev_name = (char*)(*i)->getProperty("device");

      vjDEBUG(1) << "found a keyboardProxy : " << proxy_name << endl << vjDEBUG_FLUSH;
      vjDEBUG(1) << "   attaching to device named: " << dev_name << endl << vjDEBUG_FLUSH;

      int devNum = FindDeviceNum(dev_name.c_str());
      vjDEBUG(1) << "	Found the device at devNum: " << devNum << endl << vjDEBUG_FLUSH;
      vjKeyboard* kb = dynamic_cast<vjKeyboard*>(m_devVector[devNum]);
      if(kb == NULL)
         vjDEBUG(0) << "FATAL ERROR: Device is not a keyboard\n" << vjDEBUG_FLUSH;

      if ( devNum != -1)
      {
         int proxy_num = AddKeyboardProxy(devNum);
         AddProxyAlias(proxy_name, proxy_num);
         vjDEBUG(1) << "   KeyboardProxy is set" << endl << vjDEBUG_FLUSH;
      }
      else
         vjDEBUG(0) << "ERROR: ConfigureKeyboardProxy: Could not create proxy: " << proxy_name
                     << "   Could not find device:" << dev_name << endl << vjDEBUG_FLUSH;
   }
   delete vChunks;
   vjDEBUG_END(1) << endl << vjDEBUG_FLUSH;
}

void vjInputManager::ConfigureGestureProxy(vjConfigChunkDB* cdb)
{
   vjDEBUG_BEGIN(1) << "vjInputManager::ConfigureGestureProxy" << endl << vjDEBUG_FLUSH;
   vector<vjConfigChunk*> *vChunks;
   vector<vjConfigChunk*>::iterator i;
   vChunks = cdb->getMatching("GestureProxy");

   for (i = vChunks->begin(); i != vChunks->end(); i++)
   {
      string proxy_name = (char*)(*i)->getProperty("name");
      string dev_name = (char*)(*i)->getProperty("device");

      vjDEBUG(1) << "found a GestureProxy : " << proxy_name << endl << vjDEBUG_FLUSH;
      vjDEBUG(1) << "   attaching to device named: " << dev_name.c_str() << endl << vjDEBUG_FLUSH;

      int devNum = FindDeviceNum(dev_name.c_str());
      vjDEBUG(1) << "	Found the device at devNum: " << devNum << endl << vjDEBUG_FLUSH;

      if ( devNum != -1)
      {
         int proxy_num = AddGestureProxy(devNum);
         AddProxyAlias(proxy_name, proxy_num);
         vjDEBUG(1) << "   GestureProxy is set" << endl << vjDEBUG_FLUSH;
      }
      else
         vjDEBUG(0) << "ERROR: ConfigureGestureProxy: Could not create proxy: " << proxy_name
                     << "   Could not find device:" << dev_name << endl << vjDEBUG_FLUSH;
   }
   delete vChunks;
   vjDEBUG_END(1) << endl << vjDEBUG_FLUSH;
}

void vjInputManager::ConfigureInputManager(vjConfigChunkDB *cdb)
{
   vjDEBUG(1) << "vjInputManager::ConfigureInputManager" << endl << vjDEBUG_FLUSH;
   vjConfigChunk *c;// = cdb->getChunk("vjInputManager");
   vector<vjConfigChunk*> *vChunks;
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

// Configures all explicit proxy aliases in config database
// PRE: none
// POST: All aliases in config file are added to proxyAlias list
void vjInputManager::ConfigureProxyAliases(vjConfigChunkDB* cdb)
{
   vjDEBUG_BEGIN(1) << "vjInputManager::ConfigureProxyAliases" << endl << vjDEBUG_FLUSH;
   vjConfigChunk* alias_chunk = NULL;
   vector<vjConfigChunk*>* vChunks;
   vChunks = cdb->getMatching("proxyAlias");    // Get all proxy aliases

   for (int i=0;i<vChunks->size();i++)      // For all proxy aliases
   {
      char* c_str = NULL;
      string alias_name, proxy_ptr;       // The string of the alias, name of proxy to pt to

      alias_chunk = (*vChunks)[i];
      c_str = alias_chunk->getProperty("aliasName");
      alias_name = c_str;

      c_str = alias_chunk->getProperty("proxyPtr");
      proxy_ptr = c_str;

      if(proxyAliases.end() == proxyAliases.find(proxy_ptr))
      {
         vjDEBUG(1) << "vjInputManager::ConfigureProxyAliases: Alias: " << alias_name
                    << "  cannot find proxy: " << proxy_ptr << endl << vjDEBUG_FLUSH;
      } else {
            // Since all proxies are already in the alias list, we just have to find the
            // one to point to and use it's index
         proxyAliases[alias_name] = proxyAliases[proxy_ptr];
         vjDEBUG(1) << "   alias:" << alias_name << "   index:"
                    << proxyAliases[proxy_ptr] << endl << vjDEBUG_FLUSH;
      }
   }
   delete vChunks;

   vjDEBUG_END(1) << endl << vjDEBUG_FLUSH;
}

//: Adds a Proxy alias to the alias list
// PRE: none - BUT: Preferable if proxyIndex  is for a valid proxy
// POST: Alias list has alias str refering to proxyIndex
// ARG: str: The string descriptor for the alias
//       NOTE: You can pass a char*, string has a copy contructor for char*
void vjInputManager::AddProxyAlias(string aliasStr, int proxyIndex)
{
   vjDEBUG(1) << "AddProxyAlias: alias:" << aliasStr << "   index:" << proxyIndex << endl << vjDEBUG_FLUSH;
   proxyAliases[aliasStr] = proxyIndex;
}

//: Gets index to the proxy/alias named by str
// PRE: none
// POST: none
// RETVAL: -1 if str not found
//         proxy index if found
int vjInputManager::GetProxyIndex(string proxyName)
{
   if(proxyAliases.end() == proxyAliases.find(proxyName))
      return -1;
   else
      return proxyAliases[proxyName];
}

