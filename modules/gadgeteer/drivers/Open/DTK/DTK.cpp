/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2010 by Iowa State University
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
 *************** <auto-copyright.pl END do not edit this line> ***************/

#include <gadget/Devices/DriverConfig.h>

#include <strstream>
#include <fstream>
#include <boost/bind.hpp>

#include <dtk.h>
#include <dlfcn.h>

#include <jccl/Config/ConfigChunk.h>
#include <gadget/Type/DeviceConstructor.h>
#include <gadget/Util/Debug.h>
#include <gadget/gadgetParam.h>
#include <drivers/Open/DTK/DTK.h>


extern "C"
{

GADGET_DRIVER_EXPORT(vpr::Uint32) getGadgeteerVersion()
{
   return __GADGET_version;
}

GADGET_DRIVER_EXPORT(void) initDevice(gadget::InputManager* inputMgr)
{
   new gadget::DeviceConstructor<gadget::DTK>(inputMgr);
}

}

// Helper to return the index for theData array
// given the stationNum we are dealing with and the bufferIndex
// to get
//! ARGS:stationNum - The number of the cube we care about
//! ARGS:bufferIndex - the value of current, progress, or valid (it is an offset in the array)
// XXX: We are going to say the cubes are 0 based

namespace gadget
{

int DTK::getStationIndex(int stationNum, int bufferIndex)
{
   // We could assert here to check that bufferIndex is valid.  As far as station index, there are several
   // places which we have different ranges of values for it.  So we can't really assert to see if it is valid.
   int ret_val = (stationNum*3)+bufferIndex;
   return ret_val;
}


DTK::DTK()
{
   _dtkSegments = NULL;
   port = NULL;
}

std::string DTK::getElementType()
{
   return "dtk";
}

bool DTK::config(jccl::ConfigChunkPtr c)
{
   vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_STATE_LVL)
      << "  DTK::config(jccl::ConfigChunkPtr)" << std::endl << vprDEBUG_FLUSH;

   if ( !Position::config(c) || !Digital::config(c) || !Analog::config(c) )
   {
      return false;
   }


// This is where we map the shared memory blocks to positional, digital, and analog data types
// The Config chunk will have the following items:
//     String: port
//        int: numberOfDevices
// Each Device will have the following items:
//     String: segmentName
//        int: dataType

   jccl::ConfigChunkPtr embeddedChunk = NULL;
   int i = 0;

/* Dynamically Load the DTK Library
    void* handle;
    char *DTKFileName = c->getProperty<std::string>("dlfilename").c_str();
    handle = dlopen(DTKFileName, RTLD_NOW|RTLD_GLOBAL);
    if(handle == NULL) return false;
*/


// Setup each embedded memory chunk

   numSegments = c->getNum("segments");
   if ( numSegments > 0 )
   {
      _dtkSegments = new DTKMemorySegmentHandle[numSegments];

      for ( i = 0; i < numSegments; i++ )
      {
         embeddedChunk = c->getProperty<jccl::ConfigChunkPtr>("segments", i);
         _dtkSegments[i] = new DTKMemorySegment;
         if ( embeddedChunk != NULL )
         {
            if ( !(_dtkSegments[i]->config(embeddedChunk)) )
            {
               continue;//ERROR!!! Go on to next item...
            }
            switch ( _dtkSegments[i]->SegmentType() )
            {
               case dtk_pos_XYZEuler:
               case dtk_pos_ZYXEuler:
               case dtk_pos_QUAT:
                  _dtkSegments[i]->setItemIndex(numPositional++);
                  break;
               case dtk_digital:
                  _dtkSegments[i]->setItemIndex(numDigital++);
                  break;
               case dtk_analog:
                  _dtkSegments[i]->setItemIndex(numAnalog++);
                  break;
            }

         }
         delete embeddedChunk; // Free the copy of the embedded memory chunk
      }
   }

   return true;
}

DTK::~DTK()
{
   this->stopSampling();
   int i = 0;

   if ( _dtkSegments != NULL )
   {
      for ( i = 0; i < numSegments; i++ )
      {
         delete _dtkSegments[i];
      }
      delete [] _dtkSegments;
      _dtkSegments = NULL;
   }

   if ( theData != NULL )
   {
      delete [] theData;
   }

   if ( mDataTimes != NULL )
   {
      delete [] mDataTimes;
   }

   if ( mAnalogData != NULL )
   {
      delete [] mAnalogData;
   }

   if ( mDigitalData != NULL )
   {
      delete [] mDigitalData;
   }
}

// Main thread of control for this active object
void DTK::controlLoop()
{
   if ( theData != NULL )
   {
      delete [] theData;
   }

   if ( mDataTimes != NULL )
   {
      delete [] mDataTimes;
   }

   if ( mAnalogData != NULL )
   {
      delete [] mAnalogData;
   }

   if ( mDigitalData != NULL )
   {
      delete [] mDigitalData;
   }

   int numbuffs = numPositional*3;
   theData = (gmtl::Matrix44f*) new gmtl::Matrix44f[numbuffs];
   mDataTimes = new jccl::TimeStamp[numbuffs];

   numbuffs = numDigital*3;
   mDigitalData = new int[numbuffs];

   numbuffs = numAnalog*3;
   mAnalogData = new float[numbuffs];

// Reset current, progress, and valid indices
//    resetIndexes();

// Loop through and keep sampling
   while(!mExitFlag)
   {
      this->sample();
   }
}

bool DTK::startSampling()
{
// make sure inertia cubes aren't already started
   if ( this->isActive() == true )
   {
      vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_WARNING_LVL)
         << "gadget::DTK was already started." << std::endl << vprDEBUG_FLUSH;
      return false;
   }

   bool started(false);

// Has the thread actually started already
   if ( mThread != NULL )
   {
      vprDEBUG(vprDBG_ERROR,vprDBG_CRITICAL_LVL)
         << clrOutNORM(clrRED,"ERROR:")
         << "gadget::DTK: startSampling called, when already sampling.\n"
         << vprDEBUG_FLUSH;
      vprASSERT(false);
   }
   else
   {
      if ( !this->startDTK() )
      {
         vprDEBUG(vprDBG_ERROR,vprDBG_CRITICAL_LVL)
            << clrOutNORM(clrRED,"ERROR:") << "gadget::DTK: \n"
            << vprDEBUG_FLUSH;
         return false;
      }

      mExitFlag = false;

      // Create a new thread to handle the control
      try
      {
         mThread = new vpr::Thread(boost::bind(&DTK::controlLoop, this));
         started = true;
      }
      catch (vpr::Exception& ex)
      {
         vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_CRITICAL_LVL)
            << clrOutBOLD(clrRED, "ERROR")
            << ": Failed to spawn thread for DTK driver!\n" << vprDEBUG_FLUSH;
         vprDEBUG_NEXT(gadgetDBG_INPUT_MGR, vprDBG_CRITICAL_LVL)
            << ex.what() << std::endl << vprDEBUG_FLUSH;
      }
   }

   return started;
}

bool DTK::sample()
{
   if ( this->isActive() == false )
   {
      return 0;
   }

   int i;
   int index;
   DTKMemorySegment* segment = NULL;
   float *floatData;
   int   *intData;

   jccl::TimeStamp sampletime;
   sampletime.set();

   vpr::Thread::yield()

   for ( i = 0; i < numSegments; i++ )
   {
      segment = _dtkSegments[i];
      if ( segment != NULL )
      {
         if ( segment->SegmentType() <= dtk_pos_QUAT && segment->ItemIndex() < numPositional )
         {
            index = getStationIndex(segment->ItemIndex(),progress);

            floatData = static_cast<float*>(*segment);
            // Check to see that the pointer is valid
            if ( floatData == NULL ) continue;

            if ( segment->SegmentType() != dtk_pos_QUAT )
            {
               // Determine which format the Euler angle is in
               if ( segment->SegmentType() == dtk_pos_XYZEuler )
               {
                  theData[index].makeXYZEuler(floatData[5], floatData[4], floatData[3]);
               }

               if ( segment->SegmentType() == dtk_pos_ZYXEuler )
               {
                  theData[index].makeZYXEuler(floatData[5], floatData[4], floatData[3]);
               }

               theData[index].setTrans(floatData[0], floatData[1], floatData[2]);
            }
            else
            {
               // Data is in Quaternion Format


            }
            mDataTimes[index] = sampletime;
         }
         else if ( segment->SegmentType() == dtk_digital && segment->ItemIndex() < numDigital )
         {
            index = getStationIndex(segment->ItemIndex(),progress);

            intData = static_cast<int*>(*segment);
            // Check to see that the pointer is valid
            if ( intData == NULL )
            {
               continue;
            }

            mDigitalData[index] = *(intData);
         }
         else if ( segment->SegmentType() == dtk_analog && segment->ItemIndex() < numAnalog )
         {
            index = getStationIndex(segment->ItemIndex(),progress);

            floatData = static_cast<float*>(*segment);
            // Check to see that the pointer is valid
            if ( floatData == NULL )
            {
               continue;
            }

            mAnalogData[index] = *(floatData);
         }
      }
   }
// Locks and then swaps the indices
   swapValidIndexes();

   return 1;
}

bool DTK::stopSampling()
{
   if ( this->isActive() == false )
   {
      return 0;
   }

   if( mThread != NULL)
   {

      mExitFlag = true;

      vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_CONFIG_LVL)
         << "gadget::DTK::stopSampling(): Stopping the DTK thread... "
         << vprDEBUG_FLUSH;

      mThread->join();
      delete mThread;
      mThread = NULL;

      this->stopDTK();

      vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_CONFIG_LVL)
         << "stopped." << std::endl << vprDEBUG_FLUSH;
   }
   return 1;
}

gmtl::Matrix44f* DTK::getPosData( int d )
{
   if ( (this->isActive() == false) || (d < 0) || (d >= numPositional) )
   {
      return NULL;
   }

   return(&theData[getStationIndex(d,current)]);
}


int DTK::getDigitalData( int d )
{
   if ( (this->isActive() == false) || (d < 0) || (d >= numDigital) )
   {
      return 0;
   }

   return mDigitalData[getStationIndex(d,current)];
}

float DTK::getAnalogData( int d )
{
   if ( (this->isActive() == false) || (d < 0) || (d >= numAnalog) )
   {
      return 0.0;
   }

   return mAnalogData[getStationIndex(d,current)];
}

jccl::TimeStamp* DTK::getPosUpdateTime(int d)
{
   if ( (this->isActive() == false) || (d < 0) || (d >= numPositional) )
   {
      return NULL;
   }

   return(&mDataTimes[getStationIndex(d,current)]);
}

void DTK::updateData()
{
   if ( this->isActive() == false )
   {
      return;
   }

// this unlocks when this object is destructed (upon return of the function)
   vpr::Guard<vpr::Mutex> updateGuard(lock);


// TODO: modify the datagrabber to get correct data
// Copy the valid data to the current data so that both are valid
   int i;

   for ( i = 0;i < numPositional; i++ )
   {
      theData[getStationIndex(i,current)] = theData[getStationIndex(i,valid)];   // first hand
   }

   for ( i = 0; i < numDigital; i++ )
   {
      mDigitalData[getStationIndex(i,current)] = mDigitalData[getStationIndex(i,valid)];
   }

   for ( i = 0; i < numDigital; i++ )
   {
      mAnalogData[getStationIndex(i,current)] = mAnalogData[getStationIndex(i,valid)];
   }

// Locks and then swap the indicies
   swapCurrentIndexes();
}

bool DTK::startDTK()
{
   int i = 0;

   if ( active || _client != NULL )
   {
      return false;
   }

   if ( port != NULL )
   {
      _client = new dtkClient( port );
   }
   else
   {
      _client = new dtkClient();
   }

   for ( i = 0; i < numSegments; i++ )
   {
      _dtkSegments[i]->connectSegment(_client);
   }

   mActive = true;

   return true;
}

bool DTK::stopDTK()
{
   if ( !active || _client == NULL )
   {
      return false;
   }
   delete _client;
   mActive = false;

   return true;
}

} // End of gadget namespace
