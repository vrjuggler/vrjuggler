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


#ifndef _VJ_SIM_ANALOG_H
#define _VJ_SIM_ANALOG_H
//#pragma once

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
   virtual int getAnalogData(int devNum=0)
   {
      vjASSERT(devNum < (int)mAnaData.size());    // Make sure we have enough space
      return mAnaData[devNum];
   }

   /* These functions don't do anything */
   int startSampling() { return 1; }
   int stopSampling() { return 1; }
   int sample() { return 1; }

   //: Update the data
   virtual void updateData();

    //: Get the name of the device
   char* getDeviceName() { return "vjSimAnalog";}

   static std::string getChunkType() { return std::string("SimAnalog");}

private:
   std::vector<int>          mAnaData;     //: The analog data that we have
   std::vector<vjKeyModPair> mSimKeysUp;   //: The keys to press for moving analog up
   std::vector<vjKeyModPair> mSimKeysDown; //: The keys to press for moving analog up
   int                     mAnaStep;      //: The analog step size
};

#endif
