#ifndef _VJ_SIM_POSITION_H
#define _VJ_SIM_POSITION_H
#pragma once

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
   vjMatrix* GetPosData(int devNum=0)
   {
      vjASSERT(devNum == 0);    // Make sure we have a valid dev
      return &mPos;
   }

   /* These functions don't do anything */
   int StartSampling() { return 1; }
   int StopSampling() { return 1; }
   int Sample() { return 1; }

   //: Update the data
   virtual void UpdateData();

   //: Get the name of the digital device
   char* GetDeviceName() { return "vjSimPosition";}

   static std::string getChunkType() { return std::string("SimPosition"); }

private:
   //: Move forward (-z) the given amount on position data n
   void MoveFor(const float amt);

   //: Move left (-X) the given amount
   void MoveLeft(const float amt);

   //: Move up (+y) the given amount
   void MoveUp(const float amt);

   //: Pitch up - rot +x axis
   void RotUp(const float amt);

   //: Yaw left - rot +Y axis
   void RotLeft(const float amt);

   //: Roll Left - rot -z axis
   void RotRollCCW(const float amt);


private:
   int            mTransCoordSystem;                 //: What coord system for translations?
   int            mRotCoordSystem;              //: What coord system for rotations?
   vjMatrix       mPos;                         //: The current position being simulated
   vjKeyModPair   mSimKeys[NUM_POS_CONTROLS];   //: The keys to press for digital simulation
   float          mDTrans;                      //: Delta translation
   float          mDRot;                        //: Delta rotation
};

#endif
