
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
