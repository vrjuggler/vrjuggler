/////////////////////////////////////////////////////////////////////////////
// vjInputManager.h
//
// Group manager for vj input devices and proxies
//
////////////////////////////////////////////////////////////////////////////
#ifndef VJ_INPUT_MANAGER_H
#define VJ_INPUT_MANAGER_H

#include <config.h>

#ifdef VJ_OS_HPUX
#   include <vector>
#   include <map>
#else
#   if defined(VJ_USE_PTHREADS) && ! defined(_PTHREADS)
#       define _PTHREADS
#   endif

#   include <vector.h>
#   include <map.h>
#   include <mstring.h>		// XXX: Use for basic_string.  Should replace with "real" imp
#endif

#include <Kernel/vjDebug.h>

#include <Input/InputManager/vjPosProxy.h>
#include <Input/InputManager/vjAnalogProxy.h>
#include <Input/InputManager/vjDigitalProxy.h>
#include <Input/vjPosition/vjDummyPosition.h>
#include <Input/vjInput/vjDummyDigital.h>
#include <Input/vjInput/vjDummyAnalog.h>

#ifndef WIN32
#include <Input/vjPosition/vjFlock.h>
#include <Input/vjInput/vjIbox.h>
#include <Input/vjGlove/vjCyberGlove.h>
#include <Input/vjInput/vjKeyboard.h>
#include <Input/vjPosition/logiclass.h>
#else
#include <Input/vjInput/vjKeyboardWin32.h>
#endif

//: The InputManager holds an manages all vj Input devices.  
//
//  The InputManager, handles all the
//  details behind organizing the input devices in the library.  It
//  provides an API for adding devices by their configChunk and deleting
//  them by their chunk (or just their string name).  
// 
//  Proxies are used to abstract away from the devices so any type and number
//  of devices may be accessed in the same way.  These proxies are also
//  set up by configChunks and should be accessed by number, rather than name.
//  (for speed)
//-------------------------------------------------------------------------------
class vjInputManager : public vjMemory
{
public:
   vjInputManager();
   ~vjInputManager();
  

   //: Dump the status of the Input manager.
   //
   //  This will output dumb info on all the devices in the Input Manager, 
   //  and all of the proxies.
   void DumpStatus();

   int FNewInput(vjConfigChunkDB *cdb);

   //: Function to get an index to the proxy/alias given in str
   //! RETURNS: -1 - Not Found
   int  GetProxyIndex(string proxyName);
   

   /************************************************************************ 
    *  Device Level API
    *  The Device level functions give direct access to the actual
    *  devices, that is functions that do not take a Config structure.
    *  These functions should primarily be used by the vjInputManager itself.
    *************************************************************************/
   
   //: Add a device to vjInputManager.
   //
   // Add the devPtr to the device Array, devPtr should
   //      * not already be in the array.  Returns -1 on failure
   //
   // modifies: self
   //! POST: m_devVector' = m_devVector \/ devPtr
   //+       return = devNum (position in the array)
   //                or -1 for fail
   int FAddDevice(vjInput* devPtr);

   //: Remove a device from the vjInputManager.  
   // Remove the device at position devNum from the
   // device Array.  Returns true on success.
   //
   // modifies: self
   // post: m_devVector[devNum]' = NULL
   int RemoveDevice(int devNum);

   int RemoveDevice(char* instName);

   //: Update the Data in all the devices. 
   // 
   // Call UpdateData() on all the devices in the devArray
   //
   // modifies: instances pointed to by devArray
   void UpdateAllData();

   //: Get the device number from its Instance Name.
   //
   //  Find the devNum of the device whose name is instName
   //
   //  returns -1 if fails
   int FindDeviceNum(char* instName);

   //: Get a pointer to the instance at devNum.
   // return the pointer at the devNum position in the array. <br>
   //
   //!POST: return = m_devVector[devNum]  (this can be NULL)
   vjInput* GetDevice(int devNum);


   /********************************************************** 
    *  Position Proxy Level 
    *  Functions for Position Proxies
    **********************************************************/
  
   //: Set the index ProxyNum in the posProxy array to proxy
   // device at devNum in the device array, with subNumber <br>
   // <br>
   // modifies: self <br>
   // post: m_posProxyArray[proxyNum]' = new Proxy at DevNum/subNum
   //       return = 0 for fail, other for success
   int SetPosProxy(int ProxyNum, int DevNum, int subNum);

   //: Get the proxy at the given index
   vjPosProxy* GetPosProxy(int posProxyIndex)
   {
      vjASSERT(m_posProxyVector.size() > posProxyIndex);    // Check array bounds
      return m_posProxyVector[posProxyIndex];
   }

   //: Add a pos proxy.
   // A pos proxy to the dev and subNum is created
   // retVal = index of new proxy
   int AddPosProxy(int DevNum, int subNum);

   //: Turn the position proxy at index ProxyNum to point back 
   // to the default dummy proxy. <br>
   // <br>
   // modifies: self <br>
   // postA:  m_posProxyArray[proxyNum]' = m_dummyPos
   void StupifyPos(int ProxyNum);


   /************************************************************************
    * Digital Proxy Level
    *  Functions for Digital proxies
    ************************************************************************/

   //: Set the index ProxyNum in the digProxy array to proxy
   // device at devNum in the device array, with subNumber <br>
   // <br>
   // modifies: self <br>
   // post: m_digProxyArray[proxyNum]' = new Proxy at DevNum/subNum
   //       return = 0 for fail, other for success
   int SetDigProxy(int ProxyNum, int DevNum, int subNum); 

   //: Get the proxy at the given index
   vjDigitalProxy* GetDigProxy(int digProxyIndex)
   {
      vjASSERT(m_digProxyVector.size() > digProxyIndex);    // Check array bounds
      return m_digProxyVector[digProxyIndex];
   }


   //: Adds a new digital proxy to the proxy vector
   // device at devNum in the device array, with subNumber <br>
   // <br>
   // modifies: self <br>
   // post: m_digProxyVector' contains new Proxy at DevNum/subNum
   //       return = vector index of new proxy
   int AddDigProxy(int DevNum, int subNum); 


   //: Turn the digital proxy at index ProxyNum to point back 
   // to the default dummy proxy.<br>
   // <br>
   // modifies: self<br>
   // postA:  m_digProxyArray[proxyNum]' = m_dummyDig
   void StupifyDig(int ProxyNum); 


   //: Get the digital data at digital Proxy d
   //
   // postA: return = integer for the digital data returned
   int GetDigData(int d);
     

   /**************************************************************
    * Analog Proxy Level
    *  Functions for Analog proxies
    **************************************************************/

   //: Set the index ProxyNum in the anaProxy array to proxy
   // device at devNum in the device array, with subNumber<br>
   // <br>
   // modifies: self<br>
   // post: m_anaProxyArray[proxyNum]' = new Proxy at DevNum/subNum
   //       return = 0 for fail, other for success
   int SetAnaProxy(int ProxyNum, int DevNum, int subNum); 

   //: Get the proxy at the given index
   vjAnalogProxy* GetAnaProxy(int anaProxyIndex)
   {
      vjASSERT(m_anaProxyVector.size() > anaProxyIndex);    // Check array bounds
      return m_anaProxyVector[anaProxyIndex];
   }


   //: Adds a new analog proxy to the proxy vector
   // device at devNum in the device array, with subNumber<br>
   // <br>
   // modifies: self<br>
   // post: m_anaProxyVector' = contains new Proxy at DevNum/subNum
   //       return = vector index of new proxy
   int AddAnaProxy(int DevNum, int subNum);


   //: Turn the analog proxy at index ProxyNum to point back 
   // to the default dummy proxy.<br>
   //<br>
   // modifies: self<br>
   // postA:  m_anaProxyArray[proxyNum]' = m_dummyAna
   void StupifyAna(int ProxyNum); 

   //: Get the analog data at analog Proxy d
   //
   // postA: return = integer version of the analog data at proxy d
   int GetAnaData(int d);


   /**************************************************************
    *  Gesture Proxy Level
    *  Functions for Gesture proxies
    **************************************************************/
   /* None yet */

protected:

      // --- Vectors of devices and proxies --- //
   vector<vjInput*>        m_devVector;
   vector<vjPosProxy*>     m_posProxyVector;
   vector<vjDigitalProxy*> m_digProxyVector;
   vector<vjAnalogProxy*>  m_anaProxyVector;
   
   vjDummyPosition  m_dummyPos;
   vjDummyDigital   m_dummyDig;
   vjDummyAnalog    m_dummyAna;

   map<string, int>    proxyAliases;      // List of alias indices for proxies

private:  
   /// @name Private functions for vjInputManager configuration from a ChunkDB
   //@{
   void ConfigureInputManager(vjConfigChunkDB* cdb);
#ifndef WIN32
   void ConfigureFlock(vjConfigChunkDB* cdb);
   void ConfigureDummyPos(vjConfigChunkDB* cdb);
   void ConfigureIbox(vjConfigChunkDB* cdb);
   void ConfigurevjCyberGlove(vjConfigChunkDB *cdb);
   void Configure3DMouse(vjConfigChunkDB *cdb);
#endif
   void ConfigureKeyboard(vjConfigChunkDB *cdb);
   void ConfigurePosProxy(vjConfigChunkDB* cdb);
   void ConfigureDigProxy(vjConfigChunkDB* cdb);
   void ConfigureAnaProxy(vjConfigChunkDB* cdb);
   //@}
   
   /// Function to configure the proxy Alias array
   void ConfigureProxyAliases(vjConfigChunkDB* cdb);
   void AddProxyAlias(string str, int proxyIndex);        
};

#endif
