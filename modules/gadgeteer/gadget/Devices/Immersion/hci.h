/***************************************************
 * - - - - - - -   IMMERSION CORP.   - - - - - - - *
 *                                                 *
 *       Platform-independent software series      *
 *                Copyright (c) 1993               *
 ***************************************************
 * HCI.H   |   Rev 2.0   |   March 1994
 *
 * Immersion Corp. Developer's Programming Library Module
 * 	Definitions and prototypes for direct communication with
 *        the Immersion HCI
 *      Requires HCI firmware version 2.0 or later
 */

#ifndef _HCI_H
#define _HCI_H

#include <vrj/vjConfig.h>

#ifdef __cplusplus
extern "C" {
#endif

/*-----------*/
/* Constants */
/*-----------*/

#define NUM_ENCODERS	6	/* Max # encoders supported by Immersion HCI */
#define NUM_ANALOGS	8	/* Max # of A/D channels supported */
#define NUM_BUTTONS	7	/* Max # of buttons supported */ 

/* These numbers are the maximum numbers available in Immersion HCI hardware.
 *   Various Immersion Corp. products based on the Immersion HCI will
 *   support various subsets of these components.
 */


/*------------*/
/* Data Types */
/*------------*/

/* Use hci_result as if it were an enumerated type.  Compare variables of
 *   this type to the string constants declared below.  An hci_result is
 *   actually a string, so it can be directly printed as well.
 */
typedef char*	hci_result;

/* Constants for hci_result variables */
extern char	BAD_PORT_NUM[];	/* Port number not valid */
extern char	CANT_OPEN_PORT[];	/* Can't open the port during start-up */
extern char	NO_HCI[];	/* No response from HCI during start-up */
extern char	CANT_BEGIN[];	/* No response to "BEGIN" at start of session */
extern char	NO_PACKET_YET[];	/* Complete packet not yet recv'd */
extern char	TIMED_OUT[];	/* Didn't get complete packet in time */
extern char	BAD_PACKET[];	/* Received packet that did not make sense */
extern char	BAD_PASSWORD[];	/* Cfg cmd requiring passwd was denied */
extern char	SUCCESS[];	/* Successful operation */
extern char	TRY__AGAIN[];	/* Try the operation again */
extern char	BAD_VERSION[];	/* Feature isn't supported by firmware */
extern char	BAD_FORMAT[];	/* Parameter block is in unreadable format */

/* Shorthand for a byte */
typedef unsigned char	byte;


/* # of bytes in longest possible packet, plus some extra room */
#define MAX_PACKET_SIZE		40

/* Max # of args to a special config command */
#define MAX_CFG_SIZE		40

/* Max length of descriptor string from HCI */
#define MAX_STRING_SIZE		32

/* Space for arguments to config commands */
extern byte	cfg_args[MAX_CFG_SIZE];
extern int	num_cfg_args; /* # of values stored in cfg_args[] */

/* Record for packet
 */
typedef struct
{
	int	parsed;		/* Flag tells whether this packet has been parsed */
	int	error;		/* Flag tells whether there has been com error */
	int	num_bytes_needed;
	byte	cmd_byte;
	byte	data[MAX_PACKET_SIZE];
	byte	*data_ptr;
} packet_rec;


/* Record containing all HCI data
 *   Declare one of these structs for each Immersion HCI in use.
 *   The hci_connect() command will establish communication with an Immersion
 *      HCI and set up an hci_rec corresponding to it.
 *   Example references: (assuming 'hci' is declared as an hci_rec)
 *      hci.button[2] - ON/OFF status of button #2
 *      hci.encoder[1] - total count for encoder #1
 *      hci.baud_rate - baud rate in use with this Immersion HCI
 */
typedef struct
{
	/* Communications parameters */
	int		port_num;	/* This HCI's serial port number */
	long int	baud_rate;	/* This Probe's baud rate */
	float		slow_timeout;	/* Timeout period for slow process */
	float		fast_timeout;	/* Timeout period for fast process */
	packet_rec	packet;		/* The current packet */
	int		packets_expected; /* Determines whether timeout is important */

	/* Marker field lets you mark different segments of data in incoming
	 *   buffer.  hci_insert_marker() makes HCI insert a marker into the
	 *   data stream, so you can switch modes in a host application
	 *   without misinterpreting some data that may still be waiting
	 *   in the buffer; just insert a marker, and don't switch modes
	 *   until you see the marker come back.
	 */
	int		marker;
	int		marker_updated;

	/* Primary quantities: */
	int	buttons;		/* button bits all together */
	int	button [NUM_BUTTONS];	/* ON/OFF flags for buttons */
	long	timer;			/* Running counter */
        int	analog [NUM_ANALOGS];	/* A/D channels */
	int	encoder [NUM_ENCODERS];	/* Encoder counts */

	/* Normalization values for primary quantities:
	 *   These values give some reference or normalization quantity
         *     for each field.
	 *   A zero in any of these fields means there is no hardware
         *     support for that data in this particular system.
         */
	int	button_supported [NUM_BUTTONS];	/* zero = button not supported */
	int	max_timer;			/* Max count reached before wrapping */
	int	max_analog [NUM_ANALOGS];	/* Full-scale A/D reading */
	int	max_encoder [NUM_ENCODERS];	/* Max value each encoder reaches
					 * INCLUDING quadrature */

	/* Status of primary fields:
	 *   A zero in any of these fields indicates that the corresponding
	 *     primary quantity is out of date (wasn't updated by the
	 *     previous packets)
         *   Note: buttons are updated with every packet
	 */
	int	timer_updated;
	int	analog_updated [NUM_ANALOGS];
        int	encoder_updated [NUM_ENCODERS];

	/* Encoder "home" position:
	 *   The relative encoders supported by the Immersion HCI only report
	 *   their NET angular motion from the time they are powered up.  If
	 *   an encoder's initial angular value is important, it must somehow
	 *   be assumed or calibrated at start-up.  These fields contain values
	 *   to be assumed at start-up.  They can be read from or written to the
	 *   Immersion HCI EEPROM.  If written to the Immersion HCI EEPROM,
         *   these "home" values will be retained even after power is turned off.
	 */
	int	home_pos [NUM_ENCODERS];

	/* Home position references:
	 *   In many cases some calibration procedure will be required to ensure
	 *   that the encoder positions truly match the assumed home position.
	 *   This array can store any data that is useful for that purpose.
	 */
         int	home_ref [NUM_ENCODERS];

	/* Descriptor strings:
	 *   These strings provide information about a particular HCI system.
	 */
	char	serial_number [MAX_STRING_SIZE];
	char	product_name [MAX_STRING_SIZE];
	char	product_id [MAX_STRING_SIZE];
	char	model_name [MAX_STRING_SIZE];
	char	comment [MAX_STRING_SIZE];
	char	param_format [MAX_STRING_SIZE];
	char	version [MAX_STRING_SIZE];

	/* Function pointers to handle application-specific functions
	 *   These pointers are initialized to NULL.
	 *   The user can make these point to handlers for each specific condition.
	 *   These handlers must be declared as follows:
	 *      hci_result   my_handler(hci_rec *hci, hci_result condition);
	 * See programmer's guide for more discussion.
	 */

	/* Handlers for errors */
	hci_result	(*BAD_PORT_handler)();
	hci_result	(*CANT_OPEN_handler)();
	hci_result	(*NO_HCI_handler)();
	hci_result	(*CANT_BEGIN_handler)();
	hci_result	(*TIMED_OUT_handler)();
	hci_result	(*BAD_PACKET_handler)();
	hci_result	(*BAD_PASSWORD_handler)();
	hci_result	(*BAD_VERSION_handler)();
	hci_result	(*BAD_FORMAT_handler)();

	/* Handler to use for an error if everything above is NULL
	 * The simplest way to get diagnostic reporting is to
	 * set default_handler to a function that outputs the string
	 * that is passed as the 'condition'.  This can be implemented
	 * under any operating system or window environment by including
	 * the appropriate o.s. calls in the function pointed to by
         * this handler pointer.
	 */
	hci_result	(*default_handler)();

	/* Extra field available for user's application-specific purpose */
        long int	user_data;
} hci_rec;



/* Macro for creating a command byte from a timer flag and the # of analog
 *  and encoder reports desired
 */
#define CMD_BYTE(t, anlg, encd)	\
	(   (t ? TIMER_BIT : 0)				\
		| (anlg > 4 ? ANALOG_BITS :		\
		    (anlg > 2 ? ANALOG_HI_BIT :		\
		    (anlg ? ANALOG_LO_BIT : 0)))	\
		| (encd > 3 ? ENCODER_BITS :		\
		    (encd > 2 ? ENCODER_HI_BIT :	\
		    (encd ? ENCODER_LO_BIT : 0)))   )


/*---------------------*/
/* Low-level Constants */
/*---------------------*/

/* Labels for the Special Configuration Commands */
#define CONFIG_MIN	0xC0	/* Minimum cmd byte for a config cmd */
#define	GET_PARAMS      0xC0
#define	GET_HOME_REF    0xC1
#define HOME_POS	0xC2
#define	SET_HOME	0xC3
#define	SET_BAUD	0xC4
#define	END_SESSION	0xC5
#define	GET_MAXES	0xC6
#define	SET_PARAMS	0xC7
#define	GET_PROD_NAME	0xC8
#define	GET_PROD_ID	0xC9
#define	GET_MODEL_NAME	0xCA
#define	GET_SERNUM	0xCB
#define	GET_COMMENT	0xCC
#define GET_PRM_FORMAT	0xCD
#define GET_VERSION	0xCE
#define REPORT_MOTION	0xCF
#define SET_HOME_REF	0xD0
#define RESTORE_FACTORY 0xD1
#define INSERT_MARKER	0xD2

/* Command bit-field place values */
#define PACKET_MARKER	0x80
#define CONFIG_BIT	0x40
#define TIMER_BIT	0x20
#define FUTURE_BIT	0x10
#define ANALOG_BITS	0x0C
#define ANALOG_HI_BIT	0x08
#define ANALOG_LO_BIT	0x04
#define ENCODER_BITS	0x03
#define ENCODER_HI_BIT	0x02
#define ENCODER_LO_BIT	0x01

/* A timeout period is the time to transmit this many chars */
#define TIMEOUT_CHARS	2*MAX_PACKET_SIZE

/* Time (sec) to wait between signon attempts in 'connect' process */
#define SIGNON_PAUSE	15e-3

/* Time (sec) to wait after ending session */
#define END_PAUSE	15e-3

/* Time (sec) to wait for each byte arg to a passwd-protected cfg cmd */
#define CFG_ARG_PAUSE	15e-3

/* Time (sec) to wait after restoring factory settings */
#define RESTORE_PAUSE	2.0

/* Code sent by HCI to accept a password */
#define PASSWD_OK	0xFF



/*---------------------*/
/* Function Prototypes */
/*---------------------*/

/* Initialize and setup an hci_rec */
void		hci_init(hci_rec *hci, int port, long int baud);
hci_result	hci_connect(hci_rec *hci);
void		hci_disconnect(hci_rec *hci);
hci_result	hci_get_strings(hci_rec *hci);
void		hci_change_baud(hci_rec *hci, long int new_baud);
void		hci_reset_com(hci_rec *hci);
hci_result	hci_autosynch(hci_rec *hci);
hci_result	hci_begin(hci_rec *hci);
void		hci_end(hci_rec *hci);
void		hci_clear_packet(hci_rec *hci);
void		hci_com_params(hci_rec *hci, int port, long int baud);
void		hci_fast_timeout(hci_rec *hci);
void		hci_slow_timeout(hci_rec *hci);

/* Issuing commands to HCI */
void		hci_std_cmd(hci_rec *hci, int timer_flag, int analog_reports,
				int encoder_reports);
void		hci_simple_cfg_cmd(hci_rec *hci, byte cmnd);
hci_result	hci_string_cmd(hci_rec *hci, byte cmnd);
hci_result	hci_passwd_cmd(hci_rec *hci, byte cmnd);
void		hci_insert_marker(hci_rec *hci, byte marker);
hci_result	hci_get_params(hci_rec *hci, byte *block, int *block_size);
hci_result	hci_set_params(hci_rec *hci, byte *block, int block_size);
hci_result	hci_get_home_ref(hci_rec *hci);
hci_result	hci_set_home_ref(hci_rec *hci, int *homeref);
hci_result	hci_go_home_pos(hci_rec *hci);
hci_result	hci_set_home_pos(hci_rec *hci, int *homepos);
hci_result	hci_get_maxes(hci_rec *hci);
hci_result	hci_factory_settings(hci_rec *hci);
void		hci_report_motion(hci_rec *hci, int timer_flag,
		int analog_reports, int encoder_reports, int delay,
		byte active_btns, int *analog_deltas, int *encoder_deltas);
void		hci_end_motion(hci_rec *hci);

/* Compatibility Checking */
float	hci_version_num(hci_rec *hci);

/* Packet monitoring functions */
hci_result	hci_wait_packet(hci_rec *hci);
hci_result	hci_check_packet(hci_rec *hci);
hci_result	hci_check_motion(hci_rec *hci);
int		hci_build_packet(hci_rec *hci);

/* Packet parsing functions */
hci_result	hci_parse_packet(hci_rec *hci);
hci_result	hci_parse_cfg_packet(hci_rec *hci);
int		hci_packet_size(int cmd);

/* Helper functions */
hci_result	hci_read_string(hci_rec *hci, char *str);
hci_result	hci_read_block(hci_rec *hci, byte *block, int *bytes_read);
void		hci_invalidate_fields(hci_rec *hci);
void		hci_strcopy(char *from, char *to);
int		hci_strcmp(char *s1, char *s2);

/* Error handling */
hci_result	hci_error(hci_rec *hci, hci_result condition);
hci_result	hci_simple_string(hci_rec *hci, hci_result condition);

/* Conversion between baud rates and 6811 BAUD register codes */
byte		baud_to_code(long int baud);
long int	code_to_baud(byte code);

#ifdef __cplusplus 
} 
#endif 

#endif

