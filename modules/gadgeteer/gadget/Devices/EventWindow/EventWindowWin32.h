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

#ifndef _GADGET_EVENT_WINDOW_WIN32_H_
#define _GADGET_EVENT_WINDOW_WIN32_H_

#include <gadget/gadgetConfig.h>

// Get windows stuff
#include <windows.h>
#include <commctrl.h>

#include <vpr/Thread/Thread.h>
#include <vpr/System.h>
#include <jccl/Config/ConfigElement.h>

#include <gadget/Type/Input.h>
#include <gadget/Type/EventWindow.h>
#include <gadget/Type/InputMixer.h>

#include <gadget/Type/EventWindow/Keys.h>
#include <gadget/Type/EventWindow/Event.h>

#include <gadget/Devices/EventWindow/InputAreaWin32.h>


namespace gadget
{

class GADGET_CLASS_API EventWindowWin32
#if _MSC_VER < 1310  // 1310 == VC++ 7.1
   : public Input, public EventWindow, public InputAreaWin32
#else
   : public InputMixer<Input, EventWindow>, public InputAreaWin32
#endif
{
public:
   EventWindowWin32()
      : mControlLoopDone(false),
        mExitFlag(false)
   {
		mEventDelegate = this;
   }

   virtual ~EventWindowWin32()
   {
      stopSampling();
   }

   virtual bool config(jccl::ConfigElementPtr e);

   /** Main thread of control for this active object. */
   void controlLoop( void* nullParam );

   /* Pure Virtuals required by gadget::Input. */
   bool startSampling();
   bool stopSampling();

   /**
    * Processes the current events.
    * Called repetatively by the controlLoop.
    */
   bool sample();
   void updateData();
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

   virtual void processEvent(UINT message, UINT wParam, LONG lParam)
   {
   }

   friend LONG APIENTRY MenuWndProc(HWND hWnd, UINT message, UINT wParam, LONG lParam);

   HINSTANCE   m_hInst;
   void createWindowWin32();

   /** @name Windows utility functions */
   //@{
   char* checkArgs(char* look_for);

   BOOL MenuInit(HINSTANCE hInstance);
   //@}

   // --- Used with remote window --- //
   std::string							mRemoteDisplayName; /**< Name of the remote display window (index in registry). */
	InputAreaWin32::InputAreaRegistry::InputAreaInfo	mRemoteInputAreaInfo;     /**< Info structure for remote window. */

   int          mScreen, mX, mY; /**< screen id, x-origin, y-origin. */

   bool     mExitFlag;           /**< Should we exit? */

   float mMouseSensitivity;
   int   mSleepTimeMS;            /**< Amount of time to sleep in milliseconds between updates. */
   bool  mControlLoopDone;
};

} // End of gadget namespace

#endif
