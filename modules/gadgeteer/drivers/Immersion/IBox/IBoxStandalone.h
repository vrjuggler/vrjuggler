/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2005 by Iowa State University
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

#ifndef _GADGET_IBOX_STANDALONE_H_
#define _GADGET_IBOX_STANDALONE_H_

#include <gadget/Devices/DriverConfig.h>
#include <string>
#include <vpr/IO/Port/SerialPort.h>

/*-----------*/
/* Constants */
/*-----------*/

#define NUM_ENCODERS    6       /* Max # encoders supported by Immersion HCI */
#define NUM_ANALOGS     8       /* Max # of A/D channels supported */
#define NUM_BUTTONS     7       /* Max # of buttons supported */

//enum(


/* These numbers are the maximum numbers available in Immersion HCI hardware.
 *   Various Immersion Corp. products based on the Immersion HCI will
 *   support various subsets of these components.
 */

/*------------*/
/* Data Types */
/*------------*/

/* Use ibox_result as if it were an enumerated type.  Compare variables of
 *   this type to the string constants declared below.  An ibox_result is
 *   actually a string, so it can be directly printed as well.
 */

//typedef const char*   ibox_result;

/* Shorthand for a byte */
typedef unsigned char   byte;	
//vpr::Uint8

/* # of bytes in longest possible packet, plus some extra room */
#define MAX_PACKET_SIZE         40

/* Max # of args to a special config command */
#define MAX_CFG_SIZE            40

/* Max length of descriptor string from HCI */
#define MAX_STRING_SIZE         32

/* Code sent by HCI to accept a password */
#define PASSWD_OK       0xFF

/* Space for arguments to config commands */

/** Record for packet
 */
struct packet_rec
{
	int     parsed;         /**< Flag tells whether this packet has been parsed */
    int     error;          /* Flag tells whether there has been com error */
	int     num_bytes_needed;
    byte    cmd_byte;
	byte    data[MAX_PACKET_SIZE];
	byte    *data_ptr;

	void clear()
	{
		num_bytes_needed = 0;
		cmd_byte = 0;
		parsed = 1;
        error = 0;
		data_ptr = data;
	}

};

//typedef struct
//{
//	int     parsed;         /* Flag tells whether this packet has been parsed */
//	int     error;          /* Flag tells whether there has been com error */
//	int     num_bytes_needed;
//	int 	packet_ex
//	byte    cmd_byte;
//	byte    data[MAX_PACKET_SIZE];
//	byte    *data_ptr;
//} packet_rec;


// Macro for creating a command byte from a timer flag and the # of analog *  and encoder reports desired

/*#define CMD_BYTE(t, anlg, encd) \
        (																						\
		  (t ? TIMER_BIT : 0) | 																\
		  (anlg > 4 ? ANALOG_BITS : (anlg > 2 ? ANALOG_HI_BIT :(anlg ? ANALOG_LO_BIT : 0))) |	\
		  (encd > 3 ? ENCODER_BITS :(encd > 2 ? ENCODER_HI_BIT :(encd ? ENCODER_LO_BIT : 0)))	\
		)
*/
		
namespace
{
    const char SIGNON_STR[5] = "IMMC";
	const char BEGIN_STR[6] = "BEGIN";
	const char END_STR[4] = "END";
	vpr::Interval fastTimeOut;
	vpr::Interval slowTimeOut;
	
	/*---------------------*/
	/* Low-level Constants */
	/*---------------------*/


    /** @name Command bit-field place values */
	//@{
	const byte PACKET_MARKER   =0x80;
	const byte CONFIG_BIT      =0x40;
	const byte TIMER_BIT       =0x20;
	const byte FUTURE_BIT      =0x10;
	const byte ANALOG_BITS     =0x0C;
	const byte ANALOG_HI_BIT   =0x08;
	const byte ANALOG_LO_BIT   =0x04;
	const byte ENCODER_BITS    =0x03;
	const byte ENCODER_HI_BIT  =0x02;
	const byte ENCODER_LO_BIT  =0x01;
	//@}

	/* Labels for the Special Configuration Commands */
	const byte CONFIG_MIN      =0xC0;    /* Minimum cmd byte for a config cmd */
	const byte GET_PARAMS      =0xC0;
	const byte GET_HOME_REF    =0xC1;
	const byte HOME_POS        =0xC2;
	const byte SET_HOME        =0xC3;
	const byte SET_BAUD        =0xC4;
	const byte END_SESSION     =0xC5;
	const byte GET_MAXES       =0xC6;
	//byte SET_PARAMS      =0xC7;
	const byte GET_PROD_NAME   =0xC8;
	const byte GET_PROD_ID     =0xC9;
	const byte GET_MODEL_NAME  =0xCA;
	const byte GET_SERNUM      =0xCB;
	const byte GET_COMMENT     =0xCC;
	const byte GET_PRM_FORMAT  =0xCD;
	const byte GET_VERSION     =0xCE;
	const byte REPORT_MOTION   =0xCF;
	const byte SET_HOME_REF    =0xD0;
	const byte RESTORE_FACTORY =0xD1;
	const byte INSERT_MARKER   =0xD2;
}

class IboxStandalone{
public:

	/**
     * Constructor.  This creates a port object: sets the default values for
     * timeouts, clears all arrays to 0, and turns off all error handlers.
     *
     * @post slow timeouts are set to 3.0 seconds, fast timeouts are set to .1
     *       all product info arrays are cleared to 0 and all error handlers
     *       are set to NULL, and port is set to NULL.
     */
    IboxStandalone();

    /**
     * Destructor. Deletes the port object.
     *
     * @post The Serial Port port is deleted.
     */
    ~IboxStandalone();

    // ------------------------------------------------------------------------
    // Setup & configuration of Immersion Interface Box
    // ------------------------------------------------------------------------


    /**
     * Connects us to the ibox
     *
     * @post An attempt is made to connect to the ibox.  It if is successful, the
     *       port is opened and has synchronized with the ibox.
     *       The result is returned to the caller.
     *
     * @param port_name A reference to a std::string where the name of the serial
     *                  port is held.
     * @param buad      The buad rate for input and output
     *
     * @return SUCCESS is returned if the the ibox connects successfully. <br>
     *         CANT_BEGIN if the ibox was synched but couldn't begin
     *         reading data. <br>
     *         NO_HCI if the ibox could not successfully synchronize. <br>
     *         CANT_OPEN_PORT if connecting to the serial port was unsuccessful.
     */
    vpr::ReturnStatus    connect(const std::string& port_name, long int baud);
	vpr::ReturnStatus    connect();
    
	/**
     * ibox_wait_update() updates ibox data.
     * @post updates the ibox data and waits for packet before going on.
     *
     * @param timer_flag is the whether or not to report the timer (0 or non-zero). <br>
     * @param num_analogs is the # of analog fields to report (0, 2, 4, or 8) <br>
     * @param num_encoders is the # of encoder fields to report (0, 2, 3, or 6)
     *
     * @return NO_PACKET_YET if a packet was not sent by the ibox; return SUCCESS
     *         otherwise.
     */
    vpr::ReturnStatus    wait_update(int timer_flag, int num_analogs
                        ,int num_encoders);

    /**
     * Closes the connection to the ibox.
     *
     * @pre The port is open to the ibox
     * @post An attempt is made to disconnect from the ibox.
     */
    void            disconnect();

    //------------------------------------------------------------------------
    // Issuing commands to HCI
    //------------------------------------------------------------------------

    /** hci_std_cmd() writes any standard (non-config) cmd and immediately exits.
     *
     * @pre The ibox is connected.
     * @post writes the standard cmd and exits
     *
     * @param timer_flag is the whether or not to report the timer (0 or non-zero). <br>
     * @param num_analogs is the # of analog fields to report (0, 2, 4, or 8) <br>
     * @param num_encoders is the # of encoder fields to report (0, 2, 3, or 6)
     */
    void            std_cmd(int timer_flag, int analog_reports,
                                 int encoder_reports);

    /**
     * hci_simple_cfg_cmd() sends the cmd byte of any 'simple' config cmd
     * and immediately exits; simple cfg cmds do not require passwords,
     * do not concern communications parameters, and have a fixed-length
     * response: GET_HOME_REF, HOME_POS, GET_MAXES, and INSERT_MARKER.
     *
     * @pre The ibox is connected.
     * @post writes the cmd byte of any simple config cmd.
     *
     * @param cmnd the byte that is being written.
     */

    void            simple_cfg_cmd(byte cmnd);

    /**
     *
     * hci_string_cmd() handles commands that request an info string.
     *
     * @pre The ibox is connected.
     * @post reads information from the ibox into the appropriate string member.
     *
     * @param cmnd byte designating which string to read into.
     *
     * @return TIMED_OUT if cmnd isn't satisfied in time or if the read failed<br>
     *         SUCCESS if a string was read into a member character array successfully
     */
    vpr::ReturnStatus    string_cmd(byte cmnd);


    /**
     * hci_passwd_cmd() handles a cmd that requires a password.
     *
     * @pre Assumes any nec arguments are in cfg_args[].Throws out any chars
     *      currently still in input buffer. Also the ibox must be connected.
     * @post Attempts to write serial number data into member array serial_number.
     *
     * @param cmnd byte designating serial number.
     *
     * @return BAD_PASSWORD if ibox sends bad data. <br>
     *         TIMED_OUT if at any point we time out before we finish reading from the
     *         ibox. <br>
     *         SUCCESS if serial_number is filled and ibox sent password back to caller.
     */
    vpr::ReturnStatus    passwd_cmd(byte cmnd);

    /**
     * hci_insert_marker() inserts a place marker packet into the HCI stream and
     * DOES NOT WAIT for response (that would defeat the marker's purpose).
     *
     * @pre the ibox is connected.
     * @post writes a marker character.
     *
     * @param marker byte to send as the marker to the ibox.
     */
    void            insert_marker(byte marker);


    /**
     * get_home_ref() gets home reference offsets from HCI, waits for response.
     *
     * @pre The ibox is connected.
     * @post Attempts to get home reference offset from HCI.
     *
     * @returns SUCCESS if the offset was successfully recieved. <br>
     *          TIMED_OUT if the offset was unsuccessfully aquired.
     */
    vpr::ReturnStatus    get_home_ref();

    /**
     * set_home_ref() defines a new set of home references for the HCI encoders.
     *
     * @pre The ibox is connected.
     * @post Attempts to define a new set of home references for the HCI encoders.
     *
     * @param homefer Pointer to the location of the new home reference.
     *
     * @return BAD_PASSWORD if ibox sends bad data. <br>
     *         TIMED_OUT if at any point we time out before we finish reading from the
     *         ibox. <br>
     *         SUCCESS if the home position was successfully changed.
     */
    vpr::ReturnStatus    set_home_ref(int *homeref);

    /**
     * go_home_pos() sets HCI encoders to home position, waits for response.
     *
     * @pre The ibox is connected.
     * @post Attempt to set the HCI encoder to home position.
     *
     * @return SUCCESS if the offset was successfully recieved. <br>
     *          TIMED_OUT if the offset was unsuccessfully aquired.
     */
    vpr::ReturnStatus    go_home_pos();

    /**
     * set_home_pos() defines a new home position for the HCI encoders.
     *
     * @pre The ibox is connected.
     * @post Attempts to define a new home position for the HCI encoders.
     *
     * @param homepos Pointer to the new home position.
     *
     * @return BAD_PASSWORD if ibox sends bad data. <br>
     *         TIMED_OUT if at any point we time out before we finish reading from the
     *         ibox. <br>
     *         SUCCESS if the home position was successfully changed.
     */
    vpr::ReturnStatus    set_home_pos(int *homepos);

    /**
     * get_maxes() asks HCI for max field values, waits for response
     *
     * @pre The ibox is connected.
     * @post reads the max field values.
     *
     * @return BAD_PASSWORD if ibox sends bad data. <br>
     *         TIMED_OUT if at any point we time out before we finish reading from the
     *         ibox. <br>
     *         SUCCESS if the max field values were successfully retrieved.
     */
    vpr::ReturnStatus    get_maxes();

    /**
     * factory_settings() restores all factory settings
     *
     * @pre The ibox is connected.
     * @post resets all member variable information.
     *
     * @return BAD_PASSWORD if ibox sends bad data. <br>
     *         TIMED_OUT if at any point we time out before we finish reading from the
     *         ibox. <br>
     *         SUCCESS if the factory settings were successfully restored.
     */
    vpr::ReturnStatus    factory_settings();

    /**
     * report_motion() sends a motion-sensitive cmd and immediately exits.
     *
     * @pre The ibox is connected.
     * @post attempts to send a motion-sensitive cmd
     *
     * @param timer_flag is the whether or not to report the timer (0 or non-zero). <br>
     * @param analog_reports is the # of analog fields to report (0, 2, 4, or 8) <br>
     * @param encoder_reports is the # of encoder fields to report (0, 2, 3, or 6)i <br>
     * @param delay is minimum delay between packets, in (approx) msec. <br>
     * @param active_btns is a bit mask indicating which buttons generate packets
     *                    when clicked. <br>
     * @param analog_deltas are min. change required to generate a packet. <br>
     * @param encoder_delta are min. change required to generate a packet. <br>
     */
    void            report_motion(int timer_flag,
                    int analog_reports, int encoder_reports, int delay,
                    byte active_btns, int *analog_deltas, int *encoder_deltas);

    /**
     * end_motion() cancels motion-reporting mode and clears all unparsed data;
     * to cancel motion-reporting without clearing data, use insert_marker().
     *
     * @pre The ibox is connected.
     * @post Attempts to cancel motion reporting mode and clear all unparsed data.
     */
    void            end_motion();

    /**
     * hci_wait_packet() waits for the given port to receive a complete packet and
     * times out if it takes too long.
     *
     * @pre The Ibox is connected.
     * @post Waits for the given port to recieve a complete packet and times out if
     *       it takes too long.
     *
     * @return TIMED_OUT if port timed out before a complete packet was recieved. <br>
     *         SUCCESS otherwise.
     */
    vpr::ReturnStatus    wait_packet();

    /**
     * check_packet() checks for a complete packet and parses it if it's ready.
     *
     * @pre The Ibox is connected.
     * @post checks for a complete packet and parses it if it's ready.
     *
     * @return TIMED_OUT if packet is not complete and it's been too long since the
     *         last timeout was started. <br>
     *         SUCCESS otherwise.
     */
    vpr::ReturnStatus check_packet();

    /**
     * check_motion() checks for a complete packet and parses it if it's ready.
     * But TIMED_OUT is not considered an error; we assume we're in motion-
     * reporting mode, and packets may be few & far between.
     *
     * @pre The Ibox is connected.
     * @post Checks for a complete packet and parses it if it's ready. But
     *       TIMED_OUT is not considered an error; we assume we're in motion-
     *       reporting mode, and packets may be few & far between.
     *
     * @return BAD_PACKET if there was a problem parsing the packet. <br>
     *         NO_PACKET_YET if there isn't a packet. <br>
     *         TIMED_OUT if a packet hasn't been recieved yet. <br>
     *         SUCCESS otherwise.
     */
    vpr::ReturnStatus	check_motion();

    /**
     * build_packet() reads chars from serial buffer into the packet array.
     *
     * @pre The Ibox is connected.
     * @post Reads chars from serial buffer into the packet array.
     *       Sets packet.error flag if the cmd_byte is not a valid cmd byte.
     *       Sets num_bytes_expected to -1 if the cmd is not one that the standard
     *       parser (parse_packet()) can deal with.
     *
     * @return FALSE if a valid packet is not yet complete. <br>
     *         TRUE when packet-building stops due to completion or an error.
     */
     vpr::ReturnStatus	build_packet();

    //---------------------------------------------------------------------------
    // Packet parsing functions
    //---------------------------------------------------------------------------

    /**
     * parse_packet() interprets the hci's packet and stores all HCI data.
     *
     * @pre Before calling this, call build_packet() to see if packet is ready.
     * @post parse_packet() interprets the hci's packet and stores all HCI data.
     *       Also marks this hci's packet as having been parsed.
     *
     * @return BAD_PACKET if the packet doesn't have enough bytes. <br>
     *         NO_PACKET_YET if the packet is not complete. <br>
     *         SUCCESS otherwise.
     */
    vpr::ReturnStatus	parse_packet();

    /**
     * parse_cfg_packet() parses a packet for a special configuration command.
     *
     * @pre The packet is COMPLETE.
     * @post parses a packet for a special configureation command.
     *
     * @return BAD_PACKET if the command was not parsed successfully. <br>
     *         SUCCESS if the command was parsed successfully.
     */
    vpr::ReturnStatus	parse_cfg_packet();

    /**
     * packet_size() returns the # of data bytes that FOLLOW a given cmd byte.
     *
     * @post returns the # of data bytes that FOLLOW a given cmd byte.
     *
     * @param cmd is an int, not a byte, for compatibility reasons; It is the cmd
     *        that marks the start for counting the number of bytes.
     *
     * @return value of -1 means packet needs special handling (i.e. passwd) or has
     *         uncertain length; too complicated for standard parser. <br>
     *         Otherwise returns the number of bytes following cmd
     */
    int             packet_size(int cmd);

    //------------------------------------------------------------------------
    //Helper functions
    //------------------------------------------------------------------------

    /**
     * reads a null-terminated string from the serial port
     * and stores it in memory pointed to by str.
     *
     * @pre The Ibox is connected.
     * @post A null-terminated string is read into str.
     *
     * @param str A pointer to the character string where the data is to be stored.
     *
     * @return TIMED_OUT if the string could not be read from the port. <br>
     *         SUCCESS if the read was successful.
     */
    vpr::ReturnStatus read_string(char *str);
    
	/**
     * invalidate_fields() sets all hci _updated fields to zero.
     * @post invalidate_fields() sets all hci _updated fields to zero.
     */
    void            invalidate_fields();

    /**
     * Returns the state of the button designated by pos.
     *
     * @post returns the state of the button designated by pos.
     *
     * @param pos the button in which to query.
     *
     * @return the state of the button.
     */
    int             buttonFunc(int pos);

    /**
     * Returns the state of the analog device desiganted by pos.
     *
     * @post returns the state of the analog device designated by pos.
     *
     * @param pos the position in the analog array in which to query.
     *
     * @return the state of the analog device.
     */
    int             analogFunc(int pos);

    //--------------------------------------------------------------------
    // Error handling
    //-------------------------------------------------------------------

	char* getString(byte cmnd);

	void setPort(const std::string& port_name)
	{
		mPortName=port_name;
	}
	const std::string& getPort()
	{
		return(mPortName);
	}
	void setBaud(long int baud)
	{
		mBaudRate = baud;
	}
	long int getBaud()
	{
		return(mBaudRate);
	}

private:
/* Serial Port declaration */
    vpr::SerialPort  *port;
	//Creat setPort
	//char	    	_port[256];
    std::string      mPortName;            /* name of serial port */
    int              mBaudRate;            /* baud rate of ibox */
    ////////////////////////////////////////////////////////////////
	int overlap;     /* keeps track of how many reads and writes are
                      * pending in overlapped opperation.
                      * However this feature isn't currently supported
                      * by the serial port interface so it is not used */
    packet_rec      mPacket;     /* The current packet */
    int             mPackets_expected; /* Determines whether timeout is important */


/* Descriptor strings:
         *   These strings provide information about a particular HCI system.
         */
    char    mSerial_number [MAX_STRING_SIZE];
    char    mProduct_name [MAX_STRING_SIZE];
    char    mProduct_id [MAX_STRING_SIZE];
    char    mModel_name [MAX_STRING_SIZE];
    char    mComment [MAX_STRING_SIZE];
    char    mParam_format [MAX_STRING_SIZE];
    char    mVersion [MAX_STRING_SIZE];

/* Normalization values for primary quantities:
         *   These values give some reference or normalization quantity
         *     for each field.
         *   A zero in any of these fields means there is no hardware
         *     support for that data in this particular system.
         */
    int     mButton_supported [NUM_BUTTONS]; /* zero = button not supported */
    int     mMax_timer;                      /* Max count reached before wrapping */
    int     mMax_analog [NUM_ANALOGS];       /* Full-scale A/D reading */
    int     mMax_encoder [NUM_ENCODERS];     /* Max value each encoder reaches
                                             * INCLUDING quadrature */
/* Marker field lets you mark different segments of data in incoming
         *   buffer.  hci_insert_marker() makes HCI insert a marker into the
         *   data stream, so you can switch modes in a host application
         *   without misinterpreting some data that may still be waiting
         *   in the buffer; just insert a marker, and don't switch modes
         *   until you see the marker come back.
         */
    int             mMarker;
    int             mMarker_updated;

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

/* Home position references:
         *   In many cases some calibration procedure will be required to ensure
         *   that the encoder positions truly match the assumed home position.
         *   This array can store any data that is useful for that purpose.
         */
    int    mHome_ref [NUM_ENCODERS];

/* Primary quantities: */
    int     mButtons;                /* button bits all together */
    int     mButton [NUM_BUTTONS];   /* ON/OFF flags for buttons */
    long    mTimer;                  /* Running counter */
    int     mAnalog [NUM_ANALOGS];   /* A/D channels */
    int     mEncoder [NUM_ENCODERS]; /* Encoder counts */

/* Status of primary fields:
         *   A zero in any of these fields indicates that the corresponding
         *     primary quantity is out of date (wasn't updated by the
         *     previous packets)
         *   Note: buttons are updated with every packet
         */
    int     mTimer_updated;
    int     mAnalog_updated [NUM_ANALOGS];
    int     mEncoder_updated [NUM_ENCODERS];

/* Helper functions */
	vpr::ReturnStatus 	autosynch();
    int     			begin();
    int     			end();


	vpr::ReturnStatus getInfo();
	unsigned char getCmdByte(int t, int anlg, int encd);


public:
/* constants */
    byte     mCfg_args[MAX_CFG_SIZE];
    int      mNum_cfg_args; /* # of values stored in cfg_args[] */

};

#endif /* _GADGET_IBOX_STANDALONE_H_ */
