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
// vjGestureProxy.h
//
// vjGesture proxy class
//
////////////////////////////////////////////////////////////////////////
#ifndef _VJ_GESTURE_PROXY_H_
#define _VJ_GESTURE_PROXY_H_

#include <vjConfig.h>
#include <math.h>

#include <Input/vjGesture/vjGesture.h>
#include <Input/InputManager/vjProxy.h>


//: Proxy to vjGesture object
//
// A proxy is used by the user to actually acces the gesture data.
// The proxy allows the user to query the current gesture information.
// Clients call the get* routines to get the current gesture (id or string)
// Once the client has the current gesture, they should test it
// against the gestures they want to respond to.
//
// See also: vjGesture
//!PUBLIC_API:
class vjGestureProxy : public vjProxy
{
public:
   //: Construct the proxy to point to the given gesture device.
   vjGestureProxy()
   { mGesturePtr = NULL; }

   virtual ~vjGestureProxy()
   {}

   //: Set the gloveProxy to point to another device and subUnit number.
   //! PRE: gesturePtr must point to a valid gesture device
   //! POST: this now proxies the given gesture device
   void set(vjGesture* gesturePtr)
   {
      vjASSERT( gesturePtr->fDeviceSupport(DEVICE_GESTURE) );
      mGesturePtr = gesturePtr;

      vjDEBUG(vjDBG_INPUT_MGR, vjDBG_VERB_LVL) << "gesturePtr: " << gesturePtr << std::endl << vjDEBUG_FLUSH;
   }

   //: Get the current gesture.
   //! RETURNS: id of current gesture
   int getGesture()
   { return mGesturePtr->getGesture(); }

   //: Return the identifier of the string gesture.
   //! ARGS: name - string name of a gesture
   //! RETURNS: -1 if not found
   int getGestureIndex(std::string name)
   { return mGesturePtr->getGestureIndex(name); }

   //: Get a gesture name
   //! RETURNS: Name of gesture with the given id (gestureId)
   //! NOTE: if gestureId = -1, returns name of current gesture
   std::string getGestureString(int gestureId = -1)
   { return mGesturePtr->getGestureString(gestureId); }

   //: Returns a pointer to the device held by this proxy.
   vjGesture* getGesturePtr()
   { return mGesturePtr; }

   static std::string getChunkType() { return "GestureProxy"; }

   bool config(vjConfigChunk* chunk);

   virtual vjInput* getProxiedInputDevice()
   {
      vjInput* ret_val = dynamic_cast<vjInput*>(mGesturePtr);
      vjASSERT(ret_val != NULL);
      return ret_val;
   }

private:
   //: Pointer to the gesture device we are proxying.
   vjGesture* mGesturePtr;
};

#endif
