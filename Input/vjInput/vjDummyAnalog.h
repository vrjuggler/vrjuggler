/////////////////////////////////////////////////////////////////////////////
// vjDummyAnalog.h
//
// Dummy Analog device
//
// History:
//
// Andy Himberger:    v0.0 - 12-1-97 - Inital version
////////////////////////////////////////////////////////////////////////////
#ifndef _VJ_DUMMYANALOG_H_
#define _VJ_DUMMYANALOG_H_

#include <vjConfig.h>
#include <Input/vjInput/vjAnalog.h>

//: A dummy Analog device for Analog proxies to default to.
//!PUBLIC_API:
class vjDummyAnalog : public vjAnalog {
  public:
	/** @name Construction/Destruction
	 */
	//@{
	vjDummyAnalog() : vjAnalog(), vjInput()
   { active = 1; }
	~vjDummyAnalog() {}
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
	char* GetDeviceName() { return "vjDummyAnalog"; }
	//@}
	
	/** @name vjAnalog pure virtual functions
	 *
	 *  pure virtual functions required by vjAnalog
	 */
	//@{
	int GetAnalogData (int d) { return 0; }
	//@}
};

#endif
