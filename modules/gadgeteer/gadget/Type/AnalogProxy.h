/*
 *  File:	    $Name$
 *  Date modified:  $Date$
 *  Version:	    $Revision$
 *
 *
 *                                VR Juggler
 *                                    by
 *                              Allen Bierbaum
 *                             Christopher Just
 *                            Carolina Cruz-Neira
 *                               Albert Baker
 *
 *                         Copyright  - 1997,1998,1999
 *                Iowa State University Research Foundation, Inc.
 *                            All Rights Reserved
 */


/////////////////////////////////////////////////////////////////////////
//
// positional proxy class
//
////////////////////////////////////////////////////////////////////////
#ifndef _VJ_ANALOGPROXY_H_
#define _VJ_ANALOGPROXY_H_

#include <vjConfig.h>
#include <assert.h>
#include <Input/vjInput/vjAnalog.h>
#include <Input/InputManager/vjProxy.h>

//: A proxy class to analog devices, used by the vjInputManager.
//
//  A vjAnalogProxy always points to an analog device and subUnit number,
//  the inputgroup can therefore keep an array of these around and
//  treat them as analog devices which only return a single
//  subDevice's amount of data.  (one int)
//
// See also: vjAnalog
//
//!PUBLIC_API:
class vjAnalogProxy : public vjMemory, public vjProxy
{

public:
   //: Constructor
   vjAnalogProxy() : m_anaPtr(NULL), m_unitNum(-1), m_data(-1)
   {;}

   ~vjAnalogProxy() {}

   //: Set the proxy to point to the given analog device
   //! PRE: anaPtr must be a valid analog device
   //+      subNum must be a valid subNum for the device
   //! POST: The proxy now references the analog device
   //! ARGS: anaPtr - Pointer to the analog device
   //! ARGS: subNum - The subunit number of the analog device
   void set(vjAnalog* anaPtr, int subNum)
   {
      assert( anaPtr->fDeviceSupport(DEVICE_ANALOG) );
      m_anaPtr = anaPtr;
      m_unitNum = subNum;
   }

   //: Update the cached data copy from the device
   void updateData()
   { m_data = m_anaPtr->getAnalogData(m_unitNum);}

   //: Get the current analog data value
   //! RETURNS: The analog data from the device
   int getData()
   { return m_data;}

   vjAnalog* getAnalogPtr()
   { return m_anaPtr;}

   int getUnit()
   { return m_unitNum;}

   static std::string getChunkType() { return "AnaProxy";}

   bool config(vjConfigChunk* chunk);

   virtual vjInput* getProxiedInputDevice()
   {
      vjInput* ret_val = dynamic_cast<vjInput*>(m_anaPtr);
      vjASSERT(ret_val != NULL);
      return ret_val;
   }

private:
   vjAnalog*   m_anaPtr;
   int         m_unitNum;
   int         m_data;
};

#endif
