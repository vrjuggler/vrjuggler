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

#ifndef _ASCENSION_MOTION_STAR_H_
#define _ASCENSION_MOTION_STAR_H_

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>


namespace BIRDNET {

// BIRDNET command/response codes.
const unsigned char MSG_WAKE_UP        = 10;
const unsigned char RSP_WAKE_UP        = 20;
const unsigned char MSG_SHUT_DOWN      = 11;
const unsigned char RSP_SHUT_DOWN      = 21;
const unsigned char MSG_GET_STATUS     = 101;
const unsigned char MSG_SEND_SETUP     = 102;
const unsigned char MSG_SINGLE_SHOT    = 103;
const unsigned char MSG_RUN_CONTINUOUS = 104;
const unsigned char MSG_STOP_DATA      = 105;
const unsigned char RSP_GET_STATUS     = 201;
const unsigned char RSP_GET_SETUP      = 202;
const unsigned char RSP_RUN_CONTINUOUS = 204;
const unsigned char RSP_STOP_DATA      = 205;
const unsigned char DATA_PACKET_MULTI  = 210;
const unsigned char RSP_ILLEGAL        = 40;
const unsigned char RSP_UNKNOWN        = 50;
const unsigned char MSG_SYNC_SEQUENCE  = 30;
const unsigned char RSP_SYNC_SEQUENCE  = 31;

//: BIRDNET singleBirdStatus packet.  It is 16 bytes long.
struct SINGLE_BIRD_STATUS {
    unsigned char	status;      //: Device status
    unsigned char	id;          //: Device type
    unsigned short int	softwareRev; //: Software revision of firmware
    unsigned char	errorCode;   //: Reserved for future use
    unsigned char	setup;       //: Device setup parameters
    unsigned char	dataFormat;  //: Data Format Code and Data Size
    unsigned char	reportRate;  //: Rate at which data is generated
                                     //+ relative to the sample rate
    unsigned short int	scaling;     //: Full scale output
    unsigned char	hemisphere;  //: Bird's hemisphere of operation
    unsigned char	FBBaddress;  //: FBB address
    unsigned char	spare1;      //: Reserved
    unsigned short int	spare2;      //: Reserved
};

//: BIRDNET FILTER_TABLE structure.  This is for an ERC/ERT (Extended Range
//+ Controller/Extended Range Transmitter).
struct FILTER_TABLE {
    unsigned short int	entry[7];
};

//: BIRDNET XYZREF_TABLE structure.  Word 1 is azimuth, word 2 is elevation
//+ and word 3 is roll.
struct ANGLES_TABLE {
    unsigned short int	angle[3];
};

//: BIRDNET indiviual Flock status packet.
struct BIRD_STATUS {
    struct SINGLE_BIRD_STATUS	status;
    struct FILTER_TABLE		alphaMin;
    struct FILTER_TABLE		alphaMax;
    struct FILTER_TABLE		Vm;
    struct ANGLES_TABLE		xyzReferenceFrame;
    struct ANGLES_TABLE		xyzAngleAlign;
};

//: BIRDNET packet header.  It is 16 bytes long.
struct HEADER {
    unsigned short int	sequence; //: 16-bit sequence number

    unsigned short int	milliseconds; //: Interval time between packets
    unsigned char	time[4];  //: Seconds since 00:00:00 Jan. 1, 1970 (GMT)

    unsigned char	type;     //: Command/response code
    unsigned char	xtype;    //: Extension to command/response code
    unsigned char	protocol; //: Protocol for this packet (BIRDNET is 3)

    unsigned char	error_code; //: Error code for an event at the server
                                    //+ concerning the previous client packet
    unsigned short int	error_code_extension; //: Additional error code info

    unsigned short int	number_bytes; //: Size of the following data field
};

//: BIRDNET general system status packet.  It is 16 bytes long.
struct RSP_GET_STATUS_all {
    unsigned char	all;                //: General system status
    unsigned char	FBBerror;           //: Reserved
    unsigned char	flockNumber;        //: Number of devices on FBB bus
    unsigned char	serverNumber;       //: Number of servers in system
    unsigned char	transmitterNumber;  //: Address of current active xmtr
    char		measurementRate[6]; //: Measurement rate * 1000 (text)
    unsigned char	chassisNumber;      //: Chassis ID number
    unsigned char	chassisDevices;     //: Number of devices in chassis
    unsigned char	firstAddress;       //: FBB address of first device
    unsigned short int	softwareRevision;   //: Software revision of server app
};

//: BIRDNET DATA_PACKET packet.  The type field of the header for this packet
//+ is 210, and the xtype field can be ignored.
// XXX: Is it safe for the buffer to be fixed at 2048 bytes?
struct DATAPACKET {
    struct HEADER	header;
    unsigned char	buffer[2048];
};

};	// namspace BIRDNET


class aMotionStar {
public:
    // ------------------------------------------------------------------------
    //: Constructor.  This initializes member variables and determines the
    //+ endianness of the host machine.
    //
    //! PRE: None.
    //! POST: Important member variables determining the setup of the driver
    //+       are initialized, and the endianness of the host machine is
    //+       determined.  This is used later when reading data words from the
    //+       server's response packets.
    //
    //! ARGS: address       - The IP address or hostname of the MotionStar
    //+                        server to which we are connecting.
    //! ARGS: port          - The port on the server to which we connect.
    //! ARGS: hemisphere    - The hemisphere in use on the transmitter.
    //! ARGS: birdFormat    - The bird format.
    //! ARGS: birdsRequired - The number of birds required.
    //! ARGS: runMode       - The run mode for the device.
    //! ARGS: birdRate      - The bird rate.
    //! ARGS: reportRate    - The report rate.
    // ------------------------------------------------------------------------
    aMotionStar(char* address, const unsigned short port, int hemisphere,
                unsigned int birdFormat, unsigned int birdsRequired,
                int runMode, double birdRate, unsigned char reportRate);

    // ------------------------------------------------------------------------
    //: Destructor.
    //
    //! PRE: None.
    //! POST: The device is stopped, and the memory allocated for the server
    //+       address is released.
    // ------------------------------------------------------------------------
    ~aMotionStar();

    // ------------------------------------------------------------------------
    //: Initializes the driver, setting the status for each bird.
    //
    //! PRE: The server address (either IP address or hostname) has been set.
    //! POST: A connection attempt is made to the server.  If successful, the
    //+       socket is set up in connected mode.  Each bird has its status
    //+       set regardless of the connection attempt results.  If the
    //+       connection attempt fails, error status is returned to the
    //+       caller.
    //
    //! RETURNS:  0 - Successful startup.
    //! RETURNS: -1 - Failed to connect to server.
    //! RETURNS: -2 - No address has been set for the server.
    // ------------------------------------------------------------------------
    int start();

    // ------------------------------------------------------------------------
    //: Stops the driver.
    //
    //! PRE: The socket has been initialized and is currently open to
    //+      receive data from the server.
    //! POST: A MSG_STOP_DATA command is sent to the server.
    // ------------------------------------------------------------------------
    void stop();

    // ------------------------------------------------------------------------
    //: Based on the current run mode, a single sample is taken (run mode is
    //+ 1), or continuous samples are taken (run mode is 0).
    //
    //! PRE: The socket has been initialized and is currently open to
    //+      receive data from the server.
    //! POST:
    // ------------------------------------------------------------------------
    void sample();

    // ------------------------------------------------------------------------
    //: Returns whether the MotionStar is active or not.
    //
    //! PRE: None.
    //! POST: A reference to active is returned to the caller.
    //
    //! RETURNS: A reference to a boolean variable stating whether or not the
    //+          device is active.
    // ------------------------------------------------------------------------
    inline bool&
    isActive () {
        return mActive;
    }

    // ------------------------------------------------------------------------
    //: Change the hemisphere of the transmitter.
    //
    //! PRE: None.
    //! POST: The hemisphere for it is reset to the given value.
    //
    //! ARGS: hemisphere - The new hemisphere to use.
    // ------------------------------------------------------------------------
    inline void
    setHemisphere (int hemisphere) {
        mHemisphere = hemisphere;
    }

    // ------------------------------------------------------------------------
    //: Get the transmitter hemisphere currently in use by the device.
    //
    //! PRE: None.
    //! POST: The current hemisphere value is returned to the caller.
    //
    //! RETURNS: An unsigned integer value for the hemisphere in use.
    // ------------------------------------------------------------------------
    inline unsigned int
    getHemisphere () {
        return mHemisphere;
    }

    // ------------------------------------------------------------------------
    //: Set the bird format to the given value.
    //
    //! PRE: None.
    //! POST: The bird format is updated to the new value.
    //
    //! ARGS: format - The new bird format to use.
    // ------------------------------------------------------------------------
    inline void
    setBirdFormat (unsigned int format) {
        mBirdFormat = format;
    }

    // ------------------------------------------------------------------------
    //: Get the bird format currently in use.
    //
    //! PRE: None.
    //! POST: The current bird format value is returned to the caller.
    //
    //! RETURNS: An unsigned integer value for the bird format.
    // ------------------------------------------------------------------------
    inline unsigned int
    getBirdFormat () {
        return mBirdFormat;
    }

    // ------------------------------------------------------------------------
    //: Set the number of birds connected to the Flock.
    //
    //! PRE: None.
    //! POST: The number of birds is updated to the new value.
    //
    //! ARGS: num_birds - The new value for the number of birds in use.
    // ------------------------------------------------------------------------
    inline void
    setNumBirds (unsigned int num_birds) {
        mBirdsRequired = num_birds;
    }

    // ------------------------------------------------------------------------
    //: Get the number of bird currently connected to the flock.
    //
    //! PRE: None.
    //! POST: The current number of connected birds is returned to the caller.
    //
    //! RETURNS: An unsigned integer value for the number of connected birds.
    // ------------------------------------------------------------------------
    inline unsigned int
    getNumBirds () {
        return mBirdsRequired;
    }

    // ------------------------------------------------------------------------
    //: Set the bird rate to the given value.
    //
    //! PRE: None.
    //! POST: The bird rate is set to the given value.
    //
    //! ARGS: rate - The new value for the bird rate.
    // ------------------------------------------------------------------------
    inline void
    setBirdRate (double rate) {
        mBirdRate = rate;
    }

    // ------------------------------------------------------------------------
    //: Get the current bird rate.
    //
    //! PRE: None.
    //! POST: The current bird rate is returned to the caller.
    //
    //! RETURNS: A double-precision floating-point value for bird rate.
    // ------------------------------------------------------------------------
    inline double
    getBirdRate () {
        return mBirdRate;
    }

    // ------------------------------------------------------------------------
    //: Set the run mode for the device.
    //
    //! PRE: None.
    //! POST: the run mode is set to the given value.
    //
    //! ARGS: mode - The new value for the run mode.
    // ------------------------------------------------------------------------
    inline void
    setRunMode (int mode) {
        mRunMode = mode;
    }

    // ------------------------------------------------------------------------
    //: Get the current run mode for the device.
    //
    //! PRE: None.
    //! POST: The current run mode is returned to the caller.
    //
    //! RETURNS: An integer value for run mode.
    // ------------------------------------------------------------------------
    inline int
    getRunMode () {
        return mRunMode;
    }

    // ------------------------------------------------------------------------
    //: Set the report rate for the device.
    //
    //! PRE: None.
    //! POST: The report rate is set to the given byte value.
    //
    //! ARGS: rate - The new value for the report rate.
    // ------------------------------------------------------------------------
    inline void
    setReportRate (unsigned char rate) {
        mReportRate = rate;
    }

    // ------------------------------------------------------------------------
    //: Get the current report rate for the device.
    //
    //! PRE: None.
    //! POST: The current report rate is returned to the caller.
    //
    //! RETURNS: An unsigned character value for report rate.
    // ------------------------------------------------------------------------
    inline unsigned char
    getReportRate () {
        return mReportRate;
    }

    // ------------------------------------------------------------------------
    //: Set the address (either IP address or hostname) for the server.
    //
    //! PRE: None.
    //! POST: If an address has already been set (i.e., mAddress is non-NULL),
    //+       that memory is freed.  A copy of addr is stored in mAddress.
    //
    //! ARGS: addr - The new value for the server address.
    // ------------------------------------------------------------------------
    void setAddress(const char* addr);

    // ------------------------------------------------------------------------
    //: Get the current server address for the device.
    //
    //! PRE: None.
    //! POST: The current value of mAddress is returned to the caller.
    //
    //! RETURNS: NULL - No address has been set.
    //! RETURNS: Non-NULL - A character array naming the server address.
    // ------------------------------------------------------------------------
    inline char*
    getAddress () {
        return mAddress;
    }

    // ------------------------------------------------------------------------
    //: Set the port on the server to which we connect.
    //
    //! PRE: None.
    //! POST: The value in port is stored in mPort.
    //
    //! ARGS: port - The new value for the server port.
    // ------------------------------------------------------------------------
    inline void
    setServerPort (const unsigned short port) {
        mPort = port;
    }

    // ------------------------------------------------------------------------
    //: Get the server port.
    //
    //! PRE: None.
    //! POST: The current value of mPort is returned to the caller.
    //
    //! RETURNS: An unsigned short integer containing the server port number.
    // ------------------------------------------------------------------------
    inline unsigned short
    getServerPort () {
        return mPort;
    }

    // ------------------------------------------------------------------------
    //: Get the x position of the i'th bird.
    //
    //! PRE: mPosinfo has valid data in it.
    //! POST: The x position for the given bird number is returned to the
    //+       caller.
    //
    //! RETURNS: A floating-point value for the current x position of the
    //+          given bird.
    // ------------------------------------------------------------------------
    inline float
    xPos (int i) {
        return mPosinfo[i][0];
    }

    // ------------------------------------------------------------------------
    //: Get the y position of the i'th bird.
    //
    //! PRE: mPosinfo has valid data in it.
    //! POST: The y position for the given bird number is returned to the
    //+       caller.
    //
    //! RETURNS: A floating-point value for the current y position of the
    //+          given bird.
    // ------------------------------------------------------------------------
    inline float
    yPos (int i) {
        return mPosinfo[i][1];
    }

    // ------------------------------------------------------------------------
    //: Get the z position of the i'th bird.
    //
    //! PRE: mPosinfo has valid data in it.
    //! POST: The z position for the given bird number is returned to the
    //+       caller.
    //
    //! RETURNS: A floating-point value for the current z position of the
    //+          given bird.
    // ------------------------------------------------------------------------
    inline float
    zPos (int i) {
        return mPosinfo[i][2];
    }

    // ------------------------------------------------------------------------
    //: Get the z rotation of the i'th bird.
    //
    //! PRE: mPosinfo has valid data in it.
    //! POST: The z rotation for the given bird number is returned to the
    //+       caller.
    //
    //! RETURNS: A floating-point value for the current z rotation of the
    //+          given bird.
    // ------------------------------------------------------------------------
    inline float
    zRot (int i) {
        return mPosinfo[i][3];
    }

    // ------------------------------------------------------------------------
    //: Get the y rotation of the i'th bird.
    //
    //! PRE: mPosinfo has valid data in it.
    //! POST: The y rotation for the given bird number is returned to the
    //+       caller.
    //
    //! RETURNS: A floating-point value for the current y rotation of the
    //+          given bird.
    // ------------------------------------------------------------------------
    inline float
    yRot (int i) {
        return mPosinfo[i][4];
    }

    // ------------------------------------------------------------------------
    //: Get the x rotation of the i'th bird.
    //
    //! PRE: mPosinfo has valid data in it.
    //! POST: The x rotation for the given bird number is returned to the
    //+       caller.
    //
    //! RETURNS: A floating-point value for the current x rotation of the
    //+          given bird.
    // ------------------------------------------------------------------------
    inline float
    xRot (int i) {
        return mPosinfo[i][5];
    }

private:
    // ------------------------------------------------------------------------
    //: Send a wakeup call to the MotionStar server.
    //
    //! PRE: The socket has been initialized and is currently open to
    //+      send and receive data from the server.
    //! POST: A MSG_WAKE_UP is sent to the server, and an acknowledgment is
    //+       expected before processing continues.
    // ------------------------------------------------------------------------
    void sendWakeup();

    // ------------------------------------------------------------------------
    //: Tell the MotionStar server to sample continuously.
    //
    //! PRE: The socket has been initialized and is currently open to
    //+      send and receive data from the server.
    //! POST: A MSG_RUN_CONTINUOUS is sent to the server, and an
    //+       acknowledgment is expected before processing continues.
    // ------------------------------------------------------------------------
    void runContinuous();

    // ------------------------------------------------------------------------
    //: Request a single sampling of the MotionStar server.
    //
    //! PRE: The socket has been initialized and is currently open to
    //+      send and receive data from the server.
    //! POST: A MSG_SINGLE_SHOT is sent to the server, and an acknowledgement
    //+       is expected before processing continues.
    // ------------------------------------------------------------------------
    void singleShot();

    // ------------------------------------------------------------------------
    //: Get the system status.
    //
    //! PRE: The socket has been initialized and is currently open to
    //+      send and receive data from the server.
    //! POST: A MSG_GET_STATUS is sent to the server, and an acknowledgement
    //+       is expected before processing continues.  Once the status is
    //+       received, a message is written to stdout describing the system
    //+       status.
    // ------------------------------------------------------------------------
    void getSystemStatus();

    // ------------------------------------------------------------------------
    //: Set the system status.
    //
    //! PRE: The socket has been initialized and is currently open to
    //+      send and receive data from the server.
    //! POST: A MSG_SEND_SETUP is sent to the server, and an acknowledgement
    //+       is expected before processing continues.
    // ------------------------------------------------------------------------
    void setSystemStatus();

    // ------------------------------------------------------------------------
    //: Get the status of an individual bird.
    //
    //! PRE: The socket has been initialized and is currently open to
    //+      send and receive data from the server.
    //! POST: A MSG_GET_STATUS is sent to the server, and a response is
    //+       expected before processing continues.
    //
    //! ARGS: fbb_addr - The bird whose status is being requested.
    // ------------------------------------------------------------------------
    void getBirdStatus(unsigned char);

    // ------------------------------------------------------------------------
    //: Set the status of an individual bird.
    //
    //! PRE: The socket has been initialized and is currently open to
    //+      send and receive data from the server.
    //! POST: A MSG_SEND_SETUP is sent to the server, and a response is
    //+       expected before processing continues.
    //
    //! ARGS: fbb_addr - The bird whose status is being set.
    // ------------------------------------------------------------------------
    void setBirdStatus(unsigned char);

    // ------------------------------------------------------------------------
    //: Combine the two given bytes (passed as high byte and low byte
    //+ respectively) into a single word in host byte order.  This is used for
    //+ reading bytes from the packets and converting them into usable values.
    //
    //! PRE: None.
    //! POST: The two given bytes are combined into a single word that is
    //+       returned to the caller.
    //
    //! ARGS: high_byte - The byte that will become the high byte of the
    //+                   returned word.
    //! ARGS: low_byte - The byte that will become the low byte of the
    //+                  returned word.
    //
    //! RETURNS: A 16-bit word in host byte order that is the combination of
    //+          the two given bytes.
    // ------------------------------------------------------------------------
    inline short
    toShort (char high_byte, char low_byte) {
        union {
            char c[sizeof(short)];
            short value;
        } result;

        // If this is a big-endian host, put the high byte in the first byte of
        // result.value and the low byte in the second byte.
        if ( m_big_endian ) {
            result.c[0] = high_byte;
            result.c[1] = low_byte;
        }
        // Otherwise, put the low byte in the first byte and the high byte in
        // the second byte.
        else {
            result.c[0] = low_byte;
            result.c[1] = high_byte;
        }

        return result.value;
    }

    // ------------------------------------------------------------------------
    //: Get the value of the translation along the x-axis from the response
    //+ buffer as a single-precision floating-point value in inches.
    //
    //! PRE: A valid data packet has been read into the response buffer.
    //! POST: The x-axis translation is read from the data packet, converted
    //+       into a usable form and returned to the caller.
    //
    //! ARGS: offset - The current offset into the response buffer.
    //
    //! RETURNS: The x-axis translation as a floating-point value in inches.
    // ------------------------------------------------------------------------
    inline float
    getXPos (const int offset) {
        return (m_xmtr_pos_scale *
                   (float) toShort(mResponse.buffer[2 + offset],
                                   mResponse.buffer[3 + offset]) /
                   m_xmtr_divisor);
    }

    // ------------------------------------------------------------------------
    //: Get the value of the translation along the y-axis from the response
    //+ buffer as a single-precision floating-point value in inches.
    //
    //! PRE: A valid data packet has been read into the response buffer.
    //! POST: The y-axis translation is read from the data packet, converted
    //+       into a usable form and returned to the caller.
    //
    //! ARGS: offset - The current offset into the response buffer.
    //
    //! RETURNS: The y-axis translation as a floating-point value in inches.
    // ------------------------------------------------------------------------
    inline float
    getYPos (const int offset) {
        return (m_xmtr_pos_scale *
                   (float) toShort(mResponse.buffer[4 + offset],
                                   mResponse.buffer[5 + offset]) /
                   m_xmtr_divisor);
    }

    // ------------------------------------------------------------------------
    //: Get the value of the translation along the z-axis from the response
    //+ buffer as a single-precision floating-point value in inches.
    //
    //! PRE: A valid data packet has been read into the response buffer.
    //! POST: The z-axis translation is read from the data packet, converted
    //+       into a usable form and returned to the caller.
    //
    //! ARGS: offset - The current offset into the response buffer.
    //
    //! RETURNS: The z-axis translation as a floating-point value in inches.
    // ------------------------------------------------------------------------
    inline float
    getZPos (const int offset) {
        return (m_xmtr_pos_scale *
                   (float) toShort(mResponse.buffer[6 + offset],
                                   mResponse.buffer[7 + offset]) /
                   m_xmtr_divisor);
    }

    // ------------------------------------------------------------------------
    //: Get the value of the rotation about along the z-axis from the response
    //+ buffer as a single-precision floating-point value in degrees.
    //
    //! PRE: A valid data packet has been read into the response buffer.
    //! POST: The z-axis rotation is read from the data packet, converted
    //+       into a usable form and returned to the caller.
    //
    //! ARGS: offset - The current offset into the response buffer.
    //
    //! RETURNS: The z-axis rotation as a floating-point value in degrees.
    // ------------------------------------------------------------------------
    inline float
    getZRot (const int offset) {
        return (m_xmtr_rot_scale *
                   (float) toShort(mResponse.buffer[8 + offset],
                                   mResponse.buffer[9 + offset]) /
                    m_xmtr_divisor);
    }

    // ------------------------------------------------------------------------
    //: Get the value of the rotation about along the y-axis from the response
    //+ buffer as a single-precision floating-point value in degrees.
    //
    //! PRE: A valid data packet has been read into the response buffer.
    //! POST: The y-axis rotation is read from the data packet, converted
    //+       into a usable form and returned to the caller.
    //
    //! ARGS: offset - The current offset into the response buffer.
    //
    //! RETURNS: The y-axis rotation as a floating-point value in degrees.
    // ------------------------------------------------------------------------
    inline float
    getYRot (const int offset) {
        return (m_xmtr_rot_scale *
                   (float) toShort(mResponse.buffer[10 + offset],
                                   mResponse.buffer[11 + offset]) /
                    m_xmtr_divisor);
    }

    // ------------------------------------------------------------------------
    //: Get the value of the rotation about along the x-axis from the response
    //+ buffer as a single-precision floating-point value in degrees.
    //
    //! PRE: A valid data packet has been read into the response buffer.
    //! POST: The x-axis rotation is read from the data packet, converted
    //+       into a usable form and returned to the caller.
    //
    //! ARGS: offset - The current offset into the response buffer.
    //
    //! RETURNS: The x-axis rotation as a floating-point value in degrees.
    // ------------------------------------------------------------------------
    inline float
    getXRot (const int offset) {
        return (m_xmtr_rot_scale *
                   (float) toShort(mResponse.buffer[12 + offset],
                                   mResponse.buffer[13 + offset]) /
                    m_xmtr_divisor);
    }

    // ------------------------------------------------------------------------
    //: Print out the MotionStar information.
    //
    //! PRE: mPosinfo has valid data in it.
    //! POST: The contents of mPosinfo are printed to stdout.
    // ------------------------------------------------------------------------
    void printInfo();

    // ------------------------------------------------------------------------
    //: Print out the MotionStar's header information.
    //
    //! PRE: None.
    //! POST: This currently does nothing.
    // ------------------------------------------------------------------------
    void displayHdr();

    // ------------------------------------------------------------------------
    //: Print the error message that corresponds to the given error code.
    //+ The message is based on the table on page 140 of the MotionStar
    //+ manual.
    //
    //! PRE: None.
    //! POST: A human-readable error message is printed to stderr.
    //
    //! ARGS: err_code - The single-byte error code.
    // ------------------------------------------------------------------------
    void printError(const unsigned char err_code);

    float                   mPosinfo[3][6];
    bool                    mActive;
    char*                   mAddress;
    unsigned char           mServerNumber;
    unsigned char           mChassisNumber;
    unsigned char           mChassisDevices;
    unsigned char           mFirstAddress;
    unsigned short int      mSoftwareRevision;
    unsigned char           mTransmitterNumber;
    double                  mRealRate;
    unsigned int            mStatusSize;
    unsigned int            mTemp;
    unsigned char           mErtFlag;
    unsigned char           mButtonState;

    struct BIRDNET::BIRD_STATUS mBird[120];
    struct BIRDNET::HEADER      mCommand;
    struct BIRDNET::HEADER*     mLpCommand;
    struct BIRDNET::DATAPACKET  mResponse;
    struct BIRDNET::DATAPACKET* mLpResponse;
 
    int           mSocket;
    unsigned int  mSequenceNumber;

    char* mNewptr;

    int             mNumberBytes;
    int             mBytesReceived;
    int             mSockfd;
    int             mDigit;
    int             mNo;
    char            mRate[7];

    int             mHemisphere;
    unsigned int    mBirdFormat;
    unsigned int    mBirdsRequired;
    double          mBirdRate;
    int             mRunMode;
    unsigned char   mAddr;
    unsigned short  mPort;
    unsigned char   mReportRate;

    float           m_xmtr_pos_scale;   //: Transmitter position scaling factor
    float           m_xmtr_rot_scale;   //: Transmitter position scaling factor
    float           m_xmtr_divisor;     //: Number by which all returned values
                                        //+ must be divided
    bool            m_big_endian;
};


#endif	/* _ASCENSION_MOTION_STAR_H_ */
