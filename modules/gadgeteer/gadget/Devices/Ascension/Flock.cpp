#include <vjConfig.h>
#include <sys/file.h>
#include <sys/ioctl.h>
#include <sys/time.h>

#include <Input/vjPosition/vjFlock.h>

#include <Kernel/vjDebug.h>


inline int getReading(int i, vjMatrix* data, int port);
inline float rawToFloat(char& r1, char& r2);
inline void  pickBird(int sensor, int port_id);
static int open_port(char* serialPort, int baud);
static void set_blocking(int port, int blocking);
static void set_sync(int port, int sync);
static void set_hemisphere(int port, BIRD_HEMI hem, int transmitter, int numbirds);
static void set_rep_and_stream(int port, char repRate);
static void set_pos_angles(int port, int transmitter, int numbirds);
static void set_filter(int port, BIRD_FILT filter);
static void set_transmitter(int port, int transmitter);
static void set_autoconfig(int port, int numbirds);
static void set_group(int port);

vjFlock::vjFlock()
{
;
}

bool vjFlock::config(vjConfigChunk *c)
{
    vjDEBUG(0) << "	 vjFlock::vjFlock(vjConfigChunk*)" << endl << vjDEBUG_FLUSH;
    
    if(!vjPosition::config(c))
     return false;
    
    syncStyle = static_cast<int>(c->getProperty("sync"));//1;
    blocking = static_cast<int>(c->getProperty("blocking"));//0;
    
    // to be added to config:
    theTransmitter = static_cast<int>(c->getProperty("transmitter"));
    numBirds = static_cast<int>(c->getProperty("num"));
    hemisphere = (BIRD_HEMI)(static_cast<int>(c->getProperty("hemi")));  //LOWER_HEMI
    char r = static_cast<char*>(c->getProperty("report"))[0];
    if ((r != 'Q') && (r != 'R') &&
      (r != 'S') && (r != 'T'))
    {
	vjDEBUG(0)  << "   illegal report rate from configChunk, defaulting to every other cycle (R)" << endl << vjDEBUG_FLUSH;
	repRate = 'R';
    }
    else
	repRate = r;
    
    vjDEBUG(0) << "	  Flock Settings: " << endl
	       << "	        theTransmitter: " << theTransmitter << endl
		    << "             numBirds      : " << numBirds << endl
	       << "	        baudRate      : " << baudRate << endl
	       << "	        deviceAbilities:" << deviceAbilities << endl
	       << "	        sPort         : " << sPort << endl
	       << "		instance name : " << instName << endl
	       << endl << vjDEBUG_FLUSH;
    
    InitCorrectionTable(c->getProperty("calfile"));
    
    return true;
}

vjFlock::vjFlock(int sync, int block, int numBrds, int transmit,
      BIRD_HEMI hemi, BIRD_FILT filt,
      char report, const char* calfile) : vjPosition(), vjInput()
{
  vjDEBUG(0)   << "        vjFlock::vjFlock(" << sync << "," << block << ","
               << numBrds << "," << transmit
               << "," << hemi << "," << report << "," << calfile << ") " << endl << vjDEBUG_FLUSH;

  port_id = -1; active = 0;
  syncStyle = sync;
  blocking = block;
  theTransmitter = transmit;
  numBirds = numBrds;
  hemisphere = hemi;
  repRate = report;

  myThread = NULL;

  InitCorrectionTable(calfile);
}

vjFlock::~vjFlock()
{
  vjDEBUG(0)  << "	vjFlock::~vjFlock()" << endl << vjDEBUG_FLUSH;
  StopSampling();
    if (theData != NULL)
       getMyMemPool()->deallocate((void*)theData);
}

static void SampleBirds(void* pointer)
{

   //struct timeval tv;
   //double start_time, stop_time;

   vjDEBUG(0) << "vjFlock: Spawned SampleBirds starting" << endl << vjDEBUG_FLUSH;

   vjFlock* devPointer = (vjFlock*) pointer;
   for (;;)
   {

//**     gettimeofday(&tv,0);
//**     start_time = (double)tv.tv_sec+ (double)tv.tv_usec / 1000000.0;

      //for (int i = 0; i < 60; i++)
         devPointer->Sample();

//**     gettimeofday(&tv,0);
//**     stop_time = (double)tv.tv_sec+ (double)tv.tv_usec / 1000000.0;
      //   cout << 1/((stop_time-start_time) / 60)
      //      << "  " << endl;


   }

}

int vjFlock::StartSampling()
{
   if (myThread == NULL)
   {
      //int i;

      if (theData != NULL)
         getMyMemPool()->deallocate((void*)theData);
//  theData = (vjPOS_DATA*)allocate((theTransmitter-1)*3*sizeof(vjPOS_DATA));
      theData = (vjMatrix*) new vjMatrix[(numBirds+1)*3];

      current = 0;
      valid = 1;
      progress = 2;

      vjDEBUG(0) << "    Getting flock ready....\n" << vjDEBUG_FLUSH;

      port_id = open_port(sPort, baudRate);
      if (port_id == -1) return 0;
      set_blocking(port_id, blocking);
      set_sync(port_id,syncStyle);
      set_group(port_id);
      set_autoconfig(port_id, numBirds);
      set_transmitter(port_id, theTransmitter);
      set_filter(port_id, filter);
      set_hemisphere(port_id, hemisphere, theTransmitter, numBirds);
      set_pos_angles(port_id, theTransmitter, numBirds);
      pickBird(theTransmitter,port_id);
      set_rep_and_stream(port_id, repRate);

//  cout << "port_id:" << port_id << endl;

      vjDEBUG(0)  << "vjFlock ready to go.." << endl << vjDEBUG_FLUSH;

      vjFlock* devicePtr = this;

      myThread = new vjThread(SampleBirds, (void*) devicePtr, 0);

      if ( myThread == NULL ) {
        return 0;	// Fail
      } else {
        return 1;
      }
   }
   else return 0; // already sampling

}

int vjFlock::Sample()
{
     int i;
     int tmp;

     for(i=1; i < (numBirds+1); i++)
     {
	if (i == theTransmitter)
		continue;

      	int index = progress*(numBirds+1)+i-1;
	// Sets index to current read buffer

         getReading(i, &theData[index], port_id);	
      	//Position_Correct(theData[index].pos[0],
      	//		           theData[index].pos[1],
      	//		           theData[index].pos[2]);
      	
         if(i==1)
         {
            //cout << "orig pt:" << theData[index].pos;
         }

         //theData[index].pos.xformFull(xformMat, theData[index].pos );
      	
         if(i==1)
         {
               //cout << "\tnew pt:" << theData[index].pos << flush;
         }

         if(i==1)
         {
             //cout << "\torig or:" << theData[index].orient;
         }


            // Transforms between the cord frames
            // See transform documentation and VR System pg 146
         vjMatrix world_T_transmitter, transmitter_T_reciever, world_T_reciever;

         //world_T_transmitter = rotMat;                   // Set transmitter offset from local info
         world_T_transmitter = xformMat;                   // Set transmitter offset from local info
         transmitter_T_reciever = theData[index];        // Get reciever data from sampled data
         world_T_reciever.mult(world_T_transmitter, transmitter_T_reciever);   // compute total transform
         theData[index] = world_T_reciever;                                     // Store corrected xform back into data

         if(i==1)
         {
                /*
                cout << "\nnew or:" << theData[index].orient << flush;

                new_dir.xformVec(transmitter_or_reciever, tracker_base_dir);
                cout << "   Base Tracker dir: " << tracker_base_dir << endl;
                cout << "\nXformed Tracker dir (in T): " << new_dir << endl;

                new_dir.xformVec(c2_or_reciever, tracker_base_dir);
                cout << "\nXformed Tracker dir: (in W)" << new_dir << endl;
                */
         }
     }
	
     lock.acquire();
     tmp = valid;
     valid = progress;
     progress = tmp;
     lock.release();

     return 1;
}

int vjFlock::StopSampling()
{
   if (myThread != NULL)
   {
      myThread->kill();
      delete myThread;
      myThread = NULL;

      sginap(1);
      char   bird_command[4];

      vjDEBUG(0) << "Stopping the flock..." << vjDEBUG_FLUSH;

      bird_command[0] = 'B';
      write(port_id, bird_command, 1);
      ioctl(port_id, TCFLSH, (char *) 0);
      sginap(5);
      bird_command[0] = 'G';
      write(port_id, bird_command, 1);
      ioctl(port_id, TCFLSH, (char *) 0);
      sleep(2);
      close(port_id);
      port_id = -1;

      vjDEBUG(0) << "stopped." << endl << vjDEBUG_FLUSH;
   }
   return 1;
}

vjMatrix* vjFlock::GetPosData( int d) // d is 0 based
{
  return (&theData[current*(numBirds+1)+d]);
}

void vjFlock::UpdateData()
{
  // swap the indicies for the pointers

  lock.acquire();
  int tmp = current;
  current = valid;
  valid = tmp;
  lock.release();

  return;
}

void vjFlock::SetHemisphere(BIRD_HEMI h)
{
  if (active) {
      vjDEBUG(0) << "Cannot change the hemisphere\n" << vjDEBUG_FLUSH;
      return;
   }
   hemisphere = h;
}

void vjFlock::SetFilters(BIRD_FILT f)
{
  if (active) {
      vjDEBUG(0) << "Cannot change filters while active\n" << vjDEBUG_FLUSH;
      return;
  }
  filter = f;
}

void vjFlock::SetReportRate(char rRate)
{
  if (active) {
      vjDEBUG(0) << "Cannot change report rate while active\n" << vjDEBUG_FLUSH;
      return;
  }
  repRate = rRate;
}

void vjFlock::SetTransmitter(int Transmit)
{
  if (active) {
      vjDEBUG(0) << "Cannot change transmitter while active\n" << vjDEBUG_FLUSH;
      return;
  }
  theTransmitter = Transmit;
}
void vjFlock::SetNumBirds(int n)
{
  if (active) {
      vjDEBUG(0) << "Cannot change num birds while active\n" << vjDEBUG_FLUSH;
      return;
  }
  numBirds = n;
}
void vjFlock::SetSync(int sync)
{
  if (active) {
      vjDEBUG(0) << "Cannot change report rate while active\n" << vjDEBUG_FLUSH;
      return;
  }
  syncStyle = sync;
}

void vjFlock::SetBlocking(int blVal)
{
  if (active) {
      vjDEBUG(0) << "Cannot change report rate while active\n" << vjDEBUG_FLUSH;
      return;
  }
  blocking = blVal;
}

void vjFlock::Position_Correct(float&x,float&y,float&z) {
  int xlo,ylo,zlo,xhi,yhi,zhi;
  float a,b,c,a1,b1,c1;

 /*   if(x<caltable.xmin || y<caltable.ymin || z<caltable.zmin ||
        x>=caltable.xmax || y>=caltable.ymax || z>=caltable.zmax){
        cerr << "Point out of range " << x << " " <<  y << " " << z << endl;
        cerr <<"                   "
             <<caltable.xmin<<" "<< caltable.xmax << " " <<  caltable.ymin
             <<caltable.ymax<<" "<< caltable.zmin << " " << caltable.zmax
             << endl;
        return;
    }
*/
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

void vjFlock::InitCorrectionTable(const char* fName)
{
  int i,j,k, xsize,ysize,zsize;
  float dump;
  ifstream inFile;

  vjDEBUG(0) << "	  Initializing calibration table ... " << endl
             << "	    " << fName << endl << vjDEBUG_FLUSH;
  inFile.open(fName);
  if (!inFile)
  {
	vjDEBUG(0) << "Unable to open calibration.table\n" << vjDEBUG_FLUSH;
        return;
  } else {
     vjDEBUG(0) << "   Calibration table opened sucessfully." << endl << vjDEBUG_FLUSH;
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
// Local functions to vjFlock.cpp
//////////////////////////////////////////////////////////////////
inline int getReading(int n, vjMatrix* data, int port)
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
         sginap(1);
	 c++;
	}
	
      if (c >= 5000)
         vjDEBUG(0) << "vjFlock: tracker timeout (" << c << ")" << endl << vjDEBUG_FLUSH;

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

inline float rawToFloat(char& r1, char& r2)
{
   // return ((float) (((r1 & 0x7f) << 9) | (r2 & 0x7f) << 2) / 0x7fff);
   short int ival1,ival2,val;
   ival1 = r1 & 0x7f;
   ival2 = r2 & 0x7f;
   val = (ival1 << 9) | ival2<<2;
   return ((float)val) / 0x7fff;
}

inline void  pickBird(int birdID, int port)
{
   char buff = 0xF0 + birdID;
   write(port, &buff, 1);
   ioctl(port, TCFLSH, (char*) 0);
   sginap(1);
}

static int open_port(char* serialPort, int baud)
{
   ///////////////////////////////////////////////////////////////////
   // Open and close the port to reset the tracker, then
   // Open the port
   ///////////////////////////////////////////////////////////////////
   int port_id = open(serialPort, O_RDWR | O_NDELAY);
   if (port_id == -1)
   {
      vjDEBUG(0) << "!!vjFlock port open (1 of 2) failed\n" << vjDEBUG_FLUSH ;
      return port_id;
   }
   else
   {
      vjDEBUG(0) << "vjFlock port open (1 of 2) successfully\n" << vjDEBUG_FLUSH;
   }
   sleep(2);
   close(port_id);
   port_id = open(serialPort,O_RDWR | O_NDELAY);
   if (port_id == -1)
   {
      vjDEBUG(0) << "!!vjFlock port open (2 of 2) failed\n" << vjDEBUG_FLUSH;
      return port_id;
   }
   else
   {
      vjDEBUG(0) << "vjFlock port open (2 of 2) successfully\n" << vjDEBUG_FLUSH;
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

  void set_blocking(int port, int blocking)
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

static void set_sync(int port, int sync)
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


static void set_hemisphere(int port, BIRD_HEMI hem, int transmitter, int numbirds)
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

static void set_rep_and_stream(int port, char repRate)
{
 char buff[1];
 /////////////////////////////////////////////////////////////////
 // Set report rate
 //             Q  Every cycle
 //  buff[0] - 'R' Every other bird cycle
 //             S  every 8 cycles
 //             T  every 32 cycles
 /////////////////////////////////////////////////////////////////
  buff[0] = repRate;
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

static void set_pos_angles(int port, int transmitter, int numbirds)
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

static void set_filter(int port, BIRD_FILT filter)
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

}

static void set_transmitter(int port, int transmitter)
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


static void set_autoconfig(int port, int numbirds)
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

static void set_group(int port)
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
