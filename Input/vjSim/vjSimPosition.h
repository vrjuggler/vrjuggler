/*
 *  File:	    $RCSfile$
 *  Date modified:  $Date$
 *  Version:	    $Revision$
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


#ifndef _VJ_SIM_POSITION_H
#define _VJ_SIM_POSITION_H
//#pragma once

#include <vjConfig.h>
#include <Input/vjSim/vjSimInput.h>
#include <Input/vjPosition/vjPosition.h>


//: Simulated positional device
// Simulates a positional device from a keyboard device.
//
// This class should not be used directly by the user.
//!PUBLIC_API:
class vjSimPosition : virtual public vjPosition, public vjSimInput
{
public:
   /* constants for the key array */
   enum {
      FORWARD = 0,
      BACK    = 1,
      LEFT    = 2,
      RIGHT   = 3,
      UP      = 4,
      DOWN    = 5,
      ROTR    = 6,
      ROTL    = 7,
      ROTU    = 8,
      ROTD    = 9,
      ROT_ROLL_CCW = 10,
      ROT_ROLL_CW = 11,
      NUM_POS_CONTROLS = 12
   };

   // Const for coord system selection
   enum {
      LOCAL = 0,
      GLOBAL = 1
   };
public:
   vjSimPosition() {;}
   ~vjSimPosition() {;}

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
   char* getDeviceName() { return "vjSimPosition";}

   static std::string getChunkType() { return std::string("SimPosition"); }

private:
   //: Move forward (-z) the given amount on position data n
   void moveFor(const float amt);

   //: Move left (-X) the given amount
   void moveLeft(const float amt);

   //: Move up (+y) the given amount
   void moveUp(const float amt);

   //: Pitch up - rot +x axis
   void rotUp(const float amt);

   //: Yaw left - rot +Y axis
   void rotLeft(const float amt);

   //: Roll Left - rot -z axis
   void rotRollCCW(const float amt);

   //: Check if movement is allowed
   //! NOTE: It is not allowed if it hits a simulated wall, etc.
   bool isTransAllowed(vjVec3 trans);


private:
   int            mTransCoordSystem;                 //: What coord system for translations?
   int            mRotCoordSystem;              //: What coord system for rotations?
   vjMatrix       mPos;                         //: The current position being simulated
   vjKeyModPair   mSimKeys[NUM_POS_CONTROLS];   //: The keys to press for digital simulation
   float          mDTrans;                      //: Delta translation
   float          mDRot;                        //: Delta rotation

    vjTimeStamp   mUpdateTime;                  //: Time of last update
};

#endif
