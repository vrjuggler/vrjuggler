/////////////////////////////////////////////////////////////////////////
// vjDigitalProxy.h
//
// positional proxy class
//
////////////////////////////////////////////////////////////////////////
#ifndef _VJ_DIGITALPROXY_H_
#define _VJ_DIGITALPROXY_H_

#include <config.h>
#include <assert.h>
#include <Input/vjInput/vjDigital.h>

//--------------------------------------------------------------------------
//: A proxy class to digital devices, used by the vjInputManager.
//
//  A vjDigitalProxy always points to a digital device and subUnit number, 
//  the inputgroup can therefore keep an array of these around and
//  treat them as digital devices which only return a single 
//  subDevice's amount of data.  (one int)
//--------------------------------------------------------------------------
class vjDigitalProxy : public vjMemory {

  public:
    /** @name Construction/Destruction */
    //@{
    vjDigitalProxy(vjDigital* digPtr, int unitNum) {
        assert( digPtr->FDeviceSupport(DEVICE_DIGITAL) );
	m_digPtr = digPtr;     
	m_unitNum = unitNum;
    }

    ~vjDigitalProxy() {    }
    //@}
    
    
    void Set(vjDigital* digPtr, int subNum)
    {
       assert( digPtr->FDeviceSupport(DEVICE_DIGITAL) );
       m_digPtr = digPtr;
       m_unitNum = subNum;
    }
    
    void UpdateData()
    {
      m_data = m_digPtr->GetDigitalData(m_unitNum);
    }
    
    int GetData()
    {
	return m_data;
    }
    
    vjDigital* GetDigitalPtr()
    {
      return m_digPtr;
    }
    
    int GetUnit()
    {
      return m_unitNum;
    }

  private:
    vjDigital*  m_digPtr; 
    int		 m_unitNum;
    int         m_data;
};

#endif
