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


//-----------------------------------------------------------------------------
//: vjPosition-derived class for running an Ascension MotionStar device that
//+ is connected to a Flock of Birds.  It is a wrapper class for the real
//+ driver class 'aMotionStar'.
//
//  vjMotionStar adds to the aMotionStar class shared memory and threading.<br>
//  vjMotionStar is a positional device driver for the Flock of Birds.  The
//  config chunk in the constructor should set up all the settings.  For these
//  to be changed, the MotionStar has to be deleted and a new instance created
//  with an updated configchunk.
//  <br>
//
//! NOTE: Some functions still remain for changing the options of the
//+       MotionStar when its not in Sampling mode, but in order to stay
//+       consistent with the vjInput/vjPosition functionality these are only
//+       left for building apps without vjConfigChunks.
//! NOTE: A note on reciever access:
//+  Clients of Juggler should access tracker recievers as [0-n].  For
//+  example, if you have recievers 1,2, and 4 with transmitter on 3, then
//+  you can access the data, in order, as 0, 1, 2.
//
// See also: vjPosition
//-----------------------------------------------------------------------------
//!PUBLIC_API:
class vjMotionStar : public vjPosition {
public:

    // ------------------------------------------------------------------------
    //: Constructor.  This invokes the aMotionStar constructor and initializes
    //+ member variables.
    //
    //! PRE: None.
    //! POST: mMotionStar is initialized, and mThread is set to NULL.
    //
    //! ARGS: address       - The IP address or hostname of the MotionStar PC
    //+                       to which we are connecting.  This defaults to
    //+                       NULL.
    //! ARGS: port          - The server port to which we are connecting.  The
    //+                       default is 6000.
    //! ARGS: hemisphere    - The hemisphere in use on the transmitter.  The
    //+                       default is 3.
    //! ARGS: birdFormat    - The bird format.  The default is 4.
    //! ARGS: birdsRequired - The number of birds required.  The default is
    //+                       10.
    //! ARGS: runMode       - The run mode for the device.  The default is 0.
    //! ARGS: birdRate      - The bird rate.  The default is 90.0.
    // ------------------------------------------------------------------------
    vjMotionStar(char* address = NULL, const unsigned short port = 6000,
                 int hemisphere = 3, unsigned int birdFormat = 4,
                 unsigned int birdsRequired = 10, int runMode = 0,
                 double birdRate = 90.0);

    // ------------------------------------------------------------------------
    //: Destructor.  Sampling is stopped, and the data pool is deallocated.
    //
    //! PRE: None.
    //! POST: Sampling is stopped, and the data pool is deallocated.
    // ------------------------------------------------------------------------
    ~vjMotionStar();

    // ------------------------------------------------------------------------
    //: Configure the MotionStar with the given config chunk.
    //
    //! PRE: None.
    //! POST: If c is a valid config chunk, the device is configured using its
    //+       contents.  Otherwise, configuration fails and false is returned
    //+       to the caller.
    //
    //! ARGS: c - A pointer to a MotionStar config chunk.
    //
    //! RETURNS: true - The device was configured succesfully.
    //! RETURNS: false - The config chunk is invalid.
    // ------------------------------------------------------------------------
    virtual bool config(vjConfigChunk* c);

    // ------------------------------------------------------------------------
    //: Begin sampling.
    //
    //! PRE: myThread is either NULL or points to a valid vjThread object.
    //! POST: If the driver is not already active and a thread has not already
    //+       been created, an attempt is made to activate the device.  First,
    //+       a connection attempt to the server is made.  If successful, the
    //+       device is started.  If that too succeeds, a new threada is
    //+       created for sampling the device.
    //
    //! RETURNS: 0 - Sampling was unable to begin for some reason.
    //! RETURNS: 1 - Sampling was started successfully.
    // ------------------------------------------------------------------------
    int startSampling();

    // ------------------------------------------------------------------------
    //: Stop sampling.
    //
    //! PRE: myThread is either NULL or points to a valid vjThread object.
    //! POST: If the driver is not currently active, no stop attempt is made.
    //+       Otherwise, if myThread is non-NULL (assumed to point to a valid
    //+       vjThread object), the thread is killed and deleted, and a stop
    //+       attempt is made on the device.
    //
    //! RETURNS: 0 - Sampling could not be stopped.  This will occur if the
    //+              device is not active or the stop attempt failed.
    //! RETURNS: 1 - Sampling was stopped successfully.
    // ------------------------------------------------------------------------
    int stopSampling();

    // ------------------------------------------------------------------------
    //: Sample data.
    //
    //! PRE: mMotionStar has been initialized.
    //! POST: If the device is active, a sample is made from it.  Data are
    //+       read from each of the birds into the current read buffer, and
    //+       transforms are constructed based on what was received.  Once
    //+       this is done, the valid index is swapped for the next round of
    //+       sampling.
    //
    //! RETURNS: 0 - No sample could be made.
    // ------------------------------------------------------------------------
    int sample();

    // ------------------------------------------------------------------------
    //: Update to the sampled data.
    //
    //! PRE: None.
    //! POST: If the device is active, the valid data for all the birds are
    //+       copied into the current data so that both are valid.  The
    //+       current indices are then swapped.
    // ------------------------------------------------------------------------
    void updateData();

    // ------------------------------------------------------------------------
    //: Get the device name.
    //
    //! PRE: None.
    //! POST: The static string "vjMotionStar" is returned to the caller.
    //
    //! RETURNS: A constant static string.
    // ------------------------------------------------------------------------
    inline char*
    getDeviceName () {
        return "vjMotionStar";
    }

    // ------------------------------------------------------------------------
    //: Return what chunk type is associated with this class.
    //
    //! PRE: None.
    //! POST: A std::string constructed from a static constant string is
    //+       returned to the caller.
    //
    //! RETURNS: A std::string containing the chunk type for this device.
    // ------------------------------------------------------------------------
    static std::string
    getChunkType () {
        return std::string("MotionStar");
    }

    // ------------------------------------------------------------------------
    //: Get the reciever transform for the given bird number.  The birds are
    //+ zero-based.
    //
    //! PRE: None.
    //! POST: If the device is active, a pointer to the given receiver's
    //+       matrix is returned.
    //
    //! ARGS: dev - The receiver (bird) number.  It defaults to 0.
    //
    //! RETURNS: NULL - The device is not active.
    //! RETURNS: Non-NULL - A pointer to the given receiver's matrix.
    //
    //! NOTE: Clients of Juggler should access tracker recievers as [0-n]
    //+  For example, if you have recievers 1,2, and 4 with transmitter on 3,
    //+  then you can access them, in order, as 0,1,2.
    // ------------------------------------------------------------------------
    vjMatrix* getPosData(int dev = 0);

    // ------------------------------------------------------------------------
    //: Not used currently -- needed for interface.
    // ------------------------------------------------------------------------
    vjTimeStamp* getPosUpdateTime(int d);

    // ------------------------------------------------------------------------
    //: See if the device is active or not.
    //
    //! PRE: mMotionStar has been initialized.
    //! POST: The active state of the MotionStar driver is returned to the
    //+       caller.
    //
    //! RETURNS: true - The MotionStar driver is active.
    //! RETURNS: false - The MotionStar driver is inactive.
    // ------------------------------------------------------------------------
    inline bool
    isActive () {
        return mMotionStar.isActive();
    }

    // ------------------------------------------------------------------------
    //: Change the hemisphere of the transmitter.
    //
    //! PRE: mMotionStar has been initialized.
    //! POST: If the device is not active, the hemisphere for it is reset to
    //+       the given value.
    //
    //! ARGS: i - The new hemisphere to use.
    // ------------------------------------------------------------------------
    void setHemisphere(int i);

    // ------------------------------------------------------------------------
    //: Get the transmitter hemisphere currently in use by the device.
    //
    //! PRE: mMotionStar has been initialized.
    //! POST: The current hemisphere value is returned to the caller.
    //
    //! RETURNS: An integer value for the hemisphere in use.
    // ------------------------------------------------------------------------
    inline unsigned int
    getHemisphere () {
        return mMotionStar.getHemisphere();
    }

    // ------------------------------------------------------------------------
    //: Set the bird format to the given value.
    //
    //! PRE: mMotionStar has been initialized.
    //! POST: If the device is not active, the bird format is updated to the
    //+       new value.
    //
    //! ARGS: n - The new bird format to use.
    // ------------------------------------------------------------------------
    void setBirdFormat(unsigned int n);

    // ------------------------------------------------------------------------
    //: Get the bird format currently in use.
    //
    //! PRE: mMotionStar has been initialized.
    //! POST: The current bird format value is returned to the caller.
    //
    //! RETURNS: An unsigned integer value for the bird format.
    // ------------------------------------------------------------------------
    inline unsigned int
    getBirdFormat () {
        return mMotionStar.getBirdFormat();
    }

    // ------------------------------------------------------------------------
    //: Set the number of birds connected to the flock.
    //
    //! PRE: mMotionStar has been initialized.
    //! POST: If the device is not active, the number of birds is updated to
    //+       the new value.
    //
    //! ARGS: n - The new value for the number of birds in use.
    // ------------------------------------------------------------------------
    void setNumBirds(unsigned int n);

    // ------------------------------------------------------------------------
    //: Get the number of bird currently connected to the flock.
    //
    //! PRE: mMotionStar has been initialized.
    //! POST: The current number of connected birds is returned to the caller.
    //
    //! RETURNS: An unsigned integer value for the number of connected birds.
    // ------------------------------------------------------------------------
    inline unsigned int
    getNumBirds () {
        return mMotionStar.getNumBirds();
    }

    // ------------------------------------------------------------------------
    //: Set the bird rate to the given value.
    //
    //! PRE: mMotionStar has been initialized.
    //! POST: If the device is not active, the bird rate is set to the given
    //+       value.
    //
    //! ARGS: d - The new value for the bird rate.
    // ------------------------------------------------------------------------
    void setBirdRate(double d);

    // ------------------------------------------------------------------------
    //: Get the current bird rate.
    //
    //! PRE: mMotionStar has been initialized.
    //! POST: The current bird rate is returned to the caller.
    //
    //! RETURNS: A double-precision floating-point value for bird rate.
    // ------------------------------------------------------------------------
    inline double
    getBirdRate () {
        return mMotionStar.getBirdRate();
    }

    // ------------------------------------------------------------------------
    //: Set the run mode for the device.
    //
    //! PRE: mMotionStar has been initialized.
    //! POST: If the device is not active, the run mode is set to the given
    //+       value.
    //
    //! ARGS: i - The new value for the run mode.
    // ------------------------------------------------------------------------
    void setRunMode(int i);

    // ------------------------------------------------------------------------
    //: Get the current run mode for the device.
    //
    //! PRE: mMotionStar has been initialized.
    //! POST: The current run mode is returned to the caller.
    //
    //! RETURNS: An integer value for run mode.
    // ------------------------------------------------------------------------
    inline int
    getRunMode() {
        return mMotionStar.getRunMode();
    }

    // ------------------------------------------------------------------------
    //: Set the report rate for the device.
    //
    //! PRE: mMotionStar has been initialized.
    //! POST: If the device is not active, the report rate is set to the given
    //+       byte value.
    //
    //! ARGS: ch - The new value for the report rate.
    // ------------------------------------------------------------------------
    void setReportRate(unsigned char ch);

    // ------------------------------------------------------------------------
    //: Get the current report rate for the device.
    //
    //! PRE: mMotionStar has been initialized.
    //! POST: The current report rate is returned to the caller.
    //
    //! RETURNS: An unsigned character value for report rate.
    // ------------------------------------------------------------------------
    inline unsigned char
    getReportRate () {
        return mMotionStar.getReportRate();
    }

    // ------------------------------------------------------------------------
    //: Set the address (either IP address or hostname) for the server.
    //
    //! PRE: mMotionStar has been initialized.
    //! POST: If the device is not active, the address is set to the given
    //+       value.
    //
    //! ARGS: n - The new value for the server address.
    // ------------------------------------------------------------------------
    void setAddress(const char* n);

    // ------------------------------------------------------------------------
    //: Get the current server address for the device.
    //
    //! PRE: mMotionStar has been initialized.
    //! POST: The current server address is returned to the caller.
    //
    //! RETURNS: A character array naming the server address.
    // ------------------------------------------------------------------------
    inline char*
    getAddress () {
        return mMotionStar.getAddress();
    }
 
    // ------------------------------------------------------------------------
    //: Set the port on the server to which we connect.
    //
    //! PRE: mMotionStar has been initialized.
    //! POST: If the device is not active, the port is set to the given value.
    //
    //! ARGS: port - The new value for the server port.
    // ------------------------------------------------------------------------
    void setServerPort(const unsigned short port);

    // ------------------------------------------------------------------------
    //: Get the server port.
    //
    //! PRE: mMotionStar has been initialized.
    //! POST: The current server port is returned to the caller.
    //
    //! RETURNS: An unsigned short integer containing the server port number.
    // ------------------------------------------------------------------------
    inline unsigned short
    getServerPort () {
        return mMotionStar.getServerPort();
    }

    // ------------------------------------------------------------------------
    //: Get the x position of the i'th reciever.
    //
    //! PRE: mMotionStar has been initialized.
    //! POST: The x position for the given bird number is returned to the
    //+       caller.
    //
    //! RETURNS: A floating-point value for the current x position of the
    //+          given bird.
    // ------------------------------------------------------------------------
    inline float
    xPos (int i) {
        return mMotionStar.xPos(i);
    }

    // ------------------------------------------------------------------------
    //: Get the y position of the i'th reciever.
    //
    //! PRE: mMotionStar has been initialized.
    //! POST: The y position for the given bird number is returned to the
    //+       caller.
    //
    //! RETURNS: A floating-point value for the current y position of the
    //+          given bird.
    // ------------------------------------------------------------------------
    inline float
    yPos (int i) {
        return mMotionStar.yPos(i);
    }

    // ------------------------------------------------------------------------
    //: Get the z position of the i'th reciever.
    //
    //! PRE: mMotionStar has been initialized.
    //! POST: The z position for the given bird number is returned to the
    //+       caller.
    //
    //! RETURNS: A floating-point value for the current z position of the
    //+          given bird.
    // ------------------------------------------------------------------------
    inline float
    zPos (int i) {
        return mMotionStar.zPos(i);
    }

    // ------------------------------------------------------------------------
    //: Get the z rotation of the i'th reciever.
    //
    //! PRE: mMotionStar has been initialized.
    //! POST: The z rotation for the given bird number is returned to the
    //+       caller.
    //
    //! RETURNS: A floating-point value for the current z rotation of the
    //+          given bird.
    // ------------------------------------------------------------------------
    inline float
    zRot (int i) {
        return mMotionStar.zRot(i);
    }

    // ------------------------------------------------------------------------
    //: Get the y rotation of the i'th reciever.
    //
    //! PRE: mMotionStar has been initialized.
    //! POST: The y rotation for the given bird number is returned to the
    //+       caller.
    //
    //! RETURNS: A floating-point value for the current y rotation of the
    //+          given bird.
    // ------------------------------------------------------------------------
    inline float
    yRot (int i) {
        return mMotionStar.yRot( i);
    }

    // ------------------------------------------------------------------------
    //: Get the x rotation of the i'th reciever.
    //
    //! PRE: mMotionStar has been initialized.
    //! POST: The x rotation for the given bird number is returned to the
    //+       caller.
    //
    //! RETURNS: A floating-point value for the current x rotation of the
    //+          given bird.
    // ------------------------------------------------------------------------
    inline float
    xRot (int i) {
        return mMotionStar.xRot(i);
    }

private:
    // ------------------------------------------------------------------------
    //: Unimplemented!
    // ------------------------------------------------------------------------
    void positionCorrect(float& x, float& y, float& z);

    // ------------------------------------------------------------------------
    //: Unimplemented!
    // ------------------------------------------------------------------------
    void initCorrectionTable(const char*);

    // ------------------------------------------------------------------------
    //: Helper to return the index for theData array given the birdNum we are
    //+ dealing with and the bufferIndex to read.
    //
    //! PRE: None.
    //! POST: The bird index into the theData array is calcualted and
    //+       returned to the caller.  An assertion is made to verify that the
    //+       index to be returned is valid.
    //
    //! ARGS: birdNum     - The bird number in which we are interested.
    //! ARGS: bufferIndex - The value of current, progress, or valid (it is
    //+                     an offset in the array).
    //
    //! RETURNS: An unsigned integer value given the index into theData for
    //+          the given bird number and buffer index.
    // ------------------------------------------------------------------------
    unsigned int getBirdIndex(int birdNum, int bufferIndex);

    vjThread*   myThread;      // The thread doing the flock sampling
    aMotionStar mMotionStar;   // Actual MotionStar device driver
};


#endif	/* _VJ_ASCENSION_MOTION_STAR_H_ */
