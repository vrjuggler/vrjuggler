#include <Input/vjSim/vjSimInput.h>

// Configure the keyboard interface
// Grabs it out of the given config chunk
bool vjSimInput::config(vjConfigChunk* chunk)
{
   string keyboardName = (char*)chunk->getProperty("keyboardProxy");    // Get the event source
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
