/***************************************************
 * - - - - - - -   IMMERSION CORP.   - - - - - - - *
 *                                                 *
 *       Platform-independent software series      *
 *                Copyright (c) 1993               *
 ***************************************************
 * IBOX.H   |   Rev 2.0   |   March 1994
 *
 * Immersion Corp. Developer's Programming Library Module
 *  Definitions for abstraction of an Immersion Interface Box
 *        This abstract module is built upon the lower-level HCI module
 *      Requires HCI firmware version 2.0 or later
 */

#ifndef _IBOX_H
#define _IBOX_H

#include <vrj/vrjConfig.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Immersion Interface Box Front Panel Assignments:

   +------------------------------------------------+
   |  Immersion Interface Box (tm)                  |
   |                                                |
   |    O    O    O    O    o    o    o    o        |
   +------------------------------------------------+
        -Digital Ports--    --Analog Ports--
          B2   B1             X    Y
                  --------Joystick #1-------
    B2   B1             X    Y
    --------Joystick #2-------

    Port Numbering:
    3    2    1    0    3    2    1    0
        ----------------    ----------------
    Digital Buttons       Analog Signals

    Use this numbering for button[] and analog[] fields in an
        ibox_rec
 */


/* The Immersion Interface Box is simple enough to require very little
 *   abstraction beyond the HCI hardware module.  Only a few functions
 *   are defined for the ibox_rec structure; all other functionality is
 *   already provided by the HCI module.  See the last section of this
 *   file for a list of the higher-level HCI functions which are likely
 *   to be most convenient for Interface Box programming.
 *
 * The HCI module supports 7 buttons, 8 analog signals, and 6 encoders.
 *   Standard Interface Box hardware supports only 4 buttons, 4 analog
 *   signals, and up to 6 optional encoders.  With custom work, the
 *   Interface Box can support the full 7 buttons and 8 analog signals.
 */


/*------------*/
/* Data Types */
/*------------*/


/* Simple synonyms */
typedef hci_result  ibox_result;
typedef hci_rec     ibox_rec;


/* Declare one ibox_rec for each Interface Box in use.  This record holds
 *   ALL data concerning the Interface Box's communications parameters,
 *   basic signal values, error handling functions, etc.  See hci.h
 *   for the underlying hci_rec definition.
 */



/*---------------------*/
/* Function prototypes */
/*---------------------*/

/* Setup & configuration of Immersion Interface Box */
ibox_result ibox_connect(ibox_rec *ibox, int port, long int baud);
ibox_result ibox_fancy_connect(ibox_rec *ibox, int port, long int baud,
                void (*installer_fun)());

/* Requesting packets */
ibox_result ibox_wait_update(ibox_rec *ibox, int timer_flag, int num_analogs,
                int num_encoders);

/* Checking the HCI version */
float   ibox_version_num(ibox_rec *ibox);


/* Useful higher-level functions from hci.c:
 *
 **** Communication ****
 * void     hci_disconnect(hci_rec *hci);
 * void     hci_change_baud(hci_rec *hci, long int new_baud);
 *
 **** Requesting and checking for packets ****
 * void     hci_std_cmd(hci_rec *hci, int timer_flag, int analog_reports,
 *              int encoder_reports);
 * hci_result   hci_check_packet(hci_rec *hci);
 * hci_result   hci_check_motion(hci_rec *hci);
 *
 **** Configuration Commands ****
 * void     hci_insert_marker(hci_rec *hci, byte marker);
 * hci_result   hci_go_home_pos(hci_rec *hci);
 * hci_result   hci_set_home_pos(hci_rec *hci, int *homepos);
 * hci_result   hci_set_home_ref(hci_rec *hci, int *homeref);
 * hci_result   hci_factory_settings(hci_rec *hci);
 * void     hci_report_motion(hci_rec *hci, int timer_flag,
 *       int analog_reports, int encoder_reports, int delay,
 *       byte active_btns, int *analog_deltas, int *encoder_deltas);
 *
 **** Simple error reporting ****
 * hci_result   hci_simple_string(hci_rec *hci, hci_result condition);
 *
 */

#ifdef __cplusplus
}
#endif

#endif
