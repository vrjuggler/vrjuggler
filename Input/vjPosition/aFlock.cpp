//#include <sys/file.h>
//#include <sys/ioctl.h>
//#include <sys/time.h>

#include <Math/vjMatrix.h>

#include "aFlock.h"

//: Configure Constructor
// Give:
//   port - such as "/dev/ttyd3"
//   baud - such as 38400, 19200, 9600, 14400, etc...
//   sync - sync type.
//   block - blocking
//   numBrds - number of birds in flock,
//   transmit - transmitter unit number,
//   hemi - hemisphere to track from,
//   filt - filtering type,
//   report -
//   calfile - a calibration file, if "", then use none.
// 
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
		  port_id(-1),
		  active(0),
		  _port(port),
		  _baud(baud),
		  _syncStyle(sync),
		  _blocking(block),
		  _numBirds(numBrds),
		  _xmitterUnitNumber(transmit),
		  _hemisphere(hemi),
		  _filter( filt ),
		  _reportRate(report),
		  _calibrationFileName( calfile )
{
  // fix the report rate if it makes no sense.
  if ((_reportRate != 'Q') && (_reportRate != 'R') &&
      (_reportRate != 'S') && (_reportRate != 'T'))
  {
     // illegal report rate, defaulting to "every other cycle" (R)
     assert(false)
     _reportRate = 'R';
  }

  if (calfile != NULL && calfile[0] != '\0')
  {
  	InitCorrectionTable(calfile);
  	_usingCorrectionTable = true;
  }	
}

aFlock::~aFlock()
{
    StopSampling();
    if (theData != NULL)
       getMyMemPool()->deallocate((void*)theData);
}

void aFlock::SetPort(const char* serialPort)
{
    if (myThread != NULL) {
     cerr << "Cannot change the serial Port while active\n";
     return;
  }
  strncpy(sPort, serialPort, (size_t)30);
}

char* aFlock::GetPort()
{
  if (sPort == NULL) 
  	return "No port";
  else 
  	return sPort;
}

void aFlock::SetBaudRate(int baud)
{
  if (myThread != NULL)
     baudRate = baud;
}

int aFlock::StartSampling()
{
      if (theData != NULL)
         getMyMemPool()->deallocate((void*)theData);
      theData = (vjMatrix*) new vjMatrix[(_numBirds+1)*3];

      current = 0;
      valid = 1;
      progress = 2;

      cout << "    Getting flock ready....\n" << flush;

      port_id = open_port( _port, _baud );
      if (port_id == -1) return 0;
      set_blocking(port_id, blocking);
      set_sync(port_id,syncStyle);
      set_group(port_id);
      set_autoconfig(port_id, _numBirds);
      set_transmitter(port_id, _xmitterUnitNumber);
      set_filter(port_id, _filter);
      set_hemisphere(port_id, _hemisphere, _xmitterUnitNumber, _numBirds);
      set_pos_angles(port_id, _xmitterUnitNumber, _numBirds);
      pickBird(_xmitterUnitNumber,port_id);
      set_rep_and_stream(port_id, _reportRate);

      cout  << "aFlock ready to go.." << endl << flush;
      
   else return 0; // already sampling
}


// call this repeatedly to update the data from the birds.
int aFlock::Sample()
{
     int i;
     int tmp;

     // for [1..n] birds, get their reading:
     for (i=1; i < (_numBirds+1); i++)
     {
	if (i == _xmitterUnitNumber)
		continue;
	
	if (_usingCorrectionTable)
	{
	    //correct the position...
	}

      	// Sets index to current read buffer
	int index = progress*(_numBirds+1)+i-1;
	

         getReading(i, &theData[index], port_id);	

	// Transforms between the cord frames
	// See transform documentation and VR System pg 146
         vjMatrix world_T_transmitter, transmitter_T_reciever, world_T_reciever;

         world_T_transmitter = xformMat;                   // Set transmitter offset from local info
         transmitter_T_reciever = theData[index];        // Get reciever data from sampled data
         world_T_reciever.mult( world_T_transmitter, transmitter_T_reciever);   // compute total transform
         theData[index] = world_T_reciever;                                     // Store corrected xform back into data
     }
	
     lock.acquire();
     tmp = valid;
     valid = progress;
     progress = tmp;
     lock.release();

     return 1;
}

int aFlock::StopSampling()
{
  char   bird_command[4];

  cout << "Stopping the flock..." << flush;

  bird_command[0] = 'B';
  write(port_id, bird_command, 1);
  ioctl(port_id, TCFLSH, (char *) 0);
  sginap( 5 );
  bird_command[0] = 'G';
  write(port_id, bird_command, 1);
  ioctl(port_id, TCFLSH, (char *) 0);
  sleep( 2 );
  close(port_id);
  port_id = -1;

  cout << "stopped." << endl << flush;
  
   return 1;
}

vjMatrix& aFlock::GetMatrix( int d ) // d is 0 based
{
  return theData[current*(_numBirds+1)+d] ;
}

void aFlock::UpdateData()
{
  // swap the indicies for the pointers
  lock.acquire();
  int tmp = current;
  current = valid;
  valid = tmp;
  lock.release();

  return;
}

void aFlock::SetHemisphere(BIRD_HEMI h)
{
    if (active) 
    {
	cout << "Cannot change the hemisphere\n" << flush;
	return;
    } else {
	// Set it.
        _hemisphere = h;
    }
}

void aFlock::SetFilters(BIRD_FILT f)
{
    if (active) 
    {
	cout << "Cannot change filters while active\n" << flush;
	return;
    } else {
	// Set it.
	_filter = f;
    }
}

void aFlock::SetReportRate(char rRate)
{
    if (active) 
    {
	cout << "Cannot change report rate while active\n" << flush;
	return;
    } else {
	// Set it.
	_reportRate = rRate;
    }
}

void aFlock::SetTransmitter(int Transmit)
{
  if (active) 
  {
      cout << "Cannot change transmitter while active\n" << flush;
      return;
  } else {
      // Set it.
      _xmitterUnitNumber = Transmit;
  }
}
void aFlock::SetNumBirds(int n)
{
    if (active) 
    {
	cout << "Cannot change num birds while active\n" << flush;
	return;
    } else {
	// Set it.
	_numBirds = n;
    }
}
void aFlock::SetSync(int sync)
{
  if (active) 
  {
      cout << "Cannot change report rate while active\n" << flush;
      return;
  } else {
      // Set it.
      syncStyle = sync;
  }
}

void aFlock::SetBlocking(int blVal)
{
  if (active) 
  {
      cout << "Cannot change report rate while active\n" << flush;
      return;
  } else {
      // Set it.
      blocking = blVal;
  }
}

void aFlock::Position_Correct(float&x,float&y,float&z) 
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

void aFlock::InitCorrectionTable(const char* fName)
{
  int i,j,k, xsize,ysize,zsize;
  float dump;
  ifstream inFile;

  cout << "	  Initializing calibration table ... " << endl
             << "	    " << fName << endl << flush;
  inFile.open(fName);
  if (!inFile)
  {
	cout << "Unable to open calibration.table\n" << flush;
        return;
  } else {
     cout << "   Calibration table opened sucessfully." << endl << flush;
  }

  inFile >> caltable.xmin >> caltable.xmax
         >> caltable.ymin >> caltable.ymax
         >> caltable.zmin >> caltable.zmax;

  xsize = caltable.xmax - caltable.xmin + 1;
  ysize = caltable.ymax - caltable.ymin + 1;
  zsize = caltable.zmax - caltable.zmin + 1;

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


///////////////////////////////////////////////////////////////////
// Local functions to aFlock.cpp
//////////////////////////////////////////////////////////////////
int aFlock::getReading(int n, vjMatrix* data, int port)
{
   char buff[12], group;
   int  c,i, addr;

   do
   {
      c=i = 0;
      while (!i && c < 99999)
      {
         c++;
         if ((read(port,buff,1) == 1) && (buff[0] & 0x80))
            i = 1;
      }
      while (i != 12 && c < 99999)
      {
         c++;
         i += read(port,&buff[i], 12-i);
      }

      while ((read(port,&group,1) == 0) && c < 99999)
      {
         sgi_nap(1);
	 c++;
	}
	
      if (c >= 5000)
         cout << "aFlock: tracker timeout (" << c << ")" << endl << flush;

      addr = group;
   }
   while (addr != n);

   //cout << "addr: " << addr << endl;

   vjVec3 pos_data, or_data;

   pos_data[0] = rawToFloat(buff[1],buff[0]) * POSITION_RANGE;       // X
   pos_data[1] = rawToFloat(buff[3],buff[2]) * POSITION_RANGE;       // Y
   pos_data[2] = rawToFloat(buff[5],buff[4]) * POSITION_RANGE;       // Z
   or_data[0] = rawToFloat(buff[7],buff[6]) * ANGLE_RANGE;           // rotZ
//  if (data->or[0] < -180.0f)
//     data->or[0] += 360.0f;
   or_data[1] = rawToFloat(buff[9],buff[8]) * ANGLE_RANGE;           // rotY
   or_data[2] = rawToFloat(buff[11],buff[10]) * ANGLE_RANGE;         // rotX

   data->makeZYXEuler(or_data[0], or_data[1], or_data[2]);
   data->setTrans(pos_data[0], pos_data[1], pos_data[2]);

   return addr;
}

float aFlock::rawToFloat(char& r1, char& r2)
{
   // return ((float) (((r1 & 0x7f) << 9) | (r2 & 0x7f) << 2) / 0x7fff);
   short int ival1,ival2,val;
   ival1 = r1 & 0x7f;
   ival2 = r2 & 0x7f;
   val = (ival1 << 9) | ival2<<2;
   return ((float)val) / 0x7fff;
}

void  aFlock::pickBird(int birdID, int port)
{
   char buff = 0xF0 + birdID;
   write(port, &buff, 1);
   ioctl(port, TCFLSH, (char*) 0);
   sginap(1);
}

int aFlock::open_port(char* serialPort, int baud)
{
   ///////////////////////////////////////////////////////////////////
   // Open and close the port to reset the tracker, then
   // Open the port
   ///////////////////////////////////////////////////////////////////
   int port_id = open(serialPort, O_RDWR | O_NDELAY);
   if (port_id == -1)
   {
      cout << "!!aFlock port open (1 of 2) failed\n" << flush ;
      return port_id;
   }
   else
   {
      cout << "aFlock port open (1 of 2) successfully\n" << flush;
   }
   sleep(2);
   close(port_id);
   port_id = open(serialPort,O_RDWR | O_NDELAY);
   if (port_id == -1)
   {
      cout << "!!aFlock port open (2 of 2) failed\n" << flush;
      return port_id;
   }
   else
   {
      cout << "aFlock port open (2 of 2) successfully\n" << flush;
   }

   //////////////////////////////////////////////////////////////////
   // Setup the port, current setting is 38400 baud
   //
   //////////////////////////////////////////////////////////////////
   termio port_a;
   ioctl(port_id,TCGETA,&port_a);

   port_a.c_iflag = port_a.c_oflag = port_a.c_lflag = 0;

   switch (baud)
   {
   case 38400:port_a.c_cflag = (B38400 | CS8 | CLOCAL | CREAD);
      break;
   case 19200:port_a.c_cflag = (B19200 | CS8 | CLOCAL | CREAD);
      break;
   case 9600:port_a.c_cflag = (B9600 | CS8 | CLOCAL | CREAD);
      break;
   case 4800:port_a.c_cflag = (B4800 | CS8 | CLOCAL | CREAD);
      break;
   }

   port_a.c_cflag = (B38400 | CS8 | CLOCAL | CREAD);
   port_a.c_cc[0] = port_a.c_cc[1] = port_a.c_cc[2] = port_a.c_cc[3] = 0;
   port_a.c_cc[4] = port_a.c_cc[5] = 1;

   // Set the new attributes
   ioctl(port_id,TCSETA,&port_a);
   ioctl(port_id,TIOCNOTTY);
   return port_id;
}

void aFlock::set_blocking(int port, int blocking)
{
 //////////////////////////////////////////////////////////////////
 // Setup a non/blocked port & Flush port
 //////////////////////////////////////////////////////////////////
  static int blockf,nonblock;
  int flags;

  flags = fcntl(port,F_GETFL,0);
  blockf = flags & ~FNDELAY;  // Turn blocking on
  nonblock = flags | FNDELAY; // Turn blocking off

  fcntl(port,F_SETFL,blocking ? blockf : nonblock); // 0 Non Blocked
                                                    // 1 Blocked
  ioctl(port,TCFLSH,2);
  sginap(10);
  char junk[1024];
  read(port, junk, 1024);
  sleep(1);

}

void aFlock::set_sync(int port, int sync)
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
  write(port,buff,2);
  ioctl(port,TCFLSH,0);
}


void set_hemisphere(int port, BIRD_HEMI hem, int transmitter, int numbirds)
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
    pickBird(i,port);
    switch(hem) {
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
    write(port, buff, 3);
    ioctl(port, TCFLSH, (char *) 0);
    sginap(5);
  }
}

void set_rep_and_stream(int port, char reportRate)
{
 char buff[1];
 /////////////////////////////////////////////////////////////////
 // Set report rate
 //             Q  Every cycle
 //  buff[0] - 'R' Every other bird cycle
 //             S  every 8 cycles
 //             T  every 32 cycles
 /////////////////////////////////////////////////////////////////
  buff[0] = _reportRate;
  write(port, buff, 1);
  ioctl(port, TCFLSH, (char *) 0);
  sginap(20);

 ////////////////////////////////////////////////////////////////
 // set stream mode
 ////////////////////////////////////////////////////////////////
  buff[0] = '@';
  write(port, buff, 1);
  ioctl(port, TCFLSH, (char *) 0);
  sginap(5);

}

void set_pos_angles(int port, int transmitter, int numbirds)
{
 //////////////////////////////////////////////////////////////////
 // Set Position Angles
 /////////////////////////////////////////////////////////////////
  char buff[1];
  for (int i = 1; i < (numbirds + 1); i++)
  {
	if (i == transmitter)
		continue;
    pickBird(i,port);
    buff[0] = 'Y';
    write(port, buff, 1);
    ioctl(port, TCFLSH, (char *) 0);
    sginap(5);
  }

}

void set_filter(int port, BIRD_FILT filter)
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
  write(port,buff,4);
  ioctl(port,TCFLSH,(char*)0);
 // Do I need to sleep here?
    sginap(120);
}

void set_transmitter(int port, int transmitter)
{
///////////////////////////////////////////////////////////////
 // Sets up the device for Transmitting (manual page 67)
 // Command (0x30) for Next Transmitter
 ///////////////////////////////////////////////////////////////
  char buff[2];
  buff[0] = (unsigned char) (0x30);
  buff[1] = (unsigned char) transmitter  << 4;
  write(port, buff, 2);
  ioctl(port, TCFLSH,(char*) 0);
  sginap(120);
}


void set_autoconfig(int port, int numbirds)
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
  ioctl(port, TCFLSH, (char*) 0);
  sleep(2);
}

void set_group(int port)
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
  ioctl(port, TCFLSH, (char *) 0);
  sleep(2);

}
