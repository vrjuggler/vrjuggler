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

#include <vrj/vrjConfig.h>

#include <vpr/Thread/Thread.h>
#include <vpr/System.h>

#include <vrj/Utils/Debug.h>
#include <vrj/Display/DisplayManager.h>
#include <vrj/Config/ConfigChunk.h>
#include <ApplicationServices/ApplicationServices.h>

#include <vrj/Input/Devices/Keyboard/KeyboardOSX.h>

namespace vrj
{

   //: Constructor
bool KeyboardOSX::config(ConfigChunk *c)
{
   if(! (Input::config(c) && Keyboard::config(c)))
      return false;

   // Done in Input --- myThread = NULL;
   int i;
   for (i =0; i < 256; i++)
      m_realkeys[i] = m_keys[i] = 0;
   m_realkeys[0] = m_keys[0] = 0;

   window_title = CFStringCreateWithCString(NULL, static_cast<std::string>(c->getProperty("name")).c_str(), kCFStringEncodingMacRoman);

   // Get size and position
   m_width = (int)c->getProperty("width");
   m_height = (int)c->getProperty("height");
   if (m_width == 0) m_width = 400;
   if (m_height == 0) m_height = 400;

   m_x = c->getProperty("origin", 0);
   m_y = c->getProperty("origin", 1);

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
void KeyboardOSX::controlLoop(void* nullParam)
{
    Point   mouseLoc;
    Rect    win_rect;
    long usleep_time(1); // to be set...
    vprDEBUG(vrjDBG_INPUT_MGR,vprDBG_STATE_LVL) << "vjKeyboardOSX::controlLoop: Thread started.\n" << vprDEBUG_FLUSH;

    // Open the carbon window
    if(mWeOwnTheWindow)
        openTheWindow();

    // Record the initial Mouse location
    GetGlobalMouse(&mouseLoc);
    mPrevX = mouseLoc.h;
    mPrevY = mouseLoc.v;
    mSnapTimer = 0;

    // If we have initial locked, then we need to lock the system
    if(mLockState == Lock_LockKey)      // Means that we are in the initially locked state
    {
    vprDEBUG(vrjDBG_INPUT_MGR,vprDBG_STATE_LVL) << "vjKeyboardOSX::controlLoop: Mouse set to initial lock. Locking it now.\n" << vprDEBUG_FLUSH;
    lockMouse();                     // Lock the mouse
    }

    // Loop on updating
    while(!mExitFlag)
    {
        //Only take input when the window is in front!
        if(gpWindow == FrontWindow())
            sample();

    usleep_time = mSleepTimeMS*1000;
    vpr::System::usleep(usleep_time);

        // This will update the window location/size information.  Which is needed to correctly lock and track the mouse
        if(mLockState == Unlocked)
        {
            GetWindowBounds ( gpWindow, kWindowContentRgn, &win_rect);
            m_y = win_rect.top;
            m_x = win_rect.left;
            m_height = win_rect.bottom - m_y;
            m_width = win_rect.right - m_x;
        }
    }

    // Exit, cleanup code
    if(mWeOwnTheWindow)
    {
    DisposeWindow (gpWindow); //Close the window
    }
}


int KeyboardOSX::startSampling()
{
   if(myThread != NULL)
   {
      vprDEBUG(vprDBG_ERROR,vprDBG_CRITICAL_LVL) << clrOutNORM(clrRED,"ERROR:")
                                              << "vjKeyboardOSX: startSampling called, when already sampling.\n" << vprDEBUG_FLUSH;
      vprASSERT(false);
   }

   resetIndexes();      // Reset the buffering variables

   // Create a new thread to handle the control
   vpr::ThreadMemberFunctor<KeyboardOSX>* memberFunctor =
      new vpr::ThreadMemberFunctor<KeyboardOSX>(this, &KeyboardOSX::controlLoop, NULL);

   vpr::Thread* new_thread;
   new_thread = new vpr::Thread(memberFunctor);
   myThread = new_thread;

   return 1;
}

int KeyboardOSX::onlyModifier(int mod)
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

void KeyboardOSX::updateData()
{
    vpr::Guard<vpr::Mutex> guard(mKeysLock);      // Lock access to the m_keys array
    // Scale mouse values based on sensitivity
    if(mHandleEventsHasBeenCalled)            // If we haven't updated anything, then don't swap stuff
    {
    mHandleEventsHasBeenCalled = false;
    m_keys[VJMOUSE_POSX] = int(float(m_keys[VJMOUSE_POSX]) * m_mouse_sensitivity);
    m_keys[VJMOUSE_NEGX] = int(float(m_keys[VJMOUSE_NEGX]) * m_mouse_sensitivity);
    m_keys[VJMOUSE_POSY] = int(float(m_keys[VJMOUSE_POSY]) * m_mouse_sensitivity);
    m_keys[VJMOUSE_NEGY] = int(float(m_keys[VJMOUSE_NEGY]) * m_mouse_sensitivity);

        // Copy over values
        for(unsigned int i=0;i<256;i++)
            m_curKeys[i] = m_keys[i];

        // Re-initialize the m_keys based on current key state in realKeys
        // Set the initial state of the m_key key counts based on the current state of the system
        for(unsigned int j = 0; j < 256; j++)
            m_keys[j] = m_realkeys[j];
    }

}

void KeyboardOSX::HandleEvents()
{
    vpr::Guard<vpr::Mutex> guard(mKeysLock);      // Lock access to the m_keys array for the duration of this function
    int vj_key = 255;
    int OSXKey = -1;
    int i = 0, j = 0;
    KeyMap  myKeys;

    Point   mouseLoc;
    CGPoint new_loc;
    int cur_x, cur_y, dx, dy;

    GetKeys(myKeys);
    for(i = 0; i < 4; i++) for(j = 31; j >= 0; j--) {
        OSXKey = (32*i+32-j)-1;     // Find out an index value for the key pressed (unique)
        vj_key = OSXKeyToKey(OSXKey);   //Remap the value to the VR Juggler Keymap Values

        // Now test to see if the key is pressed.
        if((myKeys[i]>>j)&0x0001) {
            //Key is currently pressed...
            m_realkeys[vj_key] = 1;
            m_keys[vj_key] += 1;
            if (vj_key == VJKEY_ESC)       // Check for Escape from bad state
            {
                vprDEBUG(vrjDBG_INPUT_MGR,vprDBG_STATE_LVL) << "vjKeyboardOSX: Trying to ESCAPE from current state.\n" << vprDEBUG_FLUSH;
                if(mLockState != Unlocked)
                {
                    vprDEBUG_NEXT(vrjDBG_INPUT_MGR,vprDBG_STATE_LVL) << "vjKeyboardOSX: STATE switch: <ESCAPE> --> Unlocked\n" << vprDEBUG_FLUSH;
                    mLockState = Unlocked;
                    unlockMouse();
                }
                else
                {
                    vprDEBUG_NEXT(vrjDBG_INPUT_MGR,vprDBG_STATE_LVL) << "vjKeyboardOSX: Already unlocked.  Cannot ESCAPE." << vprDEBUG_FLUSH;
                }
            }
            else if(mLockState == Unlocked)
            {
                if(vj_key != mLockToggleKey)
                {
                    mLockState = Lock_KeyDown;       // Switch state
                    mLockStoredKey = vj_key;         // Store the VJ key that is down
                    vprDEBUG(vrjDBG_INPUT_MGR,vprDBG_STATE_LVL) << "vjKeyboardOSX: STATE switch: Unlocked --> Lock_KeyDown\n" << vprDEBUG_FLUSH;
                    lockMouse();
                }
                else if(vj_key == mLockToggleKey)
                {
                    mLockState = Lock_LockKey;
                    vprDEBUG(vrjDBG_INPUT_MGR,vprDBG_STATE_LVL) << "vjKeyboardOSX: STATE switch: Unlocked --> Lock_LockKey\n" << vprDEBUG_FLUSH;
                    lockMouse();
                }
            }
            else if((mLockState == Lock_KeyDown) && (vj_key == mLockToggleKey))     // Just switch the current locking state
            {
                mLockState = Lock_LockKey;
                vprDEBUG(vrjDBG_INPUT_MGR,vprDBG_STATE_LVL) << "vjKeyboardOSX: STATE switch: Lock_KeyDown --> Lock_LockKey\n" << vprDEBUG_FLUSH;
            }
            else if((mLockState == Lock_LockKey) && (vj_key == mLockToggleKey))
            {
                mLockState = Unlocked;
                vprDEBUG(vrjDBG_INPUT_MGR,vprDBG_STATE_LVL) << "vjKeyboardOSX: STATE switch: Lock_LockKey --> Unlocked\n" << vprDEBUG_FLUSH;
            }

        } else {
            m_realkeys[vj_key] = 0;
            if((mLockState == Lock_KeyDown) && (vj_key == mLockStoredKey))
            {
                mLockState = Unlocked;
                vprDEBUG(vrjDBG_INPUT_MGR,vprDBG_STATE_LVL) << "vjXWinKeyboard: STATE switch: Lock_KeyDown --> Unlocked\n" << vprDEBUG_FLUSH;
            }
        }
    }

    // Determine how far the mouse pointer moved since the last event.
    // event.xmotion.x & y are relative to the x window
    GetGlobalMouse(&mouseLoc);
    cur_x = mouseLoc.h;
    cur_y = mouseLoc.v;

    if(mLockState == Unlocked)
    {
        dx = cur_x - mPrevX;
        dy = cur_y - mPrevY;

        mPrevX = cur_x;
        mPrevY = cur_y;
    }
    else
    {
        int win_center_x = m_width/2 + m_x;
        int win_center_y = m_height/2 + m_y;

        dx = cur_x - win_center_x; // Base delta off of center of window
        dy = cur_y - win_center_y;
        mPrevX = win_center_x;     // Must do this so if state changes, we have accurate dx,dy next time
        mPrevY = win_center_y;

        // Warp back to center, IF we are at least a certain distance away
        if(((dx != 0) || (dy != 0)) && (mSnapTimer++ < 2))
        {
            mSnapTimer = 0;
            new_loc.x = win_center_x;
            new_loc.y = win_center_y;
            CGWarpMouseCursorPosition( new_loc );
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

    mHandleEventsHasBeenCalled = true;
}

int KeyboardOSX::stopSampling()
{
  if (myThread != NULL)
  {
    mExitFlag = true;
    vpr::System::sleep(1);
  }

  return 1;
}

/*****************************************************************/
/*****************************************************************/
/**********************OS X WINDOW STUFF**************************/
/*****************************************************************/
/*****************************************************************/
// Open the X window to sample from
int KeyboardOSX::openTheWindow()
{
    vprDEBUG(vrjDBG_INPUT_MGR,0) << "vjKeyboardOSX::open()" << std::endl << vprDEBUG_FLUSH;
    rectWin.top = m_y;
    rectWin.left = m_x;
    rectWin.bottom = m_y + m_height;
    rectWin.right = m_x + m_width;

    if (noErr != CreateNewWindow (kDocumentWindowClass, kWindowStandardDocumentAttributes, &rectWin, &gpWindow))
    {
        vprDEBUG(vrjDBG_INPUT_MGR,0) << "vjKeyboardOSX::open()    Window failed to open!" << std::endl << vprDEBUG_FLUSH;
        return false;
    }
    SetWindowTitleWithCFString(gpWindow,window_title);
    InstallStandardEventHandler(GetWindowEventTarget(gpWindow));
    ChangeWindowAttributes(gpWindow, NULL, kWindowCloseBoxAttribute|kWindowResizableAttribute);

    ShowWindow (gpWindow);
    return true;
}

// Called when locking states
// - Recenter the mouse
void KeyboardOSX::lockMouse()
{
    CGPoint new_loc;
    vprDEBUG(vrjDBG_INPUT_MGR,vprDBG_STATE_LVL) << "vjKeyboardOSX: LOCKING MOUSE..." << vprDEBUG_FLUSH;
    new_loc.x = mPrevX = m_width/2 + m_x;
    new_loc.y = mPrevY = m_height/2 + m_y;
    CGWarpMouseCursorPosition( new_loc );
}

// Called when locking ends
void KeyboardOSX::unlockMouse()
{
   vprDEBUG(vrjDBG_INPUT_MGR,vprDBG_STATE_LVL) << "vjKeyboardOSX: UN-LOCKING MOUSE..." << vprDEBUG_FLUSH;
}

int KeyboardOSX::OSXKeyToKey(int xKey)
{
   switch (xKey)
   {
   case OSX_KEY_UP          : return VJKEY_UP;
   case OSX_KEY_DOWN            : return VJKEY_DOWN;
   case OSX_KEY_LEFT            : return VJKEY_LEFT;
   case OSX_KEY_RIGHT           : return VJKEY_RIGHT;
   case OSX_KEY_CONTROL     : return VJKEY_CTRL;
   case OSX_KEY_SHIFT           : return VJKEY_SHIFT;
   case OSX_KEY_COMMAND     : return VJKEY_ALT;

// Map all number keys
// Note we map keypad and normal keys making no distinction
   case OSX_KEYPAD_1            : return VJKEY_1;
   case OSX_KEY_1           : return VJKEY_1;
   case OSX_KEYPAD_2            : return VJKEY_2;
   case OSX_KEY_2           : return VJKEY_2;
   case OSX_KEYPAD_3            : return VJKEY_3;
   case OSX_KEY_3           : return VJKEY_3;
   case OSX_KEYPAD_4            : return VJKEY_4;
   case OSX_KEY_4           : return VJKEY_4;
   case OSX_KEYPAD_5            : return VJKEY_5;
   case OSX_KEY_5           : return VJKEY_5;
   case OSX_KEYPAD_6            : return VJKEY_6;
   case OSX_KEY_6           : return VJKEY_6;
   case OSX_KEYPAD_7            : return VJKEY_7;
   case OSX_KEY_7           : return VJKEY_7;
   case OSX_KEYPAD_8            : return VJKEY_8;
   case OSX_KEY_8           : return VJKEY_8;
   case OSX_KEYPAD_9            : return VJKEY_9;
   case OSX_KEY_9           : return VJKEY_9;
   case OSX_KEYPAD_0            : return VJKEY_0;
   case OSX_KEY_0           : return VJKEY_0;

   case OSX_KEY_A         : return VJKEY_A;
   case OSX_KEY_B         : return VJKEY_B;
   case OSX_KEY_C         : return VJKEY_C;
   case OSX_KEY_D         : return VJKEY_D;
   case OSX_KEY_E         : return VJKEY_E;
   case OSX_KEY_F         : return VJKEY_F;
   case OSX_KEY_G         : return VJKEY_G;
   case OSX_KEY_H         : return VJKEY_H;
   case OSX_KEY_I         : return VJKEY_I;
   case OSX_KEY_J         : return VJKEY_J;
   case OSX_KEY_K         : return VJKEY_K;
   case OSX_KEY_L         : return VJKEY_L;
   case OSX_KEY_M         : return VJKEY_M;
   case OSX_KEY_N         : return VJKEY_N;
   case OSX_KEY_O         : return VJKEY_O;
   case OSX_KEY_P         : return VJKEY_P;
   case OSX_KEY_Q         : return VJKEY_Q;
   case OSX_KEY_R         : return VJKEY_R;
   case OSX_KEY_S         : return VJKEY_S;
   case OSX_KEY_T         : return VJKEY_T;
   case OSX_KEY_U         : return VJKEY_U;
   case OSX_KEY_V         : return VJKEY_V;
   case OSX_KEY_W         : return VJKEY_W;
   case OSX_KEY_X         : return VJKEY_X;
   case OSX_KEY_Y         : return VJKEY_Y;
   case OSX_KEY_Z         : return VJKEY_Z;
   case OSX_KEY_ESCAPE    : return VJKEY_ESC;
   default: return 255;
   }

}

};
