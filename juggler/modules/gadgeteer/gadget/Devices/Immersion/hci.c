/***************************************************
 * - - - - - - -   IMMERSION CORP.   - - - - - - - *
 *                                                 *
 *       Platform-independent software series      *
 *                Copyright (c) 1993               *
 ***************************************************
 * HCI.C   |   Rev 2.0   |   March 1994
 *
 * Immersion Corp. Developer's Programming Library Module
 * 	Functions for direct communication with the Immersion HCI
 *      Requires HCI firmware version 2.0 or later
 */

#include <vjConfig.h>
#include <stdio.h>
#include <Input/ibox/hci.h>
#include <Input/ibox/driveunx.h>


/* HCI handles all direct communication with the Immersion HCI box.
 * It can support one HCI per serial port.
 * For most purposes, you should not need to call HCI functions
 *    directly.  Use the higher-level modules available for convenient
 *    access to specific Immersion Corp. products based on the HCI.
 *
 * There are three ways to issue commands using this HCI module:
 *    1) Send a command and wait idly for the response.
 *             Use hci_wait_packet() after issuing command.
 *    2) Send a command, do a "background" task, and then check periodically
 *          for a response.
 *             Use hci_check_packet() after issuing the command.
 *    3) Issue one command that puts the HCI in "motion-reporting" mode.
 *          The Immersion HCI will then generate a response packet whenever
 *          sufficient motion is detected on any of its signal lines.
 *          You will need to periodically check for incoming packets.
 *             Use hci_check_motion() after issuing the command.
 *    In message-driven environments, you can install one of the check_packet
 *         or parse_packet functions as a message handler (or have a message
 *         handler call them).  This is the most effective way to use motion
 *         reporting, but it is not supported by this module due to the wide
 *         variety of messaging implementations across various platforms.
 */



/*-----------*/
/* Variables */
/*-----------*/

/* Argument list for Special Configuration Commands
 */
byte	cfg_args[MAX_CFG_SIZE];
int	num_cfg_args;	/* The # of valid bytes stored in cfg_args[] */

/* Strings for establishing connection */
char	SIGNON_STR[5] = "IMMC";
char	BEGIN_STR[6] = "BEGIN";

/* Result Codes for which there are no error handlers */
char	SUCCESS[8] = "Success";
char	NO_PACKET_YET[32] = "Current packet not yet complete";
char	TRY__AGAIN[29] = "Try this HCI operation again";
/* TRY__AGAIN[] is provided for use by modules built on hci.c.  This
 *   hci module does not directly use TRY__AGAIN[].  The idea is to
 *   have error handlers return TRY__AGAIN if an error has been fixed
 *   (such as getting a new port # from user).  Then higher-level modules
 *   should respond to TRY__AGAIN by repeating whatever they were trying
 *   to do when the error occured.
 */

/* Result Codes for which there are error handlers
 */
char	TIMED_OUT[29] = "Timed out waiting for packet";
char	BAD_PORT_NUM[25] = "Port number out of range";
char	BAD_PACKET[17] = "Corrupted packet";
char	NO_HCI[19] = "Unable to find HCI";
char	CANT_BEGIN[34] = "Found HCI but can't begin session";
char	CANT_OPEN_PORT[27] = "Unable to open serial port";
char	BAD_PASSWORD[40] = "Password rejected during config command";
char	BAD_VERSION[47] = "Firmware version does not support this feature";
char	BAD_FORMAT[34] = "Unknown firmware parameter format";



/*-------------------*/
/* Setting up an HCI */
/*-------------------*/


/* hci_clear_packet() resets packet-parsing variables
 */
void hci_clear_packet(hci_rec *hci)
{
	hci->packet.num_bytes_needed = 0;
	hci->packet.cmd_byte = 0;
	hci->packet.parsed = 1;
	hci->packet.error = 0;
	hci->packet.data_ptr = hci->packet.data;
	hci->packets_expected = 0;
}


/* hci_com_params() sets up an Immersion HCI's communications parameters
 */
void hci_com_params(hci_rec *hci, int port, long int baud)
{
	hci->port_num = port;

	host_fix_baud(&baud);
	hci->baud_rate = baud;

	hci->slow_timeout = 3.0;	/* 3 seconds */
	hci->fast_timeout = TIMEOUT_CHARS * 8.0 / (float) baud;
	if (hci->fast_timeout < MIN_TIMEOUT) hci->fast_timeout = MIN_TIMEOUT;
        hci_clear_packet(hci);
}


/* hci_init() initializes variables in an hci_rec
 */
void hci_init(hci_rec *hci, int port, long int baud)
{
	hci_com_params(hci, port, baud);
	hci_clear_packet(hci);

        /* Set all descr. strings to null strings */
	hci->serial_number[0] = 0;
	hci->product_name[0] = 0;
	hci->product_id[0] = 0;
	hci->model_name[0] = 0;
	hci->comment[0] = 0;
	hci->param_format[0] = 0;
	hci->version[0] = 0;

	/* By default, no error handlers are installed */
	hci->BAD_PORT_handler = NULL;
	hci->CANT_OPEN_handler = NULL;
	hci->NO_HCI_handler = NULL;
	hci->CANT_BEGIN_handler = NULL;
	hci->TIMED_OUT_handler = NULL;
	hci->BAD_PACKET_handler = NULL;
	hci->BAD_VERSION_handler = NULL;
	hci->BAD_FORMAT_handler = NULL;

	hci->default_handler = NULL;

        /* This field is free for the user's own purpose */
	hci->user_data = (long int) 0;
}


/* hci_fast_timeout() sets the timeout period for a fast process.
 *   Subsequent host_start_timeout() calls will result in a short
 *     timeout period.  Used for typical "standard" packet reception.
 */
void hci_fast_timeout(hci_rec *hci)
{
	host_set_timeout(hci->port_num, hci->fast_timeout);
}


/* hci_slow_timeout() sets the timeout period for a slow process
 *   Subsequent host_start_timeout() calls will result in a long
 *     timeout period.  Used for special (or config) packet reception.
 */
void hci_slow_timeout(hci_rec *hci)
{
	host_set_timeout(hci->port_num, hci->slow_timeout);
}


/* hci_connect() connects to an Immersion HCI by opening its serial port
 *    and running the 'autosynch' and 'begin' sequences.
 */
hci_result hci_connect(hci_rec *hci)
{
	hci_result	result;
	int	port = hci->port_num;

	if ( host_port_valid(port) )
	{
		/* Open the port */
		if (host_open_serial(port, hci->baud_rate))
		{
			/* Get ready for slow process */
			hci_slow_timeout(hci);

			/* Then synch to the HCI */
			result = hci_autosynch(hci);
			if (result == SUCCESS)
			{
				/* If it worked, ready to BEGIN session */
				result = hci_begin(hci);
			}
		}
		else result = CANT_OPEN_PORT;
	}
	else
	{
		result = BAD_PORT_NUM;
	}

	return hci_error(hci, result);
}


/* hci_autosynch() leads the Immersion HCI through the baudrate
 *    auto-synch process.  This requires that the HCI was either
 *    just powered-on or has just been given an END_SESSION command,
 *    such as by a previous call to hci_end().
 */
hci_result hci_autosynch(hci_rec *hci)
{
	int ch, port = hci->port_num;
	char *sign_ch = SIGNON_STR;
	int  signed_on = 0;

	host_start_timeout(port);
	while ( !signed_on && !host_timed_out(port) )
	{
		hci_end(hci);	/* In case session wasn't ended before */
		host_write_string(port, SIGNON_STR);
                host_pause(SIGNON_PAUSE);
		while (((ch=host_read_char(port)) != -1) && !signed_on)
		{
			if (ch == *sign_ch)
			{
				if (!*++sign_ch)
				{
					signed_on = 1;
				}
			}
			else
			{
				sign_ch = SIGNON_STR;
			}
		}
	}
	host_flush_serial(port);	/* Get rid of excess SIGNON strings in buffer */

	if (signed_on) return SUCCESS;
	else return NO_HCI;
}


/* hci_begin() opens the Immersion HCI for input after the signon process.
 *   This step is necessary to allow the host to clear its output buffer
 *   of signon strings before the HCI begins parsing commands.
 */
hci_result hci_begin(hci_rec *hci)
{
	int port = hci->port_num;

	host_write_string(port, BEGIN_STR);
	if (hci_read_string(hci, hci->product_id) == SUCCESS)
		return SUCCESS;
	else
		return CANT_BEGIN;
}


/* hci_end() issues an END_SESSION command.
 *   The next time the host runs hci_autosynch(), the HCI will
 *   respond without having to be turned off & on.
 */
void hci_end(hci_rec *hci)
{
	host_write_char(hci->port_num, END_SESSION);
}


/* hci_disconnect() ends the session and closes the serial port.
 *   The next time the host runs hci_connect(), the HCI will
 *   respond without having to be turned off & on.
 */
void hci_disconnect(hci_rec *hci)
{
	host_flush_serial(hci->port_num);
	hci_end(hci);
        host_pause(END_PAUSE);
	host_close_serial(hci->port_num);
}


/* hci_reset_com() clears the host serial i/o buffers.
 *   This often helps to recover from a BAD_PACKET error and may be useful
 *     in a user-defined BAD_PACKET_handler.
 */
void hci_reset_com(hci_rec *hci)
{
	host_pause(5e-2);	/* Wait 50 ms for HCI to settle */
	host_flush_serial(hci->port_num);
	hci_clear_packet(hci);
}


/* hci_change_baud() changes the HCI's baud rate and changes the host's
 *   baud rate to match.  ANY PENDING SERIAL DATA IS LOST.
 */
void hci_change_baud(hci_rec *hci, long int new_baud)
{
	byte baud_code;

	host_fix_baud(&new_baud);
	baud_code = baud_to_code(new_baud);
	new_baud = code_to_baud(baud_code);
	host_write_char(hci->port_num, SET_BAUD);
	host_write_char(hci->port_num, baud_code);
	host_pause(END_PAUSE);
	host_close_serial(hci->port_num);
	host_open_serial(hci->port_num, new_baud);
}


/* hci_get_strings() gets all descriptor strings from the HCI and puts
 *    them in its hci_rec.
 */
hci_result hci_get_strings(hci_rec *hci)
{
	hci_result result;

	result = hci_string_cmd(hci, GET_PROD_NAME);
	if (result == SUCCESS) result = hci_string_cmd(hci, GET_PROD_ID);
	if (result == SUCCESS) result = hci_string_cmd(hci, GET_MODEL_NAME);
	if (result == SUCCESS) result = hci_string_cmd(hci, GET_SERNUM);
	if (result == SUCCESS) result = hci_string_cmd(hci, GET_COMMENT);
	if (result == SUCCESS) result = hci_string_cmd(hci, GET_PRM_FORMAT);
	if (result == SUCCESS) result = hci_string_cmd(hci, GET_VERSION);

	return result;
}



/*-------------------*/
/* Issuing commands */
/*------------------*/


/* hci_std_cmd() sends any standard (non-config) cmd and immediately exits.
 *    The host will have to check periodically on the status of the
 *    response packet.  This can be done synchronously using hci_wait_packet()
 *    or asynchronously using hci_check_packet()
 */
void hci_std_cmd(hci_rec *hci, int timer_flag, int analog_reports,
		int encoder_reports)
{
	byte	cmnd = CMD_BYTE(timer_flag, analog_reports, encoder_reports);

	host_write_char(hci->port_num, cmnd);

        /* If there are no pending packets, start timing this one */
	if (! hci->packets_expected++)
	{
		hci_fast_timeout(hci);
		host_start_timeout(hci->port_num);
        }
}


/* hci_simple_cfg_cmd() sends the cmd byte of any 'simple' config cmd
 *    and immediately exits.  Simple cfg cmds do not require passwords,
 *    do not concern communications parameters, and have a fixed-length
 *    response: GET_HOME_REF, HOME_POS, GET_MAXES, and INSERT_MARKER.
 *    They are simple enough to be interpreted later by hci_parse_packet().
 */
void hci_simple_cfg_cmd(hci_rec *hci, byte cmnd)
{
	host_write_char(hci->port_num, cmnd);

        /* If there are no pending packets, start timing this one */
	if (! hci->packets_expected++)
	{
		hci_fast_timeout(hci);
		host_start_timeout(hci->port_num);
        }
}


/* hci_get_params() gets main parameter block from HCI, stores it in
 *   block supplied by main application.
 */
hci_result hci_get_params(hci_rec *hci, byte *block, int *block_size)
{
	int port = hci->port_num, ch;

	host_write_char(port, GET_PARAMS);
	hci_fast_timeout(hci);
	host_start_timeout(port);
	while ( (ch=host_read_char(port)) != GET_PARAMS)
	{
		if (ch != -1) host_start_timeout(port);
		if (host_timed_out(port)) break;
	}
	if (ch != GET_PARAMS) return hci_error(hci, TIMED_OUT);

	*block_size = -1;	/* Means take 1st byte as the block length */

	return hci_read_block(hci, block, block_size);
}


/* hci_set_params() changes main parameter block on HCI.  Takes values
 *   stored in block and writes them to HCI's EEPROM.
 */
hci_result hci_set_params(hci_rec *hci, byte *block, int block_size)
{
	int i;

	num_cfg_args = block_size;
	for (i=0;i<block_size;i++) cfg_args[i] = block[i];

	return hci_passwd_cmd(hci, SET_PARAMS);
}


/* hci_get_home_ref() gets home reference offsets from HCI, waits for response
 */
hci_result hci_get_home_ref(hci_rec *hci)
{
	hci_simple_cfg_cmd(hci, GET_HOME_REF);
        return hci_wait_packet(hci);
}


/* hci_go_home_pos() sets HCI encoders to home position, waits for response
 */
hci_result hci_go_home_pos(hci_rec *hci)
{
	hci_simple_cfg_cmd(hci, HOME_POS);
	return hci_wait_packet(hci);
}


/* hci_get_maxes() asks HCI for max field values, waits for response
 */
hci_result hci_get_maxes(hci_rec *hci)
{
	hci_simple_cfg_cmd(hci, GET_MAXES);
	return hci_wait_packet(hci);
}


/* hci_insert_marker() inserts a place marker packet into the HCI stream.
 *   DOES NOT WAIT for response (that would defeat the marker's purpose).
 */
void hci_insert_marker(hci_rec *hci, byte marker)
{
	hci_simple_cfg_cmd(hci, INSERT_MARKER);
	host_write_char(hci->port_num, marker);
}


/* hci_string_cmd() handles commands that request an info string
 */
hci_result hci_string_cmd(hci_rec *hci, byte cmnd)
{
	hci_result result;
        int ch, port = hci->port_num;

	host_write_char(port, cmnd);
	hci_fast_timeout(hci);
	host_start_timeout(port);
	while ( (ch=host_read_char(port)) != cmnd)
	{
		if (ch != -1) host_start_timeout(port);
		if (host_timed_out(port)) break;
	}
	if (ch != cmnd) return hci_error(hci, TIMED_OUT);

	switch (cmnd)
	{
		case GET_PROD_NAME:
			result = hci_read_string(hci, hci->product_name);
			break;
		case GET_PROD_ID:
			result = hci_read_string(hci, hci->product_id);
			break;
		case GET_MODEL_NAME:
			result = hci_read_string(hci, hci->model_name);
			break;
		case GET_SERNUM:
			result = hci_read_string(hci, hci->serial_number);
			break;
		case GET_COMMENT:
			result = hci_read_string(hci, hci->comment);
			break;
		case GET_PRM_FORMAT:
			result = hci_read_string(hci, hci->param_format);
			break;
		case GET_VERSION:
			result = hci_read_string(hci, hci->version);
			break;
		default:
			result = SUCCESS;
			break;
	}

	return result;
}


/* hci_passwd_cmd() handles a cmd that requires a password.
 *    Assumes any nec arguments are in cfg_args[].
 *    Throws out any chars currently still in input buffer.
 */
hci_result hci_passwd_cmd(hci_rec *hci, byte cmnd)
{
	int ch, port = hci->port_num, i;

	host_write_char(port, cmnd);
        hci_fast_timeout(hci);
        host_start_timeout(port);
	while( (ch=host_read_char(port)) != cmnd)
	{
		if (ch != -1) host_start_timeout(port);
		if (host_timed_out(port)) break;
	}
	if (ch == cmnd)
	{
		host_write_string(port, hci->serial_number);
		host_write_char(port, 0x00);	/* Write the null to terminate */
	        host_start_timeout(port);
		while( (ch=host_read_char(port)) == -1)
			if (host_timed_out(port)) break;
		if (ch == -1) return hci_error(hci, TIMED_OUT);
		else if (ch == PASSWD_OK)
		{
			for(i=0; i<num_cfg_args; i++)
				host_write_char(hci->port_num, cfg_args[i]);
			for(i=0; i<num_cfg_args; i++)
				host_pause(CFG_ARG_PAUSE);
			return SUCCESS;
		}
                else return BAD_PASSWORD;
	}
        else return hci_error(hci, TIMED_OUT);
}


/* hci_set_home_pos() defines a new home position for the HCI encoders
 */
hci_result hci_set_home_pos(hci_rec *hci, int *homepos)
{
	num_cfg_args = 2*NUM_ENCODERS;
	cfg_args[0] = homepos[0] >> 8;
	cfg_args[1] = homepos[0] & 0x00FF;
	cfg_args[2] = homepos[1] >> 8;
	cfg_args[3] = homepos[1] & 0x00FF;
	cfg_args[4] = homepos[2] >> 8;
	cfg_args[5] = homepos[2] & 0x00FF;
	cfg_args[6] = homepos[3] >> 8;
	cfg_args[7] = homepos[3] & 0x00FF;
	cfg_args[8] = homepos[4] >> 8;
	cfg_args[9] = homepos[4] & 0x00FF;
	cfg_args[10] = homepos[5] >> 8;
	cfg_args[11] = homepos[5] & 0x00FF;
	return hci_passwd_cmd(hci, SET_HOME);
}


/* hci_set_home_ref() defines a new set of home references for the HCI encoders
 */
hci_result hci_set_home_ref(hci_rec *hci, int *homeref)
{
	num_cfg_args = 2*NUM_ENCODERS;
	cfg_args[0] = homeref[0] >> 8;
	cfg_args[1] = homeref[0] & 0x00FF;
	cfg_args[2] = homeref[1] >> 8;
	cfg_args[3] = homeref[1] & 0x00FF;
	cfg_args[4] = homeref[2] >> 8;
	cfg_args[5] = homeref[2] & 0x00FF;
	cfg_args[6] = homeref[3] >> 8;
	cfg_args[7] = homeref[3] & 0x00FF;
	cfg_args[8] = homeref[4] >> 8;
	cfg_args[9] = homeref[4] & 0x00FF;
	cfg_args[10] = homeref[5] >> 8;
	cfg_args[11] = homeref[5] & 0x00FF;
	return hci_passwd_cmd(hci, SET_HOME_REF);
}


/* hci_factory_settings() restores all factory settings
 */
hci_result hci_factory_settings(hci_rec *hci)
{
	hci_result result;

	num_cfg_args = 0;
	result = hci_passwd_cmd(hci, RESTORE_FACTORY);
	host_pause(RESTORE_PAUSE);

	return result;
}



/*---------------------------------*/
/* Issuing Motion-sensing commands */
/*---------------------------------*/


/* hci_report_motion() sends a motion-sensitive cmd and immediately exits.
 *    timer_flag, analog_reports, and encoder_reports are as in hci_std_cmd()
 *    delay is minimum delay between packets, in (approx) msec
 *    active_btns is a bit mask indicating which buttons generate packets
 *      when clicked.
 *    analog_deltas and encoder_deltas are min. change required, in each
 *      field, to generate a packet.
 *    The host will have to check periodically for packets, unless
 *    message-driven functions are installed.
 */
void hci_report_motion(hci_rec *hci, int timer_flag, int analog_reports,
		int encoder_reports, int delay, byte active_btns,
		int *analog_deltas, int *encoder_deltas)
{
	byte	cmnd = CMD_BYTE(timer_flag, analog_reports, encoder_reports);
	int	port = hci->port_num, i;

	host_write_char(port, REPORT_MOTION);
	host_write_char(port, delay >> 8);
	host_write_char(port, delay & 0x00FF);
	host_write_char(port, cmnd);
	host_write_char(port, active_btns);
	for (i=0; i<NUM_ANALOGS; i++)
		host_write_char(port, analog_deltas[i]);
	for (i=0; i<NUM_ENCODERS; i++)
	{
		host_write_char(port, encoder_deltas[i] >> 8);
		host_write_char(port, encoder_deltas[i] & 0x00FF);
	}
}


/* hci_end_motion() cancels motion-reporting mode and clears all unparsed data.
 *   To cancel motion-reporting without clearing data, use hci_insert_marker().
 */
void hci_end_motion(hci_rec *hci)
{
	int port = hci->port_num;

	host_write_char(port, 0);
	host_pause(5e-2);
        host_flush_serial(port);
}



/*------------------------*/
/* Compatibility Checking */
/*------------------------*/


/* hci_version_num() returns the firmware version number of an hci_rec
 *   that has been connected.  Use this to check for compatibility.
 */
float hci_version_num(hci_rec *hci)
{
	float vers = 0.0;

	sscanf(hci->version, "HCI %f", &vers);

	return vers;
}



/*-------------------*/
/* Packet Monitoring */
/*-------------------*/


/* hci_wait_packet() waits for the given port to receive a complete packet.
 *   Times out if it takes too long.
 */
hci_result hci_wait_packet(hci_rec *hci)
{
	hci_result result;

	hci_fast_timeout(hci);
	host_start_timeout(hci->port_num);

	/* Watch the port until something happens */
	while ( (result = hci_check_packet(hci)) == NO_PACKET_YET);

	return result;
}


/* hci_check_packet() checks for a complete packet and parses it if it's ready.
 *   Returns TIMED_OUT if packet is not complete and it's been too long since
 *     since the last timeout was started.
 */
hci_result hci_check_packet(hci_rec *hci)
{
	hci_result result;

	if (hci_build_packet(hci))
	{
		result = hci_parse_packet(hci);
		if (result == SUCCESS) return result;
		else return hci_error(hci, result);
	}
	else
	{
		if (host_timed_out(hci->port_num))
			return hci_error(hci, TIMED_OUT);
		else
			return NO_PACKET_YET;
	}
}


/* hci_check_motion() checks for a complete packet and parses it if it's ready.
 *   But TIMED_OUT is not considered an error; we assume we're in motion-
 *    reporting mode, and packets may be few & far between.
 */
hci_result hci_check_motion(hci_rec *hci)
{
	hci_result result;

	if (hci_build_packet(hci))
	{
		result = hci_parse_packet(hci);
		if (result == SUCCESS) return result;
		else return hci_error(hci, result);
	}
	else return NO_PACKET_YET;
}


/* hci_build_packet() reads chars from serial buffer into the packet array.
 *   Returns false if a valid packet is not yet complete
 *   Returns true when packet-building stops due to completion or an error.
 *   Sets packet.error flag if the cmd_byte is not a valid cmd byte.
 *   Sets num_bytes_expected to -1 if the cmd is not one that the standard
 *     parser (hci_parse_packet()) can deal with.
 */
int hci_build_packet(hci_rec *hci)
{
	static int	ch;
	static int	port;

	port = hci->port_num;
	if (hci->packet.parsed)
	{
		if ((ch = host_read_char(port)) == -1) return 0;
		else
		{
			hci->packet.cmd_byte = (byte) ch;
			if (ch < 0x80)
			{
                                hci->packet.error = 1;
				return 1;
			}
			else
			{
				hci->packet.parsed = 0;
                                hci->packet.error = 0;
				hci->packet.data_ptr = hci->packet.data;
				hci->packet.num_bytes_needed = hci_packet_size(ch);
				hci->packets_expected--;
				hci_fast_timeout(hci);
				host_start_timeout(port);
			}
		}
        }
	else if (hci->packet.num_bytes_needed > 0)
	{
		while ((ch = host_read_char(port)) != -1)
		{
			*(hci->packet.data_ptr)++ = ch;
			if (--hci->packet.num_bytes_needed == 0) break;
		}
	}

	return (hci->packet.num_bytes_needed <= 0);
}


/* packet_size() returns the # of data bytes that FOLLOW a given cmd byte
 *   The cmd arg is an int, not a byte, for compatibility with host_read_char()
 *   Return val of -1 means packet needs special handling (i.e. passwd)
 *   or has uncertain length; too complicated for standard parser.
 */
int hci_packet_size(int cmd)
{
	int size = 1;	/* Regular cmds always include buttons byte */
        int bits;

	if (cmd < CONFIG_MIN)
	{
		if (cmd & TIMER_BIT) size += 2;
		bits = cmd & ANALOG_BITS;
		if (bits == ANALOG_LO_BIT) size += 2 + 1;
		else if (bits == ANALOG_HI_BIT) size += 4 + 1;
		else if (bits == ANALOG_BITS) size += 8 + 1;
		bits = cmd & ENCODER_BITS;
		if (bits == ENCODER_LO_BIT) size += 2*2;
		else if (bits == ENCODER_HI_BIT) size += 2*3;
		else if (bits == ENCODER_BITS) size += 2*6;
	}
	else switch (cmd)
	{
		case GET_HOME_REF:
			size = 12;
			break;
		case SET_BAUD:
		case INSERT_MARKER:
			size = 1;
			break;
		case GET_MAXES:
			size = 24;
			break;
		case HOME_POS:
		case END_SESSION:
		case REPORT_MOTION:
			size = 0;
			break;
		case GET_PARAMS:
		case GET_PROD_NAME:
		case GET_PROD_ID:
		case GET_MODEL_NAME:
		case GET_SERNUM:
		case GET_COMMENT:
		case GET_PRM_FORMAT:
		case GET_VERSION:
		case SET_PARAMS:
		case SET_HOME:
		case SET_HOME_REF:
		case RESTORE_FACTORY:
			size = -1;
                        break;
	}

        return size;
}


/* hci_invalidate_fields() sets all hci _updated fields to zero
 */
void hci_invalidate_fields(hci_rec *hci)
{
	hci->timer_updated = 0;
	hci->analog_updated[0] = 0;
	hci->analog_updated[1] = 0;
	hci->analog_updated[2] = 0;
	hci->analog_updated[3] = 0;
	hci->analog_updated[4] = 0;
	hci->analog_updated[5] = 0;
	hci->analog_updated[6] = 0;
	hci->analog_updated[7] = 0;
	hci->encoder_updated[0] = 0;
	hci->encoder_updated[1] = 0;
	hci->encoder_updated[2] = 0;
	hci->encoder_updated[3] = 0;
	hci->encoder_updated[4] = 0;
	hci->encoder_updated[5] = 0;
        hci->marker_updated = 0;
}


/* hci_parse_packet() interprets the hci's packet and stores all HCI data
 *   in the HCI record.
 *   Also marks this hci's packet as having been parsed.
 *   Before calling this, call hci_build_packet() to see if packet is ready.
 */
hci_result hci_parse_packet(hci_rec *hci)
{
	int cmnd = hci->packet.cmd_byte, bits, temp;
	hci_result result = SUCCESS;
	byte *dp;
        int *p, *q;

	if (hci->packet.num_bytes_needed)
	{
		if (hci->packet.num_bytes_needed < 0)
			result = BAD_PACKET;
		else return NO_PACKET_YET;
        }

	if (hci->packet.error) result = hci_error(hci, BAD_PACKET);

	if (result == SUCCESS)
        {
		hci_invalidate_fields(hci);
		dp = hci->packet.data;
		if (cmnd < CONFIG_MIN)
		{
			bits = *dp++;
			hci->buttons = bits;
                        p = hci->button;
			*p++ = bits & 0x01;
			*p++ = bits & 0x02;
			*p++ = bits & 0x04;
			*p++ = bits & 0x08;
			*p++ = bits & 0x10;
			*p++ = bits & 0x20;
			*p++ = bits & 0x40;
			if (cmnd & TIMER_BIT)
			{
				temp = *dp++ << 7;
				temp += *dp++;
				hci->timer = temp;
				hci->timer_updated = 1;
			}
			bits = (cmnd & ANALOG_BITS) >> 2;
			if (bits--)
			{
				p = hci->analog;
                                q = hci->analog_updated;
				*p++ = *dp++ << 1;
				*p++ = *dp++ << 1;
				*q++ = 1, *q++ = 1;
				if (bits--)
				{
					*p++ = *dp++ << 1;
					*p++ = *dp++ << 1;
					*q++ = 1, *q++ = 1;
					if (bits--)
					{
						*p++ = *dp++ << 1;
						*p++ = *dp++ << 1;
						*p++ = *dp++ << 1;
						*p++ = *dp++ << 1;
						*q++ = 1, *q++ = 1;
						*q++ = 1, *q++ = 1;
					}
				}
				p = hci->analog;
				*p++ |= (*dp & 0x40 ? 1 : 0);
				*p++ |= (*dp & 0x20 ? 1 : 0);
				*p++ |= (*dp & 0x10 ? 1 : 0);
				*p++ |= (*dp & 0x08 ? 1 : 0);
				*p++ |= (*dp & 0x04 ? 1 : 0);
				*p++ |= (*dp & 0x02 ? 1 : 0);
				*p++ |= (*dp++ & 0x01 ? 1 : 0);
			}

			bits = cmnd & ENCODER_BITS;
			if (bits--)
			{
				p = hci->encoder;
                                q = hci->encoder_updated;
				*p = *dp++ << 7;
				*p++ += *dp++;
				*p = *dp++ << 7;
				*p++ += *dp++;
				*q++ = 1, *q++ = 1;
				if (bits--)
				{
					*p = *dp++ << 7;
					*p++ += *dp++;
					*q++ = 1;
					if (bits--)
					{
						*p = *dp++ << 7;
						*p++ += *dp++;
						*p = *dp++ << 7;
						*p++ += *dp++;
						*p = *dp++ << 7;
						*p++ += *dp++;
						*q++ = 1, *q++ = 1, *q++ = 1;
					}
                                }
			}
		}
		else result = hci_parse_cfg_packet(hci);
		hci->packet.parsed = 1;
	}

        return result;
}


/* hci_parse_cfg_packet() parses a packet for a special configuration command
 *   Assumes the packet is COMPLETE
 */
hci_result hci_parse_cfg_packet(hci_rec *hci)
{
	hci_result result = SUCCESS;
	byte *dp = hci->packet.data;
        int *p;

	switch(hci->packet.cmd_byte)
	{
		case GET_HOME_REF:
			p = hci->home_ref;
			*p = *dp++ << 8; *p++ += *dp++;
			*p = *dp++ << 8; *p++ += *dp++;
			*p = *dp++ << 8; *p++ += *dp++;
			*p = *dp++ << 8; *p++ += *dp++;
			*p = *dp++ << 8; *p++ += *dp++;
			*p = *dp++ << 8; *p++ += *dp++;
			break;
		case GET_MAXES:
			p = hci->button_supported;
			*p++ = *dp & 0x01; *p++ = *dp & 0x02;
			*p++ = *dp & 0x04; *p++ = *dp & 0x08;
			*p++ = *dp & 0x10; *p++ = *dp & 0x20;
			*p++ = *dp++ & 0x40;

			hci->max_timer = *dp++ << 8;
			hci->max_timer |= *dp++;

			p = hci->max_analog;
			*p++ = *dp++; *p++ = *dp++; *p++ = *dp++;
			*p++ = *dp++; *p++ = *dp++; *p++ = *dp++;
			*p++ = *dp++; *p++ = *dp++;
			p = hci->max_analog;
			*p++ |= (0x40 & *dp ? 0x01 : 0);
			*p++ |= (0x20 & *dp ? 0x01 : 0);
			*p++ |= (0x10 & *dp ? 0x01 : 0);
			*p++ |= (0x08 & *dp ? 0x01 : 0);
			*p++ |= (0x04 & *dp ? 0x01 : 0);
			*p++ |= (0x02 & *dp ? 0x01 : 0);
			*p++ |= (0x01 & *dp++ ? 0x01 : 0);

			p = hci->max_encoder;
			*p = *dp++ << 8; *p++ += *dp++;
			*p = *dp++ << 8; *p++ += *dp++;
			*p = *dp++ << 8; *p++ += *dp++;
			*p = *dp++ << 8; *p++ += *dp++;
			*p = *dp++ << 8; *p++ += *dp++;
			*p = *dp++ << 8; *p++ += *dp++;
			break;
		case INSERT_MARKER:
			hci->marker = *dp;
			hci->marker_updated = 1;
			break;
		case HOME_POS:	/* No action needed */
		case REPORT_MOTION:
			break;
		default:
			result = BAD_PACKET;
			break;
	}

	return result;
}


/* hci_read_string() reads a null-terminated string from the serial port
 *    and stores it in memory pointed to by str
 */
hci_result hci_read_string(hci_rec *hci, char *str)
{
	int port = hci->port_num, ch;

        host_start_timeout(port);
	while (!host_timed_out(port))
	{
		ch=host_read_char(port);
		if (ch != -1)
		{
			*str++ = (byte) ch;
			if (ch == 0) return SUCCESS;
		}
	}
	return TIMED_OUT;
}


/* hci_read_block() reads a block of binary data from the serial port
 *   and puts it in memory at 'block' location.
 *   num_bytes tells how many bytes to read, and it returns # bytes read.
 *   If num_bytes is negative, the 1st byte is interp'd as # bytes to follow.
 */
hci_result hci_read_block(hci_rec *hci, byte *block, int *num_bytes)
{
	int port = hci->port_num, ch, count;

	count = *num_bytes;
	*num_bytes = 0;
	hci_slow_timeout(hci);
	host_start_timeout(port);
	while (!host_timed_out(port))
	{
		if (count < 0) count = host_read_char(port);
		else
		{
			ch = host_read_char(port);
			if (ch != -1)
			{
				*block++ = (byte) ch;
				(*num_bytes)++;
				if (--count == 0) return SUCCESS;
			}
                }
	}

	return TIMED_OUT;
}



/*-----------------*/
/* Handling Errors */
/*-----------------*/


/* hci_simple_string() prints the condition string.  Install this as the
 *   default_handler for simple default error handling.  For graphical
 *   environments, replace this with something like simple_dialog() or
 *   warning_box() (not included in this module).
 */
hci_result hci_simple_string(hci_rec *hci, hci_result condition)
{
	printf("\n**HCI Error: %s\n", condition);

	return condition;
}


/* hci_error() handles HCI module errors by looking for error handler that
 *   corresponds to the condition.  Uses default_handler if it is NULL.
 *   If default_handler is NULL too, it simply returns the condition.
 */
hci_result hci_error(hci_rec *hci, hci_result condition)
{
	hci_result (*handler)();

	/* These two are not really errors */
	if (condition == SUCCESS) return SUCCESS;
	if (condition == NO_PACKET_YET) return NO_PACKET_YET;

	/* These errors have handler pointers */
	if (condition == BAD_PORT_NUM)
		handler = hci->BAD_PORT_handler;
	else if (condition == CANT_OPEN_PORT)
		handler = hci->CANT_OPEN_handler;
	else if (condition == NO_HCI)
		handler = hci->NO_HCI_handler;
	else if (condition == CANT_BEGIN)
		handler = hci->CANT_BEGIN_handler;
	else if (condition == TIMED_OUT)
		handler = hci->TIMED_OUT_handler;
	else if (condition == BAD_PACKET)
		handler = hci->BAD_PACKET_handler;
	else if (condition == BAD_PASSWORD)
		handler = hci->BAD_PASSWORD_handler;
	else if (condition == BAD_VERSION)
		handler = hci->BAD_VERSION_handler;
	else if (condition == BAD_FORMAT)
		handler = hci->BAD_FORMAT_handler;
	else handler = NULL;

	if (handler == NULL) handler = hci->default_handler;
	if (handler == NULL) return condition;

	return (*handler)(hci, condition);
}



/*----------------------------*/
/* Internal Utility Functions */
/*----------------------------*/


/* baud_to_code() converts a baud rate to a 68HC11 BAUD register code
 *   Assumes a 7.3728 MHz crystal
 */
byte baud_to_code(long int baud)
{
	byte code;

	switch(baud)
	{
		case 115200:
			code = 0x00;
			break;
		case 57600:
			code = 0x01;
			break;
		case 28800:
			code = 0x02;
			break;
		case 14400:
			code = 0x03;
			break;
		case 38400:
			code = 0x10;
			break;
		case 19200:
			code = 0x11;
			break;
		case 9600:
			code = 0x12;
			break;
		default:	/* When in doubt, use 9600 */
			code = 0x12;
			break;
	}

	return code;
}


/* code_to_baud() converts a 68HC11 BAUD register code to a baud rate
 *   Assumes a 7.3728 MHz crystal
 */
long int code_to_baud(byte code)
{
	long int baud;

	switch(code)
	{
		case 0x00:
			baud = 115200;
			break;
		case 0x01:
			baud = 57600;
			break;
		case 0x02:
			baud = 28800;
			break;
		case 0x03:
			baud = 14400;
			break;
		case 0x10:
			baud = 38400;
                        break;
		case 0x11:
			baud = 19200;
			break;
		case 0x12:
			baud = 9600;
                        break;
		default:	/* When in doubt, use 9600 */
			baud = 9600;
			break;
	}

	return baud;
}


/* hci_strcopy() copies one string to another.  This function is included
 *   just to avoid variations in linking to standard libraries
 */
void hci_strcopy(char *from, char *to)
{
	while(*to++ = *from++);
}


/* hci_strcmp() compares two strings, returns non-zero if they are identical.
 *   This function is included just to avoid variations in linking to string
 *   libraries
 */
int hci_strcmp(char *s1, char *s2)
{
	int match;

	while(*s1 && (match = (*s1++ == *s2++)) );
	return match;
}


