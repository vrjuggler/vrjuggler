#include <Input/vjSim/vjSimDigital.h>

bool vjSimDigital::config(vjConfigChunk* chunk)
{
   if((!vjDigital::config(chunk)) || (!vjSimInput::config(chunk)))
      return false;

   // Get keyboard pairs
   int num_keys = chunk->getNum("keys");
   int num_mods = chunk->getNum("modKeys");
   int num_pairs = VJ_MIN2(num_keys, num_mods);   // Trim to smaller value

   // Fill the key pairs
   for(int i=0;i<num_pairs;i++)
   {
      vjKeyModPair key_pair;
      // Down pair
      key_pair.mKey      = (int)chunk->getProperty("keys",i);
      key_pair.mModifier = (int)chunk->getProperty("modKeys",i);
      mSimKeys.push_back(key_pair);
   }

   mDigitalData = vector<int>(num_pairs,0);      // Initialize to all zeros

   return true;
}

// Updates the state of the digital data vector <br>
//
// NOTE: Digital is on when key is held down
//+     When key is release, digital goes to off state
void vjSimDigital::UpdateData()
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
