#include <vjConfig.h>
#include <sys/file.h>
#include <sys/ioctl.h>
#include <sys/time.h>
#include <Input/vjPosition/aFlock.h>
#include <Input/vjPosition/vjFlock.h>

#include <Kernel/vjDebug.h>

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
//   report - flock report rate.                         <BR>
//   calfile - a calibration file, if "", then use none. <BR>
//                                                       <BR>
// Result: configures internal data members, 
//         doesn't actually talk to the FOB yet.
vjFlock::vjFlock(const char* const port, 
		const int& baud, 
		const int& sync, 
		const int& block, 
		const int& numBrds, 
		const int& transmit, 
		const BIRD_HEMI& hemi, 
		const BIRD_FILT& filt, 
		const char& report, 
		const char* const calfile) : mFlockOfBirds(port, 
						    baud, 
						    sync, 
						    block, 
						    numBrds, 
						    transmit, 
						    hemi, 
						    filt, 
						    report, 
						    calfile)
{
}

bool vjFlock::config(vjConfigChunk *c)
{
    port_id = -1;
    myThread = NULL;
    
    vjDEBUG(0) << "	 vjFlock::vjFlock(vjConfigChunk*)" << endl << vjDEBUG_FLUSH;
    
    // read in vjPosition's config stuff,
    // --> this will be the port and baud fields
    if (!vjPosition::config(c))
     return false;
    
    // keep aFlock's port and baud members in sync with vjInput's port and baud members.
    mFlockOfBirds.setPort( vjInput::GetPort() );
    mFlockOfBirds.setBaudRate( vjInput::GetBaudRate() );
    
    // set mFlockOfBirds with the config info.
    mFlockOfBirds.setSync( static_cast<int>(c->getProperty("sync")) );
    mFlockOfBirds.setBlocking( static_cast<int>(c->getProperty("blocking")) );
    mFlockOfBirds.setNumBirds( static_cast<int>(c->getProperty("num")) );
    mFlockOfBirds.setTransmitter( static_cast<int>(c->getProperty("transmitter")) );
    mFlockOfBirds.setHemisphere( (BIRD_HEMI)(static_cast<int>(c->getProperty("hemi"))) ); //LOWER_HEMI
    mFlockOfBirds.setFilterType( (BIRD_FILT)(static_cast<int>(c->getProperty("filt"))) ); //

    // sanity check the report rate
    char r = static_cast<char*>(c->getProperty("report"))[0];
    if ((r != 'Q') && (r != 'R') &&
      (r != 'S') && (r != 'T'))
    {
	vjDEBUG(0)  << "   illegal report rate from configChunk, defaulting to every other cycle (R)" << endl << vjDEBUG_FLUSH;
	mFlockOfBirds.setReportRate( 'R' );
    }
    else
	mFlockOfBirds.setReportRate( r );
    
    // output what was read.
    vjDEBUG(0) << "	  Flock Settings: " << endl
	       << "	        aFlock::getTransmitter(): " << mFlockOfBirds.getTransmitter() << endl
		    << "             aFlock::getNumBirds()      : " << mFlockOfBirds.getNumBirds() << endl
	       << "	        aFlock::getBaudRate()      : " << mFlockOfBirds.getBaudRate() << endl
	       << "	        deviceAbilities:" << deviceAbilities << endl
	       << "	        aFlock::getPort()         : " << mFlockOfBirds.getPort() << endl
	       << "		instance name : " << instName << endl
	       << endl << vjDEBUG_FLUSH;
    
    // init the correction table with the calibration file.
    mFlockOfBirds.initCorrectionTable( c->getProperty("calfile") );
 
    return true;
}

vjFlock::~vjFlock()
{
    vjDEBUG(0)  << "	vjFlock::~vjFlock()" << endl << vjDEBUG_FLUSH;
    this->StopSampling();
    if (theData != NULL)
       getMyMemPool()->deallocate((void*)theData);
}

static void SampleBirds(void* pointer)
{
   vjDEBUG(0) << "vjFlock: Spawned SampleBirds starting" << endl << vjDEBUG_FLUSH;

   vjFlock* devPointer = (vjFlock*) pointer;
   for (;;)
   {
         devPointer->Sample();
   }
}

int vjFlock::StartSampling()
{
    // make sure birds aren't already started
    if (this->isActive() == true)
    {
	vjDEBUG(0)  << "vjFlock was already started." << endl << vjDEBUG_FLUSH;
	return 0;
    } 
    
    if (myThread == NULL)
    {
	if (theData != NULL)
	getMyMemPool()->deallocate((void*)theData);
	
	theData = (vjMatrix*) new vjMatrix[(mFlockOfBirds.getNumBirds()+1)*3];
	
	current = 0;
	valid = 1;
	progress = 2;
	
	vjDEBUG(0) << "    Getting flock ready....\n" << vjDEBUG_FLUSH;
	
	
	mFlockOfBirds.start();
	
	//sanity check.. make sure birds actually started
	if (this->isActive() == false)
	{
	    vjDEBUG(0)  << "vjFlock failed to start.." << endl << vjDEBUG_FLUSH;
	    return 0;
	} 
	
	vjDEBUG(0)  << "vjFlock ready to go.." << endl << vjDEBUG_FLUSH;
	
	vjFlock* devicePtr = this;
	
	myThread = new vjThread(SampleBirds, (void*) devicePtr, 0);
	
	if ( myThread == NULL ) 
	{
	    return 0;	// Fail
	} else {
	    return 1;   // success
	}
    } else 
	return 0; // already sampling

}

int vjFlock::Sample()
{
     if (this->isActive() == false)
     	return 0;
     
     int i;
     int tmp;

    mFlockOfBirds.sample();
	
     for (i=1; i < (mFlockOfBirds.getNumBirds()+1); i++)
     {
	if (i == mFlockOfBirds.getTransmitter())
		continue;

      	int index = progress*(mFlockOfBirds.getNumBirds()+1)+i-1;
	// Sets index to current read buffer

      	theData[index].makeZYXEuler(mFlockOfBirds.zRot( i ), 
				mFlockOfBirds.yRot( i ), 
				mFlockOfBirds.xRot( i ));
   	theData[index].setTrans(mFlockOfBirds.xPos( i ),
				mFlockOfBirds.yPos( i ), 
				mFlockOfBirds.zPos( i ));
      	
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
    if (this->isActive() == false)
	return 0;
    
    if (myThread != NULL)
    {
	vjDEBUG(0) << "Stopping the flock thread..." << vjDEBUG_FLUSH;
	
	myThread->kill();
	delete myThread;
	myThread = NULL;
	
	vjDEBUG(0) << "  Stopping the flock..." << vjDEBUG_FLUSH;
	
	mFlockOfBirds.stop();
	
	// sanity check: did the flock actually stop?
	if (this->isActive() == true)
	{
	    vjDEBUG(0) << "Flock didn't stop." << endl << vjDEBUG_FLUSH;
	    return 0;
	}
	
	vjDEBUG(0) << "stopped." << endl << vjDEBUG_FLUSH;
    }
    
    return 1;
}

vjMatrix* vjFlock::GetPosData( int d ) // d is 0 based
{
  if (this->isActive() == false)
	return NULL;
	
  return (&theData[current*(mFlockOfBirds.getNumBirds()+1)+d]);
}

void vjFlock::UpdateData()
{
    if (this->isActive() == false)
	return;
	
  // swap the indicies for the pointers

  lock.acquire();
  int tmp = current;
  current = valid;
  valid = tmp;
  lock.release();

  return;
}

void vjFlock::setHemisphere(const BIRD_HEMI& h)
{
    if (this->isActive()) 
    {
	vjDEBUG(0) << "Cannot change the hemisphere while active\n" << vjDEBUG_FLUSH;
	return;
    }
    mFlockOfBirds.setHemisphere( h );
}

void vjFlock::setFilterType(const BIRD_FILT& f)
{
  if (this->isActive()) 
  {
      vjDEBUG(0) << "Cannot change filters while active\n" << vjDEBUG_FLUSH;
      return;
  }
  mFlockOfBirds.setFilterType( f );
}

void vjFlock::setReportRate(const char& rRate)
{
  if (this->isActive()) 
  {
      vjDEBUG(0) << "Cannot change report rate while active\n" << vjDEBUG_FLUSH;
      return;
  }
  mFlockOfBirds.setReportRate( rRate );
}

void vjFlock::setTransmitter(const int& Transmit)
{
  if (this->isActive()) 
  {
      vjDEBUG(0) << "Cannot change transmitter while active\n" << vjDEBUG_FLUSH;
      return;
  }
  mFlockOfBirds.setTransmitter( Transmit );
}
void vjFlock::setNumBirds(const int& n)
{
  if (this->isActive()) 
  {
      vjDEBUG(0) << "Cannot change num birds while active\n" << vjDEBUG_FLUSH;
      return;
  }
  mFlockOfBirds.setNumBirds( n );
}
void vjFlock::setSync(const int& sync)
{
  if (this->isActive()) 
  {
      vjDEBUG(0) << "Cannot change report rate while active\n" << vjDEBUG_FLUSH;
      return;
  }
  mFlockOfBirds.setSync( sync );
}

void vjFlock::setBlocking(const int& blVal)
{
  if (this->isActive()) 
  {
      vjDEBUG(0) << "Cannot change report rate while active\n" << vjDEBUG_FLUSH;
      return;
  }
  mFlockOfBirds.setBlocking( blVal );
}

//: set the port to use
//  this will be a string in the form of the native OS descriptor <BR>
//  ex: unix - "/dev/ttyd3", win32 - "COM3" <BR>
//  NOTE: flock.isActive() must be false to use this function
void vjFlock::setPort( const char* const serialPort )
{
    if (this->isActive()) 
    {
      vjDEBUG(0) << "Cannot change port while active\n" << vjDEBUG_FLUSH;
      return;
    }
    mFlockOfBirds.setPort( serialPort );
    
    // keep vjInput's port and baud members in sync 
    // with aFlock's port and baud members.
    vjInput::SetPort( serialPort );
}

//: set the baud rate
//  this is generally 38400, consult flock manual for other rates. <BR>
//  NOTE: flock.isActive() must be false to use this function
void vjFlock::setBaudRate( const int& baud )
{
    if (this->isActive()) 
    {
      vjDEBUG(0) << "Cannot change baud rate while active\n" << vjDEBUG_FLUSH;
      return;
    }
    mFlockOfBirds.setBaudRate( baud );
    
    // keep vjInput's port and baud members in sync 
    // with aFlock's port and baud members.
    vjInput::SetBaudRate( baud );
}
