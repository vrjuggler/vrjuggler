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

#ifndef _GADGET_GESTURE_H_
#define _GADGET_GESTURE_H_
//#pragma once

#include <gadget/gadgetConfig.h>
#include <jccl/Config/ConfigChunk.h>


namespace gadget
{

//-------------------------------------------------------------------------
//: Base Juggler Gesture interface class
// This class is the base interface for all gesture recognition objects
//
// All gesture objects have two parts to their interfaces. <br>
// Getting gestures and Trainging new gestures
//
// In order to retrieve gesture information from the device,
// the user call the get functions to get the index, or
// string representation of the current gesture.
//
//-------------------------------------------------------------------------
//!PUBLIC_API:
class Gesture
{
public:
   Gesture()
   {}

   virtual ~Gesture()
   {;}

   virtual bool config(jccl::ConfigChunk* c)
   { return true;}


public:  // **** GET GESTURES **** //
   //: Retuns the id of the named gesture.
   //! RETURNS: -1 - Gesture name Not found.
   virtual int getGestureIndex(std::string name) = 0;

   //: Get the string name of the gesture.
   // If id is -1, then it returns the string name of the current gesture.
   virtual std::string getGestureString(int id) = 0;

   //: Get the current gesture.
   //! RETURNS: id of current gesture
   virtual int getGesture() = 0;


public:  // **** TRAINING **** //

   //: This creates a new gesture with the given name.
   // Returns the identifier of the new gesture.
   virtual int createGesture(std::string gestureName) = 0;

   //: Adds a new sample of the given gesture to the training data.
   virtual void addSample(int gestureId) = 0;

   //: This actually starts the training on the given data.
   virtual void train() = 0;

   //: Clear the samples that we have taken so far.
   //! ARGS: gestureId = -1 (default) then we clear all gestures else we clear only
   // the gesture that is specified.
   virtual void clearSamples(int gestureId = -1) = 0;

   //: Load trained data for the gesture object
   virtual void loadTrainedFile(std::string fileName) = 0;

   //: Save a trained data file for the gesture object
   virtual void saveTrainedFile(std::string fileName) = 0;

   //: Loads the sample training data specified.
   // This file contains previous samples for the gesture recognizer to train from.
   virtual void loadSamplesFile(std::string filename) = 0;

   //: Saves the sample training data specified.
   // This data can be loaded at a later time to do more sample training.
   virtual void saveSamplesFile(std::string filename) = 0;

};

};

#endif
