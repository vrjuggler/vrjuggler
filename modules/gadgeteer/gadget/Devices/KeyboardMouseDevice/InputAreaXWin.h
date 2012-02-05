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

#ifndef GADGET_INPUT_AREA_XWIN_H
#define GADGET_INPUT_AREA_XWIN_H

#include <gadget/gadgetConfig.h>

#include <X11/Xlib.h>

#include <gadget/Type/KeyboardMouse/Keys.h>
#include <gadget/Event/Event.h>
#include <gadget/Devices/KeyboardMouseDevice/InputArea.h>


namespace gadget
{
   class KeyboardMouseDevice;
}

namespace gadget
{

/** \class InputAreaXWin InputAreaXWin.h gadget/Devices/KeyboardMouseDevice/InputAreaXWin.h
 *
 * Base class for X11 windows that accept keyboard and mouse input.
 */
class GADGET_API InputAreaXWin
   : public InputArea
{
protected:
   InputAreaXWin();

public:
   virtual ~InputAreaXWin();

protected:
   /** @name gadget::InputArea Interface Implementation */
   //@{
   /**
    * Locks the mouse to the center of this input area.
    *
    * @see unlockMouse()
    *
    * @since 1.3.27
    */
   virtual void lockMouseInternal();

   /**
    * Unlocks the mouse so that it can move again.
    *
    * @since 1.3.27
    */
   virtual void unlockMouseInternal();
   //@}

   void lockMouse(XEvent* ev);

   void unlockMouse(XEvent* ev);

   void updateOriginAndSize(unsigned int width, unsigned int height);

   /**
    * Returns the X11 window attributes for the root display.
    *
    * @since 1.3.16
    */
   const XWindowAttributes getDisplayAttributes() const;

protected:
   /**
    * Handles any events in the system.
    * Copies mKeys to mCurKeys.
    */
   void handleEvents();
   void handleEvent(::XEvent& event);

private:
   /**
    * Handles the interpretation of the given XEvent object as a mouse button
    * press or release event.
    *
    * @post A new gadget::MouseEvent object is added to the keyboard/mouse
    *       event queue with its type set to either
    *       gadget::MouseButtonPressEvent (for an X11 ButtonPress event) or
    *       gadget::MouseButtonReleaseEvent (for an X11 ButtonRelease event).
    *
    * @since 1.3.22
    */
   void handleMouseButtonEvent(const XEvent& event);

   /**
    * Handles the interpretation of the given XEvent object as a mouse scroll
    * event.
    *
    * @post If the event is interpreted properly, a new gadget::MouseEvent
    *       object is added to the keyboard/mouse event queue with its type
    *       set to gadget::MouseScrollEvent.
    *
    * @since 1.3.22
    */
   void handleMouseScrollEvent(const XEvent& event);

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
   void addKeyEvent(const gadget::Keys key, const gadget::EventType type,
                    XKeyEvent* event);

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
   void addMouseButtonEvent(const gadget::Keys button,
                            const gadget::EventType type,
                            const XButtonEvent& event);

   /**
    * Adds a new mouse motion event to the event queue for this window.
    *
    * @post A new event (gadget::MouseEvent) is added to the event queue.
    *
    * @param event The X11 motion event object.
    */
   void addMouseMoveEvent(const XMotionEvent& event);

   /**
    * Constructs a windowing system-independent mask of modifier keys and
    * mouse buttons from the given X11 state value.
    *
    * @param state An integer value from the X Window System that gives the
    *              current state of depressed keyboard modifiers and mouse
    *              buttons.
    */
   int getMask(const int state);

   /**
    * Constructs a windowing system-independent mask of mouse buttons from 
    * the given X11 button state value.
    *
    * @param state An integer value from the X Window System that gives the
    *              current state of depressed mouse buttons.
    *
    * @note There is currently an assumption that exactly one button is in
    *       the pressed state.
    */
   int getButtonMask(const int state);
    
   /**
    * Converts X Window key to gadget::Keys value.
    * @note Keypad keys are transformed ONLY to number keys.
    */
   gadget::Keys xKeyToKey(KeySym xKey);
   
protected:
   /** Do any extra event processing needed. */
   virtual void processEvent(::XEvent)
   {
      /* Do nothing. */ ;
   }

   ::Display*           mXDisplay;
   ::Window             mXWindow;

   unsigned int mWidth;          /**< Input area width */
   unsigned int mHeight;         /**< Input area height */
   bool         mBlocking;       /**< Are we using a display we manage ourselves (true) or a remote one (false). */
   Cursor       mEmptyCursor;    /**< "Blank" cursor for X. */
   bool         mEmptyCursorSet; /**< If true, then empty cursor has been created. */
   int          mPrevX;          /**< Previous mouse location X-coordinate */
   int          mPrevY;          /**< Previous mouse location Y-coordinate */

};

} // end namespace gadget

#endif /*GADGET_INPUT_AREA_XWIN_H*/
