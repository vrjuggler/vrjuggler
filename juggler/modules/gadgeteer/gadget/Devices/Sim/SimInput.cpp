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


#include <Input/vjSim/vjSimInput.h>
#include <Config/vjConfigChunk.h>

namespace vrj
{
   
//: Construct the mod pair from a mod pair chunk
std::vector<SimInput::KeyModPair> SimInput::readKeyList(std::vector<VarValue*>& keyList)
{
   if(keyList.size() > 0)
   {
#ifdef VJ_DEBUG
      ConfigChunk* first_chunk = (ConfigChunk*)(*(keyList[0]));
      std::string chunk_type = first_chunk->getType();
      vprASSERT(chunk_type == std::string("KeyModPair"));
#endif

      std::vector<KeyModPair> keys;
      int num_keys = keyList.size();

      for(int i=0;i<num_keys;i++)
      {
         KeyModPair key_pair;
         VarValue* var_val = keyList[i];
         ConfigChunk* chunk = (ConfigChunk*)(*var_val);
         key_pair.mKey = chunk->getProperty("key");
         key_pair.mModifier = chunk->getProperty("modKey");
         keys.push_back(key_pair);
      }

      return keys;
   }
   else
      return std::vector<KeyModPair>();
}


// Configure the keyboard interface
// Grabs it out of the given config chunk
bool SimInput::config(ConfigChunk* chunk)
{
   std::string keyboardName = chunk->getProperty("keyboardProxy");    // Get the event source
   mKeyboard.init(keyboardName);

   return true;
}

int SimInput::checkKeyPair(KeyModPair& pair)
{
   if (pair.mModifier == -1)      // ANY modifier
   {
      return mKeyboard->keyPressed(pair.mKey);
   }
   if (mKeyboard->modifierOnly(pair.mModifier))
   {
      return mKeyboard->keyPressed(pair.mKey);
   }
   else                 // Mod not pressed
      return 0;
}

int SimInput::checkKey(int keyId)
{
   return mKeyboard->keyPressed(keyId);
}

};
