/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998, 1999, 2000 by Iowa State University
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
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

#ifndef _DIVERSE_TOOLKIT_
#define _DIVERSE_TOOLKIT_

#include <vjConfig.h>
#include <Input/vjInput/vjDigital.h>
#include <Input/vjInput/vjAnalog.h>
#include <Input/vjPosition/vjPosition.h>
#include <Input/vjDTK/vjDTKMemorySegment.h>

class vjDTKMemorySegment;
typedef vjDTKMemorySegment* vjDTKMemorySegmentHandle;

class dtkClient;

class VJ_CLASS_API vjDTK : public vjInput, public vjPosition, public vjDigital,
                           public vjAnalog
{
public:
    // ------------------------------------------------------------------------
    //: Constructor.
    // ------------------------------------------------------------------------
    vjDTK();

    // ------------------------------------------------------------------------
    //: Destructor.
    //
    //! PRE: None.
    //! POST: Shared memory is released
    // ------------------------------------------------------------------------
   virtual ~vjDTK();
   
//: configure the flock with a config chunk
    virtual bool config(vjConfigChunk* c);

//: begin sampling
    int startSampling();

// Main thread of control for this active object
    void controlLoop(void* nullParam);

//: stop sampling
    int stopSampling();

//: sample data
    int sample();

//: update to the sampled data.
    void updateData();

//: get the device name
    char* getDeviceName() { return "vjDTK"; }

//: return what chunk type is associated with this class.
    static std::string getChunkType() { return std::string("DTK");}

//: Get the receiver transform
//! ARGS: dev - is the reciever number
//! POST: returns a pointer to the receiver's matrix
//! NOTE: Clients of juggler should access tracker recievers as [0-n]
//+  For example, if you have recievers 1,2, and 4 with transmitter on 3,
//+  then you can access them, in order, as 0,1,2.
    vjMatrix* getPosData( int dev = 0); // 0 base

//: Get the digital and analog data
//! ARGS: d - the button number
//! POST: returns a boolean value where 0 = false and 1 = true
//! NOTE: Since the tracker has multiple possible devices but digital
    int getDigitalData(int d = 0);
    float getAnalogData(int d = 0);

//: Get time of last update for this receiver
//! ARGS: dev - is the reciever number
//! POST: returns a pointer to the reciever's timestamp
    vjTimeStamp* getPosUpdateTime (int dev = 0);

//: see if the DTK is active or not
    inline bool isActive() { return active; };

private:   
    int getStationIndex(int stationNum, int bufferIndex);

//: DTK related data (could be in a wrapper, but not necessary)

    bool active;
    
    dtkClient*		_client;
    
    int numPositional;    
    int numDigital;
    int numAnalog; 
    int numSegments;
    
    vjDTKMemorySegmentHandle *_dtkSegments;
    int	*mDigitalData;
    float *mAnalogData;
    char*		port;

//: Start the DTK Client Connection
    bool startDTK();
    
//: Stop the DTK Client Connection
    bool stopDTK();
    
};

#endif
