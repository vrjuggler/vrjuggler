/*
 * VRJuggler
 *   Copyright (C) 1997,1998,1999,2000
 *   Iowa State University Research Foundation, Inc.
 *   All Rights Reserved
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
 */


#ifndef _VJ_GLX_WIN_H
#define _VJ_GLX_WIN_H
//#pragma once

#include <vjConfig.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <GL/glx.h>

#include <Kernel/GL/vjGlWindow.h>
#include <Kernel/vjKernel.h>
#include <Kernel/vjDebug.h>
#include <Kernel/vjDisplay.h>

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
class vjGlxWindow: public vjGlWindow
{
public:
   vjGlxWindow();

   void swapBuffers();
   int open();
   int close();
   bool makeCurrent();

   void config(vjDisplay* _display);

public:  /**** Static Helpers *****/
   /* static */ virtual bool createHardwareSwapGroup(std::vector<vjGlWindow*> wins);

private:

   Display         *x_display;
   XVisualInfo     *visual_info;
   GLXContext      glx_context;
   Window          x_window;
   std::string     window_name;
   int             pipe;

   /* private member functions.  these get profoundly painful */
   XVisualInfo *GetGlxVisInfo (Display *display, int screen);

   //!PRE:  window is an xwindow under display
   //!POST: returns true if e is a mapnotify event for window, else false
   //!NOTE: this is a utility function for InitGfx,  used to wait
   //+       until a window has actually been mapped.
   static int EventIsMapNotify (Display *display,  XEvent *e,  XPointer window);

};

#endif
