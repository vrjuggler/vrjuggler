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

#include <gadget/gadgetConfig.h>

#include <vpr/vpr.h>
#include <vpr/Util/Assert.h>

#include <gadget/Type/EventWindow.h>


namespace gadget
{

vpr::ReturnStatus EventWindow::writeObject(vpr::ObjectWriter* writer)
{
   writer->writeUint16(MSG_DATA_KEYBOARD); // Write out the data type so that we can assert if reading in wrong place
   writer->writeUint16(256);

   for ( unsigned i = 0; i < 256; ++i )
   {
      writer->writeUint16(mCurKeys[i]);
   }

   return vpr::ReturnStatus::Succeed;
}

vpr::ReturnStatus EventWindow::readObject(vpr::ObjectReader* reader)
{
   //vprASSERT(reader->attribExists("rim.timestamp.delta"));
   //vpr::Uint64 delta = reader->getAttrib<vpr::Uint64>("rim.timestamp.delta");

   // ASSERT if this data is really not Digital Data
   vpr::Uint16 temp = reader->readUint16();
   vprASSERT(temp==MSG_DATA_KEYBOARD && "[Remote Input Manager]Not Digital Data");
   unsigned numVectors = reader->readUint16();

   for (unsigned i = 0; i < numVectors; ++i )
   {
      mCurKeys[i] = reader->readUint16();
   }

   return vpr::ReturnStatus::Succeed;
}

/**
 * Checks for the given modifier key pressed only.
 * @return true if key pressed exclusively.
 */
//virtual bool modifierOnly(int modKey) =0;
bool EventWindow::modifierOnly(int modKey)
{
   switch (modKey)
   {
      case gadget::KEY_NONE:
         return (!mCurKeys[gadget::KEY_SHIFT] && !mCurKeys[gadget::KEY_CTRL] && !mCurKeys[gadget::KEY_ALT]);
      case gadget::KEY_SHIFT:
         return (mCurKeys[gadget::KEY_SHIFT] && !mCurKeys[gadget::KEY_CTRL] && !mCurKeys[gadget::KEY_ALT]);
      case gadget::KEY_CTRL:
         return (!mCurKeys[gadget::KEY_SHIFT] && mCurKeys[gadget::KEY_CTRL] && !mCurKeys[gadget::KEY_ALT]);
      case gadget::KEY_ALT:
         return (!mCurKeys[gadget::KEY_SHIFT] && !mCurKeys[gadget::KEY_CTRL] && mCurKeys[gadget::KEY_ALT]);
      default:
         vprASSERT(false);
         return 0;
   }
}

std::string EventWindow::getKeyName(int keyId)
{
   switch(keyId)
   {
      case gadget::KEY_NONE: return std::string("VJKEY_NONE");
      case gadget::KEY_UP: return std::string("VJKEY_UP");
      case gadget::KEY_DOWN: return std::string("VJKEY_DOWN");
      case gadget::KEY_LEFT: return std::string("VJKEY_LEFT");
      case gadget::KEY_RIGHT: return std::string("VJKEY_RIGHT");
      case gadget::KEY_SHIFT: return std::string("VJKEY_SHIFT");
      case gadget::KEY_CTRL: return std::string("VJKEY_CTRL");
      case gadget::KEY_ALT: return std::string("VJKEY_ALT");
      case gadget::KEY_1: return std::string("VJKEY_1");
      case gadget::KEY_2: return std::string("VJKEY_2");
      case gadget::KEY_3: return std::string("VJKEY_3");
      case gadget::KEY_4: return std::string("VJKEY_4");
      case gadget::KEY_5: return std::string("VJKEY_5");
      case gadget::KEY_6: return std::string("VJKEY_6");
      case gadget::KEY_7: return std::string("VJKEY_7");
      case gadget::KEY_8: return std::string("VJKEY_8");
      case gadget::KEY_9: return std::string("VJKEY_9");
      case gadget::KEY_0: return std::string("VJKEY_0");
      case gadget::KEY_A: return std::string("VJKEY_A");
      case gadget::KEY_B: return std::string("VJKEY_B");
      case gadget::KEY_C: return std::string("VJKEY_C");
      case gadget::KEY_D: return std::string("VJKEY_D");
      case gadget::KEY_E: return std::string("VJKEY_E");
      case gadget::KEY_F: return std::string("VJKEY_F");
      case gadget::KEY_G: return std::string("VJKEY_G");
      case gadget::KEY_H: return std::string("VJKEY_H");
      case gadget::KEY_I: return std::string("VJKEY_I");
      case gadget::KEY_J: return std::string("VJKEY_J");
      case gadget::KEY_K: return std::string("VJKEY_K");
      case gadget::KEY_L: return std::string("VJKEY_L");
      case gadget::KEY_M: return std::string("VJKEY_M");
      case gadget::KEY_N: return std::string("VJKEY_N");
      case gadget::KEY_O: return std::string("VJKEY_O");
      case gadget::KEY_P: return std::string("VJKEY_P");
      case gadget::KEY_Q: return std::string("VJKEY_Q");
      case gadget::KEY_R: return std::string("VJKEY_R");
      case gadget::KEY_S: return std::string("VJKEY_S");
      case gadget::KEY_T: return std::string("VJKEY_T");
      case gadget::KEY_U: return std::string("VJKEY_U");
      case gadget::KEY_V: return std::string("VJKEY_V");
      case gadget::KEY_W: return std::string("VJKEY_W");
      case gadget::KEY_X: return std::string("VJKEY_X");
      case gadget::KEY_Y: return std::string("VJKEY_Y");
      case gadget::KEY_Z: return std::string("VJKEY_Z");
      case gadget::KEY_ESC: return std::string("VJKEY_ESC");
      case gadget::MOUSE_POSX: return std::string("VJMOUSE_POSX");
      case gadget::MOUSE_NEGX: return std::string("VJMOUSE_NEGX");
      case gadget::MOUSE_POSY: return std::string("VJMOUSE_POSY");
      case gadget::MOUSE_NEGY: return std::string("VJMOUSE_NEGY");
      case gadget::MBUTTON1: return std::string("VJMBUTTON1");
      case gadget::MBUTTON2: return std::string("VJMBUTTON2");
      case gadget::MBUTTON3: return std::string("VJMBUTTON3");
   }

   // If all of the above fell through ...
   return std::string("Unknown key");
}

} // End of gadget namespcae
