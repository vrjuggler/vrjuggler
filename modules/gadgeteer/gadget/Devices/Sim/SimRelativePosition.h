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

#ifndef _VRJ_SIM_RELATIVE_POSITION_H
#define _VRJ_SIM_RELATIVE_POSITION_H

#include <gad/gadConfig.h>
#include <gad/Type/Input.h>
#include <gad/Type/Position.h>
#include <gad/Type/PosInterface.h>

namespace vrj
{
   class ConfigChunk;

//: Simulated a position device by making
// An existing device behave relative to another one.
//
// One device is the base coordinate system frame of reference.
// The other device has it's position information transformed
// to be relative to that frame.
// ie. There is a matrix multiply. :)
//
// This class should not be used directly by the user.
//!PUBLIC_API:
class SimRelativePosition : public Input, public Position
{
public:
   SimRelativePosition() {;}
   virtual ~SimRelativePosition() {;}

   virtual bool config(ConfigChunk* chunk);

   //: Return position data
   Matrix* getPosData(int devNum=0)
   {
      vprASSERT(devNum == 0);    // Make sure we have a valid dev
      return &mPos;
   }

   TimeStamp* getPosUpdateTime (int devNum = 0) {
      return &mUpdateTime;
    }

   /* These functions don't do anything */
   int startSampling() { return 1; }
   int stopSampling() { return 1; }
   int sample() { return 1; }

   //: Update the data
   virtual void updateData();

   static std::string getChunkType() { return std::string("SimRelativePosition"); }

private:
   Matrix          mPos;                         //: The current position being simulated
   PosInterface    mBaseFrame;                   //: The base frame of reference
   PosInterface    mRelativePos;                 //: the relative position

   TimeStamp   mUpdateTime;                  //: Time of last update
};

};

#endif
