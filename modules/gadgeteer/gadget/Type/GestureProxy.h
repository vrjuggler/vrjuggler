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

//: Proxy to vjGesture object
//
// See also: vjGesture
class vjGestureProxy
{
public:
     //: Construct the proxy to point to the given gesture device.
  vjGestureProxy(vjGesture* gesturePtr)
  { Set(gesturePtr); }

  ~vjGestureProxy()
  {}

  //: Set the gloveProxy to point to another device and subUnit number.
  void Set(vjGesture* gesturePtr)
  {
      vjASSERT( gesturePtr->FDeviceSupport(DEVICE_GESTURE) );
      mGlovePtr = glovePtr;
      mUnitNum = unitNum;
   }

   //: Get the current gesture.
   //! RETURNS: id of current gesture
   int getGesture()
   { return mGesturePtr->getGesture(); }

   //: Return the identifier of the string gesture.
   //! RETURNS: -1 if not found
   int getGestureIndex(string name)
   { return mGesturePtr->getGestureIndex(name); }

   //: Get a gesture name
   //! RETURNS: Name of gesture with the given id (gestureId)
   // NOTE: if gestureId = -1, returns name of current gesture
   string getGestureString(int gestureId = -1)
   { return mGesturePtr->getGestureString(gestureId); }

   //: Returns a pointer to the device held by this proxy.
   vjGesture* getGesturePtr()
   { return mGesturePtr; }

private:
   //: Pointer to the gesture device we are proxying.
   vjGesture* mGesturePtr;
};

#endif
