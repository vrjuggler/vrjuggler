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

#ifndef _VRJ_OPENGL_WINDOW_WIN32_H_
#define _VRJ_OPENGL_WINDOW_WIN32_H_

#include <vrj/Draw/OpenGL/Config.h>

#pragma warning(disable:4786)  // identifier truncated to 255 characters in the debug information

#include <GL/gl.h>

#include <gadget/Devices/KeyboardMouseDevice/InputAreaWin32.h>
#include <vrj/Draw/OpenGL/Window.h>
#include <vrj/Draw/OpenGL/ExtensionLoaderWGL.h>


namespace vrj
{

namespace opengl
{

// NOTE: Win32 message processing works like the following:
//
// - vrj::opengl::DrawManager calls checkEvents()
// - While there are events
//   - Translate the event
//   - If we have a valid KeyboardMouseDevice
//     - Handle the input from the window.
//   - Dispatch the message to WndPrc
//   - If WndProc can find vrj::opengl::WindowWin32
//     - Send message to vrj::opengl::WindowWin32
//       - Process all context specific stuff
//   - Else DefWindowProc()
//
// Note: We have to have a static registry since Win32 message
//       processing send messages to a static method of a class
//       and to handle certain messages like PAINT we need to
//       have context information contained in each instance
//       of vrj::opengl::WindowWin32.
// TODO: Try to find a way to process PAINT event at the same
//       time as input events.

/** \class vrj::opengl::WindowWin32 WindowWin32.h vrj/Draw/OpenGL/WindowWin32.h
 *
 * Win32-specific OpenGL windowing class.
 *
 * @note This class was renamed from vrj::GlWindowWin32 in VR Juggler 2.3.11.
 */
class VJ_OGL_CLASS_API WindowWin32
   : public vrj::opengl::Window
   , public gadget::InputAreaWin32
{
public:
   WindowWin32();

   virtual ~WindowWin32();

   /**
    * Opens the OpenGL window.
    *
    * @pre this has been configured.
    * @post Window created.  Create message sets the visual.
    *
    * @return true if the window opened correctly.
    */
   virtual bool open();

   /**
    * Closes the OpenGL window.
    *
    * @return true if the window closed correctly.
    * @note Must be called by the same thread that called open.
    */
   virtual bool close();

   /**
    * Sets the current OpenGL context to this window.
    * @post this.context is active context.
    */
   virtual bool makeCurrent();

   /** Performs an OpenGL swap buffers command. */
   virtual void swapBuffers();

   /**
    * Checks for window events.
    * @post All win32 events have ben dispatched and dealt with.
    */
   virtual void checkEvents();

   /**
    * Configure the vrj::opengl::Window using information from the Display.
    */
   void configWindow(vrj::DisplayPtr disp);

private:
   // WindowProcedure to deal with the events generated.
   // Called only for the window that we are controlling
   LRESULT handleEvent(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

   /**
    * Sets the pixel format for the given display context.
    * @return success
    */
   bool setPixelFormat(HDC hDC);

   void sizeChanged(long width, long height);

   // Register the window class with windows
   static bool registerWindowClass();
   static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam,
                                   LPARAM lParam);

   static WNDCLASS mWinClass;            /**< The window class to register */
   static bool     mWinRegisteredClass;  /**< Have we already registered */

   // Keep a map of window handles to OpenGL windows
   // This can be used by the message handler to call a class specific
   // message handler for each window

   /** Adds a window to the map. */
   static void addWindow(HWND handle, vrj::opengl::WindowWin32* glWin);

   /** Removes a window from the map. */
   static void removeWindow(HWND handle);

   /** Gets the OpenGL window we are dealing with. */
   static vrj::opengl::WindowWin32* getGlWin(HWND handle);

   static std::map<HWND, vrj::opengl::WindowWin32*> mGlWinMap;

   HGLRC mRenderContext;  /**< Permenant Rendering context */
   HDC   mDeviceContext;  /**< Private GDI Device context */
   ExtensionLoaderWGL mExtensions;   /**< Extensions for this window. */
   
   std::string  mWindowName;
   int          mPipe;
   std::string  mXDisplayName;       /**<  Name of the X11 display to use */
};

} // End of opengl namespace

} // End of vrj namespace


#endif
