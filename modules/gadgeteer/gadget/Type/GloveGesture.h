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


#ifndef _VJ_GLOVE_GESTURE_H_
#define _VJ_GLOVE_GESTURE_H_
//#pragma once

#include <vjConfig.h>
#include <Input/vjInput/vjInput.h>
#include <Input/vjGesture/vjGesture.h>
#include <Input/vjGlove/vjGlove.h>
#include <Input/InputManager/vjGloveProxy.h>
#include <Config/vjConfigChunk.h>

namespace vrj
{
   
//: Abstract base class for all glove gesture recognition
//
//
//!PUBLIC_API:
class GloveGesture : public Gesture
{
public:
   GloveGesture() {
      mGloveProxy = NULL;
   }

   virtual bool config(ConfigChunk* c)
   { return Gesture::config(c);}

public:
   //: Get a gesture name
   //! RETURNS: Name of gesture with the given id (gestureId)
   // NOTE: if gestureId = -1, returns name of current gesture
   virtual std::string getGestureString(int gestureId = -1);

   //: Create a new gesture
   //! ARGS: gestureName - Name of the new gesture
   //! RETURNS: int id of the new gesture
   virtual int createGesture(std::string gestureName);

   //: Load the header of a glove data file.
   // This is both for the samples and for the trained files.
   //
   // The header format is:
   //   -Comments -- # starting
   //   - <num gestures>
   //   -Gesture names
   //   -Gesture samples
   // infile is a file that is already open and ready for reading.
   //
   //!POST: After running, this routines will leave the file pointer immedately after
   // the header.
   //!POST: mGestureNames & mGestureExamples will be filled with correct data
   virtual void loadFileHeader(std::ifstream& infile);

   //: Save a file header
   //! NOTE: The client of this routine may add their own initial lines to the
   //+ header as long as they remove them before calling loadFileHeader.
   virtual void saveFileHeader(std::ofstream& outFile);

   //: Return the identifier of the string gesture.
   //! RETURNS: -1 if not found
   virtual int getGestureIndex(std::string gestureName);

   //: Return the number of gestures in system
   virtual int getNumGestures()
   { return mGestureNames.size(); }

protected:
   // mGestureNames and mGestureExamples MUST always be same size
   std::vector<std::string> mGestureNames;      //: List of all gesture names
   std::vector<GloveData> mGestureExamples;   //: Examples of all gestures
   GloveProxy*            mGloveProxy;        //: Proxy to the glove
};

};

#endif
