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
 * A proxy class to event window devices, used by the Input Manager.
 *
 * An event window proxy always points to an event window device and unit
 * number within that device.  The Input Manager can therefore keep an array
 * of these around and treat them as digital devices that only return a single
 * sub-device's amount of data (an event queue and individual keys).
 *
 * @see gagdet::EventWindow
 */
class GADGET_CLASS_API EventWindowProxy : public TypedProxy<EventWindow>
{
public:
   EventWindowProxy()
   { ; }

   /**
    * Returns a pointer to the gadget::EventWindow object held by this proxy.
    */
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

   /** Returns the time of the last update. */
   virtual vpr::Interval getTimeStamp() const
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
    * Determines if the given modifier key is the only modifier pressed.
    *
    * @pre The given key identifier is one of gadget::NONE, gadget::KEY_ALT,
    *      gadget::KEY_CTRL, or gadget::KEY_SHIFT.
    *
    * @param modKey A gadget::Keys value that must be one of gadget::NONE,
    *               gadget::KEY_ALT, gadget::KEY_CTRL, or gadget::KEY_SHIFT.
    *
    * @return true if the given modifier key is the only modifier key pressed.
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

   /**
    * Returns the number of times the given key was pressed during the last
    * frame.  The value returned can be used in a conditional expression to
    * determine if the key was pressed at all.
    *
    * @param keyId The identifier for the key whose state will be queried.
    *
    * @return The number of times the key was pressed since last update.
    */
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

   /**
    * Returns a copy of the current queue of events for the proxied window.
    */
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

   static std::string getElementType();

   bool config(jccl::ConfigElementPtr element);

   virtual Input* getProxiedInputDevice()
   {
      if ( NULL == mTypedDevice || isStupified() )
      {
         return NULL;
      }

      Input* ret_val = dynamic_cast<Input*>(mTypedDevice);
      vprASSERT((ret_val != NULL) && "Cross-cast in EventWindowProxy failed");
      return ret_val;
   }
};


} // End of gadget namespace

#endif
