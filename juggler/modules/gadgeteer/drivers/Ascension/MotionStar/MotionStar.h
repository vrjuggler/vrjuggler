/*
 *  File:          $RCSfile$
 *  Date modified: $Date$
 *  Version:       $Revision$
 *
 *
 *                                VR Juggler
 *                                    by
 *                              Allen Bierbaum
 *                             Christopher Just
 *                             Patrick Hartling
 *                            Carolina Cruz-Neira
 *                               Albert Baker
 *
 *                  Copyright (C) - 1997, 1998, 1999, 2000
 *              Iowa State University Research Foundation, Inc.
 *                            All Rights Reserved
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

    vjMotionStar(unsigned int format = 5,
                  unsigned int birdFormat = 5,
                  unsigned int birdsRequired = 10,
                  double birdRate = 90.0,
                  int runMode = 0
//                unsigned char reportRate
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
 
    //: see if the flock is active or not
    inline bool isActive() { return mMotionStar.isActive(); }
 
    void            setFormat(unsigned int i );
    inline unsigned int  getFormat()  { return mMotionStar.getFormat();}

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
