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

#ifndef _GADGET_KEYBOARD_H_
#define _GADGET_KEYBOARD_H_

#include <gadget/gadgetConfig.h>


// --------- VJKEY's ------------ //
// Names of defined keys
const int VJKEY_NONE   = 0;
const int VJKEY_UP     = 1; //65362;
const int VJKEY_DOWN   = 2; // 65361;
const int VJKEY_LEFT   = 3; //65363;
const int VJKEY_RIGHT  = 4; //65364;
const int VJKEY_SHIFT  = 5;
const int VJKEY_CTRL   = 6;
const int VJKEY_ALT    = 7;
const int VJKEY_1      = 8;
const int VJKEY_2      = 9;
const int VJKEY_3      = 10;
const int VJKEY_4      = 11;
const int VJKEY_5      = 12;
const int VJKEY_6      = 13;
const int VJKEY_7      = 14;
const int VJKEY_8      = 15;
const int VJKEY_9      = 16;
const int VJKEY_0      = 17;
const int VJKEY_A      = 18;
const int VJKEY_B      = 19;
const int VJKEY_C      = 20;
const int VJKEY_D      = 21;
const int VJKEY_E      = 22;
const int VJKEY_F      = 23;
const int VJKEY_G      = 24;
const int VJKEY_H      = 25;
const int VJKEY_I      = 26;
const int VJKEY_J      = 27;
const int VJKEY_K      = 28;
const int VJKEY_L      = 29;
const int VJKEY_M      = 30;
const int VJKEY_N      = 31;
const int VJKEY_O      = 32;
const int VJKEY_P      = 33;
const int VJKEY_Q      = 34;
const int VJKEY_R      = 35;
const int VJKEY_S      = 36;
const int VJKEY_T      = 37;
const int VJKEY_U      = 38;
const int VJKEY_V      = 39;
const int VJKEY_W      = 40;
const int VJKEY_X      = 41;
const int VJKEY_Y      = 42;
const int VJKEY_Z      = 43;
const int VJKEY_ESC    = 44;

#define LAST_KEY 44  /* Don't forget to change this when new keys are added */

const int VJMOUSE_POSX = LAST_KEY + 1;
const int VJMOUSE_NEGX = LAST_KEY + 2;
const int VJMOUSE_POSY = LAST_KEY + 3;
const int VJMOUSE_NEGY = LAST_KEY + 4;
const int VJMBUTTON1   = LAST_KEY + 5;
const int VJMBUTTON2   = LAST_KEY + 6;
const int VJMBUTTON3   = LAST_KEY + 7;


namespace gadget
{

//----------------------------------------------------------------------------
//: Keyboard is an abstract class for interfacing with keyboard (and other
//+ keybased) devices.
//
// Informally, a keyboard can be thought of as map of keys to number of times
// prseed since last update.
//
// That is to say, that Keyboard counts the number of keypresses between
// updates.  Updates in Juggler occur once per frame.
//-----------------------------------------------------------------------------
//!PUBLIC_API:
class Keyboard
{
public:
   Keyboard()
   { ; }

   virtual ~Keyboard() {;}

   virtual bool config(jccl::ConfigChunkPtr chunk)
   { return true; }

      //: Is the given key pressed.
   //! RETURNS: The number of times the key was pressed since last update.
   virtual int keyPressed(int keyId) =0;

   //:Check for the given modifier key pressed only.
   //! RETURNS: true - key pressed exclusively
   virtual bool modifierOnly(int modKey) =0;

   std::string getKeyName(int keyId)
   {
      switch(keyId)
      {
         case VJKEY_NONE: return std::string("VJKEY_NONE");
         case VJKEY_UP: return std::string("VJKEY_UP");
         case VJKEY_DOWN: return std::string("VJKEY_DOWN");
         case VJKEY_LEFT: return std::string("VJKEY_LEFT");
         case VJKEY_RIGHT: return std::string("VJKEY_RIGHT");
         case VJKEY_SHIFT: return std::string("VJKEY_SHIFT");
         case VJKEY_CTRL: return std::string("VJKEY_CTRL");
         case VJKEY_ALT: return std::string("VJKEY_ALT");
         case VJKEY_1: return std::string("VJKEY_1");
         case VJKEY_2: return std::string("VJKEY_2");
         case VJKEY_3: return std::string("VJKEY_3");
         case VJKEY_4: return std::string("VJKEY_4");
         case VJKEY_5: return std::string("VJKEY_5");
         case VJKEY_6: return std::string("VJKEY_6");
         case VJKEY_7: return std::string("VJKEY_7");
         case VJKEY_8: return std::string("VJKEY_8");
         case VJKEY_9: return std::string("VJKEY_9");
         case VJKEY_0: return std::string("VJKEY_0");
         case VJKEY_A: return std::string("VJKEY_A");
         case VJKEY_B: return std::string("VJKEY_B");
         case VJKEY_C: return std::string("VJKEY_C");
         case VJKEY_D: return std::string("VJKEY_D");
         case VJKEY_E: return std::string("VJKEY_E");
         case VJKEY_F: return std::string("VJKEY_F");
         case VJKEY_G: return std::string("VJKEY_G");
         case VJKEY_H: return std::string("VJKEY_H");
         case VJKEY_I: return std::string("VJKEY_I");
         case VJKEY_J: return std::string("VJKEY_J");
         case VJKEY_K: return std::string("VJKEY_K");
         case VJKEY_L: return std::string("VJKEY_L");
         case VJKEY_M: return std::string("VJKEY_M");
         case VJKEY_N: return std::string("VJKEY_N");
         case VJKEY_O: return std::string("VJKEY_O");
         case VJKEY_P: return std::string("VJKEY_P");
         case VJKEY_Q: return std::string("VJKEY_Q");
         case VJKEY_R: return std::string("VJKEY_R");
         case VJKEY_S: return std::string("VJKEY_S");
         case VJKEY_T: return std::string("VJKEY_T");
         case VJKEY_U: return std::string("VJKEY_U");
         case VJKEY_V: return std::string("VJKEY_V");
         case VJKEY_W: return std::string("VJKEY_W");
         case VJKEY_X: return std::string("VJKEY_X");
         case VJKEY_Y: return std::string("VJKEY_Y");
         case VJKEY_Z: return std::string("VJKEY_Z");
         case VJKEY_ESC: return std::string("VJKEY_ESC");
         case VJMOUSE_POSX: return std::string("VJMOUSE_POSX");
         case VJMOUSE_NEGX: return std::string("VJMOUSE_NEGX");
         case VJMOUSE_POSY: return std::string("VJMOUSE_POSY");
         case VJMOUSE_NEGY: return std::string("VJMOUSE_NEGY");
         case VJMBUTTON1: return std::string("VJMBUTTON1");
         case VJMBUTTON2: return std::string("VJMBUTTON2");
         case VJMBUTTON3: return std::string("VJMBUTTON3");
         default: return std::string("Unknown key");
      }

      //return std::string("n/a");
   }
};



} // end namespace

#endif
