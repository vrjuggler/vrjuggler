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
#ifndef _GADGET_POSITION_XFORM_FILTER_H_
#define _GADGET_POSITION_XFORM_FILTER_H_

#include <gadget/gadgetConfig.h>
#include <typeinfo>
#include <vector>

#include <jccl/Config/ConfigChunkPtr.h>
#include <gadget/Filter/Position/PositionFilter.h>
#include <gadget/Type/PositionData.h>

#include <gmtl/Matrix.h>
#include <vpr/Util/Debug.h>


namespace gadget
{
/** PositionFilter is the abstract base class that all position filters derive from
*
* compute Sensor = preTrans*preRot*(Scale*Sensor)*postTrans*postRot
*/
class GADGET_CLASS_API PositionXformFilter : public PositionFilter
{
public:
   /** Constructor */
   PositionXformFilter()
      : mScaleValue(0.0f)
   {;}

   /** Destructor */
   virtual ~PositionXformFilter()
   {;}

   /** Configuration for the filter
   * @return Returns truee if configured correctly
   */
   virtual bool config(jccl::ConfigChunkPtr c);

   /** Apply the pos filter.
   * Apply the filter to the posSample in place.
   * @param posSample - The sample to modify in place
   * @post posSample is updated with a modified version.
   */
   virtual void apply(std::vector< PositionData >& posSample);

   static std::string getChunkType() { return std::string("PosXformFilter"); }

protected:
   /** Given Sensor pos xform in sensor coordinate system: Sensor
   * world - world coordindate system that the transformed sensor "ends up in"
   * Sensor' - Modified sensor coordinate system
   *
   * preXform is the transform from world to sensor (world_M_sensor)
   * postXform is the transform from sensor to sensor' (sensor_M_newsensor)
   *
   * compute Sensor = preTrans*preRot*(Scale*Sensor)*postTrans*postRot
   */
   gmtl::Matrix44f mPreXform;    /**< pre transformation */
   gmtl::Matrix44f mPostXform;   /**< post transformation */
   float           mScaleValue;     /**< The value to use for scaling */
};


} // End of gadget namespace

#endif

