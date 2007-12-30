/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2007 by Iowa State University
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
 *************** <auto-copyright.pl END do not edit this line> ***************/

#include <gadget/gadgetConfig.h>
#include <fstream>
#include <gadget/Type/GloveGesture.h>

namespace gadget
{

/**
 * Gets a gesture name.
 * @return (gestureId in range) - string desc of gesture
 * @return (gesture not in range) - empty string
 */
std::string GloveGesture::getGestureString(const int gestureId) const
{
   if(gestureId < 0)
      return getGestureString(this->getGesture());    // Get string of current gesture
   else if((unsigned)gestureId < mGestureNames.size())
      return mGestureNames[gestureId];
   else
      return std::string("");
}

/**
 * Creates a new gesture.
 * Pushes the gesture onto the list of gesture names.
 * Also pushes an example on the vector.
 */
int GloveGesture::createGesture(std::string gestureName)
{
   mGestureNames.push_back(gestureName);        // Push it back
   mGestureExamples.push_back(GloveData());  // Push back an empty vector of floats
   vprASSERT(mGestureNames.size() == mGestureExamples.size());

   return (mGestureExamples.size() -1);
}

/**
 * Loads the header of a glove data file.
 * This is both for the samples and for the trained files.
 *
 * The header format is:
 *   -Comments -- # starting<br>
 *   - num gestures<br>
 *   -Gesture names<br>
 *   -Gesture samples<br>
 *
 * @param infile A file that is already open and ready for reading.
 *               After running, this routines will leave the file pointer
 *               immedately after the header.
 */
void GloveGesture::loadFileHeader(std::ifstream& infile)
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

   mGestureExamples = std::vector<GloveData>(num_gestures);

   // Get gesture data
   for(i=0;i<num_gestures;i++)
      mGestureExamples[i].inputAngles(infile);
}


/**
 * Saves a file header.
 * @note The client of this routine may add their own initial lines to the
 *       header as long as they remove them before calling loadFileHeader.
 */
void GloveGesture::saveFileHeader(std::ofstream& outFile)
{
   vprASSERT(mGestureNames.size() == mGestureExamples.size());     // The must be same size

   outFile << "# GloveGesture v1.0" << std::endl
           << "#       GloveGesture Header" << std::endl
           << "#  Contains gesture names and examples" << std::endl
           << "#  As well and trainer information" << std::endl;

   outFile << mGestureNames.size() << std::endl;

   unsigned int i;
   for(i =0;i<mGestureNames.size();i++)
      outFile << mGestureNames[i].c_str() << std::endl;

   for(i=0;i<mGestureExamples.size();i++)
   {
      mGestureExamples[i].outputAngles(outFile);
      outFile << std::endl;
   }
}

/** Returns the gesture identifier of the gesture. */
int GloveGesture::getGestureIndex(const std::string& gestureName) const
{
   unsigned i = 0;
   int found = -1;
   while((found <0) && (i<mGestureNames.size()))
   {
      if(mGestureNames[i] == gestureName)
         found = i;
      i++;
   }

   return found;
}

} // End of gadget namespace
