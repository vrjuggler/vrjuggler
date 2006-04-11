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

#ifndef _GADGET_KEYBOARD_OSX_H_
#define _GADGET_KEYBOARD_OSX_H_

#include <Carbon/Carbon.h>

#include <gadget/Type/Input.h>
#include <gadget/Type/Keyboard.h>
#include <gadget/Type/InputMixer.h>
#include <jccl/Config/ConfigChunkPtr.h>
#include <gadget/gadgetConfig.h>

namespace gadget
{

/**
 * Call back function to register with the carbon event loop.  The userData
 * argument contains a reference to the instance of the KeyboardOSX class that
 * registered to receive the event for its window.
 */
pascal OSStatus keyboardHandlerOSX ( EventHandlerCallRef  nextHandler,
                                     EventRef             theEvent,
                                     void*                userData);

/**
 * OSX Keyboard class.
 * Converts keyboard input into simulated input devices.
 *
 * This device is a source of keyboard events.  The device should not be
 * used directly, it should be referenced through proxies.
 *
 * Change to reflect the removal of the mouse lock on every button press.
 *
 * Mouse Locking:<br>
 *    This device receives input from the carbon event loop.  As such,
 *  the window must have focus to generate events.  In order to help
 *  users keep the window in focus, there are two cases where the
 *  driver will "lock" the mouse to the window, thus preventing loss of focus.<br>
 *  CASE 1: The user holds down any key. (ie. a,b, ctrl, shift, etc)<br>
 *  CASE 2: The user can toggle locking using a special "locking" key
 *           defined in the configuration chunk.
 *
 * @see Keyboard, KeyboardProxy
 */
class KeyboardOSX : public InputMixer<Input,Keyboard>
{
public:
   /** Enum to keep track of current lock state for state machine. */
   enum lockState
   {
      Unlocked,     /**< The mouse is free */
      Lock_LockKey, /**< The mouse is locked due to lock toggle key press */
      Lock_KeyDown  /**< The mouse is locked due to a key being held down */
   };

   KeyboardOSX()
   {
        mLockState = Unlocked;     // Initialize to unlocked.
        //Sampling has not been called yet
        mAmSampling = false;
        //mExitFlag = false;
        mHandleEventsHasBeenCalled = false;      // Initialize it to not being called yet
        //mWeOwnTheWindow = true;
        //mShared = false;                         // False by default
   }

   ~KeyboardOSX() { stopSampling();}

   virtual bool config(jccl::ConfigChunkPtr c);

   /** @name Pure Virtuals required by Input */
   //@{
   int startSampling();
   int stopSampling();
   //@}

   /**
    * Do nothing the events are handled by call backs from the
    * Carbon event manager.
    */
   int sample() { return 1; }
   void updateData();

   static std::string getChunkType() { return std::string("Keyboard");}

   /**
    * Returns the number of times the key was pressed during the
    * last frame, so you can put this in an if to check if was
    * pressed at all, or if you are doing processing based on this
    * catch the actual number..
    */
   int isKeyPressed(int Key)
   {  return m_curKeys[Key];}


   /**
    * Called by callback function.
    * Processes the event.
    */
   pascal OSStatus gotKeyEvent (  EventHandlerCallRef  nextHandler,
                                  EventRef             theEvent,
                                  void*                userData);

   /**
    * Invokes the global scope delete operator.  This is required for proper
    * releasing of memory in DLLs on Win32.
    */
   void operator delete(void* p)
   {
      ::operator delete(p);
   }

protected:
   /**
    * Deletes this object.  This is an implementation of the pure virtual
    * gadget::Input::destroy() method.
    */
   void destroy()
   {
      delete this;
   }

protected:
    int attachEvents(WindowRef aWindow);

private:
   /** @name Private functions for processing input data */
   //@{
   int onlyModifier(int);
   //@}

   /** @name OSX utility functions */
   //@{
   /**
    * Converts OSXKey to VjKey.
    * @note Keypad keys are transformed ONLY to number keys.
    */
   int osxKeyToKey( UInt32 osxKey );

   /** Opens the carbon window to get events from. */
   int openTheWindow();
   

   /** Performs anything that must be done when state switches. */
   void lockMouse();
   void unlockMouse();
   //@}



protected:
  // bool         mWeOwnTheWindow;       // True if this class owns the window (is reposible for opening and closing)
                                       // NOTE: In a case where it does not, then the window vars must be set prior
                                       //    to starting the controlLoop (startSampling)
   bool           mShared;   /**< True if the window is shared between multiple processes */
//   vpr::Mutex     mXfuncLock;      /**< Lock for exclusive access to x functions.  Must be held if mshared==true */

//   ::Window       m_window;
//   ::XVisualInfo* m_visual;
//   ::Display*     m_display;
//   ::XSetWindowAttributes m_swa;
   int          m_screen, m_x, m_y;    /**< screen id, x_origin, y_origin */
   unsigned int m_width,m_height;

   /** @name Keyboard state holders */
   //@{
   // NOTE: This driver does not use the normal triple buffering mechanism.
   // Instead, it just uses a modified double buffering system.
   int      m_keys[256];         /**< (0,*): The num key presses during an UpdateData (ie. How many keypress events). */
   int      m_realkeys[256];     /**< (0,1): The real keyboard state, all events processed (ie. what is the key now). */
   vpr::Mutex  mKeysLock;           /**< Must hold this lock when accessing m_keys OR mHandleEventsHasBeenCalled. */
   bool     mHandleEventsHasBeenCalled;  /**< This flag keeps track of wether or not HandleEvents has been called since the last updateData.
                                              It is used by updateData to make sure we don't get a "blank" update where no keys are pressed. */
   //bool     mExitFlag;           /**< Should we exit? */

   lockState   mLockState;       /**< The current state of locking. */
   int         mLockStoredKey;   /**< The key that was pressed down. */
   int         mLockToggleKey;   /**< The key that toggles the locking. */
   //@}

   //std::string mXDisplayString;   /**< The display string to use from systemDisplay config info. */

   float m_mouse_sensitivity;
   //int   mSleepTimeMS;            /**< Amount of time to sleep in milliseconds between updates. */
   //int   mPrevX, mPrevY;          /**< Previous mouse location. */
   bool  mAmSampling;
   WindowRef mWindow;           /**< The carbon window. */
};


// Note we are using the key code.
// This might break on international keyboards

enum OSXKeyMap
{
    OSXK_a = 0,
    OSXK_b = 11,
    OSXK_c = 8,
    OSXK_d = 2,
    OSXK_e = 14,
    OSXK_f = 3,
    OSXK_g = 5,
    OSXK_h = 4,
    OSXK_i = 34,
    OSXK_j = 38,
    OSXK_k = 40,
    OSXK_l = 37,
    OSXK_m = 46,
    OSXK_n = 45,
    OSXK_o = 31,
    OSXK_p = 35,
    OSXK_q = 12,
    OSXK_r = 15,
    OSXK_s = 1,
    OSXK_t = 17,
    OSXK_u = 32,
    OSXK_v = 9,
    OSXK_w = 13,
    OSXK_x = 7,
    OSXK_y = 16,
    OSXK_z = 6,

    OSXK_1 = 18,
    OSXK_2 = 19,
    OSXK_3 = 20,
    OSXK_4 = 21,
    OSXK_5 = 23,
    OSXK_6 = 22,
    OSXK_7 = 26,
    OSXK_8 = 28,
    OSXK_9 = 25,
    OSXK_0 = 29,

    OSXK_KP_1 = 83,
    OSXK_KP_2 = 84,
    OSXK_KP_3 = 85,
    OSXK_KP_4 = 86,
    OSXK_KP_5 = 87,
    OSXK_KP_6 = 88,
    OSXK_KP_7 = 89,
    OSXK_KP_8 = 91,
    OSXK_KP_9 = 92,
    OSXK_KP_0 = 82,

    OSXK_Return = 36,
    OSXK_Enter = 76,
    OSXK_NumLock = 71,
    OSXK_Help = 114,
    OSXK_Del = 117,
    OSXK_Home = 115,
    OSXK_End = 119,
    OSXK_Page_Up = 116,
    OSXK_Page_Down = 121,
    OSXK_Ecs = 53,

    OSXK_Up = 126,
    OSXK_Down = 125,
    OSXK_Left = 123,
    OSXK_Right = 124


};

} // end namespace


#endif
