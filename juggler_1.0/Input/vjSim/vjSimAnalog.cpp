#include <vjConfig.h>
#include <Input/vjSim/vjSimAnalog.h>

bool vjSimAnalog::config(vjConfigChunk* chunk)
{
   if((!vjAnalog::config(chunk)) || (!vjSimInput::config(chunk)))
      return false;

   // Get keyboard pairs
   int num_keys_up = chunk->getNum("keysUp");
   int num_mods_up = chunk->getNum("modKeysUp");
   int num_keys_down = chunk->getNum("keysDown");
   int num_mods_down = chunk->getNum("modKeysDown");
   int num_pairs = VJ_MIN4(num_keys_up, num_mods_up, num_keys_down, num_mods_down);   // Trim to smaller value

   // Fill the key pairs
   for(int i=0;i<num_pairs;i++)
   {
      vjKeyModPair key_pair;
      // Up pair
      key_pair.mKey      = (int)chunk->getProperty("keysUp",i);
      key_pair.mModifier = (int)chunk->getProperty("modKeysUp",i);
      mSimKeysUp.push_back(key_pair);
      // Down pair
      key_pair.mKey      = (int)chunk->getProperty("keysDown",i);
      key_pair.mModifier = (int)chunk->getProperty("modKeysDown",i);
      mSimKeysDown.push_back(key_pair);
   }

   vjASSERT(mSimKeysUp.size() == mSimKeysDown.size());   // If trimming worked, then they should be same size

   mAnaData = std::vector<int>(num_pairs,0); // Initialize to all zeros
   mAnaStep = chunk->getProperty("anastep");

   return true;
}

void vjSimAnalog::UpdateData()
{
   // -- Update analog data --- //
   for (int i = 0; i < mSimKeysUp.size(); i++)
   {
      mAnaData[i] += checkKeyPair(mSimKeysUp[i]) * mAnaStep;
      mAnaData[i] -= checkKeyPair(mSimKeysDown[i]) * mAnaStep;

      if (mAnaData[i] < 0) mAnaData[i] = 0;
      if (mAnaData[i] > 255) mAnaData[i] = 255;
   }
}
