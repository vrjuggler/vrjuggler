#include <Input/vjSim/vjSimGloveGesture.h>
#include <iostream.h>
#include <fstream.h>

//: Construct the vjSimGloveGesture
// -Set the keyboard key pairs <b>
// -Load the sample file
// -Trim the smallest so they are same length
vjSimGloveGesture::vjSimGloveGesture(vjConfigChunk* chunk)
   :  vjGloveGesture(chunk), vjGlove(chunk), vjSimInput(chunk)
{
   mCurGesture = 0;     // We are in no gesture yet

   // Get keyboard pairs
   int num_keys = chunk->getNum("keys");
   int num_mods = chunk->getNum("modKeys");
   int num_pairs = ( (num_keys < num_mods) ? num_keys : num_mods);   // Trim to smaller value

   // Fill the key pairs
   for(int i=0;i<num_pairs;i++)
   {
      vjKeyModPair key_pair;
      key_pair.mKey      = (int)chunk->getProperty("keys",i);
      key_pair.mModifier = (int)chunk->getProperty("modKeys",i);
      mSimKeys.push_back(key_pair);
   }

   // Get sample filename
   string sample_file = (char*)chunk->getProperty("trainedFilename");
   loadTrainedFile(sample_file);

   // Trim the lengths
   int num_gestures = getNumGestures();
   while(num_gestures < mSimKeys.size())     // If we have to many keys
   {
      mSimKeys.pop_back();
      vjDEBUG(0) << "vjSimGloveGesture: Not enough gestures. Trimming" << endl << vjDEBUG_FLUSH;
   }

   resetIndexes();
}

//: Get the current gesture.
//! RETURNS: id of current gesture
int vjSimGloveGesture::getGesture()
{ return mCurGesture; }

//: Update the device data
// -Get the gesture id
// -Set the glove params
void vjSimGloveGesture::UpdateData()
{
   // Get the current gesture
   for(int i=0;i<mSimKeys.size();i++)
   {
      if(checkKeyPair(mSimKeys[i]) > 0)
      {
         mCurGesture = i;
         vjDEBUG(0) << "vjSimGloveGesture: Got gesture: " << getGestureString(mCurGesture) << endl << vjDEBUG_FLUSH;
      }
   }

   // Set the glove to the sample
   mTheData[0][current] = mGestureExamples[mCurGesture];    // Copy over the example
   mTheData[0][current].calcXforms();                       // Update the xform data
}


//: Load trained data for the gesture object
// Loads the file for trained data
void vjSimGloveGesture::loadTrainedFile(string fileName)
{
   ifstream inFile(fileName.c_str());

   this->loadFileHeader(inFile);
}

