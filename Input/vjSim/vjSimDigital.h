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


#ifndef _VJ_SIM_DIGITAL_H
#define _VJ_SIM_DIGITAL_H
//#pragma once

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
//!PUBLIC_API:
class vjSimDigital : virtual public vjDigital, public vjSimInput
{
public:
   vjSimDigital() {;}
   ~vjSimDigital() {;}

   virtual bool config(vjConfigChunk* chunk);

   //: Return analog data
   virtual int getDigitalData(int devNum=0)
   {
      vjASSERT(devNum < (int)mDigitalData.size());    // Make sure we have enough space
      return mDigitalData[devNum];
   }

   /* These functions don't do anything */
   int startSampling() { return 1; }
   int stopSampling() { return 1; }
   int sample() { return 1; }

   //: Update the data
   virtual void updateData();

   //: Get the name of the digital device
   char* getDeviceName() { return "vjSimDigital";}
   static std::string getChunkType() { return std::string("SimDigital");}

private:
   std::vector<int>          mDigitalData;   //: The digital data that we have
   std::vector<vjKeyModPair> mSimKeys;       //: The keys to press for digital simulation
};

#endif
