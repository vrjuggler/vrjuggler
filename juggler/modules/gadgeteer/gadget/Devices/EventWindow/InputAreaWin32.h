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

#ifndef GADGET_INPUT_AREA_WIN32_H
#define GADGET_INPUT_AREA_WIN32_H

#include <gadget/gadgetConfig.h>
#include <gadget/Util/Debug.h>
#include <gadget/Type/Input.h>
#include <gadget/Type/EventWindow/KeyEvent.h>
#include <gadget/Type/EventWindow/MouseEvent.h>
#include <gadget/Type/EventWindow/Keys.h>
#include <gadget/Type/EventWindow/Event.h>

#include <vpr/IO/BufferObjectReader.h>

#ifndef GET_X_LPARAM
#  define GET_X_LPARAM(lp)   ((int)(short)LOWORD(lp))
#endif
#ifndef GET_Y_LPARAM
#  define GET_Y_LPARAM(lp)   ((int)(short)HIWORD(lp))
#endif

namespace gadget
{
   class EventWindowWin32;
}

namespace gadget
{
class GADGET_CLASS_API InputAreaWin32
{
public:  // --- Internal helper class ----- //
   /**
    * Holds list of registered windows that may be used for Win32 Input.
    * This is used by EventWindow routines to find any windows
    * opened by other system components but that we still want to get input
    * from.
    */
   class GADGET_CLASS_API InputAreaRegistry
   {
   public:
      struct InputAreaInfo
      {
         std::string mDisplayName;            /**< The Win32 display name the window is on. */
         gadget::InputAreaWin32* mInputArea;  /**< The handle to the window. */
      };

   public:
      InputAreaRegistry()
      {;}

      /** Add the given window to the registry.
       * @return true if window is added, false if matches existing window name.
       */
      bool addInputArea(std::string name, InputAreaInfo winInfo);

      /** Remove the window with the id of "name". */
      void removeInputArea(std::string name);

      /** Get the window information. */
      bool getInputArea(std::string name, InputAreaInfo& winInfo);

   protected:
      typedef std::map<std::string,InputAreaInfo> input_area_map_t;
      input_area_map_t    mInputAreaMap;    /**< Map Window name to the data needed for it. */

      vprSingletonHeader(InputAreaRegistry);
   };
public:
   /** Enum to keep track of current lock state for state machine. */
   enum lockState
   {
      Unlocked,     /**< The mouse is free. */
      Lock_LockKey, /**< The mouse is locked due to lock toggle key press. */
      Lock_KeyDown  /**< The mouse is locked due to a key being held down. */
   };

   InputAreaWin32();
   ~InputAreaWin32();

   /**
    * Handles any events in the system.
    * Copies mKeys to mCurKeys.
    */
   void handleEvents();
   void updKeys(const MSG& message);
   virtual void setDelegate(gadget::EventWindowWin32* delegate)
   {
      mEventDelegate = delegate;
   }

protected:
   void lockMouse();
   void unlockMouse();
   void resize(long width, long height);
   virtual void addKeyEvent(const gadget::Keys& key,
                            const gadget::EventType& type, const MSG& message);
   virtual void addMouseButtonEvent(const gadget::Keys& button,
                                    const gadget::EventType& type,
                                    const MSG& message);
   virtual void addMouseMoveEvent(const MSG& message);
   gadget::Keys VKKeyToKey(int vkKey);
   static void doInternalError( const std::string& msg );

protected:
   EventWindowWin32* mEventDelegate;
   HWND              mWinHandle;       /**< Window handle */

   // NOTE: This driver does not use the normal triple buffering mechanism.
   // Instead, it just uses a modified double buffering system.
   /* Event window state holders */
   /**
    * (0,*): The num key presses during an UpdateData (ie. How many keypress
    * events).
    */
   int mKeys[gadget::LAST_KEY];

   /**
    * (0,1): The real keyboard state, all events processed (ie. what is the
    * key now).
    */
   int mRealkeys[gadget::LAST_KEY];

   lockState    mLockState;       /**< The current state of locking. */
   int          mLockStoredKey;   /**< The key that was pressed down. */
   int          mLockToggleKey;   /**< The key that toggles the locking. */
   unsigned int mWidth,mHeight;
   bool         mUseOwnDisplay;   /**< Are we using a display we manage ourselves (true) or a remote one (false). */
   vpr::Mutex   mKeysLock;        /**< Must hold this lock when accessing mKeys. */
   int   mPrevX, mPrevY;         /**< Previous mouse location. */
};

} // end namespace gadget

#endif /*GADGET_INPUT_AREA_WIN32_H*/
