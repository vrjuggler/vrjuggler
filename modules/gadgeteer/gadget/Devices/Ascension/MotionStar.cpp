/*
 * VRJuggler
 *   Copyright (C) 1997,1998,1999,2000
 *   Iowa State University Research Foundation, Inc.
 *   All Rights Reserved
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
 * File:          $RCSfile: vjMotionStar.cpp,v
 * Date modified: $Date: 2000/05/11 17:57:49
 * Version:       $Revision: 1.9
 * -----------------------------------------------------------------
 */


#include <vjConfig.h>
#include <sys/file.h>
#include <sys/ioctl.h>
#include <sys/time.h>
#include <Input/vjPosition/aMotionStar.h>
#include <Input/vjPosition/vjMotionStar.h>
#include <Math/vjCoord.h>

#include <Kernel/vjDebug.h>

#include <fstream.h>

// Helper to return the index for theData array
// given the birdNum we are dealing with and the bufferIndex
// to get
//! ARGS: birdNum - The number of the bird we care about
//! ARGS:bufferIndex - the value of current, progress, or valid (it is an offset in the array)
// XXX: We are going to say the birds are 0 based

int vjMotionStar::getBirdIndex(int birdNum, int bufferIndex)
{
   int ret_val = (birdNum*3)+bufferIndex;
   vjASSERT((ret_val >= 0) && (ret_val < ((getNumBirds()+1)*3)));
   //assertIndexes();   // Can't assert here because it is possible the indexes are switching right now
   return ret_val;
}

vjMotionStar::vjMotionStar(char* ipAddress,
                int hemisphere,
                unsigned int birdFormat,
                unsigned int birdsRequired,
                int runMode,
                double birdRate
//              unsigned char reportRate
                ) : mMotionStar(ipAddress,
                                hemisphere,
                                birdFormat,
                                birdsRequired,
                                runMode,
                                birdRate,
                                1 // reportRate
                                )
{
   myThread = NULL;
}

bool vjMotionStar::config(vjConfigChunk *c)
{

   if (!vjPosition::config(c))
       return false;
   vjDEBUG(vjDBG_INPUT_MGR,3) << "       vjMotionStar::vjMotionStar(vjConfigChunk*)" << endl << vjDEBUG_FLUSH;

   // set mMotionStar with the config info.
   mMotionStar.setHemisphere((unsigned int)(static_cast<int> (c->getProperty("hemisphere"))));
   mMotionStar.setBirdFormat((unsigned int)(static_cast<int> (c->getProperty("bformat"))));
   mMotionStar.setNumBirds((unsigned int)(static_cast<int> (c->getProperty("num"))));
   mMotionStar.setBirdRate((double)(static_cast<int>(c->getProperty("brate"))));
   mMotionStar.setRunMode((static_cast<int> (c->getProperty("mode"))));
   mMotionStar.setAddress(static_cast<std::string>(c->getProperty("address")).c_str());
//   mMotionStar.setReportRate( (static_cast<unsigned char>(c->getProperty("rrate"))) ); 
 
/*
   // output what was read.
   vjDEBUG(vjDBG_INPUT_MGR,1) << "        Flock Settings: " << endl
      << "              aFlock::getTransmitter(): " << mFlockOfBirds.getTransmitter() << endl
      << "             aFlock::getNumBirds()      : " << mFlockOfBirds.getNumBirds() << endl
      << "              aFlock::getBaudRate()      : " << mFlockOfBirds.getBaudRate() << endl
      << "              deviceAbilities:" << deviceAbilities << endl
      << "              aFlock::getPort()         : " << mFlockOfBirds.getPort() << endl
      << "              instance name : " << instName << endl
      << endl << vjDEBUG_FLUSH;
*/

   return true;
}

vjMotionStar::~vjMotionStar()
{
   this->stopSampling();
   if (theData != NULL)
      getMyMemPool()->deallocate((void*)theData);
}

static void sampleBirds(void* pointer)
{
   vjDEBUG(vjDBG_INPUT_MGR,3) << "vjMotionStar: Spawned SampleBirds starting" << endl
 << vjDEBUG_FLUSH;

   vjMotionStar* devPointer = (vjMotionStar*) pointer;
   for (;;)
   {
      devPointer->sample();
   }
}

int vjMotionStar::startSampling()
{
   // make sure birds aren't already started
   if (this->isActive() == true)
   {
      vjDEBUG(vjDBG_INPUT_MGR,2)  << "vjMotionStar was already started." << endl << vjDEBUG_FLUSH;
      return 0;
   }

   if (myThread == NULL)
   {
      if (theData != NULL)
         getMyMemPool()->deallocate((void*)theData);

      // Allocate buffer space for birds
      int numbuffs = (mMotionStar.getNumBirds()+1)*3;
      theData = (vjMatrix*) new vjMatrix[numbuffs];

      // Reset current, progress, and valid indices
      resetIndexes();

         vjDEBUG(vjDBG_INPUT_MGR,1) << "    Getting flock ready....\n" << vjDEBUG_FLUSH;
      if ( mMotionStar.start() == -1 ) {
         vjDEBUG(vjDBG_INPUT_MGR, 0) << "vjMotionStar failed to connect to server"
                                     << endl << vjDEBUG_FLUSH;
         return 0;
      }

      //sanity check.. make sure birds actually started
      if (this->isActive() == false)
      {
         vjDEBUG(vjDBG_INPUT_MGR,0)  << "vjMotionStar failed to start.." << endl << vjDEBUG_FLUSH;
         return 0;
      }

      vjDEBUG(vjDBG_INPUT_MGR,1)  << "vjMotionStar ready to go.." << endl << vjDEBUG_FLUSH;

      vjMotionStar* devicePtr = this;

      myThread = new vjThread(sampleBirds, (void*) devicePtr, 0);

      if ( myThread == NULL )
      {
          assert (false);
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

int vjMotionStar::sample()
{
   if (this->isActive() == false)
   {
       vjDEBUG(vjDBG_ALL,0) << "NOT ACTIVE IN SAMPLE" << endl << vjDEBUG_FLUSH;
       return 0;
   }
   mMotionStar.sample();
   int i;

   // For each bird
   for (i=0; i < (mMotionStar.getNumBirds()); i++)

      {
      // Get the index to the current read buffer
      int index = getBirdIndex(i,progress);

      //if (i==0)
      //vjDEBUG(vjDBG_ALL,0) << "i:  " << i << "  x: " << xPos(i) << "  y: " << yPos(i) << endl << vjDEBUG_FLUSH;

      theData[index].makeZYXEuler(mMotionStar.zRot( i ),
                                  mMotionStar.yRot( i ),
                                  mMotionStar.xRot( i ));

      theData[index].setTrans(mMotionStar.xPos( i ),
                              mMotionStar.yPos( i ),
                              mMotionStar.zPos( i ));

//      if (i==1)
//         vjDEBUG(vjDBG_ALL,2) << "Flock: bird1:    orig:" << vjCoord(theData[index]).pos << endl << vjDEBUG_FLUSH;

      // Transforms between the cord frames
      // See transform documentation and VR System pg 146
      // Since we want the reciver in the world system, Rw
      // wTr = wTt*tTr
      vjMatrix world_T_transmitter, transmitter_T_reciever, world_T_reciever;

      world_T_transmitter = xformMat;                    // Set transmitter offset from local info
      transmitter_T_reciever = theData[index];           // Get reciever data from sampled data
      world_T_reciever.mult(world_T_transmitter, transmitter_T_reciever);   // compute total transform
      theData[index] = world_T_reciever;                                     //Store corrected xform back into data


//      if (i == 1)
//         vjDEBUG(vjDBG_ALL,2) << "Flock: bird1: xformed:" << vjCoord(theData[index]).pos << endl << vjDEBUG_FLUSH;
   }

   // Locks and then swaps the indices
   swapValidIndexes();

   return 1;
}

int vjMotionStar::stopSampling()
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

      mMotionStar.stop();

      // sanity check: did the flock actually stop?
      if (this->isActive() == true)
      {
         vjDEBUG(vjDBG_INPUT_MGR,0) << "Flock didn't stop." << endl << vjDEBUG_FLUSH;
         return 0;
      }

      vjDEBUG(vjDBG_INPUT_MGR,1) << "stopped." << endl << vjDEBUG_FLUSH;
   }

   return 1;
}

vjMatrix* vjMotionStar::getPosData( int d ) // d is 0 based
{
    if (this->isActive() == false)
    {
        vjDEBUG(vjDBG_ALL,0) << "Not active in getPosData()" << endl << vjDEBUG_FLUSH;
        return NULL;
    }
    return (&theData[getBirdIndex(d,current)]);
}

vjTimeStamp* vjMotionStar::getPosUpdateTime (int d)
{
    return (&mDataTimes[getBirdIndex(d,current)]);
}

void vjMotionStar::updateData()
{
   if (this->isActive() == false)
   {
      vjDEBUG(vjDBG_ALL,0) << "Not active in updateData()" << endl << vjDEBUG_FLUSH;
      return;
   }
   vjGuard<vjMutex> updateGuard(lock);

   // Copy the valid data to the current data so that both are valid
   for(int i=0;i<getNumBirds();i++)
      theData[getBirdIndex(i,current)] = theData[getBirdIndex(i,valid)];   // first hand

   // Locks and then swap the indices
   swapCurrentIndexes();

   return;
}



void vjMotionStar::setHemisphere ( int i)
{
    if (this->isActive())
    {
      vjDEBUG(vjDBG_INPUT_MGR,2) << "vjMotionStar: Cannot change hemisphere while active\n" << vjDEBUG_FLUSH;
      return;
    }
    mMotionStar.setHemisphere ( i );
}


void vjMotionStar::setBirdFormat (unsigned int i)
{
    if (this->isActive())
    {
      vjDEBUG(vjDBG_INPUT_MGR,2) << "vjMotionStar: Cannot change format while active\n" << vjDEBUG_FLUSH;
      return;
    }
    mMotionStar.setBirdFormat ( i );
}


void vjMotionStar::setNumBirds (unsigned int i)
{
    if (this->isActive())
    {
      vjDEBUG(vjDBG_INPUT_MGR,2) << "vjMotionStar: Cannot change number of birds while active\n" << vjDEBUG_FLUSH;
      return;
    }
    mMotionStar.setNumBirds ( i );
}


void vjMotionStar::setBirdRate (double d)
{
    if (this->isActive())
    {
      vjDEBUG(vjDBG_INPUT_MGR,2) << "vjMotionStar: Cannot change sample rate while active\n" << vjDEBUG_FLUSH;
      return;
    }
    mMotionStar.setBirdRate ( d );
}


void vjMotionStar::setRunMode (int i)
{
    if (this->isActive())
    {
      vjDEBUG(vjDBG_INPUT_MGR,2) << "vjMotionStar: Cannot change run mode while active\n" << vjDEBUG_FLUSH;
      return;
    }
    mMotionStar.setRunMode ( i );
}


void vjMotionStar::setReportRate (unsigned char ch)
{
    if (this->isActive())
    {
      vjDEBUG(vjDBG_INPUT_MGR,2) << "vjMotionStar: Cannot change report rate while active\n" << vjDEBUG_FLUSH;
      return;
    }
    mMotionStar.setReportRate ( ch );
}

void vjMotionStar::setAddress (const char* n)    
{
    if (this->isActive())
    {
      vjDEBUG(vjDBG_INPUT_MGR,2) << "vjMotionStar: Cannot change ip address while active\n" << vjDEBUG_FLUSH;
      return;
    }
    mMotionStar.setAddress ( n );
}
