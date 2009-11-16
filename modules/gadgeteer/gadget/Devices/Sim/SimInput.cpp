/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2009 by Iowa State University
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
 *************** <auto-copyright.pl END do not edit this line> ***************/

#include <gadget/gadgetConfig.h>

#include <jccl/Config/ConfigElement.h>
#include <gadget/Devices/Sim/SimInput.h>

namespace gadget
{

/** Constructs the mod pair from a mod pair element. */
std::vector<SimInput::KeyModPair> SimInput::readKeyList(std::vector<jccl::ConfigElementPtr>& keyList)
{
   std::vector<KeyModPair> key_vec;

   if ( ! keyList.empty() )
   {
#ifdef GADGET_DEBUG
      jccl::ConfigElementPtr first_element = keyList[0];
      std::string element_type(first_element->getID());
      vprASSERT(element_type == std::string("key_modifier_pair"));
#endif

      for ( std::vector<jccl::ConfigElementPtr>::iterator i = keyList.begin();
            i != keyList.end();
            ++i )
      {
         key_vec.push_back(KeyModPair((gadget::Keys) (*i)->getProperty<int>("key"),
                                      (gadget::Keys) (*i)->getProperty<int>("modifier_key")));
      }
   }

   return key_vec;
}


/**
 * Configures the keyboard interface.  Grabs it out of the given config element.
 */
bool SimInput::config(jccl::ConfigElementPtr element)
{
   // Get the event source.
   mKeyboardMouse.init(element->getProperty<std::string>("keyboard_mouse_proxy"));

   return true;
}

int SimInput::checkKeyPair(KeyModPair& pair)
{
   // If the modifier is any
   //   - Then check to see if the key is pressed.
   // Else
   //   - The modifier and the key must be pressed.
   
   // NOTE: If the modifier is NONE, then if any modifier
   //       is pressed we will return 0;

   if (pair.mModifier == -1)
   {
      return mKeyboardMouse->keyPressed(pair.mKey);
   }
   if (mKeyboardMouse->modifierOnly(pair.mModifier))
   {
      return mKeyboardMouse->keyPressed(pair.mKey);
   }
   else
   {
      return 0;
   }
}

} // End of gadget namespace
