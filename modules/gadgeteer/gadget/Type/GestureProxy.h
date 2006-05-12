/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2006 by Iowa State University
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

/////////////////////////////////////////////////////////////////////////
// GestureProxy.h
//
// Gesture proxy class
//
////////////////////////////////////////////////////////////////////////

#ifndef _GADGET_GESTURE_PROXY_H_
#define _GADGET_GESTURE_PROXY_H_

#include <gadget/gadgetConfig.h>
#include <math.h>

#include <gadget/Type/Gesture.h>
#include <gadget/Type/Proxy.h>

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
public:
   /** Constructs the proxy to point to the given gesture device. */
   GestureProxy()
   { ;}

   virtual ~GestureProxy()
   {}

   /**
    * Gets the current gesture.
    * @return id of current gesture.
    */
   int getGesture()
   {
      const int defaultGesture(-1);

      if(isStupefied())
         return defaultGesture;
      else
         return mTypedDevice->getGesture();
   }

   /**
    * Returns the identifier of the string gesture.
    * @param name The string name of a gesture.
    * @return -1 if not found
    */
   int getGestureIndex(std::string name)
   {
      const int defaultGestureIndex(-1);
      if(isStupefied())
         return defaultGestureIndex;
      else
         return mTypedDevice->getGestureIndex(name);
   }

   /**
    * Gets a gesture name.
    * @return Name of gesture with the given id (gestureId).
    * @note if gestureId = -1, returns name of current gesture.
    */
   std::string getGestureString(int gestureId = -1)
   {
      if(isStupefied())
         return std::string("");
      else
         return mTypedDevice->getGestureString(gestureId);
   }

   /** Returns time of last update. */
   vpr::Interval getTimeStamp() const
   {
      // XXX: Broken for now, this is a case similar to the KeyboardMouse type in that
      //      it does not point to one data element like digital, analog, and position.
      return vpr::Interval();
   }

   /** Returns a pointer to the device held by this proxy. */
   Gesture* getGesturePtr()
   {
      if(isStupefied())
         return NULL;
      else
         return mTypedDevice;
   }

   static std::string getElementType();

   bool config(jccl::ConfigElementPtr element);

   virtual Input* getProxiedInputDevice()
   {
      if((NULL == mTypedDevice) || (isStupefied)())
         return NULL;

      Input* ret_val = dynamic_cast<Input*>(mTypedDevice);
      vprASSERT((ret_val != NULL) && "Cross-cast in GestureProxy failed");
      return ret_val;
   }
};

} // End of gadget namespace

#endif
