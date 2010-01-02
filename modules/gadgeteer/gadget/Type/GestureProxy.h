/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2010 by Iowa State University
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

#ifndef _GADGET_GESTURE_PROXY_H_
#define _GADGET_GESTURE_PROXY_H_

#include <gadget/gadgetConfig.h>
#include <math.h>

#include <gadget/Type/Gesture.h>
#include <gadget/Type/Proxy.h>
#include <gadget/Type/GestureProxyPtr.h>

namespace gadget
{

/** \class GestureProxy GestureProxy.h gadget/Type/GestureProxy.h
 *
 * Proxy to Gesture object.
 *
 * A proxy is used by the user to actually acces the gesture data.
 * The proxy allows the user to query the current gesture information.
 * Clients call the get* routines to get the current gesture (id or string)
 * Once the client has the current gesture, they should test it
 * against the gestures they want to respond to.
 *
 * @see gadget::Gesture
 */
class GADGET_CLASS_API GestureProxy : public TypedProxy<Gesture>
{
protected:
   /** Constructs the proxy to point to the given gesture device. */
   GestureProxy(const std::string& deviceName = "UnknownGesture");

public:
   /**
    * Creates a GestureProxy instance and returns it wrapped in a
    * GestureProxyPtr object.
    *
    * @since 1.3.7
    */
   static GestureProxyPtr create(const std::string& deviceName = "UnknownGesture");

   virtual ~GestureProxy();

   /**
    * Gets the current gesture.
    * @return id of current gesture.
    */
   int getGesture() const
   {
      const int defaultGesture(-1);

      if(isStupefied())
      {
         return defaultGesture;
      }
      else
      {
         return mTypedDevice->getGesture();
      }
   }

   /**
    * Returns the identifier of the string gesture.
    * @param name The string name of a gesture.
    * @return -1 if not found
    */
   int getGestureIndex(const std::string& name) const
   {
      const int defaultGestureIndex(-1);
      if(isStupefied())
      {
         return defaultGestureIndex;
      }
      else
      {
         return mTypedDevice->getGestureIndex(name);
      }
   }

   /**
    * Gets a gesture name.
    * @return Name of gesture with the given id (gestureId).
    * @note if gestureId = -1, returns name of current gesture.
    */
   const std::string getGestureString(const int gestureId = -1)
   {
      if(isStupefied())
      {
         return std::string("");
      }
      else
      {
         return mTypedDevice->getGestureString(gestureId);
      }
   }

   /** Returns time of last update. */
   vpr::Interval getTimeStamp() const;

   /** Returns a pointer to the device held by this proxy. */
   const GesturePtr getGesturePtr() const
   {
      if(isStupefied())
      {
         return GesturePtr();
      }
      else
      {
         return mTypedDevice;
      }
   }

   static std::string getElementType();

   bool config(jccl::ConfigElementPtr element);
};

} // End of gadget namespace

#endif
