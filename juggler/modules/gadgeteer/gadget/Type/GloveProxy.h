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
// Glove proxy class
//
////////////////////////////////////////////////////////////////////////

#ifndef _VRJ_GLOVE_PROXY_H_
#define _VRJ_GLOVE_PROXY_H_

#include <vrj/vjConfig.h>
#include <math.h>

#include <vrj/Input/Type/Glove.h>
#include <vrj/Input/Type/Proxy.h>

namespace vrj
{
   
//: Glove proxy class.
//!PUBLIC_API:
class VJ_CLASS_API GloveProxy : public TypedProxy<Glove>
{
public:
     //: Construct the proxy to point to the given glove device and sub-unit number.
  GloveProxy()
  {
     mUnitNum = -1;
     mVisible = true;
  }

  virtual ~GloveProxy()
  {}

  float getAngle(GloveData::GloveComponent component,
                 GloveData::GloveJoint joint)
  {
    if(mStupified)
       return 0.0f;
    else
       return mTypedDevice->getGloveAngle(component, joint, mUnitNum);
  }


  Vec3 getVector(GloveData::GloveComponent component)
  {
     if(mStupified)
        return Vec3(0,0,0);
     else
        return mTypedDevice->getGloveVector(component, mUnitNum);
  }

  Matrix getPos( GloveData::GloveComponent component = GloveData::WRIST)
  {
     if(mStupified)
        return Matrix();
     else
      return mTypedDevice->getGlovePos(component, mUnitNum);
  }


  GloveData getData()
  {
     if(mStupified)
        return GloveData();
     else
        return mTypedDevice->getGloveData(mUnitNum);
  }


  //: Returns a pointer to the device held by this proxy.
  Glove* getGlovePtr()
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

   bool config(ConfigChunk* chunk);

   virtual Input* getProxiedInputDevice()
   {
      if(NULL == mTypedDevice)
         return NULL;

      Input* ret_val = dynamic_cast<Input*>(mTypedDevice);
      vprASSERT(ret_val != NULL);
      return ret_val;
   }


private:
   //: Should we be drawn on the screen
   bool  mVisible;

   //: The subUnit number to use in the device.
   int mUnitNum;
};

};

#endif
