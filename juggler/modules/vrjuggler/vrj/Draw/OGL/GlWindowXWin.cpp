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

#include <vrj/vrjConfig.h>

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <GL/glx.h>

#include <vpr/vpr.h>
#include <vrj/Draw/OGL/GlWindow.h>
#include <vrj/Kernel/Kernel.h>
#include <vrj/Util/Debug.h>
#include <vrj/Display/Display.h>
#include <vrj/Display/DisplayManager.h>
#include <jccl/Config/ConfigChunk.h>

#include <vrj/Draw/OGL/GlWindowXWin.h>


namespace vrj
{

GlWindowXWin::GlWindowXWin():GlWindow() {
   window_is_open = false;
   window_width = window_height = -1;
   x_display = NULL;
   visual_info = NULL;
   glx_context = NULL;
   window_name = std::string("");
   mPipe = -1;
   mXDisplayName = std::string("");
   mAreKeyboardDevice = false;
   gadget::KeyboardXWin::mShared = true;       // Window is shared
}



GlWindowXWin::~GlWindowXWin() {
    close();
}



void GlWindowXWin::swapBuffers()
{
vpr::Guard<vpr::Mutex> xguard(mXfuncLock);

   glXSwapBuffers (x_display,  x_window);
}



int GlWindowXWin::open()
{
vpr::Guard<vpr::Mutex> xguard(mXfuncLock);

    /* attempts to open the glxWindow & create the gl context.  Does nothing
     * if the window is already open (& returns true).
     * returns true for success, false for failure.
     * The newly opened window will be set as the calling proccess'
     * current gl context.
     */

    ::XEvent fooevent;
    ::XSetWindowAttributes w_attrib;
    int screen;
    char* foo;
    ::XSizeHints *sizehints;
    ::XClassHint *classhint;

    vprDEBUG(vrjDBG_DRAW_MGR,3) << "glxWindow: Open window\n" << vprDEBUG_FLUSH;

    if (window_is_open)
        return true;

    if (window_width == -1) {
        vprDEBUG(vprDBG_ERROR, 0) << clrOutNORM(clrRED,"ERROR:") << "vjGlWindowXWin: Window has not been configured\n" << vprDEBUG_FLUSH;
        return false;
    }

    if (! (x_display = ::XOpenDisplay (mXDisplayName.c_str()))) {
        vprDEBUG(vprDBG_ERROR, 0) << clrOutNORM(clrRED,"ERROR:") << "vjGlWindowXWin: Unable to open display '" << mXDisplayName << "'.\n" << vprDEBUG_FLUSH;
        return false;
    }

    screen = DefaultScreen (x_display);

    // get an XVisualInfo*, which we'll need below
    if ((visual_info = GetGlxVisInfo (x_display, screen)) == NULL) {
        vprDEBUG(vprDBG_ERROR,0) << clrOutNORM(clrRED,"ERROR:") << "glXChooseVisual failed\n" << vprDEBUG_FLUSH;
        goto OPEN_FAIL;
    }

    // window attributes.
    if ((w_attrib.colormap = ::XCreateColormap (x_display,
                                              RootWindow(x_display, screen),
                                              visual_info->visual,
                                              AllocNone)) == 0) {
        vprDEBUG(vprDBG_ERROR,0) << clrOutNORM(clrRED,"ERROR:") << "vjGlWindowXWin: XCreateColorMap failed on '" << mXDisplayName << "'.\n" << vprDEBUG_FLUSH;
        goto OPEN_FAIL;
    }

    w_attrib.event_mask = ExposureMask | StructureNotifyMask
        | PointerMotionMask | KeyPressMask;
    w_attrib.border_pixel = 0x0;


    // get screen dimensions for translating window origin.
    ::XWindowAttributes winattrs;
    ::XGetWindowAttributes (x_display, RootWindow (x_display, screen), &winattrs);
//    std::cout << "------------------------------------------------------------------\n"
//          << "    screen dims: " << winattrs.width << ", " << winattrs.height
//          << "    win pos: " << origin_x << ", " <<  winattrs.height - origin_y - window_height << std::endl;


    // create window
    if ((x_window = ::XCreateWindow (x_display, RootWindow(x_display, screen),
                                   origin_x, winattrs.height - origin_y - window_height,
                                   window_width, window_height,
                                   0, visual_info->depth,
                                   InputOutput,
                                   visual_info->visual,
                                   CWEventMask | CWColormap | CWBorderPixel,
                                   /* ^--attrib mask*/
                                   &w_attrib))  /* Attributes */
        == 0)
        {
            vprDEBUG(vrjDBG_DRAW_MGR,0)
               << clrOutNORM(clrRED,"ERROR:")
               << "vjGlWindowXWin: Couldn't create window for " << mXDisplayName
               << std::endl << vprDEBUG_FLUSH;
            goto OPEN_FAIL;
        }

    /***************** Set Window Name/Class/Size/Pos *********************/

    /* Before we map the window, we need a name for it (this is also useful for
     * the resource cruft that'll get rid of the borders).
     */
    classhint = ::XAllocClassHint();
    classhint->res_name = (char*)window_name.c_str();
    classhint->res_class = "vj GLX";
    //XSetClassHint (x_display, x_window, classhint);

    // InSoc makes things simple
    // X makes things complicated
    ::XTextProperty w_name;
    foo = (char*)window_name.c_str();
    ::XStringListToTextProperty(&foo, 1, &w_name);

    /* The next few lines of crud are needed to get it through the window
     * manager's skull that yes, we DO have a REASON for wanting the window
     * to be positioned WHERE WE TOLD YOU TO, and not where YOU think it should
     * go, thank you very much, I'M the APPLICATION so stop $%^*&%#@! SECOND
     * GUESSING ME!
     */
    sizehints = XAllocSizeHints();
    sizehints->flags = USPosition;

    ::XSetWMProperties (x_display, x_window, &w_name, &w_name,
                      NULL, 0, sizehints, NULL, classhint);

    ::XFree (w_name.value);
    ::XFree (classhint);
    ::XFree (sizehints);

    /***************** Border Stuff ***************************/

    /* Get rid of window border, if configured to do so.
     * This technique doesn't require any modifications to the .XDefaults file
     * or anything, but it will only work with window managers based on MWM
     * (the Motif window manager).  That covers most cases.
     * Unfortunately, the generic X resources for communicating with a window
     * manager don't support this feature.
     */
    if (!border) {
        vprDEBUG(vrjDBG_DRAW_MGR,5) << "attempting to make window borderless"
                                  << std::endl << vprDEBUG_FLUSH;
        Atom MotifHints = XInternAtom (x_display, "_MOTIF_WM_HINTS", 0);
        if (MotifHints == None) {
            vprDEBUG(vrjDBG_DRAW_MGR,0)
               << clrOutNORM(clrRED,"ERROR:")
               << "vjGlWindowXWin: Could not get X atom for _MOTIF_WM_HINTS."
               << std::endl << vprDEBUG_FLUSH;
        }
        else {
            MotifWmHints hints;
            hints.flags = MWM_HINTS_DECORATIONS;
            hints.decorations = 0;
            XChangeProperty(x_display, x_window,
                            MotifHints, MotifHints, 32,
                            PropModeReplace, (unsigned char *) &hints, 4);
        }
    }

    /********************* Mapping Window **************************/

    /* Now that we've straightened that out with a minimum of bloodshed,
     * we can actually map the window.  The XIfEvent makes us wait until
     * it's actually on screen.
     */

    ::XMapWindow (x_display, x_window);
    ::XIfEvent (x_display, &fooevent, EventIsMapNotify, (XPointer)x_window);
    ::XSync(x_display,0);

    vprDEBUG(vrjDBG_DRAW_MGR,4) << "vjGlWindowXWin: done mapping window\n" << vprDEBUG_FLUSH;

    /********************* OpenGL Context Stuff *********************/

    glx_context = glXCreateContext (x_display,visual_info, NULL, True);
    if (NULL == glx_context) {
        vprDEBUG(vprDBG_ERROR,0) << clrOutNORM(clrRED,"ERROR:") << "Couldn't create GlxContext for '" << mXDisplayName << "'\n" << vprDEBUG_FLUSH;
        goto OPEN_FAIL;
    }

    window_is_open = true;

    // ----------- Keyboard device starting -------------- //
    if(true == mAreKeyboardDevice)     // Are we going to act like a keyboard device
    {
       // Set the parameters that we will need to get events
       gadget::KeyboardXWin::m_window = x_window;
       gadget::KeyboardXWin::m_visual = visual_info;
       gadget::KeyboardXWin::m_display = x_display;

       // Start up the device
       gadget::KeyboardXWin::startSampling();

       gadget::Input* dev_ptr = dynamic_cast<gadget::Input*>(this);

       // XXX: Possibly not the best way to add this to input manager
       vrj::Kernel::instance()->getInputManager()->addDevice(dev_ptr);
    }

    return true;

 OPEN_FAIL:
    // close() is coincidentally safe to call on a partially-opened
    // GlWindowXWin, and will deallocate all the stuff we might have
    // allocated above.
    close();
    return false;
}



//: Closes the window given
//! NOTE: this function mucks with the current rendering context */
int GlWindowXWin::close()
{
vpr::Guard<vpr::Mutex> xguard(mXfuncLock);

//     if (!window_is_open)
//         return true;

   if (glx_context)
   {
      makeCurrent();    // Might not need this
      //glFlush();      // This is done by the changing context
      glXMakeCurrent ( x_display, None, NULL);     // Release the context, and don't assign a new one
      glXDestroyContext (x_display,  glx_context);
      glx_context = NULL;
   }
   if (x_window)
   {
      ::XDestroyWindow (x_display, x_window);
      x_window = 0;
   }
   if (visual_info)
   {
      ::XFree (visual_info);
      visual_info = NULL;
   }
   if (x_display)
   {
      ::XCloseDisplay (x_display);
      x_display = NULL;
   }

   window_is_open = false;
   return true;

} /* close() */



bool GlWindowXWin::makeCurrent()
{
vpr::Guard<vpr::Mutex> xguard(mXfuncLock);

   /* returns true for success,
    * false for failure (eg window not open or glXMakeCurrent fails)
    */
   if (!window_is_open)
      return false;

   vprASSERT(glx_context != NULL);
   return glXMakeCurrent ( x_display, x_window, glx_context  );
}

void GlWindowXWin::config(vrj::Display* disp)
{
   vprDEBUG(vrjDBG_INPUT_MGR,0) << "vjGlWindowXWin::config: _display: " << (*disp)
                              << "\nConfig chunk:\n" << (*(disp->getConfigChunk()))
                              << std::endl << vprDEBUG_FLUSH;

   const char neg_one_STRING[] = "-1";
   vrj::GlWindow::config(disp);

    // Get the vector of display chunks
   jccl::ConfigChunkPtr dispSysChunk = DisplayManager::instance()->getDisplaySystemChunk();
   jccl::ConfigChunkPtr displayChunk = disp->getConfigChunk();

   window_name = disp->getName();
   mPipe = disp->getPipe();
   vprASSERT(mPipe >= 0);

   mXDisplayName = (std::string)dispSysChunk->getProperty("xpipes", mPipe);
   if(mXDisplayName == neg_one_STRING)    // Use display env
   {
       const std::string DISPLAY_str("DISPLAY");    // DISPLAY_str[] = "DISPLAY";
       mXDisplayName = std::string(getenv(DISPLAY_str.c_str()));
   }
   vprDEBUG(vrjDBG_DRAW_MGR,4) << "glxWindow::config: display name is: "
                             << mXDisplayName << std::endl << vprDEBUG_FLUSH;

   mAreKeyboardDevice = (bool)displayChunk->getProperty("act_as_keyboard_device");
   // if should have keyboard device
   if(true == mAreKeyboardDevice)
   {
      mAreKeyboardDevice = true;       // Set flag saying that we need to have the local device

      // Configure keyboard device portion
      jccl::ConfigChunkPtr keyboard_chunk = displayChunk->getProperty("keyboard_device_chunk");

      // Set the name of the chunk to the same as the parent chunk (so we can point at it)
      //keyboard_chunk->setProperty("name", (std::string)displayChunk->getProperty("name"));

      gadget::KeyboardXWin::config(keyboard_chunk);

      // Custom configuration
      gadget::KeyboardXWin::m_width = GlWindowXWin::window_width;
      gadget::KeyboardXWin::m_height = GlWindowXWin::window_height;

      mWeOwnTheWindow = false;      // Keyboard device does not own window
   }
}


/**** Static Helpers *****/
/* static */ bool GlWindowXWin::createHardwareSwapGroup(std::vector<GlWindow*> wins)
{
   // Convert to glx windows
   std::vector<GlWindowXWin*> glx_wins;
   unsigned int i;

   if(wins.size() <= 0)
      vprDEBUG(vprDBG_ERROR,0) << "WARNING: createHardwareSwapGroup called with no windows\n" << vprDEBUG_FLUSH;

   for(i=0;i<wins.size();i++)
   {
      GlWindowXWin* glx_win = dynamic_cast<GlWindowXWin*>(wins[i]);
      vprASSERT(glx_win != NULL);    // Make sure we have the right type
      glx_wins.push_back(glx_win);
   }

   // Create hardware group
#ifdef VPR_OS_IRIX
   for(i=0;i<glx_wins.size();i++)      // For each window
   {
      if(glx_wins[i] != this)    // If not me
      {                          //then add with me to the swap group
         glXJoinSwapGroupSGIX(x_display, x_window, glx_wins[i]->x_window);
      }
   }

#else
   vprDEBUG(vprDBG_ERROR,0) << "WARNING: createHardwareSwapGroup not supported.\n" << vprDEBUG_FLUSH;
#endif
   return true;
}


/***********************************************************/
/* private member functions.  these get profoundly painful */
/***********************************************************/
::XVisualInfo* GlWindowXWin::GetGlxVisInfo (::Display *display, int screen)
{
   /* pre:  screen is a screen on the current XDisplay, and
    *       XDisplay is already defined and valid.
    * post: returns a pointer to an XVisualInfo to be used for GLX.
    *       Note that it doesn't necessarily have _all_ of the requested
    *       features... it will attempt to get a visual in mono or
    *       without alpha if it's virst attempts fail.
    *       Returns NULL if it can't come up with a reasonably close
    *       XVisualInfo, or if the display in question doesn't support
    *       GLX
    */


   ::XVisualInfo *vi;
   const int MaxAttributes = 32;
   int viattrib[MaxAttributes] = {
      GLX_DOUBLEBUFFER,
      GLX_RGBA,
      GLX_DEPTH_SIZE, 1,
      GLX_RED_SIZE, 1,
      GLX_GREEN_SIZE, 1,
      GLX_BLUE_SIZE, 1,
      GLX_ALPHA_SIZE, 1,
   };
   int NumAttribs = 12;
   int AlphaAttribIndex = 11;

   /* Notes on viattrib:  by using 1 for GLX_RED_SIZE et.al. we ask
    * for the _largest_ available buffers.  If this fails,  we might
    * want to try setting alpha size to 0 (smallest possible, maybe 0)
    * which is required eg. for alpha on the indys.
    *
    * Implementation note: the code below makes assumptions about the
    * exact order of the arguments in viattrib.  Alter those, & you'll
    * Need to redo the indices used.
    */


   if (!glXQueryExtension (display, NULL, NULL))
   {
      vprDEBUG(vprDBG_ERROR,0) << clrOutNORM(clrRED, "ERROR:") << " X Display '"<< mXDisplayName <<
      "' doesn't support GLX.\n" << vprDEBUG_FLUSH;
      return NULL;
   }

   if (mDisplay->inStereo())
   {
      viattrib[NumAttribs++] = GLX_STEREO;
      in_stereo = true;
   }
   else
      in_stereo = false;

   // Add terminator
   viattrib[NumAttribs] = None;


   // first, see if we can get exactly what we want.
   if ( (vi = glXChooseVisual (display, screen, viattrib)) )
      return vi;

   // still no luck. if we were going for stereo, let's try without.
   if (mDisplay->inStereo())
   {
      vprDEBUG(vrjDBG_DRAW_MGR,0) << "WARNING: Display process for '" << mXDisplayName
                 << "' couldn't get display in stereo - trying mono.\n"
                 << vprDEBUG_FLUSH;
      in_stereo = false;
      viattrib[NumAttribs-1] = GLX_USE_GL; // should be a reasonable 'ignore' tag
      if( (vi = glXChooseVisual (display, screen, viattrib)) )
         return vi;
   }

   // if we reach here, we didn't.  Maybe we should make alpha optional.
   vprDEBUG(vrjDBG_DRAW_MGR,0) << "WARNING: Display process for '" << mXDisplayName
              << "' couldn't get display with alpha channel - trying without.\n"
              << vprDEBUG_FLUSH;
   viattrib[AlphaAttribIndex] = 0;
   if( (vi = glXChooseVisual (display, screen, viattrib)) )
      return vi;

   // But they told me to please just go f___ myself
   // You know you just can't win.  -d. gilmour
   return NULL;
}

//!PRE:  window is an xwindow under display
//!POST: returns true if e is a mapnotify event for window, else false
//!NOTE: this is a utility function for InitGfx,  used to wait
//+       until a window has actually been mapped.
int GlWindowXWin::EventIsMapNotify (::Display *display,  ::XEvent *e,  ::XPointer window) {

    return ((e->type == MapNotify) && (e->xmap.window == (Window)window));
}

void GlWindowXWin::processEvent(::XEvent event)
{
   switch (event.type)
   {
   case ConfigureNotify:
      updateOriginSize( vrj::GlWindow::origin_x, vrj::GlWindow::origin_y,
                        event.xconfigure.width, event.xconfigure.height );
      vrj::GlWindow::setDirtyViewport(true);
      break;

   default:
      break;
   }
}

} // namespace vrj
