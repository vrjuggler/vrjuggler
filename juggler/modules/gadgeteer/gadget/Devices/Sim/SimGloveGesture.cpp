/*
 *  File:	    $RCSfile$
 *  Date modified:  $Date$
 *  Version:	    $Revision$
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
#include <Input/vjSim/vjSimGloveGesture.h>

//: Construct the vjSimGloveGesture
// -Set the keyboard key pairs <b>
// -Load the sample file
// -Trim the smallest so they are same length
// -Find/Set pos proxy for glove
bool vjSimGloveGesture::config(vjConfigChunk* chunk)
{
   if((!vjGloveGesture::config(chunk)) || (!vjSimInput::config(chunk)))
      return false;

   mCurGesture = 0;     // We are in no gesture yet

   std::vector<vjVarValue*> key_list = chunk->getAllProperties("keyPairs");
   mSimKeys = readKeyList(key_list);

   // Get sample filename
   std::string sample_file = chunk->getProperty("trainedFilename");
   loadTrainedFile(sample_file);

   // Trim the lengths
   int num_gestures = getNumGestures();
   while(num_gestures < mSimKeys.size())     // If we have to many keys
   {
      mSimKeys.pop_back();
      vjDEBUG(vjDBG_INPUT_MGR,1) << "vjSimGloveGesture: Not enough gestures. Trimming" << endl << vjDEBUG_FLUSH;
   }

   // Find pos proxy
   std::string glove_pos_proxy = chunk->getProperty("glovePos");    // Get the name of the pos_proxy
   if(glove_pos_proxy == std::string(""))
   {
      vjDEBUG(vjDBG_INPUT_MGR,0) << "ERROR: vjSimGloveGesture has no posProxy." << endl << vjDEBUG_FLUSH;
      return false;
   }
   // init glove proxy interface
   int proxy_index = vjKernel::instance()->getInputManager()->getProxyIndex(glove_pos_proxy);
   if(proxy_index != -1)
      mGlovePos[0] = vjKernel::instance()->getInputManager()->getPosProxy(proxy_index);
   else
      vjDEBUG(vjDBG_INPUT_MGR,0) << "ERROR: vjSimGloveGesture::vjCyberGlove: Can't find posProxy." << endl << vjDEBUG_FLUSH << endl;

   // Set the indexes to defaults
   resetIndexes();

   return true;
}

//: Get the current gesture.
//! RETURNS: id of current gesture
int vjSimGloveGesture::getGesture()
{ return mCurGesture; }

//: Update the device data
// -Get the gesture id
// -Set the glove params
void vjSimGloveGesture::updateData()
{
   // Get the current gesture
   for(int i=0;i<mSimKeys.size();i++)
   {
      if(checkKeyPair(mSimKeys[i]) > 0)
      {
         mCurGesture = i;
         vjDEBUG(vjDBG_INPUT_MGR,3) << "vjSimGloveGesture: Got gesture: " << getGestureString(mCurGesture).c_str() << endl << vjDEBUG_FLUSH;

         // Set the glove to the sample
         mTheData[0][current] = mGestureExamples[mCurGesture];    // Copy over the example
         mTheData[0][current].calcXforms();                       // Update the xform data
      }
   }

}


//: Load trained data for the gesture object
// Loads the file for trained data
void vjSimGloveGesture::loadTrainedFile(std::string fileName)
{
   ifstream inFile(fileName.c_str());

   if(inFile)
   {
      this->loadFileHeader(inFile);
      inFile.close();                     // Close the file
   }
   else
   {
      vjDEBUG(vjDBG_INPUT_MGR,0) << "vjSimGloveGesture:: Can't load trained file: " << fileName.c_str() << endl << vjDEBUG_FLUSH;
   }
}

