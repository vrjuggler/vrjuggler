/*
 *  File:	    $Name$
 *  Date modified:  $Date$
 *  Version:	    $Revision$
 *
 *
 *                                VR Juggler
 *                                    by
 *                              Allen Bierbaum
 *                             Christopher Just
 *                            Carolina Cruz-Neira
 *                               Albert Baker
 *
 *                         Copyright  - 1997,1998,1999
 *                Iowa State University Research Foundation, Inc.
 *                            All Rights Reserved
 */


#ifndef _VJ_GLX_WIN_H
#define _VJ_GLX_WIN_H
#pragma once

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
