#include <stdio.h>
#include <gad/Devices/Immersion/hci.h>
#include <gad/Devices/Immersion/iboxStandalone.h>

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
	    /* Background task is simply to count */
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

