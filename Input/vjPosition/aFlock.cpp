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

#include <iostream>        // for std::cout
#include <fstream>         // for std::ifstream
#include <string.h>
		
#include <fcntl.h>         // for open
#include <termios.h>
#include <unistd.h>        // for sleep, and ioctl
#include <sys/types.h>     // for open
#include <sys/stat.h>      // for open
#include <assert.h>        // for assert

#if defined(__sun__) || defined(__hpux)
#include <sys/file.h>
#endif

#include <Input/vjPosition/aFlock.h>


const int aFlock::MAXCHARSTRINGSIZE = 256;
const int aFlock::mSleepFactor = 3;


//: Configure Constructor
// Give:                                                 <BR>
//   port - such as "/dev/ttyd3"                         <BR>
//   baud - such as 38400, 19200, 9600, 14400, etc...    <BR>
//   sync - sync type.                                   <BR>
//   block - blocking                                    <BR>
//   numBrds - number of birds in flock,                 <BR>
//   transmit - transmitter unit number,                 <BR>
//   hemi - hemisphere to track from,                    <BR>
//   filt - filtering type,                              <BR>
//   report -                                            <BR>
//   calfile - a calibration file, if "", then use none. <BR>
//                                                       <BR>
// Result: configures internal data members,
//         doesn't actually talk to the FOB yet.
aFlock::aFlock(const char* const port,
		const int& baud,
		const int& sync,
		const int& block,
		const int& numBrds,
		const int& transmit,
		const BIRD_HEMI& hemi,
		const BIRD_FILT& filt,
		const char& report,
		const char* const calfile) :
		  _reportRate(report),
		  _hemisphere(hemi),
		  _filter( filt ),
		  _portId(-1),
		  _baud(baud),
		  _syncStyle(sync),
		  _blocking(block),
		  _numBirds(numBrds),
		  _xmitterUnitNumber(transmit),
		  _usingCorrectionTable(false),
		  _active(false)
{
  if (port != NULL)
  	strncpy( _port, port, aFlock::MAXCHARSTRINGSIZE );

  // fix the report rate if it makes no sense.
  if ((_reportRate != 'Q') && (_reportRate != 'R') &&
      (_reportRate != 'S') && (_reportRate != 'T'))
  {
     // illegal report rate, defaulting to "every other cycle" (R)
     assert(false);
     _reportRate = 'R';
  }

  if (calfile != NULL && calfile[0] != '\0')
  {
  	strncpy( _calibrationFileName, calfile, aFlock::MAXCHARSTRINGSIZE );
  	this->initCorrectionTable(_calibrationFileName);
  	_usingCorrectionTable = true;
  }	
}

//: Destructor
aFlock::~aFlock()
{
    this->stop();
}

//: see if the flock is active or not
const bool& aFlock::isActive() const
{
	return _active;
}

//: set the port to use
//  this will be a string in the form of the native OS descriptor <BR>
//  ex: unix - "/dev/ttyd3", win32 - "COM3" <BR>
//  NOTE: flock.isActive() must be false to use this function
void aFlock::setPort(const char* const serialPort)
{
    if (_active)
    {
	std::cout << "Flock: Cannot change the serial Port while active\n";
	return;
    }
    if (serialPort != NULL)
  	strncpy( _port, serialPort, aFlock::MAXCHARSTRINGSIZE );
}

//: get the port used
//  this will be a string in the form of the native OS descriptor <BR>
//  ex: unix - "/dev/ttyd3", win32 - "COM3"
const char* aFlock::getPort() const
{
    return _port;
}

//: set the baud rate
//  this is generally 38400, consult flock manual for other rates. <BR>
//  NOTE: flock.isActive() must be false to use this function
void aFlock::setBaudRate(const int& baud)
{
    if (_active)
    {
	std::cerr << "Flock: Cannot change the baud rate while active\n";
	return;
    } else {
	_baud = baud;
    }
}

//: call this to connect to the flock device.
//  NOTE: flock.isActive() must be false to use this function
int aFlock::start()
{
    if (!_active)
    {
      std::cout << "aFlock: Getting ready....\n\n" << std::flush;

      std::cout<<"aFlock: Opening port\n"<<std::flush;
      aFlock::open_port( _port, _baud, _portId );
      if (_portId == -1)
	       return 0;

      std::cout<<"aFlock: Setting blocking\n"<<std::flush;
      aFlock::set_blocking( _portId, _blocking );

      std::cout<<"aFlock: Setting sync\n"<<std::flush;
      aFlock::set_sync( _portId, _syncStyle );

      std::cout<<"aFlock: Setting group\n"<<std::flush;
      aFlock::set_group( _portId );

      std::cout<<"aFlock: Setting autoconfig\n"<<std::flush;
      aFlock::set_autoconfig( _portId, _numBirds );

      std::cout<<"aFlock: Setting transmitter\n"<<std::flush;
      aFlock::set_transmitter( _portId, _xmitterUnitNumber );

      std::cout<<"aFlock: Setting filter\n"<<std::flush;
      aFlock::set_filter( _portId, _filter );

      std::cout<<"aFlock: Setting hemisphere\n"<<std::flush;
      aFlock::set_hemisphere( _portId, _hemisphere, _xmitterUnitNumber, _numBirds );

      std::cout<<"aFlock: Setting pos_angles\n"<<std::flush;
      aFlock::set_pos_angles( _portId, _xmitterUnitNumber, _numBirds );

      std::cout<<"aFlock: Setting pickBird\n"<<std::flush;
      aFlock::pickBird( _xmitterUnitNumber,_portId );

      std::cout<<"aFlock: Setting rep_and_stream\n"<<std::flush;
      aFlock::set_rep_and_stream( _portId, _reportRate );

      std::cout  << "aFlock: ready to go.." << std::endl << std::flush;

      // flock is active.
      _active = true;

      // return success
      return 1;
    } 
    
    else
	   return 0; // already sampling
}


//: call this repeatedly to update the data from the birds.
//  NOTE: flock.isActive() must be true to use this function
int aFlock::sample()
{
     // can't sample when not active.
     assert( _active == true );
     int i;
     int loopCount = _numBirds + 1;
     if (_xmitterUnitNumber <= _numBirds) {loopCount++;} 

     // for [1..n] birds, get their reading:
     int j = 0;
     for (i=1; i < loopCount && i < MAX_SENSORS; i++)
	{
		j++;
// If the transmitter number is less than or equal to the number of birds, we need to ignore it.

          if (i == _xmitterUnitNumber)
	  { 
                j--;
		continue;
          }
 	
// However, we need to still copy the data into consecutive values in the wrapper class, so we
// introduce "j" to account for that correction.  It is equal to "i" while we haven't encountered
// the transmitter, but equal to "i-1" afterwards.

	// you can never go above the maximum number of sensors.
	assert( i < MAX_SENSORS );
	aFlock::getReading(i, _portId,
		this->xPos(j),
		this->yPos(j),
		this->zPos(j),
		this->zRot(j),
		this->yRot(j),
		this->xRot(j));	
		
	if (_usingCorrectionTable)
	{
	    this->positionCorrect( this->xPos(j),
			    this->yPos(j),
			    this->zPos(j) );
	}
     }

     return 1;
}

//: stop the flock
int aFlock::stop()
{
    char   bird_command[4];

    std::cout << "Flock: Stopping the flock..." << std::flush;

    bird_command[0] = 'B';
    write( _portId, bird_command, 1 );

    tcflush(_portId, TCIOFLUSH);

    usleep( 500 * aFlock::mSleepFactor );
    bird_command[0] = 'G';
    write( _portId, bird_command, 1 );

    tcflush(_portId, TCIOFLUSH);

    usleep( 200 * aFlock::mSleepFactor );
    close( _portId );
    _portId = -1;

    // flock is not active now.
    _active = false;

    std::cout << "stopped." << std::endl << std::flush;

    return 1;
}

//: Set the hemisphere that the transmitter transmits from.
//  NOTE: flock.isActive() must be false to use this function
void aFlock::setHemisphere( const BIRD_HEMI& h )
{
    if (_active)
    {
	std::cout << "Flock: Cannot change the hemisphere\n" << std::flush;
	return;
    } else {
	// Set it.
        _hemisphere = h;
    }
}

//: Set the type of filtering that the flock uses
//  NOTE: flock.isActive() must be false to use this function
void aFlock::setFilterType( const BIRD_FILT& f )
{
    if (_active)
    {
	std::cout << "Flock: Cannot change filter type while active\n"
                  << std::flush;
	return;
    } else {
	// Set it.
	_filter = f;
    }
}

//: Set the report rate that the flock uses
//  NOTE: flock.isActive() must be false to use this function
void aFlock::setReportRate( const char& rRate )
{
    if (_active)
    {
	std::cout << "Flock: Cannot change report rate while active\n"
                  << std::flush;
	return;
    } else {
	// Set it.
	_reportRate = rRate;
    }
}

//: Set the unit number of the transmitter
//  give - an integer that is the same as the dip switch
//         setting on the transmitter box (for the unit number) <BR>
//  NOTE: flock.isActive() must be false to use this function
void aFlock::setTransmitter( const int& Transmit )
{
  if (_active)
  {
      std::cout << "Flock: Cannot change transmitter while active\n"
                << std::flush;
      return;
  } else {
      // Set it.
      _xmitterUnitNumber = Transmit;
  }
}

//: Set the number of birds to use in the flock.
//  give - an integer number not more than the number of
//         birds attached to the system <BR>
//  NOTE: flock.isActive() must be false to use this function
void aFlock::setNumBirds( const int& n )
{
    if (_active)
    {
	std::cout << "Flock: Cannot change num birds while active\n"
                  << std::flush;
	return;
    } else {
	// Set it.
	_numBirds = n;
    }
}

//: set the video sync type
//  this option allows the Flock to syncronize its pulses with
//  your video display.  This will eliminate most flicker caused
//  by the magnetic distortion. <BR>
//  - Refer to your flock manual for what number to use.
//  NOTE: flock.isActive() must be false to use this function
void aFlock::setSync(const int& sync)
{
  if (_active)
  {
      std::cout << "Flock: Cannot change report rate while active\n"
                << std::flush;
      return;
  } else {
      // Set it.
      _syncStyle = sync;
  }
}

//: set blocking of flock
//  see flock manual for details.
//  NOTE: flock.isActive() must be false to use this function
void aFlock::setBlocking( const int& blVal )
{
  if (_active)
  {
      std::cout << "Flock: Cannot change blocking while active\n" << std::flush;
      return;
  } else {
      // Set it.
      _blocking = blVal;
  }
}

//: with the calibration table info, correct a given position
//  give - a position in x, y, z euclidian coordinates
//  returns - a corrected position in x, y, z euclidian coordinates
void aFlock::positionCorrect( float& x, float& y, float& z )
{
    int xlo,ylo,zlo,xhi,yhi,zhi;
    float a,b,c,a1,b1,c1;

    // Find corners
    xlo = (int)(x-caltable.xmin);
    ylo = (int)(y-caltable.ymin);
    zlo = (int)(z-caltable.zmin);
    xhi = xlo+1;
    yhi = ylo+1;
    zhi = zlo+1;
    a1 = 1 - (a = caltable.xmin + xhi - x);
    b1 = 1 - (b = caltable.ymin + yhi - y);
    c1 = 1 - (c = caltable.zmin + zhi - z);

    x = a * b* c*caltable.xloc[xlo][ylo][zlo] +
        a * b*c1*caltable.xloc[xlo][ylo][zhi] +
        a *b1* c*caltable.xloc[xlo][yhi][zlo]+
        a *b1*c1*caltable.xloc[xlo][yhi][zhi]+
        a1* b* c*caltable.xloc[xhi][ylo][zlo]+
        a1* b*c1*caltable.xloc[xhi][ylo][zhi]+
        a1*b1* c*caltable.xloc[xhi][yhi][zlo]+
        a1*b1*c1*caltable.xloc[xhi][yhi][zhi];
    y = a * b* c*caltable.yloc[xlo][ylo][zlo] +
        a * b*c1*caltable.yloc[xlo][ylo][zhi] +
        a *b1* c*caltable.yloc[xlo][yhi][zlo]+
        a *b1*c1*caltable.yloc[xlo][yhi][zhi]+
        a1* b* c*caltable.yloc[xhi][ylo][zlo]+
        a1* b*c1*caltable.yloc[xhi][ylo][zhi]+
        a1*b1* c*caltable.yloc[xhi][yhi][zlo]+
        a1*b1*c1*caltable.yloc[xhi][yhi][zhi];
    z = a * b* c*caltable.zloc[xlo][ylo][zlo] +
        a * b*c1*caltable.zloc[xlo][ylo][zhi] +
        a *b1* c*caltable.zloc[xlo][yhi][zlo]+
        a *b1*c1*caltable.zloc[xlo][yhi][zhi]+
        a1* b* c*caltable.zloc[xhi][ylo][zlo]+
        a1* b*c1*caltable.zloc[xhi][ylo][zhi]+
        a1*b1* c*caltable.zloc[xhi][yhi][zlo]+
        a1*b1*c1*caltable.zloc[xhi][yhi][zhi];
    return;
}

//: init the correction table from a file
//  give - a file name of the calibration file.
//  result - initializes the correction table with the file's info
void aFlock::initCorrectionTable( const char* const fName )
{
  int i,j,k, xsize,ysize,zsize;
  float dump;
  std::ifstream inFile;

  std::cout << "	  Initializing calibration table ... " << std::endl
            << "	    " << fName << std::endl << std::flush;
  inFile.open( fName );
  if (!inFile)
  {
	std::cout << "Unable to open calibration.table\n" << std::flush;
        return;
  } else {
     std::cout << "   Calibration table opened sucessfully." << std::endl
               << std::flush;
  }

  inFile >> caltable.xmin >> caltable.xmax
         >> caltable.ymin >> caltable.ymax
         >> caltable.zmin >> caltable.zmax;

  xsize = (int) (caltable.xmax - caltable.xmin) + 1;
  ysize = (int) (caltable.ymax - caltable.ymin) + 1;
  zsize = (int) (caltable.zmax - caltable.zmin) + 1;

  for (i = 0; i < xsize; i++)
    for (j = 0; j < ysize; j++)
      for (k = 0; k < zsize; k++)
         {
	    inFile >> dump >> dump >> dump
		   >> caltable.xloc[i][j][k]
		   >> caltable.yloc[i][j][k]
		   >> caltable.zloc[i][j][k];
	 }

   inFile.close();
}

 float& aFlock::xPos( const int& i )  { assert( i < MAX_SENSORS ); return _position[i][0]; }
 float& aFlock::yPos( const int& i )  { assert( i < MAX_SENSORS ); return _position[i][1]; }
 float& aFlock::zPos( const int& i )  { assert( i < MAX_SENSORS ); return _position[i][2]; }
 float& aFlock::zRot( const int& i )  { assert( i < MAX_SENSORS ); return _orientation[i][0]; }
 float& aFlock::yRot( const int& i )  { assert( i < MAX_SENSORS ); return _orientation[i][1]; }
 float& aFlock::xRot( const int& i )  { assert( i < MAX_SENSORS ); return _orientation[i][2]; }


///////////////////////////////////////////////////////////////////
// Local functions to aFlock.cpp
//////////////////////////////////////////////////////////////////
//: get a reading
//  give - n:    the bird unit number  <BR>
//  give - port: the flock port number <BR>
//  give - xyz positions               <BR>
//  give - zyx rotations
int aFlock::getReading( const int& n, const int& port,
    float& xPos, float& yPos, float& zPos,
    float& zRot, float& yRot, float& xRot )
{
    char buff[12], group;
    int  c,i, addr;

    do
    {
	c=i = 0;
	while (!i && c < 99999)
	{
	    c++;
	    if ((read( port,buff,1 ) == 1) && (buff[0] & 0x80))
		i = 1;
	}
	while (i != 12 && c < 99999)
	{
	    c++;
	    i += read( port,&buff[i], 12-i );
	}
	
	while ((read( port,&group,1 ) == 0) && c < 99999)
	{
	    usleep(100 * aFlock::mSleepFactor);
	    c++;
	}
	
	if (c >= 5000)
	    std::cout << "aFlock: tracker timeout (" << c << ")" << std::endl
                      << std::flush;
	
	addr = group;
    } while (addr != n);

    //std::cout << "addr: " << addr << std::endl;

    // Position
    xPos = rawToFloat(buff[1],buff[0])   * POSITION_RANGE;
    yPos = rawToFloat(buff[3],buff[2])   * POSITION_RANGE;
    zPos = rawToFloat(buff[5],buff[4])   * POSITION_RANGE;

    // Orientation
    zRot = rawToFloat(buff[7],buff[6])   * ANGLE_RANGE;
    yRot = rawToFloat(buff[9],buff[8])   * ANGLE_RANGE;
    xRot = rawToFloat(buff[11],buff[10]) * ANGLE_RANGE;

    return addr;
}

float aFlock::rawToFloat( char& r1, char& r2 )
{
   // return ((float) (((r1 & 0x7f) << 9) | (r2 & 0x7f) << 2) / 0x7fff);

   short int ival1,ival2,val;
   ival1 = r1 & 0x7f;
   ival2 = r2 & 0x7f;
   val = (ival1 << 9) | ival2<<2;
   return ((float)val) / 0x7fff;
}

void  aFlock::pickBird( const int& birdID, const int& port )
{
    char buff = 0xF0 + birdID;
    write( port, &buff, 1 );

    tcflush(port, TCIOFLUSH);

    usleep ( 100 * aFlock::mSleepFactor );
}

//: Open the port.
//  give - a serial port
//  give - a baud rate
//  returns portId twice (created by the open function)
//  NOTE: portId is returned from both ends of this function.
//  if portId == -1 then function failed to open the port.
int aFlock::open_port( const char* const serialPort,
			const int& baud,
			int& portId )
{
    ///////////////////////////////////////////////////////////////////
    // Open and close the port to reset the tracker, then
    // Open the port
    ///////////////////////////////////////////////////////////////////
    portId = open( serialPort, O_RDWR | O_NDELAY);
    if (portId == -1)
    {
	std::cout << "  port reset failed (because port open failed)\n"
                  << std::flush;
	return portId;
    } else {
	sleep(2);
	close( portId );
	std::cout << "  port reset successfully (port was opened then closed)\n"
                  << std::flush;
    }

    portId = open( serialPort, O_RDWR | O_NDELAY);

    if (portId == -1)
    {
	std::cout << "  port open failed\n" << std::flush;
	return portId;
    } else {
	std::cout << "  port open successfully\n" << std::flush;
    }

    //////////////////////////////////////////////////////////////////
    // Setup the port, current setting is 38400 baud
    //
    //////////////////////////////////////////////////////////////////

    std::cout << "  Getting current term setting\n" << std::flush;
    termios termIoPort;
    tcgetattr(portId, &termIoPort);

    // Set the flags to 0 to clear out any cruft and to ensure that we are
    // setting fresh values.
    termIoPort.c_iflag = termIoPort.c_oflag = termIoPort.c_lflag = 0;

    // get the B* format baud rate
    // i.e.: B38400 is baud = 38400
    int magicBaudRate = 0;
    switch (baud)
    {
	case 150: magicBaudRate = B150; break;
	case 200: magicBaudRate = B200; break;
	case 300: magicBaudRate = B300; break;
	case 600: magicBaudRate = B600; break;
	case 1200: magicBaudRate = B1200; break;
	case 1800: magicBaudRate = B1800; break;
	case 2400: magicBaudRate = B2400; break;
	case 4800: magicBaudRate = B4800; break;
	case 9600: magicBaudRate = B9600; break;
	case 19200: magicBaudRate = B19200; break;
	
#ifndef _POSIX_SOURCE
	case 57600: magicBaudRate = B57600; break;
#   ifdef B76800
	    case 76800: magicBaudRate = B76800; break;
#   endif
	case 115200: magicBaudRate = B115200; break;
#endif

        case 38400:
        default:
          magicBaudRate = B38400;
          break;
    }

    termIoPort.c_cflag = CS8 | CLOCAL | CREAD;

    // Set the baud rate flag.
    cfsetospeed(&termIoPort, magicBaudRate);
    cfsetispeed(&termIoPort, magicBaudRate);

    // make sure we're not going off the end
    assert( NCCS >= 16 );

    // Set the new attributes
    int result = 0;
    std::cout << "  Setting new term setting: "<<baud<<" baud..." << std::flush;
    result = tcsetattr(portId, TCSANOW, &termIoPort);

    // did it succeed?
    if (result == 0)
	std::cout<<" success\n"<<std::flush;
    else
	std::cout<<" failed\n"<<std::flush;

    // return the portID
    return portId;
}

void aFlock::set_blocking( const int& port, const int& blocking )
{
    //////////////////////////////////////////////////////////////////
    // Setup a non/blocked port & Flush port
    //////////////////////////////////////////////////////////////////
    static int blockf,nonblock;
    int flags;

    flags = fcntl( port,F_GETFL,0 );

    // Turn blocking on
    blockf   = flags & ~FNDELAY;

    // Turn blocking off
    nonblock = flags | FNDELAY;

    // 0 Non Blocked
    // 1 Blocked
    fcntl( port, F_SETFL, blocking ? blockf : nonblock );

    tcflush(port, TCIOFLUSH);

    usleep( 1000 * aFlock::mSleepFactor );
}

void aFlock::set_sync( const int& port, const int& sync )
{
    /////////////////////////////////////////////////////////////////
    // Set CRT sync: (manual page 82)
    //   set crt sync
    //   nosync    -   0
    //   > 72Hz    -   1 (type 1)
    //                 2 (type 2)
    /////////////////////////////////////////////////////////////////
    unsigned char buff[4] = {'A', 1};
    buff[1] = sync;
    write( port,buff,2 );

    tcflush(port, TCIOFLUSH);
}


void aFlock::set_hemisphere( const int& port,
			const BIRD_HEMI& hem,
			const int& transmitter,
			const int& numbirds )
{
    /////////////////////////////////////////////////////////////////
    // Set Hemisphere for birds taking input
    //
    //  buff   [1]   [2]
    // Front: 0x00, 0x00
    // Aft  : 0x00, 0x01
    // Upper: 0x0C, 0x01
    // Lower: 0x0C, 0x00
    // Left : 0x06, 0x01
    // Right: 0x06, 0x00
    /////////////////////////////////////////////////////////////////
    char buff[3];
    buff[0] = 'L';
    for (int i = 1; i < (numbirds+1); i++)
    {
	if (i == transmitter)
		continue;
	pickBird( i,port );
	switch (hem)
	{
	case FRONT_HEM:
	    buff[1] = 0x00;
	    buff[2] = 0x00;
	    break;
	case AFT_HEM:
	    buff[1] = 0x00;
	    buff[2] = 0x01;
	    break;
	case UPPER_HEM:
	    buff[1] = 0x0C;
	    buff[2] = 0x01;
	    break;
	case LOWER_HEM:
	    buff[1] = 0x0C;
	    buff[2] = 0x00;
	    break;		
	case LEFT_HEM:
	    buff[1] = 0x06;
	    buff[2] = 0x01;
	    break;
	case RIGHT_HEM:
	    buff[1] = 0x06;
	    buff[2] = 0x00;
	    break;
	}
	write( port, buff, 3 );

        tcflush(port, TCIOFLUSH);

	usleep( 500 * aFlock::mSleepFactor );
    }
}

void aFlock::set_rep_and_stream(const int& port, const char& reportRate)
{
    char buff[1];
    /////////////////////////////////////////////////////////////////
    // Set report rate
    //             Q  Every cycle
    //  buff[0] - 'R' Every other bird cycle
    //             S  every 8 cycles
    //             T  every 32 cycles
    /////////////////////////////////////////////////////////////////
    buff[0] = reportRate;
    write( port, buff, 1 );

    tcflush(port, TCIOFLUSH);

    usleep( 2000 * aFlock::mSleepFactor );

    ////////////////////////////////////////////////////////////////
    // set stream mode
    ////////////////////////////////////////////////////////////////
    buff[0] = '@';
    write( port, buff, 1 );

    tcflush(port, TCIOFLUSH);

    usleep( 500 * aFlock::mSleepFactor );
}

void aFlock::set_pos_angles(const int& port, const int& transmitter, const int& numbirds)
{
    //////////////////////////////////////////////////////////////////
    // Set Position Angles
    /////////////////////////////////////////////////////////////////
    char buff[1];
    for (int i = 1; i < (numbirds + 1); i++)
    {
	if (i == transmitter)
		continue;
	aFlock::pickBird( i,port );
	buff[0] = 'Y';
	write( port, buff, 1 );

        tcflush(port, TCIOFLUSH);

	usleep( 500 * aFlock::mSleepFactor );
    }
}

void aFlock::set_filter(const int& port, const BIRD_FILT& filter)
{
    ///////////////////////////////////////////////////////////////
    // Turn filters on (manual page 48)
    // 0s turn AC NARROW notch filter ON
    //         AC WIDE notch filter ON
    //         DC filter ON
    ///////////////////////////////////////////////////////////////
    char buff[4];
    buff[0] = 'P';
    buff[1] = 0x04;
    buff[2] = 0x00;
    buff[3] = 0;
    write(port, buff, 4);

    tcflush(port, TCIOFLUSH);

    //TODO: Do I need to sleep here?
    usleep(12000 * aFlock::mSleepFactor);
}

void aFlock::set_transmitter(const int& port, const int& transmitter)
{
    ///////////////////////////////////////////////////////////////
    // Sets up the device for Transmitting (manual page 67)
    // Command (0x30) for Next Transmitter
    ///////////////////////////////////////////////////////////////
    char buff[2];
    buff[0] = (unsigned char) (0x30);
    buff[1] = (unsigned char) transmitter  << 4;
    write(port, buff, 2);

    tcflush(port, TCIOFLUSH);

    usleep(12000 * aFlock::mSleepFactor);
}


void aFlock::set_autoconfig(const int& port, const int& numbirds)
{
    ///////////////////////////////////////////////////////////////
    // FBB AUTO-CONFIGURATION (manual page 60)
    //
    // Must wait 300 milliseconds before and after this command
    ///////////////////////////////////////////////////////////////
    char buff[3];
    buff[0] = 'P';
    buff[1] = 0x32;
    buff[2] = numbirds+1;  //number of input devices + 1 for transmitter
    write(port, buff,3);

    tcflush(port, TCIOFLUSH);

    sleep(2);
}

void aFlock::set_group(const int& port)
{
    ////////////////////////////////////////////////////////////////
    // Setup group mode: (manual page 59)
    // 'P' Change Parameter
    // Number 35 (hex 23),
    // Set flag to 1 enabling group mode.
    ////////////////////////////////////////////////////////////////
    char buff[3];
    buff[0] = 'P';
    buff[1] = 0x23;
    buff[2] = 0x01;
    write(port, buff, 3);

    tcflush(port, TCIOFLUSH);

    sleep(2);
}
