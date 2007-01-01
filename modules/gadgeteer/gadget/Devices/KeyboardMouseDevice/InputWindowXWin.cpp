/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2007 by Iowa State University
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
 *************** <auto-copyright.pl END do not edit this line> ***************/

#include <gadget/gadgetConfig.h>

#include <iomanip>
#include <boost/bind.hpp>

#include <vpr/vpr.h>
#include <vpr/System.h>
#include <vpr/Thread/Thread.h>
#include <jccl/Config/ConfigElement.h>
#include <jccl/RTRC/ConfigManager.h>

#include <gadget/Util/Debug.h>
#include <gadget/InputManager.h>
#include <gadget/Type/KeyboardMouse/KeyEvent.h>
#include <gadget/Type/KeyboardMouse/MouseEvent.h>

#include <gadget/Devices/KeyboardMouseDevice/InputWindowXWin.h> // my header


namespace gadget
{

std::string InputWindowXWin::getElementType()
{
   return std::string("input_window");
}

bool InputWindowXWin::config(jccl::ConfigElementPtr e)
{
   unsigned required_definition_ver(1);

   if (e->getVersion() < required_definition_ver)
   {
      vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_CRITICAL_LVL)
         << clrOutBOLD(clrRED, "ERROR")
         << ": [gadget::InputWindowXWin::config()] Element named '"
         << e->getName() << "'" << std::endl << vprDEBUG_FLUSH;
      vprDEBUG_NEXT(gadgetDBG_INPUT_MGR, vprDBG_CRITICAL_LVL)
         << "is version " << e->getVersion()
         << ", but we require at least version " << required_definition_ver
         << ". Ignoring.\n" << vprDEBUG_FLUSH;
      return false;
   }

   if ( ! ( InputArea::config(e) && Input::config(e) ) )
   {
      return false;
   }

   const char neg_one_STRING[] = "-1";

   // Get size and position
   mWidth  = e->getProperty<int>("size", 0);
   mHeight = e->getProperty<int>("size", 1);

   // Sanity checks.
   if (mWidth == 0)
   {
      mWidth = 400;
   }
   if (mHeight == 0)
   {
      mHeight = 400;
   }

   mX = e->getProperty<int>("origin", 0);
   mY = e->getProperty<int>("origin", 1);

   // Get the X display string
   int x_disp_num = e->getProperty<int>("display_number");
   jccl::ConfigElementPtr disp_sys_elt =
      gadget::InputManager::instance()->getDisplaySystemElement();

   vprDEBUG(vprDBG_ERROR,vprDBG_CONFIG_LVL)
         << "gadget::InputWindowXWin: display_number: " << x_disp_num << std::endl
         << vprDEBUG_FLUSH;
   
   if (NULL == disp_sys_elt.get())
   {
      vprDEBUG(vprDBG_ERROR,vprDBG_CONFIG_LVL)
         << clrOutNORM(clrRED,"ERROR")
         << ": gadget::InputWindowXWin: display_system element is NULL.\n"
         << vprDEBUG_FLUSH;
      return false;
   }

   
   if ((x_disp_num >= 0) && (disp_sys_elt.get() != NULL) )
   {
      mXDisplayString = disp_sys_elt->getProperty<std::string>("x11_pipes",
                                                               x_disp_num);
      
      vprDEBUG(vprDBG_ERROR,vprDBG_CONFIG_LVL)
            << "gadget::InputWindowXWin: display: " << mXDisplayString << std::endl
            << vprDEBUG_FLUSH;
   }
   else
   {
      mXDisplayString = std::string("-1");
   }

   if ((mXDisplayString.empty()) || (strcmp(mXDisplayString.c_str(), neg_one_STRING) == 0))    // Use display env
   {
      const std::string DISPLAY_str("DISPLAY");
      vpr::System::getenv(DISPLAY_str, mXDisplayString);
   }
   return true;
}

bool InputWindowXWin::startSampling()
{
   if (mThread != NULL)
   {
      vprDEBUG(vprDBG_ERROR,vprDBG_CRITICAL_LVL)
         << clrOutNORM(clrRED,"ERROR")
         << ": gadget::InputWindowXWin: startSampling called, when already sampling.\n"
         << vprDEBUG_FLUSH;
      vprASSERT(false);
   }

   bool started(false);
   mExitFlag = false;
   
   // Create a new thread to handle the control
   try
   {
      mThread = new vpr::Thread(boost::bind(&InputWindowXWin::controlLoop,
                                            this));
      started = true;
   }
   catch (vpr::Exception& ex)
   {
      vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_CRITICAL_LVL)
         << clrOutBOLD(clrRED, "ERROR")
         << ": Failed to spawn thread for X11 input window driver!\n"
         << vprDEBUG_FLUSH;
      vprDEBUG_NEXT(gadgetDBG_INPUT_MGR, vprDBG_CRITICAL_LVL)
         << ex.what() << std::endl << vprDEBUG_FLUSH;
   }

   return started;
}

bool InputWindowXWin::stopSampling()
{
   // If there is a thread for us and we actually own the window
   if (mThread != NULL)
   {
      mExitFlag = true;

      // Dummy event to get the event handling thread to respond. Since we
      // want the window to close anyway, this is a logical choice for the
      // dummy event.
      XUnmapWindow(mXDisplay, mXWindow);
      XFlush(mXDisplay);

      mThread->join();

      delete mThread;
      mThread = NULL;
   }

   return true;
}

// Main thread of control for this active object
void InputWindowXWin::controlLoop()
{
   vprDEBUG(gadgetDBG_INPUT_MGR,vprDBG_STATE_LVL)
      << "gadget::InputWindowXWin::controlLoop: Thread started.\n"
      << vprDEBUG_FLUSH;

   while (NULL == vpr::Thread::self())
   {
      vpr::System::usleep(50);
      vprDEBUG(vprDBG_ALL,vprDBG_VERB_LVL)
         << "gadget::InputWindowXWin: Waiting for (thread::self() != NULL)\n"
         << vprDEBUG_FLUSH;
   }
   mThread = (vpr::Thread*) vpr::Thread::self();

   // Open the local X window
   openLocalWindow();

   // Obtain width and height info from windows
   setupWindowWidthAndHeight();

   // Sync up with window
   XSync(mXDisplay, 0);

   // If we have initial locked, then we need to lock the system
   if (mLockState == Lock_LockKey)      // Means that we are in the initially locked state
   {
      vprDEBUG(gadgetDBG_INPUT_MGR,vprDBG_STATE_LVL)
         << "gadget::InputWindowXWin::controlLoop: Mouse set to initial lock. Locking it now.\n"
         << vprDEBUG_FLUSH;
      lockMouse( NULL );                     // Lock the mouse
   }

   // Loop on updating
   while (!mExitFlag)
   {
      sample();
      long usleep_time(1); // to be set...

      usleep_time = mSleepTimeMS*1000;

      vpr::System::usleep(usleep_time);
   }

   XFlush(mXDisplay);

   if ( mEmptyCursorSet )
   {
      XFreeCursor(mXDisplay, mEmptyCursor);
   }

   // Exit, cleanup code
   XDestroyWindow(mXDisplay, mXWindow);
   XCloseDisplay((::Display*) mXDisplay);
}


/*****************************************************************/
/*****************************************************************/
/***********************X WINDOW STUFF****************************/
/*****************************************************************/
/*****************************************************************/
// Open the X window to sample from
bool InputWindowXWin::openLocalWindow()
{
   int i;

   mXDisplay = XOpenDisplay(mXDisplayString.c_str());    // Open display on given XDisplay
   if (NULL == mXDisplay)
   {
      vprDEBUG(vprDBG_ERROR, vprDBG_CRITICAL_LVL)
         <<  clrOutNORM(clrRED, "ERROR")
         << ": [gadget::InputWindowXWin::openTheWindow()] "
         << "Failed to open display '" << mXDisplayString << "'" << std::endl
         << vprDEBUG_FLUSH;
      return 0;
   }

   mScreen = DefaultScreen(mXDisplay);

   XVisualInfo vTemplate, *vis_infos;
   long vMask = VisualScreenMask;
   vTemplate.screen = mScreen;
   int nVisuals;

   vis_infos = XGetVisualInfo(mXDisplay, vMask, &vTemplate, &nVisuals);

   // Verify that we got at least one visual from XGetVisualInfo(3).
   if ( vis_infos != NULL && nVisuals >= 1 )
   {
      XVisualInfo* p_visinfo;

      // Try to find a visual with color depth of at least 8 bits.  Having
      // such a visual ensures that the input windows at least have a
      // black background.
      for ( i = 0, p_visinfo = vis_infos; i < nVisuals; i++, p_visinfo++ )
      {
         if ( p_visinfo->depth >= 8 )
         {
            mVisual = p_visinfo;
            break;
         }
      }

      // If we couldn't find a visual with at least 8-bit color, just use the
      // first one in the list.
      if ( i == nVisuals )
      {
          mVisual = vis_infos;
      }
   }
   // If we didn't get a matching visual, we're in trouble.
   else
   {
      vprDEBUG(vprDBG_ERROR,vprDBG_CRITICAL_LVL)
         <<  clrOutNORM(clrRED,"ERROR")
         << ": [gadget::InputWindowXWin::openTheWindow()] find visual failed"
         << std::endl << vprDEBUG_FLUSH;
      return 0;
   }

   mSWA.colormap = XCreateColormap(mXDisplay,
                                   RootWindow(mXDisplay, mVisual->screen),
                                   mVisual->visual, AllocNone);
   mSWA.background_pixel = BlackPixel(mXDisplay, mScreen);
   mSWA.border_pixel = WhitePixel(mXDisplay, mScreen);
   const unsigned int event_mask =
      ExposureMask | StructureNotifyMask | KeyPressMask | KeyReleaseMask |
      ButtonPressMask | ButtonReleaseMask | ButtonMotionMask |
      PointerMotionMask | StructureNotifyMask;
   mSWA.event_mask = event_mask;

   mXWindow = createWindow(DefaultRootWindow(mXDisplay), 1);
   createEmptyCursor(mXDisplay, mXWindow);

   setHints(mXWindow, const_cast<char*>(mInstName.c_str()),
            "VJm_keys" , "VJInputWindow", "VJInputD");

   XSelectInput(mXDisplay, mXWindow, event_mask);
   XMapWindow(mXDisplay, mXWindow);
   XFlush(mXDisplay);
   XRaiseWindow(mXDisplay, mXWindow);
   XClearWindow(mXDisplay, mXWindow);    // Try to clear the background

   vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_STATE_LVL)
      << "[gadget::InputWindowXWin::openTheWindow()] done." << std::endl
      << vprDEBUG_FLUSH;

   XFree(vis_infos);

   return 1;
}

void InputWindowXWin::setupWindowWidthAndHeight()
{
   ::XWindowAttributes x_attr;
   ::XGetWindowAttributes(mXDisplay, mXWindow, &x_attr);
   mWidth = x_attr.width;
   mHeight = x_attr.height;
   mX = x_attr.x;
   mY = x_attr.y;
}

void InputWindowXWin::createEmptyCursor(Display* display, Window root)
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

/* Sets basic window manager hints for a window. */
void InputWindowXWin::setHints(Window window, char* window_name,
                               char* icon_name, char* class_name,
                               char* class_type)
{
    XTextProperty  w_name;
    XTextProperty  i_name;
    XSizeHints     sizehints;
    XWMHints       wmhints;
    XClassHint     classhints;
    int            status;

    /*
     * Generate window and icon names.
     */
   status = XStringListToTextProperty(&window_name, 1, &w_name);

   if (0 == status)
   {
      vprDEBUG(vprDBG_ERROR, vprDBG_CRITICAL_LVL)
         << "Error allocating XString\n" << vprDEBUG_FLUSH;
   }

   status = XStringListToTextProperty(&icon_name, 1, &i_name);

   if (0 == status)
   {
      vprDEBUG(vprDBG_ERROR, vprDBG_CRITICAL_LVL)
         << "Error allocating XString\n" << vprDEBUG_FLUSH;
   }

   sizehints.width       = mWidth;    /* -- Obsolete in R4 */
   sizehints.height      = mHeight;   /* -- Obsolete in R4 */
   sizehints.base_width  = mWidth;    /* -- New in R4 */
   sizehints.base_height = mHeight;   /* -- New in R4 */

   /* Set up flags for all the fields we've filled in. */
   sizehints.flags = USPosition | USSize | PBaseSize;

   /*   assume the window starts in "normal" (rather than
    *    iconic) state and wants keyboard input.
    */
   wmhints.initial_state = NormalState;
   wmhints.input         = True;
   wmhints.flags         = StateHint | InputHint;

   /* Fill in class name. */
   classhints.res_name  = class_name;
   classhints.res_class = class_type;

   XSetWMProperties(mXDisplay, window, &w_name, &i_name,
                    //argv, argc, /* Note reversed order. */
                    NULL, 0,
                    &sizehints, &wmhints, &classhints);

   XFree(w_name.value);
   XFree(i_name.value);
}

Window InputWindowXWin::createWindow(Window parent,
                                     const unsigned int borderWidth)
{
   Window window;

   // need screen size so we can convert origin from lower-left
   XWindowAttributes winattrs;
   XGetWindowAttributes(mXDisplay, RootWindow(mXDisplay, DefaultScreen(mXDisplay)),
                        &winattrs);

   // create it
   window = XCreateWindow(mXDisplay, parent, mX, winattrs.height - mY - mHeight,
                          mWidth, mHeight, borderWidth, mVisual->depth,
                          InputOutput, mVisual->visual,
                          CWBackPixel | CWBorderPixel | CWColormap | CWEventMask,
                          &mSWA);

   return window;
}

} // end namespace
