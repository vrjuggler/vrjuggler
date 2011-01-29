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

#ifndef _GADGET_INPUT_AREA_COCOA_H_
#define _GADGET_INPUT_AREA_COCOA_H_

#include <gadget/gadgetConfig.h>

#include <vpr/Sync/Mutex.h>

#include <gadget/Event/Event.h>
#include <gadget/Devices/KeyboardMouseDevice/InputArea.h>

// Allow this header to be included by C++ and Objective-C++ code.
#if defined(__OBJC__)
@class NSConditionLock;
@class NSWindow;
@class NSEvent;
@class NSView;
#else
class NSConditionLock;
class NSWindow;
class NSEvent;
class NSView;
#endif

namespace gadget
{

/** \class InputAreaCocoa InputAreaCocoa.h gadget/Devices/KeyboardMouseDevice/InputAreaCocoa.h
 *
 * Base class for Cocoa windows that accept keyboard and mouse input. This
 * is implemented using Objective-C++ to make the connection between Cocoa
 * (an Objective-C API) and Gadgeteer. This class can (and is) used by plain
 * C++ code.
 *
 * @see InputViewCocoa
 */
class GADGET_CLASS_API InputAreaCocoa : public InputArea
{
protected:
   InputAreaCocoa();

public:
   virtual ~InputAreaCocoa();

   /**
    * Returns the global lock used to ensure that windows are not opened
    * before the primordial thread enters its run loop. It is up to the
    * calling code to acquire this lock prior to spawning the thread(s) that
    * will open windows and then to release it once the run loop starts. The
    * NSApp main controller should unlock this lock in its
    * \c -applicationDidFinishLaunching: method.
    */
   static NSConditionLock* getWindowLock()
   {
      return sWinOpenLock;
   }

   /**
    * @name Event Queue Manipulators
    *
    * These methods are public (regrettably) so that they can be invoked by
    * methods of InputViewCocoa. Objective-C has no concept of friends.
    *
    * @note These methdos are invoked by InputViewCocoa from the event loop.
    *       This will be in the primordial thread, meaning that it will happen
    *       in parallel to the Input Manager's update loop.
    */
   //@{

   /**
    * Adds a new key press/release event to the event queue for this input
    * area. The key that was modified is extracted from the given event
    * object.
    *
    * @post A new event (gadget::KeyEvent) is added to the event queue.
    *
    * @param type  The type of key event (KeyPress or KeyRelease).
    * @param event A pointer to the Cocoa event structure associated with the
    *              key event.
    */
   void addKeyEvent(const EventType type, NSEvent* event);

   /**
    * Adds a new modifier key press/release event to the event queue for this
    * input area. This overload does not perform any translation on the
    * characters associated with the NSEvent object and expects that there are
    * zero or one such characters.
    *
    * @post A new event (gadget::KeyEvent) is added to the event queue.
    *
    * @param key   The modifier key whose state changed.
    * @param type  The type of key event (KeyPress or KeyRelease).
    * @param event A pointer to the Cocoa event structure associated with the
    *              key event.
    */
   void addModifierEvent(const Keys key, const EventType type, NSEvent* event);

   /**
    * Adds a new mouse motion event to the event queue for this input area.
    * The mouse button is determined by converting the value returned from
    * the NSEvent method -buttonNumber.
    *
    * @post A new event (gadget::MouseEvent) is added to the event queue.
    *
    * @param type  The type of button event (MouseButtonPressEvent or
    *              MouseButtonReleaseEvent).
    * @param event A pointer to the Cocoa event structure associated with the
    *              button event.
    */
   void addMouseButtonEvent(const EventType type, NSEvent* event);

   /**
    * Adds a new mouse motion event to the event queue for this input area.
    *
    * @post A new event (gadget::MouseEvent) is added to the event queue.
    *
    * @param button The mouse button whose state changed.
    * @param type   The type of button event (MouseButtonPressEvent or
    *               MouseButtonReleaseEvent).
    * @param event  A pointer to the Cocoa event structure associated with the
    *               button event.
    */
   void addMouseButtonEvent(const Keys button, const EventType type,
                            NSEvent* event);

   /**
    * Adds a new mouse button press/release event to the event queue for this
    * input area.
    *
    * @post A new event (gadget::MouseEvent) is added to the event queue.
    *
    * @param event A pointer to the Cocoa event structure associated with the
    *              mouse movement event.
    */
   void addMouseMoveEvent(NSEvent* event);

   /**
    * Adds a new mouse scroll event to the event queue for this input area.
    *
    * @post A new event (gadget::MouseEvent) is added to the event queue.
    *
    * @param event A pointer to the Cocoa event structure associated with the
    *              mouse scroll event.
    *
    * @since 1.3.22
    */
   void addMouseScrollEvent(NSEvent* event);

   /**
    * Transforms a change in the modifier flags into a key press or key
    * release event.
    *
    * @post \c mLastModifiers holds the new set of pressed modifier keys.
    */
   void flagsChanged(NSEvent* event);
   //@}

   /**
    * Updates the size of this input area.
    *
    * @since 1.3.15
    */
   void resize(const float width, const float height);

protected:
   /** @name gadget::InputArea Interface Implementation */
   //@{
   /**
    * Locks the mouse to the center of this input area.
    *
    * @see unlockMouse()
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

   /**
    * Translates the mouse button number into a gadget::Keys value.
    */
   Keys getButtonFromNum(const int buttonNum) const;

   /**
    * Converts \p mask into the gadget::Keys corresponding value. The value of
    * \p mask must be a mask value.
    */
   Keys getKeyFromModifierMask(const unsigned int mask) const;

   void warpCursorToCenter();

   /**
    * Constructs a windowing system-independent mask of modifier keys from the
    * given Cocoa modifiers.
    *
    * @param modifiers An integer value from Cocoa that gives the current
    *                  state of depressed keyboard modifiers.
    */
   int getMask(const unsigned int modifiers) const;

   bool isModifier(const Keys key) const;

   /**
    * Converts Cocoa virtual key to gadget::Keys value.
    *
    * @param event
    *
    * @note Keypad keys are transformed ONLY to number keys.
    */
   Keys vKeyToKey(const unsigned short keyChar, const unsigned short keyCode,
                  const unsigned int modifiers);

   /**
    * Press and release state changes for keys and mouse buttons are recorded
    * in \c mKeyboardMouseDevice->mRealkeys and \c mKeyboardMouseDevice->mKeys.
    *
    * @post If the event type is gadget::KeyPressEvent,
    *       gadget::KeyReleaseEvent, gadget::MouseButtonPressEvent, or
    *       gadget::MouseButtonReleaseEvent, then the identified key has its
    *       state in \c mKeyboardMouseDevice updated accordingly.
    */
   void storeEventKey(const EventType eventType, const Keys key);

   /**
    * Adds the given event to the keyboard/mouse event queue.
    *
    * @pre \p key has been stored in \c mKeyboardMouseDevice (use
    *      storeEventKey() to do this).
    *
    * @see storeEventKey()
    */
   void doAddEvent(EventPtr event, const Keys key);

   /**
    * Clips the cursor so that when the mouse is being dragged that the mouse
    * stays within the given input area.
    */
   void clipCursorArea();

   /**
    * A global lock used to ensure that no windows are opened until the
    * application has entered its run loop in the primordial thread. This
    * allows windows to be opened from a thread other than the non-primoardial
    * thread prior to the primordial thread entering its run loop. (Trying to
    * open windows before the primordial thread enters its run loop will cause
    * the application to crash.)
    */
   static NSConditionLock* sWinOpenLock;

   /** @name Input Area Data */
   //@{
   float mWidth;        /**< Input area width */
   float mHeight;       /**< Input area height */

   unsigned int mLastModifiers; /**< The current keyboard modifiers */

   NSWindow* mCocoaWindow;      /**< The window for this input area. */
   NSView*   mMainView;         /**< The window's view. */
   //@}
};

}


#endif /* _GADGET_INPUT_AREA_COCOA_H_ */
