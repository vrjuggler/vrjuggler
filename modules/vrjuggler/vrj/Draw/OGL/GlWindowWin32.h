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

#ifndef _VRJ_GL_WINDOW_WIN32_H_
#define _VRJ_GL_WINDOW_WIN32_H_

#pragma warning(disable:4786)  // identifier truncated to 255 characters in the debug information

#include <GL/gl.h>

#include <gadget/Devices/Keyboard/KeyboardWin32.h>

#include <vrj/vrjConfig.h>
#include <vrj/Draw/OGL/GlWindow.h>

namespace vrj
{

class VJ_CLASS_API GlWindowWin32 : public GlWindow, public gadget::KeyboardWin32
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
   virtual int open();

   /**
    * Closes the OpenGL window.
    *
    * @return true if the window closed correctly.
    * @note Must be called by the same thread that called open.
    */
   virtual int close();

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

   void config( vrj::Display* disp );

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

   /**
    * Called with any events to process from win keyboard.
    * Called from seperate process (keyboard device update).
    */
   virtual void processEvent( UINT message, UINT wParam, LONG lParam );

   /** do the stuff needed to become a gadgeteer device.
    *  @pre can be called any time
    */
   void becomeKeyboardDevice();

   /** do the stuff to make this not a gadgeteer device. 
    *  @pre can be called any time
    */
   void removeKeyboardDevice();
public:
    HWND  mWinHandle;      /**< Window handle */
    HGLRC mRenderContext;  /**< Permenant Rendering context */
    HDC   mDeviceContext;  /**< Private GDI Device context */

    std::string    window_name;
    int            mPipe;
    std::string    mXDisplayName;       /**<  Name of the x display to use */

    PIXELFORMATDESCRIPTOR* mMatch;    /**< Points the the found pixel format */
    bool           mAreKeyboardDevice;  /**< Should we act as a keyboard device too? */
};

} // End of vrj namespace


#endif
