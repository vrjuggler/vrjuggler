/*
 *  File:          $RCSfile$
 *  Date modified: $Date$
 *  Version:       $Revision$
 *
 *
 *                                VR Juggler
 *                                    by
 *                              Allen Bierbaum
 *                             Christopher Just
 *                             Patrick Hartling
 *                            Carolina Cruz-Neira
 *                               Albert Baker
 *
 *                  Copyright (C) - 1997, 1998, 1999, 2000
 *              Iowa State University Research Foundation, Inc.
 *                            All Rights Reserved
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
