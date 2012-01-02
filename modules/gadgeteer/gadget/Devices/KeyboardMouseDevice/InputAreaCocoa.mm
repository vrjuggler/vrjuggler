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

#include <gadget/gadgetConfig.h>

#include <gadget/Event/KeyboardMouse/KeyEvent.h>
#include <gadget/Event/KeyboardMouse/MouseEvent.h>
#include <gadget/Util/Debug.h>

#include <gadget/Devices/KeyboardMouseDevice/InputAreaCocoa.h>

// This header must be included before the Cocoa headers to avoid declaration
// problems.
#include <ApplicationServices/ApplicationServices.h>

#import <Foundation/NSString.h>
#import <Foundation/NSLock.h>
#import <AppKit/NSCursor.h>
#import <AppKit/NSEvent.h>
#import <AppKit/NSView.h>
#import <AppKit/NSWindow.h>
#import <mach/mach_time.h>


namespace gadget
{

NSConditionLock* InputAreaCocoa::sWinOpenLock = [[NSConditionLock alloc] init];

InputAreaCocoa::InputAreaCocoa()
   : InputArea()
   , mWidth(0)
   , mHeight(0)
   , mLastModifiers(0)
   , mCocoaWindow(nil)
   , mMainView(nil)
{
   /* Do nothing. */ ;
}

InputAreaCocoa::~InputAreaCocoa()
{
   /* Do nothing. */ ;
}

void InputAreaCocoa::addKeyEvent(const EventType type, NSEvent* event)
{
   const unsigned int modifiers = [event modifierFlags];

   // The Caps Lock key is treated as a modifier in Mac OS X. We treat it as
   // a key press, though this does not make a lot of sense since it is a
   // stateful button on the keyboard.
   if ( modifiers & NSAlphaShiftKeyMask )
   {
      storeEventKey(type, KEY_CAPS_LOCK);
      EventPtr key_event(new KeyEvent(type, KEY_CAPS_LOCK, getMask(modifiers),
                                      mach_absolute_time() * 0.000001, this));
      doAddEvent(key_event, KEY_CAPS_LOCK);
   }
   else
   {
      NSString* key_chars = [event charactersIgnoringModifiers];
      if ( [key_chars length] >= 1 )
      {
         if ( [key_chars length] > 1 )
         {
            vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_WARNING_LVL)
               << clrOutBOLD(clrYELLOW, "WARNING")
               << ": Multi-character keyboard events currently extract only "
               << "the first character!" << std::endl << vprDEBUG_FLUSH;
         }

         const unichar key_char = [key_chars characterAtIndex:0];
         const Keys key = vKeyToKey(key_char, [event keyCode], modifiers);

         storeEventKey(type, key);
         EventPtr key_event(new KeyEvent(type, key,
                                         getMask([event modifierFlags]),
                                         mach_absolute_time() * 0.000001,
                                         this, static_cast<char>(key_char),
                                         key_char));
         doAddEvent(key_event, key);
      }
   }
}

void InputAreaCocoa::addModifierEvent(const Keys key, const EventType type,
                                      NSEvent* event)
{
   storeEventKey(type, key);
   EventPtr key_event(new KeyEvent(type, key, getMask([event modifierFlags]),
                                   mach_absolute_time() * 0.000001, this));
   doAddEvent(key_event, key);
}

void InputAreaCocoa::addMouseButtonEvent(const EventType type, NSEvent* event)
{
   // If the user uses the scroll wheel then set the button output
   // appropriately. This behavior is deprecated.
   if ( [event type] == NSScrollWheel )
   {
      vprASSERT(mKeyboardMouseDevice->useButtonsForScrolling());

      if ( [event deltaY] > 0.0f )
      {
         addMouseButtonEvent(mKeyboardMouseDevice->getScrollUpButton(),
                             type, event);
      }
      else if ( [event deltaY] < 0.0f )
      {
         addMouseButtonEvent(mKeyboardMouseDevice->getScrollDownButton(),
                             type, event);
      }
      else if ( [event deltaX] > 0.0f )
      {
         addMouseButtonEvent(mKeyboardMouseDevice->getScrollLeftButton(),
                             type, event);
      }
      else
      {
         addMouseButtonEvent(mKeyboardMouseDevice->getScrollRightButton(),
                             type, event);
      }
   }
   else
   {
      addMouseButtonEvent(getButtonFromNum([event buttonNumber]), type, event);
   }
}

void InputAreaCocoa::addMouseButtonEvent(const Keys button,
                                         const EventType type, NSEvent* event)
{
   const NSPoint view_loc = [mMainView convertPoint:[event locationInWindow]
                                           fromView:nil];
   const NSPoint root_loc = [NSEvent mouseLocation];

   storeEventKey(type, button);
   EventPtr mouse_event(new MouseEvent(type, button, view_loc.x, view_loc.y,
                                       root_loc.x, root_loc.y, 0.0f, 0.0f,
                                       getMask([event modifierFlags]),
                                       mach_absolute_time() * 0.000001,
                                       this));
   doAddEvent(mouse_event, button);
}

void InputAreaCocoa::addMouseMoveEvent(NSEvent* event)
{
   const NSPoint view_loc = [mMainView convertPoint:[event locationInWindow]
                                           fromView:nil];
   const NSPoint root_loc = [NSEvent mouseLocation];
   EventPtr move_event(new MouseEvent(MouseMoveEvent, NO_MBUTTON, view_loc.x,
                                      view_loc.y, root_loc.x, root_loc.y,
                                      0.0f, 0.0f,
                                      getMask([event modifierFlags]),
                                      mach_absolute_time() * 0.000001, this));

   const float dx = [event deltaX];
   const float dy = [event deltaY];

   if ( mLockState != Unlocked )
   {
      warpCursorToCenter();
   }

   NSEventType mouse_event_type = [event type];
   if ( mouse_event_type == NSLeftMouseDragged  ||
        mouse_event_type == NSRightMouseDragged ||
        mouse_event_type == NSOtherMouseDragged )
   {
       // Clip the cursor area whenever the mouse button is down and dragging.
       clipCursorArea();
   }

   vpr::Guard<vpr::Mutex> guard(mKeyboardMouseDevice->mKeysLock);

   if ( dx > 0 )
   {
      mKeyboardMouseDevice->mKeys[MOUSE_POSX] += dx;
   }
   else
   {
      mKeyboardMouseDevice->mKeys[MOUSE_NEGX] += -dx;
   }

   if ( dy > 0 )
   {
      mKeyboardMouseDevice->mKeys[MOUSE_POSY] += dy;
   }
   else
   {
      mKeyboardMouseDevice->mKeys[MOUSE_NEGY] += -dy;
   }

   mKeyboardMouseDevice->addEvent(move_event);
}

void InputAreaCocoa::addMouseScrollEvent(NSEvent* event)
{
   // Check for handling of the deprecated scrolling behavior.
   if ( mKeyboardMouseDevice->useButtonsForScrolling() )
   {
      // Handling this as a press event and then as a release event is to be
      // consistent with the X Window System.
      addMouseButtonEvent(MouseButtonPressEvent, event);
      addMouseButtonEvent(MouseButtonReleaseEvent, event);
   }
   // Use the new scrolling behavior.
   else
   {
      const NSPoint view_loc = [mMainView convertPoint:[event locationInWindow]
                                              fromView:nil];
      const NSPoint root_loc = [NSEvent mouseLocation];

      // Note that the X delta value is negated. Treating values in the
      // negative X axis as scrolling to the right seems counter-intuitive.
      // By negating the value, we get a postive X delta for scrolling to the
      // right and a negative X delta for scrolling to the left.
      const float dx(-[event deltaX]);
      const float dy([event deltaY]);
      EventPtr scroll_event(new MouseEvent(MouseScrollEvent, NO_MBUTTON,
                                           view_loc.x, view_loc.y, root_loc.x,
                                           root_loc.y, dx, dy,
                                           getMask([event modifierFlags]),
                                           mach_absolute_time() * 0.000001,
                                           this));

      // Hold the keys lock for only as long as we need it.
      {
         vpr::Guard<vpr::Mutex> guard(mKeyboardMouseDevice->mKeysLock);

         // Handle the deltas for the X and Y axes separately. A mouse with a
         // roller ball can scroll in both axes simultaneously.

         if ( dx > 0.0f )
         {
            mKeyboardMouseDevice->mKeys[MOUSE_SCROLL_RIGHT] +=
               static_cast<int>(dx);
         }
         else if ( dx < 0.0f )
         {
            mKeyboardMouseDevice->mKeys[MOUSE_SCROLL_LEFT] +=
               static_cast<int>(-dx);
         }

         if ( dy > 0.0f )
         {
            mKeyboardMouseDevice->mKeys[MOUSE_SCROLL_UP] +=
               static_cast<int>(dy);
         }
         else if ( dy < 0.0f )
         {
            mKeyboardMouseDevice->mKeys[MOUSE_SCROLL_DOWN] +=
               static_cast<int>(-dy);
         }
      }

      mKeyboardMouseDevice->addEvent(scroll_event);
   }
}

void InputAreaCocoa::flagsChanged(NSEvent* event)
{
   const unsigned int cur_modifiers = [event modifierFlags];

   // Compare the new modifier set with the old to find out what modifier
   // key has been pressed or released.
   const unsigned int new_modifiers = ~mLastModifiers & cur_modifiers;
   const unsigned int old_modifiers = mLastModifiers & ~cur_modifiers;

   if ( new_modifiers != 0 )
   {
      addModifierEvent(getKeyFromModifierMask(new_modifiers), KeyPressEvent,
                       event);
   }
   else if ( old_modifiers != 0 )
   {
      addModifierEvent(getKeyFromModifierMask(old_modifiers), KeyReleaseEvent,
                       event);
   }

   mLastModifiers = cur_modifiers;
}

void InputAreaCocoa::resize(const float width, const float height)
{
   mWidth  = width;
   mHeight = height;
}

void InputAreaCocoa::lockMouseInternal()
{
   NSPoint lock_center = [mCocoaWindow mouseLocationOutsideOfEventStream];
   mLockXCenter = lock_center.x;
   mLockYCenter = lock_center.y;

   [NSCursor hide];
}

void InputAreaCocoa::unlockMouseInternal()
{
   [NSCursor unhide];
}

Keys InputAreaCocoa::getButtonFromNum(const int buttonNum) const
{
   Keys button(NO_MBUTTON);

   switch ( buttonNum )
   {
      case 0:
         button = MBUTTON1;
         break;
      // To be consistent with other windowing systems, we treat button 1 (the
      // right button in AppKit's view) as button 3.
      case 1:
         button = MBUTTON3;
         break;
      // We treat button 2 (the first "other" button for AppKit, which tends
      // to be the middle button) as button 2.
      case 2:
         button = MBUTTON2;
         break;
      case 3:
         button = MBUTTON4;
         break;
      case 4:
         button = MBUTTON5;
         break;
      case 5:
         button = MBUTTON6;
         break;
      case 6:
         button = MBUTTON7;
         break;
      case 7:
         button = MBUTTON8;
         break;
      case 8:
         button = MBUTTON9;
         break;
   }

   return button;
}

Keys InputAreaCocoa::getKeyFromModifierMask(const unsigned int mask) const
{
   Keys key(KEY_NONE);

   if ( mask & NSCommandKeyMask )
   {
      key = KEY_COMMAND;
   }
   else if ( mask & NSAlternateKeyMask )
   {
      key = KEY_ALT;
   }
   else if ( mask & NSControlKeyMask )
   {
      key = KEY_CTRL;
   }
   else if ( mask & NSShiftKeyMask )
   {
      key = KEY_SHIFT;
   }

   return key;
}

void InputAreaCocoa::warpCursorToCenter()
{
   NSRect frame = [mCocoaWindow frame];
   NSRect screen_frame = [[mCocoaWindow screen] frame];

   // Apparently Core Graphics uses a different coordinate frame than Cocoa.
   CGPoint center_pt =
      { frame.origin.x + mLockXCenter,
        screen_frame.size.height - (frame.origin.y + mLockYCenter)
      };
   CGWarpMouseCursorPosition(center_pt);
}

int InputAreaCocoa::getMask(const unsigned int modifiers) const
{
   int mask(0);

   if ( modifiers & NSShiftKeyMask )
   {
      mask |= SHIFT_MASK;
   }

   if ( modifiers & NSControlKeyMask )
   {
      mask |= CTRL_MASK;
   }

   if ( modifiers & NSAlternateKeyMask )
   {
      mask |= ALT_MASK;
   }

   if ( modifiers & NSCommandKeyMask )
   {
      mask |= KEY_COMMAND;
   }

   // XXX: mKeyboardMouseDevice->mKeysLock is not held here! This may not
   // really matter for Cocoa since all event handling has to be done in the
   // primordial thread.

   if ( mKeyboardMouseDevice->mKeys[MBUTTON1] )
   {
      mask |= BUTTON1_MASK;
   }

   if ( mKeyboardMouseDevice->mKeys[MBUTTON2] )
   {
      mask |= BUTTON2_MASK;
   }

   if ( mKeyboardMouseDevice->mKeys[MBUTTON3] )
   {
      mask |= BUTTON3_MASK;
   }

   if ( mKeyboardMouseDevice->mKeys[MBUTTON4] )
   {
      mask |= BUTTON4_MASK;
   }

   if ( mKeyboardMouseDevice->mKeys[MBUTTON5] )
   {
      mask |= BUTTON5_MASK;
   }

   if ( mKeyboardMouseDevice->mKeys[MBUTTON6] )
   {
      mask |= BUTTON6_MASK;
   }

   if ( mKeyboardMouseDevice->mKeys[MBUTTON7] )
   {
      mask |= BUTTON7_MASK;
   }

   return mask;
}

bool InputAreaCocoa::isModifier(const Keys key) const
{
   return key == KEY_ALT || key == KEY_CTRL ||
          key == KEY_SHIFT || key == KEY_COMMAND;
}

// XXX: This method implementation is pretty lame.
Keys InputAreaCocoa::vKeyToKey(const unsigned short keyChar,
                               const unsigned short keyCode,
                               const unsigned int modifiers)
{
   Keys key;

   // XXX: Where are the constants for these? Magic numbers are bad.
   switch ( keyCode )
   {
      case 76:
         key = KEY_ENTER;
         break;
      case 53:
         key = KEY_ESC;
         break;
      case 51:
         key = KEY_BACKSPACE;
         break;
      case 48:
         if ( modifiers & NSShiftKeyMask )
         {
            key = KEY_BACKTAB;
         }
         else
         {
            key = KEY_TAB;
         }
         break;
      case 36:
         key = KEY_RETURN;
         break;
      default:
         key = KEY_NONE;
         break;
   }

   if ( KEY_NONE == key )
   {
      switch ( keyChar )
      {
         case NSUpArrowFunctionKey:
            key = KEY_UP;
            break;
         case NSDownArrowFunctionKey:
            key = KEY_DOWN;
            break;
         case NSLeftArrowFunctionKey:
            key = KEY_LEFT;
            break;
         case NSRightArrowFunctionKey:
            key = KEY_RIGHT;
            break;
         case NSF1FunctionKey:
            key = KEY_F1;
            break;
         case NSF2FunctionKey:
            key = KEY_F2;
            break;
         case NSF3FunctionKey:
            key = KEY_F3;
            break;
         case NSF4FunctionKey:
            key = KEY_F4;
            break;
         case NSF5FunctionKey:
            key = KEY_F5;
            break;
         case NSF6FunctionKey:
            key = KEY_F6;
            break;
         case NSF7FunctionKey:
            key = KEY_F7;
            break;
         case NSF8FunctionKey:
            key = KEY_F8;
            break;
         case NSF9FunctionKey:
            key = KEY_F9;
            break;
         case NSF10FunctionKey:
            key = KEY_F10;
            break;
         case NSF11FunctionKey:
            key = KEY_F11;
            break;
         case NSF12FunctionKey:
            key = KEY_F12;
            break;
         case NSF13FunctionKey:
            key = KEY_F13;
            break;
         case NSF14FunctionKey:
            key = KEY_F14;
            break;
         case NSF15FunctionKey:
            key = KEY_F15;
            break;
         case NSF16FunctionKey:
            key = KEY_F16;
            break;
         case NSF17FunctionKey:
            key = KEY_F17;
            break;
         case NSF18FunctionKey:
            key = KEY_F18;
            break;
         case NSF19FunctionKey:
            key = KEY_F19;
            break;
         case NSF20FunctionKey:
            key = KEY_F20;
            break;
         case NSF21FunctionKey:
            key = KEY_F21;
            break;
         case NSF22FunctionKey:
            key = KEY_F22;
            break;
         case NSF23FunctionKey:
            key = KEY_F23;
            break;
         case NSF24FunctionKey:
            key = KEY_F24;
            break;
         case NSF25FunctionKey:
            key = KEY_F25;
            break;
         case NSF26FunctionKey:
            key = KEY_F26;
            break;
         case NSF27FunctionKey:
            key = KEY_F27;
            break;
         case NSF28FunctionKey:
            key = KEY_F28;
            break;
         case NSF29FunctionKey:
            key = KEY_F29;
            break;
         case NSF30FunctionKey:
            key = KEY_F30;
            break;
         case NSF31FunctionKey:
            key = KEY_F31;
            break;
         case NSF32FunctionKey:
            key = KEY_F32;
            break;
         case NSF33FunctionKey:
            key = KEY_F33;
            break;
         case NSF34FunctionKey:
            key = KEY_F34;
            break;
         case NSF35FunctionKey:
            key = KEY_F35;
            break;
         case NSInsertFunctionKey:
            key = KEY_INSERT;
            break;
         case NSDeleteFunctionKey:
            key = KEY_DELETE;
            break;
         case NSHomeFunctionKey:
         case NSBeginFunctionKey:
            key = KEY_HOME;
            break;
         case NSEndFunctionKey:
            key = KEY_END;
            break;
         case NSPageUpFunctionKey:
            key = KEY_PRIOR;
            break;
         case NSPageDownFunctionKey:
            key = KEY_NEXT;
            break;
         case NSPrintScreenFunctionKey:
         case NSPrintFunctionKey:
            key = KEY_PRINT;
            break;
         //case :
         //   key = KEY_NUM_LOCK;
         //   break;
         case NSScrollLockFunctionKey:
            key = KEY_SCROLL_LOCK;
            break;
         case NSPauseFunctionKey:
            key = KEY_PAUSE;
            break;
         case NSSysReqFunctionKey:
            key = KEY_SYSREQ;
            break;
         case NSHelpFunctionKey:
            key = KEY_HELP;
            break;
         case ' ':
            key = KEY_SPACE;
            break;
         case '!':
            key = KEY_EXCLAM;
            break;
         case '@':
            key = KEY_AT;
            break;
         case '"':
            key = KEY_QUOTE_DBL;
            break;
         case '\'':
            key = KEY_APOSTROPHE;
            break;
         case '#':
            key = KEY_NUMBER_SIGN;
            break;
         case '$':
            key = KEY_DOLLAR;
            break;
         case '^':
            key = KEY_ASCII_CIRCUM;
            break;
         case '%':
            key = KEY_PERCENT;
            break;
         case '*':
            key = KEY_ASTERISK;
            break;
         case '(':
            key = KEY_PAREN_LEFT;
            break;
         case ')':
            key = KEY_PAREN_RIGHT;
            break;
         case '+':
            key = KEY_PLUS;
            break;
         case ',':
            key = KEY_COMMA;
            break;
         case '-':
            key = KEY_MINUS;
            break;
         case '.':
            key = KEY_PERIOD;
            break;
         case '/':
            key = KEY_SLASH;
            break;
         case ':':
            key = KEY_COLON;
            break;
         case ';':
            key = KEY_SEMICOLON;
            break;
         case '<':
            key = KEY_LESS;
            break;
         case '>':
            key = KEY_GREATER;
            break;
         case '=':
            key = KEY_EQUAL;
            break;
         case '?':
            key = KEY_QUESTION;
            break;
         case '[':
            key = KEY_BRACKET_LEFT;
            break;
         case ']':
            key = KEY_BRACKET_RIGHT;
            break;
         case '\\':
            key = KEY_BACKSLASH;
            break;
         case '_':
            key = KEY_UNDERSCORE;
            break;
         case '{':
            key = KEY_BRACE_LEFT;
            break;
         case '}':
            key = KEY_BRACE_RIGHT;
            break;
         case '|':
            key = KEY_BAR;
            break;
         case '~':
            key = KEY_ASCII_TILDE;
            break;
         case 'a':
         case 'A':
            key = KEY_A;
            break;
         case 'b':
         case 'B':
            key = KEY_B;
            break;
         case 'c':
         case 'C':
            key = KEY_C;
            break;
         case 'd':
         case 'D':
            key = KEY_D;
            break;
         case 'e':
         case 'E':
            key = KEY_E;
            break;
         case 'f':
         case 'F':
            key = KEY_F;
            break;
         case 'g':
         case 'G':
            key = KEY_G;
            break;
         case 'h':
         case 'H':
            key = KEY_H;
            break;
         case 'i':
         case 'I':
            key = KEY_I;
            break;
         case 'j':
         case 'J':
            key = KEY_J;
            break;
         case 'k':
         case 'K':
            key = KEY_K;
            break;
         case 'l':
         case 'L':
            key = KEY_L;
            break;
         case 'm':
         case 'M':
            key = KEY_M;
            break;
         case 'n':
         case 'N':
            key = KEY_N;
            break;
         case 'o':
         case 'O':
            key = KEY_O;
            break;
         case 'p':
         case 'P':
            key = KEY_P;
            break;
         case 'q':
         case 'Q':
            key = KEY_Q;
            break;
         case 'r':
         case 'R':
            key = KEY_R;
            break;
         case 's':
         case 'S':
            key = KEY_S;
            break;
         case 't':
         case 'T':
            key = KEY_T;
            break;
         case 'u':
         case 'U':
            key = KEY_U;
            break;
         case 'v':
         case 'V':
            key = KEY_V;
            break;
         case 'w':
         case 'W':
            key = KEY_W;
            break;
         case 'x':
         case 'X':
            key = KEY_X;
            break;
         case 'y':
         case 'Y':
            key = KEY_Y;
            break;
         case 'z':
         case 'Z':
            key = KEY_Z;
            break;
         case '0':
            key = KEY_0;
            break;
         case '1':
            key = KEY_1;
            break;
         case '2':
            key = KEY_2;
            break;
         case '3':
            key = KEY_3;
            break;
         case '4':
            key = KEY_4;
            break;
         case '5':
            key = KEY_5;
            break;
         case '6':
            key = KEY_6;
            break;
         case '7':
            key = KEY_7;
            break;
         case '8':
            key = KEY_8;
            break;
         case '9':
            key = KEY_9;
            break;
         default:
            key = KEY_NONE;
            break;
      }
   }

   return key;
}

void InputAreaCocoa::storeEventKey(const EventType eventType, const Keys key)
{
   vpr::Guard<vpr::Mutex> guard(mKeyboardMouseDevice->mKeysLock);

   switch (eventType)
   {
      case KeyPressEvent:
      case MouseButtonPressEvent:
         mKeyboardMouseDevice->mRealkeys[key] = 1;
         mKeyboardMouseDevice->mKeys[key] += 1;
         break;
      case KeyReleaseEvent:
      case MouseButtonReleaseEvent:
         mKeyboardMouseDevice->mRealkeys[key] = 0;
         break;
      default:
         break;
   }
}

void InputAreaCocoa::doAddEvent(EventPtr event, const Keys key)
{
   mKeyboardMouseDevice->addEvent(event);

   // Update the mouse cursor locked state based on keyboard events.
   if ( mAllowMouseLocking )
   {
      switch ( event->type() )
      {
         case KeyPressEvent:
            if ( key == KEY_ESC )
            {
               if ( mLockState != Unlocked )
               {
                  mLockState = Unlocked;
               }
            }
            else if ( mLockState == Unlocked )
            {
               if ( key != mLockToggleKey && isModifier(key) )
               {
                  mLockState     = Lock_KeyDown;
                  mLockStoredKey = key;

                  vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_STATE_LVL)
                     << "[gadget::InputAreaCocoa] STATE switch: "
                     << "Unlocked --> Lock_KeyDown\n" << vprDEBUG_FLUSH;

                  lockMouseInternal();
               }
               else if ( key == mLockToggleKey )
               {
                  mLockState = Lock_LockKey;

                  vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_STATE_LVL)
                     << "[gadget::InputAreaCocoa] STATE switch: "
                     << "Unlocked --> Lock_LockKey\n" << vprDEBUG_FLUSH;

                  lockMouseInternal();
               }
            }
            // Switch the current locking state.
            else if ( mLockState == Lock_KeyDown && key == mLockToggleKey )
            {
               mLockState = Lock_LockKey;
               vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_STATE_LVL)
                  << "[gadget::InputAreaCocoa] STATE switch: "
                  << "Lock_KeyDown --> Lock_LockKey\n" << vprDEBUG_FLUSH;

            }
            else if ( mLockState == Lock_LockKey && key == mLockToggleKey )
            {
               mLockState = Unlocked;

               vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_STATE_LVL)
                  << "[gadget::InputAreaCocoa] STATE switch: "
                  << "Lock_LockKey --> Unlocked\n" << vprDEBUG_FLUSH;

               unlockMouseInternal();
            }
            break;
         case KeyReleaseEvent:
            if ( mLockState == Lock_KeyDown && key == mLockStoredKey )
            {
               mLockState = Unlocked;

               vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_STATE_LVL)
                  << "[gadget::InputAreaCocoa] STATE switch: "
                  << "Lock_KeyDown --> Unlocked\n" << vprDEBUG_FLUSH;

               unlockMouseInternal();
            }
            break;
         default:
            break;
      }
   }
}

void InputAreaCocoa::clipCursorArea()
{
    const NSPoint mouse_loc = [mCocoaWindow mouseLocationOutsideOfEventStream];
    NSRect bounds = [mMainView bounds];
    bounds.origin.x += 1;
    bounds.origin.y += 1;
    bounds.size.width -= 2;
    bounds.size.height -= 2;

    // Subtract one from the bounds and add one to the origin so that the rect
    // is smaller than the window so that we can acurrately detect when
    // the mouse is about to the leave the tracking area. This is to mimic
    // the functionality in X11 and Win32.
    if (! [mMainView mouse:mouse_loc inRect:bounds])
    {
        // Place cursor at the edge of the bounds.
        NSRect frame = [mCocoaWindow frame];
        NSRect screen_frame = [[mCocoaWindow screen] frame];

        // Apparently Core Graphics uses a different coordinate frame than
        // Cocoa.
        CGPoint center_pt =
           { frame.origin.x + (mouse_loc.x - 1),
             screen_frame.size.height - (frame.origin.y + (mouse_loc.y - 1))
           };
        CGWarpMouseCursorPosition(center_pt);
     }
}

}
