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


#ifndef _VJ_DUMMY_GESTURE_H_
#define _VJ_DUMMY_GESTURE_H_

//: A dummy gesture device for gesture devices
//
// See also: vjGesture
//!PUBLIC_API:
class vjDummyGesture : public vjGesture
{
public:
   vjDummyGesture() : vjGesture()
   { active = 1; }
   ~vjDummyGesture() {}

   /* Dummies do nothing */
   int startSampling() { return 1; }
	int stopSampling() { return 0; }
	int sample() { return 1;}
	void updateData() { return ; }

   char* getDeviceName()  {return "vjDummyGesture"; }

   //: Retuns the id of the named gesture.
   //! RETURNS: -1 - Gesture name Not found.
   virtual int getGestureIndex(std::string name) { return -1; }

   //: Not implemented
   // returns null string
   virtual std::string getGestureString(int id) { return std::string(""); }

   //: Not implemented
   virtual void train() { }

   //: Get the current gesture. Not implemented
   //! RETURNS: -1
   virtual int getGesture() { return -1; }

   //: Not implemented
   virtual int createGesture(std::string gestureName) { return -1; }

   //: Not implemented
   virtual void addSample(int gestureId) { }

   //: Not implemented
   virtual void loadSamplesFile(std::string filename) { }

   //: Not implemented
   virtual void clearSamples(int gestureId=-1) { }

   //: Not implemented
   virtual void loadTrainedFile(std::string fileName) {}

   //: Not implemented
   virtual void saveTrainedFile(std::string fileName) {}

   //: Not implemented
   virtual void saveSamplesFile(std::string filename) {}
};

#endif
