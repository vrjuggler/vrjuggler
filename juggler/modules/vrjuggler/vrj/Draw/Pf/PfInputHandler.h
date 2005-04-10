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

#ifndef _VRJ_PF_INPUT_HANDLER_H_
#define _VRJ_PF_INPUT_HANDLER_H_

#include <vrj/vrjConfig.h>
#include <vrj/Util/Debug.h>

#ifndef VPR_OS_Win32
#  include <X11/Xlib.h>
#  include <X11/Xutil.h>
#endif

#include <jccl/Config/ConfigElement.h>
#include <jccl/Config/ConfigElementPtr.h>

#ifdef VPR_OS_Win32
#  include <gadget/Devices/KeyboardMouseDevice/InputAreaWin32.h>
#else
#  include <gadget/Devices/KeyboardMouseDevice/InputAreaXWin.h>
#endif

#include <vrj/Display/Display.h>
#include <Performer/pf.h>

#include <string>


#ifdef VPR_OS_Win32
typedef struct _pfuWin32Event
{
  HWND hwnd;
  UINT uMsg;
  WPARAM wParam; // we should copy this and lParam
  LPARAM lParam;
  double time;
} pfuWin32Event;
#endif

namespace vrj
{

#ifdef VPR_OS_Win32
LRESULT CALLBACK eventCallback(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam);
#endif

/** \class PfInputHandler PfInputHandler.h vrj/Draw/Pf/PfInputHandler.h
 *
 * Performer input handler.
 */
class PfInputHandler
#ifdef VPR_OS_Win32
   : public gadget::InputAreaWin32
#else
   : public gadget::InputAreaXWin
#endif
{
public:
   /**
    * Creates an adaptor which will route events from Performer into
    * platform-specific events.
    *
    * @param pWin        Performer window to grab events from.
    * @param displayName The name of the display.
    */
   PfInputHandler(pfPipeWindow* pipeWindow, const std::string& displayName);

   /**
    * Configures the adaptor.
    *
    * @param e    ConfigElement that contains all configuration
    *             information to configure an InputArea.
    * @param disp VR Juggler display which contains size info.
    */
   void config(jccl::ConfigElementPtr e, vrj::Display* disp)
   {
      gadget::InputArea::config(e);
      int originX, originY, width, height;
      disp->getOriginAndSize(originX, originY, width, height);
      mWidth = (unsigned int)width;
      mHeight = (unsigned int)height;
   }

#ifndef VPR_OS_Win32
   /**
    * Grab events from Performer window and sends them to
    * InputAreaXWin::handleEvent() or InputAreaWin32::handleEvent()
    *
    * @pre openConnection has been called.
    */
   void handleEvents();

private:
   void openConnection();
#endif
   void checkEvents();
   
private:
   std::string     mName;
   pfPipe*         mPipe;
   pfPipeWindow*   mPipeWindow;
};

}
#endif
