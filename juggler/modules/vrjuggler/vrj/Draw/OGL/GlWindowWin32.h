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

#ifndef _VRJ_GL_WINDOW_WIN32_H_
#define _VRJ_GL_WINDOW_WIN32_H_

#pragma warning(disable:4786)  // identifier truncated to 255 characters in the debug information

#include <vrj/vrjConfig.h>
#include <GL/gl.h>
#include <vrj/Draw/OGL/GlWindow.h>

namespace vrj
{

class VJ_CLASS_API GlWindowWin32 : public GlWindow
{
public:
   GlWindowWin32()
   {
        match = NULL;       // Initialize all values to null
        hWnd = NULL;
        hRC = NULL;
        hDC = NULL;
   }

public:

   //: Open the OpenGL window
   //! PRE: this has been configured
    // POST: Window created
    //          Create message sets the visual
    //! RETURN: true - window opened correctly
   virtual int open();

   //: Close the OpenGL window
    //! NOTE: Must be called by the same thread that called open
    //! RETURN: true - window closed correctly
   virtual int close();

   //: Sets the current OpenGL context to this window
   //! POST: this.context is active context
   virtual bool makeCurrent();

   //: Performs an OpenGL swap buffers command
   virtual void swapBuffers();

   //: Check for window events
   //! POST: All win32 events have ben dispatched and dealt with
   virtual void checkEvents();

protected:
    // WindowProcedure to deal with the events generated.
    // Called only for the window that we are controlling
    LRESULT handleEvent(HWND    hWnd,
                              UINT  message,
                              WPARAM    wParam,
                              LPARAM    lParam);

    // Set the pixel format for the given display context
    //! RETURN: success
    bool setPixelFormat(HDC hDC);

    void sizeChanged(long width, long height);

protected:
    // Register the window class with windows
    static bool registerWindowClass();
    static LRESULT CALLBACK WndProc(    HWND    hWnd,
                                                UINT    message,
                                                WPARAM  wParam,
                                                LPARAM  lParam);

    static WNDCLASS winClass;           // The window class to register
    static bool      mWinRegisteredClass;   // Have we already registered

protected:
    // Keep a map of window handles to ogl windows
    // This can be used by the message handler to call a class specific
    // message handler for each window

    // Add a window to the map
    static void addWindow(HWND handle, GlWindowWin32* glWin);
    // Remove a window from the map
    static void removeWindow(HWND handle);
    // Get the glWin we are dealing with
    static GlWindowWin32* getGlWin(HWND handle);

   static std::map<HWND, GlWindowWin32*>        glWinMap;

public:
    HWND    hWnd;       // Window handle
    HGLRC hRC;      // Permenant Rendering context
    HDC hDC;        // Private GDI Device context

    PIXELFORMATDESCRIPTOR* match;       // Points the the found pixel format
};





};
#endif

