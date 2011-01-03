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

#ifndef _GADGET_FULL_POSITION_XFORM_FILTER_H_
#define _GADGET_FULL_POSITION_XFORM_FILTER_H_

#include <gadget/gadgetConfig.h>

#include <vector>

#include <gmtl/Matrix.h>

#include <jccl/Config/ConfigElementPtr.h>
#include <gadget/Filter/Position/PositionFilter.h>
#include <gadget/Type/PositionData.h>


namespace gadget
{

/** \class FullPositionXformFilter FullPositionXformFilter.h gadget/Filter/Position/FullPositionXformFilter.h
 *
 * Fully configurable position transformation filter. Users define the entire
 * pre- and post-transformation matrices for the filter in a config element.
 *
 * @since 1.3.20
 */
class GADGET_CLASS_API FullPositionXformFilter : public PositionFilter
{
public:
   /** Constructor. */
   FullPositionXformFilter();

   /** Destructor. */
   virtual ~FullPositionXformFilter();

   /**
    * Configures this position filter.
    *
    * @return \c true is returned if configured correctly.
    */
   virtual bool config(jccl::ConfigElementPtr e);

   /**
    * Applies the position filter to the given samples.
    *
    * @post Each gadget::PositionData object in \p posSamples is updated with
    *       a transformed version.
    *
    * @param posSamples The collection of samples to modify (in place).
    */
   virtual void apply(std::vector<PositionData>& posSamples);

   static std::string getElementType();

   /** @name Transformation Matrix Accessors */
   //@{
   const gmtl::Matrix44f& getPreXform() const
   {
      return mPreXform;
   }

   void setPreXform(const gmtl::Matrix44f& xform)
   {
      mPreXform = xform;
   }

   const gmtl::Matrix44f& getPostXform() const
   {
      return mPostXform;
   }

   void setPostXform(const gmtl::Matrix44f& xform)
   {
      mPostXform = xform;
   }
   //@}

private:
   /**
    * Given Sensor position transform in sensor coordinate system: Sensor
    * world - world coordindate system that the transformed sensor "ends up in"
    * Sensor' - Modified sensor coordinate system.
    *
    * preXform is the transform from world to sensor (world_M_sensor).
    * postXform is the transform from sensor to sensor' (sensor_M_newsensor).
    *
    * compute Sensor = preTrans*preRot*(Scale*Sensor)*postTrans*postRot
    */
   gmtl::Matrix44f mPreXform;    /**< Pre-transformation */
   gmtl::Matrix44f mPostXform;   /**< Post-transformation */
};

} // End of gadget namespace


#endif /* _GADGET_FULL_POSITION_XFORM_FILTER_H_ */
