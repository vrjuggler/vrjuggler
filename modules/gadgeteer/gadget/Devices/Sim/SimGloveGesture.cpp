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
   std::string sample_file = (char*)chunk->getProperty("trainedFilename");
   loadTrainedFile(sample_file);

   // Trim the lengths
   int num_gestures = getNumGestures();
   while(num_gestures < mSimKeys.size())     // If we have to many keys
   {
      mSimKeys.pop_back();
      vjDEBUG(0) << "vjSimGloveGesture: Not enough gestures. Trimming" << endl << vjDEBUG_FLUSH;
   }

   // Find pos proxy
   std::string glove_pos_proxy = (char*)chunk->getProperty("glovePos");    // Get the name of the pos_proxy
   if(glove_pos_proxy == std::string(""))
   {
      vjDEBUG(0) << "ERROR: vjSimGloveGesture has no posProxy." << endl << vjDEBUG_FLUSH;
      return false;
   }
   // init glove proxy interface
   int proxy_index = vjKernel::instance()->getInputManager()->GetProxyIndex(glove_pos_proxy);
   if(proxy_index != -1)
      mGlovePos[0] = vjKernel::instance()->getInputManager()->GetPosProxy(proxy_index);
   else
      vjDEBUG(0) << "ERROR: vjSimGloveGesture::vjCyberGlove: Can't find posProxy." << endl << vjDEBUG_FLUSH << endl;

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
void vjSimGloveGesture::UpdateData()
{
   // Get the current gesture
   for(int i=0;i<mSimKeys.size();i++)
   {
      if(checkKeyPair(mSimKeys[i]) > 0)
      {
         mCurGesture = i;
         vjDEBUG(1) << "vjSimGloveGesture: Got gesture: " << getGestureString(mCurGesture) << endl << vjDEBUG_FLUSH;

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
      vjDEBUG(0) << "vjSimGloveGesture:: Can't load trained file: " << fileName << endl << vjDEBUG_FLUSH;
   }
}

