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
#ifndef _VJ_OSX_KEYBOARD_H_
#define _VJ_OSX_KEYBOARD_H_

#include <vjConfig.h>

#include <Input/vjInput/vjInput.h>
#include <Input/vjInput/vjKeyboard.h>
#include <vpr/Sync/Mutex.h>
#include <Carbon/Carbon.h>
#include <Kernel/vjOSXSystemFactory.h>

class vjConfigChunk;

//---------------------------------------------------------------
//: XWin Keyboard class
// Converts keyboard input into simulated input devices
//
// This device is a source of keyboard events.  The device should not be
// used directly, it should be referenced through proxies.
//
// Mouse Locking:
//    This device recieves input from the XWindows display.  As such,
//  the xwindow must have focus to generate events.  In order to help
//  users keep the window in focus, there are two cases where the
//  driver will "lock" the mouse to the window, thus preventing loss of focus.
//  CASE 1: The user holds down any key. (ie. a,b, ctrl, shift, etc)
//  CASE 2: The user can toggle locking using a special "locking" key
//           defined in the configuration chunk.
//
// See also: vjKeyboard, vjKeyboardProxy
//--------------------------------------------------------------
class vjOSXKeyboard : public vjInput, public vjKeyboard
{
public:
   // Enum to keep track of current lock state for state machine
   // Unlocked - The mouse is free
   // Lock_LockKey - The mouse is locked due to lock toggle key press
   // Lock_LockKeyDown - The mouse is locked due to a key being held down
   enum lockState { Unlocked, Lock_LockKey, Lock_KeyDown};
   vjOSXKeyboard()
   {
      mPrevX = 0; mPrevY = 0;
      mLockState = Unlocked;     // Initialize to unlocked.
      mExitFlag = false;
      mWeOwnTheWindow = true;
      vjOSXSystemFactory::instance();
   }
   ~vjOSXKeyboard() { stopSampling();}

   virtual bool config(vjConfigChunk* c);

   // Main thread of control for this active object
   void controlLoop(void* nullParam);

   /* Pure Virtuals required by vjInput */
   int startSampling();
   int stopSampling();

   // process the current x-events
   // Called repetatively by the controlLoop
   int sample() { HandleEvents(); return 1; }
   void updateData();

   static std::string getChunkType() { return std::string("Keyboard");}

   // returns the number of times the key was pressed during the
   // last frame, so you can put this in an if to check if was
   // pressed at all, or if you are doing processing based on this
   // catch the actual number..
   int isKeyPressed(int vjKey)
   {  return m_curKeys[vjKey];}

   virtual int keyPressed(int keyId)
   { return isKeyPressed(keyId); }

   virtual bool modifierOnly(int modKey)
   { return onlyModifier(modKey); }

protected:


private:
   /* Private functions for processing input data */
   int onlyModifier(int);

   //: Handle any events in the system
   // Copies m_keys to m_curKeys
   void HandleEvents();

   // Open the Carbon window to sample from
   int openTheWindow();

   //: Perform anything that must be done when state switches
   void lockMouse();
   void unlockMouse();
   
   int OSXKeyTovjKey(int xKey);
   
protected:  
   bool         mWeOwnTheWindow;       // True if this class owns the window (is reposible for opening and closing)
                                       // NOTE: In a case where it does not, then the window vars must be set prior
                                       //    to starting the controlLoop (startSampling)
   int          m_x, m_y;    // x_origin, y_origin
   unsigned int m_width,m_height;
   
   Rect 			rectWin;
   WindowPtr 			gpWindow;
   CFStringRef			window_title;

   /* Keyboard state holders */
   // NOTE: This driver does not use the normal triple buffering mechanism.
   // Instead, it just uses a modified double buffering system.
   int      m_keys[256];         // (0,*): The num key presses during an UpdateData (ie. How many keypress events)
   int      m_curKeys[256];      // (0,*): Copy of m_keys that the user reads from between updates
   int      m_realkeys[256];     // (0,1): The real keyboard state, all events processed (ie. what is the key now)
   vpr::Mutex  mKeysLock;           // Must hold this lock when accessing m_keys OR mHandleEventsHasBeenCalled
   bool     mHandleEventsHasBeenCalled;  // This flag keeps track of wether or not HandleEvents has been called since the last updateData.
                                    // It is used by updateData to make sure we don't get a "blank" update where no keys are pressed.
   bool     mExitFlag;           // Should we exit

   lockState   mLockState;       // The current state of locking
   int         mLockStoredKey;   // The key that was pressed down
   int         mLockToggleKey;   // The key that toggles the locking

   float m_mouse_sensitivity;
   int   mSleepTimeMS;            // Amount of time to sleep in milliseconds between updates
   int   mPrevX, mPrevY;          // Previous mouse location
   int	 mSnapTimer;
};

enum vjOSXKeyMap {
    vjOSX_KEY_X = 0,
    vjOSX_KEY_Z = 1,
    vjOSX_KEY_G = 2,
    vjOSX_KEY_H = 3,
    vjOSX_KEY_F = 4,
    vjOSX_KEY_D = 5,
    vjOSX_KEY_S = 6,
    vjOSX_KEY_A = 7,
    vjOSX_KEY_R = 8,
    vjOSX_KEY_E = 9,
    vjOSX_KEY_W = 10,
    vjOSX_KEY_Q = 11,
    vjOSX_KEY_B = 12,
    vjOSX_KEY_V = 14,
    vjOSX_KEY_C = 15,
    vjOSX_KEY_5 = 16,
    vjOSX_KEY_6 = 17,
    vjOSX_KEY_4 = 18,
    vjOSX_KEY_3 = 19,
    vjOSX_KEY_2 = 20,
    vjOSX_KEY_1 = 21,
    vjOSX_KEY_T = 22,
    vjOSX_KEY_Y = 23,
    vjOSX_KEY_O = 24,
    vjOSX_KEY_BRACE_LEFT = 25,
    vjOSX_KEY_0 = 26,
    vjOSX_KEY_8 = 27,
    vjOSX_KEY_MINUS = 28,
    vjOSX_KEY_7 = 29,
    vjOSX_KEY_9 = 30,
    vjOSX_KEY_PLUS = 31,
    vjOSX_KEY_QUOTE = 32,
    vjOSX_KEY_J = 33,
    vjOSX_KEY_L = 34,
    vjOSX_KEY_RETURN = 35,
    vjOSX_KEY_P = 36,
    vjOSX_KEY_I = 37,
    vjOSX_KEY_BRACE_RIGHT = 38,
    vjOSX_KEY_U = 39,
    vjOSX_KEY_GREATER_THAN = 40,
    vjOSX_KEY_M = 41,
    vjOSX_KEY_N = 42,
    vjOSX_KEY_QUESTION_MARK = 43,
    vjOSX_KEY_LESS_THAN = 44,
    vjOSX_KEY_FORWARD_SLASH = 45,
    vjOSX_KEY_SEMI_COLLEN = 46,
    vjOSX_KEY_K = 47,
    vjOSX_KEY_COMMAND = 48,
    vjOSX_KEY_ESCAPE = 50,
    vjOSX_KEY_BACKSPACE = 52,
    vjOSX_KEY_TILDE = 53,
    vjOSX_KEY_SPACE = 54,
    vjOSX_KEY_TAB = 55,
    vjOSX_KEY_CONTROL = 60,
    vjOSX_KEY_OPTION = 61,
    vjOSX_KEY_CAPS = 62,
    vjOSX_KEY_SHIFT = 63,
    vjOSX_KEYPAD_CLEAR = 64,
    vjOSX_KEYPAD_PLUS = 66,
    vjOSX_KEYPAD_MULT = 68,
    vjOSX_KEYPAD_DECIMAL = 70,
    vjOSX_KEYPAD_MINUS = 73,
    vjOSX_KEYPAD_ENTER = 75,
    vjOSX_KEYPAD_DIVIDE = 76,
    vjOSX_KEYPAD_5 = 80,
    vjOSX_KEYPAD_4 = 81,
    vjOSX_KEYPAD_3 = 82,
    vjOSX_KEYPAD_2 = 83,
    vjOSX_KEYPAD_1 = 84,
    vjOSX_KEYPAD_0 = 85,
    vjOSX_KEYPAD_EQUAL = 86,
    vjOSX_KEYPAD_9 = 91,
    vjOSX_KEYPAD_8 = 92,
    vjOSX_KEYPAD_7 = 94,
    vjOSX_KEYPAD_6 = 95,
    vjOSX_KEY_F9 = 98,
    vjOSX_KEY_F8 = 99,
    vjOSX_KEY_F3 = 100,
    vjOSX_KEY_F7 = 101,
    vjOSX_KEY_F6 = 102,
    vjOSX_KEY_F5 = 103,
    vjOSX_KEY_F12 = 104,
    vjOSX_KEY_F10 = 106,
    vjOSX_KEY_F14 = 108,
    vjOSX_KEY_F13 = 110,
    vjOSX_KEY_END = 112,
    vjOSX_KEY_F4 = 113,
    vjOSX_KEY_DEL = 114,
    vjOSX_KEY_PAGEUP = 115,
    vjOSX_KEY_HOME = 116,
    vjOSX_KEY_HELP = 117,
    vjOSX_KEY_F15 = 118,
    vjOSX_KEY_UP = 121,
    vjOSX_KEY_DOWN = 122,
    vjOSX_KEY_RIGHT = 123,
    vjOSX_KEY_LEFT = 124,
    vjOSX_KEY_F1 = 125,
    vjOSX_KEY_PAGEDOWN = 126,
    vjOSX_KEY_F2 = 127
   };

#endif
