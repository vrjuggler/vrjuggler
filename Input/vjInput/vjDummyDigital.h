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
