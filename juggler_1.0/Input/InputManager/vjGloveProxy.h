/////////////////////////////////////////////////////////////////////////
// vjGloveProxy.h
//
// vjGlove proxy class
//
////////////////////////////////////////////////////////////////////////
#ifndef _VJ_GLOVE_PROXY_H_
#define _VJ_GLOVE_PROXY_H_

#include <vjConfig.h>
#include <math.h>
#include <assert.h>

#include <Input/vjGlove/vjGlove.h>

class vjGloveProxy
{
public:
     //: Construct the proxy to point to the given glove device and sub-unit number.
  vjGloveProxy(vjGlove* glovePtr, int unitNum)
  {
     Set(glovePtr,unitNum);
     mVisible = true;
  }

  ~vjGloveProxy()
  {}

  //: Set the gloveProxy to point to another device and subUnit number.
  void Set(vjGlove* glovePtr, int unitNum)
  {
     vjASSERT( glovePtr->FDeviceSupport(DEVICE_GLOVE) );
     mGlovePtr = glovePtr;
     mUnitNum = unitNum;
  }

  float getAngle(vjGloveData::vjGloveComponent component,
                 vjGloveData::vjGloveJoint joint)
  {
    return mGlovePtr->getGloveAngle(component, joint, mUnitNum);
  }


  vjVec3 getVector(vjGloveData::vjGloveComponent component)
  {
     return mGlovePtr->getGloveVector(component, mUnitNum);
  }

  vjMatrix getPos( vjGloveData::vjGloveComponent component = vjGloveData::WRIST)
  {
     return mGlovePtr->getGlovePos(component, mUnitNum);
  }


  vjGloveData getData()
  {
     return mGlovePtr->getGloveData(mUnitNum);
  }


  //: Returns a pointer to the device held by this proxy.
  vjGlove* getGlovePtr()
  { return mGlovePtr; }


  //: Returns the subUnit number that this proxy points to.
  int getUnit()
  { return mUnitNum; }

   bool isVisible()
   { return mVisible; }

private:
   //: Pointer to the glove device we are proxying.
   vjGlove* mGlovePtr;

   //: Should we be drawn on the screen
   bool  mVisible;

   //: The subUnit number to use in the device.
   int mUnitNum;
};

#endif

