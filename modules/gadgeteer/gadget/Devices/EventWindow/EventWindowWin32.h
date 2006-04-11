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


/////////////////////////////////////////////////////////////////////////////
// EventWindowWin32.h
//
// Event window input device for win32
//
// History:
//
// Andy Himberger:    v0.0 - 12-1-97 - Inital version
// Allen Bierbaum:    v1.0 -  7-23-99 - Refactored to use new keyboard method
////////////////////////////////////////////////////////////////////////////

#ifndef _GADGET_EVENT_WINDOW_WIN32_H_
#define _GADGET_EVENT_WINDOW_WIN32_H_

#include <gadget/gadgetConfig.h>

// Get windows stuff
#include <windows.h>
#include <commctrl.h>

#include <vpr/Thread/Thread.h>
#include <vpr/System.h>
#include <jccl/Config/ConfigChunk.h>

#include <gadget/Type/Input.h>
#include <gadget/Type/EventWindow.h>
#include <gadget/Type/InputMixer.h>

#include <gadget/Type/EventWindow/Keys.h>
#include <gadget/Type/EventWindow/Event.h>


namespace gadget
{

class GADGET_CLASS_API EventWindowWin32
#if _MSC_VER < 1310  // 1310 == VC++ 7.1
   : public Input, public EventWindow
#else
   : public InputMixer<Input, EventWindow>
#endif
{
public:
   /**< Enum to keep track of current lock state for state machine. */
   enum lockState
   {
      Unlocked,     /**< The mouse is free. */
      Lock_LockKey, /**< The mouse is locked due to lock toggle key press. */
      Lock_KeyDown  /**< The mouse is locked due to a key being held down. */
   };

   EventWindowWin32()
      : mControlLoopDone(false),
        mPrevX(0),
        mPrevY(0),
        mLockState(Unlocked),
        mExitFlag(false),
        mWeOwnTheWindow(true)
   {
   }

   virtual ~EventWindowWin32()
   {
      stopSampling();
   }

   virtual bool config(jccl::ConfigChunkPtr c);

   /** Main thread of control for this active object. */
   void controlLoop( void* nullParam );

   /* Pure Virtuals required by gadget::Input. */
   int startSampling();
   int stopSampling();

   /**
    * Processes the current events.
    * Called repetatively by the controlLoop.
    */
   int sample();
   void updateData();
   static std::string getChunkType();

   /**
    * Returns the number of times the key was pressed during the last frame.
    * You can put this in an if to check if was pressed at all, or if you are
    * doing processing based on this catch the actual number.
    */
   int isKeyPressed(int Key);

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

   virtual void processEvent(UINT message, UINT wParam, LONG lParam)
   {
   }

   void addKeyEvent(const gadget::Keys& key, const gadget::EventType& type,
                    const MSG& message);

   void addMouseButtonEvent(const gadget::Keys& button,
                            const gadget::EventType& type, const MSG& message);

   void addMouseMoveEvent(const MSG& message);

   void lockMouse();
   void unlockMouse();
   friend LONG APIENTRY MenuWndProc(HWND hWnd, UINT message, UINT wParam, LONG lParam);

   HINSTANCE   m_hInst;
   HWND        m_hWnd;
   void createWindowWin32();
   void updKeys(const MSG& message);

   /**
    * Handles any events in the system.
    * Copies mKeys to mCurKeys.
    */
   void handleEvents();

   /** @name Private functions for processing input data */
   /* Private functions for processing input data */
   //@{
   int onlyModifier(int);
   //@}

   /** @name Windows utility functions */
   //@{
   gadget::Keys VKKeyToKey(int vkKey);
   char* checkArgs(char* look_for);

   BOOL MenuInit(HINSTANCE hInstance);
   //@}

   bool         mWeOwnTheWindow; /**< True if this class owns the window (is reposible for opening, closing, and event processing). */
   int          mScreen, mX, mY; /**< screen id, x-origin, y-origin. */
   unsigned int mWidth,mHeight;

      /* Event window state holders */
   // NOTE: This driver does not use the normal triple buffering mechanism.
   // Instead, it just uses a modified double buffering system.
   int      mKeys[gadget::LAST_KEY];         /**< (0,*): The num key presses during an UpdateData (ie. How many keypress events). */
   int      mRealkeys[gadget::LAST_KEY];     /**< (0,1): The real keyboard state, all events processed (ie. what is the key now). */

   vpr::Mutex  mKeysLock;        /**< Must hold this lock when accessing mKeys. */
   bool     mExitFlag;           /**< Should we exit? */

   lockState   mLockState;       /**< The current state of locking. */
   int         mLockStoredKey;   /**< The key that was pressed down. */
   int         mLockToggleKey;   /**< The key that toggles the locking. */

   float mMouseSensitivity;
   int   mSleepTimeMS;            /**< Amount of time to sleep in milliseconds between updates. */
   int   mPrevX, mPrevY;          /**< Previous mouse location. */
   bool  mControlLoopDone;
};

} // End of gadget namespace

#endif
