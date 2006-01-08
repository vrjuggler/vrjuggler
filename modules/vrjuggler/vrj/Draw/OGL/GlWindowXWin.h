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
#include <vrj/Draw/OGL/GlxExtensionLoader.h>
#include <gadget/Devices/KeyboardMouseDevice/InputAreaXWin.h>

namespace vrj
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
}
MotifWmHints;


/** \class GlWindowXWin GlWindowXWin.h vrj/Draw/OGL/GlWindowXWin.h
 *
 * A GLX-specific window for displaying OpenGL graphics.
 * It holds all the information specific to dealing with a GLX window.
 *
 * This class is responsbile for connecting to the X server, getting a GLX
 * visual, opening a GL window and associated context, and handling all
 * managment of that context.
 */
class GlWindowXWin: public vrj::GlWindow, public gadget::InputAreaXWin
{
public:
   GlWindowXWin();
   virtual ~GlWindowXWin();

   void swapBuffers();
   virtual bool open();
   virtual bool close();
   bool makeCurrent();

   /**
    * Checks events.
    * Processes window systems events each frame.
    */
   virtual void checkEvents();
   
   /** Configure the window settings based on the display information. */
   void configWindow(vrj::Display* disp);   

protected:
   /** Do any extra event processing needed. */
   virtual void processEvent(::XEvent event)
   {
      switch ( event.type )
      {
         case ConfigureNotify:
            updateOriginSize(vrj::GlWindow::mOriginX, vrj::GlWindow::mOriginY,
                             event.xconfigure.width, event.xconfigure.height);
            vrj::GlWindow::setDirtyViewport(true);
            break;

         default:
            break;
      }
   }


   XVisualInfo* getGlxVisInfo(::Display* display, int screen);

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
   GLXContext   mGlxContext;
   GlxExtensionLoader   mExtensions;   /**< Extensions for this window. */

   //::Window     mXWindow;
   std::string  mWindowName;
   int          mPipe;
   std::string  mXDisplayName;       /**<  Name of the x display to use */

   Cursor mEmptyCursor;
   bool   mEmptyCursorSet;
};


} // end namespace
#endif
