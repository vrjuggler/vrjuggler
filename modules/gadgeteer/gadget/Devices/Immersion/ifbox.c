/***************************************************
 * - - - - - - -   IMMERSION CORP.   - - - - - - - *
 *                                                 *
 *       Platform-independent software series      *
 *                Copyright (c) 1993               *
 ***************************************************
 * IBOX.C   |   Rev 2.0   |   March 1994
 *
 * Immersion Corp. Developer's Programming Library Module
 *  Functions for abstraction of an Immersion Interface Box
 *        This abstract module is built upon the lower-level HCI module
 *      Requires HCI firmware version 2.0 or later
 */

#include <vrj/vrjConfig.h>
#include <stdio.h>
#include <gad/Devices/Immersion/hci.h>
#include <gad/Devices/Immersion/iboxStandalone.h>
#include <gad/Devices/Immersion/driveunx.h>



/*-----------------------------*/
/* Setting up an Interface Box */
/*-----------------------------*/


/* ibox_connect() initializes an ibox_rec and establishes communication
 *   with its corresponding Interface Box.  Note hci_init() sets all
 *   error handlers to NULL.  To install handlers before connecting,
 *   use ibox_fancy_connect().
 */
ibox_result ibox_connect(ibox_rec *ibox, int port, long int baud)
{
    ibox_result result;

    hci_init(ibox, port, baud);
    result = hci_connect(ibox);
    if (result == SUCCESS) result = hci_get_strings(ibox);
    if (result == SUCCESS) result = hci_get_maxes(ibox);
    if (result == SUCCESS) result = hci_get_home_ref(ibox);

    return result;
}


/* ibox_fancy_connect() initializes an ibox_rec and establishes communication
 *     with its corresponding Interface Box.
 *   installer_fun is a function pointer that points to a user-defined
 *     function which will install error handlers in an ibox_rec.  See hci.h
 *     for a listing of error handler fields.
 *   Use this definition for installer_fun:
 *     void installer_fun(ibox_rec *ibox)
 */
ibox_result ibox_fancy_connect(ibox_rec *ibox, int port, long int baud,
                void (*installer_fun)())
{
    ibox_result result;

    hci_init(ibox, port, baud);
        (*installer_fun)(ibox);
    result = hci_connect(ibox);
    if (result == SUCCESS) hci_get_strings(ibox);

    return result;
}



/*--------------------------*/
/* Checking the HCI Version */
/*--------------------------*/


/* ibox_version_num() returns the firmware version number of an ibox_rec
 *   that has been connected.  Use this to check for compatibility.
 */
float ibox_version_num(ibox_rec *ibox)
{
    float vers;

    sscanf(ibox->version, "HCI %f", &vers);

    return vers;
}



/*----------------------*/
/* Updating Data Fields */
/*----------------------*/


/* ibox_wait_update() updates ibox data.  Waits for packet before going on.
 *   timer_flag = whether or not to report the timer (0 or non-zero).
 *   num_analogs = # of analog fields to report (0, 2, 4, or 8)
 *   num_encoders = # of encoder fields to report (0, 2, 3, or 6)
 */
ibox_result ibox_wait_update(ibox_rec *ibox, int timer_flag, int num_analogs,
                int num_encoders)
{
    hci_std_cmd(ibox, timer_flag, num_analogs, num_encoders);
    return hci_wait_packet(ibox);
}


/* To get an asynchronous update, use hci_std_cmd() directly, and check
 *   for packets yourself periodically with hci_check_packet().
 */


