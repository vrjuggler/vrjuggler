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
 * File:          $RCSfile$
 * Date modified: $Date$
 * Version:       $Revision$
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


// ============================================================================
// Helper functions speccfic to this file.
// ============================================================================

// ----------------------------------------------------------------------------
// Continuously sample the device.
//
// PRE: The given argument is pointer to a valid vjMotionStar object.
// POST: vjMotionStar::sample() is executed repeated until the thread is
//       stopped.
//
// ARGS: arg - A pointer to a vjMotionStar object cast to a void*.
// ----------------------------------------------------------------------------
static void
sampleBirds (void* arg) {
   vjMotionStar* devPointer = (vjMotionStar*) arg;

   vjDEBUG(vjDBG_INPUT_MGR,3) << "vjMotionStar: Spawned SampleBirds starting\n"
                              << vjDEBUG_FLUSH;

   for (;;) {
      devPointer->sample();
   }
}

// ============================================================================
// Public methods.
// ============================================================================

// ----------------------------------------------------------------------------
// Constructor.  This invokes the aMotionStar constructor and initializes
// member variables.
// ----------------------------------------------------------------------------
vjMotionStar::vjMotionStar (char* address, int hemisphere,
                            unsigned int birdFormat,
                            unsigned int birdsRequired, int runMode,
                            double birdRate)
    : mMotionStar(address, hemisphere, birdFormat, birdsRequired, runMode,
                  birdRate, 1)
{
   myThread = NULL;
}

// ----------------------------------------------------------------------------
// Destructor.  Sampling is stopped, and the data pool is deallocated.
// ----------------------------------------------------------------------------
vjMotionStar::~vjMotionStar () {
   this->stopSampling();
   if (theData != NULL)
      getMyMemPool()->deallocate((void*)theData);
}

// ----------------------------------------------------------------------------
// Configure the MotionStar with the given config chunk.
// ----------------------------------------------------------------------------
bool
vjMotionStar::config (vjConfigChunk *c) {
   if (!vjPosition::config(c))
       return false;

   vjDEBUG(vjDBG_INPUT_MGR,3) << "       vjMotionStar::config(vjConfigChunk*)\n"
                              << vjDEBUG_FLUSH;

   // set mMotionStar with the config info.
   mMotionStar.setHemisphere((unsigned int)(static_cast<int> (c->getProperty("hemisphere"))));
   mMotionStar.setBirdFormat((unsigned int)(static_cast<int> (c->getProperty("bformat"))));
   mMotionStar.setNumBirds((unsigned int)(static_cast<int> (c->getProperty("num"))));
   mMotionStar.setBirdRate((double)(static_cast<int>(c->getProperty("brate"))));
   mMotionStar.setRunMode((static_cast<int> (c->getProperty("mode"))));
   mMotionStar.setAddress(static_cast<std::string>(c->getProperty("address")).c_str());
//   mMotionStar.setReportRate( (static_cast<unsigned char>(c->getProperty("rrate"))) ); 
 
   return true;
}

// ----------------------------------------------------------------------------
// Begin sampling.
// ----------------------------------------------------------------------------
int
vjMotionStar::startSampling () {
   // Make sure the device isn't already started.
   if ( this->isActive() == true ) {
      vjDEBUG(vjDBG_INPUT_MGR,2) << "vjMotionStar was already started.\n"
                                 << vjDEBUG_FLUSH;
      return 0;
   }

   if ( myThread == NULL ) {
      if ( theData != NULL ) {
         getMyMemPool()->deallocate((void*)theData);
      }

      // Allocate buffer space for birds
      int numbuffs = (mMotionStar.getNumBirds()+1)*3;
      theData = (vjMatrix*) new vjMatrix[numbuffs];

      // Reset current, progress, and valid indices
      resetIndexes();

      vjDEBUG(vjDBG_INPUT_MGR,1) << "    Getting MotionStar ready ...\n"
                                 << vjDEBUG_FLUSH;

      if ( mMotionStar.start() == -1 ) {
         vjDEBUG(vjDBG_INPUT_MGR, 0) << "vjMotionStar failed to connect to server\n"
                                     << vjDEBUG_FLUSH;
         return 0;
      }

      //sanity check.. make sure birds actually started
      if ( this->isActive() == false ) {
         vjDEBUG(vjDBG_INPUT_MGR,0)  << "vjMotionStar failed to start.\n"
                                     << vjDEBUG_FLUSH;
         return 0;
      }

      vjDEBUG(vjDBG_INPUT_MGR,1) << "vjMotionStar ready to go.\n"
                                 << vjDEBUG_FLUSH;

      myThread = new vjThread(sampleBirds, (void*) this, 0);

      if ( myThread == NULL ) {
          assert (false);
          return 0;  // Fail
      }
      else {
          return 1;   // success
      }
   }
   // The thread has been started, so we are already sampling.
   else {
      return 0;
   }
}

// ----------------------------------------------------------------------------
// Stop sampling.
// ----------------------------------------------------------------------------
int
vjMotionStar::stopSampling () {
   int retval;

   // If we are not active, we cannot stop the device.
   if ( this->isActive() == false ) {
      retval = 0;
   }
   // If the sampling thread was started, stop it and the device.
   else if ( myThread != NULL ) {
      vjDEBUG(vjDBG_INPUT_MGR,1) << "Stopping the MotionStar thread ...\n"
                                 << vjDEBUG_FLUSH;
      myThread->kill();
      delete myThread;
      myThread = NULL;

      vjDEBUG(vjDBG_INPUT_MGR,1) << "  Stopping the MotionStar ...\n"
                                 << vjDEBUG_FLUSH;
      mMotionStar.stop();

      // sanity check: did the device actually stop?
      if ( this->isActive() == true ) {
         vjDEBUG(vjDBG_INPUT_MGR,0) << "MotionStar didn't stop.\n"
                                    << vjDEBUG_FLUSH;
         retval = 0;
      }
      else {
         vjDEBUG(vjDBG_INPUT_MGR,1) << "Stopped.\n" << endl << vjDEBUG_FLUSH;
         retval = 1;
      }
   }
   // If the thread was not started, then the device is stopped.
   else {
      retval = 1;
   }

   return retval;
}

// ----------------------------------------------------------------------------
// Sample data.
// ----------------------------------------------------------------------------
int
vjMotionStar::sample () {
   if ( this->isActive() == false ) {
       vjDEBUG(vjDBG_ALL,0) << "NOT ACTIVE IN SAMPLE" << endl << vjDEBUG_FLUSH;
       return 0;
   }

   mMotionStar.sample();

   // For each bird
   for ( int i = 0; i < mMotionStar.getNumBirds(); i++ ) {
      // Get the index to the current read buffer
      int index = getBirdIndex(i,progress);

      //if (i==0)
      //vjDEBUG(vjDBG_ALL,0) << "i:  " << i << "  x: " << xPos(i) << "  y: " << yPos(i) << endl << vjDEBUG_FLUSH;

      theData[index].makeZYXEuler(mMotionStar.zRot(i), mMotionStar.yRot(i),
                                  mMotionStar.xRot(i));

      theData[index].setTrans(mMotionStar.xPos(i), mMotionStar.yPos(i),
                              mMotionStar.zPos(i));

//      if (i==1)
//         vjDEBUG(vjDBG_ALL,2) << "Flock: bird1:    orig:" << vjCoord(theData[index]).pos << endl << vjDEBUG_FLUSH;

      // Transforms between the cord frames
      // See transform documentation and VR System pg 146
      // Since we want the reciver in the world system, Rw
      // wTr = wTt*tTr
      vjMatrix world_T_transmitter, transmitter_T_reciever, world_T_reciever;

      // Set transmitter offset from local info.
      world_T_transmitter = xformMat;

      // Get reciever data from sampled data.
      transmitter_T_reciever = theData[index];

      // Compute total transform.
      world_T_reciever.mult(world_T_transmitter, transmitter_T_reciever);

      // Store corrected xform back into data.
      theData[index] = world_T_reciever;

//      if (i == 1)
//         vjDEBUG(vjDBG_ALL,2) << "Flock: bird1: xformed:" << vjCoord(theData[index]).pos << endl << vjDEBUG_FLUSH;
   }

   // Locks and then swaps the indices.
   swapValidIndexes();

   return 1;
}

// ----------------------------------------------------------------------------
// Update to the sampled data.
// ----------------------------------------------------------------------------
void
vjMotionStar::updateData () {
   // If the device is not active, we cannot update the data.
   if ( this->isActive() == false ) {
      vjDEBUG(vjDBG_ALL,0) << "Not active in updateData()\n" << vjDEBUG_FLUSH;
   }
   // Otherwise, go through with the update.
   else {
      vjGuard<vjMutex> updateGuard(lock);

      // Copy the valid data to the current data so that both are valid.
      for ( int i = 0; i < getNumBirds(); i++ ) {
         theData[getBirdIndex(i,current)] = theData[getBirdIndex(i,valid)];   // first hand
      }

      // Locks and then swap the indices.
      swapCurrentIndexes();
   }
}

// ----------------------------------------------------------------------------
// Get the reciever transform for the given bird number.  The birds are
// zero-based.
// ----------------------------------------------------------------------------
vjMatrix*
vjMotionStar::getPosData (int d) {
    if ( this->isActive() == false ) {
        vjDEBUG(vjDBG_ALL,0) << "Not active in getPosData()\n" << vjDEBUG_FLUSH;
        return NULL;
    }
    return (&theData[getBirdIndex(d,current)]);
}

// ----------------------------------------------------------------------------
// Not used currently -- needed for interface.
// ----------------------------------------------------------------------------
vjTimeStamp*
vjMotionStar::getPosUpdateTime (int d) {
    return (&mDataTimes[getBirdIndex(d,current)]);
}

// ----------------------------------------------------------------------------
// Change the hemisphere of the transmitter.
// ----------------------------------------------------------------------------
void
vjMotionStar::setHemisphere (int i) {
   // If the device active, we cannot change the hemisphere.
   if ( this->isActive() ) {
      vjDEBUG(vjDBG_INPUT_MGR,2)
         << "vjMotionStar: Cannot change hemisphere while active\n"
         << vjDEBUG_FLUSH;
   } else {
      mMotionStar.setHemisphere(i);
   }
}

// ----------------------------------------------------------------------------
// Set the bird format to the given value.
// ----------------------------------------------------------------------------
void
vjMotionStar::setBirdFormat (unsigned int i) {
   // If the device active, we cannot change the bird format.
   if ( this->isActive() ) {
      vjDEBUG(vjDBG_INPUT_MGR,2)
          << "vjMotionStar: Cannot change format while active\n"
          << vjDEBUG_FLUSH;
   } else {
       mMotionStar.setBirdFormat(i);
   }
}

// ----------------------------------------------------------------------------
// Set the number of birds connected to the flock.
// ----------------------------------------------------------------------------
void
vjMotionStar::setNumBirds (unsigned int i) {
   // If the device active, we cannot change the number of birds.
   if ( this->isActive() ) {
      vjDEBUG(vjDBG_INPUT_MGR,2)
         << "vjMotionStar: Cannot change number of birds while active\n"
         << vjDEBUG_FLUSH;
   } else {
      mMotionStar.setNumBirds(i);
   }
}

// ----------------------------------------------------------------------------
// Set the bird rate to the given value.
// ----------------------------------------------------------------------------
void
vjMotionStar::setBirdRate (double d) {
   // If the device active, we cannot change the bird rate.
   if ( this->isActive() ) {
      vjDEBUG(vjDBG_INPUT_MGR,2)
         << "vjMotionStar: Cannot change sample rate while active\n"
         << vjDEBUG_FLUSH;
   } else {
      mMotionStar.setBirdRate(d);
   }
}

// ----------------------------------------------------------------------------
// Set the run mode for the device.
// ----------------------------------------------------------------------------
void
vjMotionStar::setRunMode (int i) {
   // If the device active, we cannot change the run mode.
   if ( this->isActive() ) {
      vjDEBUG(vjDBG_INPUT_MGR,2)
         << "vjMotionStar: Cannot change run mode while active\n"
         << vjDEBUG_FLUSH;
   } else {
      mMotionStar.setRunMode(i);
   }
}

// ----------------------------------------------------------------------------
// Set the report rate for the device.
// ----------------------------------------------------------------------------
void
vjMotionStar::setReportRate (unsigned char ch) {
   // If the device active, we cannot change the report rate.
   if (this->isActive()) {
      vjDEBUG(vjDBG_INPUT_MGR,2)
         << "vjMotionStar: Cannot change report rate while active\n"
         << vjDEBUG_FLUSH;
   } else {
      mMotionStar.setReportRate(ch);
   }
}

// ----------------------------------------------------------------------------
// Set the address (either IP address or hostname) for the server.
// ----------------------------------------------------------------------------
void
vjMotionStar::setAddress (const char* n) {
   // If the device active, we cannot change the server address.
   if ( this->isActive() ) {
      vjDEBUG(vjDBG_INPUT_MGR,2)
         << "vjMotionStar: Cannot change server address while active\n"
         << vjDEBUG_FLUSH;
   } else {
      mMotionStar.setAddress(n);
   }
}

// ============================================================================
// Private methods.
// ============================================================================

// ----------------------------------------------------------------------------
// Helper to return the index for theData array given the birdNum we are
// dealing with and the bufferIndex to read.
//
// XXX: We are going to say the birds are 0 based.
// ----------------------------------------------------------------------------
int
vjMotionStar::getBirdIndex (int birdNum, int bufferIndex) {
   int ret_val = (birdNum*3)+bufferIndex;
   vjASSERT((ret_val >= 0) && (ret_val < ((getNumBirds()+1)*3)));
   //assertIndexes();   // Can't assert here because it is possible the indexes are switching right now
   return ret_val;
}
