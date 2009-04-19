/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2008 by Iowa State University
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

#include <gadget/Util/Debug.h>
#include <gadget/Devices/KeyboardMouseDevice/KeyboardMouseDevice.h>
#include <jccl/Config/ConfigElement.h>

namespace gadget
{
vprSingletonImp(KeyboardMouseDevice::KeyboardMouseDeviceRegistry);

KeyboardMouseDevice::KeyboardMouseDevice()
   : mMouseSensitivity(1.0f)
   , mUseButtonsForScrolling(false)
   , mScrollUpButton(gadget::KEY_NONE)
   , mScrollDownButton(gadget::KEY_NONE)
   , mScrollLeftButton(gadget::KEY_NONE)
   , mScrollRightButton(gadget::KEY_NONE)
{
   /* Do nothing. */ ;
}

KeyboardMouseDevice::~KeyboardMouseDevice()
{
   stopSampling();
}

std::string KeyboardMouseDevice::getElementType()
{
   return std::string("keyboard_mouse_device");
}

bool KeyboardMouseDevice::config(jccl::ConfigElementPtr e)
{
   unsigned required_definition_ver(2);

   if(e->getVersion() < required_definition_ver)
   {
      vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_CRITICAL_LVL)
         << clrOutBOLD(clrRED, "ERROR")
         << ": [gadget::KeyboardMouseDevice::config()] Element named '"
         << e->getName() << "'" << std::endl << vprDEBUG_FLUSH;
      vprDEBUG_NEXT(gadgetDBG_INPUT_MGR, vprDBG_CRITICAL_LVL)
         << "is version " << e->getVersion()
         << ", but we require at least version " << required_definition_ver
         << ". Ignoring.\n" << vprDEBUG_FLUSH;
      return false;
   }

   if ( ! (Input::config(e) && KeyboardMouse::config(e) ) )
   {
      return false;
   }

   mMouseSensitivity = e->getProperty<float>("mouse_sensitivity");
   if (0.0f == mMouseSensitivity)
   {
      mMouseSensitivity = 0.5;
   }

   mUseButtonsForScrolling = e->getProperty<bool>("scroll_as_buttons");

   if ( mUseButtonsForScrolling )
   {
      mScrollUpButton =
         static_cast<gadget::Keys>(
            e->getProperty<unsigned int>("scroll_up_button")
         );
      mScrollDownButton =
         static_cast<gadget::Keys>(
            e->getProperty<unsigned int>("scroll_down_button")
         );
      mScrollLeftButton =
         static_cast<gadget::Keys>(
            e->getProperty<unsigned int>("scroll_left_button")
         );
      mScrollRightButton =
         static_cast<gadget::Keys>(
            e->getProperty<unsigned int>("scroll_right_button")
         );
   }

   for ( int i = 0; i < gadget::LAST_KEY; ++i )
   {
      mRealkeys[i] = mKeys[i] = mCurKeys[i];
   }

   KeyboardMouseDeviceRegistry::KeyboardMouseDeviceInfo event_source_info;
   event_source_info.mSourceName = e->getName();
   event_source_info.mKeyboardMouseDevice = this;

   KeyboardMouseDeviceRegistry::instance()->addKeyboardMouseDevice(
      e->getName(), event_source_info
   );

   return true;
}

void KeyboardMouseDevice::updateData()
{
vpr::Guard<vpr::Mutex> guard(mKeysLock);      // Lock access to the mKeys array

// Scale mouse values based on sensitivity
   mKeys[gadget::MOUSE_POSX] = int(float(mKeys[gadget::MOUSE_POSX]) * mMouseSensitivity);
   mKeys[gadget::MOUSE_NEGX] = int(float(mKeys[gadget::MOUSE_NEGX]) * mMouseSensitivity);
   mKeys[gadget::MOUSE_POSY] = int(float(mKeys[gadget::MOUSE_POSY]) * mMouseSensitivity);
   mKeys[gadget::MOUSE_NEGY] = int(float(mKeys[gadget::MOUSE_NEGY]) * mMouseSensitivity);

   /*
   vprDEBUG(vprDBG_ALL, vprDBG_CRITICAL_LVL)
      << "gadget::KeyboardMouseDevice::updateData:" << mInstName << " -- "
      << "mouse_keys: px:" << mKeys[gadget::MOUSE_POSX]
      << " nx:" << mKeys[gadget::MOUSE_NEGX]
      << " py:" << mKeys[gadget::MOUSE_POSY]
      << " ny:" << mKeys[gadget::MOUSE_NEGY]
      << std::endl << vprDEBUG_FLUSH;
   */

   // Copy over values
   for ( unsigned int i = 0; i < gadget::LAST_KEY; ++i )
   {
      mCurKeys[i] = mKeys[i];
   }

   // Re-initialize the mKeys based on current key state in realKeys
   // Set the initial state of the mKey key counts based on the current state
   // of the system this is to ensure that if a key is still down, we get at
   // least one event for it.
   for ( unsigned int j = 0; j < gadget::LAST_KEY; ++j )
   {
      mKeys[j] = mRealkeys[j];
   }

   updateEventQueue();
}
/** Add the given window to the registry. */
bool KeyboardMouseDevice::KeyboardMouseDeviceRegistry::addKeyboardMouseDevice(const std::string& name,
                        KeyboardMouseDevice::KeyboardMouseDeviceRegistry::KeyboardMouseDeviceInfo winInfo)
{
   event_source_map_t::iterator found_window = mKeyboardMouseDeviceMap.find(name);
   if(found_window != mKeyboardMouseDeviceMap.end())
   {
      vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_WARNING_LVL)
         << "Warning: Attempted to add event source twice. named '"
         << name << "'. Ignoring second.\n" << vprDEBUG_FLUSH;
      return false;
   }
   else
   {
      vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_STATE_LVL)
         << "Adding event source: '" << name << "'" << std::endl << vprDEBUG_FLUSH;
      mKeyboardMouseDeviceMap[name] = winInfo;
      return true;
   }
}

/** Remove the window with the id of "name". */
void KeyboardMouseDevice::KeyboardMouseDeviceRegistry::removeKeyboardMouseDevice(const std::string& name)
{
   unsigned num_erased = mKeyboardMouseDeviceMap.erase(name);
   if(0 == num_erased)
   {
      vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_WARNING_LVL)
         << "Warning: Attempted to remove event source not found. named '"
         << name << "'. \n" << vprDEBUG_FLUSH;
   }
}

bool KeyboardMouseDevice::KeyboardMouseDeviceRegistry::getKeyboardMouseDevice(const std::string& name,
                                                KeyboardMouseDeviceInfo& winInfo)
{
   event_source_map_t::iterator found_window = mKeyboardMouseDeviceMap.find(name);
   if(found_window != mKeyboardMouseDeviceMap.end())
   {
      winInfo = (*found_window).second;
      return true;
   }
   else
   {
      return false;
   }
}

}
