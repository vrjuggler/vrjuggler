#include <vjConfig.h>
#include <iostream.h>
#include <iomanip.h>
#include <assert.h>

#include <Input/InputManager/vjInputManager.h>
#include <Kernel/vjDebug.h>



/**********************************************************
  vjInputManager::vjInputManager()

  vjInputManager Constructor

*********************************************** ahimberg */
vjInputManager::vjInputManager() : m_dummyPos(), m_dummyDig(), m_dummyAna()
{
    ;
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
   vjDEBUG(2) << "  vjInputManager::InitialConfig(vjConfigChunkDB*) " << endl << vjDEBUG_FLUSH;


   ConfigureInputManager(cdb);

#ifndef WIN32    
   ConfigureFlock(cdb);
   ConfigureIbox(cdb);
   ConfigureDummyPos(cdb);
   ConfigurevjCyberGlove(cdb);
   Configure3DMouse(cdb);
#endif
   ConfigureKeyboard(cdb);

   ConfigurePosProxy(cdb);
   ConfigureAnaProxy(cdb);
   ConfigureDigProxy(cdb);

   ConfigureProxyAliases(cdb);         // Configure the default device array

   DumpStatus();                      // Dump the status
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
int vjInputManager::FindDeviceNum(char* instName)
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
   if (i == -1) return 0;
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
int vjInputManager::GetDigData(int d)
{
   return m_digProxyVector[d]->GetData(); 
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
int vjInputManager::GetAnaData(int d)
{
   return m_anaProxyVector[d]->GetData();
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * *
  * * * * * * * * * * * * * * * * * * * * * * * * * * * *
	PRIVATE FUNCTIONS TO INPUTGROUP
* * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#ifndef WIN32
void vjInputManager::ConfigureFlock(vjConfigChunkDB *cdb)
{    
   vjDEBUG(2) << "   vjInputManager::ConfigureFlock" << endl << vjDEBUG_FLUSH;
   vector<vjConfigChunk*> *vChunks;
   vector<vjConfigChunk*>::iterator i;

   vChunks = cdb->getMatching("Flock");   
   for (i = vChunks->begin(); i != vChunks->end(); i++)
   {
      vjDEBUG(5) << "	  found a flock named: " 
                  << (char*)( (*i)->getProperty("name") ) 
                  << endl << vjDEBUG_FLUSH;
      
      vjFlock* aFlock = new (getMyMemPool()) vjFlock ( *i );

      if (aFlock->StartSampling())
      {
         FAddDevice(aFlock); 
         vjDEBUG(2) << "   Flock started sampling and has been added." << endl << vjDEBUG_FLUSH;
      }
      else
      {
         vjDEBUG(2) << "   Flock failed to start, was not added to vjInputManager" << endl << vjDEBUG_FLUSH;
         delete aFlock;
      }
   }
   delete vChunks;

}

void vjInputManager::Configure3DMouse(vjConfigChunkDB *cdb)
{    
   vjDEBUG(2) << "   vjInputManager::Configure3DMouse" << endl << vjDEBUG_FLUSH;
   vector<vjConfigChunk*> *vChunks;
   vector<vjConfigChunk*>::iterator i;

   vChunks = cdb->getMatching("ThreeDMouse");   
   for (i = vChunks->begin(); i != vChunks->end(); i++)
   {
      vjDEBUG(5) << "	  found a 3dmouse named: " 
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
   vjDEBUG(2) << "   vjInputManager::DummyPos" << endl << vjDEBUG_FLUSH;
   vector<vjConfigChunk*> *vChunks;
   vector<vjConfigChunk*>::iterator i;

   vChunks = cdb->getMatching("DummyPosition");   
   for (i = vChunks->begin(); i != vChunks->end(); i++)
   {
      vjDEBUG(5) << "	  found a dummyPosition named: " 
      << (char*)( (*i)->getProperty("name") ) 
      << endl << vjDEBUG_FLUSH;
      
      vjDummyPosition* aDummy = new (getMyMemPool()) vjDummyPosition ( *i );
      FAddDevice(aDummy);
   }
   delete vChunks;
}

void vjInputManager::ConfigureIbox(vjConfigChunkDB *cdb)
{   
   vjDEBUG(2) << "   vjInputManager::ConfigureIbox" << endl << vjDEBUG_FLUSH;
   vector<vjConfigChunk*> *vChunks;
   vector<vjConfigChunk*>::iterator i;

   vChunks = cdb->getMatching("IBox");

   for (i = vChunks->begin(); i != vChunks->end(); i++)
   {
      vjDEBUG(5) << "	  found an ibox named: " 
                  << (char*)( (*i)->getProperty("name") ) 
                  << endl << vjDEBUG_FLUSH;
      
      vjIBox* anIbox = new (getMyMemPool()) vjIBox ( *i );
      FAddDevice(anIbox);
      anIbox->StartSampling();
   }
   delete vChunks;
}

void vjInputManager::ConfigurevjCyberGlove(vjConfigChunkDB *cdb)
{   
   vjDEBUG(2) << "   vjInputManager::ConfigurevjCyberGlove" << endl << vjDEBUG_FLUSH;
   vector<vjConfigChunk*> *vChunks;
   vector<vjConfigChunk*>::iterator i;

   vChunks = cdb->getMatching("vjCyberGlove");

   for (i = vChunks->begin(); i != vChunks->end(); i++)
   {
      vjDEBUG(5) << "	  found a vjCyberGlove named: " 
      << (char*)( (*i)->getProperty("name") ) 
      << endl << vjDEBUG_FLUSH;

      vjCyberGlove* aGlove = new (getMyMemPool()) vjCyberGlove ( *i );
      FAddDevice(aGlove);
      aGlove->StartSampling();
   }
   delete vChunks;
}
#endif // ndef WIN32

void vjInputManager::ConfigureKeyboard(vjConfigChunkDB *cdb)
{   
   vjDEBUG(2) << "   vjInputManager::ConfigureKeyboard" << endl << vjDEBUG_FLUSH;
   vector<vjConfigChunk*> *vChunks;
   vector<vjConfigChunk*>::iterator i;

   vChunks = cdb->getMatching("Keyboard");

   for (i = vChunks->begin(); i != vChunks->end(); i++)
   {
      vjDEBUG(5) << "	  found a Keyboard named: " 
                  << (char*)( (*i)->getProperty("name") ) 
                  << endl << vjDEBUG_FLUSH;

      vjKeyboard* aKeyboard = new (getMyMemPool()) vjKeyboard ( *i );

      if (aKeyboard->StartSampling())
         FAddDevice(aKeyboard);
      else
      {
         vjDEBUG(1) << "      Keyboard failed to start.. deleting instance" << endl << vjDEBUG_FLUSH;
         delete aKeyboard;
      }
   }
   delete vChunks;
}

void vjInputManager::ConfigurePosProxy(vjConfigChunkDB *cdb)
{ 
   vjDEBUG(2) << "   vjInputManager::ConfigurePosProxy" << endl << vjDEBUG_FLUSH;
   vector<vjConfigChunk*> *vChunks;
   vector<vjConfigChunk*>::iterator i;
   vChunks = cdb->getMatching("PosProxy");

   for (i = vChunks->begin(); i != vChunks->end(); i++)
   {
      int unitNum = (*i)->getProperty("unit");
      string proxy_name = (char*)(*i)->getProperty("name");
      
      vjDEBUG(5) << "	  found a posProxy:" << proxy_name << endl << vjDEBUG_FLUSH;
      vjDEBUG(5) << "		  attaching to device named: " 
                 << (char*) ( (*i)->getProperty("device") ) << endl << vjDEBUG_FLUSH;
      vjDEBUG(5) << "		  at unit number: " << unitNum << endl << vjDEBUG_FLUSH;

      int devNum = FindDeviceNum((*i)->getProperty("device"));
      vjDEBUG(5) << "	  Found the device at devNum: " << devNum << endl << vjDEBUG_FLUSH;

      if ( devNum != -1)
      {
         int proxyNum = AddPosProxy(devNum, unitNum);
         AddProxyAlias(proxy_name, proxyNum);                  // Set the alias for this proxy name
         vjDEBUG(5) << "		PosProxy is set" << endl << vjDEBUG_FLUSH;

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

/*		vjVec3 pos(1,0,0), or(0,0,0);
      pos.xformFull( m_posProxyVector[proxyNum]->GetTransform() ,
                pos);
      or.xformVec  (m_posProxyVector[proxyNum]->GetTransform() ,
                or);
      cout << "pos(1,0,0) after translate: " << endl << pos
           << "or (0,0,0) after translate: " << endl << or << endl;
*/
         }
      }

   }
   delete vChunks;
}

void vjInputManager::ConfigureDigProxy(vjConfigChunkDB *cdb)
{ 
   vjDEBUG(2) << "   vjInputManager::ConfigureDigProxy" << endl << vjDEBUG_FLUSH;
   vector<vjConfigChunk*> *vChunks;
   vector<vjConfigChunk*>::iterator i;
   vChunks = cdb->getMatching("DigProxy");

   for (i = vChunks->begin(); i != vChunks->end(); i++)
   {
      int unitNum = (*i)->getProperty("unit");
      string proxy_name = (char*)(*i)->getProperty("name");
      
      vjDEBUG(5)  << "	  found a digProxy: " << proxy_name << endl
                  << "		attaching to device named: " << (char*) ( (*i)->getProperty("device") ) << endl
                  << "		at unit number: " << unitNum << endl << vjDEBUG_FLUSH;
      
      int devNum = FindDeviceNum((*i)->getProperty("device"));
      vjDEBUG(5) << "	Found the device at devNum: " << devNum << endl << vjDEBUG_FLUSH;
      
      if ( devNum != -1)
      {
         int proxy_num = AddDigProxy(devNum, unitNum);
         AddProxyAlias(proxy_name, proxy_num);                  // Set the alias for this proxy name
         vjDEBUG(5) << "	      DigProxy is set" << endl << vjDEBUG_FLUSH;
      }
   }
   delete vChunks;   
}

void vjInputManager::ConfigureAnaProxy(vjConfigChunkDB *cdb)
{ 
   vjDEBUG(2) << "   vjInputManager::ConfigureAnaProxy" << endl << vjDEBUG_FLUSH;
   vector<vjConfigChunk*> *vChunks;
   vector<vjConfigChunk*>::iterator i;
   vChunks = cdb->getMatching("AnaProxy");

   for (i = vChunks->begin(); i != vChunks->end(); i++)
   {
      int unitNum = (*i)->getProperty("unit");
      string proxy_name = (char*)(*i)->getProperty("name");

      vjDEBUG(5) << "	  found a anaProxy : " << proxy_name << endl << vjDEBUG_FLUSH;
      vjDEBUG(5) << "		attaching to device named: " << (char*) ( (*i)->getProperty("device") ) << endl << vjDEBUG_FLUSH;
      vjDEBUG(5) << "		at unit number: " << unitNum << endl << vjDEBUG_FLUSH;
      
      int devNum = FindDeviceNum((*i)->getProperty("device"));
      vjDEBUG(2) << "	Found the device at devNum: " << devNum << endl << vjDEBUG_FLUSH;
      
      if ( devNum != -1)
      {
         int proxy_num = AddAnaProxy(devNum, unitNum);
         AddProxyAlias(proxy_name, proxy_num);
         vjDEBUG(2) << "	      AnaProxy is set" << endl << vjDEBUG_FLUSH;
      }
   }
   delete vChunks;
}

void vjInputManager::ConfigureInputManager(vjConfigChunkDB *cdb)
{
   vjDEBUG(2) << "   vjInputManager::ConfigureInputManager" << endl << vjDEBUG_FLUSH;
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
   vjDEBUG(5) << "   vjInputManager::ConfigureProxyAliases" << endl << vjDEBUG_FLUSH;
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
                    << " cannot find proxy: " << proxy_ptr << endl << vjDEBUG_FLUSH;
      } else {
            // Since all proxies are already in the alias list, we just have to find the
            // one to point to and use it's index
         proxyAliases[alias_name] = proxyAliases[proxy_ptr];
         vjDEBUG(5) << "      alias:" << alias_name << "   index:" << proxyAliases[proxy_ptr] << endl << vjDEBUG_FLUSH;
      }
   }
   delete vChunks;
}

//: Adds a Proxy alias to the alias list
// PRE: none - BUT: Preferable if proxyIndex  is for a valid proxy
// POST: Alias list has alias str refering to proxyIndex
// ARG: str: The string descriptor for the alias
//       NOTE: You can pass a char*, string has a copy contructor for char*
void vjInputManager::AddProxyAlias(string aliasStr, int proxyIndex)
{
   vjDEBUG(5) << "AddProxyAlias: alias:" << aliasStr << "   index:" << proxyIndex << endl << vjDEBUG_FLUSH;
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

