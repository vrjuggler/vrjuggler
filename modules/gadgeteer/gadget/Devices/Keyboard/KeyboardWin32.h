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


/////////////////////////////////////////////////////////////////////////////
// KeyboardWin32.h
//
// Keyboard input device for win32
//
// History:
//
// Andy Himberger:    v0.0 - 12-1-97 - Inital version
// Allen Bierbaum:    v1.0 -  7-23-99 - Refactored to use new keyboard method
////////////////////////////////////////////////////////////////////////////

#ifndef _GADGET_KEYBOARD_WIN32_H_
#define _GADGET_KEYBOARD_WIN32_H_

#include <gadget/gadgetConfig.h>


// Get windows stuff
#include <windows.h>
#include <commctrl.h>

#include <gadget/Type/Input.h>
#include <gadget/Type/Keyboard.h>
#include <vpr/Thread/Thread.h>

namespace gadget
{

class KeyboardWin32 : public Input, public Keyboard
{
public:

   KeyboardWin32()
   {
      for (int i =0; i < 256; i++)
         m_realkeys[i] = m_keys[i] = m_framekeys[i] = 0;

      m_realkeys[0] = m_keys[0] = m_framekeys[0] = 1;
   }
   ~KeyboardWin32() { stopSampling();}


   virtual bool config(jccl::ConfigChunkPtr c);

   /* Pure Virtuals required by Input */
   int startSampling();
   int stopSampling();
   int sample() { return 1;}
   void updateData();

   static std::string getChunkType() { return std::string("Keyboard");}

   // returns the number of times the key was pressed during the
   // last frame, so you can put this in an if to check if was
   // pressed at all, or if you are doing processing based on this
   // catch the actual number..
   int isKeyPressed(int Key)
   {  return m_keys[Key];}

   virtual int keyPressed(int keyId)
   { return isKeyPressed(keyId); }

   virtual bool modifierOnly(int modKey)
   { return ((onlyModifier(modKey) == 1) ? true : false); }

   void createWindowWin32 ();
   void updKeys(  UINT message,  UINT wParam, LONG lParam);

public:
   HINSTANCE   m_hInst;
   HWND        m_hWnd;

private:
   /** @name Private functions for processing input data */
   /* Private functions for processing input data */
     //@{
   int onlyModifier(int);


   /** @name Windows utility functions */
   //@{
   int VKKeyToKey(int vkKey);
   char* checkArgs(char* look_for);

   BOOL MenuInit (HINSTANCE hInstance);
   //@}


   int      m_screen, m_x, m_y;
   unsigned int m_width,m_height;

   // Use m_keys to know how many times each key was KEYDOWN during the loop
   // m_realkeys is the current up/down state of each key
   // m_framekeys is a buffer of the keys that have been pressed
   // since the last updateData.  It creates a non-thread accurate buffer
   int m_keys[256];        //: Last frames state, set by the updateData
   int m_framekeys[256];   //: The keyboard state during a frame of update, without KeyUp events included
   int m_realkeys[256];    //: The real keyboard state, all events processed

   float m_mouse_sensitivity;

   int oldx,oldy,newx,newy;  // Mousemove variables
};

};

#endif
