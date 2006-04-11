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

#include <gadget/gadgetConfig.h>

#include <sys/types.h>

#include <jccl/Config/ConfigChunk.h>
#include <gadget/Util/Debug.h>
#include <gadget/Type/EventWindow/KeyEvent.h>
#include <gadget/Type/EventWindow/MouseEvent.h>
#include <gadget/Devices/EventWindow/EventWindowWin32.h>

#ifndef GET_X_LPARAM
#  define GET_X_LPARAM(lp)   ((int)(short)LOWORD(lp))
#endif
#ifndef GET_Y_LPARAM
#  define GET_Y_LPARAM(lp)   ((int)(short)HIWORD(lp))
#endif


namespace gadget
{

bool EventWindowWin32::config(jccl::ConfigChunkPtr c)
{
   vpr::DebugOutputGuard dbg_output(gadgetDBG_INPUT_MGR, vprDBG_STATE_LVL,
                                    std::string("gadget::EventWindowWin32::config() entered\n"),
                                    std::string("gadget::EventWindowWin32::config() done\n"));

   // Call base class config function first
   if ( ! (Input::config(c) && EventWindow::config(c)) )
   {
      return false;
   }

   for ( int i = 0; i < gadget::LAST_KEY; ++i )
   {
      mRealkeys[i] = mKeys[i] = mCurKeys[i];
   }

   // Get size and position
   mWidth  = c->getProperty<int>("width");
   mHeight = c->getProperty<int>("height");

   // default to something "sane" if too small
   if ( mWidth == 0 ) mWidth = 400;
   if ( mHeight == 0 ) mHeight = 400;

   mX = c->getProperty<int>("origin", 0);
   mY = c->getProperty<int>("origin", 1);

   // Get the lock information
   mLockToggleKey    = c->getProperty<int>("lock_key");
   bool start_locked = c->getProperty<bool>("start_locked");

   if ( start_locked )
   {
      mLockState = Lock_LockKey;     // Initialize to the locked state
   }

   mMouseSensitivity = c->getProperty<float>("msens");

   // Sanity check.
   if ( 0.0f == mMouseSensitivity )
   {
      mMouseSensitivity = 0.5f;
   }

   vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_STATE_LVL)
      << "Mouse Sensititivty: " << mMouseSensitivity << std::endl
      << vprDEBUG_FLUSH;

   mSleepTimeMS = c->getProperty<int>("sleep_time");

   // Sanity check.
   if ( mSleepTimeMS == 0 )
   {
      mSleepTimeMS = 50;
   }

   // Default to owning the window
   mWeOwnTheWindow = true;

   return true;
}

void EventWindowWin32::controlLoop(void* devPtr)
{
   mControlLoopDone = false;

   // Open the window...
   // The Window has to be created in the same thread that
   // we run the message pump because all window messages
   // dispatched are dispatched to the thread that created
   // the window.  (And we want to receive the messages
   // in the spawned thread)
   this->createWindowWin32();

   // If we have initial locked, then we need to lock the system
   if ( mLockState == Lock_LockKey )     // Means that we are in the initially locked state
   {
      vprDEBUG(gadgetDBG_INPUT_MGR,vprDBG_STATE_LVL)
         << "gadget::EventWindowWin32::controlLoop: Mouse set to initial lock. Locking it now.\n"
         << vprDEBUG_FLUSH;
      this->lockMouse();                // Lock the mouse
   }

   // When there are messages, process them all.  Otherwise,
   // sleep for a while...
   while ( !mExitFlag )
   {
      this->sample();

      // user-specified sleep time.
      vpr::System::usleep(mSleepTimeMS * 1000);
   }

   // clean up, delete the window!
   ::CloseWindow(m_hWnd); // send a message to the window to close
   mControlLoopDone = true;
}

// do not call from any other thread than controlLoop()!!!!
// Peek and GetMessage retrieves messages for any window
// that belongs to the calling thread...
void EventWindowWin32::handleEvents()
{
   MSG msg;
   bool have_events_to_check = false;
   if ( mWeOwnTheWindow )
   {
      // block until message received.
      int retval = ::GetMessage(&msg, m_hWnd, 0, 0);
      vprASSERT(retval != -1 && "invalid m_hWnd window handle or invalid lpMsg pointer");
      if ( retval == -1 ) return; // for opt mode...
      have_events_to_check = true;
   }
   // check for event, if none, then exit.
   else if ( ::PeekMessage(&msg, m_hWnd, 0, 0, PM_REMOVE) )
   {
      have_events_to_check = true;
   }
   else
   {
      have_events_to_check = false;
      return; // don't wait on the lock since there is nothing
              // afterwards that will be called when this is false.
   }

// GUARD mKeys for duration of loop
// Doing it here gives makes sure that we process all events and don't get only part of them for an update
// In order to copy data over to the mCurKeys array
// Lock access to the mKeys array for the duration of this function
vpr::Guard<vpr::Mutex> guard(mKeysLock);

   while ( have_events_to_check )
   {
      // Since we have no accelerators, no need to call
      // TranslateAccelerator here.
      ::TranslateMessage(&msg);

      // do our own special handling of kb/mouse...
      this->updKeys(msg);

      // send the message to the registered event handler
      ::DispatchMessage(&msg);

      // see if there is more messages immediately waiting
      // (don't block), process them all at once...
      int retval = ::PeekMessage(&msg, m_hWnd, 0, 0, PM_REMOVE);
      if ( retval != 0 ) // messages != 0, nomessages == 0
      {
         have_events_to_check = true;
      }
      else
      {
         have_events_to_check = false;
      }

      // user-specified sleep time.
      //vpr::System::usleep( mSleepTimeMS * 1000 );
   }
}

// Create a win32 window and start a thread
// processing it's messages
int EventWindowWin32::startSampling()
{
   if ( mThread != NULL )
   {
      vprDEBUG(vprDBG_ERROR,vprDBG_CRITICAL_LVL)
         << clrOutNORM(clrRED,"ERROR:")
         << "gadget::EventWindowWin32: startSampling called, when already sampling.\n"
         << vprDEBUG_FLUSH;
      vprASSERT(mThread == NULL);
      return 0;
   }

   vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_CONFIG_LVL)
      << "gadget::EventWindowWin32::startSampling() : ready to go.."
      << std::endl << vprDEBUG_FLUSH;

   // Create a new thread to handle the control
   vpr::ThreadMemberFunctor<EventWindowWin32>* memberFunctor =
      new vpr::ThreadMemberFunctor<EventWindowWin32>(this,
                                                     &EventWindowWin32::controlLoop,
                                                     (void*) this);

   mThread = new vpr::Thread(memberFunctor);

   // return success value...
   if ( ! mThread->valid() )
   {
      return 0; // fail
   }

   return 1; // success
}

int EventWindowWin32::onlyModifier(int mod)
{
   switch ( mod )
   {
      case gadget::KEY_NONE:
         return(!mCurKeys[gadget::KEY_SHIFT] && !mCurKeys[gadget::KEY_CTRL] && !mCurKeys[gadget::KEY_ALT]);
      case gadget::KEY_SHIFT:
         return(mCurKeys[gadget::KEY_SHIFT] && !mCurKeys[gadget::KEY_CTRL] && !mCurKeys[gadget::KEY_ALT]);
      case gadget::KEY_CTRL:
         return(!mCurKeys[gadget::KEY_SHIFT] && mCurKeys[gadget::KEY_CTRL] && !mCurKeys[gadget::KEY_ALT]);
      case gadget::KEY_ALT:
         return(!mCurKeys[gadget::KEY_SHIFT] && !mCurKeys[gadget::KEY_CTRL] && mCurKeys[gadget::KEY_ALT]);
      default:
         vprASSERT(false);
         return 0;
   }
}

void EventWindowWin32::updateData()
{
   vpr::Guard<vpr::Mutex> guard(mKeysLock);     // Lock access to the mKeys array

   // Scale mouse values based on sensitivity
   mKeys[gadget::MOUSE_POSX] = int(float(mKeys[gadget::MOUSE_POSX]) * mMouseSensitivity);
   mKeys[gadget::MOUSE_NEGX] = int(float(mKeys[gadget::MOUSE_NEGX]) * mMouseSensitivity);
   mKeys[gadget::MOUSE_POSY] = int(float(mKeys[gadget::MOUSE_POSY]) * mMouseSensitivity);
   mKeys[gadget::MOUSE_NEGY] = int(float(mKeys[gadget::MOUSE_NEGY]) * mMouseSensitivity);

   // Copy over values
   for ( unsigned int i = 0; i < gadget::LAST_KEY; ++i )
   {
      mCurKeys[i] = mKeys[i];
   }

   // Re-initialize the mKeys based on current key state in realKeys
   // Set the initial state of the mKey key counts based on the current state of the system
   // this is to ensure that if a key is still down, we get at least one event for it
   for ( unsigned int j = 0; j < gadget::LAST_KEY; ++j )
   {
      mKeys[j] = mRealkeys[j];
   }

   updateEventQueue();
}

// UpdKeys: translates windows message into key updates
// The WNDPROC uses its USERDATA pointer to the keyboard
// to forward on messages to be handled from in the keyboard object.
void EventWindowWin32::updKeys(const MSG& message)
{
   vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_HVERB_LVL)
      << mInstName << ": KeyWin32::updKeys: Processing keys.\n"
      << vprDEBUG_FLUSH;

   gadget::Keys key;

   switch ( message.message )
   {
      case WM_ACTIVATE:
         // if was activated, and not minimized
         if ( (WA_ACTIVE == LOWORD(message.wParam) ||
               WA_CLICKACTIVE == LOWORD(message.wParam)) &&
              0 == HIWORD(message.wParam) )
         {
            // and was previously locked...
            if ( Unlocked != mLockState )
            {
               this->lockMouse();
            }
         }
         // if was deactivated and minimized
         // otherwise let CAPTURECHANGED handle the lose capture case
         else if ( WA_INACTIVE == LOWORD(message.wParam) &&
                   0 != HIWORD(message.wParam) )
         {
            // and was locked...
            if ( Unlocked != mLockState )
            {
               // we will leave mLockState in the locked state so ACTIVATE puts it back.
               this->unlockMouse();
            }
         }
         break;

         // sent to the window that is losing the mouse capture
      case WM_CAPTURECHANGED:
         // if locked, unlock it
         if ( Unlocked != mLockState )
         {
            // we will leave mLockState in the locked state so ACTIVATE puts it back.
            this->unlockMouse();
         }
         break;

         // keys and buttons
         // When we hit a key (or button), then set the mKey and mRealkey
         // When release, only set real so that we don't lose a press of the
         // button then when the updateData happens, the framekeys will be set to
         // non-pressed from real_keys

         // press
      case WM_SYSKEYDOWN:  //Need WM_SYSKEYDOWN to capture ALT key
      case WM_KEYDOWN:
         {
            // collect data about the keypress.
            key = this->VKKeyToKey(message.wParam);

            addKeyEvent(key, gadget::KeyPressEvent, message);

            // get the repeat count in case the key was pressed
            // multiple times since last we got this message
            int repeat_count = message.lParam & 0x0000ffff;

            // check if the key was down already
            // this indicates key repeat, which we [may] want to ignore.
            bool was_down = (message.lParam & 0x40000000) == 0x40000000;
            if ( was_down )
            {
               break;
            }

            // process the keypress.
            mKeys[key] += repeat_count;
            mRealkeys[key] += 1;

            // Check for Escape from bad state
            // this provides a sort of failsafe to
            // get out of the locked state...
            // @todo this is sort of hard coded, do we want it this way?
            if ( key == gadget::KEY_ESC )
            {
               if ( mLockState != Unlocked )
               {
                  mLockState = Unlocked;
                  this->unlockMouse();
               }
            }
            else if ( mLockState == Unlocked )
            {
               if ( (key != mLockToggleKey) &&
                    ((gadget::KEY_ALT == key) || (gadget::KEY_CTRL == key) || (gadget::KEY_SHIFT == key)) )
               {
                  mLockState = Lock_KeyDown;       // Switch state
                  mLockStoredKey = key;         // Store the VJ key that is down
                  this->lockMouse();
               }
               else if ( key == mLockToggleKey )
               {
                  mLockState = Lock_LockKey;
                  this->lockMouse();
               }
            }
            // Just switch the current locking state
            else if ( (mLockState == Lock_KeyDown) && (key == mLockToggleKey) )
            {
               mLockState = Lock_LockKey;
            }
            else if ( (mLockState == Lock_LockKey) && (key == mLockToggleKey) )
            {
               mLockState = Unlocked;
               this->unlockMouse();
            }
         }
         break;

         // release
      case WM_SYSKEYUP:  //Need WM_SYSKEYUP to capture ALT key
      case WM_KEYUP:
         key = VKKeyToKey(message.wParam);
         mRealkeys[key] = 0;
         vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_HVERB_LVL)
            << mInstName << ": WM_KEYUP: " << key << std::endl
            << vprDEBUG_FLUSH;

         addKeyEvent(key, gadget::KeyReleaseEvent, message);

         // -- Update lock state -- //
         // lock_keyDown[key==storedKey]/unlockMouse -> unlocked
         if ( (mLockState == Lock_KeyDown) && (key == mLockStoredKey) )
         {
            mLockState = Unlocked;
            this->unlockMouse();
         }
         break;

         // press...
      case WM_LBUTTONDOWN:
         mRealkeys[gadget::MBUTTON1] = 1;
         mKeys[gadget::MBUTTON1] += 1;
         addMouseButtonEvent(gadget::MBUTTON1, gadget::MouseButtonPressEvent,
                             message);
         vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_HVERB_LVL)
            << "LeftButtonDown\n" << vprDEBUG_FLUSH;
         break;
      case WM_MBUTTONDOWN:
         mRealkeys[gadget::MBUTTON2] = 1;
         mKeys[gadget::MBUTTON2] += 1;
         addMouseButtonEvent(gadget::MBUTTON2, gadget::MouseButtonPressEvent,
                             message);
         vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_HVERB_LVL)
            << "MiddleButtonDown\n" << vprDEBUG_FLUSH;
         break;
      case WM_RBUTTONDOWN:
         mRealkeys[gadget::MBUTTON3] = 1;
         mKeys[gadget::MBUTTON3] += 1;
         addMouseButtonEvent(gadget::MBUTTON3, gadget::MouseButtonPressEvent,
                             message);
         vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_HVERB_LVL)
            << "RightButtonDown\n" << vprDEBUG_FLUSH;
         break;

         // release...
      case WM_LBUTTONUP:
         mRealkeys[gadget::MBUTTON1] = 0;
         addMouseButtonEvent(gadget::MBUTTON1,
                             gadget::MouseButtonReleaseEvent, message);
         vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_HVERB_LVL)
            << "LeftButtonUp\n" << vprDEBUG_FLUSH;
         break;
      case WM_MBUTTONUP:
         mRealkeys[gadget::MBUTTON2] = 0;
         addMouseButtonEvent(gadget::MBUTTON2,
                             gadget::MouseButtonReleaseEvent, message);
         vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_HVERB_LVL)
            << "MiddleButtonUp\n" << vprDEBUG_FLUSH;
         break;
      case WM_RBUTTONUP:
         mRealkeys[gadget::MBUTTON3] = 0;
         addMouseButtonEvent(gadget::MBUTTON3,
                             gadget::MouseButtonReleaseEvent, message);
         vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_HVERB_LVL)
            << "RightButtonUp\n" << vprDEBUG_FLUSH;
         break;

         // mouse movement
      case WM_MOUSEMOVE:
         {
            addMouseMoveEvent(message);

            int win_center_x(mWidth / 2), win_center_y(mHeight / 2);

            int cur_x, cur_y, dx, dy;

            // Determine how far the mouse pointer moved since the last event.
            // x & y are relative to the x window
            cur_x = GET_X_LPARAM(message.lParam);
            cur_y = GET_Y_LPARAM(message.lParam);

            vprDEBUG(vprDBG_ALL,vprDBG_HVERB_LVL) << "MotionNotify: x:"
               << std::setw(6) << cur_x
               << "  y:" << std::setw(6) << cur_y << std::endl
               << vprDEBUG_FLUSH;

            if ( mLockState == Unlocked )
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
               mPrevX = win_center_x;    // Must do this so if state changes, we have accurate dx,dy next time
               mPrevY = win_center_y;

               // Warp back to center, IF we are not there already
               // This prevents us from sending an event based on our XWarp event
               if ( (dx != 0) || (dy != 0) )
               {
                  vprDEBUG(vprDBG_ALL,vprDBG_HVERB_LVL) << "CORRECTING: x:"
                     << std::setw(6) << dx << "  y:"
                     << std::setw(6) << dy
                     << std::endl << vprDEBUG_FLUSH;

                  // convert windows coords to screen coords.
                  POINT pt;
                  pt.x = win_center_x;
                  pt.y = win_center_y;
                  ::ClientToScreen(m_hWnd, &pt);
                  ::SetCursorPos(pt.x, pt.y);
               }
            }

            // Update mKeys based on key pressed and store in the key array
            if ( dx > 0 )
            {
               mKeys[gadget::MOUSE_POSX] += dx;     // Positive movement in the x direction.
            }
            else
            {
               mKeys[gadget::MOUSE_NEGX] += -dx;    // Negative movement in the x direction.
            }

            if ( dy > 0 )
            {
               mKeys[gadget::MOUSE_POSY] += dy;     // Positive movement in the y direction.
            }
            else
            {
               mKeys[gadget::MOUSE_NEGY] += -dy;    // Negative movement in the y direction.
            }
         }
         break;
   } // end of switch...

   // Let any other event watchers process their events
   this->processEvent(message.message, message.wParam, message.lParam);
}

int EventWindowWin32::stopSampling()
{
   if ( mThread != NULL )
   {
      mExitFlag = true;
      ::PostMessage( m_hWnd, WM_USER, 0, 0 );// send a dummy message to the window to close
      // 1000000 usec timeout before we give up and kill the thread...
      for ( int x = 0; x < 100000 && !mControlLoopDone; ++x )
      {
         // give the window thread a chance before we delete...
         vpr::System::usleep( 10 );
      }
      delete mThread;
      mThread = NULL;
      //std::cout << "Stopping event window.." << std::endl;
   }
   return 1;
}

gadget::Keys EventWindowWin32::VKKeyToKey(int vkKey)
{
   switch ( vkKey )
   {
      case VK_UP      : return gadget::KEY_UP;
      case VK_DOWN    : return gadget::KEY_DOWN;
      case VK_LEFT    : return gadget::KEY_LEFT;
      case VK_RIGHT   : return gadget::KEY_RIGHT;
      case VK_CONTROL  : return gadget::KEY_CTRL;
      case VK_SHIFT   : return gadget::KEY_SHIFT;
      case VK_MENU     : return gadget::KEY_ALT;
      case /*VK_1*/0x31  : return gadget::KEY_1;
      case VK_NUMPAD1   : return gadget::KEY_1;
      case /*VK_2*/0x32  : return gadget::KEY_2;
      case VK_NUMPAD2   : return gadget::KEY_2;
      case /*VK_3*/0x33 : return gadget::KEY_3;
      case VK_NUMPAD3   : return gadget::KEY_3;
      case /*VK_4*/0x34  : return gadget::KEY_4;
      case VK_NUMPAD4   : return gadget::KEY_4;
      case /*VK_5*/0x35 : return gadget::KEY_5;
      case VK_NUMPAD5   : return gadget::KEY_5;
      case /*VK_6*/0x36  : return gadget::KEY_6;
      case VK_NUMPAD6   : return gadget::KEY_6;
      case /*VK_7*/0x37  : return gadget::KEY_7;
      case VK_NUMPAD7   : return gadget::KEY_7;
      case /*VK_8*/0x38  : return gadget::KEY_8;
      case VK_NUMPAD8   : return gadget::KEY_8;
      case /*VK_9*/0x39  : return gadget::KEY_9;
      case VK_NUMPAD9   : return gadget::KEY_9;
      case /*VK_0*/0x30  : return gadget::KEY_0;
      case VK_NUMPAD0   : return gadget::KEY_0;
      case /*VK_A*/0x41   : return gadget::KEY_A;
      case /*VK_B*/0x42   : return gadget::KEY_B;
      case /*VK_C*/0x43   : return gadget::KEY_C;
      case /*VK_D*/0x44   : return gadget::KEY_D;
      case /*VK_E*/0x45   : return gadget::KEY_E;
      case /*VK_F*/0x46   : return gadget::KEY_F;
      case /*VK_G*/0x47   : return gadget::KEY_G;
      case /*VK_H*/0x48   : return gadget::KEY_H;
      case /*VK_I*/0x49   : return gadget::KEY_I;
      case /*VK_J*/0x4a   : return gadget::KEY_J;
      case /*VK_K*/0x4b   : return gadget::KEY_K;
      case /*VK_L*/0x4c   : return gadget::KEY_L;
      case /*VK_M*/0x4d   : return gadget::KEY_M;
      case /*VK_N*/0x4e   : return gadget::KEY_N;
      case /*VK_O*/0x4f   : return gadget::KEY_O;
      case /*VK_P*/0x50   : return gadget::KEY_P;
      case /*VK_Q*/0x51   : return gadget::KEY_Q;
      case /*VK_R*/0x52   : return gadget::KEY_R;
      case /*VK_S*/0x53   : return gadget::KEY_S;
      case /*VK_T*/0x54   : return gadget::KEY_T;
      case /*VK_U*/0x55   : return gadget::KEY_U;
      case /*VK_V*/0x56   : return gadget::KEY_V;
      case /*VK_W*/0x57   : return gadget::KEY_W;
      case /*VK_X*/0x58   : return gadget::KEY_X;
      case /*VK_Y*/0x59   : return gadget::KEY_Y;
      case /*VK_Z*/0x5a   : return gadget::KEY_Z;

      case VK_ESCAPE  : return gadget::KEY_ESC;
      case VK_TAB     : return gadget::KEY_TAB;
//      case VK_BACKTAB : return gadget::KEY_BACKTAB;
      case VK_BACK    : return gadget::KEY_BACKSPACE;
      case VK_RETURN  : return gadget::KEY_RETURN;
      case VK_INSERT  : return gadget::KEY_INSERT;
      case VK_DELETE  : return gadget::KEY_DELETE;
      case VK_PAUSE   : return gadget::KEY_PAUSE;
//      case VK_???     : return gadget::KEY_SYSREQ;
      case VK_HOME    : return gadget::KEY_HOME;
      case VK_END     : return gadget::KEY_END;
      case VK_PRIOR   : return gadget::KEY_PRIOR;
      case VK_NEXT    : return gadget::KEY_NEXT;
      case VK_CAPITAL : return gadget::KEY_CAPS_LOCK;
      case VK_NUMLOCK : return gadget::KEY_NUM_LOCK;
      case VK_SCROLL  : return gadget::KEY_SCROLL_LOCK;

      case VK_F1  : return gadget::KEY_F1;
      case VK_F2  : return gadget::KEY_F2;
      case VK_F3  : return gadget::KEY_F3;
      case VK_F4  : return gadget::KEY_F4;
      case VK_F5  : return gadget::KEY_F5;
      case VK_F6  : return gadget::KEY_F6;
      case VK_F7  : return gadget::KEY_F7;
      case VK_F8  : return gadget::KEY_F8;
      case VK_F9  : return gadget::KEY_F9;
      case VK_F10 : return gadget::KEY_F10;
      case VK_F11 : return gadget::KEY_F11;
      case VK_F12 : return gadget::KEY_F12;
      case VK_F13 : return gadget::KEY_F13;
      case VK_F14 : return gadget::KEY_F14;
      case VK_F15 : return gadget::KEY_F15;
      case VK_F16 : return gadget::KEY_F16;
      case VK_F17 : return gadget::KEY_F17;
      case VK_F18 : return gadget::KEY_F18;
      case VK_F19 : return gadget::KEY_F19;
      case VK_F20 : return gadget::KEY_F20;
      case VK_F21 : return gadget::KEY_F21;
      case VK_F22 : return gadget::KEY_F22;
      case VK_F23 : return gadget::KEY_F23;
      case VK_F24 : return gadget::KEY_F24;

      case VK_HELP  : return gadget::KEY_HELP;
      case VK_SPACE : return gadget::KEY_SPACE;

      default: return gadget::KEY_UNKNOWN;
   }

}

/*****************************************************************/
/*****************************************************************/
/***********************WINDOW STUFF****************************/
/*****************************************************************/
/*****************************************************************/

/****************************************************************************
 *                                                        *
 *  FUNCTION   : MenuWndProc (hWnd, message, wParam, lParam)            *
 *                                                        *
 *  PURPOSE   : Window function for the main app. window. Processes all the*
 *            menu selections and oter messages.                   *
 *                                                        *
 ****************************************************************************/
LONG APIENTRY MenuWndProc(HWND hWnd, UINT message, UINT wParam, LONG lParam)
{
   switch ( message )
   {
      case WM_SYSCOMMAND:
         return DefWindowProc(hWnd, message, wParam, lParam);

//      case WM_COMMAND:
//         break;

      case WM_SIZE:
         if ( lParam )
         {
            InvalidateRect(hWnd, NULL, TRUE);
         }
         break;

      case WM_DESTROY:
         PostQuitMessage(0);
         break;

      default:
         return DefWindowProc(hWnd, message, wParam, lParam);
   }
   return 0;
}

void EventWindowWin32::createWindowWin32()
{
   int root_height;

   InitCommonControls();

   //m_hInst = g_hInst;
   m_hInst = GetModuleHandle(NULL);   // Just try to get the application's handle
   MenuInit(m_hInst);

   root_height = GetSystemMetrics(SM_CYSCREEN);

   /* Create the app. window */
   m_hWnd = CreateWindow("Gadgeteer Event Window", mInstName.c_str(),
                         WS_OVERLAPPEDWINDOW, mX,
                         root_height - mY - mHeight, mWidth, mHeight,
                         (HWND) NULL, NULL, m_hInst, (LPSTR) NULL);
   ShowWindow(m_hWnd,SW_SHOW);
   UpdateWindow(m_hWnd);

   // Attach a pointer to the device for use from the WNDPROC
   SetWindowLong(m_hWnd, GWL_USERDATA, (LPARAM)this);

   DWORD dwErr = GetLastError();
   if ( !m_hWnd )
   {
      return;
   }

} /*CreateWindow*/

BOOL EventWindowWin32::MenuInit(HINSTANCE hInstance)
{
   HANDLE   hMemory;
   PWNDCLASS pWndClass;
   BOOL     bSuccess;

   /* Initialize the menu window class */
   hMemory   = LocalAlloc(LPTR, sizeof(WNDCLASS));
   if ( !hMemory )
   {
      MessageBox(NULL, ("<MenuInit> Not enough memory."), NULL, MB_OK | MB_ICONHAND);
      return(FALSE);
   }

   pWndClass = (PWNDCLASS) LocalLock(hMemory);

   pWndClass->style         = 0;
   pWndClass->lpfnWndProc   = (WNDPROC) MenuWndProc;
   pWndClass->hInstance     = hInstance;
//   pyrate_x = (HICON)LoadIcon(hInstance, _T("prof"));
   pWndClass->hIcon         = (HICON) LoadIcon(hInstance, TEXT("Juggler"));
   pWndClass->hCursor       = (HCURSOR) LoadCursor(NULL, IDC_ARROW);
   pWndClass->hbrBackground = (HBRUSH) GetStockObject(WHITE_BRUSH);
   pWndClass->lpszMenuName  = ("MenuMenu"),
   pWndClass->lpszClassName = ("Gadgeteer Event Window");

   bSuccess = RegisterClass(pWndClass);
   LocalUnlock(hMemory);
   LocalFree(hMemory);

   return bSuccess;
}


// process the current window events
// Called repeatedly by
// - the controlLoop when "we own the window",
// - the GlWindow if we "dont own the window"
int EventWindowWin32::sample()
{
   this->handleEvents();
   return 1;
}

std::string EventWindowWin32::getChunkType()
{
   return std::string("EventWindow");
}

// returns the number of times the key was pressed during the
// last frame, so you can put this in an if to check if was
// pressed at all, or if you are doing processing based on this
// catch the actual number..
int EventWindowWin32::isKeyPressed(int Key)
{
   return mCurKeys[Key];
}

void EventWindowWin32::lockMouse()
{
   // Center the mouse
   int win_center_x(mWidth / 2), win_center_y(mHeight / 2);

   // convert windows coords to screen coords.
   ::POINT pt;
   pt.x = win_center_x;
   pt.y = win_center_y;
   ::ClientToScreen(m_hWnd, &pt);
   ::SetCursorPos(pt.x, pt.y);

   // capture the mouse for this window...
   HWND previous_capture = ::SetCapture(m_hWnd);
}

void EventWindowWin32::unlockMouse()
{
   BOOL result = ::ReleaseCapture();
}

void EventWindowWin32::addKeyEvent(const gadget::Keys& key,
                                   const gadget::EventType& type,
                                   const MSG& msg)
{
   // XXX: Missing modifier key information here...
   // XXX: Missing ASCII character value here...
   gadget::EventPtr key_event(new gadget::KeyEvent(type, key, 0, msg.time));
   addEvent(key_event);
}

void EventWindowWin32::addMouseButtonEvent(const gadget::Keys& button,
                                           const gadget::EventType& type,
                                           const MSG& msg)
{
   // XXX: Missing keyboard modifier information here...
   gadget::EventPtr mouse_event(new gadget::MouseEvent(type, button,
                                                       GET_X_LPARAM(msg.lParam),
                                                       GET_Y_LPARAM(msg.lParam),
                                                       msg.pt.x, msg.pt.y, 0,
                                                       msg.time));
   addEvent(mouse_event);
}

void EventWindowWin32::addMouseMoveEvent(const MSG& msg)
{
   gadget::EventPtr mouse_event(new gadget::MouseEvent(gadget::MouseMoveEvent,
                                                       gadget::NO_MBUTTON,
                                                       GET_X_LPARAM(msg.lParam),
                                                       GET_Y_LPARAM(msg.lParam),
                                                       msg.pt.x, msg.pt.y, 0,
                                                       msg.time));
   addEvent(mouse_event);
}

} // End of gadget namespace
