#include <vjConfig.h>
#include <Input/vjSim/vjSimAnalog.h>

bool vjSimAnalog::config(vjConfigChunk* chunk)
{
   if((!vjAnalog::config(chunk)) || (!vjSimInput::config(chunk)))
      return false;

   std::vector<vjVarValue*> key_inc_list = chunk->getAllProperties("keyPairsInc");
   std::vector<vjVarValue*> key_dec_list = chunk->getAllProperties("keyPairsDec");

   mSimKeysUp = readKeyList(key_inc_list);
   mSimKeysDown = readKeyList(key_dec_list);

   int num_pairs = mSimKeysUp.size();

   mAnaData = std::vector<int>(num_pairs,0); // Initialize to all zeros
   mAnaStep = chunk->getProperty("anastep");

   return true;
}

void vjSimAnalog::updateData()
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
