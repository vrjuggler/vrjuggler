/*
 * VRJuggler
 *   Copyright (C) 1997,1998,1999,2000
 *   Iowa State University Research Foundation, Inc.
 *   All Rights Reserved
 *
 * Original Authors:
 *   Allen Bierbaum, Christopher Just,
 *   Patrick Hartling, Kevin Meinert,
 *   Carolina Cruz-Neira, Albert Baker
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 *
 * -----------------------------------------------------------------
 * File:          $RCSfile$
 * Date modified: $Date$
 * Version:       $Revision$
 * -----------------------------------------------------------------
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
