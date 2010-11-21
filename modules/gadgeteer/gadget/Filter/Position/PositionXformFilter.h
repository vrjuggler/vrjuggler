/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2010 by Iowa State University
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

#ifndef _GADGET_POSITION_XFORM_FILTER_H_
#define _GADGET_POSITION_XFORM_FILTER_H_

#include <gadget/gadgetConfig.h>
#include <typeinfo>
#include <vector>

#include <jccl/Config/ConfigElementPtr.h>
#include <gadget/Filter/Position/PositionFilter.h>
#include <gadget/Type/PositionData.h>

#include <gmtl/Matrix.h>
#include <vpr/Util/Debug.h>


namespace gadget
{

/** \class PositionXformFilter PositionXformFilter.h gadget/Filter/Position/PositionXformFilter.h
 *
 * Position transformation filter.
 */
class GADGET_CLASS_API PositionXformFilter : public PositionFilter
{
public:
   /** Constructor. */
   PositionXformFilter()
      : mScaleValue(0.0f)
   {;}

   /** Destructor. */
   virtual ~PositionXformFilter()
   {;}

   /**
    * Configuration for the filter.
    * @return Returns truee if configured correctly.
    */
   virtual bool config(jccl::ConfigElementPtr e);

   /**
    * Applies the position filter to the given sample in place.
    * @post posSample is updated with a modified version.
    * @param posSample The sample to modify in place.
    */
   virtual void apply(std::vector< PositionData >& posSample);

   static std::string getElementType();

protected:
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
   gmtl::Matrix44f mPreXform;    /**< pre transformation */
   gmtl::Matrix44f mPostXform;   /**< post transformation */
   float           mScaleValue;     /**< The value to use for scaling */
};


} // End of gadget namespace

#endif
