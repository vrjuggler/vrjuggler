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

#define MYPORT 4000 /* Indy port number */
#define TCP_PORT 6000 /* bird server port */


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

//    time_t		time;   SGI formatted as 6 bytes (see time_t) */
//    unsigned short int	tm1;
//    unsigned short int	tm2;

    unsigned char	time[4];  //: Seconds since 00:00:00 Jan. 1, 1970 (GMT)
    unsigned short int	milliseconds; //: Interval time between packets

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
    //! ARGS: _address       - The IP address or hostname of the MotionStar
    //+                        server to which we are connecting.
    //! ARGS: _hemisphere    - The hemisphere in use on the transmitter.
    //! ARGS: _birdFormat    - The bird format.
    //! ARGS: _birdsRequired - The number of birds required.
    //! ARGS: _runMode       - The run mode for the device.
    //! ARGS: _birdRate      - The bird rate.
    //! ARGS: _reportRate    - The report rate.
    // ------------------------------------------------------------------------
    aMotionStar(char* _address, int _hemisphere, unsigned int _birdFormat,
                unsigned int _birdsRequired, int _runMode, double _birdRate,
                unsigned char _reportRate);

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
        return active;
    }

    // ------------------------------------------------------------------------
    //: Change the hemisphere of the transmitter.
    //
    //! PRE: None.
    //! POST: The hemisphere for it is reset to the given value.
    //
    //! ARGS: n - The new hemisphere to use.
    // ------------------------------------------------------------------------
    inline void
    setHemisphere (int n) {
        hemisphere = n;
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
        return hemisphere;
    }

    // ------------------------------------------------------------------------
    //: Set the bird format to the given value.
    //
    //! PRE: None.
    //! POST: The bird format is updated to the new value.
    //
    //! ARGS: n - The new bird format to use.
    // ------------------------------------------------------------------------
    inline void
    setBirdFormat (unsigned int n) {
        birdFormat = n;
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
        return birdFormat;
    }

    // ------------------------------------------------------------------------
    //: Set the number of birds connected to the Flock.
    //
    //! PRE: None.
    //! POST: The number of birds is updated to the new value.
    //
    //! ARGS: n - The new value for the number of birds in use.
    // ------------------------------------------------------------------------
    inline void
    setNumBirds (unsigned int n) {
        birdsRequired = n;
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
        return birdsRequired;
    }

    // ------------------------------------------------------------------------
    //: Set the bird rate to the given value.
    //
    //! PRE: None.
    //! POST: The bird rate is set to the given value.
    //
    //! ARGS: n - The new value for the bird rate.
    // ------------------------------------------------------------------------
    inline void
    setBirdRate (double n) {
        birdRate = n;
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
        return birdRate;
    }

    // ------------------------------------------------------------------------
    //: Set the run mode for the device.
    //
    //! PRE: None.
    //! POST: the run mode is set to the given value.
    //
    //! ARGS: i - The new value for the run mode.
    // ------------------------------------------------------------------------
    inline void
    setRunMode (int n) {
        runMode = n;
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
        return runMode;
    }

    // ------------------------------------------------------------------------
    //: Set the report rate for the device.
    //
    //! PRE: None.
    //! POST: The report rate is set to the given byte value.
    //
    //! ARGS: ch - The new value for the report rate.
    // ------------------------------------------------------------------------
    inline void
    setReportRate (unsigned char n) {
        reportRate = n;
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
        return reportRate;
    }

    // ------------------------------------------------------------------------
    //: Set the address (either IP address or hostname) for the server.
    //
    //! PRE: None.
    //! POST: If an address has already been set (i.e., address is non-NULL),
    //+       that memory is freed.  A copy of n is stored in address.
    //
    //! ARGS: n - The new value for the server address.
    // ------------------------------------------------------------------------
    void setAddress(const char* n);

    // ------------------------------------------------------------------------
    //: Get the current server address for the device.
    //
    //! PRE: None.
    //! POST: The current value of address is returned to the caller.
    //
    //! RETURNS: NULL - No address has been set.
    //! RETURNS: Non-NULL - A character array naming the server address.
    // ------------------------------------------------------------------------
    inline char*
    getAddress () {
        return address;
    }

    // ------------------------------------------------------------------------
    //: Get the x position of the i'th bird.
    //
    //! PRE: posinfo has valid data in it.
    //! POST: The x position for the given bird number is returned to the
    //+       caller.
    //
    //! RETURNS: A floating-point value for the current x position of the
    //+          given bird.
    // ------------------------------------------------------------------------
    inline float
    xPos (int i) {
        return posinfo[i][0];
    }

    // ------------------------------------------------------------------------
    //: Get the y position of the i'th bird.
    //
    //! PRE: posinfo has valid data in it.
    //! POST: The y position for the given bird number is returned to the
    //+       caller.
    //
    //! RETURNS: A floating-point value for the current y position of the
    //+          given bird.
    // ------------------------------------------------------------------------
    inline float
    yPos (int i) {
        return posinfo[i][1];
    }

    // ------------------------------------------------------------------------
    //: Get the z position of the i'th bird.
    //
    //! PRE: posinfo has valid data in it.
    //! POST: The z position for the given bird number is returned to the
    //+       caller.
    //
    //! RETURNS: A floating-point value for the current z position of the
    //+          given bird.
    // ------------------------------------------------------------------------
    inline float
    zPos (int i) {
        return posinfo[i][2];
    }

    // ------------------------------------------------------------------------
    //: Get the z rotation of the i'th bird.
    //
    //! PRE: posinfo has valid data in it.
    //! POST: The z rotation for the given bird number is returned to the
    //+       caller.
    //
    //! RETURNS: A floating-point value for the current z rotation of the
    //+          given bird.
    // ------------------------------------------------------------------------
    inline float
    zRot (int i) {
        return posinfo[i][3];
    }

    // ------------------------------------------------------------------------
    //: Get the y rotation of the i'th bird.
    //
    //! PRE: posinfo has valid data in it.
    //! POST: The y rotation for the given bird number is returned to the
    //+       caller.
    //
    //! RETURNS: A floating-point value for the current y rotation of the
    //+          given bird.
    // ------------------------------------------------------------------------
    inline float
    yRot (int i) {
        return posinfo[i][4];
    }

    // ------------------------------------------------------------------------
    //: Get the x rotation of the i'th bird.
    //
    //! PRE: posinfo has valid data in it.
    //! POST: The x rotation for the given bird number is returned to the
    //+       caller.
    //
    //! RETURNS: A floating-point value for the current x rotation of the
    //+          given bird.
    // ------------------------------------------------------------------------
    inline float
    xRot (int i) {
        return posinfo[i][5];
    }

//*****************************************************

private:
    // ------------------------------------------------------------------------
    //: Send a wakeup call to the MotionStar server.
    //
    //! PRE: The socket has been initialized and is currently open to
    //+      send and receive data from the server.
    //! POST: A MSG_WAKE_UP is sent to the server, and an acknowledgment is
    //+       expected before processing continues.
    // ------------------------------------------------------------------------
    void send_wakeup();

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
    void get_status_all();

    // ------------------------------------------------------------------------
    //: Set the system status.
    //
    //! PRE: The socket has been initialized and is currently open to
    //+      send and receive data from the server.
    //! POST: A MSG_SEND_SETUP is sent to the server, and an acknowledgement
    //+       is expected before processing continues.
    // ------------------------------------------------------------------------
    void set_status_all();

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
    void get_status_fbb(unsigned char);

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
    void set_status_fbb(unsigned char);

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
                   (float) toShort(response.buffer[2 + offset],
                                   response.buffer[3 + offset]) /
                   32767.0);
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
                   (float) toShort(response.buffer[4 + offset],
                                   response.buffer[5 + offset]) /
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
                   (float) toShort(response.buffer[6 + offset],
                                   response.buffer[7 + offset]) /
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
    getRoll (const int offset) {
      return (m_xmtr_rot_scale *
               (float) toShort(response.buffer[8 + offset],
                               response.buffer[9 + offset]) /
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
    getAzimuth (const int offset) {
        return (m_xmtr_rot_scale *
                   (float) toShort(response.buffer[10 + offset],
                                   response.buffer[11 + offset]) /
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
    getElevation (const int offset) {
        return (m_xmtr_rot_scale *
                   (float) toShort(response.buffer[12 + offset],
                                   response.buffer[13 + offset]) /
                    m_xmtr_divisor);
    }

    // ------------------------------------------------------------------------
    //: Print out the MotionStar information.
    //
    //! PRE: posinfo has valid data in it.
    //! POST: The contents of posinfo are printed to stdout.
    // ------------------------------------------------------------------------
    void printInfo();

    // ------------------------------------------------------------------------
    //: Print out the MotionStar's header information.
    //
    //! PRE: None.
    //! POST: This currently does nothing.
    // ------------------------------------------------------------------------
    void display_hdr();

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

    float                   posinfo[3][6];
    bool                    active;
    char*                   address;
    int x;
    unsigned char           all;
    unsigned char           FBBerror;
    unsigned char           flockNumber;
    unsigned char           serverNumber;
    unsigned char           chassisNumber;
    unsigned char           chassisDevices;
    unsigned char           firstAddress;
    unsigned short int      softwareRevision;
    unsigned char           transmitterNumber;
    char                    szRate[7];
    int                     mRate;
    double                  realRate;
    unsigned int            statusSize, temp;
    unsigned char           ertFlag;
    unsigned char           buttonState;

    struct BIRD_STATUS Bird[120];
    struct HEADER command, *lpCommand;
    struct RSP_GET_STATUS_all RspGetStatAll, *lpRspGetStatAll;
    struct DATAPACKET response, *lpResponse;
    struct sockaddr_in server;
 
    int s,rtn;
    unsigned  long i,n;
    unsigned int sequenceNumber;

    char *newptr;

    int errno; // EXTERN!!

    int             numberBytes;
    int             bytesReceived;
#if defined(__FreeBSD__) || defined(_AIX)
    socklen_t       Size;
    socklen_t*      lpSize;
#else
    int             Size;
    int*            lpSize;
#endif
    int             sockfd;
    time_t          timeSeconds;
    char            timeChar[64];
    struct tm       newtime;
    int             digit;
    int             no;
    int             j;
    char            rate[7];

    unsigned int    format;
    int             hemisphere;
    unsigned int    birdFormat;
    unsigned int    birdsRequired;
    double          birdRate;
    int             runMode;
    unsigned char   addr;
    unsigned char   reportRate;

    float           m_xmtr_pos_scale;   //: Transmitter position scaling factor
    float           m_xmtr_rot_scale;   //: Transmitter position scaling factor
    float           m_xmtr_divisor;     //: Number by which all returned values
                                        //+ must be divided
    bool            m_big_endian;
};


#endif	/* _ASCENSION_MOTION_STAR_H_ */
