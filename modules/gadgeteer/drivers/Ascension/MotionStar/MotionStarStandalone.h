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

#ifndef _VRJ_ASCENSION_MOTION_STAR_STANDALONE_H_
#define _VRJ_ASCENSION_MOTION_STAR_STANDALONE_H_

#include <iostream>
#include <string>
#include <vector>

#include <vpr/vpr.h>
#include <vpr/IO/Socket/InetAddr.h>
#include <vpr/IO/Socket/Socket.h>
#include <vpr/Util/ReturnStatus.h>


//: Ascension Flock namespace.
namespace FLOCK
{

//: Flock hemispheres.  The integer values correspond to the values used by
//+ the flock.
enum hemisphere
{
   FRONT_HEMISPHERE = 0,
   REAR_HEMISPHERE  = 1,
   UPPER_HEMISPHERE = 2,
   LOWER_HEMISPHERE = 3,
   LEFT_HEMISPHERE  = 4,
   RIGHT_HEMISPHERE = 5
};

//: Possible data formats that devices connected to a Flock can use.  The
//+ integer values correspond to the values used by the Flock.
enum data_format
{
   NO_BIRD_DATA        = 0,    //: Inactive device
   POSITION            = 1,    //: Position information only
   ANGLES              = 2,    //: Euler angle information only
   MATRIX              = 3,    //: Angle information in matrix form only
   POSITION_ANGLES     = 4,    //: Position and Euler angle information
   POSITION_MATRIX     = 5,    //: Position and angle information in a matrix
   INVALID             = 6,    //: Placeholder
   QUATERNION          = 7,    //: Angle information as quaternion parameters
   POSITION_QUATERNION = 8     //: Possition information and angle information
                               //+ as quaternion parameters
};

//: Data blocks corresponding to the Flock data formats.  Refer to page 136 of
//+ the MotionStar Operation and Installation Guide for a table of the data
//+ format sizes.
union data_block
{
   unsigned char POSITION[6];
   unsigned char ANGLES[6];
   unsigned char MATRIX[18];
   unsigned char POSITION_ANGLES[12];
   unsigned char POSITION_MATRIX[24];
   unsigned char QUATERNION[8];
   unsigned char POSITION_QUATERNION[14];
};

// Constants indicating the bit to test to get the corresponding value about
// a device connected to a Flock.
extern const unsigned char ERROR;
extern const unsigned char RUNNING;
//extern const unsigned char RESERVED1;
//extern const unsigned char RESERVED2;
extern const unsigned char BUTTONS_PRESENT;
extern const unsigned char SENSOR_PRESENT;
extern const unsigned char TRANSMITTER_PRESENT;
extern const unsigned char TRANSMITTER_RUNNING;

// ----------------------------------------------------------------------------
//: Convert the given Flock data format into a human-readable string that
//+ names the format.
//
//! PRE: None.
//! POST: A std::string is returned with a name corresponding to the given
//+       format value.
//
//! ARGS: format - The data format whose name is being requested.
//
//! RETURNS: A std::string object containing the name of the given format.
// ----------------------------------------------------------------------------
std::string getFormatName(const data_format format);

// ----------------------------------------------------------------------------
//: Convert the given Flock hemisphere ID into a human-readable string that
//+ names the hemisphere.
//
//! PRE: None.
//! POST: A std::string is returned with a name corresponding to the given
//+       hemisphere ID value.
//
//! ARGS: hemi - The hemisphere whose name is being requested.
//
//! RETURNS: A std::string object containing the name of the given hemisphere.
// ----------------------------------------------------------------------------
std::string getHemisphereName(const hemisphere hemi);

}  // namspace FLOCK

//: Fast Bird Bus (FBB) namespace.  This collects information common to the
//+ Ascension FBB system into one space.
namespace FBB
{

extern const unsigned char ACCESS;  //: Bit to test if the device is accessible
extern const unsigned char RUNNING; //: Bit to test if the device is running
extern const unsigned char SENSOR;  //: Bit to test if the device has a sensor
extern const unsigned char ERC;     //: Bit to test if the devices is an ERC
extern const unsigned char ERT3;    //: Bit to test if ERT #3 is present
extern const unsigned char ERT2;    //: Bit to test if ERT #2 is present
extern const unsigned char ERT1;    //: Bit to test if ERT #1 is present
extern const unsigned char ERT0;    //: Bit to test if ERT #0 is present

//: Fast Bird Bus device structure.
struct Device {
    // These are set using the FBB status byte returned when the general
    // system status is requested from the master server.
    bool                accessible;   //:
    bool                running;      //:
    bool                has_sensor;   //:
    bool                is_erc;       //:
    bool                ert0_present; //:
    bool                ert1_present; //:
    bool                ert2_present; //:
    bool                ert3_present; //:

    // These are set when the individual status of the bird is requested.
    unsigned char       data_words;   //: Number of words in formatted data
    FLOCK::data_format  format;       //: Format of data read from device
    unsigned char       report_rate;  //: Report rate relative to sample rate
    FLOCK::hemisphere   hemisphere;   //:
    unsigned char       address;      //: FBB address of this device
    FLOCK::data_block   data;         //: Data read from this device

    unsigned char       addr;
};

}  // namespace FBB

//: BIRDNET namespace.  This collects all MotionStar BIRDNET protocol-specific
//+ things into one namespace to prevent potential name collisions.  The
//+ protocol in use is as specified in the MotionStar Installation and
//+ Operation Guide dated May 27, 1999.
namespace BIRDNET
{

// BIRDNET command/response codes.  See pages 112-3 of the MotionStar
// Installation and Operation Guide.
extern const unsigned char MSG_WAKE_UP;        //: Wake up the server
extern const unsigned char RSP_WAKE_UP;        //: Server response to wake-up
extern const unsigned char MSG_SHUT_DOWN;      //: Shutdown the server
extern const unsigned char RSP_SHUT_DOWN;      //: Server response to shutdown
extern const unsigned char MSG_GET_STATUS;     //: Request system status
extern const unsigned char MSG_SEND_SETUP;     //: Send setup to server
extern const unsigned char MSG_SINGLE_SHOT;    //: Request single asyncronous
                                               //+ data packet
extern const unsigned char MSG_RUN_CONTINUOUS; //: Request continuous data
extern const unsigned char MSG_STOP_DATA;      //: Stop continuous data
extern const unsigned char RSP_GET_STATUS;     //: Response of system status
extern const unsigned char RSP_SEND_SETUP;     //: Response to sent setup
extern const unsigned char RSP_RUN_CONTINUOUS; //: Response to continuous data 
                                               //+ request
extern const unsigned char RSP_STOP_DATA;      //: Response to continuous data
                                               //+ stop request
extern const unsigned char DATA_PACKET_MULTI;  //: Single-shot and continuous
                                               //+ data packet from the server
extern const unsigned char RSP_ILLEGAL;        //: Response to packet received
                                               //+ at an inappropriate time
extern const unsigned char RSP_UNKNOWN;        //: Response to packet with
                                               //+ invalid command/response
                                               //+ code
extern const unsigned char MSG_SYNC_SEQUENCE;  //: Reset server sequence
                                               //+ numbers
extern const unsigned char RSP_SYNC_SEQUENCE;  //: Response to sequence number
                                               //+ reset request

extern const unsigned char BN_PROTOCOL;        //: BIRDNET protocol ID

//: BIRDNET packet header.  It is 16 bytes long.  See page 111 of the
//+ MotionStar Installation and Operation Guide.
struct HEADER
{
   // ------------------------------------------------------------------------
   //: Simple constructor.
   //
   //! PRE: None.
   //! POST: Commonly used fields are initialized to standard values.  The
   //+       type and the sequence fields are NOT initialized.
   // ------------------------------------------------------------------------
   HEADER (void)
      : xtype(0), protocol(BN_PROTOCOL), error_code(0),
        error_code_extension(0), number_bytes(0)
   {
      /* Do nothing. */ ;
   }

   // ------------------------------------------------------------------------
   //: Simple constructor for a header of the given type.
   //
   //! PRE: None.
   //! POST: Commonly used fields are initialized to standard values, and the
   //+       type field is set to msg_type.  The sequence field is not
   //+       initialized.
   //
   //! ARGS: msg_type - The type of message being constructed.
   // ------------------------------------------------------------------------
   HEADER (const unsigned char msg_type)
      : type(msg_type), xtype(0), protocol(BN_PROTOCOL), error_code(0),
        error_code_extension(0), number_bytes(0)
   {
      /* Do nothing. */ ;
   }

   unsigned short      sequence;       //: 16-bit sequence number
   unsigned short      milliseconds;   //: Interval time between packets
   unsigned char       time[4];        //: Seconds since 00:00:00 Jan. 1,
                                       //+ 1970 (GMT)
   unsigned char       type;           //: Command/response code
   unsigned char       xtype;          //: Extension to command/response code
   unsigned char       protocol;       //: Protocol for this packet (BIRDNET
                                       //+ is 3)
   unsigned char       error_code;     //: Error code for an event at the
                                       //+ server concerning the previous
                                       //+ client packet
   unsigned short      error_code_extension; //: Additional error code info
   unsigned short      number_bytes;   //: Size of the following data field
};

//: BIRDNET general system status packet.  It is 16 bytes long.  See page 121
//+ of the MotionStar Installation and Operation Guide.
struct SYSTEM_STATUS
{
   unsigned char all;                //: General system status
   unsigned char error;              //: Reserved
   unsigned char flockNumber;        //: Number of devices on FBB bus
   unsigned char serverNumber;       //: Number of servers in system
   unsigned char transmitterNumber;  //: Address of current active xmtr
   unsigned char measurementRate[6]; //: Measurement rate * 1000 (text)
   unsigned char chassisNumber;      //: Chassis ID number
   unsigned char chassisDevices;     //: Number of devices in chassis
   unsigned char firstAddress;       //: FBB address of first device
   unsigned char softwareRevision[2]; //: Software rev. of server app
};

//: BIRDNET singleBirdStatus packet.  It is 16 bytes long.  See page 125 of
//+ the MotionStar Installation and Operation Guide.
struct SINGLE_BIRD_STATUS
{
   unsigned char status;         //: Device status
   unsigned char id;             //: Device type
   unsigned char softwareRev[2]; //: Software revision of firmware
   unsigned char errorCode;      //: Reserved for future use
   unsigned char setup;          //: Device setup parameters
   unsigned char dataFormat;     //: Data Format Code and Data Size
   unsigned char reportRate;     //: Rate at which data is generated
                                 //+ relative to the sample rate
   unsigned char scaling[2];     //: Full scale output
   unsigned char hemisphere;     //: Bird's hemisphere of operation
   unsigned char FBBaddress;     //: FBB address
   unsigned char spare1;         //: Reserved
   unsigned char spare2[2];      //: Reserved
};

//: BIRDNET FILTER_TABLE structure.  This is for an ERC/ERT (Extended Range
//+ Controller/Extended Range Transmitter).  See page 131 of the MotionStar
//+ Installation and Operation Guide.
struct FILTER_TABLE
{
   unsigned short entry[7];
};

//: BIRDNET XYZREF_TABLE structure.  Word 1 is azimuth, word 2 is elevation
//+ and word 3 is roll.  See page 132 of the MotionStar Installation and
//+ Operation Guide.
struct XYZREF_TABLE
{
   unsigned short angle[3];
};

//: BIRDNET indiviual Flock status packet.  It is 70 bytes long.  See page 124
//+ of the MotionStar Installation and Operation Guide.
struct BIRD_STATUS
{
   struct SINGLE_BIRD_STATUS status;           //: See SINGLE_BIRD_STATUS
   struct FILTER_TABLE       alphaMin;         //: Page 61 of I&O Guide
   struct FILTER_TABLE       alphaMax;         //: Pages 65-6 of I&O Guide
   struct FILTER_TABLE       Vm;               //: Pages 66-7 of I&O Guide
   struct XYZREF_TABLE       xyzReferenceFrame;
   struct XYZREF_TABLE       xyzAngleAlign;
};

//: Generic BIRDNET DATA_PACKET packet.  This is used when reading device data
//+ from the server.
// XXX: Is it safe for the buffer to be fixed at 2048 bytes?  It should be
// set to MAXDATA, but I do not know where that is defined.
struct DATA_PACKET
{
   struct HEADER header;       //: Header that describes the data buffer
   unsigned char buffer[2048]; //: Memory block containing read data
};

//: BIRDNET DATA_RECORD structure.  Refer to page 138 of the MotionStar
//+ Installation and Operatiion Guide.
struct DATA_RECORD
{
   unsigned char     address;        //: FBB address of the device whose
                                     //+ data is contained in the record
   unsigned char     data_info;      //: Description of the data block
   FLOCK::data_block data;           //: Variable-sized data read from the
                                     //+ device
   unsigned char     button_data[2]; //: Information about the optional
                                     //+ button attached to the sensor (this
                                     //+ may not always be part of the data
                                     //+ block sent by teh server)
};

//: Possible transmission protocols for use between the client and the server.
enum protocol
{
   UDP,
   TCP
};

//: Possible data request modes.
enum run_mode
{
   CONTINUOUS,     //: Read data continuously from the server
   SINGLE_SHOT     //: Read a single asynchronous packet of data
};

//: Possible units being used by the data read from the server.
enum units
{
   INCHES
};

}  // namspace BIRDNET


//: The protocol in use is as specified in the MotionStar Installation and
//+ Operation Guide dated May 27, 1999.
class MotionStarStandalone
{
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
   //! ARGS: address          - The IP address or hostname of the MotionStar
   //+                          server to which we are connecting.
   //! ARGS: port             - The port on the server to which we connect.
   //! ARGS: proto            - The transmission protocol to use between the
   //+                          client and the server.
   //! ARGS: master           - A flag stating whether this server chassis is
   //+                          a master (true) or a slave (false).
   //! ARGS: hemisphere       - The hemisphere in use on the transmitter.
   //! ARGS: bird_format      - The bird data format.
   //! ARGS: run_mode         - The data request mode for the server.
   //! ARGS: report_rate      - The rate at which data is generated by the
   //+                          birds relative to the sample rate (0-255).
   //! ARGS: measurement_rate -
   //! ARGS: birds_requested  - The number of birds whose data will be
   //+                          sampled.
   // ------------------------------------------------------------------------
   MotionStarStandalone(const std::string& address, const unsigned short port,
                        const BIRDNET::protocol proto, const bool master,
                        const FLOCK::hemisphere hemisphere,
                        const FLOCK::data_format bird_format,
                        const BIRDNET::run_mode run_mode,
                        const unsigned char report_rate,
                        const double measurement_rate,
                        const unsigned int birds_requested);

   // ------------------------------------------------------------------------
   //: Destructor.
   //
   //! PRE: None.
   //! POST: The device is stopped, and the memory allocated for the server
   //+       address is released.
   // ------------------------------------------------------------------------
   ~MotionStarStandalone(void);

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
   //! RETURNS: -2 - The socket could not be created.
   //! RETURNS: -3 - No address has been set for the server.
   //! RETURNS: -4 - The position scaling factor was not determined from
   //+               information sent by the server.
   // ------------------------------------------------------------------------
   int start(void);

   // ------------------------------------------------------------------------
   //: The data flow from the server is stopped, the server is told to go to
   //+ sleep and the socket is closed.
   //
   //! PRE: Refer to stopData() and shutdown().
   //! POST: If the run mode is BIRDNET::CONTINUOUS, the data flow is
   //+       stopped, the server is told to shut down, the state is set to
   //+       inactive and the socket is closed.
   // ------------------------------------------------------------------------
   void stop(void);

   // ------------------------------------------------------------------------
   //: Based on the current run mode, a single sample is taken (run mode is
   //+ 1), or continuous samples are taken (run mode is 0).
   //
   //! PRE: The socket has been initialized and is currently open to
   //+      receive data from the server.
   //! POST:
   // ------------------------------------------------------------------------
   void sample(void);

   // ------------------------------------------------------------------------
   //: Stops the data flow if it is in continuous mode.
   //
   //! PRE: If the current run mode is BIRDNET::CONTINUOUS, then the socket
   //+      has been initialized and is currently open for sending and
   //+      receiving data.
   //! POST: A MSG_STOP_DATA command is sent to the server.
   //
   //! RETURNS:  0 - The data flow was stopped.
   //! RETURNS: -1 - The data flow could not be stopped for some reason.
   // ------------------------------------------------------------------------
   vpr::ReturnStatus stopData(void);

   // ------------------------------------------------------------------------
   //: Shut down the server chassis.
   //
   //! PRE: The socket has been initialized and is currently open to
   //+      receive data from the server.
   //! POST: A MSG_SHUT_DOWN command is sent to the server.  If the server is
   //+       shut down successfully, m_active is set to false.
   //
   //! RETURNS:  0 - Data flow was stopped.
   //! RETURNS: -1 - Data flow could not be stopped.
   // ------------------------------------------------------------------------
   vpr::ReturnStatus shutdown(void);

   // ------------------------------------------------------------------------
   //: Returns whether the MotionStar is active or not.
   //
   //! PRE: None.
   //! POST: The value in m_active is returned to the caller.
   //
   //! RETURNS: A boolean value stating whether or not the device is active.
   // ------------------------------------------------------------------------
   const bool& isActive (void) const
   {
      return m_active;
   }

   // ------------------------------------------------------------------------
   //: Set the address (either IP address or hostname) for the server.
   //
   //! PRE: None.
   //! POST: The value in m_address is replaced with the string pointed to by
   //+       addr.
   //
   //! ARGS: addr - The new value for the server address.
   // ------------------------------------------------------------------------
   void setAddressName (const char* addr)
   {
      m_address.setAddress(addr);
   }

   // ------------------------------------------------------------------------
   //: Set the address (either IP address or hostname) for the server using a
   //+ std::string object reference.
   //
   //! PRE: None.
   //! POST:
   //
   //! ARGS: addr - The new value for the server address.
   // ------------------------------------------------------------------------
   void setAddressName (const std::string& addr)
   {
      m_address.setAddress(addr);
   }

   // ------------------------------------------------------------------------
   //: Get the current server address as a string.
   //
   //! PRE: None.
   //! POST:
   //
   //! RETURNS: NULL - No address has been set.
   //! RETURNS: Non-NULL - A character array naming the server address.
   // ------------------------------------------------------------------------
   const std::string getAddressName (void) const
   {
      return m_address.getAddressString();
   }

   // ------------------------------------------------------------------------
   //: Set the address object for the server.
   //
   //! PRE: None.
   //! POST: The value in m_address is replaced with the value in addr.
   //
   //! ARGS: addr - The new value for the server address.
   // ------------------------------------------------------------------------
   void setAddress (const vpr::InetAddr& addr)
   {
      m_address = addr;
   }

   // ------------------------------------------------------------------------
   //: Get the current server address object.
   //
   //! PRE: None.
   //! POST: The current value of m_address is returned to the caller.
   //
   //! RETURNS: NULL - No address has been set.
   //! RETURNS: Non-NULL - A character array naming the server address.
   // ------------------------------------------------------------------------
   const vpr::InetAddr& getAddress (void) const
   {
      return m_address;
   }

   // ------------------------------------------------------------------------
   //: Set the port on the server to which we connect.
   //
   //! PRE: None.
   //! POST: The value in port is stored in m_port.
   //
   //! ARGS: port - The new value for the server port.
   // ------------------------------------------------------------------------
   void setServerPort (const vpr::Uint16 port)
   {
      m_address.setPort(port);
   }

   // ------------------------------------------------------------------------
   //: Get the server port.
   //
   //! PRE: None.
   //! POST: The current value of m_port is returned to the caller.
   //
   //! RETURNS: An unsigned short integer containing the server port number.
   // ------------------------------------------------------------------------
   const vpr::Uint16 getServerPort (void) const
   {
      return m_address.getPort();
   }

   // ------------------------------------------------------------------------
   //: Set the transmission protocol used between the client and the server.
   //
   //! PRE: None.
   //! POST: The value in proto is stored in m_proto.
   //
   //! ARGS: proto - The new value for the transmission protocol.
   // ------------------------------------------------------------------------
   void setProtocol (const enum BIRDNET::protocol proto)
   {
      m_proto = proto;
   }

   // ------------------------------------------------------------------------
   //: Get the transmission protocol used between the client and the server.
   //
   //! PRE: None.
   //! POST: The current value of m_proto is returned to the caller.
   //
   //! RETURNS: 
   // ------------------------------------------------------------------------
   const BIRDNET::protocol& getProtocol (void) const
   {
      return m_proto;
   }

   // ------------------------------------------------------------------------
   //: Set the master status for this server chassis.
   //
   //! PRE: None.
   //! POST: The value in master is stored in m_master.
   //
   //! ARGS: master - The new value for the master status of this server
   //+                chassis.
   // ------------------------------------------------------------------------
   void setMasterStatus (const bool master)
   {
      m_master = master;
   }

   // ------------------------------------------------------------------------
   //: Get the master status for this server chassis.
   //
   //! PRE: None.
   //! POST: The current value of m_master is returned to the caller.
   //
   //! RETURNS:  true - This server chassis is a master.
   //! RETURNS: false - This server chassis is a slave.
   // ------------------------------------------------------------------------
   const bool& getMasterStatus (void) const
   {
      return m_master;
   }

   // ------------------------------------------------------------------------
   //: Set the run mode for the device.
   //
   //! PRE: None.
   //! POST: If the driver is active and the run mode is being toggled from
   //+       BIRDNET::CONTINUOUS to BIRDNET::SINGLE_SHOT, the data flow from
   //+       the server is stopped.  If the driver is active and the run mode
   //+       being toggled from BIRDNET::SINGLE_SHOT to BIRDNET::CONTINUOUS,
   //+       the server is told to begin sending a continuous stream of data.
   //+       m_run_mode is set to mode after all of that is handled.
   //
   //! ARGS: mode - The new value for the run mode.
   // ------------------------------------------------------------------------
   void setRunMode(const BIRDNET::run_mode mode);

   // ------------------------------------------------------------------------
   //: Get the current run mode for the device.
   //
   //! PRE: None.
   //! POST: The current run mode is returned to the caller.
   //
   //! RETURNS: An integer value for run mode.
   // ------------------------------------------------------------------------
   const BIRDNET::run_mode& getRunMode (void) const
   {
      return m_run_mode;
   }

   // ------------------------------------------------------------------------
   //: Set the number of birds connected to the Flock.
   //
   //! PRE: None.
   //! POST: The number of birds is updated to the new value.
   //
   //! ARGS: num_birds - The new value for the number of birds in use.
   // ------------------------------------------------------------------------
   void setNumBirds (const unsigned int num_birds)
   {
      m_birds_requested = num_birds;
   }

   // ------------------------------------------------------------------------
   //: Get the number of bird currently connected to the flock.
   //
   //! PRE: None.
   //! POST: The current number of connected birds is returned to the caller.
   //
   //! RETURNS: An unsigned integer value for the number of connected birds.
   // ------------------------------------------------------------------------
   const unsigned int& getNumBirds (void) const
   {
       return m_birds_requested;
   }

   // ------------------------------------------------------------------------
   //: Change the birds' hemisphere of operation.
   //
   //! PRE: None.
   //! POST: The hemisphere for it is reset to the given value.
   //
   //! ARGS: hemisphere - The new hemisphere to use.
   // ------------------------------------------------------------------------
   void setHemisphere (const FLOCK::hemisphere hemisphere)
   {
      m_hemisphere = hemisphere;
   }

   // ------------------------------------------------------------------------
   //: Get the transmitter hemisphere currently in use by the birds.
   //
   //! PRE: None.
   //! POST: The current hemisphere value is returned to the caller.
   //
   //! RETURNS: An unsigned integer value for the hemisphere in use.
   // ------------------------------------------------------------------------
   const FLOCK::hemisphere& getHemisphere (void) const
   {
      return m_hemisphere;
   }

   // ------------------------------------------------------------------------
   //: Set the data format the birds will use to the given value.
   //
   //! PRE: None.
   //! POST: The bird data format is updated to the new value.
   //
   //! ARGS: format - The new bird data format to use.
   // ------------------------------------------------------------------------
   void setBirdFormat (const FLOCK::data_format format)
   {
      m_bird_format = format;
   }

   // ------------------------------------------------------------------------
   //: Get the bird data format currently in use.
   //
   //! PRE: None.
   //! POST: The current bird data format value is returned to the caller.
   //
   //! RETURNS: An unsigned integer value for the bird data format.
   // ------------------------------------------------------------------------
   const FLOCK::data_format& getBirdFormat (void) const
   {
      return m_bird_format;
   }

   // ------------------------------------------------------------------------
   //: Set the report rate for the birds.
   //
   //! PRE: None.
   //! POST: The bird report rate is set to the given byte value.
   //
   //! ARGS: rate - The new value for the bird report rate.
   // ------------------------------------------------------------------------
   void setReportRate (const unsigned char rate)
   {
      m_report_rate = rate;
   }

   // ------------------------------------------------------------------------
   //: Get the current report rate for the birds.
   //
   //! PRE: None.
   //! POST: The current bird report rate is returned to the caller.
   //
   //! RETURNS: An unsigned character value for the bird report rate.
   // ------------------------------------------------------------------------
   const unsigned char& getReportRate (void) const
   {
      return m_report_rate;
   }

   // ------------------------------------------------------------------------
   //: Set the data measurement rate for the chassis.
   //
   //! PRE: None.
   //! POST: The chassis data measurement rate is set to the given byte
   //+       value.
   //
   //! ARGS: rate - The new value for the chassis data measurement rate.
   // ------------------------------------------------------------------------
   void setMeasurementRate (const double rate)
   {
      m_measurement_rate = rate;
   }

   // ------------------------------------------------------------------------
   //: Get the current data measurement rate.
   //
   //! PRE: None.
   //! POST: The current data measurement rate is returned to the caller.
   //
   //! RETURNS: A double-precision floating-point value for data measurement
   //+          rate.
   // ------------------------------------------------------------------------
   const double& getMeasurementRate (void) const
   {
      return m_measurement_rate;
   }

   // ------------------------------------------------------------------------
   //: Get a pointer to the FBB device indexed by the given bird ID.  The
   //+ indexing is zero-based.
   //
   //! PRE: The given bird index is in the vector of defined devices.
   //! POST: A pointer to the requested device is returned to the caller.
   //
   //! ARGS: bird - The zero-based index of the FBB device objcet that is to
   //+              be returned.
   //
   //! RETURNS: An FBB::Device pointer that points to the requested FBB
   //+          device object.
   // ------------------------------------------------------------------------
   const FBB::Device* getBird (const unsigned int bird)
   {
      return m_birds[bird];
   }

   // ------------------------------------------------------------------------
   //: Get the data format in use by a specific bird.
   //
   //! PRE: The given bird index is in the vector of defined devices.
   //! POST: The data format for the requested FBB device is returned to the
   //+       caller.
   //
   //! ARGS: bird - The zero-based index of the FBB device whose data format
   //+              is to be returned.
   //
   //! RETURNS: A FLOCK::data_format value representing the data format of
   //+          the requested device.
   // ------------------------------------------------------------------------
   const FLOCK::data_format& getBirdDataFormat (const unsigned int bird)
   {
      return m_birds[bird]->format;
   }

   // ------------------------------------------------------------------------
   //: Get the x position of the i'th bird.  This is only valid if the bird
   //+ in question is generating positional data.
   //
   //! PRE: The given bird index is in the vector of defined devices.
   //! POST: The x position for the given bird number is returned to the
   //+       caller.
   //
   //! ARGS: i - The zero-based index of the FBB device whose current x
   //+           position is to be returned.
   //
   //! RETURNS: A floating-point value for the current x position of the
   //+          given bird.
   // ------------------------------------------------------------------------
   float getXPos(const unsigned int i) const;

   // ------------------------------------------------------------------------
   //: Get the y position of the i'th bird.  This is only valid if the bird
   //+ in question is generating positional data.
   //
   //! PRE: The given bird index is in the vector of defined devices.
   //! POST: The y position for the given bird number is returned to the
   //+       caller.
   //
   //! ARGS: i - The zero-based index of the FBB device whose current y
   //+           position is to be returned.
   //
   //! RETURNS: A floating-point value for the current y position of the
   //+          given bird.
   // ------------------------------------------------------------------------
   float getYPos(const unsigned int i) const;

   // ------------------------------------------------------------------------
   //: Get the z position of the i'th bird.  This is only valid if the bird
   //+ in question is generating positional data.
   //
   //! PRE: The given bird index is in the vector of defined devices.
   //! POST: The z position for the given bird number is returned to the
   //+       caller.
   //
   //! ARGS: i - The zero-based index of the FBB device whose current z
   //+           position is to be returned.
   //
   //! RETURNS: A floating-point value for the current z position of the
   //+          given bird.
   // ------------------------------------------------------------------------
   float getZPos(const unsigned int i) const;

   // ------------------------------------------------------------------------
   //: Get the z rotation of the i'th bird.  This is only valid if the bird
   //+ in question is generating angular data.
   //
   //! PRE: The given bird index is in the vector of defined devices.
   //! POST: The z rotation for the given bird number is returned to the
   //+       caller.
   //
   //! ARGS: i - The zero-based index of the FBB device whose current z
   //+           rotation is to be returned.
   //
   //! RETURNS: A floating-point value for the current z rotation of the
   //+          given bird.
   // ------------------------------------------------------------------------
   float getZRot(const unsigned int i) const;

   // ------------------------------------------------------------------------
   //: Get the y rotation of the i'th bird.  This is only valid if the bird
   //+ in question is generating angular data.
   //
   //! PRE: The given bird index is in the vector of defined devices.
   //! POST: The y rotation for the given bird number is returned to the
   //+       caller.
   //
   //! ARGS: i - The zero-based index of the FBB device whose current y
   //+           rotation is to be returned.
   //
   //! RETURNS: A floating-point value for the current y rotation of the
   //+          given bird.
   // ------------------------------------------------------------------------
   float getYRot(const unsigned int i) const;

   // ------------------------------------------------------------------------
   //: Get the x rotation of the i'th bird.  This is only valid if the bird
   //+ in question is generating angular data.
   //
   //! PRE: The given bird index is in the vector of defined devices.
   //! POST: The x rotation for the given bird number is returned to the
   //+       caller.
   //
   //! ARGS: i - The zero-based index of the FBB device whose current x
   //+           rotation is to be returned.
   //
   //! RETURNS: A floating-point value for the current x rotation of the
   //+          given bird.
   // ------------------------------------------------------------------------
   float getXRot(const unsigned int i) const;

   // ------------------------------------------------------------------------
   //: Get the rotation angles of the requested bird as matrix elements that
   //+ are then stored in the given array.
   //
   //! PRE: The given bird index is in the vector of defined devices.
   //! POST: The rotations about the x-, y- and z-axes are extracted from the
   //+       rotational matrix read from the bird and are stored in the
   //+       angles array.
   //
   //! ARGS: bird   - The zero-based index of the FBB device whose current
   //+                rotation is to be returned.
   //! ARGS: angles - A 3-element array used to store the x-, y- and z-axis
   //+                rotations extracted from the rotation matrix.
   // ------------------------------------------------------------------------
   void getMatrixAngles(const unsigned int bird, float angles[3]) const;

   // ------------------------------------------------------------------------
   //: Get the rotation angles of the requested bird as quaternion parameters
   //+ that are then stored in the given array.
   //
   //! PRE: The given bird index is in the vector of defined devices.
   //! POST: The rotations about the x-, y- and z-axes are extracted as
   //+       quaternion parameters read from the bird and are stored in the
   //+       quat array.
   //
   //! ARGS: bird - The zero-based index of the FBB device whose quaternion
   //+              parameters are to be returned.
   //! ARGS: quat - A 4-element array used to store the quaternion parameters
   //+              extracted from the quatternion data read by the bird.
   // ------------------------------------------------------------------------
   void getQuaternion(const unsigned int bird, float quat[4]) const;

private:
   // ------------------------------------------------------------------------
   //: Send a wake-up call to the MotionStar server.
   //
   //! PRE: The socket has been initialized and is currently open to
   //+      send and receive data from the server.
   //! POST: A MSG_WAKE_UP packet is sent to the server.  If the server
   //+       responds but has set an error code, send a MSG_SHUT_DOWN packet
   //+       and another MSG_WAKE_UP packet to restart the server.
   //
   //! RETURNS:  0 - The server was awakened.
   //! RETURNS: -1 - The server could not be awakened for some reason.
   // ------------------------------------------------------------------------
   vpr::ReturnStatus sendWakeUp(void);

   // ------------------------------------------------------------------------
   //: Get the system status.
   //
   //! PRE: The socket has been initialized and is currently open to
   //+      send and receive data from the server.
   //! POST: A MSG_GET_STATUS packet is sent to the server.  The response is
   //+       handled by recording all the important configuration data read
   //+       from the server.
   //
   //! RETURNS: Non-NULL - A pointer to a BIRDNET::SYSTEM_STATUS object that
   //+                     fully describes the system state.
   //! RETURNS: NULL     - The system status could not be read.
   // ------------------------------------------------------------------------
   BIRDNET::SYSTEM_STATUS* getSystemStatus(void);

   // ------------------------------------------------------------------------
   //: Set the system status.
   //
   //! PRE: The socket has been initialized and is currently open to
   //+      send and receive data from the server.
   //! POST: The system configuration is sent by sending a configuration
   //+       packet for device 0.  The configuration packet sets the
   //+       transmitter number and the data rate.
   //
   //! ARGS: sys_status - The current system status.
   //! ARGS: xmtr_num   - The transmitter number to use.
   //! ARGS: data_rate  - The data measurement rate to use.
   //
   //! RETURNS:  0 - The configuration was sent successfully.
   //! RETURNS: -1 - The configuration could not be send to the server.
   // ------------------------------------------------------------------------
   vpr::ReturnStatus setSystemStatus(BIRDNET::SYSTEM_STATUS* sys_status,
                                     const unsigned char xmtr_num,
                                     const char data_rate[6]);

   // ------------------------------------------------------------------------
   //: Read the configurations of all the birds and send our configuration
   //+ data to them.
   //
   //! PRE: The socket has been initialized and is currently open to
   //+      send and receive data from the server.
   //! POST: Each bird's configuration is read individually.  Based on the
   //+       current configuration and whatever configuration parameters have
   //+       been set in the driver configuration, a new configuration is
   //+       sent back to the server telling it how we want to use each bird.
   //+       Any birds that are available but are not requested by the
   //+       current configuration are disabled.
   //
   //! RETURNS: An integer stating how many birds were configured.
   //
   //! NOTE: The scaling factor, the measurement units (e.g., inches)  and
   //+       the report rate for all the birds are determined here.
   // ------------------------------------------------------------------------
   unsigned int configureBirds(void);

   // ------------------------------------------------------------------------
   //: Get the status of an individual bird.
   //
   //! PRE: The socket has been initialized and is currently open to
   //+      send and receive data from the server.  The requested bird index
   //+      is in the vector of birds.
   //! POST: The status of the given device is requested from the server.  If
   //+       it is read successfully, the FBB device object in the m_birds
   //+       vector corresponding to the given bird index is updated to
   //+       contain the configuration read.
   //
   //! ARGS: bird - The bird whose status is being requested.  This is a
   //+              zero-based value.
   //
   //! RETURNS: Non-NULL - A pointer to a BIRDNET::SINGLE_BIRD_STATUS object
   //+                     describing the requested bird's current status.
   //! RETURNS: NULL     - No status information could be read for the
   //+                     requested bird for some reason.
   // ------------------------------------------------------------------------
   BIRDNET::SINGLE_BIRD_STATUS* getBirdStatus(const unsigned char bird);

   // ------------------------------------------------------------------------
   //: Set the status of an individual bird.
   //
   //! PRE: The socket has been initialized and is currently open to
   //+      send and receive data from the server.  The requested bird index
   //+      is in the vector of birds.
   //! POST: The FBB device corresponding to the given bird index is sent
   //+       the passed configuration block.
   //
   //! ARGS: bird   - The bird whose status is being set.  This is a
   //+                zero-based value.
   //! ARGS: status - A pointer to a BIRDNET::SINGLE_BIRD_STATUS object
   //+                describing the new configuration for the given bird.
   //
   //! RETURNS:  0 - The bird was configured successfully.
   //! RETURNS: -1 - The bird could not be configured for some reason.
   // ------------------------------------------------------------------------
   vpr::ReturnStatus setBirdStatus(const unsigned char bird,
                                   BIRDNET::SINGLE_BIRD_STATUS* status);

   // ------------------------------------------------------------------------
   //: Get the status of the requested FBB device.  The device number must be
   //+ that of the device's address on the Fast Bird Bus.  Thus, a value of
   //+ 0 is interpreted as a request for the overall system status.  The
   //+ birds are addressed from 1 through 120.
   //
   //! PRE: The socket has been initialized and is currently open to
   //+      send and receive data from the server.
   //! POST: The status for the given device is requested from the server
   //+       and is stored in an allocated BIRDNET::DATA_PACKET block that
   //+       is then returned to the caller.
   //
   //! ARGS: device - The FBB address of the device to be configured.
   //
   //! RETURNS: Non-NULL - A pointer to a BIRDNET::DATA_PACKET that the
   //+                     caller should delete when it has finished with it.
   //! RETURNS: NULL     - The status for the given device could not be
   //+                     requested for some reason.
   // ------------------------------------------------------------------------
   BIRDNET::DATA_PACKET* getDeviceStatus(const unsigned char device);

   // ------------------------------------------------------------------------
   //: Set the status of the requested FBB device using the given
   //+ configuration block.  The device number must be that of the device's
   //+ address on the Fast Bird Bus.  Thus, a value of 0 is interpreted as a
   //+ configuation block for the overall system.  The birds are addressed
   //+ from 1 through 120.
   //
   //! PRE: The socket has been initialized and is currently open to
   //+      send and receive data from the server.
   //! POST: A full packet is constructed from the variable-sized data block
   //+       passed by the user.  This packet is then sent to the server as
   //+       a configuration packet for the named device.
   //
   //! ARGS: device      - The FBB address of the device to be configured.
   //! ARGS: buffer      - A configuration block for the given device.  The
   //+                     real type should correspond to the type of device
   //+                     being configured (either the whole system or an
   //+                     individual bird).  This will be written into the
   //+                     data buffer of a constructed packet that is then
   //+                     sent to the server.
   //! ARGS: buffer_size - The size of the configuration buffer.
   // ------------------------------------------------------------------------
   vpr::ReturnStatus setDeviceStatus(const unsigned char device,
                                     const char* buffer,
                                     const unsigned short buffer_size);

   // ------------------------------------------------------------------------
   //: Tell the MotionStar server to sample continuously.
   //
   //! PRE: The socket has been initialized and is currently open to
   //+      send and receive data from the server.
   //! POST: A MSG_RUN_CONTINUOUS packet is sent to the server.  The server
   //+       responds and then begins sending a continuous stream of data.
   //
   //! RETURNS:  0 - The server is now sending continuous data.
   //! RETURNS: -1 - The run mode could not be set to BIRDNET::CONTINUOUS.
   // ------------------------------------------------------------------------
   int setContinuous(void);

   // ------------------------------------------------------------------------
   //: Convert the raw positional information in the given array to the
   //+ translation along the x-axis which is returned as a single-precision
   //+ floating-point value.
   //
   //! PRE: The address given is the base address of a 6-byte array
   //+      containing raw positional information read from the server.
   //! POST: The indices in the data block corresponding to the raw x
   //+       position are combined into a single floating-point value that
   //+       is returned to the caller.
   //
   //! ARGS: base - The base address of a 6-byte positional data array.
   //
   //! RETURNS: The x-axis translation as a floating-point value in the units
   //+          being used by the device.
   // ------------------------------------------------------------------------
   float toXPos (const unsigned char* base) const
   {
      return toFloatPos(base[0], base[1]);
   }

   // ------------------------------------------------------------------------
   //: Convert the raw positional information in the given array to the
   //+ translation along the y-axis which is returned as a single-precision
   //+ floating-point value.
   //
   //! PRE: The address given is the base address of a 6-byte array
   //+      containing raw positional information read from the server.
   //! POST: The indices in the data block corresponding to the raw y
   //+       position are combined into a single floating-point value that
   //+       is returned to the caller.
   //
   //! ARGS: base - The base address of a 6-byte positional data array.
   //
   //! RETURNS: The y-axis translation as a floating-point value in the units
   //+          being used by the device.
   // ------------------------------------------------------------------------
   float toYPos (const unsigned char* base) const
   {
      return toFloatPos(base[2], base[3]);
   }

   // ------------------------------------------------------------------------
   //: Convert the raw positional information in the given array to the
   //+ translation along the z-axis which is returned as a single-precision
   //+ floating-point value.
   //
   //! PRE: The address given is the base address of a 6-byte array
   //+      containing raw positional information read from the server.
   //! POST: The indices in the data block corresponding to the raw z
   //+       position are combined into a single floating-point value that
   //+       is returned to the caller.
   //
   //! ARGS: base - The base address of a 6-byte positional data array.
   //
   //! RETURNS: The z-axis translation as a floating-point value in the units
   //+          being used by the device.
   // ------------------------------------------------------------------------
   float toZPos (const unsigned char* base) const
   {
      return toFloatPos(base[4], base[5]);
   }

   // ------------------------------------------------------------------------
   //: Convert the raw rotational information in the given array to the
   //+ rotation about the z-axis which is returned as a single-precision
   //+ floating-point value in degrees.
   //
   //! PRE: The address given is the base address of a 6-byte array
   //+      containing raw rotational information read from the server.
   //! POST: The indices in the data block corresponding to the raw z
   //+       rotation are combined into a single floating-point value that
   //+       is returned to the caller.
   //
   //! ARGS: base - The base address of a 6-byte rotational data array.
   //
   //! RETURNS: The z-axis rotation as a floating-point value in degrees.
   // ------------------------------------------------------------------------
   float toZRot (const unsigned char* base) const
   {
      return toFloatRot(base[0], base[1]);
   }

   // ------------------------------------------------------------------------
   //: Convert the raw rotational information in the given array to the
   //+ rotation about the y-axis which is returned as a single-precision
   //+ floating-point value in degrees.
   //
   //! PRE: A valid data packet has been read into the response buffer.
   //! POST: The indices in the data block corresponding to the raw y
   //+       rotation are combined into a single floating-point value that
   //+       is returned to the caller.
   //
   //! ARGS: base - The base address of a 6-byte rotational data array.
   //
   //! RETURNS: The y-axis rotation as a floating-point value in degrees.
   // ------------------------------------------------------------------------
   float toYRot (const unsigned char* base) const
   {
      return toFloatRot(base[2], base[3]);
   }

   // ------------------------------------------------------------------------
   //: Convert the raw rotational information in the given array to the
   //+ rotation about the x-axis which is returned as a single-precision
   //+ floating-point value in degrees.
   //
   //! PRE: A valid data packet has been read into the response buffer.
   //! POST: The indices in the data block corresponding to the raw x
   //+       rotation are combined into a single floating-point value that
   //+       is returned to the caller.
   //
   //! ARGS: base - The base address of a 6-byte rotational data array.
   //
   //! RETURNS: The x-axis rotation as a floating-point value in degrees.
   // ------------------------------------------------------------------------
   float toXRot (const unsigned char* base) const
   {
      return toFloatRot(base[4], base[5]);
   }

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
   //! ARGS: low_byte  - The byte that will become the low byte of the
   //+                   returned word.
   //
   //! RETURNS: A 16-bit word in host byte order that is the combination of
   //+          the two given bytes.
   // ------------------------------------------------------------------------
   short toShort(const char high_byte, const char low_byte) const;

   // ------------------------------------------------------------------------
   //: Convert the given two bytes into a single-precision floating-point
   //+ positional value.  This is done by converting the bytes to a float
   //+ that is multiplied by the position scaling factor and divided by the
   //+ transmitter divisor as documented in the MotionStar Operation and
   //+ Installation Guide.  Refer to page 88 for the formula.
   //
   //! PRE: None.
   //! POST: The given two bytes are used to generate a single-precision
   //+       floating-point number representing a positional value.
   //
   //! ARGS: high_byte - The most significant byte of the raw data
   //+                   representing a floating-point number.
   //! ARGS: low_byte  - The least significant byte of the raw data
   //+                   representing a floating-point number.
   //
   //! RETURNS: A floating-point number representing a positional value.
   // ------------------------------------------------------------------------
   float toFloatPos (const unsigned char high_byte,
                     const unsigned char low_byte)
      const
   {
      return (m_xmtr_pos_scale * (float) toShort(high_byte, low_byte) /
                 m_xmtr_divisor);
   }

   // ------------------------------------------------------------------------
   //: Convert the given two bytes into a single-precision floating-point
   //+ rotational value (i.e., an angle).  This is done by converting the
   //+ bytes to a float that is multiplied by the rotational scaling factor
   //+ and divided by the transmitter divisor as documented in the MotionStar
   //+ Operation and Installation Guide.  Refer to pages 46-7 for the
   //+ details and the formula.
   //
   //! PRE: None.
   //! POST: The given two bytes are used to generate a single-precision
   //+       floating-point number representing a rotational value.
   //
   //! ARGS: high_byte - The most significant byte of the raw data
   //+                   representing a floating-point number.
   //! ARGS: low_byte  - The least significant byte of the raw data
   //+                   representing a floating-point number.
   //
   //! RETURNS: A floating-point number representing a rotational value.
   // ------------------------------------------------------------------------
   float toFloatRot (const unsigned char high_byte,
                     const unsigned char low_byte)
      const
   {
      return (m_xmtr_rot_scale * (float) toShort(high_byte, low_byte) /
                  m_xmtr_divisor);
   }

   // ------------------------------------------------------------------------
   //: Convert the given two bytes into a single-precision floating-point
   //+ value that falls within a defined range.  This is documented in the
   //+ MotionStar Operation and Installation Guide.
   //
   //! PRE: None.
   //! POST: The given bytes are combined into a floating-point value in the
   //+       range -1.0 to 0.99996.
   //
   //! ARGS: high_byte - The most significant byte of the raw data
   //+                   representing a floating-point number.
   //! ARGS: low_byte  - The least significant byte of the raw data
   //+                   representing a floating-point number.
   //
   //! RETURNS: A floating-point number.
   // ------------------------------------------------------------------------
   float toFloat(const unsigned char high_byte,
                 const unsigned char low_byte) const;

   // ------------------------------------------------------------------------
   //: Convert the given 6-byte array of characters to a double-precision
   //+ floating-point number representing the data rate.
   //
   //! PRE: None.
   //! POST: The given character array is converted to a data measurement
   //+       rate value represented by a double-precision floating-point
   //+       number.
   //
   //! ARGS: rate - A 6-element character array containing the string
   //+              representation of the data measurement rate.
   //
   //! RETURNS: A double-precision floating-point number giving the data
   //+          measurement rate.
   // ------------------------------------------------------------------------
   double convertMeasurementRate(const unsigned char rate[6]);

   // ------------------------------------------------------------------------
   //: Convert the given double-precision floating-point number to a 6-byte
   //+ array of characters representing the data rate.
   //
   //! PRE: None.
   //! POST: The given double-precisoin floating-point value is converted to
   //+       the BIRDNET string representation.  It is a 6-character string
   //+       containing the floating-point value multiplied by 1000.
   //
   //! ARGS: rate     - The measurement rate as a double-precision
   //+                  floating-point number.
   //! ARGS: str_rate - Storage for the string representation of the data
   //+                  measurement rate.
   // ------------------------------------------------------------------------
   void convertMeasurementRate(const double rate, std::string& str_rate);

   // ------------------------------------------------------------------------
   //: Extract the information regarding the measurement units (e.g., inches)
   //+ and the position scaling factor.
   //
   //! PRE: The given single bird status block is valid.
   //! POST: The units for the data generated are read from the status block,
   //+       and the conversion factor is recorded appropriately.  The
   //+       position scaling factor is also read and recorded in
   //+       m_pos_scaling_factor for later use.
   //
   //! ARGS: bird        - The bird whose data block is being passsed.  This
   //+                     is primarily for debugging use.
   //! ARGS: bird_status - The single bird status block being used to get the
   //+                     unit information.
   // ------------------------------------------------------------------------
   void getUnitInfo(const unsigned int bird,
                    const BIRDNET::SINGLE_BIRD_STATUS* bird_status);

   // ------------------------------------------------------------------------
   //: Send the given message to the server.
   //
   //! PRE: The socket to the server is open and is usable for sending data.
   //! POST: The packet is sent to the server.
   //
   //! ARGS: packet      - A pointer to the message to be sent to the server.
   //! ARGS: packet_size - The size of the message being sent.
   //
   //! RETURNS:  0 - The message was sent without error.
   //! RETURNS: -1 - The message could not be sent to the server.  This is
   //+               usually due to a system error.
   //! RETURNS: -2 - Nothing was sent to the server.  This is likely to be
   //+               caused by an attempt to send an empty packet.
   // ------------------------------------------------------------------------
   vpr::ReturnStatus sendMsg(const void* packet, const size_t packet_size);

   // ------------------------------------------------------------------------
   //: Get the server's response to a sent message.
   //
   //! PRE: The socket to the server is open and is usable for reading data.
   //! POST: A packet is read from the server.
   //
   //! ARGS: packet      - A pointer to the memory block into which the
   //+                     server's response packet will be read.
   //! ARGS: packet_size - The size of the given memory block.
   //
   //! RETURNS:  0 - The response was received without error.
   //! RETURNS: -1 - The response could not be read from the server.  This is
   //+               usually due to a system error.
   //! RETURNS: -2 - Nothing was read from the server.  This is likely to be
   //+               caused by the server sending an empty packet for some
   //+               reason.
   // ------------------------------------------------------------------------
   vpr::ReturnStatus getRsp(void* packet, const size_t packet_size);

   // ------------------------------------------------------------------------
   //: Print the system status as read from the server.
   //
   //! PRE: The system status has been read from the server using
   //+      getSystemStatus().
   //! POST: The system status information is printed to stdout in a
   //+       human-readable format.
   //
   //! ARGS: status_info - A pointer to a BIRDNET::SYSTEM_STATUS object
   //+                     that describes the status of the overall system.
   // ------------------------------------------------------------------------
   void printSystemStatus(const BIRDNET::SYSTEM_STATUS* status_info);

   // ------------------------------------------------------------------------
   //: Print the status for all the devices that have not been disabled.
   //
   //! PRE: All the devices have been configured and/or have had their
   //+      current status values queried and set appropriately.
   //! POST: All the device status information is printed to stdout in a
   //        human-readable format.
   // ------------------------------------------------------------------------
   void printDeviceStatus(void);

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

   // General driver member variables.
   bool              m_active;            //: Active state of the driver

   // Server chassis member variables.
   vpr::InetAddr     m_address;           //: Address of the server 
   vpr::Socket*      m_socket;            //: Socket to the server
   BIRDNET::protocol m_proto;             //: Transmission protocol in use
   bool              m_master;            //: Flag stating if this server is a
                                           //+ master (true) or a slave (false)
   unsigned char     m_chassis_id;        //: Unique ID of this chassis
   unsigned char     m_chassis_dev_count; //: Number of devices connected to
                                           //+ this chassis
   unsigned int      m_seq_num;           //: Packet sequence number
   double            m_cur_mrate;         //: Current data measurement rate
                                           //+ (as read from the server)
   double            m_measurement_rate;  //: Measurement rate for the chassis
   BIRDNET::run_mode m_run_mode;          //: Data request mode

   // Bird member variables.
   FLOCK::hemisphere  m_hemisphere;       //: The transmitter hemisphere being
                                          //+ used by the birds
   FLOCK::data_format m_bird_format;      //: Data format for the birds
   unsigned char      m_report_rate;      //: Report rate of the birds
                                          //+ relative to sample rate
   unsigned int       m_birds_requested;  //: Number of birds whose data will
                                          //+ be read
   unsigned int       m_birds_active;     //: Number of birds being sampled

   std::vector<FBB::Device*> m_erc_vec; //: Vector of ERC devices
   std::vector<FBB::Device*> m_birds;   //: Vector of all devices (birds)
                                        //+ connected to the server chassis

   // Data management member variables.
   BIRDNET::units m_units;             //: Units in which data are read
   float          m_unit_conv;         //: Data unit conversion factor
   float          m_xmtr_pos_scale;    //: Transmitter position scaling factor
   float          m_xmtr_rot_scale;    //: Transmitter position scaling factor
   float          m_xmtr_divisor;      //: Number by which all returned values
                                       //+ must be divided
   bool           m_big_endian;        //: If true, the host system is
                                       //+ big-endian
};


#endif  /* _VRJ_ASCENSION_MOTION_STAR_STANDALONE_H_ */
