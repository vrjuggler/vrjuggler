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


#include <vjConfig.h>
#include <Input/vjGesture/vjGloveGesture.h>

//: Get a gesture name
//! RETURNS: (gestureId in range) - string desc of gesture
//! RETURNS: (gesture not in range) - empty string
std::string vjGloveGesture::getGestureString(int gestureId)
{
   if(gestureId < 0)
      return getGestureString(this->getGesture());    // Get string of current gesture
   else if(gestureId < mGestureNames.size())
      return mGestureNames[gestureId];
   else
      return std::string("");
}

//: Create a new gesture
// Pushes the gesture onto the list of gesture names
// Also pushes an example on the vector
int vjGloveGesture::createGesture(std::string gestureName)
{
   mGestureNames.push_back(gestureName);        // Push it back
   mGestureExamples.push_back(vjGloveData());  // Push back an empty vector of floats
   vjASSERT(mGestureNames.size() == mGestureExamples.size());

   return (mGestureExamples.size() -1);
}

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
void vjGloveGesture::loadFileHeader(ifstream& infile)
{
   // skip comments
   while(infile.peek() == '#')
      infile.ignore(4096, '\n');    // Ignore the entire line.

   // Get num gestures
   int num_gestures;
   infile >> num_gestures;          // Get the number of gestures

   infile.ignore(4096, '\n');       // Ignore the rest of the line

   // Get gesture names
   int i;
   char gest_name[512];

   mGestureNames = std::vector<std::string>(num_gestures);
   for(i=0;i<num_gestures;i++)
   {
      infile.getline(gest_name,512);
      mGestureNames[i] = std::string(gest_name);
   }

   mGestureExamples = std::vector<vjGloveData>(num_gestures);

   // Get gesture data
   for(i=0;i<num_gestures;i++)
      mGestureExamples[i].inputAngles(infile);
}


//: Save a file header
//! NOTE: The client of this routine may add their own initial lines to the
//+ header as long as they remove them before calling loadFileHeader.
void vjGloveGesture::saveFileHeader(ofstream& outFile)
{
   vjASSERT(mGestureNames.size() == mGestureExamples.size());     // The must be same size

   outFile << "# vjGloveGesture v1.0" << endl
           << "#       vjGloveGesture Header" << endl
           << "#  Contains gesture names and examples" << endl
           << "#  As well and trainer information" << endl;

   outFile << mGestureNames.size() << endl;

   int i;
   for(i =0;i<mGestureNames.size();i++)
      outFile << mGestureNames[i].c_str() << endl;

   for(i=0;i<mGestureExamples.size();i++)
   {
      mGestureExamples[i].outputAngles(outFile);
      outFile << endl;
   }
}

//: Return the gesture identifier of the gesture.
int vjGloveGesture::getGestureIndex(std::string gestureName)
{
   int i = 0;
   int found = -1;
   while((found <0) && (i<mGestureNames.size()))
   {
      if(mGestureNames[i] == gestureName)
         found = i;
      i++;
   }

   return found;
}
