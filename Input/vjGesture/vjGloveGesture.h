#ifndef _VJ_GLOVE_GESTURE_H_
#define _VJ_GLOVE_GESTURE_H_
#pragma once

#include <vjConfig.h>
#include <Input/vjGesture/vjGesture.h>

//: Abstract base class for all glove gesture recognition
//
//
class vjGloveGesture : public vjGesture
{
public:
   vjGesture(vjConfigChunk* c) : vjGesture(c)
   {;}

   vjGesture()
   {;}

public:
   //: Get a gesture name
   //! RETURNS: Name of gesture with the given id (gestureId)
   // NOTE: if gestureId = -1, returns name of current gesture
   virtual void getGestureString(int gestureId = -1);

   //: Create a new gesture
   //! ARGS: gestureName - Name of the new gesture
   //! RETURNS: int id of the new gesture
   virtual int createGesture(string gestureName);

   //: Load the header of a glove data file.
   // This is both for the samples and for the trained files.
   //
   // The header format is:
   //   -Comments -- # starting
   //   - <num gestures>
   //   -Gesture names
   //   -Gesture samples
   // infile is a file that is already open and ready for reading.
   // After running, this routines will leave the file pointer immedately after
   // the header.
   virtual void loadFileHeader(ifstream& infile);


   //: Save a file header
   //! NOTE: The client of this routine may add their own initial lines to the
   //+ header as long as they remove them before calling loadFileHeader.
   virtual void saveFileHeader(ofstream& outFile);

   //: Return the string identifier of the gesture.
   virtual void getGestureIndex(int retval, string gestureName);

protected:
   vector<string>          mGestureNames;       //: List of all gesture names
   vector<vjGloveData>     mGestureExamples;    //: Examples of all gestures
   vjGloveProxy*           mGloveProxy;         //: Proxy to the glove
};

#endif
