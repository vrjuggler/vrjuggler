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
#include <assert.h>

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

   ~vjGestureProxy()
   {}

   //: Set the gloveProxy to point to another device and subUnit number.
   //! PRE: gesturePtr must point to a valid gesture device
   //! POST: this now proxies the given gesture device
   void Set(vjGesture* gesturePtr)
   {
      vjASSERT( gesturePtr->FDeviceSupport(DEVICE_GESTURE) );
      mGesturePtr = gesturePtr;
   }

   //: Get the current gesture.
   //! RETURNS: id of current gesture
   int getGesture()
   { return mGesturePtr->getGesture(); }

   //: Return the identifier of the string gesture.
   //! ARGS: name - string name of a gesture
   //! RETURNS: -1 if not found
   int getGestureIndex(string name)
   { return mGesturePtr->getGestureIndex(name); }

   //: Get a gesture name
   //! RETURNS: Name of gesture with the given id (gestureId)
   //! NOTE: if gestureId = -1, returns name of current gesture
   string getGestureString(int gestureId = -1)
   { return mGesturePtr->getGestureString(gestureId); }

   //: Returns a pointer to the device held by this proxy.
   vjGesture* getGesturePtr()
   { return mGesturePtr; }

   static string getChunkType() { return "GestureProxy"; }

   bool config(vjConfigChunk* chunk);

private:
   //: Pointer to the gesture device we are proxying.
   vjGesture* mGesturePtr;
};

#endif
