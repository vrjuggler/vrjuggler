/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2005 by Iowa State University
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

#define PF_C_API 1

#include <vrj/Util/Debug.h>
#include <vrj/Draw/Pf/PfInputHandler.h>

namespace vrj
{

PfInputHandler::PfInputHandler(pfPipeWindow* pipeWindow, const std::string& displayName)
{
   mName = displayName;
   mPipeWindow = pipeWindow;
   mPipe = pfGetPWinPipe(mPipeWindow);
#ifdef VPR_OS_Win32
   mWinHandle = pipeWindow->getWSWindow();
   //mWinHandle = pfGetPWinWSWindow(pipeWindow);
#else
   // Get the XWindow from that we are going to recieve events from.
   mXWindow = pfGetPWinWSWindow(mPipeWindow);
#endif

   openConnection();
}

void PfInputHandler::openConnection()
{
#ifdef VPR_OS_Win32
   HWND hwnd;

   // Store pointer to old WndProcs
   hwnd = pfGetWinWSWindow(pfGetPWinSelect(mPipeWindow));
   if(hwnd == NULL)
   {
      displayWarningAboutNULLHWND("openWin32Input","Window");
      initializedWins = 0;
   }
   else
   {
      //mWinHandle = hwnd;
      mOldWndProc = SetWindowLong(hwnd, GWL_WNDPROC, (LONG) collectWin32InputWrapper);
   }
#else
   static Atom wm_protocols, wm_delete_window;
   
   if (!mPipe)
   {
      vprDEBUG(vprDBG_ERROR, vprDBG_CRITICAL_LVL)
         << clrOutNORM(clrRED, "ERROR:")
         << "pfInputHandler::openConnection() NULL mPipe!"
         << std::endl << vprDEBUG_FLUSH;
      return;
   }

   const char* str = pfGetPipeWSConnectionName(mPipe);

   mXDisplay = pfOpenWSConnection(str, false);
   if (NULL == mXDisplay)
   {
      vprDEBUG(vprDBG_ERROR, vprDBG_CRITICAL_LVL)
         << clrOutNORM(clrRED, "ERROR:")
         << "pfInputHandler::openConnection() Can't open display '" << str
         << "'\n" << vprDEBUG_FLUSH;
      return;
   }

   // XSync in order to ensure that window is initialized
   // for this connection.
   XSync(mXDisplay, false);

   static long event_mask = (FocusChangeMask | ExposureMask |
                             VisibilityChangeMask | /*StructureNotifyMask |*/
                             KeyPressMask | KeyReleaseMask | 
                             ButtonPressMask | ButtonReleaseMask |
                             PointerMotionMask);

   wm_protocols = XInternAtom(mXDisplay, "WM_PROTOCOLS", 1);
   wm_delete_window = XInternAtom(mXDisplay, "WM_DELETE_WINDOW", 1);

   XSetWMProtocols(mXDisplay, mXWindow, &wm_delete_window, 1);
   XSelectInput(mXDisplay, mXWindow, event_mask);
   XMapWindow(mXDisplay, mXWindow);
   XSync(mXDisplay,false);
#endif
}

#ifdef VPR_OS_Win32
LRESULT CALLBACK eventCallback(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
   int i;

   collectWin32Input(hwnd,uMsg,wParam,lParam);

   if(hwnd == mWinHandle)
   {
      return CallWindowProc(oldWndProc, hwnd, uMsg, wParam, lParam);
   }

   // Invoke the default window handler if none found.
   return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
#endif

#ifdef VPR_OS_Win32
void pfuCollectWin32EventStream(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
   HWND parentWindow = hwnd;
   LPTSTR tmpName[101];
   pfuWin32Event event;

   event.hwnd = hwnd; /* or should this too point to the parentWindow, if any? */
   event.uMsg = uMsg;
   event.wParam = wParam;
   event.lParam = lParam;
   event.time = pfGetTime();

   /* we want the inWin member of the pfuMouse to be == to the
   * HWND for the pipe window regardless of whether or not
   * the event occurred in the child or parent of the pipe win
   * In theory this variable is just a bool but it's used in
   * some sample programs to check the window id of the pipe
   * window. gui.c works with just a bool but ...
   */
   if(GetClassName(hwnd,tmpName,100) > 0)
   {
      if(!strcmp(tmpName,"pfChildWNDCLASS"))
      {
         parentWindow = (HWND)GetWindowLong(hwnd,GWL_HWNDPARENT);
      }
      if(parentWindow == NULL)
      {
         parentWindow = hwnd; /* and spew error message ... */
      }
   }
   else
   {
      vprDEBUG(vprDBG_ERROR, vprDBG_CRITICAL_LVL)
         << clrOutNORM(clrRED, "ERROR:")
         << " Unable to determine class name of win: " << std::hex << hwnd 
         << std::dec << std::endl << vprDEBUG_FLUSH;
   }


   // If we have a valid KeyboardMouseDevice, process
   // all keyboard/mouse events
   if ( NULL != mKeyboardMouseDevice )
   {
      // Forward events on to subclass. The magic of inheritance :)
      InputAreaWin32::updKeys( message );
   }
}
#else

void PfInputHandler::handleEvents()
{
   XEvent		event;

   /*
   // In Linux, XEventsQueued with QueuedAlready results in an
   // extreme amount of latency in SP mode since the event queue
   // only ever reports one event per call.  It's necessary to
   // change this to re-query the X server for more events after
   // the first one is pulled from the queue.  However this results
   // in a server roundtrip (in Linux) and probably causes a performance
   // hit.  So only being defined for LINUX
   */
#ifdef __linux__
   while (XPending(mXDisplay)) 
#else
   while (XEventsQueued(mXDisplay, QueuedAlready)) 
#endif
   {
      XNextEvent(mXDisplay, &event);
      
      switch (event.type) 
      {
      case ClientMessage:
         //if ((event.xclient.message_type == wm_protocols) &&
         //    (event.xclient.data.l[0] == wm_delete_window))
         {
            //events->devQ[events->numDevs] = PFUDEV_WINQUIT;
            //events->devVal[events->numDevs] = 1;
            //events->devCount[PFUDEV_WINQUIT] += 1;
            //events->numDevs++;
         } 
         break;
      default:
         // If we have a valid KeyboardMouseDevice, process
         // all keyboard/mouse events
         if ( NULL != mKeyboardMouseDevice )
         {
            // Forward events on to subclass. The magic of inheritance :)
            handleEvent(event);
         }
         break;
      }
   }
#endif /* VPR_OS_Win32 */
}

} // End of vrj namespace
