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


/////////////////////////////////////////////////////////////////////////////
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
	int startSampling() { return 1; }
	int stopSampling() { return 0; }
	int sample() { return 1;}
	void updateData() { return ; }
	
	/*
    *  virtual functions that inherited members should
	 *  override but are not required to
	 */
	char* getDeviceName() { return "vjDummyAnalog"; }
	
	//: Get the analog data
   int getAnalogData (int d) { return 0; }
};

#endif
