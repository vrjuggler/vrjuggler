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


/////////////////////////////////////////////////////////////////////////
//
// positional tracker base class
//
////////////////////////////////////////////////////////////////////////
#ifndef _VJ_ANALOG_H_
#define _VJ_ANALOG_H_

#include <vjConfig.h>
#include <Input/vjInput/vjInput.h>

//-----------------------------------------------------------------------------
//: vjAnalog is the abstract base class that devices with digital data derive
//+ from.
//
//  vjAnalog is the base class that digital devices must derive from.
//  vjAnalog inherits from vjInput, so it has pure virtual function
//  constraints from vjInput in the following functions: StartSampling,
//  StopSampling, Sample, and UpdateData. <br> <br>
//
//  vjAnalog adds one new pure virtual function, GetAnalogData for retreiving
//  the digital data, similar to the addition for vjPosition and vjDigital.
//!PUBLIC_API:
//-----------------------------------------------------------------------------
class vjAnalog : virtual public vjInput
{
public:

	//: Constructor
   //! POST: Set device abilities
   //! NOTE: Must be called from all derived classes
   vjAnalog () {  deviceAbilities = deviceAbilities | DEVICE_ANALOG; }
	~vjAnalog() {}
	
   // Just call base class config
   //! NOTE: Let constructor set device abilities
   virtual bool config(vjConfigChunk* c)
   { return vjInput::config(c); }

	/* vjInput pure virtual functions */
	virtual int startSampling() = 0;
	virtual int stopSampling() = 0;
	virtual int sample() = 0;
	virtual void updateData() = 0;
	
	//: Get the device name
	char* getDeviceName() { return "vjAnalog"; }
	
	/* New pure virtual functions */
	//: Return analog data
	virtual int getAnalogData(int devNum = 0) = 0;
};


#endif	/* _VJ_ANALOG_H_ */
