#ifndef _VJ_SIM_DIGITAL_H
#define _VJ_SIM_DIGITAL_H
#pragma once

#include <vjConfig.h>
#include <Input/vjInput/vjDigital.h>
#include <Input/vjSim/vjSimInput.h>

//: Simulated digital device
// Simulates a digital device from a keyboard device.
// It allows any number of simulated digital devices to be created.
//
// When the key configured is held down, the digital device is on.
// When the key is released, the device is no longer set.
//
// This class should not be used directly by the user.
class vjSimDigital : virtual public vjDigital, public vjSimInput
{
public:
   vjSimDigital(vjConfigChunk* chunk);
   ~vjSimDigital() {;}

   //: Return analog data
   virtual int GetDigitalData(int devNum=0)
   {
      vjASSERT(devNum < mDigitalData.size());    // Make sure we have enough space
      return mDigitalData[devNum];
   }

   /* These functions don't do anything */
   int StartSampling() { return 1; }
   int StopSampling() { return 1; }
   int Sample() { return 1; }

   //: Update the data
   virtual void UpdateData();

   //: Get the name of the digital device
   char* GetDeviceName() { return "vjSimDigital";}

private:
   vector<int>             mDigitalData;     //: The digital data that we have
   vector<vjKeyModPair>    mSimKeys;         //: The keys to press for digital simulation
};

#endif
