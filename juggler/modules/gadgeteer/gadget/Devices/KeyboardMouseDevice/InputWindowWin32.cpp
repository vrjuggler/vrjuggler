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

#include <iomanip>
#include <sys/types.h>

#include <jccl/Config/ConfigElement.h>
#include <gadget/Util/Debug.h>
#include <gadget/InputManager.h>
#include <gadget/Type/KeyboardMouse/KeyEvent.h>
#include <gadget/Type/KeyboardMouse/MouseEvent.h>
#include <gadget/Devices/KeyboardMouseDevice/InputWindowWin32.h>

namespace gadget
{

bool InputWindowWin32::config(jccl::ConfigElementPtr e)
{
   unsigned required_definition_ver(1);

   if(e->getVersion() < required_definition_ver)
   {
      vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_CRITICAL_LVL)
         << clrOutBOLD(clrRED, "ERROR")
         << " [gadget::InputWindowWin32::config()] Element named '"
         << e->getName() << "'" << std::endl << vprDEBUG_FLUSH;
      vprDEBUG_NEXT(gadgetDBG_INPUT_MGR, vprDBG_CRITICAL_LVL)
         << "is version " << e->getVersion()
         << ", but we require at least version " << required_definition_ver
         << ". Ignoring...\n" << vprDEBUG_FLUSH;
      return false;
   }

   if ( !(InputArea::config(e) && Input::config(e)) )
   {
      return false;
   }

   const char neg_one_STRING[] = "-1";

   /*
   for ( int i = 0; i < gadget::LAST_KEY; ++i )
   {
      mRealkeys[i] = mKeys[i] = mCurKeys[i];
   }
   */

   // Check for a remote display
   //std::string remote_display_name =
   //   e->getProperty<std::string>("event_source_name");

   // Default to owning the window
   mUseOwnDisplay = true;

   // Get size and position
   mWidth  = e->getProperty<int>("size", 0);
   mHeight = e->getProperty<int>("size", 1);

   // Sanity checks.
   if (mWidth == 0) mWidth = 400;
   if (mHeight == 0) mHeight = 400;

   mX = e->getProperty<int>("origin", 0);
   mY = e->getProperty<int>("origin", 1);

   // Get the X display string
   int x_disp_num = e->getProperty<int>("display_number");
   jccl::ConfigElementPtr disp_sys_elt =
      gadget::InputManager::instance()->getDisplaySystemElement();
 
   // Get the lock information
   /*
   mLockToggleKey = e->getProperty<int>("lock_key");
   bool start_locked = e->getProperty<bool>("start_locked");

   if (start_locked)
   {
      mLockState = Lock_LockKey;      // Initialize to the locked state
   }
   */
   /*
   mMouseSensitivity = e->getProperty<float>("mouse_sensitivity");
   if (0.0f == mMouseSensitivity)
   {
      mMouseSensitivity = 0.5;
   }

   vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_STATE_LVL)
      << "Mouse Sensititivty: " << mMouseSensitivity << std::endl
      << vprDEBUG_FLUSH;

   mSleepTimeMS = e->getProperty<int>("sleep_time");

   // Sanity check.
   if (mSleepTimeMS == 0)
   {
      mSleepTimeMS = 50;
   }
   */

   return true;
}

void InputWindowWin32::controlLoop(void* devPtr)
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
         << "[gadget::InputWindowWin32::controlLoop()] "
         << "Mouse set to initial lock. Locking it now.\n"
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
   ::CloseWindow(mWinHandle); // send a message to the window to close
   mControlLoopDone = true;
}

// Create a win32 window and start a thread
// processing it's messages
bool InputWindowWin32::startSampling()
{
   // XXX: Simple fix for now. We don't need to do any sampling.
   /*
   if (!mUseOwnDisplay)
   {
      return true;
   }
   */

   if ( mThread != NULL )
   {
      vprDEBUG(vprDBG_ERROR,vprDBG_CRITICAL_LVL)
         << clrOutNORM(clrRED,"ERROR:")
         << "[gadget::InputWindowWin32::startSampling()] "
         << "startSampling called, when already sampling.\n"
         << vprDEBUG_FLUSH;
      vprASSERT(mThread == NULL);
      return 0;
   }

   vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_CONFIG_LVL)
      << "gadget::InputWindowWin32::startSampling() : ready to go.."
      << std::endl << vprDEBUG_FLUSH;

   // Create a new thread to handle the control
   mExitFlag = false;
   vpr::ThreadMemberFunctor<InputWindowWin32>* memberFunctor =
      new vpr::ThreadMemberFunctor<InputWindowWin32>(this,
                                                     &InputWindowWin32::controlLoop,
                                                     (void*) this);

   mThread = new vpr::Thread(memberFunctor);

   // return success value...
   if ( ! mThread->valid() )
   {
      return false; // fail
   }

   return true; // success
}


void InputWindowWin32::updateData()
{
   /*
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
   */
}

bool InputWindowWin32::stopSampling()
{
   if ( mThread != NULL )
   {
      mExitFlag = true;
      ::PostMessage( mWinHandle, WM_USER, 0, 0 );// send a dummy message to the window to close
      mThread->join();
      delete mThread;
      mThread = NULL;
      //std::cout << "Stopping event window.." << std::endl;
   }
   return true;
}


/*********************** WINDOW STUFF ****************************/

/** 
 * Window function for the main application window. Processes all 
 * the menu selections and oter messages.
 */
LONG APIENTRY MenuWndProc(HWND hWnd, UINT message, UINT wParam, LONG lParam)
{
   switch ( message )
   {
      // Catch the ALT key so that it does not open the system menu.
      case WM_SYSKEYDOWN:
      case WM_SYSKEYUP:
         break;
      case WM_SYSCOMMAND:
         return DefWindowProc(hWnd, message, wParam, lParam);
         break;
      case WM_DESTROY:
         PostQuitMessage(0);
         break;
      // Make sure that the resize event stays around until it is processed.
      case WM_SIZE:
         break;
      default:
         return DefWindowProc(hWnd, message, wParam, lParam);
   }
   return 0;
}

void InputWindowWin32::createWindowWin32()
{
   if (mUseOwnDisplay)
   {
      int root_height;

      InitCommonControls();

      m_hInst = GetModuleHandle(NULL);   // Just try to get the application's handle
      MenuInit(m_hInst);

      root_height = GetSystemMetrics(SM_CYSCREEN);

      /* Create the app. window */
      mWinHandle = CreateWindow("Gadgeteer Event Window", mInstName.c_str(),
                                WS_OVERLAPPEDWINDOW, mX,
                                root_height - mY - mHeight, mWidth, mHeight,
                                (HWND) NULL, NULL, m_hInst, (LPSTR) NULL);
      ShowWindow(mWinHandle,SW_SHOW);
      UpdateWindow(mWinHandle);

      // Attach a pointer to the device for use from the WNDPROC
      SetWindowLong(mWinHandle, GWL_USERDATA, (LPARAM)this);

      if ( NULL == mWinHandle )
      {
         doInternalError("Could not create InputWindowWin32!");
      }
   }
} /*CreateWindow*/

BOOL InputWindowWin32::MenuInit(HINSTANCE hInstance)
{
   HANDLE    hMemory;
   PWNDCLASS pWndClass;
   BOOL      bSuccess;

   /* Initialize the menu window class */
   hMemory   = LocalAlloc(LPTR, sizeof(WNDCLASS));
   if ( !hMemory )
   {
      MessageBox(NULL, ("<MenuInit> Not enough memory."), NULL,
                 MB_OK | MB_ICONHAND);
      return(FALSE);
   }

   pWndClass = (PWNDCLASS) LocalLock(hMemory);

   pWndClass->style         = 0;
   pWndClass->lpfnWndProc   = (WNDPROC) MenuWndProc;
   pWndClass->hInstance     = hInstance;
   pWndClass->hIcon         = LoadIcon(hInstance, "GADGETEER_ICON");
   pWndClass->hCursor       = (HCURSOR) LoadCursor(NULL, IDC_ARROW);
   pWndClass->hbrBackground = (HBRUSH) GetStockObject(WHITE_BRUSH);
   pWndClass->lpszMenuName  = ("MenuMenu"),
   pWndClass->lpszClassName = ("Gadgeteer Event Window");

#ifdef _DEBUG
#  define LIBNAME "gadget_d.dll"
#else
#  define LIBNAME "gadget.dll"
#endif

   if (pWndClass->hIcon == NULL)
   {
      HINSTANCE hDLLInstance = LoadLibrary( LIBNAME );
      if (hDLLInstance != NULL)
      {
         pWndClass->hIcon = LoadIcon(hDLLInstance, "GADGETEER_ICON");
      }
   }

   bSuccess = RegisterClass(pWndClass);
   LocalUnlock(hMemory);
   LocalFree(hMemory);

   return bSuccess;
}

// process the current window events
// Called repeatedly by
// - the controlLoop when "we own the window",
// - the GlWindow if we "dont own the window"
bool InputWindowWin32::sample()
{
   this->handleEvents();
   return true;
}

std::string InputWindowWin32::getElementType()
{
   return "input_window";
}
} // End of gadget namespace
