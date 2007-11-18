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
   const unsigned required_definition_ver(1);

   if ( e->getVersion() < required_definition_ver )
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

   vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_CONFIG_LVL)
      << "gadget::InputWindowXWin: display_number: " << x_disp_num
      << std::endl << vprDEBUG_FLUSH;

   if ( NULL == disp_sys_elt.get() )
   {
      vprDEBUG(vprDBG_ERROR, vprDBG_CONFIG_LVL)
         << clrOutNORM(clrRED, "ERROR")
         << ": [gadget::InputWindowXWin::config()] display_system element "
         << "is NULL.\n" << vprDEBUG_FLUSH;
      return false;
   }

   if ( x_disp_num >= 0 && disp_sys_elt.get() != NULL )
   {
      if ( disp_sys_elt->getVersion() < 3 )
      {
         vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_WARNING_LVL)
            << clrOutBOLD(clrYELLOW, "WARNING") << ": Display system element '"
            << disp_sys_elt->getName() << "'" << std::endl;
         vprDEBUG_NEXTnl(gadgetDBG_INPUT_MGR, vprDBG_WARNING_LVL)
            << "         is out of date.\n";
         vprDEBUG_NEXTnl(gadgetDBG_INPUT_MGR, vprDBG_WARNING_LVL)
            << "         Expected version 3 but found version "
            << disp_sys_elt->getVersion() << ".  Pipe\n";
         vprDEBUG_NEXTnl(gadgetDBG_INPUT_MGR, vprDBG_WARNING_LVL)
            << "         configurations will not work.\n" << vprDEBUG_FLUSH;
      }

      mXDisplayString = disp_sys_elt->getProperty<std::string>("pipes",
                                                               x_disp_num);
      
      vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_CONFIG_LVL)
         << "[gadget::InputWindowXWin::config()] Display: " << mXDisplayString
         << std::endl << vprDEBUG_FLUSH;
   }
   else
   {
      mXDisplayString = std::string("-1");
   }

   if ( mXDisplayString.empty() ||
        strcmp(mXDisplayString.c_str(), neg_one_STRING) == 0 )
   {
      // Use the value of the DISPLAY environment variable.
      const std::string DISPLAY_str("DISPLAY");
      vpr::System::getenv(DISPLAY_str, mXDisplayString);
   }

   return true;
}

bool InputWindowXWin::startSampling()
{
   if ( NULL != mThread )
   {
      vprDEBUG(vprDBG_ERROR, vprDBG_CRITICAL_LVL)
         << clrOutNORM(clrRED, "ERROR")
         << ": [gadget::InputWindowXWin::startSampling] "
         << "Called when already sampling.\n" << vprDEBUG_FLUSH;
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
   if ( NULL != mThread )
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
   vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_STATE_LVL)
      << "[gadget::InputWindowXWin::controlLoop()] Thread started.\n"
      << vprDEBUG_FLUSH;

   while ( NULL == vpr::Thread::self() )
   {
      vpr::System::usleep(50);
      vprDEBUG(vprDBG_ALL, vprDBG_VERB_LVL)
         << "[gadget::InputWindowXWin::controlLoop()] "
         << "Waiting for (thread::self() != NULL)\n" << vprDEBUG_FLUSH;
   }
   mThread = vpr::Thread::self();

   // Open the local X window
   openLocalWindow();

   // Obtain width and height info from windows
   setupWindowWidthAndHeight();

   // Sync up with window
   XSync(mXDisplay, 0);

   // If we have initial locked, then we need to lock the system.
   if ( mLockState == Lock_LockKey )
   {
      vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_STATE_LVL)
         << "[gadget::InputWindowXWin::controlLoop()] "
         << "Mouse set to initial lock. Locking it now.\n" << vprDEBUG_FLUSH;
      lockMouse(NULL);                     // Lock the mouse
   }

   // Loop on updating
   while ( ! mExitFlag )
   {
      sample();
      vpr::System::usleep(mSleepTimeMS * 1000);
   }

   XFlush(mXDisplay);

   if ( mEmptyCursorSet )
   {
      XFreeCursor(mXDisplay, mEmptyCursor);
   }

   // Exit, cleanup code
   XDestroyWindow(mXDisplay, mXWindow);
   XCloseDisplay(mXDisplay);
}

/*****************************************************************/
/*****************************************************************/
/***********************X WINDOW STUFF****************************/
/*****************************************************************/
/*****************************************************************/
// Open the X window to sample from
bool InputWindowXWin::openLocalWindow()
{
   mXDisplay = XOpenDisplay(mXDisplayString.c_str());    // Open display on given XDisplay
   if ( NULL == mXDisplay )
   {
      vprDEBUG(vprDBG_ERROR, vprDBG_CRITICAL_LVL)
         <<  clrOutNORM(clrRED, "ERROR")
         << ": [gadget::InputWindowXWin::openTheWindow()] "
         << "Failed to open display '" << mXDisplayString << "'" << std::endl
         << vprDEBUG_FLUSH;
      return false;
   }

   mScreen = DefaultScreen(mXDisplay);

   XVisualInfo vi_template;
   vi_template.screen = mScreen;
   int num_visuals(0);

   XVisualInfo* vis_infos = XGetVisualInfo(mXDisplay, VisualScreenMask,
                                           &vi_template, &num_visuals);

   // Verify that we got at least one visual from XGetVisualInfo(3).
   if ( vis_infos != NULL && num_visuals >= 1 )
   {
      XVisualInfo* p_visinfo;
      int i;

      // Try to find a visual with color depth of at least 8 bits.  Having
      // such a visual ensures that the input windows at least have a
      // black background.
      for ( i = 0, p_visinfo = vis_infos; i < num_visuals; ++i, ++p_visinfo )
      {
         if ( p_visinfo->depth >= 8 )
         {
            mVisual = p_visinfo;
            break;
         }
      }

      // If we couldn't find a visual with at least 8-bit color, just use the
      // first one in the list.
      if ( i == num_visuals )
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
      return false;
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

   setHints(mXWindow, mInstName, mInstName, "VRJInputWindow",
            "VR Juggler Input Windows");

   XSelectInput(mXDisplay, mXWindow, event_mask);
   XMapWindow(mXDisplay, mXWindow);
   XFlush(mXDisplay);
   XRaiseWindow(mXDisplay, mXWindow);
   XClearWindow(mXDisplay, mXWindow);    // Try to clear the background

   vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_STATE_LVL)
      << "[gadget::InputWindowXWin::openTheWindow()] done." << std::endl
      << vprDEBUG_FLUSH;

   XFree(vis_infos);

   return true;
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

void InputWindowXWin::setHints(Window window, const std::string& windowName,
                               const std::string& iconName,
                               const std::string& className,
                               const std::string& classType)
{
   // NOTE: Below, the char* held by each of the std::string parameters to
   // this method is assigned to a value in an X structure. This is done
   // without copying the bytes; rather the pointer value is simply assigned.
   // This is safe because the X functions that receive the structures make
   // copies of char* input.

   XTextProperty w_name;
   XTextProperty i_name;
   int status;

   // Generate window and icon names.
   char* window_name = const_cast<char*>(windowName.c_str());
   status = XStringListToTextProperty(&window_name, 1, &w_name);

   if ( 0 == status )
   {
      vprDEBUG(vprDBG_ERROR, vprDBG_CRITICAL_LVL)
         << "Error allocating XTextProperty for window name (" << windowName
         << ")\n" << vprDEBUG_FLUSH;
   }

   char* icon_name = const_cast<char*>(iconName.c_str());
   status = XStringListToTextProperty(&icon_name, 1, &i_name);

   if ( 0 == status )
   {
      vprDEBUG(vprDBG_ERROR, vprDBG_CRITICAL_LVL)
         << "Error allocating TextProperty for icon name (" << iconName
         << ")\n" << vprDEBUG_FLUSH;
   }

   XSizeHints size_hints;
   size_hints.width       = mWidth;    // -- Obsolete in R4
   size_hints.height      = mHeight;   // -- Obsolete in R4
   size_hints.base_width  = mWidth;    // -- New in R4
   size_hints.base_height = mHeight;   // -- New in R4

   // Set up flags for all the fields we've filled in.
   size_hints.flags = USPosition | USSize | PBaseSize;

   // Assume that the window starts in "normal" (rather than iconic) state and
   // wants keyboard input.
   XWMHints wm_hints;
   wm_hints.initial_state = NormalState;
   wm_hints.input         = True;
   wm_hints.flags         = StateHint | InputHint;

   /* Fill in class name. */
   XClassHint class_hints;
   class_hints.res_name  = const_cast<char*>(className.c_str());
   class_hints.res_class = const_cast<char*>(classType.c_str());

   char** argv(NULL);
   int argc(0);
   XSetWMProperties(mXDisplay, window, &w_name, &i_name, argv, argc,
                    &size_hints, &wm_hints, &class_hints);

   XFree(w_name.value);
   XFree(i_name.value);
}

Window InputWindowXWin::createWindow(Window parent,
                                     const unsigned int borderWidth)
{
   // need screen size so we can convert origin from lower-left
   const XWindowAttributes winattrs = getDisplayAttributes();

   // Create the window.
   return XCreateWindow(mXDisplay, parent, mX, winattrs.height - mY - mHeight,
                        mWidth, mHeight, borderWidth, mVisual->depth,
                        InputOutput, mVisual->visual,
                        CWBackPixel | CWBorderPixel | CWColormap | CWEventMask,
                        &mSWA);
}

} // end namespace
