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

#ifndef _VRJ_SIM_GLOVE_GESTURE_H_
#define _VRJ_SIM_GLOVE_GESTURE_H_

#include <gad/gadConfig.h>
#include <gad/Type/Input.h>
#include <gad/Type/Digital.h>
#include <gad/Type/GloveGesture.h>
#include <gad/Type/Glove.h>
#include <gad/Type/Gesture.h>
#include <gad/Devices/Sim/SimInput.h>

namespace vrj
{


//: Simulated glove class
//
// This class simulates a gesture input device
//
// By default the glove is in gesture 0
//!PUBLIC_API:
class SimGloveGesture
   :  virtual public Input, public GloveGesture,  public Glove,  public Digital, public SimInput
{
public:
   //: Construct a SimGloveGesture
   SimGloveGesture() {;}

   virtual bool config(ConfigChunk* chunk);

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

   static std::string getChunkType() { return std::string("SimGloveGesture");}

   /*** These are not supported in sim ***/
   void saveTrainedFile(std::string fileName) { ;}
   void loadSamplesFile(std::string filename) {;}
   void saveSamplesFile(std::string filename) {;}
   void clearSamples(int gestureId=-1) {;}
   void addSample(int gestureId) {;}
   void train() { ; }

private:
   std::vector<KeyModPair> mSimKeys;    //: The keys to press for the gestures
   int                     mCurGesture;   //: The current gesture id
};

};

#endif
