/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2009 by Iowa State University
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

#define PF_C_API 1

#include <vrj/Draw/Pf/Config.h>

#include <iostream>
#include <Performer/pf/pfPipeWindow.h>

#include <vrj/Util/Debug.h>
#include <vrj/Draw/Pf/InputHandler.h>

#ifndef GET_X_LPARAM
#  define GET_X_LPARAM(lp)   ((int)(short)LOWORD(lp))
#endif
#ifndef GET_Y_LPARAM
#  define GET_Y_LPARAM(lp)   ((int)(short)HIWORD(lp))
#endif

namespace vrj
{

namespace pf
{

InputHandler::InputHandler(pfPipeWindow* pipeWindow,
                           const std::string& displayName)
{
   mName = displayName;
   mPipeWindow = pipeWindow;
   mPipe = pfGetPWinPipe(mPipeWindow);
   mBlocking = false;
#ifdef VPR_OS_Windows
   mWinHandle = pipeWindow->getWSWindow();
#else
   // Get the XWindow from that we are going to recieve events from.
   mXWindow = pfGetPWinWSWindow(mPipeWindow);
   openConnection();
#endif
}

#ifndef VPR_OS_Windows
void InputHandler::openConnection()
{
   static Atom wm_protocols, wm_delete_window;

   if ( ! mPipe )
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

   XSetErrorHandler( errorHandler );
   //XWindowAttributes* attrib = 0;
   //XGetWindowAttributes( mXDisplay, mXWindow, attrib );

   //std::cout << attrib->x << std::endl
   //            << attrib->y << std::endl
   //            << attrib->width << std::endl
   //            << attrib->height << std::endl;

   // XSync in order to ensure that window is initialized
   // for this connection.
   XSync(mXDisplay, false);

   // Get the same events as the GLXWindow gets....
   unsigned long event_mask = ExposureMask | StructureNotifyMask |
                              KeyPressMask | KeyReleaseMask | ButtonPressMask |
                              ButtonReleaseMask | ButtonMotionMask |
                              PointerMotionMask | StructureNotifyMask;

   wm_protocols = XInternAtom(mXDisplay, "WM_PROTOCOLS", 1);
   wm_delete_window = XInternAtom(mXDisplay, "WM_DELETE_WINDOW", 1);

   XSetWMProtocols(mXDisplay, mXWindow, &wm_delete_window, 1);
   XSelectInput(mXDisplay, mXWindow, event_mask);
   XMapWindow(mXDisplay, mXWindow);
   XSync(mXDisplay,false);
}
#endif

void InputHandler::checkEvents()
{
   handleEvents();
}

#ifdef VPR_OS_Windows
LRESULT CALLBACK eventCallback(HWND hwnd, UINT message, WPARAM wParam,
                               LPARAM lParam)
{
   switch ( message )
   {
      // Catch the ALT key so that it does not open the system menu.
      case WM_SYSKEYDOWN:
      case WM_SYSKEYUP:
         break;
      case WM_SYSCOMMAND:
         return DefWindowProc(hwnd, message, wParam, lParam);
         break;
      case WM_DESTROY:
         PostQuitMessage(0);
         break;
      // Make sure that the resize event stays around until it is processed.
      case WM_SIZE:
         break;
      default:
         return DefWindowProc(hwnd, message, wParam, lParam);
   }
   return 0;
}
#endif

#ifndef VPR_OS_Windows
void InputHandler::handleEvents()
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

   while (XPending(mXDisplay)) 
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
}

int InputHandler::errorHandler(::Display*, XErrorEvent* e)
{
   char* errorOutput = new char[512];
   XGetErrorText(e->display, (int) e->error_code, errorOutput, 512);
   vprDEBUG(vprDBG_ERROR, vprDBG_CRITICAL_LVL)
      << clrOutNORM(clrRED, "ERROR")
      << ": [vrj::pf::InputHandler::errorHandler()] Caught X Error \n'"
      << errorOutput << "'\n" << vprDEBUG_FLUSH;
   delete [] errorOutput;
   return 0;
}
#endif /* VPR_OS_Windows */

} // End of pf namespace

} // End of vrj namespace
