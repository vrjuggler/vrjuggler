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
#include <Input/InputManager/vjProxy.h>


//: vjGlove proxy class.
//!PUBLIC_API:
class VJ_CLASS_API vjGloveProxy : public vjTypedProxy<vjGlove>
{
public:
     //: Construct the proxy to point to the given glove device and sub-unit number.
  vjGloveProxy()
  {
     mUnitNum = -1;
     mVisible = true;
  }

  virtual ~vjGloveProxy()
  {}

  float getAngle(vjGloveData::vjGloveComponent component,
                 vjGloveData::vjGloveJoint joint)
  {
    if(mStupified)
       return 0.0f;
    else
       return mTypedDevice->getGloveAngle(component, joint, mUnitNum);
  }


  vjVec3 getVector(vjGloveData::vjGloveComponent component)
  {
     if(mStupified)
        return vjVec3(0,0,0);
     else
        return mTypedDevice->getGloveVector(component, mUnitNum);
  }

  vjMatrix getPos( vjGloveData::vjGloveComponent component = vjGloveData::WRIST)
  {
     if(mStupified)
        return vjMatrix();
     else
      return mTypedDevice->getGlovePos(component, mUnitNum);
  }


  vjGloveData getData()
  {
     if(mStupified)
        return vjGloveData();
     else
        return mTypedDevice->getGloveData(mUnitNum);
  }


  //: Returns a pointer to the device held by this proxy.
  vjGlove* getGlovePtr()
  {
     if(mStupified)
        return NULL;
     else
      return mTypedDevice;
  }


  //: Returns the subUnit number that this proxy points to.
  int getUnit()
  { return mUnitNum; }

   bool isVisible()
   { return mVisible; }

   static std::string getChunkType() { return "GloveProxy"; }

   bool config(vjConfigChunk* chunk);

   virtual vjInput* getProxiedInputDevice()
   {
      if(NULL == mTypedDevice)
         return NULL;

      vjInput* ret_val = dynamic_cast<vjInput*>(mTypedDevice);
      vjASSERT(ret_val != NULL);
      return ret_val;
   }


private:
   //: Should we be drawn on the screen
   bool  mVisible;

   //: The subUnit number to use in the device.
   int mUnitNum;
};

#endif
