////////////////////////////////////////////////////////////////////////////
//
// File:     logiclass.C
//
// Contains: Driver for the Logitech 3D mouse. This driver is based on
//	    Logitech's "logidrvr.c" (from Jim Barnes), but does not have
//	    full functionality. In particular, only 6D mouse functions in
//	    Euler mode not supported (i.e,. no quaternions or 2D mouse
//	    modes).
//
// Author:   Terry Fong <terry@ptolemy.arc.nasa.gov>
// History:  27-Mar-92  original version
///////////////////////////////////////////////////////////////////////////////


#include <config.h>
#include <stdio.h>	 // for perror(3C) 
#include <sys/types.h>	 // for open(2) 
#include <sys/stat.h>	 // for open(2) 
#include <fcntl.h>	 // for open(2) 
#include <termios.h>	 // for tcsetattr(3T) 
#include <limits.h>	 // for sginap(2)
#include <unistd.h>	 // for close()
#include <iostream.h>	    // for buffered I/O 
#include <sys/time.h>

#include <Input/vjPosition/logiclass.h>	// classprototypes and data types 

// uncommenting the following will produce debug print statements */
//
//#define DEBUG

int vjThreeDMouse::StartSampling()
{
  if (myThreadID == 0) {
   int i;
   current = 0; valid = 1; progress = 2;
   OpenMouse(sPort);
   vjThreeDMouse* devicePtr = this;
   void SampleMouse(void*);

   myThreadID = vjThread::spawn(SampleMouse, (void *) devicePtr, 0);
   if ( myThreadID == NULL ) {
      return -1;
   } else {
      cout << "going " << endl;
      return 1;
   }
  } else {
      return -2; // already sampling
  }

}

void SampleMouse(void* pointer) {
 struct timeval tv;
  double start_time, stop_time;

    vjThreeDMouse* devPointer = (vjThreeDMouse*)pointer;
    for(;;) {
     gettimeofday(&tv,0);
     start_time = (double)tv.tv_sec+ (double)tv.tv_usec / 1000000.0;
 
    for(int i = 0; i < 60; i++)
	devPointer->Sample();
 
     gettimeofday(&tv,0);
     stop_time = (double)tv.tv_sec+ (double)tv.tv_usec / 1000000.0;
     cout << 1/((stop_time-start_time) / 60)
          << "  " << endl;



    }
}

int vjThreeDMouse::StopSampling()
{
  if (myThreadID != 0) {
    vjThread::kill(myThreadID,SIGKILL);
    
    myThreadID = 0;
//    sginap(1);
    cout << "stopping the vjThreeDMouse.." << endl;
   }
   return 1;
}

void vjThreeDMouse::UpdateData() {
  lock.acquire();
  int tmp = valid;
  valid = progress;
  progress = tmp;
  lock.release();
}

// XXX: Bad stuff
vjMatrix* vjThreeDMouse::GetPosData(int devNum)
{
  return (vjMatrix*)&theData[current];
}

void vjThreeDMouse::GetPosData(vjPOS_DATA* &data){
  data = &theData[current];
}


vjThreeDMouse::vjThreeDMouse(vjConfigChunk *c) : vjPosition(c)
    // PURPOSE: Constructor - Setup all vars
{
//   strncpy(sPort,"/dev/ttyd2",11);
//    baseVector.setValue(0, 0, 0);   // Setup base offest as origin
  baseVector[0] = baseVector[1] = baseVector[2] = 0;
}



/////////////////////////////////////////////////////////////
// Name: OpenMouse
//
// Open the mouse device and set the fd
////////////////////////////////////////////////////////////
int vjThreeDMouse::OpenMouse(char* portName)
{
    int fd;
    fd = vjThreeDMouse::LogitechOpen(portName);
    if (fd==-1)
	return -1;  // error
    else {
	mouseFD = fd;	// assign the fd
	return 0;
    }
}


///////////////////////////////////////////////////////////////////////////
// Connect the mouse by opening a serial port (19200 baud, 8 data, 1 stop,
// no parity). Reset the mouse once connected and verify diagnostics.
//
// Return: file descriptor to serial port or -1 if error opening port
//////////////////////////////////////////////////////////////////////////
int vjThreeDMouse::LogitechOpen (char* port_name)
{
  int fd;
  struct termios t;
  char data[DIAGNOSTIC_SIZE];	/* for diagnostics info */

  /* open a serial port, read/write */
  if ((fd = open (port_name, O_RDWR)) < 0) {
    perror (port_name);
    return (-1);
  }
  
    /// Can now safely set the mouseFD value
  mouseFD = fd; 
    
    
  /* disable all input mode processing */
  t.c_iflag = 0;

  /* disable all output mode processing */
  t.c_oflag = 0;

  /* hardware control flags: 19200 baud, 8 data bits, 1 stop bits,
     no parity, enable receiver */
  t.c_cflag = B19200 | CS8 | CSTOPB | CREAD;

  /* disable local control processing (canonical, control sigs, etc) */
  t.c_lflag = 0;

  /* set control characters for non-canonical reads */
  t.c_cc[VMIN] = 1;
  t.c_cc[VTIME]= 0;

  /* control port immediately (TCSANOW) */
  if (tcsetattr(mouseFD, TCSANOW, &t) < 0) {
    perror ("error controlling serial port");
    return (-1);
  }

  /* reset the mouse */
  vjThreeDMouse::ResetControlUnit ();

  /* do diagnostics */
  vjThreeDMouse::GetDiagnostics (data);

#ifdef DEBUG
  printf ("diag[0]: %2x=", data[0]);
  print_bin (data[0]);
  printf ("\n");
  printf ("diag[1]: %2x=", data[1]);
  print_bin (data[1]);
  printf ("\n");
#endif

  /* check diagnostic return */
  if ((data[0] != 0xbf) || (data[1] != 0x3f)) {
    fprintf (stderr, "Mouse diagnostics failed\n");
    return (-1);
  }
  return (fd);
}


//////////////////////////////////////////////////////////////////////////////
// Close the mouse by closing the serial port.
//
// Return 0 on success, -1 on failure.
///////////////////////////////////////////////////////////////////////////////
int vjThreeDMouse::CloseMouse()
{
  if (close (mouseFD) < 0) {
    perror ("error closing serial port");
    return (-1);
  }
  else
    return (0);
}


///////////////////////////////////////////////////////////////////////////////
// Command demand reporting
///////////////////////////////////////////////////////////////////////////////
void vjThreeDMouse::CuDemandReporting ()
{

#ifdef DEBUG
  printf ("demand reporting enabled\n");
#endif
  struct termios t;

  tcgetattr (mouseFD, &t);

  /* set control characters for non-canonical reads */
  t.c_cc[VMIN] = EULER_RECORD_SIZE;
  t.c_cc[VTIME]= 1;

  /* control port immediately (TCSANOW) */
  if (tcsetattr(mouseFD, TCSANOW, &t) < 0) {
    perror ("error controlling serial port");
  }

  write (mouseFD, "*D", 2);
}


///////////////////////////////////////////////////////////////////////////////
// Command control unit to Euler mode
//////////////////////////////////////////////////////////////////////////////
void vjThreeDMouse::CuEulerMode ()
{

#ifdef DEBUG
  printf ("euler data mode enabled\n");
#endif

  write (mouseFD, "*G", 2);
}


///////////////////////////////////////////////////////////////////////////////
// Command control unit to head tracker mode
///////////////////////////////////////////////////////////////////////////////
void vjThreeDMouse::CuHeadtrackerMode ()
{

#ifdef DEBUG
  printf ("headtracking mode enabled\n");
#endif

  write (mouseFD, "*H", 2);
}


/////////////////////////////////////////////////////////////////////////////
// Command control unit to mouse mode
/////////////////////////////////////////////////////////////////////////////
void vjThreeDMouse::CuMouseMode ()
{

#ifdef DEBUG
  printf ("mouse mode enabled\n");
#endif

  write (mouseFD, "*h", 2);
}


//////////////////////////////////////////////////////////////////////////////
// Command control unit to perform diagnostics
//////////////////////////////////////////////////////////////////////////////
void vjThreeDMouse::CuRequestDiagnostics ()
{

#ifdef DEBUG
  printf ("performing diagnostics\n");
#endif
  struct termios t;

  tcgetattr (mouseFD, &t);

  /* set control characters for non-canonical reads */
  t.c_cc[VMIN] = DIAGNOSTIC_SIZE;
  t.c_cc[VTIME]= 1;

  /* control port immediately (TCSANOW) */
  if (tcsetattr(mouseFD, TCSANOW, &t) < 0) {
    perror ("error controlling serial port");
  }

  write (mouseFD, "*\05", 2);
}



///////////////////////////////////////////////////////////////////////////////
// Command a reset
///////////////////////////////////////////////////////////////////////////////
void vjThreeDMouse::CuResetControlUnit ()
{

#ifdef DEBUG
  printf ("resetting control unit\n");
#endif

  write (mouseFD, "*R", 2);
}


///////////////////////////////////////////////////////////////////////////////
// Retrieve diagnostics report
///////////////////////////////////////////////////////////////////////////////
void vjThreeDMouse::GetDiagnostics ( char data[])
{
  vjThreeDMouse::CuRequestDiagnostics ();	/* command diagnostics */
  //sginap (100);			/* wait 1 second */
  sleep(1); 
  read (mouseFD, data, DIAGNOSTIC_SIZE);
}


///////////////////////////////////////////////////////////////////////////////
// Retrieve a single record. This routine will spin until a valid record
// (i.e., 16 bytes and bit 7, byte 0 is on) is received.
///////////////////////////////////////////////////////////////////////////////
int vjThreeDMouse::GetRecord ( vjPOS_DATA* data)
{
  int num_read;
  byte record[EULER_RECORD_SIZE];

  vjThreeDMouse::CuRequestReport ();
  num_read = read (mouseFD, record, EULER_RECORD_SIZE);

  /* if didn't get a complete record or if invalid record, then try
     to get a good one */
  while ((num_read < EULER_RECORD_SIZE) || !(record[0] & logitech_FLAGBIT)) { 

    #ifdef DEBUG
	printf ("get_record: only got %d bytes\n", num_read);
    #endif
    
    /* flush the buffer */
    ioctl (mouseFD, TCFLSH, 0);

    vjThreeDMouse::CuRequestReport ();
    num_read = read (mouseFD, record, EULER_RECORD_SIZE);
  }

#ifdef DEBUG
  printf ("%d bytes read...", num_read);
#endif

  vjThreeDMouse::EulerToAbsolute (record, data);

  return (0);
}


//////////////////////////////////////////////////////////////////////////////
// Set control unit into demand reporting, send reset command, and wait for 
// the reset.
//////////////////////////////////////////////////////////////////////////////
void vjThreeDMouse::ResetControlUnit ()
{
  vjThreeDMouse::CuDemandReporting ();	/* make sure control unit is processing */
  sginap ((long) 10);		/* wait 10 clock ticks = 100 ms */
  vjThreeDMouse::CuResetControlUnit ();	/* command a reset */
  sleep(1);
  //sginap ((long) 100);		/* wait 1 second */
}


void vjThreeDMouse::SetBaseOrigin()
    // PURPOSE: Sets the current mouse X,Y,Z position to be the base origin
{
    baseVector[0] = theData[current].pos[0];
    baseVector[1] = theData[current].pos[1];
    baseVector[2] = theData[current].pos[2];
    // Setup currrent offest as origin   
}



/////////////////////////////////////////////////////////////////////////
// convert from raw Euler data record to absolute data
////////////////////////////////////////////////////////////////////////
void vjThreeDMouse::EulerToAbsolute (byte record[], vjPOS_DATA* data)
{
  long ax, ay, az, arx, ary, arz;

 // data->buttons = (byte) record[0];

  ax = (record[1] & 0x40) ? 0xFFE00000 : 0;
  ax |= (long)(record[1] & 0x7f) << 14;
  ax |= (long)(record[2] & 0x7f) << 7;
  ax |= (record[3] & 0x7f);

  ay = (record[4] & 0x40) ? 0xFFE00000 : 0;
  ay |= (long)(record[4] & 0x7f) << 14;
  ay |= (long)(record[5] & 0x7f) << 7;
  ay |= (record[6] & 0x7f);
  
  az = (record[7] & 0x40) ? 0xFFE00000 : 0;
  az |= (long)(record[7] & 0x7f) << 14;
  az |= (long)(record[8] & 0x7f) << 7;
  az |= (record[9] & 0x7f);

  data->pos[0] = ((float) ax) / 1000.0;
  data->pos[1] = ((float) ay) / 1000.0;
  data->pos[2] = ((float) az) / 1000.0;

  arx  = (record[10] & 0x7f) << 7;
  arx += (record[11] & 0x7f);
  
  ary  = (record[12] & 0x7f) << 7;
  ary += (record[13] & 0x7f);
   
  arz  = (record[14] & 0x7f) << 7;
  arz += (record[15] & 0x7f);

  data->pos[0] = ((float) arx) / 40.0;
  data->pos[1] = ((float) ary) / 40.0;
  data->pos[2] = ((float) arz) / 40.0;
}


/////////////////////////////////////////////////////////////////////////
// print an 8-bit binary string
/////////////////////////////////////////////////////////////////////////
void vjThreeDMouse::PrintBin (char a)
{
  int i;

  for (i=7; i>=0; i--)
    printf ("%c", (a&(1<<i)) ? '1' : '0');
}
