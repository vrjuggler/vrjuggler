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

#ifndef _GADGET_KEYS_H_
#define _GADGET_KEYS_H_

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

/// Mask values for keyboard modifier keys.
enum ModiferMask
{
   SHIFT_MASK = (1 << 0),  /**< Shift key mask */
   ALT_MASK   = (1 << 1),  /**< Control (CTRL) key mask */
   CTRL_MASK  = (1 << 2)   /**< Alt key mask */
};

/// Mask values for up to five mouse buttons.
enum ButtonMask
{
   BUTTON1_MASK = (1 << 3),  /**< Value for mouse button 1 */
   BUTTON2_MASK = (1 << 4),  /**< Value for mouse button 2 */
   BUTTON3_MASK = (1 << 5),  /**< Value for mouse button 3 */
   BUTTON4_MASK = (1 << 6),  /**< Value for mouse button 4 */
   BUTTON5_MASK = (1 << 7)   /**< Value for mouse button 5 */
};

} // End of gadget namespace

#endif /* _GADGET_KEYS_H_ */
