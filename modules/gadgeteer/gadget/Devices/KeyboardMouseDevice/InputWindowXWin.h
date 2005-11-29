/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2005 by Iowa State University
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

#ifndef _GADGET_EVENT_WINDOW_XWIN_H_
#define _GADGET_EVENT_WINDOW_XWIN_H_

#include <gadget/gadgetConfig.h>

#include <X11/Xutil.h>

#include <jccl/Config/ConfigElementPtr.h>

#include <gadget/Type/Input.h>
#include <gadget/Devices/KeyboardMouseDevice/InputAreaXWin.h>


namespace gadget
{

/** \class InputWindowXWin InputWindowXWin.h gadget/Devices/KeyboardMouseDevice/InputWindowXWin.h
 *
 * X Window System input window class.
 * This device is a source of keyboard events.  The device should not be
 * used directly, it should be referenced through proxies.
 *
 * There are two modes that this device can operate in:
 *
 * 1. Controlling local window
 *
 *    In this mode, the device opens and manages its own X11 window for
 *    getting input.
 *
 * 2. Connecting to remotely management window
 *
 *    In this mode, the device connects through X11 to a
 *    window (and display) that have been opened by
 *    another process (normally a GLX window from VR Juggler).
 *
 * Mouse Locking:
 *
 *  This device recieves input from the XWindows display.  As such,
 *  the xwindow must have focus to generate events.  In order to help
 *  users keep the window in focus, there are two cases where the
 *  driver will "lock" the mouse to the window, thus preventing loss of focus.
 *
 *  CASE 1: The user holds down any key (for example, a, b, ctrl, shift,
 *          etc.).<br>
 *  CASE 2: The user can toggle locking using a special "locking" key
 *          defined in the configuration element.
 *
 * @see KeyboardMouseDevice, KeyboardMouseProxy
 */
class InputWindowXWin : public InputAreaXWin, public Input
{
public:

   InputWindowXWin()
      : mVisual(NULL)
      , mScreen(-1)
      , mX(-1)
      , mY(-1)
      , mExitFlag(false)
   {
      mBlocking = true;
      vprASSERT(NULL == mThread);      // Should have been initialized in base constructor
   }

   ~InputWindowXWin()
   {
      //stopSampling();
   }

   virtual bool config(jccl::ConfigElementPtr e);

   /** Main thread of control for this active object. */
   void controlLoop(void* nullParam);

   /** Start the windows sampling. */
   virtual bool startSampling();

   /** Stop window sampling. */
   virtual bool stopSampling();

   /**
    * Processes the current x-events.
    * Called repetatively by the controlLoop.
    */
   virtual bool sample()
   {
      handleEvents();
      return true;
   }

   /** Update the keys and event queue data structures with current data. */
   virtual void updateData()
   {;}

   /** Return the element type associated with this device type. */
   static std::string getElementType();

   /**
    * Invokes the global scope delete operator.  This is required for proper
    * releasing of memory in DLLs on Win32.
    */
   void operator delete(void* p)
   {
      ::operator delete(p);
   }

protected:
   /**
    * Deletes this object.  This is an implementation of the pure virtual
    * gadget::Input::destroy() method.
    */
   void destroy()
   {
      delete this;
   }

   /** Do any extra event processing needed. */
   virtual void processEvent(XEvent)
   {
      /* Do nothing. */ ;
   }

private:
   /** @name X-Window System utility functions */
   //@{

   /** Opens the local X window to sample from. */
   bool openLocalWindow();

   /** Opens the X connection to remote X window to sample from. */
   bool setupRemoteWindow();

   void setupWindowWidthAndHeight();

   Window createWindow(Window parent, const unsigned int borderWidth);

   void setHints(Window window, char*  window_name, char*  icon_name,
                 char* class_name, char* class_type);
   //@}

protected:
   void createEmptyCursor(Display* display, Window root);

   ::XVisualInfo* mVisual;
   ::XSetWindowAttributes mSWA;

   // --- Used with local window --- //
   int mScreen;         /**< Screen ID */
   int mX;              /**< Origin X-coordinate */
   int mY;              /**< Origin Y-coordinate */

   /** @name KeyboardMouse state holders
    * @note This driver does not use the normal triple buffering mechanism.
    * Instead, it just uses a modified double buffering system.
    */
   //@{
   bool       mExitFlag;         /**< Should we exit? */
   //@}

   std::string mXDisplayString;  /**< The display string to use from systemDisplay config info. */
};

} // end namespace


#endif
