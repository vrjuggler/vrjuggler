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


#ifndef _VJ_DUMMYTRACKER_H_
#define _VJ_DUMMYTRACKER_H_

#include <vjConfig.h>
#include <Input/vjPosition/vjPosition.h>

//: Position derived class for impersonating a position device.
//
// vjDummyPosition is a dummy positional device for holding semi-static
// device data.
//!PUBLIC_API:
class vjDummyPosition: public vjPosition {

  public:

    //: Constructor
    vjDummyPosition() : vjPosition() { 
	active = 1;
	instName = NULL;
	mTimeStamp.set();
    }

    //: Destructor
    ~vjDummyPosition();


   virtual bool config(vjConfigChunk *c);


    //: vjInput pure virtual functions
    int startSampling();
    int stopSampling();
    int sample();
    void updateData();

    //: vjInput virtual functions
    char* getDeviceName() { return "vjDummyPosition"; }

	
    //: vjPosition pure virtual functions
    vjMatrix* getPosData (int d = 0);
    vjTimeStamp* getPosUpdateTime(int d = 0);
	
	
    //: Function to get access to the position matrix
    vjMatrix& posData()
	{
	    return mydata;
	}

    static std::string getChunkType() { return std::string("DummyPosition");}

private:
    vjMatrix mydata;     // Location data
    vjTimeStamp mTimeStamp;  // dummy stamp value;
};

#endif
