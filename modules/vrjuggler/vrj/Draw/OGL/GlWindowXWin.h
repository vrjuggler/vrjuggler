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

#ifndef _VRJ_GLX_WIN_H
#define _VRJ_GLX_WIN_H
//#pragma once

#include <vrj/vrjConfig.h>

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <GL/glx.h>

#include <vrj/Draw/OGL/GlWindow.h>
#include <vrj/Kernel/Kernel.h>
#include <vrj/Util/Debug.h>
#include <vrj/Display/Display.h>
#include <gadget/Devices/Keyboard/KeyboardXWin.h>


namespace vrj
{

// this simple motif-esqe definition was taken from GLUT
typedef struct {
#define MWM_HINTS_DECORATIONS   2
  long flags;
  long functions;
  long decorations;
  long input_mode;
} MotifWmHints;


//------------------------------------
//: A GLX specific glWindow
//------------------------------------
// Has all information specific
// to dealing with a GLX window
// in OpenGL
//------------------------------------
class GlWindowXWin: public vrj::GlWindow, public gadget::KeyboardXWin
{
public:
    GlWindowXWin();
    ~GlWindowXWin();

    void swapBuffers();
    int open();
    int close();
    bool makeCurrent();

   void config(vrj::Display* disp);

public:  /**** Static Helpers *****/
   /* static */ virtual bool createHardwareSwapGroup(std::vector<vrj::GlWindow*> wins);

protected:
      /* private member functions.  these get profoundly painful */
   XVisualInfo *GetGlxVisInfo (::Display *display, int screen);

   //!PRE:  window is an xwindow under display
   //!POST: returns true if e is a mapnotify event for window, else false
   //!NOTE: this is a utility function for InitGfx,  used to wait
   //+       until a window has actually been mapped.
   static int EventIsMapNotify (::Display *display,  ::XEvent *e,  ::XPointer window);

   //: Called with any x-events to process
   // From X-win keyboard
   // Called from seperate process (keyboard device update)
   virtual void processEvent(::XEvent event);

private:
   ::Display*       x_display;
   ::XVisualInfo*   visual_info;
   ::GLXContext     glx_context;
   ::Window         x_window;
   std::string    window_name;
   int            mPipe;
   std::string    mXDisplayName;       //: Name of the x display to use
   bool           mAreKeyboardDevice;  // Should we act as a keyboard device to
};


} // end namespace
#endif
