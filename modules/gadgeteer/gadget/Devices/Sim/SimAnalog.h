#ifndef _VJ_SIM_ANALOG_H
#define _VJ_SIM_ANALOG_H
#pragma once

#include <vjConfig.h>
#include <Input/vjInput/vjAnalog.h>
#include <Input/vjSim/vjSimInput.h>

//: Simulated analog device
// Simulates an analog device from a keyboard device.
// It allows any number of simulated analog devices to be created.
//
// This class should not be used directly by the user.
//!PUBLIC_API:
class vjSimAnalog : virtual public vjAnalog, public vjSimInput
{
public:
   vjSimAnalog() {;}
   ~vjSimAnalog() {;}

   virtual bool config(vjConfigChunk* chunk);

   //: Return analog data
   virtual int GetAnalogData(int devNum=0)
   {
      vjASSERT(devNum < mAnaData.size());    // Make sure we have enough space
      return mAnaData[devNum];
   }

   /* These functions don't do anything */
   int StartSampling() { return 1; }
   int StopSampling() { return 1; }
   int Sample() { return 1; }

   //: Update the data
   virtual void UpdateData();

    //: Get the name of the device
   char* GetDeviceName() { return "vjSimAnalog";}

   static std::string getChunkType() { return std::string("SimAnalog");}

private:
   std::vector<int>          mAnaData;     //: The analog data that we have
   std::vector<vjKeyModPair> mSimKeysUp;   //: The keys to press for moving analog up
   std::vector<vjKeyModPair> mSimKeysDown; //: The keys to press for moving analog up
   int                     mAnaStep;      //: The analog step size
};

#endif

