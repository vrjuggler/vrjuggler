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


#ifndef _VJ_SIM_GLOVE_GESTURE_H_
#define _VJ_SIM_GLOVE_GESTURE_H_

#include <vjConfig.h>
#include <Input/vjGesture/vjGloveGesture.h>
#include <Input/vjGesture/vjGesture.h>
#include <Input/vjSim/vjSimInput.h>


//: Simulated glove class
//
// This class simulates a gesture input device
//
// By default the glove is in gesture 0
//!PUBLIC_API:
class vjSimGloveGesture
   : virtual public vjGloveGesture, virtual public vjGlove, virtual public vjDigital, public vjSimInput
{
public:
   //: Construct a vjSimGloveGesture
   vjSimGloveGesture() {;}

   virtual bool config(vjConfigChunk* chunk);

   //: Get the current gesture.
   //! RETURNS: id of current gesture
   virtual int getGesture();

   virtual int getDigitalData(int devNum = 0);
   
   //: Load trained data for the gesture object
   // Loads the file for trained data
   void loadTrainedFile(std::string fileName);

   int startSampling() { return 1; }
   int stopSampling() { return 1; }
   int sample() { return 1; }

   //: Update the device data
   void updateData ();
   
   //: Return our name
   char* getDeviceName() { return "vjSimGloveGesture"; }
   static std::string getChunkType() { return std::string("SimGloveGesture");}

   /*** These are not supported in sim ***/
   void saveTrainedFile(std::string fileName) { ;}
   void loadSamplesFile(std::string filename) {;}
   void saveSamplesFile(std::string filename) {;}
   void clearSamples(int gestureId=-1) {;}
   void addSample(int gestureId) {;}
   void train() { ; }

private:
   std::vector<vjKeyModPair> mSimKeys;    //: The keys to press for the gestures
   int                     mCurGesture;   //: The current gesture id
};

#endif
