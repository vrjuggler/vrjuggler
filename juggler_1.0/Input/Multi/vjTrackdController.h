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

#ifndef _VJ_TRACKD_CONTROLLER_H_
#define _VJ_TRACKD_CONTROLLER_H_

#include <vjConfig.h>
#include <Input/vjInput/vjDigital.h>
#include <Input/vjSim/vjSimInput.h>
#include <Input/Multi/aTrackdController.h>
#include <vector>


//-----------------------------------------------------------------------------
//: Class interfacing with trackd sensor data
//+ located on the local machine in a shared memory arena
//
//! NOTE: A note on reciever access:
//+  Clients of Juggler should access tracker recievers as [0-n].  For
//+  example, if you have recievers 1,2, and 4 with transmitter on 3, then
//+  you can access the data, in order, as 0, 1, 2.
//
// See also: vjDigital, vjAnalog
//-----------------------------------------------------------------------------
//!PUBLIC_API:
class vjTrackdController : virtual public vjDigital, virtual public vjAnalog
{
public:

   // ------------------------------------------------------------------------
   //: Constructor.
   // ------------------------------------------------------------------------
   vjTrackdController();

   // ------------------------------------------------------------------------
   //: Destructor.
   //
   //! PRE: None.
   //! POST: Shared memory is released
   // ------------------------------------------------------------------------
   ~vjTrackdController();

   // ------------------------------------------------------------------------
   //: Configure the trackd controller with the given config chunk.
   //
   //! PRE: c must be a chunk that has trackd controller config information
   //! POST: If c is a valid config chunk, the device is configured using its
   //+       contents.  Otherwise, configuration fails and false is returned
   //+       to the caller.
   //
   //! ARGS: c - A pointer to a config chunk.
   //
   //! RETURNS: true - The device was configured succesfully.
   //! RETURNS: false - The config chunk is invalid.
   // ------------------------------------------------------------------------
   virtual bool config(vjConfigChunk* c);

   //: Begin sampling
   int startSampling() { return 1;}

   //: Stop sampling
   int stopSampling() { return 1;}

   //: Sample a value
   int sample() { return 1;}

   // ------------------------------------------------------------------------
   //: Update to the sampled data.
   //
   //! PRE: None.
   //! POST: Most recent value is copied over to temp area
   // ------------------------------------------------------------------------
   void updateData();

   //: Get the device name.
   inline char*
   getDeviceName () {
      return "vjTrackdController";
   }

   //: Return what chunk type is associated with this class.
   static std::string
   getChunkType () {
      return std::string("TrackdController");
   }

   //: Return digital data
   virtual int getDigitalData(int devNum=0)
   {
      vjASSERT(devNum < (int)mCurButtons.size() && "Digital index out of range");    // Make sure we have enough space
      return mCurButtons[devNum];
   }

   //: Return analog data
   virtual int getAnalogData(int devNum=0)
   {
      vjASSERT(devNum < (int)mCurValuators.size() && "Analog index out of range");    // Make sure we have enough space
      return static_cast<int>(mCurValuators[devNum]);
   }

private:
   aTrackdController*      mTrackdController;      // The controller we are dealing with
   std::vector<float>      mCurValuators;          // The current (up-to-date) values
   std::vector<bool>       mCurButtons;            // The current button states
};

#endif

