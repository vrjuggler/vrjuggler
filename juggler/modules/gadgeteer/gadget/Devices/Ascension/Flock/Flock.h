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


//===============================================================
// Flock (a Wrapper for aFlock)
//
// Purpose:
//      VR Juggler Ascention Flock of birds tracking class
//
// Author:
// Kevin Meinert
//
// Last Modified: 4-22-99
//===============================================================
#ifndef _VJ_ASCENSION_FLOCK_OF_BIRDS_H_
#define _VJ_ASCENSION_FLOCK_OF_BIRDS_H_

#include <vjConfig.h>
#include <Input/vjInput/vjInput.h>
#include <Input/vjPosition/vjPosition.h>
#include <vpr/Thread/Thread.h>
#include <Input/vjPosition/aFlock.h>

namespace vrj
{
   
//----------------------------------------------------------------------------
//: Position derived class for running a Flock of Birds.
//: , also a wrapper class for the real ascension flock class "aFlock"
//
//  Flock adds to the aFlock class shared memory and threading.<br>
//  Flock is a positional device driver for the Flock of Birds, the config
//  chunk in the constructor should set up all the settings, for these to be
//  changed the Flock has to be deleted and a new instance created with an
//  updated configchunk.
//  <br>
//! NOTE: Some functions still remain for changing the options of
//+    the flock when its not in Sampling mode, but in order to stay
//+    consistent with the Input/vjPosition functionality these
//+    are only left for building apps without ConfigChunks
//! NOTE: A note on reciever access:
//+  Clients of juggler should access tracker recievers as [0-n]
//+  For example, if you have recievers 1,2, and 4 with transmitter on 3,
//+  then you can access the data, in order, as 0,1,2.
//
// See also: Position
//---------------------------------------------------------------------------
//!PUBLIC_API:
class Flock : public Input, public Position {
    public:
        //: Configure Constructor
   //! ARGS: port - such as "/dev/ttyd3"                         <BR>
   //! ARGS: baud - such as 38400, 19200, 9600, 14400, etc...    <BR>
   //! ARGS: sync - sync type.                                   <BR>
   //! ARGS: block - blocking                                    <BR>
   //! ARGS: numBrds - number of birds in flock,                 <BR>
   //! ARGS: transmit - transmitter unit number,                 <BR>
   //! ARGS: hemi - hemisphere to track from,                    <BR>
   //! ARGS: filt - filtering type,                              <BR>
   //! ARGS: report - flock report rate.                         <BR>
   //! ARGS: calfile - a calibration file, if "", then use none. <BR>
   //                                                       <BR>
   //! POST: configures internal data members,
   //+         doesn't actually talk to the FOB yet.
   Flock(const char* const port = "/dev/ttyd3",
      const int& baud = 38400,
      const int& sync = 1,
      const int& block = 0,
      const int& numBrds = 3,
      const int& transmit = 3,
      const BIRD_HEMI& hemi = LOWER_HEM,
      const BIRD_FILT& filt = AC_NARROW,
      const char& report = 'R',
      const char* const calfile = "");
   ~Flock();


    //: configure the flock with a config chunk
    virtual bool config(ConfigChunk* c);

    //: begin sampling
    int startSampling();

    //: stop sampling
    int stopSampling();

    //: sample data
    int sample();

    //: update to the sampled data.
    void updateData();

    //: return what chunk type is associated with this class.
    static std::string getChunkType() { return std::string("Flock");}

    //: Get the reciever transform
    //! ARGS: dev - is the reciever number
    //! POST: returns a pointer to the reciever's matrix
    //! NOTE: Clients of juggler should access tracker recievers as [0-n]
    //+  For example, if you have recievers 1,2, and 4 with transmitter on 3,
    //+  then you can access them, in order, as 0,1,2.
    Matrix* getPosData( int dev = 0); // 0 base

    //: Get time of last update for this receiver
    //! ARGS: dev - is the reciever number
    //! POST: returns a pointer to the reciever's timestamp
    TimeStaMp* getPosUpdateTime (int dev = 0);

    //: see if the flock is active or not
    inline const bool& isActive() const { return mFlockOfBirds.isActive(); }

    //: set the port to use
    //  this will be a string in the form of the native OS descriptor <BR>
    //  ex: unix - "/dev/ttyd3", win32 - "COM3" <BR>
    //! PRE: flock.isActive() must be false to use this function
    void     setPort( const char* const serialPort );

    //: get the port used
    //  this will be a string in the form of the native OS descriptor <BR>
    //  ex: unix - "/dev/ttyd3", win32 - "COM3"
    inline const char*  getPort() const { return mFlockOfBirds.getPort(); }


    //: set the baud rate
    //  this is generally 38400, consult flock manual for other rates. <BR>
    //! NOTE: flock.isActive() must be false to use this function
    void     setBaudRate( const int& baud );

    //: get the baud rate
    //  this is generally 38400, consult flock manual for other rates. <BR>
    inline const int&  getBaudRate()  const { return mFlockOfBirds.getBaudRate();}

    //: Set the unit number of the transmitter
    //! ARGS: Transmit - an integer that is the same as the dip switch
    //+         setting on the transmitter box (for the unit number) <BR>
    //! NOTE: flock.isActive() must be false to use this function
    void     setTransmitter( const int& Transmit );

    //: Get the unit number of the transmitter
    //! POST: returns an integer that is the same as the dip switch
    //         setting on the transmitter box (for the unit number) <BR>
    inline const int&  getTransmitter() const { return mFlockOfBirds.getTransmitter(); }


    //: Set the number of birds to use in the flock.
    //! ARGS: n - an integer number not more than the number of
    //+         birds attached to the system <BR>
    //! NOTE: flock.isActive() must be false to use this function
    void     setNumBirds( const int& n );

    //: Get the number of birds to use in the flock.
    //! POST: - an integer number not more than the number of
    //+         birds attached to the system <BR>
    inline const int&  getNumBirds() const { return mFlockOfBirds.getNumBirds(); }


    //: set the video sync type
    //  this option allows the Flock to syncronize its pulses with
    //  your video display.  This will eliminate most flicker caused
    //  by the magnetic distortion. <BR>
    //! NOTE: Refer to your flock manual for what number to use.
    //! PRE: flock.isActive() must be false to use this function
    void     setSync( const int& sync );

    //: Get the video sync type
    //  this option allows the Flock to syncronize its pulses with
    //  your video display.  This will eliminate most flicker caused
    //  by the magnetic distortion. <BR>
    //! POST: returns the sync type
    //! NOTE: Refer to your flock manual for what the return number means
    inline const int&  getSync() const {return mFlockOfBirds.getSync(); }


    //: Set blocking of flock
    //! PRE: flock.isActive() must be false to use this function
    //! NOTE: see flock manual for details.
    void     setBlocking( const int& blVal );

    //: Get flock's blocking type
    //! NOTE: see flock manual for details.
    inline const int&  getBlocking() const { return mFlockOfBirds.getBlocking(); }


    //: Set the type of filtering that the flock uses
    //! NOTE: flock.isActive() must be false to use this function
    void     setFilterType( const BIRD_FILT& f );

    //: Set the type of filtering that the flock uses
    inline const BIRD_FILT&  getFilterType() const { return mFlockOfBirds.getFilterType(); }


    //: Set the hemisphere that the transmitter transmits from.
    //! NOTE: flock.isActive() must be false to use this function
    void     setHemisphere( const BIRD_HEMI& h );

    //: Set the hemisphere that the transmitter transmits from.
    inline const BIRD_HEMI&  getHemisphere() const {return mFlockOfBirds.getHemisphere(); }


    //: Set the report rate that the flock uses
    //! NOTE: flock.isActive() must be false to use this function
    void     setReportRate( const char& rRate );

    //: Set the report rate that the flock uses
    inline const char& getReportRate() const {return mFlockOfBirds.getReportRate(); }


   //: get the x position of the i'th reciever
   inline float&      xPos( const int& i ) { return mFlockOfBirds.xPos( i ); }

   //: get the y position of the i'th reciever
   inline float&      yPos( const int& i ) { return mFlockOfBirds.yPos( i ); }

   //: get the z position of the i'th reciever
   inline float&      zPos( const int& i ) { return mFlockOfBirds.zPos( i ); }

   //: get the z rotation of the i'th reciever
   inline float&      zRot( const int& i ) { return mFlockOfBirds.zRot( i ); };

   //: get the y rotation of the i'th reciever
   inline float&      yRot( const int& i ) { return mFlockOfBirds.yRot( i ); }

   //: get the x rotation of the i'th reciever
   inline float&      xRot( const int& i ) { return mFlockOfBirds.xRot( i ); }



private:
    void controlLoop(void* nullParam);
    void positionCorrect(float&x,float&y,float&z);
    void initCorrectionTable(const char*);

    int getBirdIndex(int birdNum, int bufferIndex);

    vpr::Thread*   myThread;      // The thread doing the flock sampling

    aFlock mFlockOfBirds;
};

};

#endif
