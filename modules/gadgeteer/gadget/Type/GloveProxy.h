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

/** \class GloveProxy GloveProxy.h gadget/Type/GloveProxy.h
 *
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

   gmtl::Vec3f getTipVector(GloveData::GloveComponent component)
   {
      if(isStupefied())
         return gmtl::Vec3f(0,0,0);
      else
         return mTypedDevice->getTipVector(component,mUnitNum);
   }

   gmtl::Matrix44f getTipTransform( GloveData::GloveComponent component)
   {
      if(isStupefied())
         return gmtl::Matrix44f();
      else
         return mTypedDevice->getTipTransform(component,mUnitNum);
   }
  
   gmtl::Matrix44f getJointTransform(GloveData::GloveComponent component,
                                     GloveData::GloveJoint joint)
   {
      if(isStupefied())
         return gmtl::Matrix44f();
      else
         return mTypedDevice->getJointTransform(component, joint, mUnitNum);
   }

   GloveData getData()
   {
      if(isStupefied())
         return GloveData();
      else
         return mTypedDevice->getGloveData(mUnitNum);
   }

   /** Returns time of last update. */
   vpr::Interval getTimeStamp() const
   {
      // XXX: Broken for now, this is a case similar to the KeyboardMouse type in that
      //      it does not point to one data element like digital, analog, and position.
      return vpr::Interval();
   }

   /** Returns a pointer to the device held by this proxy. */
   Glove* getGlovePtr()
   {
      if(isStupefied())
         return NULL;
      else
         return mTypedDevice;
   }

   /** Returns the subUnit number that this proxy points to. */
   int getUnit()
   {
      return mUnitNum;
   }

   bool isVisible()
   {
      return mVisible;
   }

   static std::string getElementType();

   bool config(jccl::ConfigElementPtr element);

   virtual Input* getProxiedInputDevice()
   {
      if((NULL == mTypedDevice) || (isStupefied()))
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
