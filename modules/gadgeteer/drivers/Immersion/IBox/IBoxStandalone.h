/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * Juggler is (C) Copyright 1998, 1999, 2000 by Iowa State University
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

// -----------------------------------------------------------
//
// Class structure for ibox functionality
//
// Author:
//      Josh Brown
//
// Note:
//      In case you get to wondering what HCI stands for, I dont' know
//      for sure but I think it is left over from an old version of the
//      driver and I believe it has to do with Imersion Corp's name for
//      the ibox.
//
// -----------------------------------------------------------

#include <vjConfig.h>
#include <VPR/IO/Port/vjSerialPort.h>
#include <VPR/IO/Port/vjPort.h>
#include <VPR/IO/Port/vjSerialPortFactory.h>
#include <VPR/IO/Port/vjPortImp.h>
#include </home/users/browner/vpr/vapor/vpr/IO/Port/SerialPort.h>

/*-----------*/
/* Constants */
/*-----------*/

#define NUM_ENCODERS    6       /* Max # encoders supported by Immersion HCI */
#define NUM_ANALOGS     8       /* Max # of A/D channels supported */
#define NUM_BUTTONS     7       /* Max # of buttons supported */



/* These numbers are the maximum numbers available in Immersion HCI hardware.
 *   Various Immersion Corp. products based on the Immersion HCI will
 *   support various subsets of these components.
 */

/*------------*/
/* Data Types */
/*------------*/

/* Use i2box_result as if it were an enumerated type.  Compare variables of
 *   this type to the string constants declared below.  An i2box_result is
 *   actually a string, so it can be directly printed as well.
 */

typedef char*   i2box_result;

/* Result Codes for which there are error handlers
 */
char    TIMED_OUT[29] = "Timed out waiting for packet";
char    BAD_PORT_NUM[25] = "Port number out of range";
char    BAD_PACKET[17] = "Corrupted packet";
char    NO_HCI[19] = "Unable to find HCI";
char    CANT_BEGIN[34] = "Found HCI but can't begin session";
char    CANT_OPEN_PORT[27] = "Unable to open serial port";
char    BAD_PASSWORD[40] = "Password rejected during config command";
char    BAD_VERSION[47] = "Firmware version does not support this feature";
char    BAD_FORMAT[34] = "Unknown firmware parameter format";
extern char 	SUCCESS[];  /* Successful operation */
extern char     NO_PACKET_YET[];        /* Complete packet not yet recv'd */

/* Shorthand for a byte */
typedef unsigned char   byte;

/* # of bytes in longest possible packet, plus some extra room */
#define MAX_PACKET_SIZE         40

/* Max # of args to a special config command */
#define MAX_CFG_SIZE            40

/* Max length of descriptor string from HCI */
#define MAX_STRING_SIZE         32

/* Code sent by HCI to accept a password */
#define PASSWD_OK       0xFF

/* Space for arguments to config commands */
extern byte     cfg_args[MAX_CFG_SIZE];
extern int      num_cfg_args; /* # of values stored in cfg_args[] */

/* Record for packet
 */
typedef struct
{
        int     parsed;         /* Flag tells whether this packet has been parsed */
        int     error;          /* Flag tells whether there has been com error */
        int     num_bytes_needed;
        byte    cmd_byte;
        byte    data[MAX_PACKET_SIZE];
        byte    *data_ptr;
} packet_rec;

/* Macro for creating a command byte from a timer flag and the # of analog *  and encoder reports desired
 */
#define CMD_BYTE(t, anlg, encd) \
        (   (t ? TIMER_BIT : 0)                         \
                | (anlg > 4 ? ANALOG_BITS :             \
                    (anlg > 2 ? ANALOG_HI_BIT :         \
                    (anlg ? ANALOG_LO_BIT : 0)))        \
                | (encd > 3 ? ENCODER_BITS :            \
                    (encd > 2 ? ENCODER_HI_BIT :        \
                    (encd ? ENCODER_LO_BIT : 0)))   )

/*---------------------*/
/* Low-level Constants */
/*---------------------*/

/* Command bit-field place values */
#define PACKET_MARKER   0x80
#define CONFIG_BIT      0x40
#define TIMER_BIT       0x20
#define FUTURE_BIT      0x10
#define ANALOG_BITS     0x0C
#define ANALOG_HI_BIT   0x08
#define ANALOG_LO_BIT   0x04
#define ENCODER_BITS    0x03
#define ENCODER_HI_BIT  0x02
#define ENCODER_LO_BIT  0x01

/* Labels for the Special Configuration Commands */
#define CONFIG_MIN      0xC0    /* Minimum cmd byte for a config cmd */
#define GET_PARAMS      0xC0
#define GET_HOME_REF    0xC1
#define HOME_POS        0xC2
#define SET_HOME        0xC3
#define SET_BAUD        0xC4
#define END_SESSION     0xC5
#define GET_MAXES       0xC6
#define SET_PARAMS      0xC7
#define GET_PROD_NAME   0xC8
#define GET_PROD_ID     0xC9
#define GET_MODEL_NAME  0xCA
#define GET_SERNUM      0xCB
#define GET_COMMENT     0xCC
#define GET_PRM_FORMAT  0xCD
#define GET_VERSION     0xCE
#define REPORT_MOTION   0xCF
#define SET_HOME_REF    0xD0
#define RESTORE_FACTORY 0xD1
#define INSERT_MARKER   0xD2


class i2box{
public:

/* constructor */
        i2box();

/* destructor */
        ~i2box();


/* Setup & configuration of Immersion Interface Box */
        i2box_result    connect(long int baud);
        i2box_result    fancy_connect(long int baud
                        , void (*installer_fun)());

        i2box_result    wait_update(int timer_flag, int num_analogs
                        ,int num_encoders); 

/* Issuing commands to HCI */
        void            std_cmd(int timer_flag, int analog_reports,
                                 int encoder_reports);
        void            simple_cfg_cmd(byte cmnd);
        i2box_result    string_cmd(byte cmnd);
        i2box_result    passwd_cmd(byte cmnd);
        void            insert_marker(byte marker);
        i2box_result    get_params(byte *block, int *block_size);
        i2box_result    set_params(byte *block, int block_size);
        i2box_result    get_home_ref();
        i2box_result    set_home_ref(int *homeref);
        i2box_result    go_home_pos();
        i2box_result    set_home_pos(int *homepos);
        i2box_result    get_maxes();
        i2box_result    factory_settings();
        void            report_motion(int timer_flag,
                        int analog_reports, int encoder_reports, int delay,
                        byte active_btns, int *analog_deltas, int *encoder_deltas);
        void            end_motion();
	i2box_result    wait_packet();
        i2box_result    check_packet();
        i2box_result    check_motion();
        int             build_packet();

/* Packet parsing functions */
        i2box_result    parse_packet();
        i2box_result    parse_cfg_packet();
        int             packet_size(int cmd);

/* Helper functions */
        i2box_result    read_string(char *str);
        i2box_result    read_block(byte *block, int *num_bytes);
        void            invalidate_fields();

/* Error handling */
        i2box_result    error(i2box_result condition);



private:
/* Serial Port declaration */
        vpr::SerialPort 	*port;
	float 		slow_timeout; 	/* slow timeout setting */
	float 		fast_timeout;	/* fast timeout setting */
	char*		name;  	/* name of serial port */
	int 		rate;	/* baud rate of ibox */
	int		overlap; 	/* keeps track of how many reads and writes are
					 * pending in overlapped opperation.
					 * However this feature isn't currently supported
					 * by the serial port interface so it is not used */
	packet_rec      packet;		/* The current packet */
        int             packets_expected; /* Determines whether timeout is important */	
	

/* Descriptor strings:
         *   These strings provide information about a particular HCI system.
         */
        char    serial_number [MAX_STRING_SIZE];
        char    product_name [MAX_STRING_SIZE];
        char    product_id [MAX_STRING_SIZE];
        char    model_name [MAX_STRING_SIZE];
        char    comment [MAX_STRING_SIZE];
        char    param_format [MAX_STRING_SIZE];
        char    version [MAX_STRING_SIZE];

/* Normalization values for primary quantities:
         *   These values give some reference or normalization quantity
         *     for each field.
         *   A zero in any of these fields means there is no hardware
         *     support for that data in this particular system.
         */
        int     button_supported [NUM_BUTTONS]; /* zero = button not supported */
        int     max_timer;                      /* Max count reached before wrapping */
        int     max_analog [NUM_ANALOGS];       /* Full-scale A/D reading */
        int     max_encoder [NUM_ENCODERS];     /* Max value each encoder reaches
                                         * INCLUDING quadrature */
/* Marker field lets you mark different segments of data in incoming
         *   buffer.  hci_insert_marker() makes HCI insert a marker into the
         *   data stream, so you can switch modes in a host application
         *   without misinterpreting some data that may still be waiting
         *   in the buffer; just insert a marker, and don't switch modes
         *   until you see the marker come back.
         */
        int             marker;
        int             marker_updated;

/* Encoder "home" position:
         *   The relative encoders supported by the Immersion HCI only report
         *   their NET angular motion from the time they are powered up.  If
         *   an encoder's initial angular value is important, it must somehow
         *   be assumed or calibrated at start-up.  These fields contain values
         *   to be assumed at start-up.  They can be read from or written to the
         *   Immersion HCI EEPROM.  If written to the Immersion HCI EEPROM,
         *   these "home" values will be retained even after power is turned off.
         */
        int     home_pos [NUM_ENCODERS];

/* Home position references:
         *   In many cases some calibration procedure will be required to ensure
         *   that the encoder positions truly match the assumed home position.
         *   This array can store any data that is useful for that purpose.
         */
         int    home_ref [NUM_ENCODERS];

/* Primary quantities: */
        int     buttons;                /* button bits all together */
        int     button [NUM_BUTTONS];   /* ON/OFF flags for buttons */
        long    timer;                  /* Running counter */
        int     analog [NUM_ANALOGS];   /* A/D channels */
        int     encoder [NUM_ENCODERS]; /* Encoder counts */

/* Status of primary fields:
         *   A zero in any of these fields indicates that the corresponding
         *     primary quantity is out of date (wasn't updated by the
         *     previous packets)
         *   Note: buttons are updated with every packet
         */
        int     timer_updated;
        int     analog_updated [NUM_ANALOGS];
        int     encoder_updated [NUM_ENCODERS];

/* Helper functions */
	int 		autosynch();
	int 		begin();
	int 		end();	


/* Handlers for errors */
        i2box_result      (*BAD_PORT_handler)();
        i2box_result      (*CANT_OPEN_handler)();
        i2box_result      (*NO_HCI_handler)();
        i2box_result      (*CANT_BEGIN_handler)();
        i2box_result      (*TIMED_OUT_handler)();
        i2box_result      (*BAD_PACKET_handler)();
        i2box_result      (*BAD_PASSWORD_handler)();
        i2box_result      (*BAD_VERSION_handler)();
        i2box_result      (*BAD_FORMAT_handler)();

/* Handler to use for an error if everything above is NULL
         * The simplest way to get diagnostic reporting is to
         * set default_handler to a function that outputs the string
         * that is passed as the 'condition'.  This can be implemented
         * under any operating system or window environment by including
         * the appropriate o.s. calls in the function pointed to by
         * this handler pointer.
         */
        i2box_result      (*default_handler)();

};




