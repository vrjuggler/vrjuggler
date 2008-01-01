/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2008 by Iowa State University
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
#include <sys/types.h>
#include <boost/bind.hpp>

#include <vpr/vpr.h>
#include <vpr/System.h>
#include <jccl/Config/ConfigElement.h>
#include <gadget/Util/Debug.h>
#include <gadget/InputManager.h>
#include <gadget/gadgetParam.h>
#include <gadget/Devices/KeyboardMouseDevice/InputWindowWin32.h>

// The following is used below in InputWindowWin32::MenuInit(). The key parts
// are GADGET_LIB_RT_OPT and GADGET_VERSION_STR.
#  if defined(GADGET_DEBUG)
#     if defined(_DEBUG)
#        define GADGET_LIB_RT_OPT "_d"
#     else
#        define GADGET_LIB_RT_OPT "_g"
#     endif
#  else
#     define GADGET_LIB_RT_OPT ""
#  endif

#  define GADGET_STRINGIZE(X) GADGET_DO_STRINGIZE(X)
#  define GADGET_DO_STRINGIZE(X) #X
#  define GADGET_VERSION_STR GADGET_STRINGIZE(__GADGET_MAJOR__) "_" \
                             GADGET_STRINGIZE(__GADGET_MINOR__) "_" \
                             GADGET_STRINGIZE(__GADGET_PATCH__)


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

   return true;
}

void InputWindowWin32::controlLoop()
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
   mExitFlag = false;
   while ( !mExitFlag )
   {
      this->sample();

      // Sleep for a user-specified sleep amount of time.
      vpr::System::msleep( mSleepTimeMS );
   }

   // Clean up, send a message to the window to close.
   ::CloseWindow(mWinHandle);
   mControlLoopDone = true;
}

bool InputWindowWin32::startSampling()
{
   if ( mThread != NULL )
   {
      vprDEBUG(vprDBG_ERROR,vprDBG_CRITICAL_LVL)
         << clrOutNORM(clrRED, "ERROR")
         << ": [gadget::InputWindowWin32::startSampling()] "
         << "startSampling() called when already sampling.\n"
         << vprDEBUG_FLUSH;
      vprASSERT(mThread == NULL);
      return false;
   }

   vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_CONFIG_LVL)
      << "gadget::InputWindowWin32::startSampling() : ready to go.."
      << std::endl << vprDEBUG_FLUSH;

   bool started(false);

   // Create a new thread to handle the control
   mExitFlag = false;

   try
   {
      mThread = new vpr::Thread(boost::bind(&InputWindowWin32::controlLoop,
                                this));
      started = true;
   }
   catch (vpr::Exception& ex)
   {
      vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_CRITICAL_LVL)
         << clrOutBOLD(clrRED, "ERROR")
         << ": Failed to spawn thread for Win32 input window driver!\n"
         << vprDEBUG_FLUSH;
      vprDEBUG_NEXT(gadgetDBG_INPUT_MGR, vprDBG_CRITICAL_LVL)
         << ex.what() << std::endl << vprDEBUG_FLUSH;
   }

   return started;
}

bool InputWindowWin32::stopSampling()
{
   if ( mThread != NULL )
   {
      mExitFlag = true;
      // Send a dummy message to the window to close.
      bool result = ::PostMessage(mWinHandle, WM_USER, 0, 0);

      if ( ! result )
      {
         char* msg_buffer(NULL);

         DWORD error_code = GetLastError();
         FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
                       NULL, error_code,
                       MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                       (LPTSTR) &msg_buffer, 0, NULL);

         vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_WARNING_LVL)
            << "WARNING: [gadget::InputWindowWin32::stopSampling()] "
            << std::endl << vprDEBUG_FLUSH;
         vprDEBUG_NEXT(gadgetDBG_INPUT_MGR, vprDBG_WARNING_LVL)
            << "PostMessage() failed with error code " << error_code
            << std::endl << vprDEBUG_FLUSH;
         vprDEBUG_NEXT(gadgetDBG_INPUT_MGR, vprDBG_WARNING_LVL)
            << msg_buffer << std::endl << vprDEBUG_FLUSH;

         LocalFree(msg_buffer);
      }
      else
      {
         mThread->join();
      }
      delete mThread;
      mThread = NULL;
   }
   return true;
}


/*********************** WIN32 STUFF ****************************/

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
   SetWindowLongPtr(mWinHandle, GWLP_USERDATA, (LPARAM)this);

   if ( NULL == mWinHandle )
   {
      doInternalError("Could not create InputWindowWin32!");
   }
}

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

   if (pWndClass->hIcon == NULL)
   {
      HINSTANCE hDLLInstance =
         LoadLibrary("gadget" GADGET_LIB_RT_OPT "-" GADGET_VERSION_STR ".dll");
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

bool InputWindowWin32::sample()
{
   // Handle all input events.
   handleEvents();

   return true;
}

std::string InputWindowWin32::getElementType()
{
   return "input_window";
}
} // End of gadget namespace
