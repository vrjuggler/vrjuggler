/*
 * VRJuggler
 *   Copyright (C) 1997,1998,1999,2000
 *   Iowa State University Research Foundation, Inc.
 *   All Rights Reserved
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
 */


#ifndef _VJ_SIM_INPUT_H
#define _VJ_SIM_INPUT_H
//#pragma once


#include <vjConfig.h>
#include <Input/InputManager/vjKeyboardInterface.h>
#include <Config/vjVarValue.h>

class vjConfigChunk;

//: Base class for all simulated input devices
//
// Has the common routines for accessing and seting
// up keyboard interface
//
//!PUBLIC_API:
class vjSimInput
{
protected:
   class vjKeyModPair
   {
   public:
      vjKeyModPair() : mKey(-1), mModifier(-1)
      {;}

   public:
      int mKey;
      int mModifier;
   };

public:
   vjSimInput() {;}

   //: Configure the simulated input device
   //! POST: Keyboard proxy is configured <br>
   bool config(vjConfigChunk* chunk);

protected:
   //: Check the given keypair
   //! RETURNS: Number of times the key was pressed
   //           with the modifier alone held down
   int  checkKeyPair(vjKeyModPair& pair);

   //: Get number of time key pressed
   //! RETURNS: Number of time the key is pressed
   int   checkKey(int keyId);

   //: Constructs a vector of key mod pairs
   // Takes as input a chunk that has a list of KeyModPair embeded chunks
   //! PRE: keyList must be full of var values containing chunks of the type "KeyModPair"
   //+      The KeyModPair chunk type must have fields name key and modKey
   //! RETURNS: vector of KeyModPairs
   std::vector<vjKeyModPair> readKeyList(std::vector<vjVarValue*>& keyList);

private:
   vjKeyboardInterface     mKeyboard;        //: The keyboard we are getting events from
};

#endif
