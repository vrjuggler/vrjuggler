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
class vjAnalogProxy : public vjMemory {

  public:
    /** @name Construction/Destruction */
    //@{
    vjAnalogProxy(vjAnalog* anaPtr, int d) {
        assert( anaPtr->FDeviceSupport(DEVICE_ANALOG) );
	m_anaPtr = anaPtr;     
	m_unitNum = d;
    }

    ~vjAnalogProxy() {    }
    //@}
    
    void Set(vjAnalog* anaPtr, int subNum)
    {
       assert( anaPtr->FDeviceSupport(DEVICE_ANALOG) );
       m_anaPtr = anaPtr;
       m_unitNum = subNum;
    }
    
    void UpdateData()
    {
	m_data = m_anaPtr->GetAnalogData(m_unitNum);
    }
    
    int GetData()
    {
	return m_data;
    }
    
    vjAnalog* GetAnalogPtr()
    {
	return m_anaPtr;
    }
    
    int GetUnit()
    {
      return m_unitNum;
    }

  private:
    vjAnalog*  m_anaPtr; 
    int	       m_unitNum;
    int	       m_data;
};

#endif
