/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2011 by Iowa State University
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
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

#ifndef _VRJ_OPENGL_WINDOW_XWIN_H_
#define _VRJ_OPENGL_WINDOW_XWIN_H_

#include <vrj/Draw/OpenGL/Config.h>
#include <vector>

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <GL/glx.h>

#include <gadget/Devices/KeyboardMouseDevice/InputAreaXWin.h>
#include <vrj/Draw/OpenGL/Window.h>
#include <vrj/Draw/OpenGL/ExtensionLoaderGLX.h>


namespace vrj
{

namespace opengl
{

class Display;

/** This simple Motif-esque definition was taken from GLUT. */
typedef struct
{
#define MWM_HINTS_DECORATIONS 2
   long flags;
   long functions;
   long decorations;
   long input_mode;
} MotifWmHints;


/** \class vrj::opengl::WindowXWin WindowXWin.h vrj/Draw/OpenGL/WindowXWin.h
 *
 * A GLX-specific window for displaying OpenGL graphics.
 * It holds all the information specific to dealing with a GLX window.
 *
 * This class is responsbile for connecting to the X server, getting a GLX
 * visual, opening a GL window and associated context, and handling all
 * managment of that context.
 *
 * @note This class was renamed from vrj::GlWindowXWin in VR Juggler 2.3.11.
 */
class WindowXWin
   : public vrj::opengl::Window
   , public gadget::InputAreaXWin
{
public:
   WindowXWin();
   virtual ~WindowXWin();

   /**
    * Buffer swapping method.
    */
   virtual void swapBuffers();

   virtual bool open();

   /**
    * Closes this window.
    */
   virtual bool close();

   /**
    * Make context current.
    */
   bool makeCurrent();

   /**
    * Checks events.
    * Processes window systems events each frame.
    */
   virtual void checkEvents();
   
   /** Configure the window settings based on the display information. */
   void configWindow(vrj::DisplayPtr disp);   

protected:
   /** Create an OpenGL Context. */
   GLXContext createContext(::Display *dpy, GLXFBConfig fbc,
                            GLXContext shareList, Bool direct);

   /** Do any extra event processing needed. */
   virtual void processEvent(::XEvent event)
   {
      // Note: the first ConfigureNotify event after opening a window may 
      // contain an incorrect y position.  This causes a visual jump during
      // move or resize if auto corner update is enabled for any contained 
      // surface viewports.  To fix, we may need a different event mask on
      // window creation and a corresponding handler below.
      switch ( event.type )
      {
         case ConfigureNotify:
            updateOriginSize(event.xconfigure.x,
               mWinAttrs.height - mWindowHeight - event.xconfigure.y,
                  event.xconfigure.width, event.xconfigure.height);
            vrj::opengl::Window::setDirtyViewport(true);
            break;

         default:
            break;
      }
   }

   GLXFBConfig* getGlxFBConfig(::Display* display, int screen);

   /**
    * @pre  The window is an X11 window under display.
    * @post Returns true if e is a mapnotify event for window, else false.
    *
    * @note This is a utility function for open(). It is used to wait until a
    *       window has actually been mapped.
    */
   static int eventIsMapNotify(::Display* display, XEvent* e, XPointer window);

   void createEmptyCursor(::Display* display, ::Window root);

private:
   //::Display*   mXDisplay;
   XVisualInfo* mVisualInfo;
   GLXFBConfig* mFBConfig;
   GLXContext   mGlxContext;
   ExtensionLoaderGLX mExtensions;   /**< Extensions for this window. */

   //::Window     mXWindow;
   XWindowAttributes mWinAttrs;
   std::string  mWindowName;
   int          mPipe;
   std::string  mXDisplayName;       /**<  Name of the x display to use */

   Cursor mEmptyCursor;
   bool   mEmptyCursorSet;
};

} // end opengl namespace

} // end vrj namespace


#endif
