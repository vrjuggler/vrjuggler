/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998, 1999, 2000 by Iowa State University
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

#include <vrj/Util/Debug.h>
#include <vrj/Display/DisplayManager.h>
#include <jccl/Config/ConfigChunk.h>

#include <gadget/Devices/Keyboard/KeyboardXWin.h> // my header

namespace gadget
{

//: Constructor
bool KeyboardXWin::config(ConfigChunk *c)
{
   if(! (Input::config(c) && Keyboard::config(c)))
      return false;

   const char neg_one_STRING[] = "-1";

   // Done in Input --- myThread = NULL;
   int i;
   for (i =0; i < 256; i++)
      m_realkeys[i] = m_keys[i] = 0;
   m_realkeys[0] = m_keys[0] = 1;

   // Get size and position
   m_width = (int)c->getProperty("width");
   m_height = (int)c->getProperty("height");
   if (m_width == 0) m_width = 400;
   if (m_height == 0) m_height = 400;

   m_x = c->getProperty("origin", 0);
   m_y = c->getProperty("origin", 1);

   // Get the X display string
   int x_disp_num = c->getProperty("display_number");
   ConfigChunk* dispSysChunk = DisplayManager::instance()->getDisplaySystemChunk();

   if ((x_disp_num >= 0) && dispSysChunk)
      mXDisplayString = (std::string)dispSysChunk->getProperty("xpipes", x_disp_num);
   else
      mXDisplayString = std::string("-1");

   if ((mXDisplayString.empty()) || (strcmp(mXDisplayString.c_str(), neg_one_STRING) == 0))    // Use display env
   {
      const std::string DISPLAY_str("DISPLAY");
      mXDisplayString = (std::string)getenv(DISPLAY_str.c_str());
   }

   // Get the lock information
   mLockToggleKey = c->getProperty("lock_key");
   bool start_locked = c->getProperty("start_locked");
   if (start_locked)
      mLockState = Lock_LockKey;      // Initialize to the locked state

   m_mouse_sensitivity = c->getProperty("msens");
   if (0.0f == m_mouse_sensitivity)
      m_mouse_sensitivity = 0.5;

   vprDEBUG(vrjDBG_INPUT_MGR, vprDBG_STATE_LVL) << "Mouse Sensititivty: "
   << m_mouse_sensitivity << std::endl << vprDEBUG_FLUSH;

   mSleepTimeMS = c->getProperty("sleep_time");

   // HACK: Use a default time until config file has defaults
   if (mSleepTimeMS == 0)
      mSleepTimeMS = 50;

   // Default to owning the window
   mWeOwnTheWindow = true;

   return true;
}

// Main thread of control for this active object
void KeyboardXWin::controlLoop(void* nullParam)
{
   vprDEBUG(vrjDBG_INPUT_MGR,vprDBG_STATE_LVL)
      << "gadget::KeyboardXWin::controlLoop: Thread started.\n"
      << vprDEBUG_FLUSH;

   while (NULL == vpr::Thread::self())
   {
      vpr::System::usleep(50);
      vprDEBUG(vprDBG_ALL,vprDBG_VERB_LVL)
         << "gadget::KeyboardXWin: Waiting for (thread::self() != NULL)\n"
         << vprDEBUG_FLUSH;
   }
   myThread = (vpr::Thread*) vpr::Thread::self();

   // Open the x-window
   if(mWeOwnTheWindow)
      openTheWindow();

   // Sync up with window
   mXfuncLock.acquire();
   XSync(m_display,0);
   mXfuncLock.release();

   // If we have initial locked, then we need to lock the system
   if(mLockState == Lock_LockKey)      // Means that we are in the initially locked state
   {
      vprDEBUG(vrjDBG_INPUT_MGR,vprDBG_STATE_LVL)
         << "gadget::KeyboardXWin::controlLoop: Mouse set to initial lock. Locking it now.\n"
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
   if(mWeOwnTheWindow)
   {
      mXfuncLock.acquire();
      XDestroyWindow(m_display,m_window);
      XCloseDisplay((::Display*) m_display);
      mXfuncLock.release();
   }
}


int KeyboardXWin::startSampling()
{
   if(myThread != NULL)
   {
      vprDEBUG(vprDBG_ERROR,vprDBG_CRITICAL_LVL)
         << clrOutNORM(clrRED,"ERROR:")
         << "gadget::KeyboardXWin: startSampling called, when already sampling.\n"
         << vprDEBUG_FLUSH;
      vprASSERT(false);
   }

   resetIndexes();      // Reset the buffering variables

   // Create a new thread to handle the control
   vpr::ThreadMemberFunctor<KeyboardXWin>* memberFunctor =
      new vpr::ThreadMemberFunctor<KeyboardXWin>(this, &KeyboardXWin::controlLoop, NULL);

   vpr::Thread* new_thread;
   new_thread = new vpr::Thread(memberFunctor);
   myThread = new_thread;

   return 1;
}

int KeyboardXWin::onlyModifier(int mod)
{
  switch (mod) {
     case VJKEY_NONE:
        return (!m_curKeys[VJKEY_SHIFT] && !m_curKeys[VJKEY_CTRL] && !m_curKeys[VJKEY_ALT]);
     case VJKEY_SHIFT:
        return (m_curKeys[VJKEY_SHIFT] && !m_curKeys[VJKEY_CTRL] && !m_curKeys[VJKEY_ALT]);
     case VJKEY_CTRL:
        return (!m_curKeys[VJKEY_SHIFT] && m_curKeys[VJKEY_CTRL] && !m_curKeys[VJKEY_ALT]);
     case VJKEY_ALT:
        return (!m_curKeys[VJKEY_SHIFT] && !m_keys[VJKEY_CTRL] && m_curKeys[VJKEY_ALT]);
     default:
       vprASSERT(false);
       return 0;
  }
}

void KeyboardXWin::updateData()
{
vpr::Guard<vpr::Mutex> guard(mKeysLock);      // Lock access to the m_keys array
   if(mHandleEventsHasBeenCalled)            // If we haven't updated anything, then don't swap stuff
   {
      mHandleEventsHasBeenCalled = false;

      // Scale mouse values based on sensitivity
      m_keys[VJMOUSE_POSX] = int(float(m_keys[VJMOUSE_POSX]) * m_mouse_sensitivity);
      m_keys[VJMOUSE_NEGX] = int(float(m_keys[VJMOUSE_NEGX]) * m_mouse_sensitivity);
      m_keys[VJMOUSE_POSY] = int(float(m_keys[VJMOUSE_POSY]) * m_mouse_sensitivity);
      m_keys[VJMOUSE_NEGY] = int(float(m_keys[VJMOUSE_NEGY]) * m_mouse_sensitivity);

      /*
      vprDEBUG(vprDBG_ALL,0)   << "gadget::KeyboardXWin::updateData:" << instName << " -- "
                             << "mouse_keys: px:" << m_keys[VJMOUSE_POSX]
                                        << " nx:" << m_keys[VJMOUSE_NEGX]
                                        << " py:" << m_keys[VJMOUSE_POSY]
                                        << " ny:" << m_keys[VJMOUSE_NEGY]
                                        << std::endl << vprDEBUG_FLUSH;
      */

      // Copy over values
      for(unsigned int i=0;i<256;i++)
         m_curKeys[i] = m_keys[i];

      // Re-initialize the m_keys based on current key state in realKeys
      // Set the initial state of the m_key key counts based on the current state of the system
      for(unsigned int j = 0; j < 256; j++)
         m_keys[j] = m_realkeys[j];
   }
}

void KeyboardXWin::HandleEvents()
{
   XEvent event;
   KeySym key;
   int    vj_key;          // The key in vj space
   bool have_more_events;  // Used by the while loop below

   // XXX: Need to add to mask to get more events for subclasses
   const long event_mask = (KeyPressMask | KeyReleaseMask | ButtonPressMask |
                             ButtonReleaseMask | ButtonMotionMask |
                             PointerMotionMask | StructureNotifyMask);
   mKeysLock.acquire();
   mHandleEventsHasBeenCalled = true;       // We have been called now -- Tell them to swap data because we don't have events
   mKeysLock.release();

   // Wait until we actually have some events -- THIS BLOCKS for an event
   mXfuncLock.acquire();
   XWindowEvent(m_display, m_window, event_mask, &event);
   mXfuncLock.release();

// GUARD m_keys for duration of loop
// Doing it here gives makes sure that we process all events and don't get only part of them for an update
// In order to copy data over to the m_curKeys array
vpr::Guard<vpr::Mutex> guard(mKeysLock);      // Lock access to the m_keys array for the duration of this function

   mHandleEventsHasBeenCalled = true;       // We have been called now - Tell tehm to swap because we do have data
   // Loop while the event queue contains events for m_window that are part
   // of the given event mask.
   do
   {
      switch (event.type)
      {
      // A KeyPress event occurred.  Flag the key that was pressed (as a
      // VJ key) as being pressed and grab the keyboard.
      case KeyPress:
         // Convert the pressed key from the event to a VJ key.
         // And store in the array of key presses
         key = XLookupKeysym((XKeyEvent*)&event,0);
         vj_key = xKeyToKey(key);
         m_realkeys[vj_key] = 1;
         m_keys[vj_key] += 1;

         // -- Update lock state -- //
         // Any[key == ESC]/unlock(ifneeded) -> Unlocked
         // Unlocked[key!=lockKey]/lockMouse -> lock_keydown
         // Unlocked[key==lockKey]/lockMouse -> lock_keylock
         // lock_keydown[key==lockKey] -> lock_keylock
         // lock_keylock[key==lockKey] -> Unlocked
         if (vj_key == VJKEY_ESC)       // Check for Escape from bad state
         {
            vprDEBUG(vrjDBG_INPUT_MGR,vprDBG_STATE_LVL)
               << "gadget::KeyboardXWin: Trying to ESCAPE from current state.\n"
               << vprDEBUG_FLUSH;
            if(mLockState != Unlocked)
            {
               vprDEBUG_NEXT(vrjDBG_INPUT_MGR,vprDBG_STATE_LVL)
                  << "gadget::KeyboardXWin: STATE switch: <ESCAPE> --> Unlocked\n"
                  << vprDEBUG_FLUSH;
               mLockState = Unlocked;
               unlockMouse();
            }
            else
            {
               vprDEBUG_NEXT(vrjDBG_INPUT_MGR,vprDBG_STATE_LVL)
                  << "gadget::KeyboardXWin: Already unlocked.  Cannot ESCAPE."
                  << vprDEBUG_FLUSH;
            }
         }
         else if(mLockState == Unlocked)
         {
            if(vj_key != mLockToggleKey)
            {
               mLockState = Lock_KeyDown;       // Switch state
               mLockStoredKey = vj_key;         // Store the VJ key that is down
               vprDEBUG(vrjDBG_INPUT_MGR,vprDBG_STATE_LVL)
                  << "gadget::KeyboardXWin: STATE switch: Unlocked --> Lock_KeyDown\n"
                  << vprDEBUG_FLUSH;
               lockMouse();
            }
            else if(vj_key == mLockToggleKey)
            {
               mLockState = Lock_LockKey;
               vprDEBUG(vrjDBG_INPUT_MGR,vprDBG_STATE_LVL)
                  << "gadget::KeyboardXWin: STATE switch: Unlocked --> Lock_LockKey\n"
                  << vprDEBUG_FLUSH;
               lockMouse();
            }
         }
         else if((mLockState == Lock_KeyDown) && (vj_key == mLockToggleKey))     // Just switch the current locking state
         {
            mLockState = Lock_LockKey;
            vprDEBUG(vrjDBG_INPUT_MGR,vprDBG_STATE_LVL)
               << "gadget::KeyboardXWin: STATE switch: Lock_KeyDown --> Lock_LockKey\n"
               << vprDEBUG_FLUSH;
         }
         else if((mLockState == Lock_LockKey) && (vj_key == mLockToggleKey))
         {
            mLockState = Unlocked;
            vprDEBUG(vrjDBG_INPUT_MGR,vprDBG_STATE_LVL)
               << "gadget::KeyboardXWin: STATE switch: Lock_LockKey --> Unlocked\n"
               << vprDEBUG_FLUSH;
            unlockMouse();
         }

         vprDEBUG(vrjDBG_INPUT_MGR, vprDBG_HVERB_LVL) << "KeyPress:  " << std::hex
                    << key << " state:" << ((XKeyEvent*)&event)->state
                    << " ==> " << xKeyToKey(key) << std::endl
                    << vprDEBUG_FLUSH;
         break;

      // A KeyRelease event occurred.  Flag the key that was released (as a
      // VJ key) as being not pressed and ungrab the keyboard.
      case KeyRelease:
         // Convert the released key from the event to a VJ key.
         key = XLookupKeysym((XKeyEvent*)&event,0);
         vj_key = xKeyToKey(key);
         m_realkeys[vj_key] = 0;

         // -- Update lock state -- //
         // lock_keyDown[key==storedKey]/unlockMouse -> unlocked
         if((mLockState == Lock_KeyDown) && (vj_key == mLockStoredKey))
         {
            mLockState = Unlocked;
            vprDEBUG(vrjDBG_INPUT_MGR,vprDBG_STATE_LVL)
               << "gadget::KeyboardXWin: STATE switch: Lock_KeyDown --> Unlocked\n"
               << vprDEBUG_FLUSH;
            unlockMouse();
         }

         vprDEBUG(vrjDBG_INPUT_MGR, vprDBG_HVERB_LVL) << "KeyRelease:" << std::hex
                    << key << " state:" << ((XKeyEvent*)&event)->state
                    << " ==> " << xKeyToKey(key) << std::endl
                    << vprDEBUG_FLUSH;
         break;

      // A MotionNotify event (mouse pointer movement) occurred.
      // If motion occurs within the window, determine how far the pointer
      // moved since the last time anything was read.
      case MotionNotify:
         {
            int win_center_x(m_width/2),win_center_y(m_height/2);

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

                  mXfuncLock.acquire();
                  XWarpPointer(m_display, None, m_window, 0,0, 0,0,
                               win_center_x, win_center_y);
                  mXfuncLock.release();
               }
            }

            // Update m_keys based on key pressed and store in the key array
            if ( dx > 0 ) {
               m_keys[VJMOUSE_POSX] += dx;      // Positive movement in the x direction.
            } else {
               m_keys[VJMOUSE_NEGX] += -dx;     // Negative movement in the x direction.
            }

            if ( dy > 0 ) {
               m_keys[VJMOUSE_POSY] += dy;      // Positive movement in the y direction.
            } else {
               m_keys[VJMOUSE_NEGY] += -dy;     // Negative movement in the y direction.
            }
         }

         break;

      // A mouse button was pressed.  We set that button to 1 in m_keys AND
      // in m_realkeys so that it will be remembered as being held down
      // untill a ButtonRelease event occurs.
      case ButtonPress:
         switch ( event.xbutton.button )
         {
         case Button1:
            m_realkeys[VJMBUTTON1] = m_keys[VJMBUTTON1] = 1;
            break;
         case Button2:
            m_realkeys[VJMBUTTON2] = m_keys[VJMBUTTON2] = 1;
            break;
         case Button3:
            m_realkeys[VJMBUTTON3] = m_keys[VJMBUTTON3] = 1;
            break;
         }

         break;

      // A mouse button was released.
      case ButtonRelease:
         switch ( event.xbutton.button )
         {
         case Button1:
            m_realkeys[VJMBUTTON1] = m_keys[VJMBUTTON1] = 0;
            break;
         case Button2:
            m_realkeys[VJMBUTTON2] = m_keys[VJMBUTTON2] = 0;
            break;
         case Button3:
            m_realkeys[VJMBUTTON3] = m_keys[VJMBUTTON3] = 0;
            break;
         }

         break;
      }

      // Let any other event watchers process their events
      this->processEvent(event);

      mXfuncLock.acquire();
      have_more_events = XCheckWindowEvent(m_display, m_window, event_mask, &event);
      mXfuncLock.release();
   }
   while ( have_more_events );

}

int KeyboardXWin::stopSampling()
{
  if (myThread != NULL)
  {
    mExitFlag = true;
    vpr::System::sleep(1);
  }

  return 1;
}

int KeyboardXWin::xKeyToKey(KeySym xKey)
{
   switch (xKey)
   {
   case XK_Up        : return VJKEY_UP;
   case XK_Down      : return VJKEY_DOWN;
   case XK_Left      : return VJKEY_LEFT;
   case XK_Right     : return VJKEY_RIGHT;
   case XK_Control_L : return VJKEY_CTRL;
   case XK_Control_R : return VJKEY_CTRL;
   case XK_Shift_L   : return VJKEY_SHIFT;
   case XK_Shift_R   : return VJKEY_SHIFT;
   case XK_Alt_L     : return VJKEY_ALT;
   case XK_Alt_R     : return VJKEY_ALT;

      // Map all number keys
      // Note we map keypad and normal keys making no distinction
   case XK_1            : return VJKEY_1;
   case XK_KP_End       : return VJKEY_1;
   case XK_2            : return VJKEY_2;
   case XK_KP_Down      : return VJKEY_2;
   case XK_3            : return VJKEY_3;
   case XK_KP_Page_Down : return VJKEY_3;
   case XK_4            : return VJKEY_4;
   case XK_KP_Left      : return VJKEY_4;
   case XK_5            : return VJKEY_5;
   case XK_KP_Begin     : return VJKEY_5;
   case XK_6            : return VJKEY_6;
   case XK_KP_Right     : return VJKEY_6;
   case XK_7            : return VJKEY_7;
   case XK_KP_Home      : return VJKEY_7;
   case XK_8            : return VJKEY_8;
   case XK_KP_Up        : return VJKEY_8;
   case XK_9            : return VJKEY_9;
   case XK_KP_Page_Up   : return VJKEY_9;
   case XK_0            : return VJKEY_0;
   case XK_KP_Insert    : return VJKEY_0;

   case XK_A         : return VJKEY_A;
   case XK_B         : return VJKEY_B;
   case XK_C         : return VJKEY_C;
   case XK_D         : return VJKEY_D;
   case XK_E         : return VJKEY_E;
   case XK_F         : return VJKEY_F;
   case XK_G         : return VJKEY_G;
   case XK_H         : return VJKEY_H;
   case XK_I         : return VJKEY_I;
   case XK_J         : return VJKEY_J;
   case XK_K         : return VJKEY_K;
   case XK_L         : return VJKEY_L;
   case XK_M         : return VJKEY_M;
   case XK_N         : return VJKEY_N;
   case XK_O         : return VJKEY_O;
   case XK_P         : return VJKEY_P;
   case XK_Q         : return VJKEY_Q;
   case XK_R         : return VJKEY_R;
   case XK_S         : return VJKEY_S;
   case XK_T         : return VJKEY_T;
   case XK_U         : return VJKEY_U;
   case XK_V         : return VJKEY_V;
   case XK_W         : return VJKEY_W;
   case XK_X         : return VJKEY_X;
   case XK_Y         : return VJKEY_Y;
   case XK_Z         : return VJKEY_Z;
   case XK_a         : return VJKEY_A;
   case XK_b         : return VJKEY_B;
   case XK_c         : return VJKEY_C;
   case XK_d         : return VJKEY_D;
   case XK_e         : return VJKEY_E;
   case XK_f         : return VJKEY_F;
   case XK_g         : return VJKEY_G;
   case XK_h         : return VJKEY_H;
   case XK_i         : return VJKEY_I;
   case XK_j         : return VJKEY_J;
   case XK_k         : return VJKEY_K;
   case XK_l         : return VJKEY_L;
   case XK_m         : return VJKEY_M;
   case XK_n         : return VJKEY_N;
   case XK_o         : return VJKEY_O;
   case XK_p         : return VJKEY_P;
   case XK_q         : return VJKEY_Q;
   case XK_r         : return VJKEY_R;
   case XK_s         : return VJKEY_S;
   case XK_t         : return VJKEY_T;
   case XK_u         : return VJKEY_U;
   case XK_v         : return VJKEY_V;
   case XK_w         : return VJKEY_W;
   case XK_x         : return VJKEY_X;
   case XK_y         : return VJKEY_Y;
   case XK_z         : return VJKEY_Z;
   case XK_Escape    : return VJKEY_ESC;
   default: return 255;
   }

}

/*****************************************************************/
/*****************************************************************/
/***********************X WINDOW STUFF****************************/
/*****************************************************************/
/*****************************************************************/
// Open the X window to sample from
int KeyboardXWin::openTheWindow()
{
vpr::Guard<vpr::Mutex> xguard(mXfuncLock);

   int i;

   m_display = XOpenDisplay(mXDisplayString.c_str());    // Open display on given XDisplay
   if (m_display == NULL)
   {
      vprDEBUG(vprDBG_ERROR,vprDBG_CRITICAL_LVL)
         <<  clrOutNORM(clrRED,"ERROR:")
         << "gadget::Keyboard::StartSampling() : failed to open display"
         << std::endl << vprDEBUG_FLUSH;
      return 0;
   }
   m_screen = DefaultScreen(m_display);

   XVisualInfo vTemplate, *vis_infos;
   long vMask = VisualScreenMask;
   vTemplate.screen = m_screen;
   int nVisuals;

   vis_infos = XGetVisualInfo( m_display, vMask, &vTemplate, &nVisuals);

   // Verify that we got at least one visual from XGetVisualInfo(3).
   if ( vis_infos != NULL && nVisuals >= 1 ) {
      XVisualInfo* p_visinfo;

      // Try to find a visual with color depth of at least 8 bits.  Having
      // such a visual ensures that the keyboard windows at least have a
      // black background.
      for ( i = 0, p_visinfo = vis_infos; i < nVisuals; i++, p_visinfo++ ) {
         if ( p_visinfo->depth >= 8 ) {
            m_visual = p_visinfo;
            break;
         }
      }

      // If we couldn't find a visual with at least 8-bit color, just use the
      // first one in the list.
      if ( i == nVisuals ) {
          m_visual = vis_infos;
      }
   }
   // If we didn't get a matching visual, we're in trouble.
   else {
      vprDEBUG(vprDBG_ERROR,vprDBG_CRITICAL_LVL) <<  clrOutNORM(clrRED,"ERROR:")
                  << "gadget::Keyboard::startSampling() : find visual failed"
                  << std::endl << vprDEBUG_FLUSH;
      return 0;
   }

   m_swa.colormap = XCreateColormap(m_display,
                                    RootWindow(m_display,m_visual->screen),
                                    m_visual->visual, AllocNone);
   // Try to make it just a black window
   m_swa.border_pixel = WhitePixel(m_display,m_screen);
   m_swa.event_mask = ExposureMask | StructureNotifyMask | KeyPressMask;
   m_swa.background_pixel = BlackPixel(m_display,m_screen);
   unsigned long event_mask = ExposureMask | StructureNotifyMask | KeyPressMask |
                              KeyReleaseMask | ButtonPressMask | ButtonReleaseMask |
                              ButtonMotionMask | PointerMotionMask | StructureNotifyMask;
   m_window = createWindow ( DefaultRootWindow(m_display) ,
                             1,
                             BlackPixel(m_display,m_screen),
                             WhitePixel(m_display, m_screen),
                             event_mask );

   setHints(m_window, const_cast<char*>(instName.c_str()) , "VJm_keys" , "VJKeyboard2", "VJInputD" );

   XSelectInput(m_display, m_window,
                event_mask );
   XMapWindow(m_display, m_window);
   XFlush(m_display);
   XRaiseWindow(m_display,m_window);
   XClearWindow(m_display,m_window);    // Try to clear the background

   vprDEBUG(vrjDBG_INPUT_MGR, vprDBG_STATE_LVL)
              << "gadget::KeyboardXWin::openTheWindow() : done." << std::endl
              << vprDEBUG_FLUSH;

   XFree (vis_infos);

   return 1;
}



/* Sets basic window manager hints for a window. */
void KeyboardXWin::setHints(Window window,
    char*  window_name,
    char*  icon_name,
    char*  class_name,
    char*  class_type)

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
   status = XStringListToTextProperty(&window_name,
         1, /* 1 string to convert */
         &w_name);

   status = XStringListToTextProperty(&icon_name,
         1, /* 1 string to convert */
         &i_name);

    sizehints.width       = m_width;    /* -- Obsolete in R4 */
    sizehints.height      = m_height;   /* -- Obsolete in R4 */
    sizehints.base_width  = m_width;    /* -- New in R4 */
    sizehints.base_height = m_height;   /* -- New in R4 */


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

    XSetWMProperties(m_display, window,
        &w_name,
        &i_name,
        //argv, argc, /* Note reversed order. */
        NULL,0,
   &sizehints,
        &wmhints,
        &classhints);

   XFree(w_name.value);
   XFree(i_name.value);

}

Window KeyboardXWin::createWindow (Window parent, unsigned int border, unsigned long
     fore, unsigned long back, unsigned long event_mask)
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
  XGetWindowAttributes (m_display, RootWindow (m_display, DefaultScreen(m_display)), &winattrs);

  // create it
  window = XCreateWindow( m_display, parent,
           m_x, winattrs.height - m_y - m_height,
           m_width,m_height, border,
           m_visual->depth, /*depth*/
           InputOutput, /*window class*/
           m_visual->visual,CWBorderPixel|CWColormap|CWEventMask, &m_swa);

  return window;
} /*CreateWindow*/


// Called when locking states
// - Recenter the mouse
void KeyboardXWin::lockMouse()
{
vpr::Guard<vpr::Mutex> xguard(mXfuncLock);

   vprDEBUG(vrjDBG_INPUT_MGR,vprDBG_STATE_LVL)
      << "gadget::KeyboardXWin: LOCKING MOUSE..." << vprDEBUG_FLUSH;

   // Center the mouse
   int win_center_x(m_width/2),win_center_y(m_height/2);
   XWarpPointer(m_display, None, m_window, 0,0, 0,0, win_center_x, win_center_y);

   // Grab the keyboard input so that holding down a key works even
   // if the window loses focus.  While the keyboard is grabbed,
   // keyboard and pointer events will be processed normally
   // (GrabModeAsync).
   XGrabKeyboard(m_display, m_window, True,
                GrabModeAsync, GrabModeAsync,
                                  CurrentTime);

   // While the pointer is grabbed, we will watch for the events in
   // event_mask, and keyboard and pointer events will be processed
   // normally (GrabModeAsync).
   unsigned int event_mask;
   event_mask = ButtonPressMask | ButtonReleaseMask |
                PointerMotionMask | ButtonMotionMask;

   XGrabPointer(m_display, m_window, True,
                event_mask, GrabModeAsync,
                GrabModeAsync, None, None,
                               CurrentTime);

   vprDEBUG_CONT(vrjDBG_INPUT_MGR,vprDBG_STATE_LVL) << "lock finished.\n" << vprDEBUG_FLUSH;
}

// Called when locking ends
void KeyboardXWin::unlockMouse()
{
vpr::Guard<vpr::Mutex> xguard(mXfuncLock);

   vprDEBUG(vrjDBG_INPUT_MGR,vprDBG_STATE_LVL)
      << "gadget::KeyboardXWin: UN-LOCKING MOUSE..." << vprDEBUG_FLUSH;

   // Un-grab the keyboard now
   XUngrabKeyboard(m_display, CurrentTime);

   // Un-grab the pointer as well
   XUngrabPointer(m_display, CurrentTime);

   vprDEBUG_CONT(vrjDBG_INPUT_MGR,vprDBG_STATE_LVL) << "un-lock finished.\n" << vprDEBUG_FLUSH;
}

} // end namespace
