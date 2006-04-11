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

#ifndef _GADGET_EVENT_WINDOW_PROXY_H_
#define _GADGET_EVENT_WINDOW_PROXY_H_

#include <gadget/gadgetConfig.h>
#include <gadget/Type/Proxy.h>
#include <gadget/Type/EventWindow.h>
#include <gadget/Type/EventWindow/Event.h>
#include <gadget/Type/EventWindow/EventPtr.h>

namespace gadget
{

/**
 * Proxy class to event window-based devices.
 */
class GADGET_CLASS_API EventWindowProxy : public TypedProxy<EventWindow>
{
public:
   EventWindowProxy()
   { ; }

   EventWindow* getEventWindowPtr()
   {
      if(isStupified())
      {
         return NULL;
      }
      else
      {
         return mTypedDevice;
      }
   }

   vpr::Interval getTimeStamp()
   {
      if ( isStupified() || (mTypedDevice == NULL) )
      {
         return vpr::Interval();
      }
      else
      {
         return mTypedDevice->getSyncTime();
      }  
   }

   /**
    * Determines if the modifier key is pressed exclusively.
    * @pre modKey is a valid modifier identifier
    * @return true if modKey is the only modifier pressed
    */
   bool modifierOnly(gadget::Keys modKey)
   {
      if ( isStupified() || (mTypedDevice == NULL) )
      {
         return false;
      }
      else
      {
         return mTypedDevice->modifierOnly(modKey);
      }
   }

   int keyPressed(gadget::Keys keyId)
   {
      if ( isStupified() || (mTypedDevice == NULL) )
      {
         return 0;
      }
      else
      {
         return mTypedDevice->keyPressed(keyId);
      }
   }

   EventWindow::EventQueue getEventQueue()
   {
      if ( isStupified() || (mTypedDevice == NULL) )
      {
         return EventWindow::EventQueue();
      }
      else
      {
         return mTypedDevice->getEventQueue();
      }
   }

   static std::string getChunkType()
   {
      return std::string("EventWindowProxy");
   }

   bool config(jccl::ConfigChunkPtr chunk);

   virtual Input* getProxiedInputDevice()
   {
      if ((NULL == mTypedDevice) || (isStupified()))
         return NULL;

      Input* ret_val = dynamic_cast<Input*>(mTypedDevice);
      vprASSERT((ret_val != NULL) && "Cross-cast in EventWindowProxy failed");
      return ret_val;
   }

};


} // End of gadget namespace

#endif
