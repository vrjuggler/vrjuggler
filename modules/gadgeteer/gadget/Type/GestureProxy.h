/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998, 1999, 2000 by Iowa State University
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

/////////////////////////////////////////////////////////////////////////
// GestureProxy.h
//
// Gesture proxy class
//
////////////////////////////////////////////////////////////////////////

#ifndef _VRJ_GESTURE_PROXY_H_
#define _VRJ_GESTURE_PROXY_H_

#include <gad/gadConfig.h>
#include <math.h>

#include <gad/Type/Gesture.h>
#include <gad/Type/Proxy.h>

namespace vrj
{

//: Proxy to Gesture object
//
// A proxy is used by the user to actually acces the gesture data.
// The proxy allows the user to query the current gesture information.
// Clients call the get* routines to get the current gesture (id or string)
// Once the client has the current gesture, they should test it
// against the gestures they want to respond to.
//
// See also: Gesture
//!PUBLIC_API:
class GAD_CLASS_API GestureProxy : public TypedProxy<Gesture>
{
public:
   //: Construct the proxy to point to the given gesture device.
   GestureProxy()
   { ;}

   virtual ~GestureProxy()
   {}

   //: Get the current gesture.
   //! RETURNS: id of current gesture
   int getGesture()
   {
      const int defaultGesture(-1);

      if(mStupified)
         return defaultGesture;
      else
         return mTypedDevice->getGesture();
   }

   //: Return the identifier of the string gesture.
   //! ARGS: name - string name of a gesture
   //! RETURNS: -1 if not found
   int getGestureIndex(std::string name)
   {
      const int defaultGestureIndex(-1);
      if(mStupified)
         return defaultGestureIndex;
      else
         return mTypedDevice->getGestureIndex(name);
   }

   //: Get a gesture name
   //! RETURNS: Name of gesture with the given id (gestureId)
   //! NOTE: if gestureId = -1, returns name of current gesture
   std::string getGestureString(int gestureId = -1)
   {
      if(mStupified)
         return std::string("");
      else
         return mTypedDevice->getGestureString(gestureId);
   }

   //: Returns a pointer to the device held by this proxy.
   Gesture* getGesturePtr()
   {
      if(mStupified)
         return NULL;
      else
         return mTypedDevice;
   }

   static std::string getChunkType() { return "GestureProxy"; }

   bool config(ConfigChunk* chunk);

   virtual Input* getProxiedInputDevice()
   {
      if(NULL == mTypedDevice)
         return NULL;

      Input* ret_val = dynamic_cast<Input*>(mTypedDevice);
      vprASSERT(ret_val != NULL);
      return ret_val;
   }
};
};
#endif
