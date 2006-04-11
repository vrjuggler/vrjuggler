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

#ifndef _GADGET_EVENT_WINDOW_XWIN_H_
#define _GADGET_EVENT_WINDOW_XWIN_H_

#include <gadget/gadgetConfig.h>

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/keysym.h>
#include <boost/concept_check.hpp>

#include <vpr/Sync/Mutex.h>

#include <gadget/Type/Input.h>
#include <gadget/Type/EventWindow.h>
#include <gadget/Type/InputMixer.h>
#include <gadget/Type/EventWindow/Event.h>

#include <jccl/Config/ConfigChunkPtr.h>


namespace gadget
{

/**
 * X Winndow System event window class.
 * This device is a source of keyboard events.  The device should not be
 * used directly, it should be referenced through proxies.
 *
 * Mouse Locking:<br>
 *    This device recieves input from the XWindows display.  As such,
 *  the xwindow must have focus to generate events.  In order to help
 *  users keep the window in focus, there are two cases where the
 *  driver will "lock" the mouse to the window, thus preventing loss of focus.<br>
 *  CASE 1: The user holds down any key. (ie. a,b, ctrl, shift, etc)<br>
 *  CASE 2: The user can toggle locking using a special "locking" key
 *           defined in the configuration chunk.
 *
 * @see EventWindow, EventWindowProxy
 */
class EventWindowXWin : public InputMixer<Input,EventWindow>
{
public:
   /** Enum to keep track of current lock state for state machine. */
   enum lockState
   {
      Unlocked,     /**< The mouse is free */
      Lock_LockKey, /**< The mouse is locked due to lock toggle key press */
      Lock_KeyDown  /**< The mouse is locked due to a key being held down */
   };

   EventWindowXWin()
      : mWeOwnTheWindow(true), mVisual(NULL), mDisplay(NULL),
        mEmptyCursorSet(false), mExitFlag(false), mControlLoopDone(false),
        mLockState(Unlocked), mPrevX(0), mPrevY(0)
   {
      //mThread = NULL; -- Should be done in base constructor
   }

   ~EventWindowXWin()
   {
      stopSampling();
   }

   virtual bool config(jccl::ConfigChunkPtr c);

   /** Main thread of control for this active object. */
   void controlLoop(void* nullParam);

   /** @name Pure Virtuals required by Input */
   //@{
   int startSampling();
   int stopSampling();
   //@}

   /**
    * Processes the current x-events.
    * Called repetatively by the controlLoop.
    */
   int sample()
   {
      HandleEvents();
      return 1;
   }

   void updateData();

   static std::string getChunkType()
   {
      return std::string("EventWindow");
   }

   /**
    * Returns the number of times the key was pressed during the last frame.
    * You can put this in an if to check if was pressed at all, or if you are
    * doing processing based on this catch the actual number.
    */
   int isKeyPressed(int Key)
   {
      return mCurKeys[Key];
   }

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

   /** Do any extra event processing needed. */
   virtual void processEvent(XEvent event)
   {
      boost::ignore_unused_variable_warning(event);
   }

private:
   /** @name Private functions for processing input data. */
   //@{
   int onlyModifier(int);
   //@}

   /** @name Event handling utility methods. */
   //@{
   /**
    * Handles any events in the system.
    * Copies mKeys to mCurKeys.
    */
   void HandleEvents();

   /**
    * Adds a new key press/release event to the event queue for this window.
    *
    * @post A new event (gadget::KeyEvent) is added to the event queue.
    *
    * @param key   The key whose state changed.
    * @param type  The type of key event (KeyPress or KeyRelease).
    * @param event A pointer to the X Window System key event structure
    *              associated with the event.
    */
   void addKeyEvent(const gadget::Keys& key, const gadget::EventType& type,
                    XKeyEvent* event);

   /**
    * Adds a new mouse motion event to the event queue for this window.
    *
    * @post A new event (gadget::MouseEvent) is added to the event queue.
    *
    * @param event The X11 motion event object.
    */
   void addMouseMoveEvent(const XMotionEvent& event);

   /**
    * Adds a new mouse button press/release event to the event queue for this
    * window.
    *
    * @post A new event (gadget::MouseEvent) is added to the event queue.
    *
    * @param button The button that triggered the event.
    * @param event  The X11 button event object.
    * @param type   The type of mouse button event (MouseButtonPress or
    *               MouseButtonRelease).
    */
   void addMouseButtonEvent(const gadget::Keys& button,
                            const XButtonEvent& event,
                            const gadget::EventType& type);

   /**
    * Constructs a windowing system-independent mask of modifier keys and
    * mouse buttons from the given X11 state value.
    *
    * @param state An integer value from the X Window System that gives the
    *              current state of depressed keyboard modifiers and mouse
    *              buttons.
    */
   int getMask(const int& state);
   //@}

   /** @name X-Window System utility functions */
   //@{
   /**
    * Converts X Window key to Key.
    * @note Keypad keys are transformed ONLY to number keys.
    */
   gadget::Keys xKeyToKey(::KeySym xKey);

   /** Opens the X window to sample from. */
   int openTheWindow();

   ::Window createWindow(::Window parent, unsigned int border,
                         unsigned long fore, unsigned long back,
                         unsigned long event_mask);

   void setHints(::Window window, char*  window_name, char*  icon_name,
                 char* class_name, char* class_type);
   //@}

   /** Perform anything that must be done when state switches. */
   void lockMouse();
   void unlockMouse();

protected:
   void createEmptyCursor(Display* display, Window root);

   bool mWeOwnTheWindow; /**< True if this class owns the window (is reposible for opening, closing, and event processing). */

   ::Window       mWindow;
   ::XVisualInfo* mVisual;
   ::Display*     mDisplay;
   ::XSetWindowAttributes mSWA;
   int          mScreen, mX, mY;    /**< screen id, x-origin, y-origin. */
   unsigned int mWidth, mHeight;

   Cursor mEmptyCursor;
   bool   mEmptyCursorSet;

   /** @name EventWindow state holders
    * @note This driver does not use the normal triple buffering mechanism.
    * Instead, it just uses a modified double buffering system.
    */
   //@{
   int        mKeys[gadget::LAST_KEY];     /**< (0,*): The num key presses during an UpdateData (ie. How many keypress events). */

   int        mRealkeys[gadget::LAST_KEY]; /**< (0,1): The real keyboard state, all events processed (ie. what is the key now). */
   vpr::Mutex mKeysLock;         /**< Must hold this lock when accessing m_keys. */
   bool       mExitFlag;         /**< Should we exit? */
   bool       mControlLoopDone;

   lockState  mLockState;        /**< The current state of locking. */
   int        mLockStoredKey;    /**< The key that was pressed down. */
   int        mLockToggleKey;    /**< The key that toggles the locking. */
   //@}

   std::string mXDisplayString;  /**< The display string to use from systemDisplay config info. */

   float mMouseSensitivity;
   int   mSleepTimeMS;           /**< Amount of time to sleep in milliseconds between updates. */
   int   mPrevX, mPrevY;         /**< Previous mouse location. */
};

} // end namespace

#endif
