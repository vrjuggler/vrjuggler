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

#include <gadget/Type/EventWindow/KeyEvent.h>


namespace gadget
{

char KeyEvent::getKeyChar() const
{
   switch (mKey)
   {
      case gadget::KEY_0: return '0';
      case gadget::KEY_1: return '1';
      case gadget::KEY_2: return '2';
      case gadget::KEY_3: return '3';
      case gadget::KEY_4: return '4';
      case gadget::KEY_5: return '5';
      case gadget::KEY_6: return '6';
      case gadget::KEY_7: return '7';
      case gadget::KEY_8: return '8';
      case gadget::KEY_9: return '9';
      case gadget::KEY_A: return 'A';
      case gadget::KEY_B: return 'B';
      case gadget::KEY_C: return 'C';
      case gadget::KEY_D: return 'D';
      case gadget::KEY_E: return 'E';
      case gadget::KEY_F: return 'F';
      case gadget::KEY_G: return 'G';
      case gadget::KEY_H: return 'H';
      case gadget::KEY_I: return 'I';
      case gadget::KEY_J: return 'J';
      case gadget::KEY_K: return 'K';
      case gadget::KEY_L: return 'L';
      case gadget::KEY_M: return 'M';
      case gadget::KEY_N: return 'N';
      case gadget::KEY_O: return 'O';
      case gadget::KEY_P: return 'P';
      case gadget::KEY_Q: return 'Q';
      case gadget::KEY_R: return 'R';
      case gadget::KEY_S: return 'S';
      case gadget::KEY_T: return 'T';
      case gadget::KEY_U: return 'U';
      case gadget::KEY_V: return 'V';
      case gadget::KEY_W: return 'W';
      case gadget::KEY_X: return 'X';
      case gadget::KEY_Y: return 'Y';
      case gadget::KEY_Z: return 'Z';
      default:            return 0;
   }
}

}
