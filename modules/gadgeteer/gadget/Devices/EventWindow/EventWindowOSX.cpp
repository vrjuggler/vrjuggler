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

#include <gadget/Util/Debug.h>
#include <gadget/InputManager.h>
#include <jccl/Config/ConfigChunk.h>

#include <gadget/Devices/EventWindow/EventWindowOSX.h> // my header

namespace gadget
{

/** Constructor. */
bool EventWindowOSX::config(jccl::ConfigChunkPtr c)
{
   if(! (Input::config(c) && EventWindow::config(c)))
      return false;

   //const char neg_one_STRING[] = "-1";

   // Done in Input --- mThread = NULL;
   int i;
   for (i =0; i < 256; ++i)
   {
      mCurKeys[i] = m_realkeys[i] = m_keys[i] = 0;
   }

   mCurKeys[0] = m_realkeys[0] = m_keys[0] = 1;

   // Get size and position
   m_width = c->getProperty<int>("width");
   m_height = c->getProperty<int>("height");
   if (m_width == 0) m_width = 400;
   if (m_height == 0) m_height = 400;

   m_x = c->getProperty<int>("origin", 0);
   m_y = c->getProperty<int>("origin", 1);

   // Get the lock information
   mLockToggleKey = c->getProperty<int>("lock_key");
   bool start_locked = c->getProperty<bool>("start_locked");
   if (start_locked)
      mLockState = Lock_LockKey;      // Initialize to the locked state

   m_mouse_sensitivity = c->getProperty<float>("msens");
   if (0.0f == m_mouse_sensitivity)
      m_mouse_sensitivity = 0.5;

   vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_STATE_LVL)
      << "Mouse Sensititivty: " << m_mouse_sensitivity << std::endl
      << vprDEBUG_FLUSH;

   return true;
}

int EventWindowOSX::startSampling()
{
    if(mAmSampling == true)
    {
        vprDEBUG(vprDBG_ERROR,vprDBG_CRITICAL_LVL)
            << clrOutNORM(clrRED,"ERROR:")
            << "gadget::EventWindowOSX: startSampling called, when already sampling.\n"
            << vprDEBUG_FLUSH;
        vprASSERT(false);
    }

    std::cout << "Start sampleing in EventWindow" << std::endl;
    openTheWindow();
    mAmSampling = true;

    return 1;
}

int EventWindowOSX::onlyModifier(int mod)
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

void EventWindowOSX::updateData()
{
    vpr::Guard<vpr::Mutex> guard(mKeysLock);      // Lock access to the m_keys array
    if(mHandleEventsHasBeenCalled)            // If we haven't updated anything, then don't swap stuff
    {
        mHandleEventsHasBeenCalled = false;

        // Scale mouse values based on sensitivity
        m_keys[gadget::MOUSE_POSX] = int(float(m_keys[gadget::MOUSE_POSX]) * m_mouse_sensitivity);
        m_keys[gadget::MOUSE_NEGX] = int(float(m_keys[gadget::MOUSE_NEGX]) * m_mouse_sensitivity);
        m_keys[gadget::MOUSE_POSY] = int(float(m_keys[gadget::MOUSE_POSY]) * m_mouse_sensitivity);
        m_keys[gadget::MOUSE_NEGY] = int(float(m_keys[gadget::MOUSE_NEGY]) * m_mouse_sensitivity);

        /*
        vprDEBUG(vprDBG_ALL,0)
           << "gadget::EventWindowOSX::updateData:" << mInstName << " -- "
           << "mouse_keys: px:" << m_keys[gadget::MOUSE_POSX]
           << " nx:" << m_keys[gadget::MOUSE_NEGX]
           << " py:" << m_keys[gadget::MOUSE_POSY]
           << " ny:" << m_keys[gadget::MOUSE_NEGY]
           << std::endl << vprDEBUG_FLUSH;
        */

        // Copy over values
        for ( unsigned int i = 0; i < 256; ++i )
        {
           mCurKeys[i] = m_keys[i];
        }

        // Re-initialize the m_keys based on current key state in realKeys
        // Set the initial state of the m_key key counts based on the current state of the system
        for ( unsigned int j = 0; j < 256; ++j )
        {
           m_keys[j] = m_realkeys[j];
        }

        //clear the mose movement
        m_keys[gadget::MOUSE_POSX] = 0.0;
        m_keys[gadget::MOUSE_NEGX] = 0.0;
        m_keys[gadget::MOUSE_POSY] = 0.0;
        m_keys[gadget::MOUSE_NEGY] = 0.0;
   }
   updateEventQueue();
}

int EventWindowOSX::stopSampling()
{

    // Add code to remove window
    // Deleate the UPP handle
    //mExitFlag = true;
    return 1;
}

int EventWindowOSX::osxKeyToKey(UInt32 osxKey)
{
    switch (osxKey)
    {
        case OSXK_Up        : return gadget::KEY_UP;
        case OSXK_Down      : return gadget::KEY_DOWN;
        case OSXK_Left      : return gadget::KEY_LEFT;
        case OSXK_Right     : return gadget::KEY_RIGHT;
        //Mod keys are handled diffrently
        //case XK_Control_L : return gadget::KEY_CTRL;
        //case XK_Control_R : return gadget::KEY_CTRL;
        //case XK_Shift_L   : return gadget::KEY_SHIFT;
        //case XK_Shift_R   : return gadget::KEY_SHIFT;
        //case XK_Alt_L     : return gadget::KEY_ALT;
        //case XK_Alt_R     : return gadget::KEY_ALT;

        // Map all number keys
        // Note we map keypad and normal keys making no distinction
        case OSXK_1 : return gadget::KEY_1;
        case OSXK_KP_1  : return gadget::KEY_1;
        case OSXK_2 : return gadget::KEY_2;
        case OSXK_KP_2  : return gadget::KEY_2;
        case OSXK_3 : return gadget::KEY_3;
        case OSXK_KP_3  : return gadget::KEY_3;
        case OSXK_4 : return gadget::KEY_4;
        case OSXK_KP_4  : return gadget::KEY_4;
        case OSXK_5 : return gadget::KEY_5;
        case OSXK_KP_5  : return gadget::KEY_5;
        case OSXK_6 : return gadget::KEY_6;
        case OSXK_KP_6  : return gadget::KEY_6;
        case OSXK_7 : return gadget::KEY_7;
        case OSXK_KP_7  : return gadget::KEY_7;
        case OSXK_8 : return gadget::KEY_8;
        case OSXK_KP_8  : return gadget::KEY_8;
        case OSXK_9 : return gadget::KEY_9;
        case OSXK_KP_9  : return gadget::KEY_9;
        case OSXK_0 : return gadget::KEY_0;
        case OSXK_KP_0  : return gadget::KEY_0;

        case OSXK_a : return gadget::KEY_A;
        case OSXK_b : return gadget::KEY_B;
        case OSXK_c : return gadget::KEY_C;
        case OSXK_d : return gadget::KEY_D;
        case OSXK_e : return gadget::KEY_E;
        case OSXK_f : return gadget::KEY_F;
        case OSXK_g : return gadget::KEY_G;
        case OSXK_h : return gadget::KEY_H;
        case OSXK_i : return gadget::KEY_I;
        case OSXK_j : return gadget::KEY_J;
        case OSXK_k : return gadget::KEY_K;
        case OSXK_l : return gadget::KEY_L;
        case OSXK_m : return gadget::KEY_M;
        case OSXK_n : return gadget::KEY_N;
        case OSXK_o : return gadget::KEY_O;
        case OSXK_p : return gadget::KEY_P;
        case OSXK_q : return gadget::KEY_Q;
        case OSXK_r : return gadget::KEY_R;
        case OSXK_s : return gadget::KEY_S;
        case OSXK_t : return gadget::KEY_T;
        case OSXK_u : return gadget::KEY_U;
        case OSXK_v : return gadget::KEY_V;
        case OSXK_w : return gadget::KEY_W;
        case OSXK_x : return gadget::KEY_X;
        case OSXK_y : return gadget::KEY_Y;
        case OSXK_z : return gadget::KEY_Z;
        case OSXK_Ecs   : return gadget::KEY_ESC;
        default     : return 255;
   }

}

/*****************************************************************/
/*****************************************************************/
/******************** OS X WINDOW STUFF **************************/
/*****************************************************************/
/*****************************************************************/
/** Open the carbon window to sample from. */
int EventWindowOSX::openTheWindow()
{
    Rect         wRect;

    //Get the size of the screen from core graphics
    //I'll need to check to see how this works with multiple monitors
    CGRect bounds;
    bounds = CGDisplayBounds(kCGDirectMainDisplay);


    //set the window size and location with the height adjusted
    SetRect(&wRect, m_x , bounds.size.height - m_y - m_height,
                            m_x + m_width, bounds.size.height - m_y); /* left, top, right, bottom */


    CreateNewWindow (   kDocumentWindowClass,
                        kWindowStandardDocumentAttributes |
                        kWindowStandardHandlerAttribute,
                        &wRect,
                        &mWindow );

    SetWindowTitleWithCFString (mWindow,
            CFStringCreateWithCString(NULL, mInstName.c_str(), kCFStringEncodingMacRoman) );
            // Set title

    attachEvents(mWindow);

    ShowWindow (mWindow);


    vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_STATE_LVL)
       << "gadget::EventWindowOSX::openTheWindow() : done." << std::endl
       << vprDEBUG_FLUSH;

    return 1;
}

int EventWindowOSX::attachEvents(WindowRef aWindow)
{
    EventTypeSpec    eventTypes[8];  //Remember to pass the size when registering ther handler
    EventHandlerUPP  handlerUPP;

    // Set up the events to listen for
    eventTypes[0].eventClass = kEventClassKeyboard;
    eventTypes[0].eventKind  = kEventRawKeyDown;

    eventTypes[1].eventClass = kEventClassKeyboard;
    eventTypes[1].eventKind  = kEventRawKeyRepeat;

    eventTypes[2].eventClass = kEventClassKeyboard;
    eventTypes[2].eventKind  = kEventRawKeyUp;

    eventTypes[3].eventClass = kEventClassKeyboard;
    eventTypes[3].eventKind  = kEventRawKeyModifiersChanged;

    eventTypes[4].eventClass = kEventClassMouse;
    eventTypes[4].eventKind  = kEventMouseDown;

    eventTypes[5].eventClass = kEventClassMouse;
    eventTypes[5].eventKind  = kEventMouseUp;

    eventTypes[6].eventClass = kEventClassMouse;
    eventTypes[6].eventKind  = kEventMouseMoved;

    eventTypes[7].eventClass = kEventClassMouse;
    eventTypes[7].eventKind  = kEventMouseDragged;


    handlerUPP = NewEventHandlerUPP(keyboardHandlerOSX);

    InstallWindowEventHandler (aWindow, handlerUPP,  // Install handler
                                8, eventTypes,
                                this, NULL);

    return 1;
}




/**
 * Called when locking states.
 * - Re-center the mouse
 */
void EventWindowOSX::lockMouse()
{
    //Used when we want a graphics window to also handle input
    //OS X will do this differently
    //vpr::Guard<vpr::Mutex> xguard(mXfuncLock);

    vprDEBUG(gadgetDBG_INPUT_MGR,vprDBG_STATE_LVL)
        << "gadget::EventWindowOSX: LOCKING MOUSE..." << vprDEBUG_FLUSH;

    //Status from system calls
    OSStatus   result;
    //Satus from core graphics calls
    CGEventErr err;

    CGRect     bounds;
    CGPoint    point;
    Rect       windowRect;

    //I think this will fail with multiple displays

    //get the bounding rectangle for the window
    result = GetWindowBounds (  mWindow,
                                kWindowContentRgn,
                                &windowRect );
    //Should make sure there is no error

    //Get the size of the display
    //this is probably not necessary since core graphics and carbon
    //use the same coordinate system
    bounds = CGDisplayBounds(kCGDirectMainDisplay);

    //Calculate the point in the middle of the window
    point.x = bounds.origin.x + windowRect.left
                + (windowRect.right - windowRect.left)/2;
    point.y = bounds.origin.y +  windowRect.top
                + (windowRect.bottom - windowRect.top)/2;
    //Move the mouse to the middle of the window
    err = CGDisplayMoveCursorToPoint(kCGDirectMainDisplay, point);
    //Should make sure there is no error

    //The cursor stays put as the mouse is moved
    err = CGAssociateMouseAndMouseCursorPosition(false);
    //Should make sure there is no error

   vprDEBUG_CONT(gadgetDBG_INPUT_MGR,vprDBG_STATE_LVL)
      << "lock finished.\n" << vprDEBUG_FLUSH;
}

/** Called when locking ends. */
void EventWindowOSX::unlockMouse()
{

    //Used when we want a graphics window to also handle input
    //OS X will do this differently
    //vpr::Guard<vpr::Mutex> xguard(mXfuncLock);

    vprDEBUG(gadgetDBG_INPUT_MGR,vprDBG_STATE_LVL)
      << "gadget::EventWindowOSX: UN-LOCKING MOUSE..." << vprDEBUG_FLUSH;

    //Status from core graphics calls
    CGEventErr err;

    err = CGAssociateMouseAndMouseCursorPosition(true);
    //Should make sure there is no error

    vprDEBUG_CONT(gadgetDBG_INPUT_MGR,vprDBG_STATE_LVL)
      << "un-lock finished.\n" << vprDEBUG_FLUSH;
}

pascal OSStatus EventWindowOSX::gotKeyEvent(EventHandlerCallRef  nextHandler,
                                            EventRef             theEvent,
                                            void*                userData)
{
            //The type of event
            UInt32     eventClass;
            UInt32     eventKind;
            int        vj_key; // The key in vj space
            //The modifier keys
            UInt32     modKeys;
            //The mouse button
            UInt16     myButton;
            //Status from system calls
            OSStatus   result;
            //Status from core graphics calls
            //CGEventErr err;

            Point      mousePos;

            vpr::Guard<vpr::Mutex> guard(mKeysLock);
            // Lock access to the m_keys array for the duration of this function

            mHandleEventsHasBeenCalled = true;

            eventClass = GetEventClass(theEvent);
            eventKind  = GetEventKind(theEvent);

            if ( ( eventClass == kEventClassKeyboard && eventKind == kEventRawKeyDown) ||
                 ( eventClass == kEventClassKeyboard && eventKind == kEventRawKeyRepeat))
            {
                UInt32 keyInt;
                GetEventParameter (theEvent, kEventParamKeyCode, typeUInt32,
                                NULL, sizeof(typeUInt32), NULL, &keyInt);

                vj_key = osxKeyToKey(keyInt);
                m_realkeys[vj_key] = 1;
                m_keys[vj_key] += 1;

                //std::cout << "Got Down: " << vj_key << std::endl;

                // -- Update lock state -- //
                // Any[key == ESC]/unlock(ifneeded) -> Unlocked
                // Unlocked[key!=lockKey]/lockMouse -> lock_keydown
                // Unlocked[key==lockKey]/lockMouse -> lock_keylock
                // lock_keydown[key==lockKey] -> lock_keylock
                // lock_keylock[key==lockKey] -> Unlocked
                if (vj_key == gadget::KEY_ESC)       // Check for Escape from bad state
                {
                    vprDEBUG(gadgetDBG_INPUT_MGR,vprDBG_STATE_LVL)
                    << "gadget::EventWindowOSX: Trying to ESCAPE from current state.\n"
                    << vprDEBUG_FLUSH;
                    if(mLockState != Unlocked)
                    {
                        vprDEBUG_NEXT(gadgetDBG_INPUT_MGR,vprDBG_STATE_LVL)
                            << "gadget::EventWindowOSX: STATE switch: <ESCAPE> --> Unlocked\n"
                            << vprDEBUG_FLUSH;
                        mLockState = Unlocked;
                        unlockMouse();
                    }
                    else
                    {
                        vprDEBUG_NEXT(gadgetDBG_INPUT_MGR,vprDBG_STATE_LVL)
                            << "gadget::EventWindowOSX: Already unlocked.  Cannot ESCAPE."
                            << vprDEBUG_FLUSH;
                    }
                }
                else if(mLockState == Unlocked)
                {
                    if(vj_key != mLockToggleKey)
                    {
                        mLockState = Lock_KeyDown;       // Switch state
                        mLockStoredKey = vj_key;         // Store the VJ key that is down
                        vprDEBUG(gadgetDBG_INPUT_MGR,vprDBG_STATE_LVL)
                            << "gadget::EventWindowOSX: STATE switch: Unlocked --> Lock_KeyDown\n"
                            << vprDEBUG_FLUSH;
                        lockMouse();
                    }
                    else if(vj_key == mLockToggleKey)
                    {
                        mLockState = Lock_LockKey;
                        vprDEBUG(gadgetDBG_INPUT_MGR,vprDBG_STATE_LVL)
                            << "gadget::EventWindowOSX: STATE switch: Unlocked --> Lock_LockKey\n"
                            << vprDEBUG_FLUSH;
                        lockMouse();
                    }
                }
                else if((mLockState == Lock_KeyDown) && (vj_key == mLockToggleKey))
                // Just switch the current locking state
                {
                    mLockState = Lock_LockKey;
                    vprDEBUG(gadgetDBG_INPUT_MGR,vprDBG_STATE_LVL)
                        << "gadget::EventWindowOSX: STATE switch: Lock_KeyDown --> Lock_LockKey\n"
                        << vprDEBUG_FLUSH;
                }
                else if((mLockState == Lock_LockKey) && (vj_key == mLockToggleKey))
                {
                    mLockState = Unlocked;
                    vprDEBUG(gadgetDBG_INPUT_MGR,vprDBG_STATE_LVL)
                        << "gadget::EventWindowOSX: STATE switch: Lock_LockKey --> Unlocked\n"
                        << vprDEBUG_FLUSH;
                    unlockMouse();
                }
                /*
                vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_HVERB_LVL)
                    << "KeyPress:  " << std::hex << key
                    << " state:" << ((XKeyEvent*)&event)->state
                    << " ==> " << xKeyToKey(key) << std::endl << vprDEBUG_FLUSH;
                break;
                */



            }

            if ( eventClass == kEventClassKeyboard && eventKind == kEventRawKeyUp )
            {
                UInt32 keyInt;
                GetEventParameter (theEvent, kEventParamKeyCode, typeUInt32,
                                NULL, sizeof(typeUInt32), NULL, &keyInt);

                vj_key = osxKeyToKey(keyInt);
                m_realkeys[vj_key] = 0;

                //std::cout << "Got Up: " << vj_key << std::endl;

                // -- Update lock state -- //
                // lock_keyDown[key==storedKey]/unlockMouse -> unlocked
                if((mLockState == Lock_KeyDown) && (vj_key == mLockStoredKey))
                {
                    mLockState = Unlocked;
                    vprDEBUG(gadgetDBG_INPUT_MGR,vprDBG_STATE_LVL)
                    << "gadget::EventWindowOSX: STATE switch: Lock_KeyDown --> Unlocked\n"
                    << vprDEBUG_FLUSH;
                    unlockMouse();
                }

                //What does this do?
                /*
                vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_HVERB_LVL)
                    << "KeyRelease:" << std::hex << key
                    << " state:" << ((XKeyEvent*)&event)->state
                    << " ==> " << xKeyToKey(key) << std::endl << vprDEBUG_FLUSH;
                */

            }


            if ( eventClass == kEventClassKeyboard && eventKind == kEventRawKeyModifiersChanged)
            {

                GetEventParameter (theEvent, kEventParamKeyModifiers, typeUInt32,
                                NULL, sizeof(typeUInt32), NULL, &modKeys);

                /* Juggler does not care about caps and command
                if (modKeys & alphaLock)
                {
                    cout << "Caps ";
                }
                if (modKeys & cmdKey)
                {
                    cout << "CMD ";
                }
                */
                if (modKeys & shiftKey)
                {
                    m_realkeys[gadget::KEY_SHIFT] = 1;
                    m_keys[gadget::KEY_SHIFT] += 1;
                    // The m_key value will not reflect the number of times the mod key
                    // was pressed but rather how many times it was pressed when
                    // any mod key changes state
                }
                else
                {
                    m_realkeys[gadget::KEY_SHIFT] = 0;
                }

                if (modKeys & optionKey)
                {

                    m_realkeys[gadget::KEY_ALT] = 1;
                    m_keys[gadget::KEY_ALT] += 1;
                }
                else
                {
                    m_realkeys[gadget::KEY_ALT] = 0;
                }

                if (modKeys & controlKey)
                {

                    m_realkeys[gadget::KEY_CTRL] = 1;
                    m_keys[gadget::KEY_CTRL] += 1;
                }
                else
                {
                    m_realkeys[gadget::KEY_CTRL] = 0;
                }

                // If any mod keys are pressed lock the mouse
                if ((modKeys & shiftKey) || (modKeys & controlKey) || (modKeys & optionKey) || (modKeys & shiftKey) )
                {
                    if ( mLockState == Unlocked)
                    {
                        mLockState = Lock_KeyDown;       // Switch state
                        vprDEBUG(gadgetDBG_INPUT_MGR,vprDBG_STATE_LVL)
                            << "gadget::EventWindowOSX: STATE switch: Unlocked --> Lock_KeyDown\n"
                            << vprDEBUG_FLUSH;
                        lockMouse();
                    }
                }

                //Unlock mouse if lock key is not down
                else
                {
                    if (  mLockState != Lock_LockKey )
                    {
                        mLockState = Unlocked;
                        vprDEBUG(gadgetDBG_INPUT_MGR,vprDBG_STATE_LVL)
                            << "gadget::EventWindowOSX: STATE switch: Lock_KeyDown --> Unlocked\n"
                            << vprDEBUG_FLUSH;
                        unlockMouse();
                    }
                }


            }

            if ( eventClass == kEventClassMouse && eventKind == kEventMouseDown)
            {

                //Allow the base handler to take care of most mouse events
                result = CallNextEventHandler (nextHandler, theEvent);

                GetEventParameter (theEvent, kEventParamMouseButton, typeMouseButton,
                                NULL, sizeof(typeMouseButton), NULL, &myButton);


                switch ( myButton )
                {
                    case kEventMouseButtonPrimary :
                        m_realkeys[gadget::MBUTTON1] = m_keys[gadget::MBUTTON1] = 1;
                        break;
                    case kEventMouseButtonSecondary:
                        m_realkeys[gadget::MBUTTON2] = m_keys[gadget::MBUTTON2] = 1;
                        break;
                    case kEventMouseButtonTertiary:
                        m_realkeys[gadget::MBUTTON3] = m_keys[gadget::MBUTTON3] = 1;
                        break;
                }
                //std::cout << "mouse down" << std::endl;
            }

            if ( eventClass == kEventClassMouse && eventKind == kEventMouseUp)
            {
                //std::cout << "mouse up" << std::endl;
                result = CallNextEventHandler (nextHandler, theEvent);

                GetEventParameter (theEvent, kEventParamMouseButton, typeMouseButton,
                                NULL, sizeof(typeMouseButton), NULL, &myButton);

                switch ( myButton )
                {
                    case kEventMouseButtonPrimary :
                        m_realkeys[gadget::MBUTTON1] = m_keys[gadget::MBUTTON1] = 0;
                        break;
                    case kEventMouseButtonSecondary:
                        m_realkeys[gadget::MBUTTON2] = m_keys[gadget::MBUTTON2] = 0;
                        break;
                    case kEventMouseButtonTertiary:
                        m_realkeys[gadget::MBUTTON3] = m_keys[gadget::MBUTTON3] = 0;
                        break;
                }

            }

            if ( eventClass == kEventClassMouse &&
                (eventKind == kEventMouseMoved || eventKind == kEventMouseDragged) )
            {

                result = CallNextEventHandler (nextHandler, theEvent);

                     //std::cout << "mouse moved" << std::endl;

                // Note even though 'kEventParamMouseDelta' is a carbon call
                // it does not work on OS 9
                GetEventParameter (theEvent, kEventParamMouseDelta, typeQDPoint,
                                NULL, sizeof(typeQDPoint), NULL, &mousePos);

                // Update m_keys based on key pressed and store in the key array
                if ( mousePos.h > 0 )
                {
                    m_keys[gadget::MOUSE_POSX] = m_keys[gadget::MOUSE_POSX] + mousePos.h;
                    // Positive movement in the x direction.
                }
                else
                {
                    m_keys[gadget::MOUSE_NEGX] = m_keys[gadget::MOUSE_NEGX] - mousePos.h;
                    // Negative movement in the x direction.
                }

                if ( mousePos.v > 0 )
                {
                    m_keys[gadget::MOUSE_POSY] = m_keys[gadget::MOUSE_POSY] + mousePos.v;
                    // Positive movement in the y direction.
                }
                else
                {
                    m_keys[gadget::MOUSE_NEGY] = m_keys[gadget::MOUSE_NEGY] - mousePos.v;
                    // Negative movement in the y direction.
                }
            }

            return noErr;                                // Report success
}



/**
 * Call back function to register with the carbon event loop.  The userData
 * argument contains a reference to the instance of the EventWindowOSX class
 * that registered to receive the event for its window.
 */
pascal OSStatus keyboardHandlerOSX ( EventHandlerCallRef  nextHandler,
                                     EventRef             theEvent,
                                     void*                userData)
{
    EventWindowOSX * theWindowClass = (EventWindowOSX *) userData;
    pascal OSStatus stat;
    stat = theWindowClass->gotKeyEvent(nextHandler, theEvent, userData);

    return stat;                                // Report success
}

} // end namespace
