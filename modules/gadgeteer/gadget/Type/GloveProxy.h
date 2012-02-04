/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2011 by Iowa State University
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
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
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
#include <gadget/Type/GloveProxyPtr.h>

namespace gadget
{

/** \class GloveProxy GloveProxy.h gadget/Type/GloveProxy.h
 *
 * Glove proxy class.
 */
class GADGET_API GloveProxy
   : public TypedProxy<Glove>
{
public:
   /** @since 2.1.1 */
   typedef GloveProxy::typed_proxy_ base_type;

protected:
   /**
    * Constructs the proxy to point to the given glove device and sub-unit
    * number.
    */
   GloveProxy(const std::string& deviceName = "UnknownGlove",
              const int unitNum = -1);

public:
   /**
    * Creates a GloveProxy instance and returns it wrapped in a
    * GloveProxyPtr object.
    *
    * @since 1.3.7
    */
   static GloveProxyPtr create(const std::string& deviceName = "UnknownGlove",
                               const int unitNum = -1);

   virtual ~GloveProxy();

   const gmtl::Vec3f getTipVector(const GloveData::GloveComponent component) 
      const
   {
      return isStupefied() ? gmtl::Vec3f(0.0f, 0.0f, 0.0f)
                           : mTypedDevice->getTipVector(component, mUnit);
   }

   const gmtl::Matrix44f
   getTipTransform(const GloveData::GloveComponent component) const
   {
      return isStupefied() ? gmtl::Matrix44f()
                           : mTypedDevice->getTipTransform(component, mUnit);
   }
  
   const gmtl::Matrix44f
   getJointTransform(const GloveData::GloveComponent component,
                     const GloveData::GloveJoint joint)
      const
   {
      return isStupefied() ? gmtl::Matrix44f()
                           : mTypedDevice->getJointTransform(component, joint,
                                                             mUnit);
   }

   virtual const GloveValues getData() const
   {
      return isStupefied() ? mStupefiedData
                           : mTypedDevice->getGloveData(mUnit).getValue();
   }

   /** Returns time of last update. */
   const vpr::Interval& getTimeStamp() const;

   bool isVisible() const
   {
      return mVisible;
   }

   static std::string getElementType();

private:
   /** Should we be drawn on the screen? */
   bool mVisible;
};

} // End of gadget namespace

#endif
