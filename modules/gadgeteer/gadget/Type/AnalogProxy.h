/////////////////////////////////////////////////////////////////////////
// vjAnalogProxy.h
//
// positional proxy class
//
////////////////////////////////////////////////////////////////////////
#ifndef _VJ_ANALOGPROXY_H_
#define _VJ_ANALOGPROXY_H_

#include <vjConfig.h>
#include <assert.h>
#include <Input/vjInput/vjAnalog.h>

//: A proxy class to analog devices, used by the vjInputManager.
//
//  A vjAnalogProxy always points to an analog device and subUnit number,
//  the inputgroup can therefore keep an array of these around and
//  treat them as analog devices which only return a single
//  subDevice's amount of data.  (one int)
//
// See also: vjAnalog
//
class vjAnalogProxy : public vjMemory {

  public:
   //: Constructor
    vjAnalogProxy(vjAnalog* anaPtr, int d) {
        vjASSERT( anaPtr->FDeviceSupport(DEVICE_ANALOG) );
	      m_anaPtr = anaPtr;
	      m_unitNum = d;
    }

    ~vjAnalogProxy() {    }

    //: Set the proxy to point to the given analog device
    //! PRE: anaPtr must be a valid analog device
    //+      subNum must be a valid subNum for the device
    //! POST: The proxy now references the analog device
    //! ARGS: anaPtr - Pointer to the analog device
    //! ARGS: subNum - The subunit number of the analog device
    void Set(vjAnalog* anaPtr, int subNum)
    {
       assert( anaPtr->FDeviceSupport(DEVICE_ANALOG) );
       m_anaPtr = anaPtr;
       m_unitNum = subNum;
    }

    //: Update the cached data copy from the device
    void UpdateData()
    { m_data = m_anaPtr->GetAnalogData(m_unitNum); }

    //: Get the current analog data value
    //! RETURNS: The analog data from the device
    int GetData()
    { return m_data; }

    vjAnalog* GetAnalogPtr()
    { return m_anaPtr; }

    int GetUnit()
    { return m_unitNum; }

  private:
    vjAnalog*   m_anaPtr;
    int	       m_unitNum;
    int	       m_data;
};

#endif
