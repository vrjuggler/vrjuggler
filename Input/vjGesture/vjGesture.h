#ifndef _VJ_GESTURE_H_
#define _VJ_GESTURE_H_
#pragma once

#include <vjConfig.h>
#include <Input/vjInput/vjInput.h>


//: Base Juggler Gesture interface class
// This class is the base interface for all gesture recognition objects
//
class vjGesture : virtual public vjInput
{
public:
   vjGesture(vjConfigChunk* c) : vjInput(c)
   {;}

   vjGesture()
   {;}

   ~vjGesture()
   {;}


public:  // **** GET GESTURES **** //
   //: Retuns the id of the named gesture.
   //! RETURNS: -1 - Gesture name Not found.
   virtual int getGestureIndex(string name) = 0;

   //: Get the string name of the gesture.
   // If id is -1, then it returns the string name of the current gesture.
   virtual string getGestureString(int id) = 0;

   //: Get the current gesture.
   //! RETURNS: id of current gesture
   virtual int getGesture() = 0;


public:  // **** TRAINING **** //

   //: This creates a new gesture with the given name.
   // Returns the identifier of the new gesture.
   virtual int createGesture(string gestureName) = 0;

   //: Adds a new sample of the given gesture to the training data.
   virtual void addSample(int gestureId) = 0;

   //: This actually starts the training on the given data.
   virtual void train() = 0;

   //: Clear the samples that we have taken so far.
   //! ARGS: gestureId = -1 (default) then we clear all gestures else we clear only
   // the gesture that is specified.
   virtual void clearSamples(int gestureId = -1) = 0;

   //: Load trained data for the gesture object
   virtual void loadTrainedFile(string fileName) = 0;

   //: Save a trained data file for the gesture object
   virtual void saveTrainedFile(string fileName) = 0;

   //: Loads the sample training data specified.
   // This file contains previous samples for the gesture recognizer to train from.
   virtual void loadSamplesFile(string filename) = 0;

   //: Saves the sample training data specified.
   // This data can be loaded at a later time to do more sample training.
   virtual void saveSamplesFile(string filename) = 0;

};

#endif
