/*
 *  File:	    $Name$
 *  Date modified:  $Date$
 *  Version:	    $Revision$
 *
 *
 *                                VR Juggler
 *                                    by
 *                              Allen Bierbaum
 *                             Christopher Just
 *                            Carolina Cruz-Neira
 *                               Albert Baker
 *
 *                         Copyright  - 1997,1998,1999
 *                Iowa State University Research Foundation, Inc.
 *                            All Rights Reserved
 */


//===============================================================
// vjPinchGlove
//
// Purpose:
//    software interface to Fakespace Pinchglove hardware
//
// Author:
//	Kevin Meinert
//
// NOTES:
//      The long function names greatly decrease the 
//       ambiguity of what the functions do ... sorry. :)
//
// Date: 1-23-99
//===============================================================


#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <math.h>
#include <termio.h>
#include <time.h>
#include <unistd.h> /* needed for this->_connectToHardware() and sginap(); */
#include <string.h> /* for strcpy(); */
#include <sys/types.h>
#include <sys/times.h>
#include <sys/param.h>
#include <sys/stat.h>
#include <iostream.h>

#include <Input/vjGlove/fsPinchGlove.h>

int pinchfd = -1;
const char* const fsPinchGlove::_openHandString = "00000.00000";

///////////////////////////////////////////////////////////
// Public Methods
///////////////////////////////////////////////////////////

// Constructor
fsPinchGlove::fsPinchGlove()
{
    // TODO: should all this be in shared memory????
    #define USCALLOCATE(var, num, type, type2) \
	var = (volatile type )new char[num * sizeof(type2)];

    USCALLOCATE( _gestureString, 1, fsPinchGlove::charString12 *, fsPinchGlove::charString12 )
    
    strcpy( (char*)_gestureString->data, fsPinchGlove::_openHandString );

    USCALLOCATE( _previousGestureString, 1, fsPinchGlove::charString12 *, fsPinchGlove::charString12 )

    strcpy( (char*)_previousGestureString->data, fsPinchGlove::_openHandString );
      
    #undef USCALLOCATE
}

// Connect to the pinch glove hardware
bool fsPinchGlove::connectToHardware(const char* const ttyPort)
{
    cout<<"[vrj] Connecting To Fakespace Hardware\n"<<flush;
    
    int result = _connectToHardware( ttyPort );    
    
    if (result == 1)
	cout<<"[vrj] Connected to pinch glove hardware on port "<<ttyPort<<"\n"<<flush;
    else
	cout<<"[vrj] connectToHardware(\""<<ttyPort<<"\") returned "<<(result == 1 ? "true":"false")<<"\n"<<flush;
	
    return result == 1 ? true:false;
}

// get the last sampled string, 
// NOTE: call ReSampleData to get most current pinch data.
void fsPinchGlove::getSampledString(char string[12])
{
    strncpy( string, (const char*)_gestureString->data, 11 );
}

// call ReSampleData to get most current pinch data. 
void fsPinchGlove::reSampleStringFromHardware()
{
    // make a copy into the previous string.
    strncpy( (char*)_previousGestureString->data, 
	    (const char*)_gestureString->data, 
	    11 );
	    
    // get the current string from the pinch hardware
    _getStringFromHardware( (char*) _gestureString->data);   
}



///////////////////////////////////////////////////////////
// Private methods
///////////////////////////////////////////////////////////

// send to hardware methods:

int fsPinchGlove::_connectToHardware(const char* const ttyPort)
{
    struct termio data;
    const int BUFFER_LEN = 100;
    char buf[BUFFER_LEN];
    int cnt;

    data.c_cflag = B9600 | CS8 | CREAD | CLOCAL;
    data.c_lflag = 0;
    data.c_iflag = 1;
    data.c_oflag = 0;
    data.c_cc[ VMIN ] = 1;

    if ((pinchfd = open(ttyPort, O_RDWR | O_NDELAY)) == -1) 
	return 0;
    
    if (ioctl(pinchfd, TCSETAF, &data) < 0) 
	return 0;

    sginap(15);

    if (ioctl(pinchfd, TCFLSH, 2) < 0) 
	return 0;

    sginap(15);

    /* Turn time stamps on */
    _sendCommandToHardware("T1", buf);
    if (buf[1] != '1') 
    {
	cout<<"[vrj] could not turn time stamps on, buf[1] != '1'.  (buf[1]=='"<<buf[1]<<"')\n";
	return 0;	
    }
    /* Version compatability */
    _sendCommandToHardware("V1", buf);
    if (buf[1] != '1') 
    {
	printf("[vrj] could not set to version 1 formatting\n");
	return 0;	
    }

    /* Get the configuration information and print it */
    printf("[vrj] Configuration:\n");
    
    cnt = _sendCommandToHardware("CP", buf); 
    buf[cnt-1] = 0; /* get rid of 0x8F */
    printf("\t%s\n",&buf[1]);
    
    cnt = _sendCommandToHardware("CL", buf); 
    buf[cnt-1] = 0; /* get rid of 0x8F */
    printf("\t%s\n",&buf[1]);
    
    cnt = _sendCommandToHardware("CR", buf);
    buf[cnt-1] = 0; /* get rid of 0x8F */
    printf("\t%s\n",&buf[1]);
    
    cnt = _sendCommandToHardware("CT", buf); 
    buf[cnt-1] = 0; /* get rid of 0x8F */
    printf("\t%s\n",&buf[1]);
    
    return 1;
}

int fsPinchGlove::_sendCommandToHardware(const char* const command, char *reply)
{
	char buf[100];
	static int first = 1;

	if (first) {
		first = 0;
		/* Make sure we flush the receive buffer on the glove end */
		write(pinchfd, "*", 1);
		sginap(45);
		if (!read(pinchfd, buf, 3)) {
			write(pinchfd, "*", 1);
			sginap(45);
			read(pinchfd, buf, 3);
		}
		sginap(25);
	}

/* Send the 2 byte command */
	write(pinchfd, &command[0], 1);
	if (ioctl(pinchfd, TCFLSH, 1)<0) return 0;
    sginap(15);
	write(pinchfd, &command[1], 1);
	if (ioctl(pinchfd, TCFLSH, 1)<0) return 0;
    sginap(45);
	
	return(_readRecordsFromHardware(100,reply));
}


// read from hardware methods:


int fsPinchGlove::_readRecordsFromHardware(const int& rec_max_len, char *records)
{
    int s;
    int numbytes = 0;
    char buf[2048];
    clock_t t1, t2;

    #define START_BYTE_DATA 0x80
    #define START_BYTE_DATA_TS 0x81
    #define START_BYTE_TEXT 0x82
    #define END_BYTE 0x8F

    records[0] = 0;
	
    while (s = read(pinchfd, buf, 1) ) 
    {
	if ( (buf[0] == START_BYTE_DATA) ||
	    (buf[0] == START_BYTE_DATA_TS) ||
	    (buf[0] == START_BYTE_TEXT)) 
	{
	    records[numbytes++] = buf[0];
	    t1 = clock();
	    t2 = clock();
	    if (t1 > t2) 
	    { 
		t1=t2; 
		t2 = clock(); 
	    } 
	
	    do {
		s = read(pinchfd, &records[numbytes], 1); 
		numbytes += s;
		if (t1 > t2) 
		{
		    t1=t2; 
		    t2 = clock();
		} 
	    } while ( (records[numbytes-s] != END_BYTE) && 
		    ((clock_t)t2 - (clock_t)t1 < CLOCKS_PER_SEC));
	}
	
	if (t1 > t2) 
	{ 
	    t1=t2; 
	    t2 = clock(); 
	}
    }
    			
	return numbytes;
}

void fsPinchGlove::_getStringFromHardware(char* gesture)
{
    const int BUFFER_LEN = 100;
    char rec[BUFFER_LEN];
    char data[BUFFER_LEN];
    //char temp[256];
    static char nges = '0';
    int i, num;
    int touch_count;
    
    
    num = _readRecordsFromHardware(BUFFER_LEN,rec);
	
    // if anything outputs from _readRecordsFromHardware(), then...
    if(num) 
    {
	strcpy(gesture, "00000.00000");		
	switch(rec[0]) 
	{
	case 0x80: 
	case 0x81:
	    if (rec[0] == 0x80) 
	    {
		for (i=1; i<num-2; ++i) 
		    data[i-1] = rec[i];
		touch_count = (num-2)/2;
	    } else {
		for (i=1; i<num-3; ++i) 
		    data[i-1] = rec[i];
		touch_count = (num-4)/2;
	    }
		    
			    
	    for (i=0; i<touch_count; ++i) 
	    {
		/* for debug onlu */
		/*printf(" %02X.%02X\n",data[2*i],data[2*i+1]); */
		switch (i)
		{
		    case 0: nges = '1'; break;
		    case 1: nges = '2'; break;
		    case 2: nges = '3'; break;
		    case 3: nges = '4'; break;
		    case 4: nges = '5'; break;
		    default: nges = '0';
		}
		
		/* left hand */
		switch (data[2*i]) 
		{ 
		  case 0x10 :  gesture[0] = nges; break;
		  case 0x01 :  gesture[4] = nges; break;
		  case 0x11 :  gesture[0] = nges; gesture[4] = nges; break;
		  case 0x02 :  gesture[3] = nges; break;
		  case 0x12 :  gesture[0] = nges; gesture[3] = nges; break;
		  case 0x03 :  gesture[3] = nges; gesture[4] = nges; break;
		  case 0x13 :  gesture[0] = nges; gesture[3] = nges; gesture[4] = nges; break;
		  case 0x04 :  gesture[2] = nges; break;
		  case 0x14 :  gesture[0] = nges; gesture[2] = nges; break;
		  case 0x05 :  gesture[2] = nges; gesture[4] = nges; break;
		  case 0x15 :  gesture[0] = nges; gesture[2] = nges; gesture[4] = nges; break;
		  case 0x06 :  gesture[2] = nges; gesture[3] = nges; break;
		  case 0x16 :  gesture[0] = nges; gesture[2] = nges; gesture[3] = nges; break;
		  case 0x07 :  gesture[2] = nges; gesture[3] = nges; gesture[4] = nges; break;
		  case 0x17 :  gesture[0] = nges; gesture[2] = nges; gesture[3] = nges; gesture[4] = nges; break;
		  case 0x08 :  gesture[1] = nges; break;
		  case 0x18 :  gesture[0] = nges; gesture[1] = nges; break;
		  case 0x09 :  gesture[1] = nges; gesture[4] = nges; break;
		  case 0x19 :  gesture[0] = nges; gesture[1] = nges; gesture[4] = nges; break;						  
		  case 0x0A :  gesture[1] = nges; gesture[3] = nges; break;
		  case 0x1A :  gesture[0] = nges; gesture[1] = nges; gesture[3] = nges; break;
		  case 0x0B :  gesture[1] = nges; gesture[3] = nges; gesture[4] = nges; break;
		  case 0x1B :  gesture[0] = nges; gesture[1] = nges; gesture[3] = nges; gesture[4] = nges; break;						  
		  case 0x0C :  gesture[1] = nges; gesture[2] = nges; break;
		  case 0x1C :  gesture[0] = nges; gesture[1] = nges; gesture[2] = nges; break;
		  case 0x0D :  gesture[1] = nges; gesture[2] = nges; gesture[4] = nges; break;
		  case 0x1D :  gesture[0] = nges; gesture[1] = nges; gesture[2] = nges; gesture[4] = nges; break;
		  case 0x0E :  gesture[1] = nges; gesture[2] = nges; gesture[3] = nges; break;
		  case 0x1E :  gesture[0] = nges; gesture[1] = nges; gesture[2] = nges; gesture[3] = nges; break;
		  case 0x0F :  gesture[1] = nges; gesture[2] = nges; gesture[3] = nges; gesture[4] = nges; break;
		  case 0x1F :  gesture[0] = nges; gesture[1] = nges; gesture[2] = nges; gesture[3] = nges; gesture[4] = nges; break;
		  default: break;
		}
		
		switch (data[2*i+1]) /* right hand */
		{ 
		  case 0x10 :  gesture[6] = nges; break;
		  case 0x01 :  gesture[10] = nges; break;
		  case 0x11 :  gesture[6] = nges; gesture[10] = nges; break;
		  case 0x02 :  gesture[9] = nges; break;
		  case 0x12 :  gesture[6] = nges; gesture[9] = nges; break;
		  case 0x03 :  gesture[9] = nges; gesture[10] = nges; break;
		  case 0x13 :  gesture[6] = nges; gesture[9] = nges; gesture[10] = nges; break;
		  case 0x04 :  gesture[8] = nges; break;
		  case 0x14 :  gesture[6] = nges; gesture[8] = nges; break;
		  case 0x05 :  gesture[8] = nges; gesture[10] = nges; break;
		  case 0x15 :  gesture[6] = nges; gesture[8] = nges; gesture[10] = nges; break;
		  case 0x06 :  gesture[8] = nges; gesture[9] = nges; break;
		  case 0x16 :  gesture[1] = nges; gesture[8] = nges; gesture[9] = nges; break;
		  case 0x07 :  gesture[8] = nges; gesture[9] = nges; gesture[10] = nges; break;
		  case 0x17 :  gesture[6] = nges; gesture[8] = nges; gesture[9] = nges; gesture[10] = nges; break;
		  case 0x08 :  gesture[7] = nges; break;
		  case 0x18 :  gesture[6] = nges; gesture[7] = nges; break;
		  case 0x09 :  gesture[7] = nges; gesture[10] = nges; break;
		  case 0x19 :  gesture[1] = nges; gesture[7] = nges; gesture[10] = nges; break;
		  case 0x0A :  gesture[7] = nges; gesture[9] = nges; break;
		  case 0x1A :  gesture[6] = nges; gesture[7] = nges; gesture[9] = nges; break;
		  case 0x0B :  gesture[7] = nges; gesture[9] = nges; gesture[10] = nges; break;
		  case 0x1B :  gesture[6] = nges; gesture[7] = nges; gesture[9] = nges; gesture[10] = nges; break;
		  case 0x0C :  gesture[7] = nges; gesture[8] = nges; break;
		  case 0x1C :  gesture[6] = nges; gesture[7] = nges; gesture[8] = nges; break;
		  case 0x0D :  gesture[7] = nges; gesture[8] = nges; gesture[10] = nges; break;
		  case 0x1D :  gesture[6] = nges; gesture[7] = nges; gesture[8] = nges; gesture[10] = nges; break;
		  case 0x0E :  gesture[7] = nges; gesture[8] = nges; gesture[9] = nges; break;
		  case 0x1E :  gesture[6] = nges; gesture[7] = nges; gesture[8] = nges; gesture[9] = nges; break;
		  case 0x0F :  gesture[7] = nges; gesture[8] = nges; gesture[9] = nges; gesture[10] = nges; break;
		  case 0x1F :  gesture[6] = nges; gesture[7] = nges; gesture[8] = nges; gesture[9] = nges; gesture[10] = nges; break;
		  default: break;
		}
	    }
	    break; /* case */
	case 0x82:	 
	    break;
	default:	 
	    break;
	} // switch ()
    } // if (num)
    /*return (gesture);*/
}
