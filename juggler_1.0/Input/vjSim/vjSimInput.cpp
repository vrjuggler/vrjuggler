#include <Input/vjSim/vjSimInput.h>
#include <Config/vjConfigChunk.h>

//: Construct the mod pair from a mod pair chunk
std::vector<vjSimInput::vjKeyModPair> vjSimInput::readKeyList(std::vector<vjVarValue*>& keyList)
{
   if(keyList.size() > 0)
   {
#ifdef VJ_DEBUG
      vjConfigChunk* first_chunk = (vjConfigChunk*)(*(keyList[0]));
      std::string chunk_type = first_chunk->getType();
      vjASSERT(chunk_type == std::string("KeyModPair"));
#endif

      std::vector<vjKeyModPair> keys;
      int num_keys = keyList.size();

      for(int i=0;i<num_keys;i++)
      {
         vjKeyModPair key_pair;
         vjVarValue* var_val = keyList[i];
         vjConfigChunk* chunk = (vjConfigChunk*)(*var_val);
         key_pair.mKey = chunk->getProperty("key");
         key_pair.mModifier = chunk->getProperty("modKey");
         keys.push_back(key_pair);
      }

      return keys;
   }
   else
      return std::vector<vjKeyModPair>();
}


// Configure the keyboard interface
// Grabs it out of the given config chunk
bool vjSimInput::config(vjConfigChunk* chunk)
{
   std::string keyboardName = chunk->getProperty("keyboardProxy");    // Get the event source
   mKeyboard.init(keyboardName);

   return true;
}

int vjSimInput::checkKeyPair(vjKeyModPair& pair)
{
   if (mKeyboard->modifierOnly(pair.mModifier))
   {
      return mKeyboard->keyPressed(pair.mKey);
   }
   else                 // Mod not pressed
      return 0;
}

int vjSimInput::checkKey(int keyId)
{
   return mKeyboard->keyPressed(keyId);
}
