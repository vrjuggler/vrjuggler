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

#ifndef _VRJ_KEYBOARD_OSX_H_
#define _VRJ_KEYBOARD_OSX_H_

#include <vrj/vjConfig.h>

#include <vrj/Input/Type/Input.h>
#include <vrj/Input/Type/Keyboard.h>
#include <vpr/Sync/Mutex.h>
#include <Carbon/Carbon.h>
#include <vrj/Kernel/OSXSystemFactory.h>

namespace vrj
{
   
   class ConfigChunk;

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
// See also: Keyboard, KeyboardProxy
//--------------------------------------------------------------
class KeyboardOSX : public Input, public Keyboard
{
public:
   // Enum to keep track of current lock state for state machine
   // Unlocked - The mouse is free
   // Lock_LockKey - The mouse is locked due to lock toggle key press
   // Lock_LockKeyDown - The mouse is locked due to a key being held down
   enum lockState { Unlocked, Lock_LockKey, Lock_KeyDown};
   KeyboardOSX()
   {
      mPrevX = 0; mPrevY = 0;
      mLockState = Unlocked;     // Initialize to unlocked.
      mExitFlag = false;
      mWeOwnTheWindow = true;
      OSXSystemFactory::instance();
   }
   ~KeyboardOSX() { stopSampling();}

   virtual bool config(ConfigChunk* c);

   // Main thread of control for this active object
   void controlLoop(void* nullParam);

   /* Pure Virtuals required by Input */
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
   int isKeyPressed(int Key)
   {  return m_curKeys[Key];}

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
   
   int OSXKeyToKey(int xKey);
   
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

enum OSXKeyMap {
    OSX_KEY_X = 0,
    OSX_KEY_Z = 1,
    OSX_KEY_G = 2,
    OSX_KEY_H = 3,
    OSX_KEY_F = 4,
    OSX_KEY_D = 5,
    OSX_KEY_S = 6,
    OSX_KEY_A = 7,
    OSX_KEY_R = 8,
    OSX_KEY_E = 9,
    OSX_KEY_W = 10,
    OSX_KEY_Q = 11,
    OSX_KEY_B = 12,
    OSX_KEY_V = 14,
    OSX_KEY_C = 15,
    OSX_KEY_5 = 16,
    OSX_KEY_6 = 17,
    OSX_KEY_4 = 18,
    OSX_KEY_3 = 19,
    OSX_KEY_2 = 20,
    OSX_KEY_1 = 21,
    OSX_KEY_T = 22,
    OSX_KEY_Y = 23,
    OSX_KEY_O = 24,
    OSX_KEY_BRACE_LEFT = 25,
    OSX_KEY_0 = 26,
    OSX_KEY_8 = 27,
    OSX_KEY_MINUS = 28,
    OSX_KEY_7 = 29,
    OSX_KEY_9 = 30,
    OSX_KEY_PLUS = 31,
    OSX_KEY_QUOTE = 32,
    OSX_KEY_J = 33,
    OSX_KEY_L = 34,
    OSX_KEY_RETURN = 35,
    OSX_KEY_P = 36,
    OSX_KEY_I = 37,
    OSX_KEY_BRACE_RIGHT = 38,
    OSX_KEY_U = 39,
    OSX_KEY_GREATER_THAN = 40,
    OSX_KEY_M = 41,
    OSX_KEY_N = 42,
    OSX_KEY_QUESTION_MARK = 43,
    OSX_KEY_LESS_THAN = 44,
    OSX_KEY_FORWARD_SLASH = 45,
    OSX_KEY_SEMI_COLLEN = 46,
    OSX_KEY_K = 47,
    OSX_KEY_COMMAND = 48,
    OSX_KEY_ESCAPE = 50,
    OSX_KEY_BACKSPACE = 52,
    OSX_KEY_TILDE = 53,
    OSX_KEY_SPACE = 54,
    OSX_KEY_TAB = 55,
    OSX_KEY_CONTROL = 60,
    OSX_KEY_OPTION = 61,
    OSX_KEY_CAPS = 62,
    OSX_KEY_SHIFT = 63,
    OSX_KEYPAD_CLEAR = 64,
    OSX_KEYPAD_PLUS = 66,
    OSX_KEYPAD_MULT = 68,
    OSX_KEYPAD_DECIMAL = 70,
    OSX_KEYPAD_MINUS = 73,
    OSX_KEYPAD_ENTER = 75,
    OSX_KEYPAD_DIVIDE = 76,
    OSX_KEYPAD_5 = 80,
    OSX_KEYPAD_4 = 81,
    OSX_KEYPAD_3 = 82,
    OSX_KEYPAD_2 = 83,
    OSX_KEYPAD_1 = 84,
    OSX_KEYPAD_0 = 85,
    OSX_KEYPAD_EQUAL = 86,
    OSX_KEYPAD_9 = 91,
    OSX_KEYPAD_8 = 92,
    OSX_KEYPAD_7 = 94,
    OSX_KEYPAD_6 = 95,
    OSX_KEY_F9 = 98,
    OSX_KEY_F8 = 99,
    OSX_KEY_F3 = 100,
    OSX_KEY_F7 = 101,
    OSX_KEY_F6 = 102,
    OSX_KEY_F5 = 103,
    OSX_KEY_F12 = 104,
    OSX_KEY_F10 = 106,
    OSX_KEY_F14 = 108,
    OSX_KEY_F13 = 110,
    OSX_KEY_END = 112,
    OSX_KEY_F4 = 113,
    OSX_KEY_DEL = 114,
    OSX_KEY_PAGEUP = 115,
    OSX_KEY_HOME = 116,
    OSX_KEY_HELP = 117,
    OSX_KEY_F15 = 118,
    OSX_KEY_UP = 121,
    OSX_KEY_DOWN = 122,
    OSX_KEY_RIGHT = 123,
    OSX_KEY_LEFT = 124,
    OSX_KEY_F1 = 125,
    OSX_KEY_PAGEDOWN = 126,
    OSX_KEY_F2 = 127
   };

};

#endif
