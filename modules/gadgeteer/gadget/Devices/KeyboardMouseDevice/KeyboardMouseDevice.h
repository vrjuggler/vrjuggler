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

#ifndef _GADGET_KEYBOARD_MOUSE_DEVICE_H_
#define _GADGET_KEYBOARD_MOUSE_DEVICE_H_

#include <gadget/gadgetConfig.h>

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/keysym.h>
#include <boost/concept_check.hpp>

#include <vpr/Sync/Mutex.h>

#include <gadget/Type/Input.h>
#include <gadget/Type/KeyboardMouse.h>
#include <gadget/Type/InputMixer.h>
#include <gadget/Type/KeyboardMouse/Event.h>

#include <jccl/Config/ConfigElementPtr.h>

#include <vpr/Util/Singleton.h>


namespace gadget
{

/**
 * X Winndow System event window class.
 * This device is a source of keyboard events.  The device should not be
 * used directly, it should be referenced through proxies.
 *
 * There are two modes that this device can operate in:
 *
 * 1. Controlling local window
 *
 *    In this mode, the device opens and manages it's own
 *    x window for getting input.
 *
 * 2. Connecting to remotely management window
 *
 *    In this mode, the device connects through X to a
 *    window (and display) that have been opened by
 *    another process (normally an GLX window from VR Juggler).
 *
 * Mouse Locking:<br>
 *    This device recieves input from the XWindows display.  As such,
 *  the xwindow must have focus to generate events.  In order to help
 *  users keep the window in focus, there are two cases where the
 *  driver will "lock" the mouse to the window, thus preventing loss of focus.<br>
 *  CASE 1: The user holds down any key. (ie. a,b, ctrl, shift, etc)<br>
 *  CASE 2: The user can toggle locking using a special "locking" key
 *           defined in the configuration element.
 *
 * @see KeyboardMouse, KeyboardMouseProxy
 */
class KeyboardMouseDevice : public InputMixer<Input,KeyboardMouse>
{
public:  // --- Internal helper class ----- //
   /** Holds list of registered windows that may be used for X-Input.
    * This is used by KeyboardMouse routines to find any windows
    * opened by other system components but that we still want to get input
    * from.
    */
   class KeyboardMouseDeviceRegistry
   {
   public:
      struct KeyboardMouseDeviceInfo
      {
         std::string    mSourceName;   /**< The KeyboardMouseDevice name. */
         KeyboardMouseDevice*   mKeyboardMouseDevice;   /**< The handle to the KeyboardMouseDevice. */
      };

   public:
      KeyboardMouseDeviceRegistry()
      {;}

      /** Add the given window to the registry.
       * @return true if window is added, false if matches existing window name.
       */
      bool addKeyboardMouseDevice(const std::string& name, KeyboardMouseDeviceInfo eventSourceInfo);

      /** Remove the window with the id of "name". */
      void removeKeyboardMouseDevice(const std::string& name);

      /** Get the window information. */
      bool getKeyboardMouseDevice(const std::string& name, KeyboardMouseDeviceInfo& eventSourceInfo);

   protected:
      typedef std::map<std::string, KeyboardMouseDeviceInfo> event_source_map_t;
      event_source_map_t    mKeyboardMouseDeviceMap;    /**< Map Window name to the data needed for it. */

      vprSingletonHeader(KeyboardMouseDeviceRegistry);
   };

public:

   KeyboardMouseDevice()
   {;}

   ~KeyboardMouseDevice()
   {
      stopSampling();
   }

   virtual bool config(jccl::ConfigElementPtr e);

   /** Start the windows sampling. */
   virtual bool startSampling()
   {
      return true;
   }

   /** Stop window sampling. */
   virtual bool stopSampling()
   {
      return true;
   }

   /** Dop nothing. */
   virtual bool sample()
   {
      // Do nothing
      return true;
   }

   /** Update the keys and event queue data structures with current data. */
   virtual void updateData();

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
private:
   float        mMouseSensitivity;

//XXX: Fix this!
public:
   vpr::Mutex   mKeysLock;         /**< Must hold this lock when accessing m_keys. */
   
   // NOTE: This driver does not use the normal triple buffering mechanism.
   // Instead, it just uses a modified double buffering system.
   
   /* Event window state holders */
   /**
    * (0,*): The num key presses during an UpdateData (ie. How many keypress
    * events).
    */
   int mKeys[gadget::LAST_KEY];

   /**
    * (0,1): The real keyboard state, all events processed (ie. what is the
    * key now).
    */
   int mRealkeys[gadget::LAST_KEY];
};


} // end namespace

#endif
