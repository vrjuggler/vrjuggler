#include <vjConfig.h>
#include <stdio.h>
#include <Input/ibox/hci.h>
#include <Input/ibox/ibox.h>

ibox_rec ibox;

void openIbox()
{
	int port=2, timer_flag=0, num_anlg=0, num_encd=0, done=0, count=0;
	long int baud = 38400L;
	ibox_result result;

	result = ibox_connect(&ibox, port, baud);

	if (result == SUCCESS)
	{
		printf("Connected to Interface Box.\n");
	}
	hci_std_cmd(&ibox, timer_flag, num_anlg, num_encd);
}

void updateIbox()
{
	int port=2, timer_flag=0, num_anlg=0, num_encd=0, count=0;
	ibox_result result;
	result = hci_check_packet(&ibox);

	if (result == NO_PACKET_YET)
	{
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
	    count++;
	}
	else if (result == SUCCESS)
	{

	    /* Replace these printf's with your processing.
	     * Anywhere in this loop, the 'ibox' variable's
	     * fields can be used for whatever you need.
	     */
	    printf("\nBtns: %2X%2X%2X%2X count is %d ",
		    ibox.button[3], ibox.button[2],
		    ibox.button[1], ibox.button[0], count);

	    hci_std_cmd(&ibox, timer_flag, num_anlg, num_encd);
	}    
}

void closeIbox()
{
	hci_disconnect(&ibox);
}

