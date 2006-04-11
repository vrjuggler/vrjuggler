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
#include <gadget/Devices/Keyboard/KeyboardWin32.h>

#ifndef GET_X_LPARAM
#define GET_X_LPARAM(lp)   ((int)(short)LOWORD(lp))
#endif
#ifndef GET_Y_LPARAM
#define GET_Y_LPARAM(lp)   ((int)(short)HIWORD(lp))
#endif

namespace gadget
{

bool KeyboardWin32::config( jccl::ConfigChunkPtr c )
{
   vprDEBUG_BEGIN(gadgetDBG_INPUT_MGR, vprDBG_STATE_LVL)
      << "vjKeyboardWin32::config " << std::endl << vprDEBUG_FLUSH;

   // Call base class config function first
   if (!(Input::config( c ) && Keyboard::config( c )))
     return false;

   int i;
   for (i =0; i < 256; i++)
      m_curKeys[i] = m_realkeys[i] = m_keys[i] = 0;
   m_curKeys[0] = m_realkeys[0] = m_keys[0] = 1;

   // Get size and position
   m_width = c->getProperty<int>( "width" );
   m_height = c->getProperty<int>( "height" );

   // default to something "sane" if too small
   if (m_width == 0) m_width = 400;
   if (m_height == 0) m_height = 400;

   m_x = c->getProperty<int>( "origin", 0 );
   m_y = c->getProperty<int>( "origin", 1 );

   // Get the lock information
   mLockToggleKey = c->getProperty<int>( "lock_key" );
   std::cout << "[][][][] Lock Key is " << (char)mLockToggleKey << " " << (int)mLockToggleKey << std::endl;
   bool start_locked = c->getProperty<bool>( "start_locked" );
   if (start_locked)
   {
      mLockState = Lock_LockKey;     // Initialize to the locked state
   }

   m_mouse_sensitivity = c->getProperty<float>( "msens" );

   // HACK: Use a default until config files have defaults
   if (0.0f == m_mouse_sensitivity)
   {
      m_mouse_sensitivity = 0.5f;
   }

   vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_STATE_LVL)
     << "Mouse Sensititivty: " << m_mouse_sensitivity << std::endl
     << vprDEBUG_FLUSH;

   mSleepTimeMS = c->getProperty<int>( "sleep_time" );

   // HACK: Use a default time until config files have defaults
   if (mSleepTimeMS == 0)
      mSleepTimeMS = 50;

   // Default to owning the window
   mWeOwnTheWindow = true;

   vprDEBUG_END(gadgetDBG_INPUT_MGR, vprDBG_STATE_LVL)
      << std::endl << vprDEBUG_FLUSH;

   return true;
}


void KeyboardWin32::controlLoop( void* devPtr )
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
   if (mLockState == Lock_LockKey)     // Means that we are in the initially locked state
   {
      vprDEBUG(gadgetDBG_INPUT_MGR,vprDBG_STATE_LVL)
        << "gadget::KeyboardWin32::controlLoop: Mouse set to initial lock. Locking it now.\n"
        << vprDEBUG_FLUSH;
      this->lockMouse();                // Lock the mouse
   }

   // When there are messages, process them all.  Otherwise,
   // sleep for a while...
   while (!mExitFlag)
   {
     this->sample();

     // user-specified sleep time.
     vpr::System::usleep( mSleepTimeMS * 1000 );
   }

   // clean up, delete the window!
   ::CloseWindow( m_hWnd ); // send a message to the window to close
   mControlLoopDone = true;
}

// do not call from any other thread than controlLoop()!!!!
// Peek and GetMessage retrieves messages for any window
// that belongs to the calling thread...
void KeyboardWin32::handleEvents()
{
   MSG msg;
   bool have_events_to_check = false;
   if (mWeOwnTheWindow)
   {
      // block until message received.
      int retval = ::GetMessage( &msg, m_hWnd, 0, 0 );
      vprASSERT( retval != -1 && "invalid m_hWnd window handle or invalid lpMsg pointer" );
      if (retval == -1) return; // for opt mode...
      have_events_to_check = true;
   }
   // check for event, if none, then exit.
   else if (::PeekMessage( &msg, m_hWnd, 0, 0, PM_REMOVE ))
   {
      have_events_to_check = true;
   }
   else
   {
      have_events_to_check = false;
      return; // don't wait on the lock since there is nothing
              // afterwards that will be called when this is false.
   }

// GUARD m_keys for duration of loop
// Doing it here gives makes sure that we process all events and don't get only part of them for an update
// In order to copy data over to the m_curKeys array
// Lock access to the m_keys array for the duration of this function
vpr::Guard<vpr::Mutex> guard( mKeysLock );

   while (have_events_to_check)
   {
      // Since we have no accelerators, no need to call
      // TranslateAccelerator here.
      ::TranslateMessage( &msg );

      // do our own special handling of kb/mouse...
      this->updKeys( msg.message, msg.wParam, msg.lParam );

      // send the message to the registered event handler
      ::DispatchMessage( &msg );

      // see if there is more messages immediately waiting
      // (don't block), process them all at once...
      int retval = ::PeekMessage( &msg, m_hWnd, 0, 0, PM_REMOVE );
      if (retval != 0) // messages != 0, nomessages == 0
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
int KeyboardWin32::startSampling()
{
   if (mThread != NULL)
   {
     vprDEBUG(vprDBG_ERROR,vprDBG_CRITICAL_LVL)
       << clrOutNORM(clrRED,"ERROR:")
       << "gadget::KeyboardWin32: startSampling called, when already sampling.\n"
       << vprDEBUG_FLUSH;
     vprASSERT( mThread == NULL );
     return 0;
   }

   vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_CONFIG_LVL)
     << "gadget::KeyboardWin32::startSampling() : ready to go.."
     << std::endl << vprDEBUG_FLUSH;

   // Create a new thread to handle the control
   vpr::ThreadMemberFunctor<KeyboardWin32>* memberFunctor =
     new vpr::ThreadMemberFunctor<KeyboardWin32>( this,
                                       &KeyboardWin32::controlLoop,
                                       (void*)this );

   mThread = new vpr::Thread(memberFunctor);

   // return success value...
   if ( ! mThread->valid() )
   {
     return 0; // fail
   }

   return 1; // success
}

int KeyboardWin32::onlyModifier( int mod )
{
  switch (mod)
  {
    case VJKEY_NONE:
      return (!m_curKeys[VJKEY_SHIFT] && !m_curKeys[VJKEY_CTRL] && !m_curKeys[VJKEY_ALT]);
    case VJKEY_SHIFT:
      return (m_curKeys[VJKEY_SHIFT] && !m_curKeys[VJKEY_CTRL] && !m_curKeys[VJKEY_ALT]);
    case VJKEY_CTRL:
      return (!m_curKeys[VJKEY_SHIFT] && m_curKeys[VJKEY_CTRL] && !m_curKeys[VJKEY_ALT]);
    case VJKEY_ALT:
      return (!m_curKeys[VJKEY_SHIFT] && !m_curKeys[VJKEY_CTRL] && m_curKeys[VJKEY_ALT]);
    default:
      vprASSERT(false);
      return 0;
  }
}

void KeyboardWin32::updateData()
{
vpr::Guard<vpr::Mutex> guard(mKeysLock);     // Lock access to the m_keys array

   // Scale mouse values based on sensitivity
   m_keys[VJMOUSE_POSX] = int(float(m_keys[VJMOUSE_POSX]) * m_mouse_sensitivity);
   m_keys[VJMOUSE_NEGX] = int(float(m_keys[VJMOUSE_NEGX]) * m_mouse_sensitivity);
   m_keys[VJMOUSE_POSY] = int(float(m_keys[VJMOUSE_POSY]) * m_mouse_sensitivity);
   m_keys[VJMOUSE_NEGY] = int(float(m_keys[VJMOUSE_NEGY]) * m_mouse_sensitivity);

   // Copy over values
   for (unsigned int i = 0; i < 256; ++i)
   {
      m_curKeys[i] = m_keys[i];
   }

   // Re-initialize the m_keys based on current key state in realKeys
   // Set the initial state of the m_key key counts based on the current state of the system
   // this is to ensure that if a key is still down, we get at least one event for it
   for (unsigned int j = 0; j < 256; j++)
   {
      m_keys[j] = m_realkeys[j];
   }
}

// UpdKeys: translates windows message into key updates
// The WNDPROC uses its USERDATA pointer to the keyboard
// to forward on messages to be handled from in the keyboard object.
void KeyboardWin32::updKeys( UINT message, UINT wParam, LONG lParam )
{
   vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_HVERB_LVL)
      << mInstName << ": KeyWin32::updKeys: Processing keys.\n"
      << vprDEBUG_FLUSH;

   int key;

   switch (message)
   {
   case WM_ACTIVATE:
      // if was activated, and not minimized
      if ((WA_ACTIVE == LOWORD( wParam ) ||
           WA_CLICKACTIVE == LOWORD( wParam )) &&
          0 == HIWORD( wParam ))
      {
         // and was previously locked...
         if (Unlocked != mLockState)
         {
            this->lockMouse();
         }
      }
      // if was deactivated and minimized
      // otherwise let CAPTURECHANGED handle the lose capture case
      else if (WA_INACTIVE == LOWORD( wParam ) && 0 != HIWORD( wParam ))
      {
         // and was locked...
         if (Unlocked != mLockState)
         {
            // we will leave mLockState in the locked state so ACTIVATE puts it back.
            this->unlockMouse();
         }
      }
      break;

   // sent to the window that is losing the mouse capture
   case WM_CAPTURECHANGED:
         // if locked, unlock it
         if (Unlocked != mLockState)
         {
            // we will leave mLockState in the locked state so ACTIVATE puts it back.
            this->unlockMouse();
         }
      break;

   // keys and buttons
   // When we hit a key (or button), then set the m_key and m_realkey
   // When release, only set real so that we don't lose a press of the
   // button then when the updateData happens, the framekeys will be set to
   // non-pressed from real_keys

   // press
   case WM_KEYDOWN:
      {
         // collect data about the keypress.
         key = this->VKKeyToKey( wParam );

         // get the repeat count in case the key was pressed
         // multiple times since last we got this message
         int repeat_count = lParam & 0x0000ffff;

         // check if the key was down already
         // this indicates key repeat, which we [may] want to ignore.
         bool was_down = (lParam & 0x40000000) == 0x40000000;
         if (was_down)
         {
            break;
         }

         // process the keypress.
         m_keys[key] += repeat_count;
         m_realkeys[key] += 1;

         // Check for Escape from bad state
         // this provides a sort of failsafe to
         // get out of the locked state...
         // @todo this is sort of hard coded, do we want it this way?
         if (key == VJKEY_ESC)
         {
            if(mLockState != Unlocked)
            {
               mLockState = Unlocked;
               this->unlockMouse();
            }
         }
         else if(mLockState == Unlocked)
         {
            if( (key != mLockToggleKey) &&
               ((VJKEY_ALT == key) || (VJKEY_CTRL == key) || (VJKEY_SHIFT == key)) )
            {
               mLockState = Lock_KeyDown;       // Switch state
               mLockStoredKey = key;         // Store the VJ key that is down
               this->lockMouse();
            }
            else if(key == mLockToggleKey)
            {
               mLockState = Lock_LockKey;
               this->lockMouse();
            }
         }
         // Just switch the current locking state
         else if((mLockState == Lock_KeyDown) && (key == mLockToggleKey))
         {
            mLockState = Lock_LockKey;
         }
         else if((mLockState == Lock_LockKey) && (key == mLockToggleKey))
         {
            mLockState = Unlocked;
            this->unlockMouse();
         }
      }
      break;

   // release
   case WM_KEYUP:
      key = VKKeyToKey(wParam);
      m_realkeys[key] = 0;
      vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_HVERB_LVL)
         << mInstName << ": WM_KEYUP: " << key << std::endl
         << vprDEBUG_FLUSH;

      // -- Update lock state -- //
      // lock_keyDown[key==storedKey]/unlockMouse -> unlocked
      if ((mLockState == Lock_KeyDown) && (key == mLockStoredKey))
      {
         mLockState = Unlocked;
         this->unlockMouse();
      }
      break;

   // press...
   case WM_LBUTTONDOWN:
      m_realkeys[VJMBUTTON1] = 1;
      m_keys[VJMBUTTON1] += 1;
      vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_HVERB_LVL)
         << "LeftButtonDown\n" << vprDEBUG_FLUSH;
      break;
   case WM_MBUTTONDOWN:
      m_realkeys[VJMBUTTON2] = 1;
      m_keys[VJMBUTTON2] += 1;
      vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_HVERB_LVL)
         << "MiddleButtonDown\n" << vprDEBUG_FLUSH;
      break;
   case WM_RBUTTONDOWN:
      m_realkeys[VJMBUTTON3] = 1;
      m_keys[VJMBUTTON3] += 1;
      vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_HVERB_LVL)
         << "RightButtonDown\n" << vprDEBUG_FLUSH;
      break;

   // release...
   case WM_LBUTTONUP:
      m_realkeys[VJMBUTTON1] = 0;
      vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_HVERB_LVL)
         << "LeftButtonUp\n" << vprDEBUG_FLUSH;
      break;
   case WM_MBUTTONUP:
      m_realkeys[VJMBUTTON2] = 0;
      vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_HVERB_LVL)
         << "MiddleButtonUp\n" << vprDEBUG_FLUSH;
      break;
   case WM_RBUTTONUP:
      m_realkeys[VJMBUTTON3] = 0;
      vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_HVERB_LVL)
         << "RightButtonUp\n" << vprDEBUG_FLUSH;
      break;

   // mouse movement
   case WM_MOUSEMOVE:
   {
      int win_center_x( m_width / 2 ),
            win_center_y( m_height / 2 );

      int cur_x, cur_y, dx, dy;

      // Determine how far the mouse pointer moved since the last event.
      // x & y are relative to the x window
      cur_x = GET_X_LPARAM( lParam );
      cur_y = GET_Y_LPARAM( lParam );

      vprDEBUG(vprDBG_ALL,vprDBG_HVERB_LVL) << "MotionNotify: x:"
              << std::setw(6) << cur_x
              << "  y:" << std::setw(6) << cur_y << std::endl
              << vprDEBUG_FLUSH;

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
         mPrevX = win_center_x;    // Must do this so if state changes, we have accurate dx,dy next time
         mPrevY = win_center_y;

         // Warp back to center, IF we are not there already
         // This prevents us from sending an event based on our XWarp event
         if ((dx != 0) || (dy != 0))
         {
            vprDEBUG(vprDBG_ALL,vprDBG_HVERB_LVL) << "CORRECTING: x:"
                  << std::setw(6) << dx << "  y:"
                  << std::setw(6) << dy
                  << std::endl << vprDEBUG_FLUSH;

            // convert windows coords to screen coords.
            POINT pt;
            pt.x = win_center_x;
            pt.y = win_center_y;
            ::ClientToScreen( m_hWnd, &pt );
            ::SetCursorPos( pt.x, pt.y );
         }
      }

      // Update m_keys based on key pressed and store in the key array
      if ( dx > 0 ) {
         m_keys[VJMOUSE_POSX] += dx;     // Positive movement in the x direction.
      } else {
         m_keys[VJMOUSE_NEGX] += -dx;    // Negative movement in the x direction.
      }

      if ( dy > 0 ) {
         m_keys[VJMOUSE_POSY] += dy;     // Positive movement in the y direction.
      } else {
         m_keys[VJMOUSE_NEGY] += -dy;    // Negative movement in the y direction.
      }
      break;
   }
   } // end of switch...

   // Let any other event watchers process their events
   this->processEvent( message, wParam, lParam );
}


int KeyboardWin32::stopSampling()
{
   if (mThread != NULL)
   {
      mExitFlag = true;
      ::PostMessage( m_hWnd, WM_USER, 0, 0 );// send a dummy message to the window to close
      // 1000000 usec timeout before we give up and kill the thread...
      for (int x = 0; x < 100000 && !mControlLoopDone; ++x)
      {
         // give the window thread a chance before we delete...
         vpr::System::usleep( 10 );
      }
      delete mThread;
      mThread = NULL;
      //std::cout << "Stopping Keyboard.." << std::endl;
   }
   return 1;
}

int KeyboardWin32::VKKeyToKey( int vkKey )
{
   switch (vkKey)
   {
    case VK_UP      : return VJKEY_UP;
    case VK_DOWN    : return VJKEY_DOWN;
    case VK_LEFT    : return VJKEY_LEFT;
    case VK_RIGHT   : return VJKEY_RIGHT;
    case VK_CONTROL  : return VJKEY_CTRL;
    case VK_SHIFT   : return VJKEY_SHIFT;
    case VK_TAB     : return VJKEY_ALT; // @todo why is tab mapped to ALT?
//    case VK_ALT     : return KEY_ALT;
    case /*VK_1*/0x31  : return VJKEY_1;
    case VK_NUMPAD1   : return VJKEY_1;
    case /*VK_2*/0x32  : return VJKEY_2;
    case VK_NUMPAD2   : return VJKEY_2;
    case /*VK_3*/0x33 : return VJKEY_3;
    case VK_NUMPAD3   : return VJKEY_3;
    case /*VK_4*/0x34  : return VJKEY_4;
    case VK_NUMPAD4   : return VJKEY_4;
    case /*VK_5*/0x35 : return VJKEY_5;
    case VK_NUMPAD5   : return VJKEY_5;
    case /*VK_6*/0x36  : return VJKEY_6;
    case VK_NUMPAD6   : return VJKEY_6;
    case /*VK_7*/0x37  : return VJKEY_7;
    case VK_NUMPAD7   : return VJKEY_7;
    case /*VK_8*/0x38  : return VJKEY_8;
    case VK_NUMPAD8   : return VJKEY_8;
    case /*VK_9*/0x39  : return VJKEY_9;
    case VK_NUMPAD9   : return VJKEY_9;
    case /*VK_0*/0x30  : return VJKEY_0;
    case VK_NUMPAD0   : return VJKEY_0;
    case /*VK_A*/0x41   : return VJKEY_A;
    case /*VK_B*/0x42   : return VJKEY_B;
    case /*VK_C*/0x43   : return VJKEY_C;
    case /*VK_D*/0x44   : return VJKEY_D;
    case /*VK_E*/0x45   : return VJKEY_E;
    case /*VK_F*/0x46   : return VJKEY_F;
    case /*VK_G*/0x47   : return VJKEY_G;
    case /*VK_H*/0x48   : return VJKEY_H;
    case /*VK_I*/0x49   : return VJKEY_I;
    case /*VK_J*/0x4a   : return VJKEY_J;
    case /*VK_K*/0x4b   : return VJKEY_K;
    case /*VK_L*/0x4c   : return VJKEY_L;
    case /*VK_M*/0x4d   : return VJKEY_M;
    case /*VK_N*/0x4e   : return VJKEY_N;
    case /*VK_O*/0x4f   : return VJKEY_O;
    case /*VK_P*/0x50   : return VJKEY_P;
    case /*VK_Q*/0x51   : return VJKEY_Q;
    case /*VK_R*/0x52   : return VJKEY_R;
    case /*VK_S*/0x53   : return VJKEY_S;
    case /*VK_T*/0x54   : return VJKEY_T;
    case /*VK_U*/0x55   : return VJKEY_U;
    case /*VK_V*/0x56   : return VJKEY_V;
    case /*VK_W*/0x57   : return VJKEY_W;
    case /*VK_X*/0x58   : return VJKEY_X;
    case /*VK_Y*/0x59   : return VJKEY_Y;
    case /*VK_Z*/0x5a   : return VJKEY_Z;
    case VK_ESCAPE   : return VJKEY_ESC;
    default: return 255;
   }

}

/*****************************************************************/
/*****************************************************************/
/***********************WINDOW STUFF****************************/
/*****************************************************************/
/*****************************************************************/

//#ifdef DEBUG
#define NEGADD1(x,y)  ((x < 0) ? x+1 : x)

/****************************************************************************
 *                                                        *
 *  FUNCTION   : HandlePaint ( hwnd )                              *
 *                                                        *
 *  PURPOSE   : Handles the repainting of window client area.           *
 *            (Outputs the data current as of the last UpdateData call   *
 ****************************************************************************/
VOID APIENTRY HandlePaint( HWND hwnd )
{
   HDC       hdc;
   PAINTSTRUCT ps;
   Keyboard* devPtr = (Keyboard*)GetWindowLong(hwnd,GWL_USERDATA);

   if (!devPtr)
      return;

   char ln1[255] = "line1";
   char ln2[255] = "line2";
   char ln3[255] = "line3";
   char ln4[255] = "line4";
   char ln5[255] = "line5";
   char ln6[255] = "line6";
   //vjgmtl::Matrix44f* McurrData = devPtr->GetPosData();

   //vjCoord coord(*McurrData);
   //ThreeDouble2String(ln1, coord.pos[0], coord.pos[1], coord.pos[2]);
   //ThreeDouble2String(ln2, coord.orient[0], coord.orient[1], coord.orient[2]);

   //vjMcurrData = devPtr->GetPosData(1);
   //vjCoord coord2(*McurrData);
   //ThreeDouble2String(ln3, coord2.pos[0], coord2.pos[1], coord2.pos[2]);
   //ThreeDouble2String(ln4, coord2.orient[0], coord2.orient[1], coord2.orient[2]);

   //wsprintf(ln5,"%li %d %d %d", devPtr->GetDigitalData(0), devPtr->GetDigitalData(1),devPtr->GetDigitalData(2),devPtr->GetDigitalData(3));
   //wsprintf(ln6,"%li %d %d %d", devPtr->GetAnalogData(0), devPtr->GetAnalogData(1),devPtr->GetAnalogData(2),devPtr->GetAnalogData(3));

   hdc = BeginPaint (hwnd, (LPPAINTSTRUCT)&ps);

   TextOut(hdc,0 ,0, "Pos0:", 5);
   TextOut(hdc,50,0,ln1, lstrlen(ln1));
   TextOut(hdc,50,20,ln2, lstrlen(ln2));

   /*
   TextOut(hdc,0 ,40, "Pos1:", 5);
   TextOut(hdc,50,40,ln3, lstrlen(ln3));
   TextOut(hdc,50,60,ln4, lstrlen(ln4));

   TextOut(hdc,0 ,90, "Digital:", 8);
   TextOut(hdc,50,90,ln5, lstrlen(ln5));

   TextOut(hdc,0 ,110, "Analog:", 7);
   TextOut(hdc,50,110,ln6, lstrlen(ln6));
   */

   EndPaint(hwnd, (LPPAINTSTRUCT)&ps);
}
//#endif // DEBUG

/****************************************************************************
 *                                                        *
 *  FUNCTION   : MenuWndProc (hWnd, message, wParam, lParam)            *
 *                                                        *
 *  PURPOSE   : Window function for the main app. window. Processes all the*
 *            menu selections and oter messages.                   *
 *                                                        *
 ****************************************************************************/
LONG APIENTRY MenuWndProc( HWND hWnd, UINT message, UINT wParam, LONG lParam )
{
   switch (message) {
      case WM_SYSCOMMAND:
           return DefWindowProc( hWnd, message, wParam, lParam );

//      case WM_COMMAND:
//         break;

      case WM_SIZE:
         if (lParam)
         {
            InvalidateRect( hWnd, NULL, TRUE );
         }
         break;

//#ifdef DEBUG  // Show the pos/dig/analog data in the window in debug
            // (really inefficient display code)
      case WM_PAINT:
         HandlePaint( hWnd );
         break;
//#endif

      case WM_DESTROY:
         PostQuitMessage( 0 );
         break;

      default:
         return DefWindowProc( hWnd, message, wParam, lParam );
   }
   return 0;
}


void KeyboardWin32::createWindowWin32()
{
   int root_height;

   InitCommonControls();

   //m_hInst = g_hInst;
   m_hInst = GetModuleHandle(NULL);   // Just try to get the application's handle
   MenuInit(m_hInst);

   root_height = GetSystemMetrics( SM_CYSCREEN );

   /* Create the app. window */
   m_hWnd = CreateWindow(("Juggler Keyboard"), mInstName.c_str(),
                   WS_OVERLAPPEDWINDOW, m_x,
                   root_height - m_y - m_height, m_width, m_height,
                   (HWND) NULL, NULL, m_hInst, (LPSTR) NULL);
   ShowWindow(m_hWnd,SW_SHOW);
   UpdateWindow(m_hWnd);

   // Attach a pointer to the device for use from the WNDPROC
   SetWindowLong(m_hWnd, GWL_USERDATA, (LPARAM)this);

   DWORD dwErr = GetLastError();
   if (!m_hWnd)
     return;

} /*CreateWindow*/

BOOL KeyboardWin32::MenuInit (HINSTANCE hInstance)
{
   HANDLE   hMemory;
   PWNDCLASS pWndClass;
   BOOL     bSuccess;

   /* Initialize the menu window class */
   hMemory   = LocalAlloc(LPTR, sizeof(WNDCLASS));
   if(!hMemory){
      MessageBox(NULL, ("<MenuInit> Not enough memory."), NULL, MB_OK | MB_ICONHAND);
      return(FALSE);
   }

   pWndClass = (PWNDCLASS) LocalLock(hMemory);

   pWndClass->style       = 0;
   pWndClass->lpfnWndProc   = (WNDPROC) MenuWndProc;
   pWndClass->hInstance    = hInstance;
//   pyrate_x = (HICON)LoadIcon(hInstance, _T("prof"));
   pWndClass->hIcon       = (HICON)LoadIcon(hInstance,TEXT("Juggler"));
   pWndClass->hCursor      = (HCURSOR)LoadCursor (NULL, IDC_ARROW);
   pWndClass->hbrBackground = (HBRUSH)GetStockObject (WHITE_BRUSH);
   pWndClass->lpszMenuName  = ( "MenuMenu"),
   pWndClass->lpszClassName = ("Juggler Keyboard");

   bSuccess = RegisterClass( pWndClass );
   LocalUnlock( hMemory );
   LocalFree( hMemory );

   return bSuccess;
}


// process the current window events
// Called repeatedly by
// - the controlLoop when "we own the window",
// - the GlWindow if we "dont own the window"
int KeyboardWin32::sample()
{
   this->handleEvents();
   return 1;
}

std::string KeyboardWin32::getChunkType()
{
   return std::string( "Keyboard" );
}

// returns the number of times the key was pressed during the
// last frame, so you can put this in an if to check if was
// pressed at all, or if you are doing processing based on this
// catch the actual number..
int KeyboardWin32::isKeyPressed( int Key )
{
   return m_curKeys[Key];
}

void KeyboardWin32::lockMouse()
{
   // Center the mouse
   int win_center_x( m_width / 2 ),
       win_center_y( m_height / 2 );

   // convert windows coords to screen coords.
   ::POINT pt;
   pt.x = win_center_x;
   pt.y = win_center_y;
   ::ClientToScreen( m_hWnd, &pt );
   ::SetCursorPos( pt.x, pt.y );

   // capture the mouse for this window...
   HWND previous_capture = ::SetCapture( m_hWnd );
}
void KeyboardWin32::unlockMouse()
{
   BOOL result = ::ReleaseCapture();
}

} // End of gadget namespace
