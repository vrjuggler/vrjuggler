/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2003 by Iowa State University
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

#ifndef _GADGET_EVENT_WINDOW_H_
#define _GADGET_EVENT_WINDOW_H_

#include <gadget/gadgetConfig.h>

#include <vpr/IO/SerializableObject.h>
#include <jccl/Config/ConfigChunkPtr.h>


namespace gadget
{

/// Names of defined keys.
enum Keys
{
   KEY_NONE,
   KEY_UP,
   KEY_DOWN,
   KEY_LEFT,
   KEY_RIGHT,
   KEY_SHIFT,
   KEY_CTRL,
   KEY_ALT,
   KEY_1,
   KEY_2,
   KEY_3,
   KEY_4,
   KEY_5,
   KEY_6,
   KEY_7,
   KEY_8,
   KEY_9,
   KEY_0,
   KEY_A,
   KEY_B,
   KEY_C,
   KEY_D,
   KEY_E,
   KEY_F,
   KEY_G,
   KEY_H,
   KEY_I,
   KEY_J,
   KEY_K,
   KEY_L,
   KEY_M,
   KEY_N,
   KEY_O,
   KEY_P,
   KEY_Q,
   KEY_R,
   KEY_S,
   KEY_T,
   KEY_U,
   KEY_V,
   KEY_W,
   KEY_X,
   KEY_Y,
   KEY_Z,
   KEY_ESC,

   // XXX: Mouse information probably shouldn't be here in the long term.
   MOUSE_POSX,
   MOUSE_NEGX,
   MOUSE_POSY,
   MOUSE_NEGY,
   MBUTTON1,
   MBUTTON2,
   MBUTTON3
};


const unsigned short MSG_DATA_KEYBOARD = 420;

/**
 * gadget::EventWindow is an abstract class for interfacing with keyboard (and
 * other key-based) devices.
 *
 * Informally, an event window can be thought of as map of keys to number of
 * times prseed since last update.
 *
 * That is to say, that gadget::EventWindow counts the number of key presses
 * between updates.  Updates in Juggler occur once per frame.
 */
class EventWindow : public vpr::SerializableObject
{
public:
   EventWindow()
   {
      /* Do nothing. */ ;
   }

   virtual ~EventWindow()
   {
      /* Do nothing. */ ;
   }

   virtual std::string getBaseType()
   {
      return std::string("EventWindow");
   }

   virtual vpr::ReturnStatus writeObject(vpr::ObjectWriter* writer);

   virtual vpr::ReturnStatus readObject(vpr::ObjectReader* reader);

   virtual bool config(jccl::ConfigChunkPtr chunk)
   {
      return true;
   }

   /**
    * Is the given key pressed?
    * @return The number of times the key was pressed since last update.
    */
   int keyPressed(int keyId)
   {
      return mCurKeys[keyId];
   }

   /**
    * Checks for the given modifier key pressed only.
    * @return true if key pressed exclusively.
    */
   bool modifierOnly(int modKey);

   std::string getKeyName(int keyId);

   int mCurKeys[256]; /**< (0,*): Copy of m_keys that the user reads from between updates. */
};

} // end namespace


#endif
