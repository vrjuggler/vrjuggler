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

 
#ifndef _VJ_ASCENSION_MOTION_STAR_H_
#define _VJ_ASCENSION_MOTION_STAR_H_
 
#include <vjConfig.h>
#include <Input/vjPosition/vjPosition.h>
#include <Threads/vjThread.h>
#include <Input/vjPosition/aMotionStar.h>
 
//----------------------------------------------------------------------------
//: Position derived class for running a Flock of Birds.
//: , also a wrapper class for the real ascension flock class "aFlock"
//
//  vjFlock adds to the aFlock class shared memory and threading.<br>
//  vjFlock is a positional device driver for the Flock of Birds, the config
//  chunk in the constructor should set up all the settings, for these to be
//  changed the Flock has to be deleted and a new instance created with an
//  updated configchunk.
//  <br>
//! NOTE: Some functions still remain for changing the options of
//+    the flock when its not in Sampling mode, but in order to stay
//+    consistent with the vjInput/vjPosition functionality these
//+    are only left for building apps without vjConfigChunks
//! NOTE: A note on reciever access:
//+  Clients of juggler should access tracker recievers as [0-n]
//+  For example, if you have recievers 1,2, and 4 with transmitter on 3,
//+  then you can access the data, in order, as 0,1,2.
//
// See also: vjPosition
//---------------------------------------------------------------------------
//!PUBLIC_API:
class vjMotionStar : public vjPosition {
    public:

    vjMotionStar(char* ipAddress = NULL,
                  int hemisphere = 3,
                  unsigned int birdFormat = 4,
                  unsigned int birdsRequired = 10,
                  int runMode = 0,
                  double birdRate = 90.0
                  //unsigned char reportRate
                  );

    ~vjMotionStar();
 
    //: configure the flock with a config chunk
    virtual bool config(vjConfigChunk* c);
 
    //: begin sampling
    int startSampling();
 
    //: stop sampling
    int stopSampling();
 
    //: sample data
    int sample();
 
    //: update to the sampled data.
    void updateData();
 
    //: get the device name
    char* getDeviceName() { return "vjMotionStar"; }
 
    //: return what chunk type is associated with this class.
    static std::string getChunkType() { return std::string("MotionStar");}
 
    //: Get the reciever transform
    //! ARGS: dev - is the reciever number
    //! POST: returns a pointer to the reciever's matrix
    //! NOTE: Clients of juggler should access tracker recievers as [0-n]
    //+  For example, if you have recievers 1,2, and 4 with transmitter on 3,
    //+  then you can access them, in order, as 0,1,2.
    vjMatrix* getPosData( int dev = 0); // 0 base
    
    //  Not used currently -- needed for interface
    vjTimeStamp* getPosUpdateTime (int d); 
 
    //: see if the flock is active or not
    inline bool isActive() { return mMotionStar.isActive(); }
 
    void            setHemisphere(int i );
    inline unsigned int  getHemisphere()  { return mMotionStar.getHemisphere();}

    void            setBirdFormat(unsigned int n );
    inline unsigned int  getBirdFormat() { return mMotionStar.getBirdFormat();}

    void            setNumBirds( unsigned int n );
    inline unsigned int  getNumBirds()  { return mMotionStar.getNumBirds();}
 
    void            setBirdRate( double n );
    inline double  getBirdRate() { return mMotionStar.getBirdRate();}

    void            setRunMode( int n );
    inline int  getRunMode() { return mMotionStar.getRunMode();}

    void            setReportRate( unsigned char n );
    inline unsigned char  getReportRate() { return mMotionStar.getReportRate();}

    void            setIpAddress( const char* n );
    inline char* getIpAddress () {return mMotionStar.getIpAddress();}
 
        //: get the x position of the i'th reciever
        inline float       xPos( int i ) { return mMotionStar.xPos(i); }
 
        //: get the y position of the i'th reciever
        inline float       yPos( int i ) { return mMotionStar.yPos( i); }
 
        //: get the z position of the i'th reciever
        inline float       zPos( int i ) { return mMotionStar.zPos( i); }
 
        //: get the z rotation of the i'th reciever
        inline float       zRot( int i ) { return mMotionStar.zRot( i); };
 
        //: get the y rotation of the i'th reciever
        inline float       yRot( int i ) { return mMotionStar.yRot( i); }
 
        //: get the x rotation of the i'th reciever
        inline float       xRot( int i ) { return mMotionStar.xRot( i); }
 
 
 
private:
    void positionCorrect(float&x,float&y,float&z);
    void initCorrectionTable(const char*);
 
    int getBirdIndex(int birdNum, int bufferIndex);
 
    vjThread*   myThread;      // The thread doing the flock sampling
 
    aMotionStar mMotionStar;
};
 
#endif
