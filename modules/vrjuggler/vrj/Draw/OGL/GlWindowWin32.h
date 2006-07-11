/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2006 by Iowa State University
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

#ifndef _VRJ_GL_WINDOW_WIN32_H_
#define _VRJ_GL_WINDOW_WIN32_H_

#include <vrj/Draw/OGL/Config.h>

#pragma warning(disable:4786)  // identifier truncated to 255 characters in the debug information

#include <GL/gl.h>

#include <gadget/Devices/KeyboardMouseDevice/InputAreaWin32.h>
#include <vrj/Draw/OGL/GlWindow.h>
#include <vrj/Draw/OGL/GlExtensionLoaderWin32.h>

namespace vrj
{

// NOTE: Win32 message processing works like the following:
//
// - GlDrawManager calls checkEvents()
// - While there are events
//   - Translate the event
//   - If we have a valid KeyboardMouseDevice
//     - Handle the input from the window.
//   - Dispatch the message to WndPrc
//   - If WndProc can find GlWindowWin32
//     - Send message to GlWindowWin32
//       - Process all context specific stuff
//   - Else DefWindowProc()
//
// Note: We have to have a static registry since Win32 message
//       processing send messages to a static method of a class
//       and to handle certain messages like PAINT we need to
//       have context information contained in each instance
//       of GlWindowWin32.
// TODO: Try to find a way to process PAINT event at the same
//       time as input events.

/** \class GlWindowWin32 GlWindowWin32.h vrj/Draw/OGL/GlWindowWin32.h
 *
 * Win32-specific OpenGL windowing class.
 */
class VJ_OGL_CLASS_API GlWindowWin32
   : public GlWindow
   , public gadget::InputAreaWin32
{
public:
   GlWindowWin32();
   virtual ~GlWindowWin32();

public:

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
    * Configure the GlWindow using information from the Display.
	 */
   void configWindow( vrj::Display* disp );

protected:
   // WindowProcedure to deal with the events generated.
   // Called only for the window that we are controlling
   LRESULT handleEvent(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

   /**
    * Sets the pixel format for the given display context.
    * @return success
    */
   bool setPixelFormat(HDC hDC);

   void sizeChanged(long width, long height);

protected:
   // Register the window class with windows
   static bool registerWindowClass();
   static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam,
                                   LPARAM lParam);

   static WNDCLASS mWinClass;            /**< The window class to register */
   static bool     mWinRegisteredClass;  /**< Have we already registered */

protected:
   // Keep a map of window handles to ogl windows
   // This can be used by the message handler to call a class specific
   // message handler for each window

   /** Adds a window to the map. */
   static void addWindow(HWND handle, GlWindowWin32* glWin);

   /** Removes a window from the map. */
   static void removeWindow(HWND handle);

   /** Gets the glWin we are dealing with. */
   static GlWindowWin32* getGlWin(HWND handle);

   static std::map<HWND, GlWindowWin32*> mGlWinMap;

private:
   HGLRC mRenderContext;  /**< Permenant Rendering context */
   HDC   mDeviceContext;  /**< Private GDI Device context */
   GlExtensionLoaderWin32   mExtensions;   /**< Extensions for this window. */
   
   std::string  mWindowName;
   int          mPipe;
   std::string  mXDisplayName;       /**<  Name of the X11 display to use */
};

} // End of vrj namespace


#endif
