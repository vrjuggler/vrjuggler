/////////////////////////////////////////////////////////////////////////////
// vjDummyDigital.h
//
// Dummy Digital device
//
// History:  
//
// Andy Himberger:    v0.0 - 12-1-97 - Inital version
////////////////////////////////////////////////////////////////////////////


#ifndef _VJ_DUMMYDIGITAL_H_
#define _VJ_DUMMYDIGITAL_H_

#include <vjConfig.h>
#include <Input/vjInput/vjDigital.h>

//: A dummy Digital device for Digital proxies to default to.
class vjDummyDigital : public vjDigital {

  public:
	/** @name Construction/Destruction */
	//@{
	vjDummyDigital() : vjDigital(), vjInput() { active = 1;}
	~vjDummyDigital() {}
	//@}

	/** @name vjInput pure virtual functions
	 *  
	 *  pure virtual functions required by vjInput
	 */
	//@{
	int StartSampling() { return 1; }
	int StopSampling() { return 0; }
	int Sample() { return 1;}
	void UpdateData() { return ; }
	//@}
	
	/** @name vjInput virtual functions
	 *
	 *  virtual functions that inherited members should
	 *  override but are not required to
	 */
	//@{
	char* GetDeviceName() { return "vjDigital"; }
	//@}

	/** @name vjAnalog pure virtual functions
	 *
	 *  pure virtual functions required by vjAnalog
	 */
	//@{
	int GetDigitalData (int d) { return 0; }
	//@}
};

#endif
