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
	vjDummyAnalog() : vjAnalog(), vjInput()
   { active = 1; }
	~vjDummyAnalog() {}
	
	
	/*
    * pure virtual functions required by vjInput
	 */
	int StartSampling() { return 1; }
	int StopSampling() { return 0; }
	int Sample() { return 1;}
	void UpdateData() { return ; }
	
	/*
    *  virtual functions that inherited members should
	 *  override but are not required to
	 */
	char* GetDeviceName() { return "vjDummyAnalog"; }
	
	//: Get the analog data
   int GetAnalogData (int d) { return 0; }
};

#endif
