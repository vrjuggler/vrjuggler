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


////////////////////////////////////////////////////////////////////////////
//
// File:     logiclass.cpp
//
// Contains: Driver for the Logitech 3D mouse. This driver is based on
//     Logitech's "logidrvr.c" (from Jim Barnes), but does not have
//     full functionality. In particular, only 6D mouse functions in
//     Euler mode not supported (i.e,. no quaternions or 2D mouse
//     modes).
//
// Author:   Terry Fong <terry@ptolemy.arc.nasa.gov>
// History:  27-Mar-92  original version
///////////////////////////////////////////////////////////////////////////////


#include <gadget/gadgetConfig.h>
#include <stdio.h>    // for perror(3C)
#include <sys/types.h>   // for open(2)
#include <sys/stat.h>    // for open(2)
#include <fcntl.h>    // for open(2)
#include <termios.h>  // for tcsetattr(3T)
#include <unistd.h>   // for close()
#include <sys/time.h>

#include <vpr/System.h>
#include <gadget/Devices/Logitech/logiclass.h>   /* classprototypes and data types */
#include <jccl/Config/ConfigChunk.h>

// uncommenting the following will produce debug print statements */
//
//#define DEBUG

int ThreeDMouse::startSampling()
{
  if (myThreadID == NULL) {
   //int i;
   current = 0; valid = 1; progress = 2;
   openMouse(sPort);
   ThreeDMouse* devicePtr = this;
   void sampleMouse(void*);

   myThread = new vpr::Thread(sampleMouse, (void *) devicePtr);
   if ( !myThread->valid() ) {
      return -1;
   } else {
      std::cout << "going " << std::endl;
      return 1;
   }
  } else {
      return -2; // already sampling
  }

}

void sampleMouse(void* pointer) {
 struct timeval tv;
  double start_time, stop_time;

    ThreeDMouse* devPointer = (ThreeDMouse*)pointer;
    for(;;) {
     gettimeofday(&tv,0);
     start_time = (double)tv.tv_sec+ (double)tv.tv_usec / 1000000.0;

    for(int i = 0; i < 60; i++)
   devPointer->sample();

     gettimeofday(&tv,0);
     stop_time = (double)tv.tv_sec+ (double)tv.tv_usec / 1000000.0;
     std::cout << 1/((stop_time-start_time) / 60) << "  " << std::endl;
    }
}

int ThreeDMouse::stopSampling()
{
  if (myThread != NULL) {
    myThread->kill();

    myThreadID = NULL;
//    sginap(1);
    std::cout << "stopping the ThreeDMouse.." << std::endl;
   }
   return 1;
}

void ThreeDMouse::updateData() {
  lock.acquire();
  int tmp = valid;
  valid = progress;
  progress = tmp;
  lock.release();
}

gadget::PositionData* ThreeDMouse::getPositionData (int dev) {
    if (this->isActive() == false) {
        return NULL;
    }
    else
        return &mData[current];
}


bool ThreeDMouse::config(jccl::ConfigChunkPtr c)
    // PURPOSE: Constructor - Setup all vars
{
//   strncpy(sPort,"/dev/ttyd2",11);
//    baseVector.setValue(0, 0, 0);   // Setup base offest as origin
   if(! (gadget::Input::config(c) && gadget::Position::config(c)))
      return false;

   baseVector[0] = baseVector[1] = baseVector[2] = 0;

   return true;
}



/////////////////////////////////////////////////////////////
// Name: OpenMouse
//
// Open the mouse device and set the fd
////////////////////////////////////////////////////////////
int ThreeDMouse::openMouse(char* portName)
{
    int fd;
    fd = ThreeDMouse::logitechOpen(portName);
    if (fd==-1)
   return -1;  // error
    else {
   mouseFD = fd;  // assign the fd
   return 0;
    }
}


///////////////////////////////////////////////////////////////////////////
// Connect the mouse by opening a serial port (19200 baud, 8 data, 1 stop,
// no parity). Reset the mouse once connected and verify diagnostics.
//
// Return: file descriptor to serial port or -1 if error opening port
//////////////////////////////////////////////////////////////////////////
int ThreeDMouse::logitechOpen (char* port_name)
{
  int fd;
  struct termios t;
  unsigned char data[DIAGNOSTIC_SIZE]; /* for diagnostics info */

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
  ThreeDMouse::resetControlUnit ();

  /* do diagnostics */
  ThreeDMouse::getDiagnostics (data);

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
int ThreeDMouse::closeMouse()
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
void ThreeDMouse::cuDemandReporting ()
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
void ThreeDMouse::cuEulerMode ()
{

#ifdef DEBUG
  printf ("euler data mode enabled\n");
#endif

  write (mouseFD, "*G", 2);
}


///////////////////////////////////////////////////////////////////////////////
// Command control unit to head tracker mode
///////////////////////////////////////////////////////////////////////////////
void ThreeDMouse::cuHeadtrackerMode ()
{

#ifdef DEBUG
  printf ("headtracking mode enabled\n");
#endif

  write (mouseFD, "*H", 2);
}


/////////////////////////////////////////////////////////////////////////////
// Command control unit to mouse mode
/////////////////////////////////////////////////////////////////////////////
void ThreeDMouse::cuMouseMode ()
{

#ifdef DEBUG
  printf ("mouse mode enabled\n");
#endif

  write (mouseFD, "*h", 2);
}


//////////////////////////////////////////////////////////////////////////////
// Command control unit to perform diagnostics
//////////////////////////////////////////////////////////////////////////////
void ThreeDMouse::cuRequestDiagnostics ()
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
void ThreeDMouse::cuResetControlUnit ()
{

#ifdef DEBUG
  printf ("resetting control unit\n");
#endif

  write (mouseFD, "*R", 2);
}


///////////////////////////////////////////////////////////////////////////////
// Retrieve diagnostics report
///////////////////////////////////////////////////////////////////////////////
void ThreeDMouse::getDiagnostics ( unsigned char data[])
{
  ThreeDMouse::cuRequestDiagnostics (); /* command diagnostics */
  vpr::System::sleep(1);
  read (mouseFD, data, DIAGNOSTIC_SIZE);
}


///////////////////////////////////////////////////////////////////////////////
// Retrieve a single record. This routine will spin until a valid record
// (i.e., 16 bytes and bit 7, byte 0 is on) is received.
///////////////////////////////////////////////////////////////////////////////
int ThreeDMouse::getRecord ( gadget::PositionData* data)
{
  int num_read;
  byte record[EULER_RECORD_SIZE];

  ThreeDMouse::cuRequestReport ();
  num_read = read (mouseFD, record, EULER_RECORD_SIZE);

  /* if didn't get a complete record or if invalid record, then try
     to get a good one */
  while ((num_read < EULER_RECORD_SIZE) || !(record[0] & logitech_FLAGBIT)) {

    #ifdef DEBUG
   printf ("get_record: only got %d bytes\n", num_read);
    #endif

    /* flush the input buffer */
    tcflush(mouseFD, TCIFLUSH);

    ThreeDMouse::cuRequestReport ();
    num_read = read (mouseFD, record, EULER_RECORD_SIZE);
  }

#ifdef DEBUG
  printf ("%d bytes read...", num_read);
#endif

  ThreeDMouse::eulerToAbsolute (record, data->getPositionData());

  return (0);
}


//////////////////////////////////////////////////////////////////////////////
// Set control unit into demand reporting, send reset command, and wait for
// the reset.
//////////////////////////////////////////////////////////////////////////////
void ThreeDMouse::resetControlUnit ()
{
  ThreeDMouse::cuDemandReporting ();    /* make sure control unit is processing */
  vpr::System::usleep ((long) 100000);  /* wait 10 clock ticks = 100 ms */
  ThreeDMouse::cuResetControlUnit ();   /* command a reset */
  vpr::System::sleep(1);
}


void ThreeDMouse::setBaseOrigin()
    // PURPOSE: Sets the current mouse X,Y,Z position to be the base origin
{
    mData[current].getPositionData()->getTrans (baseVector[0],
                                                baseVector[1],
                                                baseVector[2]);
//      baseVector[0] = mData[current].getPositionData()->pos[0];
//      baseVector[1] = mData[current].getPositionData()->pos[1];
//      baseVector[2] = mData[current].getPositionData()->pos[2];
    // Setup currrent offest as origin
}



/////////////////////////////////////////////////////////////////////////
// convert from raw Euler data record to absolute data
////////////////////////////////////////////////////////////////////////
void ThreeDMouse::eulerToAbsolute (byte record[], vrj::Matrix* data)
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

  data->setTrans (((float) ax) / 1000.0,
                  ((float) ay) / 1000.0,
                  ((float) az) / 1000.0);
//    data->pos[0] = ((float) ax) / 1000.0;
//    data->pos[1] = ((float) ay) / 1000.0;
//    data->pos[2] = ((float) az) / 1000.0;

  arx  = (record[10] & 0x7f) << 7;
  arx += (record[11] & 0x7f);

  ary  = (record[12] & 0x7f) << 7;
  ary += (record[13] & 0x7f);

  arz  = (record[14] & 0x7f) << 7;
  arz += (record[15] & 0x7f);

  data->setTrans (((float) arx) / 40.0,
                  ((float) ary) / 40.0,
                  ((float) arz) / 40.0);
//    data->pos[0] = ((float) arx) / 40.0;
//    data->pos[1] = ((float) ary) / 40.0;
//    data->pos[2] = ((float) arz) / 40.0;
}


/////////////////////////////////////////////////////////////////////////
// print an 8-bit binary string
/////////////////////////////////////////////////////////////////////////
void ThreeDMouse::printBin (char a)
{
  int i;

  for (i=7; i>=0; i--)
    printf ("%c", (a&(1<<i)) ? '1' : '0');
}
