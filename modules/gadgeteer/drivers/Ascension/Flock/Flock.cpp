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


//===============================================================
// vjFlock (a Wrapper for aFlock)
//
// Purpose:
//      VR Juggler Ascention Flock of birds tracking class
//
// Author:
// Kevin Meinert
//
// Date: 4-22-99
//===============================================================
#include <vjConfig.h>
#include <sys/file.h>
#include <sys/ioctl.h>
#include <sys/time.h>

#include <Math/vjCoord.h>
#include <Utils/vjDebug.h>
#include <Utils/vjFileIO.h>
#include <Input/vjPosition/aFlock.h>
#include <Config/vjConfigChunk.h>
#include <vpr/System.h>
#include <Input/vjPosition/vjFlock.h>

// Helper to return the index for theData array
// given the birdNum we are dealing with and the bufferIndex
// to get
//! ARGS: birdNum - The number of the bird we care about
//! ARGS:bufferIndex - the value of current, progress, or valid (it is an offset in the array)
// XXX: We are going to say the birds are 0 based
int vjFlock::getBirdIndex(int birdNum, int bufferIndex)
{
   int ret_val = (birdNum*3)+bufferIndex;
   vjASSERT((ret_val >= 0) && (ret_val < ((getNumBirds()+1)*3)));
   //assertIndexes();   // Can't assert here because it is possible the indexes are switching right now
   return ret_val;
}

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
   myThread = NULL;
}

bool vjFlock::config(vjConfigChunk *c)
{
   port_id = -1;

   vjDEBUG(vjDBG_INPUT_MGR,3) << "   vjFlock::vjFlock(vjConfigChunk*)"
                              << std::endl << vjDEBUG_FLUSH;

   // read in vjPosition's config stuff,
   // --> this will be the port and baud fields
   if(! (vjInput::config(c) && vjPosition::config(c)))
      return false;

   // keep aFlock's port and baud members in sync with vjInput's port and baud members.
   mFlockOfBirds.setPort( vjInput::getPort() );
   mFlockOfBirds.setBaudRate( vjInput::getBaudRate() );

   // set mFlockOfBirds with the config info.
   mFlockOfBirds.setSync( static_cast<int>(c->getProperty("sync")) );
   mFlockOfBirds.setBlocking( static_cast<int>(c->getProperty("blocking")) );
   mFlockOfBirds.setNumBirds( static_cast<int>(c->getProperty("num")) );
   mFlockOfBirds.setTransmitter( static_cast<int>(c->getProperty("transmitter")) );
   mFlockOfBirds.setHemisphere( (BIRD_HEMI)(static_cast<int>(c->getProperty("hemi"))) ); //LOWER_HEMI
   mFlockOfBirds.setFilterType( (BIRD_FILT)(static_cast<int>(c->getProperty("filt"))) ); //

   // sanity check the report rate
   char r = static_cast<std::string>(c->getProperty("report")).c_str()[0];
   if ((r != 'Q') && (r != 'R') &&
       (r != 'S') && (r != 'T'))
   {
      vjDEBUG(vjDBG_INPUT_MGR,1)
         << "   illegal report rate from configChunk, defaulting to every other cycle (R)"
         << std::endl << vjDEBUG_FLUSH;
      mFlockOfBirds.setReportRate( 'R' );
   }
   else
      mFlockOfBirds.setReportRate( r );

   // output what was read.
   vjDEBUG(vjDBG_INPUT_MGR,1) << "    Flock Settings: " << std::endl
      << "          aFlock::getTransmitter(): " << mFlockOfBirds.getTransmitter() << std::endl
      << "             aFlock::getNumBirds()      : " << mFlockOfBirds.getNumBirds() << std::endl
      << "          aFlock::getBaudRate()      : " << mFlockOfBirds.getBaudRate() << std::endl
      << "          aFlock::getPort()         : " << mFlockOfBirds.getPort() << std::endl
      << "     instance name : " << instName << std::endl
      << std::endl << vjDEBUG_FLUSH;

   // init the correction table with the calibration file.
   char* calfile = c->getProperty("calfile").cstring();
   mFlockOfBirds.initCorrectionTable(vjFileIO::replaceEnvVars(calfile).c_str());

   return true;
}

vjFlock::~vjFlock()
{
   this->stopSampling();
   if (theData != NULL)
      delete theData;
      //getMyMemPool()->deallocate((void*)theData);
   if (mDataTimes != NULL)
      delete mDataTimes;
}

void vjFlock::controlLoop(void* nullParam)
{
   // vjDEBUG(vjDBG_INPUT_MGR,3) << "vjFlock: Spawned SampleBirds starting"
   //                            << std::endl << vjDEBUG_FLUSH;

   for (;;)
   {
      this->sample();
   }
}

int vjFlock::startSampling()
{
   // make sure birds aren't already started
   if (this->isActive() == true)
   {
      vjDEBUG(vjDBG_INPUT_MGR,2) << "vjFlock was already started."
                                 << std::endl << vjDEBUG_FLUSH;
      return 0;
   }

   if (myThread == NULL)
   {
      if (theData != NULL)
         delete theData;
      //   getMyMemPool()->deallocate((void*)theData);
      if (mDataTimes != NULL)
         delete mDataTimes;

      // Allocate buffer space for birds
      int numbuffs = (mFlockOfBirds.getNumBirds()+1)*3;
      theData = new vjMatrix[numbuffs];
      mDataTimes = new vjTimeStamp[numbuffs];

      // Reset current, progress, and valid indices
      resetIndexes();

         vjDEBUG(vjDBG_INPUT_MGR,1) << "    Getting flock ready....\n" << vjDEBUG_FLUSH;
      mFlockOfBirds.start();

      //sanity check.. make sure birds actually started
      if (this->isActive() == false)
      {
         vjDEBUG(vjDBG_INPUT_MGR,0) << "vjFlock failed to start.."
                                    << std::endl << vjDEBUG_FLUSH;
         return 0;
      }

      vjDEBUG(vjDBG_INPUT_MGR,1) << "vjFlock ready to go.." << std::endl
                                 << vjDEBUG_FLUSH;

      vjFlock* devicePtr = this;

      // Create a new thread to handle the control
      vpr::ThreadMemberFunctor<vjFlock>* memberFunctor =
          new vpr::ThreadMemberFunctor<vjFlock>(this, &vjFlock::controlLoop, NULL);
      vpr::Thread* new_thread;
      new_thread = new vpr::Thread(memberFunctor);
      myThread = new_thread;

      if ( myThread == NULL )
      {
         return 0;  // Fail
      }
      else
      {
         return 1;   // success
      }
   }
   else
      return 0; // already sampling
}

int vjFlock::sample()
{
   if (this->isActive() == false)
      return 0;

   int i;
      vjTimeStamp sampletime;
   sampletime.set();
   mFlockOfBirds.sample();

   vpr::Thread::yield();

   // For each bird
   for (i=0; i < (mFlockOfBirds.getNumBirds()); i++)
   {
      // Get the index to the current read buffer
      int index = getBirdIndex(i,progress);


      // We add 1 to "i" to account for the fact that aFlock is 1-based
      theData[index].makeZYXEuler(mFlockOfBirds.zRot( i+1 ),
                                  mFlockOfBirds.yRot( i+1 ),
                                  mFlockOfBirds.xRot( i+1 ));

      theData[index].setTrans(mFlockOfBirds.xPos( i+1 ),
                              mFlockOfBirds.yPos( i+1 ),
                              mFlockOfBirds.zPos( i+1 ));
      mDataTimes[index] = sampletime;


      //if (i==1)
         //vjDEBUG(vjDBG_ALL,2) << "Flock: bird1:    orig:" << vjCoord(theData[index]).pos << std::endl << vjDEBUG_FLUSH;

      // Transforms between the cord frames
      // See transform documentation and VR System pg 146
      // Since we want the reciver in the world system, Rw
      // wTr = wTt*tTr
      vjMatrix world_T_transmitter, transmitter_T_reciever, world_T_reciever;

      world_T_transmitter = xformMat;                    // Set transmitter offset from local info
      transmitter_T_reciever = theData[index];           // Get reciever data from sampled data
      world_T_reciever.mult(world_T_transmitter, transmitter_T_reciever);   // compute total transform
      theData[index] = world_T_reciever;                                     // Store corrected xform back into data


      //if (i == 1)
         //vjDEBUG(vjDBG_ALL,2) << "Flock: bird1: xformed:" << vjCoord(theData[index]).pos << std::endl << vjDEBUG_FLUSH;
   }

   // Locks and then swaps the indices
   swapValidIndexes();

   return 1;
}

int vjFlock::stopSampling()
{
   if (this->isActive() == false)
      return 0;

   if (myThread != NULL)
   {
      vjDEBUG(vjDBG_INPUT_MGR,1) << "Stopping the flock thread..." << vjDEBUG_FLUSH;

      myThread->kill();
      delete myThread;
      myThread = NULL;

      vjDEBUG(vjDBG_INPUT_MGR,1) << "  Stopping the flock..." << vjDEBUG_FLUSH;

      mFlockOfBirds.stop();

      // sanity check: did the flock actually stop?
      if (this->isActive() == true)
      {
         vjDEBUG(vjDBG_INPUT_MGR,0) << "Flock didn't stop." << std::endl
                                    << vjDEBUG_FLUSH;
         return 0;
      }

      vjDEBUG(vjDBG_INPUT_MGR,1) << "stopped." << std::endl << vjDEBUG_FLUSH;
   }

   return 1;
}

vjMatrix* vjFlock::getPosData( int d ) // d is 0 based
{
    if (this->isActive() == false)
      return NULL;

    return (&theData[getBirdIndex(d,current)]);
}

vjTimeStamp* vjFlock::getPosUpdateTime (int d) {
    if (this->isActive() == false)
      return NULL;

    return (&mDataTimes[getBirdIndex(d,current)]);
}

void vjFlock::updateData()
{
   if (this->isActive() == false)
      return;

   // this unlocks when this object is destructed (upon return of the function)
   vpr::Guard<vpr::Mutex> updateGuard(lock);

   // Copy the valid data to the current data so that both are valid
   for(int i=0;i<getNumBirds();i++)
      theData[getBirdIndex(i,current)] = theData[getBirdIndex(i,valid)];   // first hand

   // Locks and then swap the indicies
   swapCurrentIndexes();

   return;
}

void vjFlock::setHemisphere(const BIRD_HEMI& h)
{
   if (this->isActive())
   {
      vjDEBUG(vjDBG_INPUT_MGR,2) << "vjFlock: Cannot change the hemisphere while active\n" << vjDEBUG_FLUSH;
      return;
   }
   mFlockOfBirds.setHemisphere( h );
}

void vjFlock::setFilterType(const BIRD_FILT& f)
{
   if (this->isActive())
   {
      vjDEBUG(vjDBG_INPUT_MGR,2) << "vjFlock: Cannot change filters while active\n" << vjDEBUG_FLUSH;
      return;
   }
   mFlockOfBirds.setFilterType( f );
}

void vjFlock::setReportRate(const char& rRate)
{
  if (this->isActive())
  {
      vjDEBUG(vjDBG_INPUT_MGR,2) << "vjFlock: Cannot change report rate while active\n" << vjDEBUG_FLUSH;
      return;
  }
  mFlockOfBirds.setReportRate( rRate );
}

void vjFlock::setTransmitter(const int& Transmit)
{
  if (this->isActive())
  {
      vjDEBUG(vjDBG_INPUT_MGR,2) << "vjFlock: Cannot change transmitter while active\n" << vjDEBUG_FLUSH;
      return;
  }
  mFlockOfBirds.setTransmitter( Transmit );
}


void vjFlock::setNumBirds(const int& n)
{
  if (this->isActive())
  {
      vjDEBUG(vjDBG_INPUT_MGR,2) << "vjFlock: Cannot change num birds while active\n" << vjDEBUG_FLUSH;
      return;
  }
  mFlockOfBirds.setNumBirds( n );
}


void vjFlock::setSync(const int& sync)
{
  if (this->isActive())
  {
      vjDEBUG(vjDBG_INPUT_MGR,2) << "vjFlock: Cannot change report rate while active\n" << vjDEBUG_FLUSH;
      return;
  }
  mFlockOfBirds.setSync( sync );
}


void vjFlock::setBlocking(const int& blVal)
{
  if (this->isActive())
  {
      vjDEBUG(vjDBG_INPUT_MGR,2) << "vjFlock: Cannot change report rate while active\n" << vjDEBUG_FLUSH;
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
      vjDEBUG(vjDBG_INPUT_MGR,2) << "vjFlock: Cannot change port while active\n" << vjDEBUG_FLUSH;
      return;
    }
    mFlockOfBirds.setPort( serialPort );

    // keep vjInput's port and baud members in sync
    // with aFlock's port and baud members.
    vjInput::setPort( serialPort );
}

//: set the baud rate
//  this is generally 38400, consult flock manual for other rates. <BR>
//  NOTE: flock.isActive() must be false to use this function
void vjFlock::setBaudRate( const int& baud )
{
    if (this->isActive())
    {
      vjDEBUG(vjDBG_INPUT_MGR,2) << "vjFlock: Cannot change baud rate while active\n" << vjDEBUG_FLUSH;
      return;
    }
    mFlockOfBirds.setBaudRate( baud );

    // keep vjInput's port and baud members in sync
    // with aFlock's port and baud members.
    vjInput::setBaudRate( baud );
}
