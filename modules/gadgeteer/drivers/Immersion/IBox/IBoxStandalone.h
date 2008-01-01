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

#ifndef _GADGET_IBOX_STANDALONE_H_
#define _GADGET_IBOX_STANDALONE_H_

#include <vpr/IO/Port/SerialPort.h>
#include <vpr/Util/Debug.h>
#include <vpr/Util/Exception.h>
#include <string>
#include <vector>

/* These numbers are the maximum numbers available in Immersion HCI hardware.
 *   Various Immersion Corp. products based on the Immersion HCI will
 *   support various subsets of these components.
 */
#define NUM_ENCODERS 6  /* Max # encoders supported by Immersion HCI */
#define NUM_ANALOGS  8  /* Max # of A/D channels supported */
#define NUM_BUTTONS  7  /* Max # of buttons supported */

/* # of bytes in longest possible packet, plus some extra room */
#define MAX_PACKET_SIZE         40

/* Max # of args to a special config command */
#define MAX_CFG_SIZE            40

/* Max length of descriptor string from HCI */
#define MAX_STRING_SIZE         32

/* Code sent by HCI to accept a password */
#define PASSWD_OK       0xFF

/** Data packet structure.
 */
struct packet_rec
{
   int error; /**< Flag tells whether there has been an IO error. */
   vpr::Uint32  num_bytes_needed;
   vpr::Uint8    cmd_byte;
   vpr::Uint8  data[MAX_PACKET_SIZE];
   vpr::Uint8    *data_ptr;

   void clear()
   {
      num_bytes_needed = 0;
      cmd_byte = 0;
      error = 0;
      data_ptr = data;
   }
};

/** Exceptions dealing with the IBox. */
class IBoxException : public vpr::Exception
{
public:
   IBoxException(const std::string& msg, const std::string& location = "")
      : vpr::Exception(msg, location)
   {;}

   virtual ~IBoxException() throw()
   {;}

   virtual std::string getExceptionName() const
   { return "IBoxException"; }
};

namespace
{
   /* Compare variables of string constants declared below as if they were enums. */
   const char SIGNON_STR[5] = "IMMC";
   const char BEGIN_STR[6] = "BEGIN";
   const char END_STR[4] = "END";

   /** @name Command bit-field place values */
   //@{
   const vpr::Uint8 PACKET_MARKER   (0x80);
   const vpr::Uint8 CONFIG_BIT      (0x40);
   const vpr::Uint8 TIMER_BIT       (0x20);
   const vpr::Uint8 FUTURE_BIT      (0x10);
   const vpr::Uint8 ANALOG_BITS     (0x0C);
   const vpr::Uint8 ANALOG_HI_BIT   (0x08);
   const vpr::Uint8 ANALOG_LO_BIT   (0x04);
   const vpr::Uint8 ENCODER_BITS    (0x03);
   const vpr::Uint8 ENCODER_HI_BIT  (0x02);
   const vpr::Uint8 ENCODER_LO_BIT  (0x01);
   //@}

   /** @name Labels for the Special Configuration Commands */
   //@{
   // Minimum cmd byte for a config cmd
   const vpr::Uint8 CONFIG_MIN      (0xC0);
   const vpr::Uint8 GET_PARAMS      (0xC0);
   const vpr::Uint8 GET_HOME_REF    (0xC1);
   const vpr::Uint8 HOME_POS        (0xC2);
   const vpr::Uint8 SET_HOME        (0xC3);
   const vpr::Uint8 SET_BAUD        (0xC4);
   const vpr::Uint8 END_SESSION     (0xC5);
   const vpr::Uint8 GET_MAXES       (0xC6);
   //vpr::Uint8 SET_PARAMS            (0xC7);
   const vpr::Uint8 GET_PROD_NAME   (0xC8);
   const vpr::Uint8 GET_PROD_ID     (0xC9);
   const vpr::Uint8 GET_MODEL_NAME  (0xCA);
   const vpr::Uint8 GET_SERNUM      (0xCB);
   const vpr::Uint8 GET_COMMENT     (0xCC);
   const vpr::Uint8 GET_PRM_FORMAT  (0xCD);
   const vpr::Uint8 GET_VERSION     (0xCE);
   const vpr::Uint8 REPORT_MOTION   (0xCF);
   const vpr::Uint8 SET_HOME_REF    (0xD0);
   const vpr::Uint8 RESTORE_FACTORY (0xD1);
   const vpr::Uint8 INSERT_MARKER   (0xD2);
   //@}
}

class IBoxStandalone
{
public:

   /**
    * Constructor.  This creates a port object: sets the default values for
    * timeouts, clears all arrays to 0, and turns off all error handlers.
    *
    * @post slow timeouts are set to 3.0 seconds, fast timeouts are set to .1
    *       all product info arrays are cleared to 0 and all error handlers
    *       are set to NULL, and port is set to NULL.
    */
   IBoxStandalone();

   /**
    * Destructor. Deletes the port object.
    *
    * @post The Serial Port port is deleted.
    */
   ~IBoxStandalone();

   // ------------------------------------------------------------------------
   // Setup & configuration of Immersion Interface Box
   // ------------------------------------------------------------------------


   /**
    * Connects us to the ibox.
    *
    * @post An attempt is made to connect to the ibox.  It if is successful, the
    *       port is opened and has synchronized with the ibox.
    *       The result is returned to the caller.
    *
    * @param port_name A reference to a std::string where the name of the serial
    *                  port is held.
    * @param buad      The buad rate for input and output
    *
    * @throw vpr::IOException is thrown if an I/O exception occurs while
    *        connecting to the IBox.
    * @throw IBoxException is thrown if the serial port could not be opened or
    *        if there is some other protocol error while communicating with
    *        the IBox.
    */
   void connect(const std::string& port_name, long int baud);

   /**
    * Blocks while waiting of an update from the ibox.
    * 
    * @post updates the ibox data and waits for packet before going on.
    *
    * @param timerFlag   Whether or not to report the timer (0 or non-zero).
    * @param numAnalogs  The number of analog fields to report (0, 2, 4, or 8).
    * @param numEncoders The number of encoder fields to report (0, 2, 3, or 6).
    *
    * @throws vpr::IOException if operation failed.
    */
   void waitForUpdate(int timerFlag, int numAnalogs, int numEncoders);

   /**
    * Closes the connection to the ibox.
    *
    * @pre The port is open to the ibox
    * @post An attempt is made to disconnect from the ibox.
    *
    * @throw vpr::IOException is thrown if an I/O exception occurs while
    *        closing the serial port.
    */
   void disconnect();

   /** Returns the state of the button specified by pos.
    *
    * @post returns the state of the button designated by pos.
    *
    * @param pos the button in which to query.
    *
    * @return the state of the button.
    */
   int getButtonData(int pos) const;

   /** Returns the state of the analog device specified by pos.
    *
    * @post returns the state of the analog device designated by pos.
    *
    * @param pos the position in the analog array in which to query.
    *
    * @return the state of the analog device.
    */
   int getAnalogData(int pos) const;

   /** @name Issuing commands */
   //@{

   /** Writes any standard (non-config) cmd and immediately exits.
    *
    * @pre The ibox is connected.
    * @post writes the standard cmd and exits
    *
    * @param timerFlag   Whether or not to report the timer (0 or non-zero).
    * @param numAnalogs  The number of analog fields to report (0, 2, 4, or 8).
    * @param numEncoders The number of encoder fields to report (0, 2, 3, or 6).
    *
    * @throws vpr::IOException if operation failed.
    */
   void sendCommand(const int timerFlag, const int numAnalog,
                    const int numEncoders) const;

   /** Sends the cmd byte of any 'simple' config cmd and immediately exits.
    *
    * Simple cfg cmds do not require passwords, do not concern communications
    * parameters, and have a fixed-length response: GET_HOME_REF, HOME_POS,
    * GET_MAXES, and INSERT_MARKER.
    *
    * @pre The ibox is connected.
    * @post writes the cmd byte of any simple config cmd.
    *
    * @param command the byte that is being written.
    */
   void sendSimpleConfigCommand(const vpr::Uint8 command) const;

   /** Handles commands that request an info string.
    *
    * @pre The ibox is connected.
    * @post reads information from the ibox into the appropriate string member.
    *
    * @param cmnd byte designating which string to read into.
    *
    * @return TIMED_OUT if cmnd isn't satisfied in time or if the read failed<br>
    *         SUCCESS if a string was read into a member character array successfully
    */
   bool sendStringCommand(const vpr::Uint8 cmnd) const;


   /** Handles a cmd that requires a password.
    *
    * @pre Assumes any nec arguments are in cfg_args[].Throws out any chars
    *      currently still in input buffer. Also the ibox must be connected.
    * @post Attempts to write serial number data into member array serial_number.
    *
    * @param cmnd byte designating serial number.
    *
    * @throw vpr::IOException if operation timed out.
    * @throw IBoxException if the password was refused.
    */
   void sendPasswordCommand(const vpr::Uint8 command,
                            const std::vector<vpr::Uint8> args);
   //@}

   /** Checks for a complete packet and parses it if it's ready.
    *
    * @pre The Ibox is connected.
    * @post checks for a complete packet and parses it if it's ready.
    *
    * @throw vpr::TimeoueException if operation timed out.
    * @throw vpr::IOException if operation failed.
    * @throw IBoxException if an invalid packet was recieved or packet parsing
    *        failed.
    */
   void checkForPacket();

   /** Blocks while waiting to receive a complete packet.
    *
    * @pre The Ibox is connected.
    * @post Waits for the given port to recieve a complete packet.
    */
   void waitForPacket();

   /** Sends a motion-sensitive cmd and immediately exits.
    *
    * @pre The ibox is connected.
    * @post attempts to send a motion-sensitive cmd
    *
    * @param timer_flag      Whether or not to report the timer (0 or
    *                        non-zero).
    * @param analog_reports  The number of analog fields to report (0, 2, 4,
    *                        or 8).
    * @param encoder_reports The number of encoder fields to report (0, 2, 3,
    *                        or 6).
    * @param delay           The minimum delay between packets, in (approx)
    *                        milliseconds.
    * @param active_btns     A bit mask indicating which buttons generate
    *                        packets when clicked.
    * @param analog_deltas   Minimum change required to generate a packet.
    * @param encoder_delta   Minimum change required to generate a packet.
    *
    * @throw vpr::IOException if operation failed.
    */
   void startMotionMode(int timer_flag, int analog_reports,
                        int encoder_reports, int delay, vpr::Uint8 active_btns,
                        std::vector<vpr::Uint8>& analog_deltas,
                        std::vector<vpr::Uint8>& encoder_deltas);

   /** Cancels motion-reporting mode and clears all unparsed data.
    *
    * To cancel motion-reporting without clearing data, use insertMarker().
    *
    * @pre The ibox is connected.
    * @post Attempts to cancel motion reporting mode and clear all unparsed data.
    */
   void stopMotionMode();

   /** Checks for a complete packet and parses it if it's ready.
    *
    * But TIMED_OUT is not considered an error; we assume we're in motion-
    * reporting mode, and packets may be few & far between.
    *
    * @pre The Ibox is connected.
    * @post Checks for a complete packet and parses it if it's ready. But
    *       TIMED_OUT is not considered an error; we assume we're in motion-
    *       reporting mode, and packets may be few & far between.
    *
    * @throw vpr::TimeoueException if operation timed out.
    * @throw vpr::IOException if operation failed.
    * @throw IBoxException if an invalid packet was recieved or packet parsing
    *        failed.
    */
   void checkForMotion();

   /** Inserts a place marker packet into the HCI stream.
    *
    * DOES NOT WAIT for response (that would defeat the marker's purpose).
    * Marker field lets you mark different segments of data in incoming
    * buffer.  insertMarker() makes HCI insert a marker into the
    * data stream, so you can switch modes in a host application
    * without misinterpreting some data that may still be waiting
    * in the buffer; just insert a marker, and don't switch modes
    * until you see the marker come back.
    *
    * @pre the ibox is connected.
    * @post writes a marker character.
    *
    * @param marker byte to send as the marker to the ibox.
    */
   void insertMarker(const vpr::Uint8 marker) const;

   /** Gets home reference offsets from HCI, waits for response.
    *
    * @pre The ibox is connected.
    * @post Attempts to get home reference offset from HCI.
    *
    * @throw vpr::TimeoueException if operation timed out.
    * @throw vpr::IOException if operation failed.
    * @throw IBoxException if an invalid packet was recieved or packet parsing
    *        failed.
    */
   void getHomeRef();

   /** Defines a new set of home references for the HCI encoders.
    *
    * @pre The ibox is connected.
    * @post Attempts to define a new set of home references for the HCI encoders.
    *
    * @param homefer Pointer to the location of the new home reference.
    *
    * @throw vpr::IOException if operation timed out.
    * @throw IBoxException if the password was refused.
    */
   void setHomeRef(int *homeref);

   /** Sets HCI encoders to home position, waits for response.
    *
    * @pre The ibox is connected.
    * @post The HCI encoder is set to its home position.
    *
    * @throw vpr::TimeoueException if operation timed out.
    * @throw vpr::IOException if operation failed.
    * @throw IBoxException if an invalid packet was recieved or packet parsing
    *        failed.
    */
   void goHomePosition();

   /** Defines a new home position for the HCI encoders.
    *
    * @pre The ibox is connected.
    * @post Attempts to define a new home position for the HCI encoders.
    *
    * @param homepos Pointer to the new home position.
    *
    * @throw vpr::IOException if operation timed out.
    * @throw IBoxException if the password was refused.
    */
   void setHomePosition(int *homepos);

   /** Asks HCI for max field values, waits for response
    *
    * @pre The ibox is connected.
    * @post reads the max field values.
    *
    * @throw vpr::TimeoueException if operation timed out.
    * @throw vpr::IOException if operation failed.
    * @throw IBoxException if an invalid packet was recieved or packet parsing
    *        failed.
    */
   void getMaxValues();

   /** Restores all factory settings
    *
    * @pre The ibox is connected.
    * @post resets all member variable information.
    *
    * @throw vpr::IOException if operation timed out.
    * @throw IBoxException if the password was refused.
    */
   void setFactoryDefaults();

private:
   /** Reads chars from serial buffer into the packet array.
    *
    * @pre The Ibox is connected.
    * @post Reads chars from serial buffer into the packet array.
    *       Sets packet.error flag if the cmd_byte is not a valid cmd byte.
    *       Sets num_bytes_expected to -1 if the cmd is not one that the standard
    *       parser (parsePacket()) can deal with.
    *
    * @throw vpr::TimeoueException if operation timed out.
    * @throw vpr::IOException if operation failed.
    * @throw IBoxException if an invalid packet was recieved.
    */
   void getDataPacket();

   /** @name Packet parsing functions. */
   //@{

   /** Interprets the hci's packet and stores all HCI data.
    *
    * @pre Before calling this, call getDataPacket() to see if packet is ready.
    * @post parsePacket() interprets the hci's packet and stores all HCI data.
    *       Also marks this hci's packet as having been parsed.
    *
    * @throw IBoxException if packet could not be parsed.
    */
   void parsePacket();

   /** Parses a packet for a special configuration command.
    *
    * @pre The packet is COMPLETE.
    * @post parses a packet for a special configureation command.
    *
    * @throw IBoxException if packet could not be parsed.
    */
   void parseConfigPacket();
   
   //@}

   /** Returns the # of data bytes that FOLLOW a given cmd byte.
    *
    * @post returns the # of data bytes that FOLLOW a given cmd byte.
    *
    * @param cmd The cmd that marks the start for counting the number of bytes.
    *
    * @return value of 0 means packet needs special handling (i.e. passwd) or
    *         has uncertain length; too complicated for standard parser. <br>
    *         Otherwise returns the number of bytes following cmd.
    */
   vpr::Uint32 getPacketSize(const vpr::Uint8 packetType);

   /** Reads a null-terminated string from the serial port.
    *
    * @pre The Ibox is connected.
    * @post A null-terminated string is read into str.
    *
    * @return String recieved from IBox.
    *
    * @throw vpr::IOException if operation failed.
    */
   std::string readString();
    
   /** Sets all updated fields to zero.
    */
   void clearUpdateFieldss();

private:
   /** @name Helper functions */
   //@{
   bool autoSync();
   bool beginCommunication();

   /** Send end command to IBox.
    *
    * @throw vpr::IOException if operation failed.
    */
   void sendEndCommand();

   /** Get information about the connected IBox device.
    *
    * @throw vpr::IOException if operation failed.
    */
   void getInfo();

   /** Get the command byte for the given parameters.
    *
    * @param timerFlag   Whether or not to report the timer (0 or non-zero).
    * @param numAnalogs  The number of analog fields to report (0, 2, 4, or 8).
    * @param numEncoders The number of encoder fields to report (0, 2, 3, or 6).
    */
   vpr::Uint8 getCommandByte(const int timerFlag, const int numAnalogs,
                             const int numEncoders) const;
   //@}
private:
   vpr::SerialPort* mSerialPort; /**< Serial port. */
   std::string mPortName; /**< Name of serial port. */
   int mBaudRate;         /**< Baud rate of IBox. */
   packet_rec mPacket;    /**< The current packet. */

   /** @name Description strings. */
   //@{
   std::string mSerialNumber;
   std::string mProductName;
   std::string mProductId;
   std::string mModelName;
   std::string mComment;
   std::string mParamFormat;
   std::string mVersion;
   //@}

   /* Normalization values for primary quantities:
    *   These values give some reference or normalization quantity
    *     for each field.
    *   A zero in any of these fields means there is no hardware
    *     support for that data in this particular system.
    */
   int mButtonSupported[NUM_BUTTONS];
   int mMaxTimer;
   int mMaxAnalog[NUM_ANALOGS];   /**< Full-scale A/D reading. */
   int mMaxEncoder[NUM_ENCODERS]; /**< Max value each encoder reaches INCLUDING quadrature. */
   int mMarker;
   int mMarkerUpdated;

   /* Encoder "home" position:
    *   The relative encoders supported by the Immersion HCI only report
    *   their NET angular motion from the time they are powered up.  If
    *   an encoder's initial angular value is important, it must somehow
    *   be assumed or calibrated at start-up.  These fields contain values
    *   to be assumed at start-up.  They can be read from or written to the
    *   Immersion HCI EEPROM.  If written to the Immersion HCI EEPROM,
    *   these "home" values will be retained even after power is turned off.
    */
   //int     mHome_pos [NUM_ENCODERS];

   /** Home position references:
    * In many cases some calibration procedure will be required to ensure
    * that the encoder positions truly match the assumed home position.
    * This array can store any data that is useful for that purpose.
    */
   int mHomeRef[NUM_ENCODERS];

   /** @name Primary fields. */
   //@{
   int mButtonBits; /**< Button bits all together. */
   int mButtonValues[NUM_BUTTONS]; /**< ON/OFF flags for buttons. */
   long mTimer;                /**< Running counter */
   int mAnalog[NUM_ANALOGS];   /**< A/D channels */
   int mEncoder[NUM_ENCODERS]; /**< Encoder counts */
   //@}

   /** @name Status of primary fields:
    *   A zero in any of these fields indicates that the corresponding
    *     primary quantity is out of date (wasn't updated by the
    *     previous packets)
    *   Note: buttons are updated with every packet
    */
   //@{
   int mTimerUpdated;
   int mAnalogUpdated[NUM_ANALOGS];
   int mEncoderUpdated[NUM_ENCODERS];
   //@}
};

#endif /* _GADGET_IBOX_STANDALONE_H_ */
