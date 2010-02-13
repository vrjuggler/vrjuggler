/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2010 by Iowa State University
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
 *************** <auto-copyright.pl END do not edit this line> ***************/

#include <gadget/gadgetConfig.h>

#include <iomanip>
#include <X11/Xutil.h>
#include <X11/keysym.h>

#include <gadget/Util/Debug.h>
#include <gadget/Type/KeyboardMouse/KeyEvent.h>
#include <gadget/Type/KeyboardMouse/MouseEvent.h>
#include <gadget/Devices/KeyboardMouseDevice/InputAreaXWin.h>


namespace gadget
{

InputAreaXWin::InputAreaXWin()
   : mXDisplay(NULL)
   , mWidth(0)
   , mHeight(0)
   , mEmptyCursorSet(false)
   , mPrevX(0)
   , mPrevY(0)
{
   /* Do nothing. */ ;
}

InputAreaXWin::~InputAreaXWin()
{
   /* Do nothing. */ ;
}

void InputAreaXWin::lockMouseInternal()
{
   lockMouse(NULL);
}

void InputAreaXWin::unlockMouseInternal()
{
   unlockMouse(NULL);
}

void InputAreaXWin::handleEvents()
{
   ::XEvent event;
   bool have_events_to_check(true);  // Used by the while loop below

   // XXX: Need to add to mask to get more events for subclasses
   const long event_mask = (KeyPressMask | KeyReleaseMask | ButtonPressMask |
                            ButtonReleaseMask | ButtonMotionMask |
                            PointerMotionMask | StructureNotifyMask);

   // Check for events
   // If we own,
   //  - Wait until we actually have some events -- THIS BLOCKS for an event
   // else
   //  - check for event, if none, then we exit

   if( mBlocking )
   {
      XWindowEvent(mXDisplay, mXWindow, event_mask, &event);
      have_events_to_check = true;
   }
   else
   {
      have_events_to_check = XCheckWindowEvent(mXDisplay, mXWindow, event_mask,
                                               &event);
   }

// GUARD mKeys for duration of loop.
// Doing it here gives makes sure that we process all events and don't get only
// part of them for an update.  In order to copy data over to the mCurKeys
// array.
vpr::Guard<vpr::Mutex> guard(mKeyboardMouseDevice->mKeysLock);      // Lock access to the mKeys array for the duration of this function

   // Loop while the event queue contains events for mXWindow that are part
   // of the given event mask.
   while(have_events_to_check)
   {

      handleEvent(event);
      have_events_to_check = XCheckWindowEvent(mXDisplay, mXWindow, event_mask,
                                               &event);
   }
}

void InputAreaXWin::handleEvent(::XEvent& event)
{
   KeySym key;
   gadget::Keys vj_key;          // The key in vj space

      switch (event.type)
      {
      // A KeyPress event occurred.  Flag the key that was pressed (as a
      // Gadgeteer key) as being pressed and grab the keyboard.
      case KeyPress:
         // Convert the pressed key from the event to a Gadgeteer key.
         // And store in the array of key presses
         key = XLookupKeysym((XKeyEvent*)&event,0);
         vj_key = xKeyToKey(key);
         mKeyboardMouseDevice->mRealkeys[vj_key] = 1;
         mKeyboardMouseDevice->mKeys[vj_key] += 1;

         vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_HVERB_LVL)
            << "vj_key press value: " << (unsigned int) vj_key << std::endl
            << vprDEBUG_FLUSH;
         addKeyEvent(vj_key, gadget::KeyPressEvent, &event.xkey);

         if ( mAllowMouseLocking )
         {
            // -- Update lock state -- //
            // Any[key == ESC]/unlock(ifneeded) -> Unlocked
            // Unlocked[key!=lockKey]/lockMouse -> lock_keydown
            // Unlocked[key==lockKey]/lockMouse -> lock_keylock
            // lock_keydown[key==lockKey] -> lock_keylock
            // lock_keylock[key==lockKey] -> Unlocked

            // Check for Escape from bad state
            if ( vj_key == gadget::KEY_ESC )
            {
               vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_STATE_LVL)
                  << "[gadget::InputAreaWin]: Trying to ESCAPE from "
                  << "current state.\n" << vprDEBUG_FLUSH;

               if ( mLockState != Unlocked )
               {
                  vprDEBUG_NEXT(gadgetDBG_INPUT_MGR, vprDBG_STATE_LVL)
                     << "[gadget::InputAreaXWin]: STATE switch: "
                     << "<ESCAPE> --> Unlocked\n" << vprDEBUG_FLUSH;
                  mLockState = Unlocked;
               }
               else
               {
                  vprDEBUG_NEXT(gadgetDBG_INPUT_MGR, vprDBG_STATE_LVL)
                     << "[gadget::InputAreaXWin]: Already unlocked.  "
                     << "Cannot ESCAPE." << vprDEBUG_FLUSH;
               }
            }
            else if ( mLockState == Unlocked )
            {
               if ( vj_key != mLockToggleKey &&
                    (gadget::KEY_ALT == vj_key || gadget::KEY_CTRL == vj_key ||
                     gadget::KEY_SHIFT == vj_key) )
               {
                  mLockState = Lock_KeyDown;  // Switch state
                  mLockStoredKey = vj_key;    // Store the Gadgeteer key that is down
                  vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_STATE_LVL)
                     << "[gadget::InputAreaXWin]: STATE switch: "
                     << "Unlocked --> Lock_KeyDown\n" << vprDEBUG_FLUSH;
                  lockMouse(&event);
               }
               else if ( vj_key == mLockToggleKey )
               {
                  mLockState = Lock_LockKey;
                  vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_STATE_LVL)
                     << "[gadget::InputAreaXWin]: STATE switch: "
                     << "Unlocked --> Lock_LockKey\n" << vprDEBUG_FLUSH;
                  lockMouse(&event);
               }
            }
            // Just switch the current locking state
            else if ( mLockState == Lock_KeyDown && vj_key == mLockToggleKey )
            {
               mLockState = Lock_LockKey;
               vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_STATE_LVL)
                  << "[gadget::InputAreaXWin]: STATE switch: "
                  << "Lock_KeyDown --> Lock_LockKey\n" << vprDEBUG_FLUSH;
            }
            else if ( mLockState == Lock_LockKey && vj_key == mLockToggleKey )
            {
               mLockState = Unlocked;
               vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_STATE_LVL)
                  << "[gadget::InputAreaXWin]: STATE switch: "
                  << "Lock_LockKey --> Unlocked\n" << vprDEBUG_FLUSH;
               unlockMouse(&event);
            }
         }

         vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_HVERB_LVL)    //vprDBG_HVERB_LVL
            << "KeyPress: " << std::hex << key
            << " state: " << ((XKeyEvent*)&event)->state
            << " ==> " << (unsigned int) xKeyToKey(key) << std::dec
            << std::endl << vprDEBUG_FLUSH;

         break;

      // A KeyRelease event occurred.  Flag the key that was released (as a
      // Gadgeteer key) as being not pressed and ungrab the keyboard.
      case KeyRelease:
         // Convert the released key from the event to a Gadgeteer key.
         key = XLookupKeysym((XKeyEvent*)&event,0);
         vj_key = xKeyToKey(key);
         mKeyboardMouseDevice->mRealkeys[vj_key] = 0;

         addKeyEvent(vj_key, gadget::KeyReleaseEvent, &event.xkey);

         // -- Update lock state -- //
         // lock_keyDown[key==storedKey]/unlockMouse -> unlocked
         if ( mAllowMouseLocking && mLockState == Lock_KeyDown &&
              vj_key == mLockStoredKey )
         {
            mLockState = Unlocked;
            vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_STATE_LVL)
               << "[gadget::InputAreaXWin]: STATE switch: "
               << "Lock_KeyDown --> Unlocked\n" << vprDEBUG_FLUSH;
            unlockMouse(&event);
         }

         vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_HVERB_LVL)
            << "KeyRelease:" << std::hex << key
            << " state:" << ((XKeyEvent*)&event)->state
            << " ==> " << (unsigned int) xKeyToKey(key) << std::dec
            << std::endl << vprDEBUG_FLUSH;
         break;

      // A MotionNotify event (mouse pointer movement) occurred.
      // If motion occurs within the window, determine how far the pointer
      // moved since the last time anything was read.
      case MotionNotify:
         {
            int win_center_x(mWidth/2), win_center_y(mHeight/2);

            int cur_x, cur_y, dx, dy;

            // Determine how far the mouse pointer moved since the last event.
            // event.xmotion.x & y are relative to the x window
            cur_x = event.xmotion.x;
            cur_y = event.xmotion.y;

            addMouseMoveEvent(event.xmotion);

            vprDEBUG(vprDBG_ALL, vprDBG_HVERB_LVL)
               << "MotionNotify: x:"  << std::setw(6) << cur_x << "  y:"
               << std::setw(6) << cur_y << std::endl << vprDEBUG_FLUSH;

            if(mLockState == Unlocked)
            {
               dx = cur_x - mPrevX;
               dy = cur_y - mPrevY;

               mPrevX = cur_x;
               mPrevY = cur_y;
            }
            else
            {
               dx = cur_x - win_center_x; // Base delta off of center of window
               dy = cur_y - win_center_y;
               mPrevX = win_center_x;     // Must do this so if state changes, we have accurate dx,dy next time
               mPrevY = win_center_y;

               // Warp back to center, IF we are not there already.
               // This prevents us from sending an event based on our XWarp
               // event.
               if((dx != 0) || (dy != 0))
               {
                  vprDEBUG(vprDBG_ALL,vprDBG_HVERB_LVL)
                     << "CORRECTING: x:" << std::setw(6) << dx
                     << "  y:" << std::setw(6) << dy << std::endl
                     << vprDEBUG_FLUSH;

                  XWarpPointer(mXDisplay, None, mXWindow, 0,0, 0,0,
                               win_center_x, win_center_y);
               }
            }

            // Update mKeys based on key pressed and store in the key array
            if ( dx > 0 )
            {
               mKeyboardMouseDevice->mKeys[gadget::MOUSE_POSX] += dx;      // Positive movement in the x direction.
            }
            else
            {
               mKeyboardMouseDevice->mKeys[gadget::MOUSE_NEGX] += -dx;     // Negative movement in the x direction.
            }

            if ( dy > 0 )
            {
               mKeyboardMouseDevice->mKeys[gadget::MOUSE_POSY] += dy;      // Positive movement in the y direction.
            }
            else
            {
               mKeyboardMouseDevice->mKeys[gadget::MOUSE_NEGY] += -dy;     // Negative movement in the y direction.
            }
         }

         break;

      // A mouse button was pressed.  We set that button to 1 in mKeys AND
      // in mRealkeys so that it will be remembered as being held down
      // untill a ButtonRelease event occurs.
      case ButtonPress:
      // A mouse button was released.
      case ButtonRelease:
         // At the moment, mouse scrolling is hard-coded to use the following
         // mapping:
         //
         //    X11 Button 4 --> Mouse scroll up
         //    X11 Button 5 --> Mouse scroll down
         //    X11 Button 6 --> Mouse scroll left
         //    X11 Button 7 --> Mouse scroll right
         //
         // It is not known if this can be expressed in a more flexible
         // manner.
         switch ( event.xbutton.button )
         {
            // Scrolling "buttons".
            case Button4:
            case Button5:
            case 6:
            case 7:
               // Check for handling of the deprecated scrolling behavior.
               if ( mKeyboardMouseDevice->useButtonsForScrolling() )
               {
                  handleMouseButtonEvent(event);
               }
               // Use the new scrolling behavior.
               else
               {
                  handleMouseScrollEvent(event);
               }

               break;
            // All other buttons.
            default:
               handleMouseButtonEvent(event);
               break;
         }
         break;

      // The windows size has changed
      case ConfigureNotify:
            updateOriginAndSize(event.xconfigure.width,
                                event.xconfigure.height);
         break;
      }

      // Let any other event watchers process their events
      this->processEvent(event);
}

// Called when locking states
// - Recenter the mouse
void InputAreaXWin::lockMouse(XEvent* ev)
{
   Display* display;
   Window window;

   // Fall back on default if event is NULL.
   if ( NULL != ev )
   {
      display = ((XAnyEvent*)ev)->display;
      window = ((XAnyEvent*)ev)->window;
   }
   else
   {
      display = mXDisplay;
      window = mXWindow;
   }

   vprDEBUG(gadgetDBG_INPUT_MGR,vprDBG_STATE_LVL)
      << "gadget::InputAreaXWin: LOCKING MOUSE..." << vprDEBUG_FLUSH;

   if ( mEmptyCursorSet )
   {
      XDefineCursor(display, window, mEmptyCursor);
   }

   // Center the mouse
   int win_center_x(mWidth/2), win_center_y(mHeight/2);
   XWarpPointer(display, None, window, 0,0, 0,0, win_center_x, win_center_y);

   // Grab the keyboard input so that holding down a key works even
   // if the window loses focus.  While the keyboard is grabbed,
   // keyboard and pointer events will be processed normally
   // (GrabModeAsync).
   XGrabKeyboard(display, window, True, GrabModeAsync, GrabModeAsync,
                 CurrentTime);

   // While the pointer is grabbed, we will watch for the events in
   // event_mask, and keyboard and pointer events will be processed
   // normally (GrabModeAsync).
   unsigned int event_mask;
   event_mask = ButtonPressMask | ButtonReleaseMask |
                PointerMotionMask | ButtonMotionMask;

   XGrabPointer(display, window, True, event_mask, GrabModeAsync,
                GrabModeAsync, None, None, CurrentTime);

   vprDEBUG_CONT(gadgetDBG_INPUT_MGR,vprDBG_STATE_LVL)
      << "lock finished.\n" << vprDEBUG_FLUSH;
}

// Called when locking ends
void InputAreaXWin::unlockMouse(XEvent* ev)
{
   Display* display;
   Window window;

   // Fall back on default if event is NULL.
   if ( NULL != ev )
   {
      display = ((XAnyEvent*)ev)->display;
      window = ((XAnyEvent*)ev)->window;
   }
   else
   {
      display = mXDisplay;
      window = mXWindow;
   }

   vprDEBUG(gadgetDBG_INPUT_MGR,vprDBG_STATE_LVL)
      << "gadget::InputAreaXWin: UN-LOCKING MOUSE..." << vprDEBUG_FLUSH;

   // Un-grab the keyboard now
   XUngrabKeyboard(display, CurrentTime);

   // Un-grab the pointer as well
   XUngrabPointer(display, CurrentTime);

   if ( mEmptyCursorSet )
   {
      XUndefineCursor(display, window);
   }

   vprDEBUG_CONT(gadgetDBG_INPUT_MGR,vprDBG_STATE_LVL)
      << "un-lock finished.\n" << vprDEBUG_FLUSH;
}


void InputAreaXWin::updateOriginAndSize(unsigned int width, unsigned int height)
{
   mWidth = width;
   mHeight = height;
}

const XWindowAttributes InputAreaXWin::getDisplayAttributes() const
{
   XWindowAttributes win_attrs;
   XGetWindowAttributes(mXDisplay,
                        RootWindow(mXDisplay, DefaultScreen(mXDisplay)),
                        &win_attrs);

   return win_attrs;
}

void InputAreaXWin::addKeyEvent(const gadget::Keys key,
                                const gadget::EventType type,
                                XKeyEvent* event)
{
   const int buffer_size(20);
   char buffer[buffer_size];
   KeySym key_sym;
   XComposeStatus cs;

   // Get ASCII representation of key.
   XLookupString(event, buffer, buffer_size, &key_sym, &cs);

#ifdef GADGET_DEBUG
   buffer[buffer_size] = '\0';
   vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_HVERB_LVL)
      << "InputAreaXWin::addKeyEvent(): Key string from event '"
      << buffer << "'\n" << vprDEBUG_FLUSH;
#endif

   gadget::EventPtr key_event(new gadget::KeyEvent(type, key,
                                                   getMask(event->state),
                                                   event->time, this,
                                                   buffer[0], buffer[0]));
   mKeyboardMouseDevice->addEvent(key_event);
}

void InputAreaXWin::addMouseMoveEvent(const XMotionEvent& event)
{
   const XWindowAttributes attrs = getDisplayAttributes();
   gadget::EventPtr mouse_event(
      new gadget::MouseEvent(gadget::MouseMoveEvent, gadget::NO_MBUTTON,
                             event.x, mHeight - event.y, event.x_root,
                             attrs.height - event.y_root, 0.0f, 0.0f,
                             getMask(event.state), event.time, this)
   );
   mKeyboardMouseDevice->addEvent(mouse_event);
}

void InputAreaXWin::addMouseButtonEvent(const gadget::Keys button,
                                        const gadget::EventType type,
                                        const XButtonEvent& event)
{
   const XWindowAttributes attrs = getDisplayAttributes();
   gadget::EventPtr mouse_event(
      new gadget::MouseEvent(type, button, event.x, mHeight - event.y,
                             event.x_root, attrs.height - event.y_root,
                             0.0f, 0.0f, getMask(event.state), event.time,
                             this)
   );
   mKeyboardMouseDevice->addEvent(mouse_event);
}

int InputAreaXWin::getMask(const int state)
{
   int mask(0);

   if ( state & ShiftMask )
   {
      mask |= gadget::SHIFT_MASK;
   }

   if ( state & ControlMask )
   {
      mask |= gadget::CTRL_MASK;
   }

   // At least with XFree86, Mod1 is the Alt key.
   if ( state & Mod1Mask )
   {
      mask |= gadget::ALT_MASK;
   }

/*
   if ( state & Mod2Mask )
   {
      std::cout << "Mod2 is pressed\n";
   }

   if ( state & Mod3Mask )
   {
      std::cout << "Mod3 is pressed\n";
   }

   if ( state & Mod4Mask )
   {
      std::cout << "Mod4 is pressed\n";
   }

   if ( state & Mod5Mask )
   {
      std::cout << "Mod5 is pressed\n";
   }
*/

   if ( state & Button1Mask )
   {
      mask |= gadget::BUTTON1_MASK;
   }

   if ( state & Button2Mask )
   {
      mask |= gadget::BUTTON2_MASK;
   }

   if ( state & Button3Mask )
   {
      mask |= gadget::BUTTON3_MASK;
   }

   if ( state & Button4Mask )
   {
      mask |= gadget::BUTTON4_MASK;
   }

   if ( state & Button5Mask )
   {
      mask |= gadget::BUTTON5_MASK;
   }

   return mask;
}

gadget::Keys InputAreaXWin::xKeyToKey(KeySym xKey)
{
   switch (xKey)
   {
   case XK_Up        : return gadget::KEY_UP;
   case XK_Down      : return gadget::KEY_DOWN;
   case XK_Left      : return gadget::KEY_LEFT;
   case XK_Right     : return gadget::KEY_RIGHT;
   case XK_Control_L : return gadget::KEY_CTRL;
   case XK_Control_R : return gadget::KEY_CTRL;
   case XK_Shift_L   : return gadget::KEY_SHIFT;
   case XK_Shift_R   : return gadget::KEY_SHIFT;
   case XK_Alt_L     : return gadget::KEY_ALT;
   case XK_Alt_R     : return gadget::KEY_ALT;

      // Map all number keys
      // Note we map keypad and normal keys making no distinction
   case XK_1            : return gadget::KEY_1;
   case XK_KP_1         : return gadget::KEY_1;
   case XK_KP_End       : return gadget::KEY_1;
   case XK_2            : return gadget::KEY_2;
   case XK_KP_2         : return gadget::KEY_2;
   case XK_KP_Down      : return gadget::KEY_2;
   case XK_3            : return gadget::KEY_3;
   case XK_KP_3         : return gadget::KEY_3;
   case XK_KP_Page_Down : return gadget::KEY_3;
   case XK_4            : return gadget::KEY_4;
   case XK_KP_4         : return gadget::KEY_4;
   case XK_KP_Left      : return gadget::KEY_4;
   case XK_5            : return gadget::KEY_5;
   case XK_KP_5         : return gadget::KEY_5;
   case XK_KP_Begin     : return gadget::KEY_5;
   case XK_6            : return gadget::KEY_6;
   case XK_KP_6         : return gadget::KEY_6;
   case XK_KP_Right     : return gadget::KEY_6;
   case XK_7            : return gadget::KEY_7;
   case XK_KP_7         : return gadget::KEY_7;
   case XK_KP_Home      : return gadget::KEY_7;
   case XK_8            : return gadget::KEY_8;
   case XK_KP_8         : return gadget::KEY_8;
   case XK_KP_Up        : return gadget::KEY_8;
   case XK_9            : return gadget::KEY_9;
   case XK_KP_9         : return gadget::KEY_9;
   case XK_KP_Page_Up   : return gadget::KEY_9;
   case XK_0            : return gadget::KEY_0;
   case XK_KP_0         : return gadget::KEY_0;
   case XK_KP_Insert    : return gadget::KEY_0;

   case XK_A         : return gadget::KEY_A;
   case XK_B         : return gadget::KEY_B;
   case XK_C         : return gadget::KEY_C;
   case XK_D         : return gadget::KEY_D;
   case XK_E         : return gadget::KEY_E;
   case XK_F         : return gadget::KEY_F;
   case XK_G         : return gadget::KEY_G;
   case XK_H         : return gadget::KEY_H;
   case XK_I         : return gadget::KEY_I;
   case XK_J         : return gadget::KEY_J;
   case XK_K         : return gadget::KEY_K;
   case XK_L         : return gadget::KEY_L;
   case XK_M         : return gadget::KEY_M;
   case XK_N         : return gadget::KEY_N;
   case XK_O         : return gadget::KEY_O;
   case XK_P         : return gadget::KEY_P;
   case XK_Q         : return gadget::KEY_Q;
   case XK_R         : return gadget::KEY_R;
   case XK_S         : return gadget::KEY_S;
   case XK_T         : return gadget::KEY_T;
   case XK_U         : return gadget::KEY_U;
   case XK_V         : return gadget::KEY_V;
   case XK_W         : return gadget::KEY_W;
   case XK_X         : return gadget::KEY_X;
   case XK_Y         : return gadget::KEY_Y;
   case XK_Z         : return gadget::KEY_Z;
   case XK_a         : return gadget::KEY_A;
   case XK_b         : return gadget::KEY_B;
   case XK_c         : return gadget::KEY_C;
   case XK_d         : return gadget::KEY_D;
   case XK_e         : return gadget::KEY_E;
   case XK_f         : return gadget::KEY_F;
   case XK_g         : return gadget::KEY_G;
   case XK_h         : return gadget::KEY_H;
   case XK_i         : return gadget::KEY_I;
   case XK_j         : return gadget::KEY_J;
   case XK_k         : return gadget::KEY_K;
   case XK_l         : return gadget::KEY_L;
   case XK_m         : return gadget::KEY_M;
   case XK_n         : return gadget::KEY_N;
   case XK_o         : return gadget::KEY_O;
   case XK_p         : return gadget::KEY_P;
   case XK_q         : return gadget::KEY_Q;
   case XK_r         : return gadget::KEY_R;
   case XK_s         : return gadget::KEY_S;
   case XK_t         : return gadget::KEY_T;
   case XK_u         : return gadget::KEY_U;
   case XK_v         : return gadget::KEY_V;
   case XK_w         : return gadget::KEY_W;
   case XK_x         : return gadget::KEY_X;
   case XK_y         : return gadget::KEY_Y;
   case XK_z         : return gadget::KEY_Z;

   case XK_Escape       : return gadget::KEY_ESC;
   case XK_Tab          : return gadget::KEY_TAB;
#ifdef XK_3270
   case XK_BackTab      : return gadget::KEY_BACKTAB;
#endif
   case XK_BackSpace    : return gadget::KEY_BACKSPACE;
   case XK_Return       : return gadget::KEY_RETURN;
   case XK_Insert       : return gadget::KEY_INSERT;
   case XK_Delete       : return gadget::KEY_DELETE;
   case XK_Pause        : return gadget::KEY_PAUSE;
   case XK_Print        : return gadget::KEY_PRINT;
   case XK_Sys_Req      : return gadget::KEY_SYSREQ;
   case XK_Home         : return gadget::KEY_HOME;
   case XK_End          : return gadget::KEY_END;
   case XK_Prior        : return gadget::KEY_PRIOR; // XXX: There is also XK_KP_Prior
   case XK_Next         : return gadget::KEY_NEXT;  // XXX: There is also XK_KP_Next
   case XK_Caps_Lock    : return gadget::KEY_CAPS_LOCK;
   case XK_Num_Lock     : return gadget::KEY_NUM_LOCK;
   case XK_Scroll_Lock  : return gadget::KEY_SCROLL_LOCK;

   case XK_F1  : return gadget::KEY_F1;
   case XK_F2  : return gadget::KEY_F2;
   case XK_F3  : return gadget::KEY_F3;
   case XK_F4  : return gadget::KEY_F4;
   case XK_F5  : return gadget::KEY_F5;
   case XK_F6  : return gadget::KEY_F6;
   case XK_F7  : return gadget::KEY_F7;
   case XK_F8  : return gadget::KEY_F8;
   case XK_F9  : return gadget::KEY_F9;
   case XK_F10 : return gadget::KEY_F10;
   case XK_F11 : return gadget::KEY_F11;
   case XK_F12 : return gadget::KEY_F12;
   case XK_F13 : return gadget::KEY_F13;
   case XK_F14 : return gadget::KEY_F14;
   case XK_F15 : return gadget::KEY_F15;
   case XK_F16 : return gadget::KEY_F16;
   case XK_F17 : return gadget::KEY_F17;
   case XK_F18 : return gadget::KEY_F18;
   case XK_F19 : return gadget::KEY_F19;
   case XK_F20 : return gadget::KEY_F20;
   case XK_F21 : return gadget::KEY_F21;
   case XK_F22 : return gadget::KEY_F22;
   case XK_F23 : return gadget::KEY_F23;
   case XK_F24 : return gadget::KEY_F24;
   case XK_F25 : return gadget::KEY_F25;
   case XK_F26 : return gadget::KEY_F26;
   case XK_F27 : return gadget::KEY_F27;
   case XK_F28 : return gadget::KEY_F28;
   case XK_F29 : return gadget::KEY_F29;
   case XK_F30 : return gadget::KEY_F30;
   case XK_F31 : return gadget::KEY_F31;
   case XK_F32 : return gadget::KEY_F32;
   case XK_F33 : return gadget::KEY_F33;
   case XK_F34 : return gadget::KEY_F34;
   case XK_F35 : return gadget::KEY_F35;

   case XK_Super_L : return gadget::KEY_SUPER_L;
   case XK_Super_R : return gadget::KEY_SUPER_R;
   case XK_Menu    : return gadget::KEY_MENU;
   case XK_Hyper_L : return gadget::KEY_HYPER_L;
   case XK_Hyper_R : return gadget::KEY_HYPER_R;
   case XK_Help    : return gadget::KEY_HELP;

   case XK_KP_Space     : return gadget::KEY_SPACE;
   case XK_space        : return gadget::KEY_SPACE;
//   case XK_Any          : return gadget::KEY_ANY;
   case XK_exclam       : return gadget::KEY_EXCLAM;
   case XK_quotedbl     : return gadget::KEY_QUOTE_DBL;
   case XK_numbersign   : return gadget::KEY_NUMBER_SIGN;
   case XK_dollar       : return gadget::KEY_DOLLAR;
   case XK_percent      : return gadget::KEY_PERCENT;
   case XK_ampersand    : return gadget::KEY_AMPERSAND;
   case XK_parenleft    : return gadget::KEY_PAREN_LEFT;
   case XK_parenright   : return gadget::KEY_PAREN_RIGHT;
   case XK_asterisk     : return gadget::KEY_ASTERISK;
   case XK_plus         : return gadget::KEY_PLUS;
   case XK_comma        : return gadget::KEY_COMMA;
   case XK_minus        : return gadget::KEY_MINUS;
   case XK_period       : return gadget::KEY_PERIOD;
   case XK_slash        : return gadget::KEY_SLASH;
   case XK_colon        : return gadget::KEY_COLON;
   case XK_semicolon    : return gadget::KEY_SEMICOLON;
   case XK_less         : return gadget::KEY_LESS;
   case XK_equal        : return gadget::KEY_EQUAL;
   case XK_greater      : return gadget::KEY_GREATER;
   case XK_question     : return gadget::KEY_QUESTION;
   case XK_at           : return gadget::KEY_AT;
   case XK_bracketleft  : return gadget::KEY_BRACKET_LEFT;
   case XK_backslash    : return gadget::KEY_BACKSLASH;
   case XK_bracketright : return gadget::KEY_BRACKET_RIGHT;
   case XK_asciicircum  : return gadget::KEY_ASCII_CIRCUM;
   case XK_underscore   : return gadget::KEY_UNDERSCORE;
   case XK_quoteleft    : return gadget::KEY_QUOTE_LEFT;
   case XK_braceleft    : return gadget::KEY_BRACE_LEFT;
   case XK_bar          : return gadget::KEY_BAR;
   case XK_braceright   : return gadget::KEY_BRACE_RIGHT;
   case XK_asciitilde   : return gadget::KEY_ASCII_TILDE;

   default: return gadget::KEY_UNKNOWN;
   }
}

void InputAreaXWin::handleMouseButtonEvent(const XEvent& event)
{
   vprASSERT(event.type == ButtonPress || event.type == ButtonRelease);

   gadget::Keys gadget_button;
   switch ( event.xbutton.button )
   {
      case Button1:
         gadget_button = gadget::MBUTTON1;
         break;
      case Button2:
         gadget_button = gadget::MBUTTON2;
         break;
      case Button3:
         gadget_button = gadget::MBUTTON3;
         break;
      case Button4:
         gadget_button = gadget::MBUTTON4;
         break;
      case Button5:
         gadget_button = gadget::MBUTTON5;
         break;
      case 6:
         gadget_button = gadget::MBUTTON6;
         break;
      case 7:
         gadget_button = gadget::MBUTTON7;
         break;
      case 8:
         gadget_button = gadget::MBUTTON8;
         break;
      case 9:
         gadget_button = gadget::MBUTTON9;
         break;
      default:
         vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_WARNING_LVL)
            << "Unknown X11 mouse button number " << event.xbutton.button
            << "! Please report this as a Gadgeteer bug.\n"
            << vprDEBUG_FLUSH;
         gadget_button = gadget::KEY_UNKNOWN;
         break;
   }

   if ( gadget::KEY_UNKNOWN != gadget_button )
   {
      gadget::EventType gadget_event;

      if ( ButtonPress == event.type )
      {
         gadget_event = gadget::MouseButtonPressEvent;
         mKeyboardMouseDevice->mRealkeys[gadget_button] = 1;
         mKeyboardMouseDevice->mKeys[gadget_button] += 1;
      }
      else
      {
         gadget_event = gadget::MouseButtonReleaseEvent;
         mKeyboardMouseDevice->mRealkeys[gadget_button] = 0;
      }

      addMouseButtonEvent(gadget_button, gadget_event, event.xbutton);
   }
}

void InputAreaXWin::handleMouseScrollEvent(const XEvent& event)
{
   vprASSERT(event.type == ButtonPress || event.type == ButtonRelease);

   float delta_x(0.0f);
   float delta_y(0.0f);

   gadget::Keys scroll_key;

   // At the moment, mouse scrolling is hard-coded to use the following
   // mapping:
   //
   //    X11 Button 4 --> Mouse scroll up
   //    X11 Button 5 --> Mouse scroll down
   //    X11 Button 6 --> Mouse scroll left
   //    X11 Button 7 --> Mouse scroll right
   //
   // This is different from other platforms where the scrolling is handled as
   // a first class event type and then mapped back onto configured button
   // presses. X11 does not have such an event type, and it is not known if
   // scrolling has to be mapped to these button presses.
   switch ( event.xbutton.button )
   {
      case Button4:
         scroll_key = gadget::MOUSE_SCROLL_UP;
         delta_y = 1.0f;
         break;
      case Button5:
         scroll_key = gadget::MOUSE_SCROLL_DOWN;
         delta_y = -1.0f;
         break;
      case 6:
         scroll_key = gadget::MOUSE_SCROLL_LEFT;
         delta_x = -1.0f;
         break;
      case 7:
         scroll_key = gadget::MOUSE_SCROLL_RIGHT;
         delta_x = 1.0f;
         break;
      default:
         vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_WARNING_LVL)
            << "Unknown X11 mouse button number " << event.xbutton.button
            << " used for scorlling! Please report this as a Gadgeteer bug.\n"
            << vprDEBUG_FLUSH;
         scroll_key = gadget::KEY_UNKNOWN;
         break;
   }

   if ( gadget::KEY_UNKNOWN != scroll_key )
   {
      if ( ButtonPress == event.type )
      {
         mKeyboardMouseDevice->mRealkeys[scroll_key] = 1;
         mKeyboardMouseDevice->mKeys[scroll_key] += 1;
      }
      else
      {
         mKeyboardMouseDevice->mRealkeys[scroll_key] = 0;
      }

      const XWindowAttributes attrs(getDisplayAttributes());
      gadget::EventPtr mouse_event(
         new gadget::MouseEvent(gadget::MouseScrollEvent, gadget::NO_MBUTTON,
                                event.xbutton.x, mHeight - event.xbutton.y,
                                event.xbutton.x_root,
                                attrs.height - event.xbutton.y_root,
                                delta_x, delta_y,
                                getMask(event.xbutton.state),
                                event.xbutton.time, this)
      );
      mKeyboardMouseDevice->addEvent(mouse_event);
   }
}

}
