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

#include <vrj/vrjConfig.h>

#include <iomanip>
#include <vector>
#include <X11/Xatom.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <GL/glx.h>

#include <vpr/vpr.h>
#include <jccl/Config/ConfigElement.h>

#include <vrj/Draw/OGL/GlWindow.h>
#include <vrj/Kernel/Kernel.h>
#include <vrj/Util/Debug.h>
#include <vrj/Display/Display.h>
#include <vrj/Display/DisplayManager.h>

#include <vrj/Draw/OGL/GlWindowXWin.h>
//#include <gadget/Devices/KeyboardMouseDevice/EventWindowXWin.h>

#include <stdexcept>

namespace
{
   class glwinx_OpenFailureException : public std::runtime_error
   {
   public:
      glwinx_OpenFailureException() throw()
         : std::runtime_error("Failed to open glx window")
      {;}
      ~glwinx_OpenFailureException() throw() {;}
   };
}

namespace vrj
{

GlWindowXWin::GlWindowXWin()
   : GlWindow()
//   , mXDisplay(NULL)
   , mVisualInfo(NULL)
   , mGlxContext(NULL)
//   , mXWindow(0)
   , mWindowName("")
   , mPipe(-1)
   , mXDisplayName(""),
   mEmptyCursorSet(false)
{
   mWindowIsOpen = false;
   mWindowWidth = mWindowHeight = -1;

   // XEvent processing is not blocking.
   mBlocking = false;
}

GlWindowXWin::~GlWindowXWin()
{
   close();
}

bool GlWindowXWin::open()
{
   /* attempts to open the glxWindow & create the gl context.  Does nothing
    * if the window is already open (& returns true).
    * returns true for success, false for failure.
    * The newly opened window will be set as the calling proccess'
    * current gl context.
    */

   ::XEvent map_event;
   ::XSetWindowAttributes w_attrib;
   int screen;
   ::XSizeHints *sizehints;
   ::XClassHint *classhint;
   unsigned long event_mask(0);    // Event masks to use
   bool ret_val(true);

   vprDEBUG(vrjDBG_DRAW_MGR, vprDBG_STATE_LVL)
      << "[vrj::GlWindowXWin::open()] Open window\n" << vprDEBUG_FLUSH;

   if ( mWindowIsOpen )
   {
      return true;
   }

   if ( mWindowWidth == -1 )
   {
      vprDEBUG(vprDBG_ERROR, vprDBG_CRITICAL_LVL)
         << clrOutNORM(clrRED,"ERROR:")
         << " [vrj::GlWindowXWin::open()] Window has not been configured\n"
         << vprDEBUG_FLUSH;
      return false;
   }

   if ( ! (mXDisplay = ::XOpenDisplay(mXDisplayName.c_str())) )
   {
      vprDEBUG(vprDBG_ERROR, vprDBG_CRITICAL_LVL)
         << clrOutNORM(clrRED,"ERROR:")
         << " [vrj::GlWindowXWin::open()] Unable to open display '"
         << mXDisplayName << "'.\n" << vprDEBUG_FLUSH;
      return false;
   }
   // Try initializing the window
   try
   {
      screen = DefaultScreen(mXDisplay);

      // get an XVisualInfo*, which we'll need below
      if ( (mVisualInfo = getGlxVisInfo(mXDisplay, screen)) == NULL )
      {
         vprDEBUG(vprDBG_ERROR, vprDBG_CRITICAL_LVL)
            << clrOutNORM(clrRED,"ERROR:") << " Failed to get a GLX visual\n"
            << vprDEBUG_FLUSH;
         throw glwinx_OpenFailureException();
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
            << " vrj::GlWindowXWin: XCreateColorMap failed on '"
            << mXDisplayName << "'.\n" << vprDEBUG_FLUSH;
         throw glwinx_OpenFailureException();
      }

      event_mask = ExposureMask | StructureNotifyMask | KeyPressMask | KeyReleaseMask |
                   ButtonPressMask | ButtonReleaseMask | ButtonMotionMask |
                   PointerMotionMask | StructureNotifyMask;
//      event_mask = ExposureMask | StructureNotifyMask;   // Don't request buttons or keys since that will be handled elsewhere
      w_attrib.event_mask = event_mask;
      w_attrib.border_pixel = 0x0;

      // get screen dimensions for translating window origin.
      ::XWindowAttributes winattrs;
      ::XGetWindowAttributes(mXDisplay, RootWindow(mXDisplay, screen),
                             &winattrs);

      // create window
      mXWindow = ::XCreateWindow(mXDisplay, RootWindow(mXDisplay, screen),
                                 mOriginX,
                                 winattrs.height - mOriginY - mWindowHeight,
                                 mWindowWidth, mWindowHeight, 0,
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
         throw glwinx_OpenFailureException();
      }

      createEmptyCursor(mXDisplay, mXWindow);
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
      ::XStringListToTextProperty(&(classhint->res_name), 1, &w_name);

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
      if ( ! mHasBorder )
      {
         vprDEBUG(vrjDBG_DRAW_MGR, vprDBG_HVERB_LVL)
            << "[vrj::GlWindowXWin::open()] Attempting to make window borderless"
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

      Atom net_wm_state = XInternAtom(mXDisplay, "_NET_WM_STATE\0", 0);
      if (None == net_wm_state)
      {
         vprDEBUG(vrjDBG_DRAW_MGR, vprDBG_CRITICAL_LVL)
            << clrOutNORM(clrRED,"ERROR:")
            << "vrj::GlWindowXWin: Could not get X atom for _NET_WM_STATE."
            << std::endl << vprDEBUG_FLUSH;
      }
      else
      {
         Atom fullscreen_hint = XInternAtom(mXDisplay, "_NET_WM_STATE_FULLSCREEN\0", 0);
         Atom above_hint = XInternAtom(mXDisplay, "_NET_WM_STATE_ABOVE\0", 0);
         Atom always_hint = XInternAtom(mXDisplay, "_NET_WM_STATE_STAYS_ON_TOP\0", 0);
         //Atom maxh_hint = XInternAtom(mXDisplay, "_NET_WM_STATE_MAXIMIZED_HORZ\0", 0);
         //Atom maxv_hint = XInternAtom(mXDisplay, "_NET_WM_STATE_MAXIMIZED_VERT\0", 0);
         //Atom modal_hint = XInternAtom(mXDisplay, "_NET_WM_STATE_MODAL\0", 0);

         Atom net_winstates[6] = { 0, 0, 0, 0, 0, 0 };
         int curr_winstate = 0;

         // Set full screen if specified.
         if (mIsFullScreen)
         {
            if (None == fullscreen_hint)
            {
               vprDEBUG(vrjDBG_DRAW_MGR, vprDBG_CRITICAL_LVL)
                  << clrOutNORM(clrRED,"ERROR:")
                  << "vrj::GlWindowXWin: Could not get X atom for _NET_WM_STATE_FULLSCREEN."
                  << std::endl << vprDEBUG_FLUSH;
            }
            else
            {
               net_winstates[curr_winstate++] = fullscreen_hint;
            }
         }

         // Set always on top if specified.
         if (mAlwaysOnTop)
         {
            if (None == above_hint || None == always_hint)
            {
               vprDEBUG(vrjDBG_DRAW_MGR, vprDBG_CRITICAL_LVL)
                  << clrOutNORM(clrRED,"ERROR:")
                  << "vrj::GlWindowXWin: Could not get X atoms for _NET_WM_STATE_ABOVE and _NET_WM_STATE_STAYS_ON_TOP."
                  << std::endl << vprDEBUG_FLUSH;
            }
            else
            {
               net_winstates[curr_winstate++] = above_hint;
               net_winstates[curr_winstate++] = always_hint;
            }
         }

         /*
         if (mMaximized)
         {
            if (None == maxh_hint || None == maxv_hint)
            {
               vprDEBUG(vrjDBG_DRAW_MGR, vprDBG_CRITICAL_LVL)
                  << clrOutNORM(clrRED,"ERROR:")
                  << "vrj::GlWindowXWin: Could not get X atoms for _NET_WM_STATE_MAXIMIZED_(HORZ/VERT)."
                  << std::endl << vprDEBUG_FLUSH;
            }
            else
            {
               net_winstates[curr_winstate++] = ATOM(_NET_WM_STATE_MAXIMIZED_HORZ);
               net_winstates[curr_winstate++] = ATOM(_NET_WM_STATE_MAXIMIZED_VERT);
            }
         }

         if (mModal)
         {
            if (None == model_hint)
            {
               vprDEBUG(vrjDBG_DRAW_MGR, vprDBG_CRITICAL_LVL)
                  << clrOutNORM(clrRED,"ERROR:")
                  << "vrj::GlWindowXWin: Could not get X atom for _NET_WM_STATE_MODAL."
                  << std::endl << vprDEBUG_FLUSH;
            }
            else
            {
               net_winstates[curr_winstate++] = modal_hint;
            }
         }
         */

         XChangeProperty(mXDisplay, mXWindow, net_wm_state, XA_ATOM,
                         32, PropModeReplace, (unsigned char *) net_winstates, curr_winstate);
      }
         
      /********************* Mapping Window **************************/

      /* Open the window, select the input events, and wait until mapped (XIfEvent) */
      ::XSelectInput(mXDisplay, mXWindow, event_mask);
      ::XMapWindow(mXDisplay, mXWindow);
      ::XIfEvent(mXDisplay, &map_event, eventIsMapNotify, (XPointer)mXWindow);
      ::XSync(mXDisplay, 0);

      vprDEBUG(vrjDBG_DRAW_MGR, vprDBG_VERB_LVL) << "vrj::GlWindowXWin: done mapping window\n"
                                  << vprDEBUG_FLUSH;

      /********************* OpenGL Context Stuff *********************/

      mGlxContext = glXCreateContext(mXDisplay, mVisualInfo, NULL, True);
      if ( NULL == mGlxContext )
      {
         vprDEBUG(vprDBG_ERROR, vprDBG_CRITICAL_LVL)
            << clrOutNORM(clrRED,"ERROR:") << "Couldn't create GlxContext for '"
            << mXDisplayName << "'\n" << vprDEBUG_FLUSH;
         throw glwinx_OpenFailureException();
      }

      mWindowIsOpen = true;

      if( mHideMouse )
      {
         XDefineCursor(mXDisplay, mXWindow, mEmptyCursor);
      }

      // ----------- Register this window with XEvent Device registry --------- //
      /*
      gadget::EventWindowXWin::WindowRegistry::WindowInfo xwin_info;
      xwin_info.displayName = mXDisplayName;
      xwin_info.xWindow = mXWindow;
      gadget::EventWindowXWin::WindowRegistry::instance()->addWindow(mVrjDisplay->getName(), xwin_info);
      */

      ret_val = true;
   }
   catch (glwinx_OpenFailureException& openFailed)
   {
      // close() is coincidentally safe to call on a partially-opened
      // GlWindowXWin, and will deallocate all the stuff we might have
      // allocated above.
      close();

      ret_val = false;
   }

   // Register extensions in this window
   makeCurrent();
   mExtensions.registerExtensions();

     // Check on using swap group
   jccl::ConfigElementPtr disp_sys_elt =
      DisplayManager::instance()->getDisplaySystemElement();
   bool use_swap_group = disp_sys_elt->getProperty<bool>("use_swap_group");

   if ( use_swap_group )
   {
      vprDEBUG_OutputGuard(vprDBG_ALL, vprDBG_CONFIG_STATUS_LVL,
                           "Attempting to set up GLX swap group.\n", "");
       
      // Try NV swap group extension
      if(mExtensions.hasSwapGroupNV())
      {
         vprDEBUG(vprDBG_ALL, vprDBG_CONFIG_STATUS_LVL)
            << "SwapGroupNV: " << mExtensions.hasSwapGroupNV() << std::endl
            << vprDEBUG_FLUSH;
         GLuint max_groups, max_barriers;
         mExtensions.glXQueryMaxSwapGroupsNV(mXDisplay, screen, &max_groups,
                                             &max_barriers);
         vprDEBUG(vprDBG_ALL, vprDBG_CONFIG_STATUS_LVL)
            << "Max groups: " << max_groups << " Max Barriers:" << max_barriers
            << std::endl << vprDEBUG_FLUSH;

         vprDEBUG(vprDBG_ALL, vprDBG_CONFIG_STATUS_LVL)
            << "Setting up NV swap group and barrier group. "
            << "Group: 1, Barrier: 1\n" << vprDEBUG_FLUSH;
         // For now, just assume both groups are group 1
         // Note: In the future this code may need to be refactored to be
         //   controlled from the GlPipe class since it is really the thing
         //   that would correspond to the group and could group the correct
         //   windows to a group id.
         mExtensions.glXJoinSwapGroupNV(mXDisplay, mXWindow, 1);
         mExtensions.glXBindSwapBarrierNV(mXDisplay, 1, 1);
      }
      else
      {
         vprDEBUG(vprDBG_ALL, vprDBG_WARNING_LVL)
            << "Could not detect any GLX extensions that support swap groups.\n"
            << vprDEBUG_FLUSH;
         vprDEBUG_NEXT(vprDBG_ALL, vprDBG_WARNING_LVL)
            << "Proceeding with no swap locking.\n" << vprDEBUG_FLUSH;
      }
   }

   return ret_val;
}

/**
 *  Closes the window given
 * @note this function mucks with the current rendering context
 */
bool GlWindowXWin::close()
{

   //vprASSERT( !mXfuncLock.test() && "Attempting to close a display window that is locked" );
   // Assert that we have not impllemented correct shutdown for the case that we
   // are an event source as well
   //vprASSERT(!mIsEventSource  && "Need to implement GLX window close with gadget::EventWindow");

   // Remove any event device from the input manager
   if ( true == mIsEventSource )
   {
      gadget::Input* dev_ptr = dynamic_cast<gadget::Input*>(this);

      // XXX: Possibly not the best way to remove this to input manager
      // - This will internally call stop sampling (and stupify and proxies)
      //   Note: The stop sampling will not do anything since the event window does not own the window
      vrj::Kernel::instance()->getInputManager()->removeDevice(dev_ptr);
   }

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

   mWindowIsOpen = false;    // We are closed now

   return true;

} /* close() */
// Buffer swapping method
void GlWindowXWin::swapBuffers()
{
   glXSwapBuffers(mXDisplay, mXWindow);
}
// Make context current
bool GlWindowXWin::makeCurrent()
{
   /* returns true for success,
    * false for failure (eg window not open or glXMakeCurrent fails)
    */
   if ( !mWindowIsOpen )
   {
      return false;
   }

   vprASSERT(mGlxContext != NULL);
   vprASSERT(mXWindow != 0);
   vprASSERT(mXDisplay != NULL);

   return glXMakeCurrent(mXDisplay, mXWindow, mGlxContext);
}

void GlWindowXWin::configWindow(vrj::Display* disp)
{
   const char neg_one_string[] = "-1";
   vrj::GlWindow::configWindow(disp);

   // Get the vector of display elements.
   jccl::ConfigElementPtr disp_sys_elt = DisplayManager::instance()->getDisplaySystemElement();
   jccl::ConfigElementPtr display_elt = disp->getConfigElement();

   // Get the lock and KeyboardMouseDevice information
   gadget::InputArea::config(display_elt);
   
   mWindowName = disp->getName();
   mPipe = disp->getPipe();
   vprASSERT(mPipe >= 0);

   // NOTE: ConfigElements return the default value for a property if a value is
   //       not present. So if a pipe string is not specified for this pipe then
   //       it gets the default value of "-1".
   mXDisplayName = disp_sys_elt->getProperty<std::string>("x11_pipes", mPipe);

   if ( mXDisplayName == neg_one_string )    // Use display env
   {
      const std::string DISPLAY_str("DISPLAY");    // DISPLAY_str[] = "DISPLAY";
      const char* d = getenv(DISPLAY_str.c_str());
      if ( NULL != d )
      {
         mXDisplayName = std::string( d );
      }
   }
   vprDEBUG(vrjDBG_DRAW_MGR, vprDBG_VERB_LVL)
      << "[vrj::GlWindowXWin::config] Display name is '" << mXDisplayName
      << "'" << std::endl << vprDEBUG_FLUSH;
}

void GlWindowXWin::checkEvents()
{
   // XXX: Move this all down sooner or later.
   if (NULL != mKeyboardMouseDevice)
   {
      handleEvents();
   }
   else
   {
      XEvent event;

      while( XPending(mXDisplay))
      {
         XNextEvent(mXDisplay,&event);

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

   if ( !glXQueryExtension(display, NULL, NULL) )
   {
      vprDEBUG(vprDBG_ERROR, vprDBG_CRITICAL_LVL)
         << clrOutNORM(clrRED, "ERROR:") << " X Display '" << mXDisplayName
         << "' doesn't support GLX.\n" << vprDEBUG_FLUSH;
      return NULL;
   }

   // Using 1 here requests the *largest* available size for each of these
   // buffers.  Refer to the glXChooseVisual() manual page for more details.
   int visual_id(-1);
   int red_size(1), green_size(1), blue_size(1), alpha_size(1), db_size(1),
       accum_red_size(1), accum_green_size(1), accum_blue_size(1),
       accum_alpha_size(1), stencil_size(1);
   // glXChooseVisual() will return the visual with the smallest number of
   // auxiliary buffers that meets or exceeds the requested count.
   int num_aux_bufs(0);
   bool want_fsaa(false);

   jccl::ConfigElementPtr gl_fb_elt = mVrjDisplay->getGlFrameBufferConfig();

   if ( gl_fb_elt.get() != NULL )
   {
      if ( gl_fb_elt->getVersion() < 2 )
      {
         vprDEBUG(vrjDBG_DRAW_MGR, vprDBG_WARNING_LVL)
            << clrOutBOLD(clrYELLOW, "WARNING:") << " Display window '"
            << mVrjDisplay->getName() << "'" << std::endl;
         vprDEBUG_NEXTnl(vrjDBG_DRAW_MGR, vprDBG_WARNING_LVL)
            << "has an out of date OpenGL frame buffer configuration."
            << std::endl;
         vprDEBUG_NEXTnl(vrjDBG_DRAW_MGR, vprDBG_WARNING_LVL)
            << "Expected version 2 but found version "
            << gl_fb_elt->getVersion() << ".\n";
         vprDEBUG_NEXTnl(vrjDBG_DRAW_MGR, vprDBG_WARNING_LVL)
            << "Default values will be used for some frame buffer settings.\n"
            << vprDEBUG_FLUSH;
      }

      visual_id        = gl_fb_elt->getProperty<int>("visual_id");
      red_size         = gl_fb_elt->getProperty<int>("red_size");
      green_size       = gl_fb_elt->getProperty<int>("green_size");
      blue_size        = gl_fb_elt->getProperty<int>("blue_size");
      alpha_size       = gl_fb_elt->getProperty<int>("alpha_size");
      num_aux_bufs     = gl_fb_elt->getProperty<int>("auxiliary_buffer_count");
      db_size          = gl_fb_elt->getProperty<int>("depth_buffer_size");
      stencil_size     = gl_fb_elt->getProperty<int>("stencil_buffer_size");
      accum_red_size   = gl_fb_elt->getProperty<int>("accum_red_size");
      accum_green_size = gl_fb_elt->getProperty<int>("accum_green_size");
      accum_blue_size  = gl_fb_elt->getProperty<int>("accum_blue_size");
      accum_alpha_size = gl_fb_elt->getProperty<int>("accum_alpha_size");
      want_fsaa        = gl_fb_elt->getProperty<bool>("fsaa_enable");
   }

   if ( visual_id != -1 )
   {
      vprDEBUG(vrjDBG_DRAW_MGR, vprDBG_CONFIG_LVL)
         << "Requesting visual 0x" << std::hex << visual_id << std::dec
         << " from GLX." << std::endl << vprDEBUG_FLUSH;

      XVisualInfo vinfo_template;
      long mask(VisualIDMask);
      int nitems;

      vinfo_template.visualid = visual_id;

      XVisualInfo* vi = XGetVisualInfo(display, mask, &vinfo_template, &nitems);

      // If we got a valid visual using the requested ID, test to see if it
      // supports stereo.  this->mInStereo needs to be set correctly based on
      // what is requested of the display window by the configuration and based
      // on what our chosen visual actually supports.
      if ( NULL != vi )
      {
         int has_stereo;

         if ( glXGetConfig(display, vi, GLX_STEREO, &has_stereo) == 0 )
         {
            mInStereo = (mVrjDisplay->isStereoRequested() &&
                         has_stereo == True);
         }
      }
      // If XGetVisualInfo(3) returned NULL, print an error message.
      else
      {
         vprDEBUG(vprDBG_ERROR, vprDBG_CRITICAL_LVL)
            << clrOutBOLD(clrRED, "ERROR:")
            << " Failed to get X11 visual info for visual ID 0x"
            << std::hex << visual_id << std::dec << std::endl
            << vprDEBUG_FLUSH;
         vprDEBUG_NEXT(vprDBG_ERROR, vprDBG_CRITICAL_LVL)
            << "Window '" << mVrjDisplay->getName() << "' cannot be opened"
            << std::endl << vprDEBUG_FLUSH;
      }

      return vi;
   }
   else
   {
      if ( red_size < 0 )
      {
         vprDEBUG(vrjDBG_DRAW_MGR, vprDBG_WARNING_LVL)
            << clrOutBOLD(clrYELLOW, "WARNING:")
            << " Color buffer red channel size was negative ("
            << red_size << ").  Setting to 1.\n" << vprDEBUG_FLUSH;
         red_size = 1;
      }

      if ( green_size < 0 )
      {
         vprDEBUG(vrjDBG_DRAW_MGR, vprDBG_WARNING_LVL)
            << clrOutBOLD(clrYELLOW, "WARNING:")
            << " Color buffer green channel size was negative ("
            << green_size << ").  Setting to 1.\n" << vprDEBUG_FLUSH;
         green_size = 1;
      }

      if ( blue_size < 0 )
      {
         vprDEBUG(vrjDBG_DRAW_MGR, vprDBG_WARNING_LVL)
            << clrOutBOLD(clrYELLOW, "WARNING:")
            << " Color buffer blue channel size was negative ("
            << blue_size << ").  Setting to 1.\n" << vprDEBUG_FLUSH;
         blue_size = 1;
      }

      if ( alpha_size < 0 )
      {
         vprDEBUG(vrjDBG_DRAW_MGR, vprDBG_WARNING_LVL)
            << clrOutBOLD(clrYELLOW, "WARNING:")
            << " Color buffer alpha channel size was negative ("
            << alpha_size << ").  Setting to 1.\n" << vprDEBUG_FLUSH;
         alpha_size = 1;
      }

      if ( num_aux_bufs < 0 )
      {
         vprDEBUG(vrjDBG_DRAW_MGR, vprDBG_WARNING_LVL)
            << clrOutBOLD(clrYELLOW, "WARNING:")
            << " Auxiliary buffer count was negative (" << num_aux_bufs
            << ").  Setting to 0.\n" << vprDEBUG_FLUSH;
         num_aux_bufs = 0;
      }

      if ( db_size < 0 )
      {
         vprDEBUG(vrjDBG_DRAW_MGR, vprDBG_WARNING_LVL)
            << clrOutBOLD(clrYELLOW, "WARNING:")
            << " Depth buffer size was negative (" << db_size
            << ").  Setting to 1.\n" << vprDEBUG_FLUSH;
         db_size = 1;
      }

      if ( stencil_size < 0 )
      {
         vprDEBUG(vrjDBG_DRAW_MGR, vprDBG_WARNING_LVL)
            << clrOutBOLD(clrYELLOW, "WARNING:")
            << " Stencil buffer size was negative (" << stencil_size
            << ").  Setting to 1.\n" << vprDEBUG_FLUSH;
         stencil_size = 1;
      }

      if ( accum_red_size < 0 )
      {
         vprDEBUG(vrjDBG_DRAW_MGR, vprDBG_WARNING_LVL)
            << clrOutBOLD(clrYELLOW, "WARNING:")
            << " Accumulation buffer red channel size was negative ("
            << accum_red_size << ").  Setting to 1.\n" << vprDEBUG_FLUSH;
         accum_red_size = 1;
      }

      if ( accum_green_size < 0 )
      {
         vprDEBUG(vrjDBG_DRAW_MGR, vprDBG_WARNING_LVL)
            << clrOutBOLD(clrYELLOW, "WARNING:")
            << " Accumulation buffer green channel size was negative ("
            << accum_green_size << ").  Setting to 1.\n" << vprDEBUG_FLUSH;
         accum_green_size = 1;
      }

      if ( accum_blue_size < 0 )
      {
         vprDEBUG(vrjDBG_DRAW_MGR, vprDBG_WARNING_LVL)
            << clrOutBOLD(clrYELLOW, "WARNING:")
            << " Accumulation buffer blue channel size was negative ("
            << accum_blue_size << ").  Setting to 1.\n" << vprDEBUG_FLUSH;
         accum_blue_size = 1;
      }

      if ( accum_alpha_size < 0 )
      {
         vprDEBUG(vrjDBG_DRAW_MGR, vprDBG_WARNING_LVL)
            << clrOutBOLD(clrYELLOW, "WARNING:")
            << " Accumulation buffer alpha channel size was negative ("
            << accum_alpha_size << ").  Setting to 1.\n" << vprDEBUG_FLUSH;
         accum_alpha_size = 1;
      }

      const unsigned int indent_level(2);
      const std::string indent_text(indent_level, ' ');
      const int pad_width_dot(40 - indent_level);
      vprDEBUG(vrjDBG_DRAW_MGR, vprDBG_CONFIG_LVL)
         << "OpenGL visual request settings for " << mVrjDisplay->getName()
         << ":\n";
      vprDEBUG_NEXTnl(vrjDBG_DRAW_MGR, vprDBG_CONFIG_LVL)
         << std::setiosflags(std::ios::left) << std::setfill('.')
         << indent_text << std::setw(pad_width_dot)
         << "Color buffer red size " << " " << red_size << std::endl;
      vprDEBUG_NEXTnl(vrjDBG_DRAW_MGR, vprDBG_CONFIG_LVL)
         << std::setiosflags(std::ios::left) << std::setfill('.')
         << indent_text << std::setw(pad_width_dot)
         << "Color buffer green size " << " " << green_size << std::endl;
      vprDEBUG_NEXTnl(vrjDBG_DRAW_MGR, vprDBG_CONFIG_LVL)
         << std::setiosflags(std::ios::left) << std::setfill('.')
         << indent_text << std::setw(pad_width_dot)
         << "Color buffer blue size " << " " << blue_size << std::endl;
      vprDEBUG_NEXTnl(vrjDBG_DRAW_MGR, vprDBG_CONFIG_LVL)
         << std::setiosflags(std::ios::left) << std::setfill('.')
         << indent_text << std::setw(pad_width_dot)
         << "Color buffer alpha size " << " " << alpha_size << std::endl;
      vprDEBUG_NEXTnl(vrjDBG_DRAW_MGR, vprDBG_CONFIG_LVL)
         << std::setiosflags(std::ios::left) << std::setfill('.')
         << indent_text << std::setw(pad_width_dot)
         << "Auxiliary buffer count " << " " << num_aux_bufs << std::endl;
      vprDEBUG_NEXTnl(vrjDBG_DRAW_MGR, vprDBG_CONFIG_LVL)
         << std::setiosflags(std::ios::left) << std::setfill('.')
         << indent_text << std::setw(pad_width_dot)
         << "Depth buffer size " << " " << db_size << std::endl;
      vprDEBUG_NEXTnl(vrjDBG_DRAW_MGR, vprDBG_CONFIG_LVL)
         << std::setiosflags(std::ios::left) << std::setfill('.')
         << indent_text << std::setw(pad_width_dot)
         << "Stencil buffer size " << " " << stencil_size << std::endl;
      vprDEBUG_NEXTnl(vrjDBG_DRAW_MGR, vprDBG_CONFIG_LVL)
         << std::setiosflags(std::ios::left) << std::setfill('.')
         << indent_text << std::setw(pad_width_dot)
         << "Accumulation buffer red size " << " " << accum_red_size
         << std::endl;
      vprDEBUG_NEXTnl(vrjDBG_DRAW_MGR, vprDBG_CONFIG_LVL)
         << std::setiosflags(std::ios::left) << std::setfill('.')
         << indent_text << std::setw(pad_width_dot)
         << "Accumulation buffer green size " << " " << accum_green_size
         << std::endl;
      vprDEBUG_NEXTnl(vrjDBG_DRAW_MGR, vprDBG_CONFIG_LVL)
         << std::setiosflags(std::ios::left) << std::setfill('.')
         << indent_text << std::setw(pad_width_dot)
         << "Accumulation buffer blue size " << " " << accum_blue_size
         << std::endl;
      vprDEBUG_NEXTnl(vrjDBG_DRAW_MGR, vprDBG_CONFIG_LVL)
         << std::setiosflags(std::ios::left) << std::setfill('.')
         << indent_text << std::setw(pad_width_dot)
         << "Accumulation buffer alpha size " << " " << accum_alpha_size
         << std::endl;
      vprDEBUG_CONTnl(vrjDBG_DRAW_MGR, vprDBG_CONFIG_LVL)
         << vprDEBUG_FLUSH;

      // Notes on viattrib:  by using 1 for GLX_RED_SIZE et.al. we ask
      // for the _largest_ available buffers.  If this fails,  we might
      // want to try setting alpha size to 0 (smallest possible, maybe 0)
      // which is required eg. for alpha on the indys.
      std::vector<int> viattrib;
      viattrib.push_back(GLX_DOUBLEBUFFER);
      viattrib.push_back(GLX_RGBA);
      viattrib.push_back(GLX_DEPTH_SIZE); viattrib.push_back(db_size);
      viattrib.push_back(GLX_RED_SIZE); viattrib.push_back(red_size);
      viattrib.push_back(GLX_GREEN_SIZE); viattrib.push_back(green_size);
      viattrib.push_back(GLX_BLUE_SIZE); viattrib.push_back(blue_size);

      // Record the index for the alpha attribute using the current size of the
      // vector *before* the attribute is actually added.
      const unsigned int alpha_attrib_index = viattrib.size();
      viattrib.push_back(GLX_ALPHA_SIZE); viattrib.push_back(alpha_size);

      viattrib.push_back(GLX_AUX_BUFFERS); viattrib.push_back(num_aux_bufs);
      viattrib.push_back(GLX_STENCIL_SIZE); viattrib.push_back(stencil_size);

      const unsigned int accum_red_attrib_index = viattrib.size();
      viattrib.push_back(GLX_ACCUM_RED_SIZE);
      viattrib.push_back(accum_red_size);

      const unsigned int accum_green_attrib_index = viattrib.size();
      viattrib.push_back(GLX_ACCUM_GREEN_SIZE);
      viattrib.push_back(accum_green_size);

      const unsigned int accum_blue_attrib_index = viattrib.size();
      viattrib.push_back(GLX_ACCUM_BLUE_SIZE);
      viattrib.push_back(accum_blue_size);

      const unsigned int accum_alpha_attrib_index = viattrib.size();
      viattrib.push_back(GLX_ACCUM_ALPHA_SIZE);
      viattrib.push_back(accum_alpha_size);

      // Enable full-screen anti-aliasing if it is available and it was
      // requested.
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

      // Record the index for the stereo attribute using the current size of
      // the vector *before* the attribute is actually added.  If stereo is
      // not added, this variable will just be ignored.
      const unsigned int stereo_attrib_index = viattrib.size();

      if ( mVrjDisplay->isStereoRequested() )
      {
         viattrib.push_back(GLX_STEREO);
         mInStereo = true;
      }
      else
      {
         mInStereo = false;
      }

      // Add terminator
      viattrib.push_back(None);

      XVisualInfo* vi(NULL);

      // First, see if we can get exactly what we want.
      if ( (vi = glXChooseVisual(display, screen, &viattrib[0])) != NULL )
      {
         return vi;
      }

      // If we have reached this point, our first attempt to get a visual
      // failed.  If stereo is enabled, we try disabling it and requesting
      // another visual.
      if ( mVrjDisplay->isStereoRequested() )
      {
         vprDEBUG(vrjDBG_DRAW_MGR, vprDBG_CRITICAL_LVL)
            << "WARNING: Could not get an OpenGL visual for '" << mXDisplayName
            << "'\n";
         vprDEBUG_NEXTnl(vrjDBG_DRAW_MGR, vprDBG_CRITICAL_LVL)
            << "with stereo rendering enabled; trying without.\n"
            << vprDEBUG_FLUSH;
         mInStereo = false;

         // GLX_USE_GL will be ignored by glXChooseVisual(3).
         viattrib[stereo_attrib_index] = GLX_USE_GL;

         if ( (vi = glXChooseVisual(display, screen, &viattrib[0])) != NULL )
         {
            return vi;
         }

         // Stereo must not have been the problem, re-enable it.
         viattrib[stereo_attrib_index] = GLX_STEREO;
      }

      // If we reached this point, we still do not have a usable GLX visual.
      // Disabling the accumulation buffer settings may give us something
      // usable.
      if ( accum_red_size > 0 || accum_green_size > 0 ||
           accum_blue_size > 0 || accum_alpha_size > 0 )
      {
         vprDEBUG(vrjDBG_DRAW_MGR, vprDBG_CRITICAL_LVL)
            << "WARNING: Could not get an OpenGL visual for '" << mXDisplayName
            << "'\n";
         vprDEBUG_NEXTnl(vrjDBG_DRAW_MGR, vprDBG_CRITICAL_LVL)
            << "with accumulation buffer settings; trying without.\n"
            << vprDEBUG_FLUSH;

         viattrib[accum_red_attrib_index + 1]   = 0;
         viattrib[accum_blue_attrib_index + 1]  = 0;
         viattrib[accum_green_attrib_index + 1] = 0;
         viattrib[accum_alpha_attrib_index + 1] = 0;

         // XXX: If this fails, should we restore the accumulation buffer
         // settings?
         if ( (vi = glXChooseVisual(display, screen, &viattrib[0])) != NULL )
         {
            return vi;
         }
      }

      // If we reached this point, we still do not have a usable GLX visual.
      // Disabling the alpha channel may give us something usable.
      vprDEBUG(vrjDBG_DRAW_MGR, vprDBG_CRITICAL_LVL)
         << "WARNING: Could not get an OpenGL visual for '" << mXDisplayName
         << "'\n";
      vprDEBUG_NEXTnl(vrjDBG_DRAW_MGR, vprDBG_CRITICAL_LVL)
         << "with a color buffer alpha channel; trying without.\n"
         << vprDEBUG_FLUSH;

      // There are two values in viattrib related to the alpha channel.
      viattrib[alpha_attrib_index    ] = GLX_USE_GL;
      viattrib[alpha_attrib_index + 1] = GLX_USE_GL;

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

         // There are four values in viattrib related to FSAA.
         viattrib[fsaa_attrib_index    ] = GLX_USE_GL;
         viattrib[fsaa_attrib_index + 1] = GLX_USE_GL;
         viattrib[fsaa_attrib_index + 2] = GLX_USE_GL;
         viattrib[fsaa_attrib_index + 3] = GLX_USE_GL;

         if ( (vi = glXChooseVisual(display, screen, &viattrib[0])) != NULL )
         {
            return vi;
         }
      }
#endif
   }

   // Failed, so return NULL
   return NULL;
}

/**
 * @pre  window is an xwindow under display.
 * @post returns true if e is a mapnotify event for window, else false.
 * @note this is a utility function for InitGfx,  used to wait
 *       until a window has actually been mapped.
 */
int GlWindowXWin::eventIsMapNotify(::Display* display, XEvent* e,
                                   XPointer window)
{
   boost::ignore_unused_variable_warning(display);
   return((e->type == MapNotify) && (e->xmap.window == (Window)window));
}

void GlWindowXWin::createEmptyCursor(::Display* display, ::Window root)
{
   Pixmap cursormask;
   XGCValues xgc;
   GC gc;
   XColor dummycolour;

   cursormask = XCreatePixmap(display, root, 1, 1, 1/*depth*/);
   xgc.function = GXclear;
   gc =  XCreateGC(display, cursormask, GCFunction, &xgc);
   XFillRectangle(display, cursormask, gc, 0, 0, 1, 1);
   dummycolour.pixel = 0;
   dummycolour.red = 0;
   dummycolour.flags = 04;
   mEmptyCursor = XCreatePixmapCursor(display, cursormask, cursormask,
                                      &dummycolour,&dummycolour, 0,0);
   XFreePixmap(display,cursormask);
   XFreeGC(display,gc);

   mEmptyCursorSet = true;
}


} // namespace vrj
