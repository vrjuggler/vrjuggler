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


/////////////////////////////////////////////////////////////////////////////
// Base class for all vj Input devices
//
// History:
//
// Andy Himberger:    v0.1 (10/12/97) - Integeration to new libraries,
//                                      support for
//              Abilities, Instance Names, Device Names, Config Chunk
//              constructors
// Andy Himberger:    v0.0  - Inital version
////////////////////////////////////////////////////////////////////////////
#ifndef VJ_INPUT_H
#define VJ_INPUT_H

#include <vjConfig.h>

#ifndef VJ_OS_Win32
#include <unistd.h>

#include <termios.h>

#ifdef HAVE_SYS_PRCTL_H
#   include <sys/prctl.h>
#endif      /* ifdef HAVE_SYS_PRCTL_H */
#endif      /* ifndef VJ_OS_Win32 */

#include <limits.h>
#include <string.h>
#include <sys/types.h>
#include <fcntl.h>
#include <signal.h>
#include <float.h>

#include <SharedMem/vjMemory.h>
#include <Sync/vjMutex.h>
#include <Sync/vjGuard.h>
#include <Threads/vjThread.h>
#include <Config/vjConfigChunkDB.h>
#include <Kernel/vjDebug.h>


typedef unsigned char byte;

/** Abilities List
 *
 *  vjInput devices can have one or more 'Abilities'
 *  The function FDeviceSupport(ability) allows a user
 *  of a vjInput object to query which types it may be
 *  casted up to.
 */
const unsigned int DEVICE_POSITION = 1;
const unsigned int DEVICE_GESTURE  = 2;
const unsigned int DEVICE_DIGITAL  = 4;
const unsigned int DEVICE_ANALOG   = 8;
const unsigned int DEVICE_GLOVE    = 16;
const unsigned int DEVICE_KEYBOARD = 32;
const unsigned int DEVICE_GROW1    = 64;
const unsigned int DEVICE_GROW2    = 128;

//-----------------------------------------------------------------------------
//: vjInput is the abstract base class that all input objects derive from.
//
//  vjInput is the base class for all Input Devices, all the devices are
//  therefore forced to implement the pure virtual functions of Sample,
//  StartSampling, StopSampling, and UpdateData. <br> <br>
//
//  Dummy devices can use a default constructor, but physical devices should
//  have a Constructor which takes a config chunk and calls the vjInput
//  constructor taking a vjConfigChunk. <br> <br>
//
//  All Physical devices will inherit from not vjInput but another abstract
//  class which inherits from vjInput, currently there is support for
//  Positional Devices, Analog Devices, and Digital devices, each has its own
//  added pure virtual functions providing a simple and equal interface to
//  themselves.
//
//! NOTE: We make the assumption in all devices that while UpdateData() is being
//+       called, no other process will try to read the current data.
//+       We can make this assumption because the whole idea of UpdateData() is
//+       to bring in a current copy of the data for threads to process for a
//+       frame.  Because of this, threads should not be reading data while
//+       it is being updated to the most recent copy.
//-----------------------------------------------------------------------------
//!PUBLIC_API:
class vjInput
{
public:
   //: Default Constructor
   //
   //  The default constructor is intended only for use by the DummyProxies
   //  which do not need to have their serial port and baud rate etc set up.
   // Also, initializes myThread, active, and deviceAbilities to null values
   vjInput();

   //: vjInput Destructor
   //
   // Free the memory for the Instance Name and Serial Port strings if
   // allocated
   virtual ~vjInput();

   //: Config method
   //
   //  This baselevel config will fill the base datamembers
   //  when found in the vjConfigChunk, such as serial port, instance name
   //  and baud rate.
   virtual bool config(vjConfigChunk *c);

   //: Sample the device
   //
   //  Every input device should have a sample function, after which the
   //  device has been sampled to have new data.  (This new data is not
   //  accessable until UpdateData is called, however)
   virtual int sample() = 0;

   //: Start a device sampling.
   //
   //  Start the device sampling, normally this will spawn a thread which will
   //  just repeatedly call Sample().
   //  This function should return true when it sucessfully starts,
   //      false otherwise.
   virtual int startSampling() = 0;

   //: StopSampling.
   //
   //  Reverse the effects of StartSampling()
   virtual int stopSampling() = 0;

   //: updateData()
   //
   //  After this function is called subsequent calls to GetData(d) will
   //  return the most recent data at the time of THIS function call.  Data is
   //  guaranteed to be valid and static until the next call to UpdateData.
   virtual void updateData() = 0;

   //: getDevicename()
   //
   //  Returns the name identifying the TYPE of Input Device
   virtual char* getDeviceName() { return "vjInputBase";}

   //: Returns the string rep of the chunk type used to config this device
   // Used by input manager to find chunks that construct devices
   static std::string getChunkType() { return std::string("Undefined"); }

   /** @name Functions to remove (?)
     *
     * It hasn't been resolved as to whether these functions should be removed
     */
   //@{
   void setPort(const char* serialPort);
   char* getPort();
   void setBaudRate(int baud);
   int  getBaudRate() { return baudRate;}
   //@}

   //: getInstanceName()
   //
   //  Returns the name identifying the INSTANCE of this InputDevice
   const char* getInstanceName() {
      if (instName == NULL) return "Undefined";
      return instName;
   }

   //: fDeviceSupport(ability)
   //
   //  Returns true/false does this input device support the ability passed in
   int  fDeviceSupport(int devAbility);

   //: Is this input device active?.
   int isActive() { return active;}

protected:  // Helpers
   //: Reset the Index Holders
   // Sets to (0,1,2) in that order
   void resetIndexes();

   //: Swap the current and valid indexes (NOT thread safe)
   // You must have a lock when you call this
   //! NOTE: In the same lock that this is called, the driver
   //+      MUST copy the valid data over the current data in order
   //+      to keep from seeing bad data in the future
   void swapCurrentIndexes();

   //: Swap the valid and progress indexes (thread safe)
   void swapValidIndexes();

   void assertIndexes()
   {
      vjASSERT((current != progress) && (current != valid) && (progress != valid));
      vjASSERT((current >= 0) && (current <= 3));
      vjASSERT((progress >= 0) && (progress <= 3));
      vjASSERT((valid >= 0) && (valid <= 3));
   }

protected:
   char*       sPort;
   char*       instName;
   int         port_id;
   vjThread*   myThread;   //: The thread being used by the driver
   int         active;     //: Is the driver active

   //: Index holders
   // Current is the index to the current data being returned by GetData
   // Progress is the index to the data being filled by Sample() at any given
   // time Valid is the index to the data that has already been filled by
   // Sample() and will become the next Current data.  (NOTE: no dirty bit
   // checking is done to make SURE that valid is newer than current on
   // UpdateData switches)
   int current, progress, valid;

   vjMutex lock;        //: Mutex for swapping the pointers.
   int baudRate;        //: Baud rate of the device (if it is serial device)
   unsigned int deviceAbilities;    //: Combined mask of device abilities
};

#endif   /* VJ_INPUT_H */
