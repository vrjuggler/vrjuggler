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
#include <Input/vjInput/vjInput.h>

//: A dummy Digital device for Digital proxies to default to.
//!PUBLIC_API:
class vjDummyDigital : public vjDigital
{
public:
	//: Constructor
	vjDummyDigital() : vjDigital()
   { active = 1;}
   //: Destructor
	~vjDummyDigital() {}

	/** @name vjInput pure virtual functions
	 *
	 *  pure virtual functions required by vjInput
	 */
	//@{
	int startSampling() { return 1; }
	int stopSampling() { return 0; }
	int sample() { return 1;}
	void updateData() { return ; }
	//@}
	
	/** @name vjInput virtual functions
	 *
	 *  virtual functions that inherited members should
	 *  override but are not required to
	 */
	//@{
	char* getDeviceName() { return "vjDummyDigital"; }
	//@}

	/** @name vjAnalog pure virtual functions
	 *
	 *  pure virtual functions required by vjAnalog
	 */
	//@{
	int getDigitalData (int d) { return 0; }
	//@}
};

#endif
