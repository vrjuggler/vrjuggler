/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2008 by Iowa State University
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
 *************** <auto-copyright.pl END do not edit this line> ***************/

#ifndef _GADGET_ASCENSION_MOTION_STAR_STANDALONE_H_
#define _GADGET_ASCENSION_MOTION_STAR_STANDALONE_H_

#include <iostream>
#include <string>
#include <vector>
#include <gmtl/Matrix.h>

#include <vpr/vpr.h>

// VPR_HAVE_HASH_MAP will be defined to 1 or 0 by vpr/vprConfig.h which is
// included by vpr/vpr.h.
#if VPR_HAVE_HASH_MAP
#  include VPR_HASH_MAP_INCLUDE
#else
#  include <map>
#endif

#include <vpr/IO/Socket/InetAddr.h>
#include <vpr/IO/Socket/Socket.h>

#include <drivers/Ascension/MotionStar/MotionStarExceptions.h>


/** Ascension Flock namespace. */
namespace FLOCK
{

/**
 * Flock hemispheres.  The integer values correspond to the values used by
 * the flock.
 */
enum hemisphere
{
   FRONT_HEMISPHERE = 0,
   REAR_HEMISPHERE  = 1,
   UPPER_HEMISPHERE = 2,
   LOWER_HEMISPHERE = 3,
   LEFT_HEMISPHERE  = 4,
   RIGHT_HEMISPHERE = 5
};

/**
 * Possible data formats that devices connected to a Flock can use.  The
 * integer values correspond to the values used by the Flock.
 */
enum data_format
{
   NO_BIRD_DATA        = 0, /**< Inactive device */
   POSITION            = 1, /**< Position information only */
   ANGLES              = 2, /**< Euler angle information only */
   MATRIX              = 3, /**< Angle information in matrix form only */
   POSITION_ANGLES     = 4, /**< Position and Euler angle information */
   POSITION_MATRIX     = 5, /**< Position and angle information in a matrix */
   INVALID             = 6, /**< Placeholder */
   QUATERNION          = 7, /**< Angle information as quaternion parameters */
   POSITION_QUATERNION = 8, /**< Possition information and angle information as quaternion parameters */
   FEEDTHROUGH_DATA    = 14
};

/**
 * Data blocks corresponding to the Flock data formats.  Refer to page 136 of
 * the MotionStar Operation and Installation Guide for a table of the data
 * format sizes.
 */
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
extern const unsigned char ERR_STATE;
extern const unsigned char RUNNING;
//extern const unsigned char RESERVED1;
//extern const unsigned char RESERVED2;
extern const unsigned char BUTTONS_PRESENT;
extern const unsigned char SENSOR_PRESENT;
extern const unsigned char TRANSMITTER_PRESENT;
extern const unsigned char TRANSMITTER_RUNNING;

// Constants indicating the bit to set when defining the individual bird setup.
extern const unsigned char SUDDEN_OUTPUT_CHANGE;
extern const unsigned char XYZ_REFERENCE;
extern const unsigned char APPEND_BUTTON_DATA;
extern const unsigned char AC_NARROW_FILTER;
extern const unsigned char AC_WIDE_FILTER;
extern const unsigned char DC_FILTER;

/**
 * Converts the given Flock data format into a human-readable string that
 * names the format.
 *
 * @pre None.
 * @post A std::string is returned with a name corresponding to the given
 *       format value.
 *
 * @param format The data format whose name is being requested.
 *
 * @return A std::string object containing the name of the given format.
 */
std::string getFormatName(const data_format format);

/**
 * Converts the given Flock hemisphere ID into a human-readable string that
 * names the hemisphere.
 *
 * @pre None.
 * @post A std::string is returned with a name corresponding to the given
 *        hemisphere ID value.
 *
 * @param hemi The hemisphere whose name is being requested.
 *
 * @return A std::string object containing the name of the given hemisphere.
 */
std::string getHemisphereName(const hemisphere hemi);

}  // namspace FLOCK

/**
 * Fast Bird Bus (FBB) namespace.  This collects information common to the
 * Ascension FBB system into one space.
 */
namespace FBB
{

extern const unsigned char ACCESS;  /**< Bit to test if the device is accessible */
extern const unsigned char RUNNING; /**< Bit to test if the device is running */
extern const unsigned char SENSOR;  /**< Bit to test if the device has a sensor */
extern const unsigned char ERC;     /**< Bit to test if the devices is an ERC */
extern const unsigned char ERT3;    /**< Bit to test if ERT #3 is present */
extern const unsigned char ERT2;    /**< Bit to test if ERT #2 is present */
extern const unsigned char ERT1;    /**< Bit to test if ERT #1 is present */
extern const unsigned char ERT0;    /**< Bit to test if ERT #0 is present */

/**< Fast Bird Bus device structure. */
struct Device
{
    // These are set using the FBB status byte returned when the general
    // system status is requested from the master server.
    bool                accessible;   /**< */
    bool                running;      /**< */
    bool                has_sensor;   /**< */
    bool                is_erc;       /**< */
    bool                ert0_present; /**< */
    bool                ert1_present; /**< */
    bool                ert2_present; /**< */
    bool                ert3_present; /**< */

    // These are set when the individual status of the bird is requested.
    unsigned char       data_words;   /**< Number of words in formatted data */
    FLOCK::data_format  format;       /**< Format of data read from device */
    unsigned char       report_rate;  /**< Report rate relative to sample rate */
    FLOCK::hemisphere   hemisphere;   /**< Hemisphere in use */
    unsigned char       address;      /**< FBB address of this device */
    FLOCK::data_block   data;         /**< Data read from this device */
    unsigned char       buttons[2];   /**< Bird button data */

    unsigned char       addr;
    unsigned char       setup;        /**< Device setup parameters */
};

}  // namespace FBB

/**
 * BIRDNET namespace.  This collects all MotionStar BIRDNET protocol-specific
 * things into one namespace to prevent potential name collisions.  The
 * protocol in use is as specified in the MotionStar Installation and
 * Operation Guide dated May 27, 1999.
 */
namespace BIRDNET
{

// BIRDNET command/response codes.  See pages 112-3 of the MotionStar
// Installation and Operation Guide.
extern const unsigned char MSG_WAKE_UP;     /**< Wake up the server */
extern const unsigned char RSP_WAKE_UP;     /**< Server response to wake-up */
extern const unsigned char MSG_SHUT_DOWN;   /**< Shutdown the server */
extern const unsigned char RSP_SHUT_DOWN;   /**< Server response to shutdown */
extern const unsigned char MSG_GET_STATUS;  /**< Request system status */
extern const unsigned char MSG_SEND_SETUP;  /**< Send setup to server */

/** Request single asyncronous data packet */
extern const unsigned char MSG_SINGLE_SHOT;

extern const unsigned char MSG_RUN_CONTINUOUS; /**< Request continuous data */
extern const unsigned char MSG_STOP_DATA;      /**< Stop continuous data */
extern const unsigned char RSP_GET_STATUS;     /**< Response of system status */
extern const unsigned char RSP_SEND_SETUP;     /**< Response to sent setup */

/** Response to continuous data request */
extern const unsigned char RSP_RUN_CONTINUOUS;

/** Response to continuous data stop request */
extern const unsigned char RSP_STOP_DATA;

/** Single-shot and continuous data packet from the server */
extern const unsigned char DATA_PACKET_MULTI;

/** Response to packet received at an inappropriate time */
extern const unsigned char RSP_ILLEGAL;

/** Response to packet with invalid command/response code */
extern const unsigned char RSP_UNKNOWN;

/** Reset server sequence numbers */
extern const unsigned char MSG_SYNC_SEQUENCE;

/** Response to sequence number reset request */
extern const unsigned char RSP_SYNC_SEQUENCE;

extern const unsigned char BN_PROTOCOL;        /**< BIRDNET protocol ID */

/**
 * BIRDNET packet header.  It is 16 bytes long.  See page 111 of the
 * MotionStar Installation and Operation Guide.
 */
struct HEADER
{
   /**
    * Simple constructor.
    *
    * @pre None.
    * @post Commonly used fields are initialized to standard values.  The
    *        type and the sequence fields are NOT initialized.
    */
   HEADER()
      : sequence(0), xtype(0), protocol(BN_PROTOCOL), error_code(0),
        error_code_extension(0), number_bytes(0)
   {
      /* Do nothing. */ ;
   }

   /**
    * Simple constructor for a header of the given type.
    *
    * @pre None.
    * @post Commonly used fields are initialized to standard values, and the
    *        type field is set to msgType.  The sequence field is not
    *        initialized.
    *
    * @param msgType The type of message being constructed.
    */
   HEADER(const unsigned char msgType)
      : sequence(0), type(msgType), xtype(0), protocol(BN_PROTOCOL),
        error_code(0), error_code_extension(0), number_bytes(0)
   {
      /* Do nothing. */ ;
   }

   unsigned short sequence;     /**< 16-bit sequence number */
   unsigned short milliseconds; /**< Interval time between packets */

   /** Seconds since 00:00:00 Jan. 1, 1970 (GMT) */
   unsigned char  time[4];
   unsigned char  type;         /**< Command/response code */
   unsigned char  xtype;        /**< Extension to command/response code */

   /** Protocol for this packet (BIRDNET is 3) */
   unsigned char  protocol;

   /**
    * Error code for an event at the server concerning the previous client
    * packet
    */
   unsigned char  error_code;

   unsigned short error_code_extension; /**< Additional error code info */
   unsigned short number_bytes;   /**< Size of the following data field */
};

/**
 * BIRDNET general system status packet.  It is 16 bytes long.  See page 121
 * of the MotionStar Installation and Operation Guide.
 */
struct SYSTEM_STATUS
{
   unsigned char all;                 /**< General system status */
   unsigned char error;               /**< Reserved */
   unsigned char flockNumber;         /**< Number of devices on FBB bus */
   unsigned char serverNumber;        /**< Number of servers in system */
   unsigned char transmitterNumber;   /**< Address of current active xmtr */
   unsigned char measurementRate[6];  /**< Measurement rate * 1000 (text) */
   unsigned char chassisNumber;       /**< Chassis ID number */
   unsigned char chassisDevices;      /**< Number of devices in chassis */
   unsigned char firstAddress;        /**< FBB address of first device */
   unsigned char softwareRevision[2]; /**< Software rev. of server app */
};

/**
 * BIRDNET singleBirdStatus packet.  It is 16 bytes long.  See page 125 of
 * the MotionStar Installation and Operation Guide.
 */
struct SINGLE_BIRD_STATUS
{
   unsigned char status;         /**< Device status */
   unsigned char id;             /**< Device type */
   unsigned char softwareRev[2]; /**< Software revision of firmware */
   unsigned char errorCode;      /**< Reserved for future use */
   unsigned char setup;          /**< Device setup parameters */
   unsigned char dataFormat;     /**< Data Format Code and Data Size */
   unsigned char reportRate;     /**< Rate at which data is generated
                                      relative to the sample rate */
   unsigned char scaling[2];     /**< Full scale output */
   unsigned char hemisphere;     /**< Bird's hemisphere of operation */
   unsigned char FBBaddress;     /**< FBB address */
   unsigned char transmitterType; /**< Transmitter type */
   unsigned char spare1;         /**< Reserved */
   unsigned char spare2[2];      /**< Reserved */
};

/**
 * BIRDNET FILTER_TABLE structure.  This is for an ERC/ERT (Extended Range
 * Controller/Extended Range Transmitter).  See page 131 of the MotionStar
 * Installation and Operation Guide.
 */
struct FILTER_TABLE
{
   unsigned short entry[7];
};

/**
 * BIRDNET XYZREF_TABLE structure.  Word 1 is azimuth, word 2 is elevation
 * and word 3 is roll.  See page 132 of the MotionStar Installation and
 * Operation Guide.
 */
struct XYZREF_TABLE
{
   unsigned short angle[3];
};

/**
 * BIRDNET indiviual Flock status packet.  It is 70 bytes long.  See page 124
 * of the MotionStar Installation and Operation Guide.
 */
struct BIRD_STATUS
{
   struct SINGLE_BIRD_STATUS status;            /**< See SINGLE_BIRD_STATUS */
   struct FILTER_TABLE       alphaMin;          /**< Page 61 of I&O Guide */
   struct FILTER_TABLE       alphaMax;          /**< Pages 65-6 of I&O Guide */
   struct FILTER_TABLE       Vm;                /**< Pages 66-7 of I&O Guide */
   struct XYZREF_TABLE       xyzReferenceFrame;
   struct XYZREF_TABLE       xyzAngleAlign;
};

/**
 * Generic BIRDNET DATA_PACKET packet.  This is used when reading device data
 * from the server.
 * XXX: Is it safe for the buffer to be fixed at 2048 bytes?  It should be
 * set to MAXDATA, but I do not know where that is defined.
 */
struct DATA_PACKET
{
   struct HEADER header;       /**< Header that describes the data buffer */
   unsigned char buffer[2048]; /**< Memory block containing read data */
};

/**
 * BIRDNET DATA_RECORD structure.  Refer to page 138 of the MotionStar
 * Installation and Operatiion Guide.
 */
struct DATA_RECORD
{
   unsigned char     address;        /**< FBB address of the device whose
                                          data is contained in the record */
   unsigned char     data_info;      /**< Description of the data block */
   FLOCK::data_block data;           /**< Variable-sized data read from the device */
   unsigned char     button_data[2]; /**< Information about the optional
                                          button attached to the sensor (this
                                          may not always be part of the data
                                          block sent by teh server) */
};

/** Possible transmission protocols for use between the client and the server. */
enum protocol
{
   UDP,
   TCP
};

/**< Possible data request modes. */
enum run_mode
{
   CONTINUOUS,     /**< Read data continuously from the server */
   SINGLE_SHOT     /**< Read a single asynchronous packet of data */
};

/**
 * Possible units being used by the data read from the server.  We list more
 * values than are actually supported at this time in the event that Ascension
 * extends the capabilities of the hardware.
 */
enum units
{
   INCHES, /**< The hardware returns inches.  This is the only supported type at this time. */
   FEET,   /**< The hardware returns feet. */
   METERS  /**< The hardware returns meters. */
};

}  // namspace BIRDNET


/**
 * The protocol in use is as specified in the MotionStar Installation and
 * Operation Guide dated May 27, 1999.
 */
class MotionStarStandalone
{
public:
   /**
    * Constructor.  This initializes member variables and determines the
    * endianness of the host machine.
    *
    * @pre None.
    * @post Important member variables determining the setup of the driver
    *       are initialized, and the endianness of the host machine is
    *       determined.  This is used later when reading data words from the
    *       server's response packets.
    *
    * @param address           The IP address or hostname of the MotionStar
    *                          server to which we are connecting.
    * @param port              The port on the server to which we connect.
    * @param proto             The transmission protocol to use between the
    *                          client and the server.
    * @param master            A flag stating whether this server chassis is
    *                          a master (true) or a slave (false).
    * @param hemisphere        The hemisphere in use on the transmitter.
    * @param birdFormat        The bird data format.
    * @param runMode           The data request mode for the server.
    * @param reportRate        The rate at which data is generated by the
    *                          birds relative to the sample rate (0-255).
    * @param measurementRate
    */
   MotionStarStandalone(const char* address, const unsigned short port,
                        const BIRDNET::protocol proto, const bool master,
                        const FLOCK::hemisphere hemisphere,
                        const FLOCK::data_format birdFormat,
                        const BIRDNET::run_mode runMode,
                        const unsigned char reportRate,
                        const double measurementRate);

   /**
    * Destructor.
    *
    * @pre None.
    * @post The device is stopped, and the memory allocated for the server
    *       address is released.
    */
   ~MotionStarStandalone();

   /**
    * Initializes the driver, setting the status for each bird.
    *
    * @pre The server address (either IP address or hostname) has been set.
    * @post A connection attempt is made to the server.  If successful, the
    *       socket is set up in connected mode.  Each bird has its status
    *       set regardless of the connection attempt results.  If the
    *       connection attempt fails, error status is returned to the
    *       caller.
    *
    * @return \c true is returned if the device is started correctly.
    *         \c false is returned otherwise.
    *
    * @throw mstar::NetworkException is thrown if the socket to be used for
    *        communication with the MotionStar chassis cannot be opened.
    * @throw mstar::ConnectException is thrown if the connection to the
    *        MotionStar chassis cannot be completed.
    * @throw mstar::ScaleFactorUnknownException is thrown if parsing the
    *        received data from the MotionStar chassis encounters in an
    *        unknown data scale factor.
    */
   bool start();

   /**
    * The data flow from the server is stopped, the server is told to go to
    * sleep and the socket is closed.
    *
    * @pre Refer to stopData() and shutdown().
    * @post If the run mode is BIRDNET::CONTINUOUS, the data flow is
    *        stopped, the server is told to shut down, the state is set to
    *        inactive and the socket is closed.
    *
    * @throw mstar::CommandException is thrown if the command sent to the
    *        MotionStar chassis telling it to stop communication fails or
    *        if the command telling the MotionStar to go to sleep fails.
    *
    * @see stopData()
    * @see shutdown()
    */
   void stop();

   /**
    * Based on the current run mode, a single sample is taken (run mode is
    * 1), or continuous samples are taken (run mode is 0).
    *
    * @pre The socket has been initialized and is currently open to
    *       receive data from the server.
    * @post
    */
   void sample();

   /**
    * Stops the data flow if it is in continuous mode.
    *
    * @pre If the current run mode is BIRDNET::CONTINUOUS, then the socket
    *      has been initialized and is currently open for sending and
    *      receiving data.
    * @post A MSG_STOP_DATA command is sent to the server.
    *
    * @throw mstar::CommandException is thrown if the command send to the
    *        MotionStar chassis telling it to stop communication fails.
    */
   void stopData();

   /**
    * Shuts down the server chassis.
    *
    * @pre The socket has been initialized and is currently open to
    *      receive data from the server.
    * @post A MSG_SHUT_DOWN command is sent to the server.  If the server is
    *       shut down successfully, m_active is set to false.
    *
    * @throw mstar::CommandException is thrown if the command send to the
    *        MotionStar chassis telling it to go to sleep fails.
    */
   void shutdown();

   /**
    * Returns whether the MotionStar is active or not.
    *
    * @pre None.
    * @post The value in m_active is returned to the caller.
    *
    * @return A boolean value stating whether or not the device is active.
    */
   const bool& isActive() const
   {
      return m_active;
   }

   /**
    * Sets the address (either IP address or hostname) for the server.
    *
    * @pre None.
    * @post The value in m_address is replaced with the string pointed to by
    *       addr.
    *
    * @param addr The new value for the server address.
    */
   void setAddressName(const char* addr)
   {
      m_address.setAddress(addr);
   }

   /**
    * Sets the address (either IP address or hostname) for the server using a
    * std::string object reference.
    *
    * @pre None.
    * @post
    *
    * @param addr The new value for the server address.
    */
   void setAddressName(const std::string& addr)
   {
      m_address.setAddress(addr);
   }

   /**
    * Gets the current server address as a string.
    *
    * @pre None.
    * @post
    *
    * @return A std::string object containing the address name.
    */
   const std::string getAddressName() const
   {
      return m_address.getAddressString();
   }

   /**
    * Sets the address object for the server.
    *
    * @pre None.
    * @post The value in m_address is replaced with the value in addr.
    *
    * @param addr The new value for the server address.
    */
   void setAddress(const vpr::InetAddr& addr)
   {
      m_address = addr;
   }

   /**
    * Get the current server address object.
    *
    * @pre None.
    * @post The current value of m_address is returned to the caller.
    *
    * @return A vpr::InetAddr object containing the server address.
    */
   const vpr::InetAddr& getAddress() const
   {
      return m_address;
   }

   /**
    * Sets the port on the server to which we connect.
    *
    * @pre None.
    * @post The value in port is stored in m_port.
    *
    * @param port  The new value for the server port.
    */
   void setServerPort(const vpr::Uint16 port)
   {
      m_address.setPort(port);
   }

   /**
    * Gets the server port.
    *
    * @pre None.
    * @post The current value of m_port is returned to the caller.
    *
    * @return An unsigned short integer containing the server port number.
    */
   vpr::Uint16 getServerPort() const
   {
      return m_address.getPort();
   }

   /**
    * Sets the transmission protocol used between the client and the server.
    *
    * @pre None.
    * @post The value in proto is stored in m_proto.
    *
    * @param proto The new value for the transmission protocol.
    */
   void setProtocol(const enum BIRDNET::protocol proto)
   {
      m_proto = proto;
   }

   /**
    * Gets the transmission protocol used between the client and the server.
    *
    * @pre None.
    * @post The current value of m_proto is returned to the caller.
    *
    * @return
    */
   const BIRDNET::protocol& getProtocol() const
   {
      return m_proto;
   }

   /**
    * Sets the master status for this server chassis.
    *
    * @pre None.
    * @post The value in master is stored in m_master.
    *
    * @param master The new value for the master status of this server chassis.
    */
   void setMasterStatus(const bool master)
   {
      m_master = master;
   }

   /**
    * Gets the master status for this server chassis.
    *
    * @pre None.
    * @post The current value of m_master is returned to the caller.
    *
    * @return true if this server chassis is a master; false if it is a slave.
    */
   const bool& getMasterStatus() const
   {
      return m_master;
   }

   /**
    * Sets the run mode for the device.
    *
    * @pre None.
    * @post If the driver is active and the run mode is being toggled from
    *       BIRDNET::CONTINUOUS to BIRDNET::SINGLE_SHOT, the data flow from
    *       the server is stopped.  If the driver is active and the run mode
    *       being toggled from BIRDNET::SINGLE_SHOT to BIRDNET::CONTINUOUS,
    *       the server is told to begin sending a continuous stream of data.
    *       m_run_mode is set to mode after all of that is handled.
    *
    * @param mode The new value for the run mode.
    *
    * @throw mstar::CommandException is thrown if communication with the
    *        MotionStar chassis fails.
    */
   void setRunMode(const BIRDNET::run_mode mode);

   /**
    * Gets the current run mode for the device.
    *
    * @pre None.
    * @post The current run mode is returned to the caller.
    *
    * @return An integer value for run mode.
    */
   const BIRDNET::run_mode& getRunMode() const
   {
      return m_run_mode;
   }

   /**
    * Gets the number of sensors currently connected to the MotionStar.
    *
    * @pre None.
    * @post The current number of connected sensors is returned to the caller.
    *
    * @return An unsigned integer value for the number of connected sensors.
    */
   const unsigned int& getNumSensors() const
   {
      return mNumSensors;
   }

   /**
    * Changes the birds' hemisphere of operation.
    *
    * @pre None.
    * @post The hemisphere for it is reset to the given value.
    *
    * @param hemisphere The new hemisphere to use.
    */
   void setHemisphere(const FLOCK::hemisphere hemisphere)
   {
      m_hemisphere = hemisphere;
   }

   /**
    * Get the transmitter hemisphere currently in use by the birds.
    *
    * @pre None.
    * @post The current hemisphere value is returned to the caller.
    *
    * @return An unsigned integer value for the hemisphere in use.
    */
   const FLOCK::hemisphere& getHemisphere() const
   {
      return m_hemisphere;
   }

   /**
    * Sets the data format the birds will use to the given value.
    *
    * @pre None.
    * @post The bird data format is updated to the new value.
    *
    * @param format The new bird data format to use.
    */
   void setBirdFormat(const FLOCK::data_format format)
   {
      m_bird_format = format;
   }

   /**
    * Get the bird data format currently in use.
    *
    * @pre None.
    * @post The current bird data format value is returned to the caller.
    *
    * @return An unsigned integer value for the bird data format.
    */
   const FLOCK::data_format& getBirdFormat() const
   {
      return m_bird_format;
   }

   /**
    * Set the report rate for the birds.
    *
    * @pre None.
    * @post The bird report rate is set to the given byte value.
    *
    * @param rate The new value for the bird report rate.
    */
   void setReportRate(const unsigned char rate)
   {
      m_report_rate = rate;
   }

   /**
    * Gets the current report rate for the birds.
    *
    * @pre None.
    * @post The current bird report rate is returned to the caller.
    *
    * @return An unsigned character value for the bird report rate.
    */
   const unsigned char& getReportRate() const
   {
      return m_report_rate;
   }

   void setExpectedUnits(const BIRDNET::units units)
   {
      mExpectedUnits = units;
   }

   /**
    * Sets the data measurement rate for the chassis.
    *
    * @pre None.
    * @post The chassis data measurement rate is set to the given byte
    *       value.
    *
    * @param rate The new value for the chassis data measurement rate.
    */
   void setMeasurementRate(const double rate)
   {
      m_measurement_rate = rate;
   }

   /**
    * Gets the current data measurement rate.
    *
    * @pre None.
    * @post The current data measurement rate is returned to the caller.
    *
    * @return A double-precision floating-point value for data measurement
    *         rate.
    */
   const double& getMeasurementRate() const
   {
      return m_measurement_rate;
   }

   /**
    * Get a pointer to the FBB device indexed by the given bird ID.  The
    * indexing is zero-based.
    *
    * @pre The given bird index is in the vector of defined devices.
    * @post A pointer to the requested device is returned to the caller.
    *
    * @param bird The zero-based index of the FBB device objcet that is to
    *             be returned.
    *
    * @return An FBB::Device pointer that points to the requested FBB
    *         device object.
    */
   const FBB::Device* getBird(const unsigned int bird)
   {
      return m_birds[bird];
   }

   /**
    * Gets the data format in use by a specific bird.
    *
    * @pre The given bird index is in the vector of defined devices.
    * @post The data format for the requested FBB device is returned to the
    *       caller.
    *
    * @param bird The zero-based index of the FBB device whose data format
    *             is to be returned.
    *
    * @return A FLOCK::data_format value representing the data format of
    *         the requested device.
    */
   const FLOCK::data_format& getBirdDataFormat(const unsigned int bird)
   {
      return m_birds[bird]->format;
   }

   const gmtl::Matrix44f& getDeviceData(const unsigned int i) const
   {
      return mSampleData[i];
   }

private:
   void storeSample(FBB::Device* birdDev, FLOCK::data_block* data);

   /**
    * Gets the rotation angles from the given data block as matrix elements
    * that are then stored in the given array.
    *
    * @pre The data is in the format FLOCK::MATRIX or FLOCK::POSITION_MATRIX.
    * @post The rotations about the x-, y- and z-axes are extracted from the
    *       rotational matrix read from the bird and are stored in the
    *       angles array.
    *
    * @param format The format of the data block.
    * @param data   The block of data from which the angle information will
    *               be extracted.
    * @param angles A 3-element array used to store the x-, y- and z-axis
    *               rotations extracted from the rotation matrix.
    */
   void getMatrixAngles(const FLOCK::data_format format,
                        const FLOCK::data_block* data, float angles[3]) const;

   /**
    * Gets the rotation angles from the given data block as quaternion
    * parameters that are then stored in the given array.
    *
    * @pre The data is in the format FLOCK::QUATERNION or
    *      FLOCK::POSITION_QUATERNION.
    * @post The rotations about the x-, y- and z-axes are extracted as
    *       quaternion parameters read from the bird and are stored in the
    *       quat array.
    *
    * @param format The format of the data block.
    * @param data   The block of data from which the angle information will
    *               be extracted.
    * @param quat    A 4-element array used to store the quaternion parameters
    *                extracted from the quatternion data read by the bird.
    */
   void getQuaternion(const FLOCK::data_format format,
                      const FLOCK::data_block* data, float quat[4]) const;

   /**
    * Sends a wake-up call to the MotionStar server.
    *
    * @pre The socket has been initialized and is currently open to
    *      send and receive data from the server.
    * @post A MSG_WAKE_UP packet is sent to the server.  If the server
    *       responds but has set an error code, send a MSG_SHUT_DOWN packet
    *       and another MSG_WAKE_UP packet to restart the server.
    *
    * @throw mstar::CommandException is thrown if it is necessary to
    *        re-initialize the MotionStar chassis with a wake-up call but
    *        that effort fails.
    */
   void sendWakeUp();

   /**
    * Gets the system status.
    *
    * @pre The socket has been initialized and is currently open to
    *      send and receive data from the server.
    * @post A MSG_GET_STATUS packet is sent to the server.  The response is
    *       handled by recording all the important configuration data read
    *       from the server.
    *
    * @return A pointer to a BIRDNET::SYSTEM_STATUS object that fully
    *         describes the system state or NULL if the system status could
    *         not be read.
    *
    * @throws mstar::CommandException is thrown if communication with the
    *         MotionStar chassis fails.
    */
   BIRDNET::SYSTEM_STATUS* getSystemStatus();

   /**
    * Sets the system status.
    *
    * @pre The socket has been initialized and is currently open to
    *      send and receive data from the server.
    * @post The system configuration is sent by sending a configuration
    *       packet for device 0.  The configuration packet sets the
    *       transmitter number and the data rate.
    *
    * @param sysStatus The current system status.
    * @param xmtrNum   The transmitter number to use.
    * @param dataRate  The data measurement rate to use.
    *
    * @return \c true if the configuration was sent successfully; \c false
    *         otherwise.
    */
   bool setSystemStatus(BIRDNET::SYSTEM_STATUS* sysStatus,
                        const unsigned char xmtrNum, const char dataRate[6]);

   /**
    * Read the configurations of all the birds and send our configuration
    * data to them.
    *
    * @pre The socket has been initialized and is currently open to
    *       send and receive data from the server.
    * @post Each bird's configuration is read individually.  Based on the
    *       current configuration and whatever configuration parameters have
    *       been set in the driver configuration, a new configuration is
    *       sent back to the server telling it how we want to use each bird.
    *       Any birds that are available but are not requested by the
    *       current configuration are disabled.
    *
    * @return An integer stating how many birds were configured.
    *
    * @note The scaling factor, the measurement units (e.g., inches)  and
    *       the report rate for all the birds are determined here.
    */
   unsigned int configureBirds();

   /**
    * Gets the status of an individual bird.
    *
    * @pre The socket has been initialized and is currently open to
    *      send and receive data from the server.  The requested bird index
    *      is in the vector of birds.
    * @post The status of the given device is requested from the server.  If
    *       it is read successfully, the FBB device object in the m_birds
    *       vector corresponding to the given bird index is updated to
    *       contain the configuration read.
    *
    * @param bird The bird whose status is being requested.  This is a
    *             zero-based value.
    *
    * @return A non-NULL pointer to a BIRDNET::BIRD_STATUS object describing
    *         the requested bird's current status.  NULL is returned if no
    *         status information could be read for the requested bird for
    *         some reason.
    */
   BIRDNET::BIRD_STATUS* getBirdStatus(const unsigned char bird);

   /**
    * Sets the status of an individual bird.
    *
    * @pre The socket has been initialized and is currently open to
    *      send and receive data from the server.  The requested bird index
    *      is in the vector of birds.
    * @post The FBB device corresponding to the given bird index is sent
    *       the passed configuration block.
    *
    * @param bird   The bird whose status is being set.  This is a
    *               zero-based value.
    * @param status A pointer to a BIRDNET::BIRD_STATUS object describing
    *               the new configuration for the given bird.
    *
    * @return \c true is returned if the bird was configured successfully.
    *         \c false is returned if the bird could not be configured for
    *         some reason.
    */
   bool setBirdStatus(const unsigned char bird,
                                   BIRDNET::BIRD_STATUS* status);

   /**
    * Gets the status of the requested FBB device.  The device number must be
    * that of the device's address on the Fast Bird Bus.  Thus, a value of
    * 0 is interpreted as a request for the overall system status.  The
    * birds are addressed from 1 through 120.
    *
    * @pre The socket has been initialized and is currently open to
    *      send and receive data from the server.
    * @post The status for the given device is requested from the server
    *       and is stored in an allocated BIRDNET::DATA_PACKET block that
    *       is then returned to the caller.
    *
    * @param device The FBB address of the device to be configured.
    *
    * @return A pointer to a BIRDNET::DATA_PACKET that the caller should
    *         delete when it has finished with it.  NULL is returned if the
    *         status for the given device could not be requested for some
    *         reason.
    *
    * @throw mstar::NoDeviceStatusException is thrown if communication with
    *        the MotionStar chassis fails.
    */
   BIRDNET::DATA_PACKET* getDeviceStatus(const unsigned char device);

   /**
    * Sets the status of the requested FBB device using the given
    * configuration block.  The device number must be that of the device's
    * address on the Fast Bird Bus.  Thus, a value of 0 is interpreted as a
    * configuation block for the overall system.  The birds are addressed
    * from 1 through 120.
    *
    * @pre The socket has been initialized and is currently open to
    *      send and receive data from the server.
    * @post A full packet is constructed from the variable-sized data block
    *       passed by the user.  This packet is then sent to the server as
    *       a configuration packet for the named device.
    *
    * @param device      The FBB address of the device to be configured.
    * @param buffer      A configuration block for the given device.  The
    *                    real type should correspond to the type of device
    *                    being configured (either the whole system or an
    *                    individual bird).  This will be written into the
    *                    data buffer of a constructed packet that is then
    *                    sent to the server.
    * @param buffer_size The size of the configuration buffer.
    */
   bool setDeviceStatus(const unsigned char device, const char* buffer,
                        const unsigned short buffer_size);

   /**
    * Tells the MotionStar server to sample continuously.
    *
    * @pre The socket has been initialized and is currently open to
    *      send and receive data from the server.
    * @post A MSG_RUN_CONTINUOUS packet is sent to the server.  The server
    *       responds and then begins sending a continuous stream of data.
    *
    * @return \c true if the server is now sending continuous data.
    *         \c false is returned if the run mode could not be set to
    *         BIRDNET::CONTINUOUS.
    *
    * @throw mstar::CommandException is thrown if communication with the
    *        MotionStar chassis fails.
    */
   bool setContinuous();

   /**
    * Converts the raw positional information in the given array to the
    * translation along the x-axis which is returned as a single-precision
    * floating-point value.
    *
    * @pre The address given is the base address of a 6-byte array
    *      containing raw positional information read from the server.
    * @post The indices in the data block corresponding to the raw x
    *       position are combined into a single floating-point value that
    *       is returned to the caller.
    *
    * @param base The base address of a 6-byte positional data array.
    *
    * @return The x-axis translation as a floating-point value in the units
    *         being used by the device.
    */
   float toXPos(const unsigned char* base) const
   {
      return toFloatPos(base[0], base[1]);
   }

   /**
    * Converts the raw positional information in the given array to the
    * translation along the y-axis which is returned as a single-precision
    * floating-point value.
    *
    * @pre The address given is the base address of a 6-byte array
    *      containing raw positional information read from the server.
    * @post The indices in the data block corresponding to the raw y
    *       position are combined into a single floating-point value that
    *       is returned to the caller.
    *
    * @param base The base address of a 6-byte positional data array.
    *
    * @return The y-axis translation as a floating-point value in the units
    *         being used by the device.
    */
   float toYPos(const unsigned char* base) const
   {
      return toFloatPos(base[2], base[3]);
   }

   /**
    * Converts the raw positional information in the given array to the
    * translation along the z-axis which is returned as a single-precision
    * floating-point value.
    *
    * @pre The address given is the base address of a 6-byte array
    *      containing raw positional information read from the server.
    * @post The indices in the data block corresponding to the raw z
    *       position are combined into a single floating-point value that
    *       is returned to the caller.
    *
    * @param base The base address of a 6-byte positional data array.
    *
    * @return The z-axis translation as a floating-point value in the units
    *         being used by the device.
    */
   float toZPos(const unsigned char* base) const
   {
      return toFloatPos(base[4], base[5]);
   }

   /**
    * Converts the raw rotational information in the given array to the
    * rotation about the z-axis which is returned as a single-precision
    * floating-point value in degrees.
    *
    * @pre The address given is the base address of a 6-byte array
    *      containing raw rotational information read from the server.
    * @post The indices in the data block corresponding to the raw z
    *       rotation are combined into a single floating-point value that
    *       is returned to the caller.
    *
    * @param base The base address of a 6-byte rotational data array.
    *
    * @return The z-axis rotation as a floating-point value in degrees.
    */
   float toZRot(const unsigned char* base) const
   {
      return toFloatRot(base[0], base[1]);
   }

   /**
    * Converts the raw rotational information in the given array to the
    * rotation about the y-axis which is returned as a single-precision
    * floating-point value in degrees.
    *
    * @pre A valid data packet has been read into the response buffer.
    * @post The indices in the data block corresponding to the raw y
    *       rotation are combined into a single floating-point value that
    *       is returned to the caller.
    *
    * @param base The base address of a 6-byte rotational data array.
    *
    * @return The y-axis rotation as a floating-point value in degrees.
    */
   float toYRot(const unsigned char* base) const
   {
      return toFloatRot(base[2], base[3]);
   }

   /**
    * Convert the raw rotational information in the given array to the
    * rotation about the x-axis which is returned as a single-precision
    * floating-point value in degrees.
    *
    * @pre A valid data packet has been read into the response buffer.
    * @post The indices in the data block corresponding to the raw x
    *       rotation are combined into a single floating-point value that
    *       is returned to the caller.
    *
    * @param base The base address of a 6-byte rotational data array.
    *
    * @return The x-axis rotation as a floating-point value in degrees.
    */
   float toXRot(const unsigned char* base) const
   {
      return toFloatRot(base[4], base[5]);
   }

   /**
    * Combines the two given bytes (passed as high byte and low byte
    * respectively) into a single word in host byte order.  This is used for
    * reading bytes from the packets and converting them into usable values.
    *
    * @pre None.
    * @post The two given bytes are combined into a single word that is
    *       returned to the caller.
    *
    * @param highByte The byte that will become the high byte of the
    *                 returned word.
    * @param lowByte  The byte that will become the low byte of the
    *                 returned word.
    *
    * @return A 16-bit word in host byte order that is the combination of
    *         the two given bytes.
    */
   short toShort(const char highByte, const char lowByte) const;

   /**
    * Converts the given two bytes into a single-precision floating-point
    * positional value.  This is done by converting the bytes to a float
    * that is multiplied by the position scaling factor and divided by the
    * transmitter divisor as documented in the MotionStar Operation and
    * Installation Guide.  Refer to page 88 for the formula.
    *
    * @pre None.
    * @post The given two bytes are used to generate a single-precision
    *       floating-point number representing a positional value.
    *
    * @param highByte The most significant byte of the raw data
    *                 representing a floating-point number.
    * @param lowByte  The least significant byte of the raw data
    *                 representing a floating-point number.
    *
    * @return A floating-point number representing a positional value.
    */
   float toFloatPos(const unsigned char highByte,
                    const unsigned char lowByte)
      const
   {
      return (m_xmtr_pos_scale * (float) toShort(highByte, lowByte) /
              m_xmtr_divisor);
   }

   /**
    * Converts the given two bytes into a single-precision floating-point
    * rotational value (i.e., an angle).  This is done by converting the
    * bytes to a float that is multiplied by the rotational scaling factor
    * and divided by the transmitter divisor as documented in the MotionStar
    * Operation and Installation Guide.  Refer to pages 46-7 for the
    * details and the formula.
    *
    * @pre None.
    * @post The given two bytes are used to generate a single-precision
    *       floating-point number representing a rotational value.
    *
    * @param highByte The most significant byte of the raw data
    *                 representing a floating-point number.
    * @param lowByte  The least significant byte of the raw data
    *                 representing a floating-point number.
    *
    * @return A floating-point number representing a rotational value.
    */
   float toFloatRot(const unsigned char highByte,
                    const unsigned char lowByte)
      const
   {
      return (m_xmtr_rot_scale * (float) toShort(highByte, lowByte) /
              m_xmtr_divisor);
   }

   /**
    * Converts the given two bytes into a single-precision floating-point
    * value that falls within a defined range.  This is documented in the
    * MotionStar Operation and Installation Guide.
    *
    * @pre None.
    * @post The given bytes are combined into a floating-point value in the
    *       range -1.0 to 0.99996.
    *
    * @param highByte The most significant byte of the raw data
    *                 representing a floating-point number.
    * @param lowByte  The least significant byte of the raw data
    *                 representing a floating-point number.
    *
    * @return A floating-point number.
    */
   float toFloat(const unsigned char highByte,
                 const unsigned char lowByte) const;

   /**
    * Converts the given 6-byte array of characters to a double-precision
    * floating-point number representing the data rate.
    *
    * @pre None.
    * @post The given character array is converted to a data measurement
    *       rate value represented by a double-precision floating-point
    *       number.
    *
    * @param rate A 6-element character array containing the string
    *             representation of the data measurement rate.
    *
    * @return A double-precision floating-point number giving the data
    *         measurement rate.
    */
   double convertMeasurementRate(const unsigned char rate[6]);

   /**
    * Converts the given double-precision floating-point number to a 6-byte
    * array of characters representing the data rate.
    *
    * @pre None.
    * @post The given double-precisoin floating-point value is converted to
    *       the BIRDNET string representation.  It is a 6-character string
    *       containing the floating-point value multiplied by 1000.
    *
    * @param rate    The measurement rate as a double-precision
    *                floating-point number.
    * @param strRate Storage for the string representation of the data
    *                measurement rate.
    */
   void convertMeasurementRate(const double rate, std::string& strRate);

   /**
    * Extracts the information regarding the measurement units (e.g., inches)
    * and the position scaling factor.
    *
    * @pre The given single bird status block is valid.
    * @post The units for the data generated are read from the status block,
    *        and the conversion factor is recorded appropriately.  The
    *        position scaling factor is also read and recorded in
    *        m_pos_scaling_factor for later use.
    *
    * @param bird       The bird whose data block is being passsed.  This
    *                   is primarily for debugging use.
    * @param birdStatus The bird status block being used to get the unit
    *                   information.
    */
   void getUnitInfo(const unsigned int bird,
                    const BIRDNET::BIRD_STATUS* birdStatus);

   /**
    * Sends the given message to the server.
    *
    * @pre The socket to the server is open and is usable for sending data.
    * @post The packet is sent to the server.
    *
    * @param packet     A pointer to the message to be sent to the server.
    * @param packetSize The size of the message being sent.
    *
    * @throw mstar::NetworkWriteException is thrown if sending data on the
    *        connected socket fails.
    * @throw mstar::NoDataWrittenException is thrown if the write succeeds
    *        but no data is written to the socket. This would mean that the
    *        send failed to do anything.
    */
   void sendMsg(const void* packet, const size_t packetSize);

   /**
    * Gets the server's response to a sent message.  This version takes a
    * pointer to a BIRDNET::HEADER as its argument because most of the
    * communication with the MotionStar chassis involves sending and
    * receiving packets using the BIRDNET::HEADER format.  Because this
    * version knows it has BIRDNET::HEADER data, it automatically manages
    * information in the header that sendMsg(void*, const size_t).  In
    * particular, it sets the sequence number of the packet being sent.
    * As such, callers should not set packet->sequence themselves.
    *
    * @pre The socket to the server is open and is usable for sending data.
    *      The sequence number in the packet is not already set.
    * @post The packet is sent to the server.
    *
    * @param packet A pointer to the message to be sent to the server.
    *
    * @throw mstar::NetworkWriteException is thrown if sending data on the
    *        connected socket fails.
    * @throw mstar::NoDataWrittenException is thrown if the write succeeds
    *        but no data is written to the socket. This would mean that the
    *        send failed to do anything.
    *
    * @see sendMsg(const void*, const size_t)
    */
   void sendMsg(BIRDNET::HEADER* packet);

   /**
    * Gets the server's response to a sent message.
    *
    * @pre The socket to the server is open and is usable for reading data.
    * @post A packet is read from the server.
    *
    * @param packet     A pointer to the memory block into which the
    *                   server's response packet will be read.
    * @param packetSize The size of the given memory block.
    *
    * @throw mstar::NetworkReadException is thrown if reading data on the
    *        connected socket fails.
    * @throw mstar::NoDataReadException is thrown if the read succeeds but no
    *        data was read from the socket. This would mean that the read
    *        failed to do anything.
    */
   void getRsp(void* packet, const size_t packetSize);

   /**
    * Gets the server's response to a sent message.  This version takes a
    * pointer to a BIRDNET::HEADER as its argument because most of the
    * communication with the MotionStar chassis involves sending and
    * receiving packets using the BIRDNET::HEADER format.  Because this
    * version knows it has BIRDNET::HEADER data, it can perform automatic
    * error checking that getRsp(void*, const size_t) cannot do.
    *
    * @pre The socket to the server is open and is usable for reading data.
    * @post A packet in the format BIRDNET::HEADER is read from the server.
    *
    * @param packet A pointer to the memory block into which the server's
    *               response packet will be read.
    *
    * @throw mstar::NetworkReadException is thrown if reading data on the
    *        connected socket fails.
    * @throw mstar::NoDataReadException is thrown if the read succeeds but no
    *        data was read from the socket. This would mean that the read
    *        failed to do anything.
    *
    * @see getRsp(void*, const size_t)
    */
   void getRsp(BIRDNET::HEADER* packet);

   /**
    * Prints the system status as read from the server.
    *
    * @pre The system status has been read from the server using
    *      getSystemStatus().
    * @post The system status information is printed to stdout in a
    *       human-readable format.
    *
    * @param statusInfo A pointer to a BIRDNET::SYSTEM_STATUS object
    *                   that describes the status of the overall system.
    */
   void printSystemStatus(const BIRDNET::SYSTEM_STATUS* statusInfo);

   /**
    * Prints the status for all the devices that have not been disabled.
    *
    * @pre All the devices have been configured and/or have had their
    *      current status values queried and set appropriately.
    * @post All the device status information is printed to stdout in a
    *       human-readable format.
    */
   void printDeviceStatus();

   /**
    * Prints the error message that corresponds to the given error code.
    * The message is based on the table on page 140 of the MotionStar manual.
    *
    * @pre None.
    * @post A human-readable error message is printed to stderr.
    *
    * @param errCode The single-byte error code.
    */
   void printError(const unsigned char errCode);

   // General driver member variables.
   bool              m_active;            /**< Active state of the driver */

   // Server chassis member variables.
   vpr::InetAddr     m_address;           /**< Address of the server */
   vpr::Socket*      m_socket;            /**< Socket to the server */
   BIRDNET::protocol m_proto;             /**< Transmission protocol in use */
   bool              m_master;            /**< Flag stating if this server is a
                                               master (true) or a slave (false) */
   unsigned char     m_chassis_id;        /**< Unique ID of this chassis */
   unsigned char     m_chassis_dev_count; /**< Number of devices connected to
                                               this chassis */
   unsigned int      m_seq_num;           /**< Packet sequence number */
   double            m_cur_mrate;         /**< Current data measurement rate
                                               (as read from the server) */
   double            m_measurement_rate;  /**< Measurement rate for the chassis */
   BIRDNET::run_mode m_run_mode;          /**< Data request mode */

   // Bird member variables.
   FLOCK::hemisphere  m_hemisphere;       /**< The transmitter hemisphere being used by the birds */
   FLOCK::data_format m_bird_format;      /**< Data format for the birds */
   unsigned char      m_report_rate;      /**< Report rate of the birds relative to sample rate */
   unsigned int       mNumSensors;        /**< Number of active sensors */

   std::vector<FBB::Device*> m_birds;   /**< Vector of all devices (birds)
                                             connected to the server chassis */

   /**
    * Define the type for \c mAddrToSensorIdMap. We use an actual map here
    * rather than, say, a vector because the range of FBB addresses is not
    * necessarily [0,n] for n birds. Indeed, a case has been encountered where
    * four birds have addresses 2, 3, 4, and 5. In that case, the size of the
    * container would be 5 (the extra is to account for the ERT), but we would
    * be trying to access (zero-based) index 5 as the largest rahter than
    * index 4.
    */
#if VPR_HAVE_HASH_MAP
#  if defined(_MSC_VER)
   typedef stdext::hash_map<int, int> addr_sensor_map_t;
#  else
   typedef std::hash_map<int, int> addr_sensor_map_t;
#  endif
#else
   typedef std::map<int, int> addr_sensor_map_t;
#endif

   /**
    * Mapping of FBB address (which can be anything greater than or equal to
    * zero) to a zero-based, sequential range of integer identifiers.
    */
   addr_sensor_map_t mAddrToSensorIdMap;
   std::vector<gmtl::Matrix44f> mSampleData;

   /** Expected units of position data.  Used for debugging. */
   BIRDNET::units mExpectedUnits;

   // Data management member variables.
   BIRDNET::units m_units;             /**< Units in which data are read */
   float          m_xmtr_pos_scale;    /**< Transmitter position scaling factor */
   float          m_xmtr_rot_scale;    /**< Transmitter rotation scaling factor */
   float          m_xmtr_divisor;      /**< Number by which all returned values must be divided */
   bool           m_big_endian;        /**< If true, the host system is big-endian */
};


#endif  /* _GADGET_ASCENSION_MOTION_STAR_STANDALONE_H_ */
