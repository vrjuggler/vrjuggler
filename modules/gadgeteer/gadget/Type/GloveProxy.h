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

/////////////////////////////////////////////////////////////////////////
//
// Glove proxy class
//
////////////////////////////////////////////////////////////////////////

#ifndef _GADGET_GLOVE_PROXY_H_
#define _GADGET_GLOVE_PROXY_H_

#include <gadget/gadgetConfig.h>
#include <math.h>

#include <gadget/Type/Glove.h>
#include <gadget/Type/Proxy.h>

namespace gadget
{

/**
 * Glove proxy class.
 */
class GADGET_CLASS_API GloveProxy : public TypedProxy<Glove>
{
public:
  /**
   * Constructs the proxy to point to the given glove device and sub-unit
   * number.
   */
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
    if(isStupified())
       return 0.0f;
    else
       return mTypedDevice->getGloveAngle(component, joint, mUnitNum);
  }


  gmtl::Vec3f getVector(GloveData::GloveComponent component)
  {
     if(isStupified())
        return gmtl::Vec3f(0,0,0);
     else
        return mTypedDevice->getGloveVector(component, mUnitNum);
  }

  gmtl::Matrix44f getPos( GloveData::GloveComponent component = GloveData::WRIST)
  {
     if(isStupified())
        return gmtl::Matrix44f();
     else
      return mTypedDevice->getGlovePos(component, mUnitNum);
  }


  GloveData getData()
  {
     if(isStupified())
        return GloveData();
     else
        return mTypedDevice->getGloveData(mUnitNum);
  }


  /** Returns a pointer to the device held by this proxy. */
  Glove* getGlovePtr()
  {
     if(isStupified())
        return NULL;
     else
      return mTypedDevice;
  }


  /** Returns the subUnit number that this proxy points to. */
  int getUnit()
  { return mUnitNum; }

   bool isVisible()
   { return mVisible; }

   static std::string getChunkType() { return "GloveProxy"; }

   bool config(jccl::ConfigChunkPtr chunk);

   virtual Input* getProxiedInputDevice()
   {
      if((NULL == mTypedDevice) || (isStupified()))
         return NULL;

      Input* ret_val = dynamic_cast<Input*>(mTypedDevice);
      vprASSERT((ret_val != NULL) && "Cross-cast in GloveProxy failed");
      return ret_val;
   }


private:
   /** Should we be drawn on the screen? */
   bool  mVisible;

   /** The sub-unit number to use in the device. */
   int mUnitNum;
};

} // End of gadget namespace

#endif
