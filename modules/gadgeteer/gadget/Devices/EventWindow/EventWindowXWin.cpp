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

#include <vpr/Thread/Thread.h>
#include <vpr/System.h>
#include <jccl/Config/ConfigChunk.h>

#include <gadget/Util/Debug.h>
#include <gadget/InputManager.h>
#include <gadget/Devices/EventWindow/EventWindowXWin.h> // my header


namespace gadget
{

/** Constructor. */
bool EventWindowXWin::config(jccl::ConfigChunkPtr c)
{
   if ( ! (Input::config(c) && EventWindow::config(c)) )
   {
      return false;
   }

   const char neg_one_STRING[] = "-1";

   // Done in Input --- mThread = NULL;
   for ( int i = 0; i < 256; ++i )
   {
      mCurKeys[i] = mRealkeys[i] = mKeys[i] = 0;
   }

   mCurKeys[0] = mRealkeys[0] = mKeys[0] = 1;

   // Get size and position
   mWidth = c->getProperty<int>("width");
   mHeight = c->getProperty<int>("height");

   // Sanity checks.
   if (mWidth == 0) mWidth = 400;
   if (mHeight == 0) mHeight = 400;

   mX = c->getProperty<int>("origin", 0);
   mY = c->getProperty<int>("origin", 1);

   // Get the X display string
   int x_disp_num = c->getProperty<int>("display_number");
   jccl::ConfigChunkPtr dispSysChunk = gadget::InputManager::instance()->getDisplaySystemChunk();

   if ((x_disp_num >= 0) && (dispSysChunk.get() != NULL) )
   {
      std::string xpipe_str;
      xpipe_str = dispSysChunk->getProperty<std::string>("xpipes", x_disp_num);
      mXDisplayString = xpipe_str;
   }
   else
   {
      mXDisplayString = std::string("-1");
   }

   if ((mXDisplayString.empty()) || (strcmp(mXDisplayString.c_str(), neg_one_STRING) == 0))    // Use display env
   {
      const std::string DISPLAY_str("DISPLAY");
      mXDisplayString = (std::string)getenv(DISPLAY_str.c_str());
   }

   // Get the lock information
   mLockToggleKey = c->getProperty<int>("lock_key");
   bool start_locked = c->getProperty<bool>("start_locked");

   if (start_locked)
   {
      mLockState = Lock_LockKey;      // Initialize to the locked state
   }

   mMouseSensitivity = c->getProperty<float>("msens");
   if (0.0f == mMouseSensitivity)
   {
      mMouseSensitivity = 0.5;
   }

   vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_STATE_LVL)
      << "Mouse Sensititivty: " << mMouseSensitivity << std::endl
      << vprDEBUG_FLUSH;

   mSleepTimeMS = c->getProperty<int>("sleep_time");

   // Sanity check.
   if (mSleepTimeMS == 0)
   {
      mSleepTimeMS = 50;
   }

   // Default to owning the window
   mWeOwnTheWindow = true;

   return true;
}

// Main thread of control for this active object
void EventWindowXWin::controlLoop(void* nullParam)
{
   vprDEBUG(gadgetDBG_INPUT_MGR,vprDBG_STATE_LVL)
      << "gadget::EventWindowXWin::controlLoop: Thread started.\n"
      << vprDEBUG_FLUSH;

   while (NULL == vpr::Thread::self())
   {
      vpr::System::usleep(50);
      vprDEBUG(vprDBG_ALL,vprDBG_VERB_LVL)
         << "gadget::EventWindowXWin: Waiting for (thread::self() != NULL)\n"
         << vprDEBUG_FLUSH;
   }
   mThread = (vpr::Thread*) vpr::Thread::self();

   vprASSERT(mWeOwnTheWindow && "control loop should not be called if we don't own window. Update in owning thread.");

   // Open the x-window
   openTheWindow();

   // Sync up with window
   XSync(mDisplay, 0);

   // If we have initial locked, then we need to lock the system
   if(mLockState == Lock_LockKey)      // Means that we are in the initially locked state
   {
      vprDEBUG(gadgetDBG_INPUT_MGR,vprDBG_STATE_LVL)
         << "gadget::EventWindowXWin::controlLoop: Mouse set to initial lock. Locking it now.\n"
         << vprDEBUG_FLUSH;
      lockMouse();                     // Lock the mouse
   }

   // Loop on updating
   while(!mExitFlag)
   {
      sample();
      long usleep_time(1); // to be set...

      usleep_time = mSleepTimeMS*1000;

      vpr::System::usleep(usleep_time);
   }

   // Exit, cleanup code
   XDestroyWindow(mDisplay, mWindow);
   XCloseDisplay((::Display*) mDisplay);
   mControlLoopDone = true;
}


int EventWindowXWin::startSampling()
{
   if(mThread != NULL)
   {
      vprDEBUG(vprDBG_ERROR,vprDBG_CRITICAL_LVL)
         << clrOutNORM(clrRED,"ERROR:")
         << "gadget::EventWindowXWin: startSampling called, when already sampling.\n"
         << vprDEBUG_FLUSH;
      vprASSERT(false);
   }

   // Create a new thread to handle the control
   vpr::ThreadMemberFunctor<EventWindowXWin>* memberFunctor =
      new vpr::ThreadMemberFunctor<EventWindowXWin>(this, &EventWindowXWin::controlLoop, NULL);

   mThread = new vpr::Thread(memberFunctor);

   // return success value...
   if ( ! mThread->valid() )
   {
      return 0; // fail
   }
   else
   {
      return 1; // success
   }
}

int EventWindowXWin::onlyModifier(int mod)
{
   switch (mod)
   {
      case gadget::KEY_NONE:
         return (!mCurKeys[gadget::KEY_SHIFT] && !mCurKeys[gadget::KEY_CTRL] && !mCurKeys[gadget::KEY_ALT]);
      case gadget::KEY_SHIFT:
         return (mCurKeys[gadget::KEY_SHIFT] && !mCurKeys[gadget::KEY_CTRL] && !mCurKeys[gadget::KEY_ALT]);
      case gadget::KEY_CTRL:
         return (!mCurKeys[gadget::KEY_SHIFT] && mCurKeys[gadget::KEY_CTRL] && !mCurKeys[gadget::KEY_ALT]);
      case gadget::KEY_ALT:
         return (!mCurKeys[gadget::KEY_SHIFT] && !mCurKeys[gadget::KEY_CTRL] && mCurKeys[gadget::KEY_ALT]);
      default:
        vprASSERT(false);
        return 0;
   }
}

void EventWindowXWin::updateData()
{
vpr::Guard<vpr::Mutex> guard(mKeysLock);      // Lock access to the mKeys array

// Scale mouse values based on sensitivity
   mKeys[gadget::MOUSE_POSX] = int(float(mKeys[gadget::MOUSE_POSX]) * mMouseSensitivity);
   mKeys[gadget::MOUSE_NEGX] = int(float(mKeys[gadget::MOUSE_NEGX]) * mMouseSensitivity);
   mKeys[gadget::MOUSE_POSY] = int(float(mKeys[gadget::MOUSE_POSY]) * mMouseSensitivity);
   mKeys[gadget::MOUSE_NEGY] = int(float(mKeys[gadget::MOUSE_NEGY]) * mMouseSensitivity);

   /*
   vprDEBUG(vprDBG_ALL,0)
      << "gadget::EventWindowXWin::updateData:" << mInstName << " -- "
      << "mouse_keys: px:" << mKeys[gadget::MOUSE_POSX]
      << " nx:" << mKeys[gadget::MOUSE_NEGX]
      << " py:" << mKeys[gadget::MOUSE_POSY]
      << " ny:" << mKeys[gadget::MOUSE_NEGY]
      << std::endl << vprDEBUG_FLUSH;
   */

   // Copy over values
   for ( unsigned int i = 0; i < 256; ++i )
   {
      mCurKeys[i] = mKeys[i];
   }

   // Re-initialize the mKeys based on current key state in realKeys
   // Set the initial state of the mKey key counts based on the current state
   // of the system this is to ensure that if a key is still down, we get at
   // least one event for it.
   for ( unsigned int j = 0; j < 256; ++j )
   {
      mKeys[j] = mRealkeys[j];
   }
}

void EventWindowXWin::HandleEvents()
{
   XEvent event;
   KeySym key;
   int    vj_key;          // The key in vj space
   bool   have_events_to_check(true);  // Used by the while loop below

   // XXX: Need to add to mask to get more events for subclasses
   const long event_mask = (KeyPressMask | KeyReleaseMask | ButtonPressMask |
                            ButtonReleaseMask | ButtonMotionMask |
                            PointerMotionMask | StructureNotifyMask);

   // Check for events
   // If we own,
   //  - Wait until we actually have some events -- THIS BLOCKS for an event
   // else
   //  - check for event, if none, then we exit
   if( mWeOwnTheWindow )
   {
      XWindowEvent(mDisplay, mWindow, event_mask, &event);
      have_events_to_check = true;
   }
   else
   {
      have_events_to_check = XCheckWindowEvent(mDisplay, mWindow, event_mask,
                                               &event);
   }

// GUARD mKeys for duration of loop.
// Doing it here gives makes sure that we process all events and don't get only
// part of them for an update.  In order to copy data over to the mCurKeys
// array.
vpr::Guard<vpr::Mutex> guard(mKeysLock);      // Lock access to the mKeys array for the duration of this function


   // Loop while the event queue contains events for mWindow that are part
   // of the given event mask.
   while(have_events_to_check)
   {
      switch (event.type)
      {
      // A KeyPress event occurred.  Flag the key that was pressed (as a
      // Gadgeteer key) as being pressed and grab the keyboard.
      case KeyPress:
         // Convert the pressed key from the event to a Gadgeteer key.
         // And store in the array of key presses
         key = XLookupKeysym((XKeyEvent*)&event,0);
         vj_key = xKeyToKey(key);
         mRealkeys[vj_key] = 1;
         mKeys[vj_key] += 1;

         // -- Update lock state -- //
         // Any[key == ESC]/unlock(ifneeded) -> Unlocked
         // Unlocked[key!=lockKey]/lockMouse -> lock_keydown
         // Unlocked[key==lockKey]/lockMouse -> lock_keylock
         // lock_keydown[key==lockKey] -> lock_keylock
         // lock_keylock[key==lockKey] -> Unlocked
         if (vj_key == gadget::KEY_ESC)       // Check for Escape from bad state
         {
            vprDEBUG(gadgetDBG_INPUT_MGR,vprDBG_STATE_LVL)
               << "gadget::EventWindowXWin: Trying to ESCAPE from current state.\n"
               << vprDEBUG_FLUSH;
            if(mLockState != Unlocked)
            {
               vprDEBUG_NEXT(gadgetDBG_INPUT_MGR,vprDBG_STATE_LVL)
                  << "gadget::EventWindowXWin: STATE switch: <ESCAPE> --> Unlocked\n"
                  << vprDEBUG_FLUSH;
               mLockState = Unlocked;
               unlockMouse();
            }
            else
            {
               vprDEBUG_NEXT(gadgetDBG_INPUT_MGR,vprDBG_STATE_LVL)
                  << "gadget::EventWindowXWin: Already unlocked.  Cannot ESCAPE."
                  << vprDEBUG_FLUSH;
            }
         }
         else if(mLockState == Unlocked)
         {
            if ( (vj_key != mLockToggleKey) &&
                 ((gadget::KEY_ALT == vj_key) || (gadget::KEY_CTRL == vj_key) ||
                  (gadget::KEY_SHIFT == vj_key)) )
            {
               mLockState = Lock_KeyDown;       // Switch state
               mLockStoredKey = vj_key;         // Store the Gadgeteer key that is down
               vprDEBUG(gadgetDBG_INPUT_MGR,vprDBG_STATE_LVL)
                  << "gadget::EventWindowXWin: STATE switch: Unlocked --> Lock_KeyDown\n"
                  << vprDEBUG_FLUSH;
               lockMouse();
            }
            else if(vj_key == mLockToggleKey)
            {
               mLockState = Lock_LockKey;
               vprDEBUG(gadgetDBG_INPUT_MGR,vprDBG_STATE_LVL)
                  << "gadget::EventWindowXWin: STATE switch: Unlocked --> Lock_LockKey\n"
                  << vprDEBUG_FLUSH;
               lockMouse();
            }
         }
         else if((mLockState == Lock_KeyDown) && (vj_key == mLockToggleKey))     // Just switch the current locking state
         {
            mLockState = Lock_LockKey;
            vprDEBUG(gadgetDBG_INPUT_MGR,vprDBG_STATE_LVL)
               << "gadget::EventWindowXWin: STATE switch: Lock_KeyDown --> Lock_LockKey\n"
               << vprDEBUG_FLUSH;
         }
         else if((mLockState == Lock_LockKey) && (vj_key == mLockToggleKey))
         {
            mLockState = Unlocked;
            vprDEBUG(gadgetDBG_INPUT_MGR,vprDBG_STATE_LVL)
               << "gadget::EventWindowXWin: STATE switch: Lock_LockKey --> Unlocked\n"
               << vprDEBUG_FLUSH;
            unlockMouse();
         }

         vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_HVERB_LVL)    //vprDBG_HVERB_LVL
            << "KeyPress:  " << std::hex << key
            << " state:" << ((XKeyEvent*)&event)->state
            << " ==> " << xKeyToKey(key) << std::endl << vprDEBUG_FLUSH;

         break;

      // A KeyRelease event occurred.  Flag the key that was released (as a
      // Gadgeteer key) as being not pressed and ungrab the keyboard.
      case KeyRelease:
         // Convert the released key from the event to a Gadgeteer key.
         key = XLookupKeysym((XKeyEvent*)&event,0);
         vj_key = xKeyToKey(key);
         mRealkeys[vj_key] = 0;

         // -- Update lock state -- //
         // lock_keyDown[key==storedKey]/unlockMouse -> unlocked
         if((mLockState == Lock_KeyDown) && (vj_key == mLockStoredKey))
         {
            mLockState = Unlocked;
            vprDEBUG(gadgetDBG_INPUT_MGR,vprDBG_STATE_LVL)
               << "gadget::EventWindowXWin: STATE switch: Lock_KeyDown --> Unlocked\n"
               << vprDEBUG_FLUSH;
            unlockMouse();
         }

         vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_HVERB_LVL)
            << "KeyRelease:" << std::hex << key
            << " state:" << ((XKeyEvent*)&event)->state
            << " ==> " << xKeyToKey(key) << std::endl << vprDEBUG_FLUSH;
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

            vprDEBUG(vprDBG_ALL,vprDBG_HVERB_LVL) << "MotionNotify: x:"  << std::setw(6) << cur_x << "  y:" << std::setw(6) << cur_y << std::endl << vprDEBUG_FLUSH;

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

               // Warp back to center, IF we are not there already
               // This prevents us from sending an event based on our XWarp event
               if((dx != 0) || (dy != 0))
               {
                  vprDEBUG(vprDBG_ALL,vprDBG_HVERB_LVL) << "CORRECTING: x:" << std::setw(6) << dx << "  y:" << std::setw(6) << dy << std::endl << vprDEBUG_FLUSH;

                  XWarpPointer(mDisplay, None, mWindow, 0,0, 0,0,
                               win_center_x, win_center_y);
               }
            }

            // Update mKeys based on key pressed and store in the key array
            if ( dx > 0 ) {
               mKeys[gadget::MOUSE_POSX] += dx;      // Positive movement in the x direction.
            } else {
               mKeys[gadget::MOUSE_NEGX] += -dx;     // Negative movement in the x direction.
            }

            if ( dy > 0 ) {
               mKeys[gadget::MOUSE_POSY] += dy;      // Positive movement in the y direction.
            } else {
               mKeys[gadget::MOUSE_NEGY] += -dy;     // Negative movement in the y direction.
            }
         }

         break;

      // A mouse button was pressed.  We set that button to 1 in mKeys AND
      // in mRealkeys so that it will be remembered as being held down
      // untill a ButtonRelease event occurs.
      case ButtonPress:
         switch ( event.xbutton.button )
         {
         case Button1:
            mRealkeys[gadget::MBUTTON1] = 1;
            mKeys[gadget::MBUTTON1] += 1;
            break;
         case Button2:
            mRealkeys[gadget::MBUTTON2] = 1;
            mKeys[gadget::MBUTTON2] += 1;
            break;
         case Button3:
            mRealkeys[gadget::MBUTTON3] = 1;
            mKeys[gadget::MBUTTON3] += 1;
            break;
         }

         break;

      // A mouse button was released.
      case ButtonRelease:
         switch ( event.xbutton.button )
         {
         case Button1:
            mRealkeys[gadget::MBUTTON1] = 0;
            break;
         case Button2:
            mRealkeys[gadget::MBUTTON2] = 0;
            break;
         case Button3:
            mRealkeys[gadget::MBUTTON3] = 0;
            break;
         }

         break;
      }

      // Let any other event watchers process their events
      this->processEvent(event);

      have_events_to_check = XCheckWindowEvent(mDisplay, mWindow, event_mask,
                                               &event);
   }
}

int EventWindowXWin::stopSampling()
{
   if (mThread != NULL)
   {
      mExitFlag = true;

      vpr::System::sleep(1);

      XResizeWindow(mDisplay, mWindow, 1,1); //Dummy event
      XFlush(mDisplay);

      while( !mControlLoopDone )
      {
         // give the window thread a chance before we delete...
         vpr::System::usleep( 20 );
      }

      delete mThread;
      mThread = NULL;
   }

   return 1;
}

int EventWindowXWin::xKeyToKey(KeySym xKey)
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
   case XK_Escape    : return gadget::KEY_ESC;
   default: return 255;
   }

}

/*****************************************************************/
/*****************************************************************/
/***********************X WINDOW STUFF****************************/
/*****************************************************************/
/*****************************************************************/
// Open the X window to sample from
int EventWindowXWin::openTheWindow()
{
   int i;

   mDisplay = XOpenDisplay(mXDisplayString.c_str());    // Open display on given XDisplay
   if (NULL == mDisplay)
   {
      vprDEBUG(vprDBG_ERROR,vprDBG_CRITICAL_LVL)
         <<  clrOutNORM(clrRED,"ERROR:")
         << "gadget::EventWindow::StartSampling() : failed to open display"
         << std::endl << vprDEBUG_FLUSH;
      return 0;
   }

   mScreen = DefaultScreen(mDisplay);

   XVisualInfo vTemplate, *vis_infos;
   long vMask = VisualScreenMask;
   vTemplate.screen = mScreen;
   int nVisuals;

   vis_infos = XGetVisualInfo(mDisplay, vMask, &vTemplate, &nVisuals);

   // Verify that we got at least one visual from XGetVisualInfo(3).
   if ( vis_infos != NULL && nVisuals >= 1 ) {
      XVisualInfo* p_visinfo;

      // Try to find a visual with color depth of at least 8 bits.  Having
      // such a visual ensures that the event windows at least have a
      // black background.
      for ( i = 0, p_visinfo = vis_infos; i < nVisuals; i++, p_visinfo++ ) {
         if ( p_visinfo->depth >= 8 ) {
            mVisual = p_visinfo;
            break;
         }
      }

      // If we couldn't find a visual with at least 8-bit color, just use the
      // first one in the list.
      if ( i == nVisuals ) {
          mVisual = vis_infos;
      }
   }
   // If we didn't get a matching visual, we're in trouble.
   else {
      vprDEBUG(vprDBG_ERROR,vprDBG_CRITICAL_LVL) <<  clrOutNORM(clrRED,"ERROR:")
                  << "gadget::EventWindow::startSampling() : find visual failed"
                  << std::endl << vprDEBUG_FLUSH;
      return 0;
   }

   mSWA.colormap = XCreateColormap(mDisplay,
                                   RootWindow(mDisplay, mVisual->screen),
                                   mVisual->visual, AllocNone);
   // Try to make it just a black window
   mSWA.border_pixel = WhitePixel(mDisplay, mScreen);
   unsigned long event_mask = ExposureMask | StructureNotifyMask | KeyPressMask |
                              KeyReleaseMask | ButtonPressMask | ButtonReleaseMask |
                              ButtonMotionMask | PointerMotionMask | StructureNotifyMask;
   mSWA.event_mask = event_mask;
   mSWA.background_pixel = BlackPixel(mDisplay, mScreen);

   mWindow = createWindow(DefaultRootWindow(mDisplay), 1,
                          BlackPixel(mDisplay, mScreen),
                          WhitePixel(mDisplay, mScreen), event_mask);

   setHints(mWindow, const_cast<char*>(mInstName.c_str()),
            "VJm_keys" , "VJEventWindow2", "VJInputD");

   XSelectInput(mDisplay, mWindow, event_mask);
   XMapWindow(mDisplay, mWindow);
   XFlush(mDisplay);
   XRaiseWindow(mDisplay, mWindow);
   XClearWindow(mDisplay, mWindow);    // Try to clear the background

   vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_STATE_LVL)
      << "gadget::EventWindowXWin::openTheWindow() : done." << std::endl
      << vprDEBUG_FLUSH;

   XFree (vis_infos);

   return 1;
}



/* Sets basic window manager hints for a window. */
void EventWindowXWin::setHints(Window window, char* window_name,
                               char* icon_name, char* class_name,
                               char* class_type)
{
    XTextProperty  w_name;
    XTextProperty  i_name;
    XSizeHints     sizehints;
    XWMHints       wmhints;
    XClassHint     classhints;
    int            status;

    /*
     * Generate window and icon names.
     */
   status = XStringListToTextProperty(&window_name, 1, &w_name);

   if(0 == status)
   {
      vprDEBUG(vprDBG_ALL, 0) << "Error allocating XString\n" << vprDEBUG_FLUSH;
   }

   status = XStringListToTextProperty(&icon_name, 1, &i_name);

   if(0 == status)
   {
      vprDEBUG(vprDBG_ALL, 0) << "Error allocating XString\n" << vprDEBUG_FLUSH;
   }

   sizehints.width       = mWidth;    /* -- Obsolete in R4 */
   sizehints.height      = mHeight;   /* -- Obsolete in R4 */
   sizehints.base_width  = mWidth;    /* -- New in R4 */
   sizehints.base_height = mHeight;   /* -- New in R4 */

   /* Set up flags for all the fields we've filled in. */
   sizehints.flags = USPosition | USSize | PBaseSize;

   /*   assume the window starts in "normal" (rather than
    *    iconic) state and wants keyboard input.
    */
   wmhints.initial_state = NormalState;
   wmhints.input         = True;
   wmhints.flags         = StateHint | InputHint;

   /* Fill in class name. */
   classhints.res_name  = class_name;
   classhints.res_class = class_type;

   XSetWMProperties(mDisplay, window, &w_name, &i_name,
                    //argv, argc, /* Note reversed order. */
                    NULL, 0,
                    &sizehints, &wmhints, &classhints);

   XFree(w_name.value);
   XFree(i_name.value);
}

Window EventWindowXWin::createWindow(Window parent, unsigned int border,
                                     unsigned long fore, unsigned long back,
                                     unsigned long event_mask)
{
   Window window;
   //UnUsed// XSetWindowAttributes attributes;
   //UnUsed// unsigned long attribute_mask;

   // set up attributes
   //UnUsed// attributes.background_pixel = back;
   //UnUsed// attributes.border_pixel = fore;
   //UnUsed// attributes.event_mask = event_mask;
   //UnUsed// attribute_mask = CWBackPixel | CWBorderPixel | CWEventMask;

   // need screen size so we can convert origin from lower-left
   XWindowAttributes winattrs;
   XGetWindowAttributes(mDisplay, RootWindow(mDisplay, DefaultScreen(mDisplay)),
                        &winattrs);

   // create it
   window = XCreateWindow(mDisplay, parent, mX, winattrs.height - mY - mHeight,
                          mWidth, mHeight, border, mVisual->depth,
                          InputOutput, mVisual->visual,
                          CWBorderPixel | CWColormap | CWEventMask, &mSWA);

   return window;
}

// Called when locking states
// - Recenter the mouse
void EventWindowXWin::lockMouse()
{
   vprDEBUG(gadgetDBG_INPUT_MGR,vprDBG_STATE_LVL)
      << "gadget::EventWindowXWin: LOCKING MOUSE..." << vprDEBUG_FLUSH;

   // Center the mouse
   int win_center_x(mWidth/2), win_center_y(mHeight/2);
   XWarpPointer(mDisplay, None, mWindow, 0,0, 0,0, win_center_x, win_center_y);

   // Grab the keyboard input so that holding down a key works even
   // if the window loses focus.  While the keyboard is grabbed,
   // keyboard and pointer events will be processed normally
   // (GrabModeAsync).
   XGrabKeyboard(mDisplay, mWindow, True, GrabModeAsync, GrabModeAsync,
                 CurrentTime);

   // While the pointer is grabbed, we will watch for the events in
   // event_mask, and keyboard and pointer events will be processed
   // normally (GrabModeAsync).
   unsigned int event_mask;
   event_mask = ButtonPressMask | ButtonReleaseMask |
                PointerMotionMask | ButtonMotionMask;

   XGrabPointer(mDisplay, mWindow, True, event_mask, GrabModeAsync,
                GrabModeAsync, None, None, CurrentTime);

   vprDEBUG_CONT(gadgetDBG_INPUT_MGR,vprDBG_STATE_LVL)
      << "lock finished.\n" << vprDEBUG_FLUSH;
}

// Called when locking ends
void EventWindowXWin::unlockMouse()
{
   vprDEBUG(gadgetDBG_INPUT_MGR,vprDBG_STATE_LVL)
      << "gadget::EventWindowXWin: UN-LOCKING MOUSE..." << vprDEBUG_FLUSH;

   // Un-grab the keyboard now
   XUngrabKeyboard(mDisplay, CurrentTime);

   // Un-grab the pointer as well
   XUngrabPointer(mDisplay, CurrentTime);

   vprDEBUG_CONT(gadgetDBG_INPUT_MGR,vprDBG_STATE_LVL)
      << "un-lock finished.\n" << vprDEBUG_FLUSH;
}

} // end namespace
