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

#include <vrj/vrjConfig.h>

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <GL/glx.h>
#include <vector>

#include <vpr/vpr.h>
#include <jccl/Config/ConfigChunk.h>

#include <vrj/Draw/OGL/GlWindow.h>
#include <vrj/Kernel/Kernel.h>
#include <vrj/Util/Debug.h>
#include <vrj/Display/Display.h>
#include <vrj/Display/DisplayManager.h>

#include <vrj/Draw/OGL/GlWindowXWin.h>


namespace vrj
{

GlWindowXWin::GlWindowXWin()
   : GlWindow(), mXDisplay(NULL), mVisualInfo(NULL), mGlxContext(NULL),
     mXWindow(0), mWindowName(""), mPipe(-1), mXDisplayName(""),
     mAreKeyboardDevice(false)
{
   window_is_open = false;
   window_width = window_height = -1;
}

GlWindowXWin::~GlWindowXWin()
{
   close();
}

int GlWindowXWin::open()
{
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
   unsigned long event_mask(0);    // Event masks to use

   vprDEBUG(vrjDBG_DRAW_MGR,3) << "glxWindow: Open window\n" << vprDEBUG_FLUSH;

   if ( window_is_open )
      return true;

   if ( window_width == -1 )
   {
      vprDEBUG(vprDBG_ERROR, vprDBG_CRITICAL_LVL)
         << clrOutNORM(clrRED,"ERROR:")
         << "vrj::GlWindowXWin: Window has not been configured\n"
         << vprDEBUG_FLUSH;
      return false;
   }

   if ( ! (mXDisplay = ::XOpenDisplay(mXDisplayName.c_str())) )
   {
      vprDEBUG(vprDBG_ERROR, vprDBG_CRITICAL_LVL)
         << clrOutNORM(clrRED,"ERROR:")
         << "vrj::GlWindowXWin: Unable to open display '" << mXDisplayName
         << "'.\n" << vprDEBUG_FLUSH;
      return false;
   }

   screen = DefaultScreen(mXDisplay);

   // get an XVisualInfo*, which we'll need below
   if ( (mVisualInfo = getGlxVisInfo(mXDisplay, screen)) == NULL )
   {
      vprDEBUG(vprDBG_ERROR, vprDBG_CRITICAL_LVL)
         << clrOutNORM(clrRED,"ERROR:") << "glXChooseVisual failed\n"
         << vprDEBUG_FLUSH;
      goto OPEN_FAIL;
   }

   vprDEBUG(vrjDBG_DRAW_MGR, vprDBG_CONFIG_LVL)
      << "Visual ID: 0x" << std::hex << mVisualInfo->visualid << std::dec
      << std::endl << vprDEBUG_FLUSH;

   // window attributes.
   w_attrib.colormap = ::XCreateColormap(mXDisplay,
                                         RootWindow(mXDisplay, screen),
                                         mVisualInfo->visual,
                                         AllocNone);

   if ( w_attrib.colormap == 0 )
   {
      vprDEBUG(vprDBG_ERROR, vprDBG_CRITICAL_LVL)
         << clrOutNORM(clrRED,"ERROR:")
         << "vrj::GlWindowXWin: XCreateColorMap failed on '" << mXDisplayName
         << "'.\n" << vprDEBUG_FLUSH;
      goto OPEN_FAIL;
   }

   event_mask = ExposureMask | StructureNotifyMask | KeyPressMask |
                KeyReleaseMask | ButtonPressMask | ButtonReleaseMask |
                ButtonMotionMask | PointerMotionMask | StructureNotifyMask;
   w_attrib.event_mask = event_mask;
   w_attrib.border_pixel = 0x0;

   // get screen dimensions for translating window origin.
   ::XWindowAttributes winattrs;
   ::XGetWindowAttributes(mXDisplay, RootWindow(mXDisplay, screen), &winattrs);

   // create window
   mXWindow = ::XCreateWindow(mXDisplay, RootWindow(mXDisplay, screen),
                              origin_x, winattrs.height - origin_y - window_height,
                              window_width, window_height, 0,
                              mVisualInfo->depth, InputOutput,
                              mVisualInfo->visual,
                              CWEventMask | CWColormap | CWBorderPixel,
                              &w_attrib);

   if ( 0 == mXWindow )
   {
      vprDEBUG(vrjDBG_DRAW_MGR, vprDBG_CRITICAL_LVL)
         << clrOutNORM(clrRED,"ERROR:")
         << "vrj::GlWindowXWin: Couldn't create window for " << mXDisplayName
         << std::endl << vprDEBUG_FLUSH;
      goto OPEN_FAIL;
   }

   /***************** Set Window Name/Class/Size/Pos *********************/

   /* Before we map the window, we need a name for it (this is also useful for
    * the resource cruft that'll get rid of the borders).
    */
   classhint = ::XAllocClassHint();
   classhint->res_name = (char*)mWindowName.c_str();
   classhint->res_class = "VRJ GLX";
   //XSetClassHint(mXDisplay, mXWindow, classhint);

   // InSoc makes things simple
   // X makes things complicated
   ::XTextProperty w_name;
   foo = (char*) mWindowName.c_str();
   ::XStringListToTextProperty(&foo, 1, &w_name);

   /* guarantee window position */
   sizehints = XAllocSizeHints();
   sizehints->flags = USPosition;

   ::XSetWMProperties(mXDisplay, mXWindow, &w_name, &w_name,
                      NULL, 0, sizehints, NULL, classhint);

   ::XFree(w_name.value);
   ::XFree(classhint);
   ::XFree(sizehints);

   /***************** Border Stuff ***************************/

   /* Get rid of window border, if configured to do so.
    * This technique doesn't require any modifications to the .XDefaults file
    * or anything, but it will only work with window managers based on MWM
    * (the Motif window manager).  That covers most cases.
    * Unfortunately, the generic X resources for communicating with a window
    * manager don't support this feature.
    */
   if ( !border )
   {
      vprDEBUG(vrjDBG_DRAW_MGR,5) << "attempting to make window borderless"
         << std::endl << vprDEBUG_FLUSH;
      Atom MotifHints = XInternAtom(mXDisplay, "_MOTIF_WM_HINTS", 0);
      if ( MotifHints == None )
      {
         vprDEBUG(vrjDBG_DRAW_MGR, vprDBG_CRITICAL_LVL)
            << clrOutNORM(clrRED,"ERROR:")
            << "vrj::GlWindowXWin: Could not get X atom for _MOTIF_WM_HINTS."
            << std::endl << vprDEBUG_FLUSH;
      }
      else
      {
         MotifWmHints hints;
         hints.flags = MWM_HINTS_DECORATIONS;
         hints.decorations = 0;
         XChangeProperty(mXDisplay, mXWindow, MotifHints, MotifHints, 32,
                         PropModeReplace, (unsigned char *) &hints, 4);
      }
   }

   /********************* Mapping Window **************************/

   /* Open the window, select the input events, and wait until mapped (XIfEvent) */
   ::XSelectInput(mXDisplay, mXWindow, event_mask);
   ::XMapWindow(mXDisplay, mXWindow);
   ::XIfEvent(mXDisplay, &fooevent, EventIsMapNotify, (XPointer)mXWindow);
   ::XSync(mXDisplay, 0);

   vprDEBUG(vrjDBG_DRAW_MGR,4) << "vrj::GlWindowXWin: done mapping window\n"
                               << vprDEBUG_FLUSH;

   /********************* OpenGL Context Stuff *********************/

   mGlxContext = glXCreateContext(mXDisplay, mVisualInfo, NULL, True);
   if ( NULL == mGlxContext )
   {
      vprDEBUG(vprDBG_ERROR, vprDBG_CRITICAL_LVL)
         << clrOutNORM(clrRED,"ERROR:") << "Couldn't create GlxContext for '"
         << mXDisplayName << "'\n" << vprDEBUG_FLUSH;
      goto OPEN_FAIL;
   }

   window_is_open = true;

   // ----------- Keyboard device starting -------------- //
   if ( true == mAreKeyboardDevice )     // Are we going to act like a keyboard device
   {
      // Set the parameters that we will need to get events
      gadget::KeyboardXWin::m_window = mXWindow;
      gadget::KeyboardXWin::m_visual = mVisualInfo;
      gadget::KeyboardXWin::m_display = mXDisplay;

      // Start up the device
      /*   Do it in out check event function
      gadget::KeyboardXWin::startSampling();
      */

      gadget::Input* dev_ptr = dynamic_cast<gadget::Input*>(this);

      // XXX: Possibly not the best way to add this to input manager
      // - What happens when the keyboard is removed at run-time???
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

/**
 *  Closes the window given
 * @note this function mucks with the current rendering context
 */
int GlWindowXWin::close()
{

   //vprASSERT( !mXfuncLock.test() && "Attempting to close a display window that is locked" );
   // Assert that we have not impllemented correct shutdown for the case that we
   // are a keyboard window as well
   vprASSERT(!mAreKeyboardDevice  && "Need to implement GLX window close with gadget-keyboard window");

   if ( mGlxContext )
   {
      makeCurrent();    // Might not need this
      //glFlush();      // This is done by the changing context
      glXMakeCurrent(mXDisplay, None, NULL);     // Release the context, and don't assign a new one
      glXDestroyContext(mXDisplay, mGlxContext);
      mGlxContext = NULL;
   }
   if ( mXWindow )
   {
      ::XDestroyWindow(mXDisplay, mXWindow);
      mXWindow = 0;
   }
   if ( mVisualInfo )
   {
      ::XFree(mVisualInfo);
      mVisualInfo = NULL;
   }
   if ( mXDisplay )
   {
      ::XCloseDisplay(mXDisplay);
      mXDisplay = NULL;
   }

   window_is_open = false;

   return true;

} /* close() */

void GlWindowXWin::swapBuffers()
{
   glXSwapBuffers(mXDisplay, mXWindow);
}

bool GlWindowXWin::makeCurrent()
{
   /* returns true for success,
    * false for failure (eg window not open or glXMakeCurrent fails)
    */
   if ( !window_is_open )
   {
      return false;
   }

   vprASSERT(mGlxContext != NULL);
   vprASSERT(mXWindow != 0);
   vprASSERT(mXDisplay != NULL);

   return glXMakeCurrent(mXDisplay, mXWindow, mGlxContext);
}

void GlWindowXWin::config(vrj::Display* disp)
{
   const char neg_one_STRING[] = "-1";
   vrj::GlWindow::config(disp);

   // Get the vector of display chunks
   jccl::ConfigChunkPtr dispSysChunk = DisplayManager::instance()->getDisplaySystemChunk();
   jccl::ConfigChunkPtr displayChunk = disp->getConfigChunk();

   mWindowName = disp->getName();
   mPipe = disp->getPipe();
   vprASSERT(mPipe >= 0);

   mXDisplayName = dispSysChunk->getProperty<std::string>("xpipes", mPipe);
   if ( mXDisplayName == neg_one_STRING )    // Use display env
   {
      const std::string DISPLAY_str("DISPLAY");    // DISPLAY_str[] = "DISPLAY";
      const char* d = getenv(DISPLAY_str.c_str());
      if ( NULL != d )
      {
         mXDisplayName = std::string( d );
      }
   }
   vprDEBUG(vrjDBG_DRAW_MGR,4) << "glxWindow::config: display name is: "
      << mXDisplayName << std::endl << vprDEBUG_FLUSH;

   mAreKeyboardDevice = displayChunk->getProperty<bool>("act_as_keyboard_device");
   // if should have keyboard device
   if ( true == mAreKeyboardDevice )
   {
      mAreKeyboardDevice = true;       // Set flag saying that we need to have the local device

      // Configure keyboard device portion
      jccl::ConfigChunkPtr keyboard_chunk =
         displayChunk->getProperty<jccl::ConfigChunkPtr>("keyboard_device_chunk");

      // Set the name of the chunk to the same as the parent chunk (so we can point at it)
      //keyboard_chunk->setProperty("name", displayChunk->getName();

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

   if ( wins.size() <= 0 )
   {
      vprDEBUG(vprDBG_ERROR, vprDBG_CRITICAL_LVL)
         << "WARNING: createHardwareSwapGroup called with no windows\n"
         << vprDEBUG_FLUSH;
   }

   for ( i = 0; i < wins.size(); ++i )
   {
      GlWindowXWin* glx_win = dynamic_cast<GlWindowXWin*>(wins[i]);
      vprASSERT(glx_win != NULL);    // Make sure we have the right type
      glx_wins.push_back(glx_win);
   }

   // Create hardware group
#ifdef VPR_OS_IRIX
   for ( i = 0; i < glx_wins.size(); ++i )      // For each window
   {
      // If not me then add with me to the swap group.
      if ( glx_wins[i] != this )
      {
         glXJoinSwapGroupSGIX(mXDisplay, mXWindow, glx_wins[i]->mXWindow);
      }
   }

#else
   vprDEBUG(vprDBG_ERROR, vprDBG_CRITICAL_LVL)
      << "WARNING: createHardwareSwapGroup not supported.\n" << vprDEBUG_FLUSH;
#endif
   return true;
}

void GlWindowXWin::checkEvents()
{
   if ( true == mAreKeyboardDevice )
   {
      gadget::KeyboardXWin::sample();    /** Sample from the xwindow (calls HandleEvents() )*/
   }

}

/***********************************************************/
/* private member functions.  these get profoundly painful */
/***********************************************************/
::XVisualInfo* GlWindowXWin::getGlxVisInfo(::Display *display, int screen)
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

   // Using 1 here requests the *largest* available size for each of these
   // buffers.  Refer to the glXChooseVisual() manual page for more details.
   int red_size(1), green_size(1), blue_size(1), alpha_size(1), db_size(1);
   bool want_fsaa(false);

   jccl::ConfigChunkPtr gl_fb_chunk = mDisplay->getGlFrameBufferConfig();

   if ( gl_fb_chunk.get() != NULL )
   {
      red_size   = gl_fb_chunk->getProperty<int>("redSize");
      green_size = gl_fb_chunk->getProperty<int>("greenSize");
      blue_size  = gl_fb_chunk->getProperty<int>("blueSize");
      alpha_size = gl_fb_chunk->getProperty<int>("alphaSize");
      db_size    = gl_fb_chunk->getProperty<int>("depthBufferSize");
      want_fsaa  = gl_fb_chunk->getProperty<bool>("fsaaEnable");

      if ( red_size < 0 )
      {
         vprDEBUG(vrjDBG_DRAW_MGR, vprDBG_WARNING_LVL)
            << "WARNING: Red channel size was negative, set to: " << red_size
            << ".  Setting to 1.\n" << vprDEBUG_FLUSH;
         red_size = 1;
      }

      if ( green_size < 0 )
      {
         vprDEBUG(vrjDBG_DRAW_MGR, vprDBG_WARNING_LVL)
            << "WARNING: Green channel size was negative, set to: "
            << green_size << ".  Setting to 1.\n" << vprDEBUG_FLUSH;
         green_size = 1;
      }

      if ( blue_size < 0 )
      {
         vprDEBUG(vrjDBG_DRAW_MGR, vprDBG_WARNING_LVL)
            << "WARNING: Blue channel size was negative, set to: " << blue_size
            << ".  Setting to 1.\n" << vprDEBUG_FLUSH;
         blue_size = 1;
      }

      if ( alpha_size < 0 )
      {
         vprDEBUG(vrjDBG_DRAW_MGR, vprDBG_WARNING_LVL)
            << "WARNING: Alpha channel size was negative, set to: "
            << alpha_size << ".  Setting to 1.\n" << vprDEBUG_FLUSH;
         alpha_size = 1;
      }

      if ( db_size < 0 )
      {
         vprDEBUG(vrjDBG_DRAW_MGR, vprDBG_WARNING_LVL)
            << "WARNING: Depth buffer size was negative, set to: " << db_size
            << ".  Setting to 1.\n" << vprDEBUG_FLUSH;
         db_size = 1;
      }
   }

   vprDEBUG(vrjDBG_DRAW_MGR, vprDBG_CONFIG_LVL)
      << "Frame buffer visual settings for " << mDisplay->getName()
      << ": R:" << red_size << " G:" << green_size << " B:" << blue_size
      << " A:" << alpha_size << " DB:" << db_size << std::endl
      << vprDEBUG_FLUSH;

   ::XVisualInfo *vi;
   std::vector<int> viattrib;
   viattrib.push_back(GLX_DOUBLEBUFFER);
   viattrib.push_back(GLX_RGBA);
   viattrib.push_back(GLX_DEPTH_SIZE); viattrib.push_back(db_size);
   viattrib.push_back(GLX_RED_SIZE); viattrib.push_back(red_size);
   viattrib.push_back(GLX_GREEN_SIZE); viattrib.push_back(green_size);
   viattrib.push_back(GLX_BLUE_SIZE); viattrib.push_back(blue_size);
   viattrib.push_back(GLX_ALPHA_SIZE); viattrib.push_back(alpha_size);
   const unsigned int AlphaAttribIndex = 11;

   // Enable full-screen anti-aliasing if it is available and it was requested.
#ifdef GLX_SAMPLES_SGIS
   // Save the current attribute vector size so we can try disabling FSAA if
   // necessary.
   const unsigned int fsaa_attrib_index = viattrib.size();

   if ( want_fsaa )
   {
      viattrib.push_back(GLX_SAMPLES_SGIS); viattrib.push_back(1);
      viattrib.push_back(GLX_SAMPLE_BUFFERS_SGIS); viattrib.push_back(1);
   }
#else
   if ( want_fsaa )
   {
      vprDEBUG(vrjDBG_DRAW_MGR, vprDBG_CONFIG_LVL)
         << "WARNING: Full-screen anti-aliasing is not available\n"
         << vprDEBUG_FLUSH;
   }
#endif

   /* Notes on viattrib:  by using 1 for GLX_RED_SIZE et.al. we ask
    * for the _largest_ available buffers.  If this fails,  we might
    * want to try setting alpha size to 0 (smallest possible, maybe 0)
    * which is required eg. for alpha on the indys.
    *
    * Implementation note: the code below makes assumptions about the
    * exact order of the arguments in viattrib.  Alter those, & you'll
    * Need to redo the indices used.
    */

   if ( !glXQueryExtension(display, NULL, NULL) )
   {
      vprDEBUG(vprDBG_ERROR, vprDBG_CRITICAL_LVL)
         << clrOutNORM(clrRED, "ERROR:") << " X Display '" << mXDisplayName
         << "' doesn't support GLX.\n" << vprDEBUG_FLUSH;
      return NULL;
   }

   if ( mDisplay->inStereo() )
   {
      viattrib.push_back(GLX_STEREO);
      in_stereo = true;
   }
   else
   {
      in_stereo = false;
   }

   // Add terminator
   viattrib.push_back(None);

   // first, see if we can get exactly what we want.
   if ( (vi = glXChooseVisual(display, screen, &viattrib[0])) != NULL )
   {
      return vi;
   }

   // still no luck. if we were going for stereo, let's try without.
   if ( mDisplay->inStereo() )
   {
      vprDEBUG(vrjDBG_DRAW_MGR, vprDBG_CRITICAL_LVL)
         << "WARNING: Display process for '" << mXDisplayName
         << "' couldn't get display in stereo - trying mono.\n"
         << vprDEBUG_FLUSH;
      in_stereo = false;
      viattrib[viattrib.size() - 1] = GLX_USE_GL; // should be a reasonable 'ignore' tag

      if ( (vi = glXChooseVisual(display, screen, &viattrib[0])) != NULL )
      {
         return vi;
      }
   }

   // if we reach here, we didn't.  Maybe we should make alpha optional.
   vprDEBUG(vrjDBG_DRAW_MGR, vprDBG_CRITICAL_LVL)
      << "WARNING: Display process for '" << mXDisplayName
      << "' couldn't get display with alpha channel - trying without.\n"
      << vprDEBUG_FLUSH;
   viattrib[AlphaAttribIndex] = 0;

   if ( (vi = glXChooseVisual(display, screen, &viattrib[0])) != NULL )
   {
      return vi;
   }

#ifdef GLX_SAMPLES_SGIS
   // Last-ditch effort: try disabling FSAA if it was enabled.
   // XXX: It might be better to try disabling FSAA *first* instead of last.
   if ( want_fsaa )
   {
      vprDEBUG(vrjDBG_DRAW_MGR, vprDBG_CRITICAL_LVL)
         << "WARNING: Display process for '" << mXDisplayName
 	 << "' couldn't get FSAA - trying without it.\n" << vprDEBUG_FLUSH;

      // Disabling is achieved by moving the terminator for the attribute
      // array up to the beginning of the FSAA attributes.  This effectively
      // blocks off anything that was added to the vector after the FSAA
      // attribute settings, so this isn't necessarily a good thing...
      viattrib[fsaa_attrib_index] = None;

      if ( (vi = glXChooseVisual(display, screen, &viattrib[0])) != NULL )
      {
         return vi;
      }
   }
#endif

   // Failed, so return NULL
   return NULL;
}

/**
 * @pre  window is an xwindow under display.
 * @post returns true if e is a mapnotify event for window, else false.
 * @note this is a utility function for InitGfx,  used to wait
 *       until a window has actually been mapped.
 */
int GlWindowXWin::EventIsMapNotify(::Display* display, ::XEvent* e,
                                   ::XPointer window)
{
   return((e->type == MapNotify) && (e->xmap.window == (Window)window));
}

void GlWindowXWin::processEvent(::XEvent event)
{
   switch ( event.type )
   {
      case ConfigureNotify:
         updateOriginSize(vrj::GlWindow::origin_x, vrj::GlWindow::origin_y,
                          event.xconfigure.width, event.xconfigure.height);
         vrj::GlWindow::setDirtyViewport(true);
         break;

      default:
         break;
   }
}

} // namespace vrj
