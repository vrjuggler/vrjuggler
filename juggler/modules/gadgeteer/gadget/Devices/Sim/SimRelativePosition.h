/*
 *  File:          $RCSfile$
 *  Date modified: $Date$
 *  Version:       $Revision$
 *
 *
 *                                VR Juggler
 *                                    by
 *                              Allen Bierbaum
 *                             Christopher Just
 *                             Patrick Hartling
 *                            Carolina Cruz-Neira
 *                               Albert Baker
 *
 *                  Copyright (C) - 1997, 1998, 1999, 2000
 *              Iowa State University Research Foundation, Inc.
 *                            All Rights Reserved
 */


#ifndef _VJ_SIM_RELATIVE_POSITION_H
#define _VJ_SIM_RELATIVE_POSITION_H

#include <vjConfig.h>
#include <Input/vjPosition/vjPosition.h>
#include <Input/InputManager/vjPosInterface.h>
class vjConfigChunk;

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
class vjSimRelativePosition : virtual public vjPosition
{
public:
   vjSimRelativePosition() {;}
   ~vjSimRelativePosition() {;}

   virtual bool config(vjConfigChunk* chunk);

   //: Return position data
   vjMatrix* getPosData(int devNum=0)
   {
      vjASSERT(devNum == 0);    // Make sure we have a valid dev
      return &mPos;
   }

   vjTimeStamp* getPosUpdateTime (int devNum = 0) {
	   return &mUpdateTime;
    }

   /* These functions don't do anything */
   int startSampling() { return 1; }
   int stopSampling() { return 1; }
   int sample() { return 1; }

   //: Update the data
   virtual void updateData();

   //: Get the name of the digital device
   char* getDeviceName() { return "SimRelativePosition";}

   static std::string getChunkType() { return std::string("SimRelativePosition"); }

private:
   vjMatrix          mPos;                         //: The current position being simulated
   vjPosInterface    mBaseFrame;                   //: The base frame of reference
   vjPosInterface    mRelativePos;                 //: the relative position

   vjTimeStamp   mUpdateTime;                  //: Time of last update
};

#endif
