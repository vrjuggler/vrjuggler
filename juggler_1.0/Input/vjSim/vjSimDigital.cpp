#include <vjConfig.h>
#include <Input/vjSim/vjSimDigital.h>

bool vjSimDigital::config(vjConfigChunk* chunk)
{
   if((!vjDigital::config(chunk)) || (!vjSimInput::config(chunk)))
      return false;

   std::vector<vjVarValue*> key_list = chunk->getAllProperties("keyPairs");
   mSimKeys = readKeyList(key_list);

   int num_pairs = mSimKeys.size();
   mDigitalData = std::vector<int>(num_pairs,0);      // Initialize to all zeros

   return true;
}

// Updates the state of the digital data vector <br>
//
// NOTE: Digital is on when key is held down
//+     When key is release, digital goes to off state
void vjSimDigital::updateData()
{
   // -- Update digital data --- //
   for (int i = 0; i < mSimKeys.size(); i++)
   {
      if(checkKeyPair(mSimKeys[i]))             // If keys pressed
         mDigitalData[i] = 1;
      else
         mDigitalData[i] = 0;
   }
}
