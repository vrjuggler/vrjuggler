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


/////////////////////////////////////////////////////////////////////////////
// vjDummyKeyboard.h
//
// Dummy Keyboard device
//
// History:
//
// Allen Bierbaum:    v0.0 - 2-1-99 - Inital version
////////////////////////////////////////////////////////////////////////////


#ifndef _VJ_DUMMYKEYBOARD_H_
#define _VJ_DUMMYKEYBOARD_H_

#include <vjConfig.h>
#include <Input/vjInput/vjKeyboard.h>

//: A dummy device for the Keyboard proxies to connect to
//!PUBLIC_API:
class vjDummyKeyboard : public vjKeyboard
{
public:
   vjDummyKeyboard() : vjKeyboard()
   { active = 1;}

   //: Is the given key pressed.
   //! RETURNS: The number of times the key was pressed since last update.
   int keyPressed(int keyId)
   { return 0; }

   //:Check for the given modifier key pressed only.
   //! RETURNS: true - key pressed exclusively
   bool modifierOnly(int modKey)
   { return false; }

   char* getDeviceName()
   { return "vjDummyKeyboard"; }

   /* These functions don't do anything */
   int startSampling() { return 1; }
	int stopSampling() { return 0; }
	int sample() { return 1;}
	void updateData() { return ; }
};


#endif
