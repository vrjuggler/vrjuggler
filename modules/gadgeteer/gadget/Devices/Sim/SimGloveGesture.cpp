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

#include <vrj/vrjConfig.h>

#include <vrj/Util/FileIO.h>
#include <vrj/Input/Devices/Sim/SimGloveGesture.h>
#include <vrj/Config/ConfigChunk.h>

namespace vrj
{

//: Construct the SimGloveGesture
// -Set the keyboard key pairs <b>
// -Load the sample file
// -Trim the smallest so they are same length
// -Find/Set pos proxy for glove
bool SimGloveGesture::config(ConfigChunk* chunk)
{
   if((!GloveGesture::config(chunk)) || (!SimInput::config(chunk)))
      return false;

   mCurGesture = 0;     // We are in no gesture yet

   std::vector<VarValue*> key_list = chunk->getAllProperties("keyPairs");
   mSimKeys = readKeyList(key_list);

   // Get sample filename
   std::string sample_file = chunk->getProperty("trainedFilename");
   loadTrainedFile(FileIO::replaceEnvVars(sample_file));

   // Trim the lengths
   unsigned int num_gestures = getNumGestures();
   while(num_gestures < mSimKeys.size())     // If we have to many keys
   {
      mSimKeys.pop_back();
      vjDEBUG(vjDBG_INPUT_MGR,1)
         << "vjSimGloveGesture: Not enough gestures. Trimming" << std::endl
         << vjDEBUG_FLUSH;
   }

   // Find pos proxy
   std::string glove_pos_proxy = chunk->getProperty("glovePos");    // Get the name of the pos_proxy
   if(glove_pos_proxy == std::string(""))
   {
      vjDEBUG(vjDBG_INPUT_MGR,0) << clrOutNORM(clrRED, "ERROR:") << " SimGloveGesture has no posProxy."
                                 << std::endl << vjDEBUG_FLUSH;
      return false;
   }
   // init glove proxy interface
   /*
   int proxy_index = Kernel::instance()->getInputManager()->getProxyIndex(glove_pos_proxy);
   if(proxy_index != -1)
      mGlovePos[0] = Kernel::instance()->getInputManager()->getPosProxy(proxy_index);
   else
      vjDEBUG(vjDBG_INPUT_MGR,0)
         << clrOutNORM(clrRED, "ERROR:") << " SimGloveGesture::CyberGlove: Can't find posProxy."
         << std::endl << std::endl << vjDEBUG_FLUSH;
   */

   // Set the indexes to defaults
   resetIndexes();

   return true;
}

//: Get the digital data for the given devNum
//  Returns digital 0 or 1, if devNum makes sense.<BR>
//  Returns -1 if function fails or if devNum is out of range.<BR>
//  NOTE: If devNum is out of range, function will fail, possibly issueing
//  an error to a log or console - but will not ASSERT.<BR>
int SimGloveGesture::getDigitalData(int devNum)
{
   int openLookupTable[] = { 0,0,0,0,0,-1,0,0,0,0,0 };
   int closedLookupTable[] = { 1,1,1,1,1,-1,1,1,1,1,1 };
   int pointingLookupTable[] = { 1,1,1,0,1,-1,1,1,1,0,1 };
   switch (mCurGesture)
   {
      case 0:
         //open
         return openLookupTable[devNum];
      case 1:
         //closed
         return closedLookupTable[devNum];
      case 2:
         // pointing
         return pointingLookupTable[devNum];
      default:
         return openLookupTable[devNum];
         //return -1;// function failed
   }
}

//: Get the current gesture.
//! RETURNS: id of current gesture
int SimGloveGesture::getGesture()
{ return mCurGesture; }

//: Update the device data
// -Get the gesture id
// -Set the glove params
void SimGloveGesture::updateData()
{
   // Get the current gesture
   for(unsigned int i=0;i<mSimKeys.size();i++)
   {
      if(checkKeyPair(mSimKeys[i]) > 0)
      {
         mCurGesture = i;
         vjDEBUG(vjDBG_INPUT_MGR,3) << "vjSimGloveGesture: Got gesture: "
                                    << getGestureString(mCurGesture).c_str()
                                    << std::endl << vjDEBUG_FLUSH;

         // Set the glove to the sample
         mTheData[0][current] = mGestureExamples[mCurGesture];    // Copy over the example
         mTheData[0][current].calcXforms();                       // Update the xform data
      }
   }

}


//: Load trained data for the gesture object
// Loads the file for trained data
void SimGloveGesture::loadTrainedFile(std::string fileName)
{
   std::ifstream inFile(fileName.c_str());

   if(inFile)
   {
      this->loadFileHeader(inFile);
      inFile.close();                     // Close the file
   }
   else
   {
      vjDEBUG(vjDBG_INPUT_MGR,0)
         << "vjSimGloveGesture:: Can't load trained file: "
         << fileName.c_str() << std::endl << vjDEBUG_FLUSH;
   }
}

};
