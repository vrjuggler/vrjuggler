
/***************************************************
 * - - - - - - -   IMMERSION CORP.   - - - - - - - *
 *                                                 *
 *               UNIX software series              *
 *                Copyright (c) 1993               *
 ***************************************************
 * DRIVEUNX.H -- Immersion Corp. Developer's Programming Libraries
 *              Functions for driving UNIX machines
 * Rev 2.0
 */


/* If you have customized hardware and need to modify the UNIX-specific
 *    routines, use this header file as a specification for a new
 *    UNIX-specific module.
 */

#ifndef _DRIVEUNIX_H
#define _DRIVEUNIX_H

#include <vjConfig.h>

/* Public constants */
#define NUM_PORTS 4
#define MIN_TIMEOUT 0.1



/* Timing functions */
void    host_pause(float delay_sec);
float   host_get_timeout(int port);
void    host_set_timeout(int port, float timeout_sec);
void    host_start_timeout(int port);
int     host_timed_out(int port);


/* Configuring serial ports */
void    host_fix_baud(long int *baud);
int     host_open_serial(int port, long int baud);
void    host_close_serial(int port);
void    host_flush_serial(int port);


/* Reading/writing serial data */
int     host_read_char(int port);
int     host_write_char(int port, int ch);
int     host_write_string(int port, char *str);
int     host_port_valid(int port);
int     host_input_count(int port);

#endif
