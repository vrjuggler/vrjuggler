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

#ifndef _VRJ_GLX_WIN_H
#define _VRJ_GLX_WIN_H
//#pragma once

#include <vrj/vrjConfig.h>
#include <vector>

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <GL/glx.h>

#include <vrj/Draw/OGL/GlWindow.h>
#include <gadget/Devices/EventWindow/EventWindowXWin.h>


namespace vrj
{

class Display;

/** this simple motif-esqe definition was taken from GLUT */
typedef struct
{
#define MWM_HINTS_DECORATIONS   2
   long flags;
   long functions;
   long decorations;
   long input_mode;
} MotifWmHints;


/**
 * A GLX specific glWindow.
 * Has all information specific to dealing with a GLX window in OpenGL.
 */
class GlWindowXWin: public vrj::GlWindow, public gadget::EventWindowXWin
{
public:
   GlWindowXWin();
   virtual ~GlWindowXWin();

   void swapBuffers();
   int open();
   int close();
   bool makeCurrent();

   /** Check events
   * @post If (areEventSource), checks the x-events and processes them
   */
   virtual void checkEvents();

   void configWindow(vrj::Display* disp);

public:  /**** Static Helpers *****/
   /* static */ virtual bool createHardwareSwapGroup(std::vector<vrj::GlWindow*> wins);

protected:
   /* private member functions.  these get profoundly painful */
   ::XVisualInfo* getGlxVisInfo(::Display* display, int screen);

   /**
    * @pre  window is an xwindow under display.
    * @post returns true if e is a mapnotify event for window, else false.
    * @note this is a utility function for InitGfx,  used to wait
    *       until a window has actually been mapped.
    */
   static int EventIsMapNotify(::Display* display, ::XEvent* e, ::XPointer window);

   /**
    * Called with any XEvents to process from X-win keyboard.
    * Called from seperate process (event window device update).
    */
   virtual void processEvent(::XEvent event);

private:
   ::Display*     mXDisplay;
   ::XVisualInfo* mVisualInfo;
   ::GLXContext   mGlxContext;
   ::Window       mXWindow;
   std::string    mWindowName;
   int            mPipe;
   std::string    mXDisplayName;       /**<  Name of the x display to use */
};


} // end namespace
#endif
