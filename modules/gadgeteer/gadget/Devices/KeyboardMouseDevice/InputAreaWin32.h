/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2011 by Iowa State University
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
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

#ifndef GADGET_INPUT_AREA_WIN32_H
#define GADGET_INPUT_AREA_WIN32_H

#include <gadget/gadgetConfig.h>

#include <vpr/Sync/Mutex.h>

#include <gadget/Devices/KeyboardMouseDevice/InputArea.h>
#include <gadget/Type/KeyboardMouse/Keys.h>
#include <gadget/Event/Event.h>


namespace gadget
{
   class InputWindowWin32;
}

namespace gadget
{

/** \class InputAreaWin32 InputAreaWin32.h gadget/Devices/KeyboardMouseDevice/InputAreaWin32.h
 *
 * Base class for Win32 windows that accept keyboard and mouse input.
 */
class GADGET_API InputAreaWin32
   : public gadget::InputArea
{
protected:
   InputAreaWin32();

public:
   virtual ~InputAreaWin32();

   /**
    * Handles any events in the system.
    * Copies mKeys to mCurKeys.
    */
   void handleEvents();
   void updKeys(const MSG& message);

protected:
   /** @name gadget::InputArea Interface Implementation */
   //@{
   /**
    * Locks the mouse to the center of this input area.
    *
    * @see unlockMouseInternal()
    *
    * @note Renamed from lockMouse() in version 1.3.27.
    */
   virtual void lockMouseInternal();

   /**
    * Unlocks the mouse so that it can move again.
    *
    * @note Renamed from unlockMouse() in version 1.3.27.
    */
   virtual void unlockMouseInternal();
   //@}

   void clipCursorArea();
   void resize(long width, long height);
   gadget::Keys VKKeyToKey(const int vkKey);
   char getAsciiKey(const int vkKey, const gadget::Keys key);
   virtual void addKeyEvent(const gadget::Keys& key,
                            const gadget::EventType type, const MSG& message);
   virtual void addMouseButtonEvent(const gadget::Keys& button,
                                    const gadget::EventType type,
                                    const MSG& message);
   virtual void addMouseMoveEvent(const float deltaX, const float deltaY,
                                  const MSG& message);

   /**
    * Adds a new mouse scroll event to the event queue for this input area.
    *
    * @post A new event (gadget::MouseEvent) is added to the event queue.
    *
    * @param deltaX The change in the X scrolling axis. A negative value
    *               indicates scrolling left, and a positive value indicates
    *               scrolling right.
    * @param deltaY The change in the Y scrolling axis. A positive value
    *               indicates scrolling up, and a negative value indicates
    *               scrolling down.
    * @param msg    The message from the windowing system describing the
    *               scroll event.
    *
    * @since 1.3.22
    */
   void addMouseScrollEvent(const float deltaX, const float deltaY,
                            const MSG& msg);

   int getModifierMask();
   int getButtonMask();
   static void doInternalError( const std::string& msg );

protected:
   HWND         mWinHandle;     /**< Window handle */
   unsigned int mWidth;         /**< Input area width */
   unsigned int mHeight;        /**< Input area height */
   bool         mBlocking;      /**< Are we using a display we manage ourselves (true) or a remote one (false). */
   vpr::Mutex   mKeysLock;      /**< Must hold this lock when accessing mKeys */
   int          mPrevX;         /**< Previous mouse location X-coordinate */
   int          mPrevY;         /**< Previous mouse location Y-coordinate */
};

} // end namespace gadget

#endif /*GADGET_INPUT_AREA_WIN32_H*/
