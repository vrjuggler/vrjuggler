/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2009 by Iowa State University
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

#ifndef _GADGET_EVENT_WINDOW_WIN32_H_
#define _GADGET_EVENT_WINDOW_WIN32_H_

#include <gadget/gadgetConfig.h>

// Get windows stuff
#include <windows.h>
#include <commctrl.h>

#include <jccl/Config/ConfigElementPtr.h>

#include <gadget/Type/Input.h>
#include <gadget/Devices/KeyboardMouseDevice/InputAreaWin32.h>


namespace gadget
{

/** \class InputWindowWin32 InputWindowWin32.h gadget/Devices/KeyboardMouseDevice/InputWindowWin32.h
 *
 * Basic Win32 input window type.
 */
class GADGET_CLASS_API InputWindowWin32
   : public Input, public InputAreaWin32
{
public:
   InputWindowWin32()
      : mControlLoopDone(false)
      , mExitFlag(false)
   {
      mBlocking = true;
   }

   virtual ~InputWindowWin32()
   {
      stopSampling();
   }

   virtual bool config(jccl::ConfigElementPtr e);

   /** Main thread of control for this active object. */
   void controlLoop();

   /* Pure Virtuals required by gadget::Input. */

  /**
   * Create a win32 window and start a thread
   * processing it's messages.
   */
   bool startSampling();
   bool stopSampling();

   /**
    * Process the current window events.
    *
    * @note - Called repeatedly by the controlLoop.
    */
   bool sample();
   
   /**
    * Do nothing since we are only sending events to
    * a KeyboardMouseDevice.
    */
   void updateData()
   {;}
   
   static std::string getElementType();

protected:
   virtual void processEvent(UINT, UINT, LONG)
   {
      /* Do nothing. */ ;
   }

   /** 
    * Window function for the main application window. Processes all 
    * the menu selections and oter messages.
    */
   friend LONG APIENTRY MenuWndProc(HWND, UINT, UINT, LONG);

   HINSTANCE   m_hInst;
   void createWindowWin32();

   /** @name Windows utility functions */
   //@{
   char* checkArgs(char* look_for);

   BOOL MenuInit(HINSTANCE hInstance);
   //@}

   // --- Used with remote window --- //
   /** Name of the remote display window (index in registry). */
   std::string mRemoteDisplayName;

   int   mScreen;           /**< Screen ID */
   int   mX;                /**< Origin X-coordinate */
   int   mY;                /**< Origin X-coordinate */
   bool  mExitFlag;         /**< Should we exit? */
   bool  mControlLoopDone;
};

} // End of gadget namespace

#endif
