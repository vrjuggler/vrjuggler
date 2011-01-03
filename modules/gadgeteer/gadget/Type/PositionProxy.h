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
// positional proxy class
//
////////////////////////////////////////////////////////////////////////

#ifndef _GADGET_POSPROXY_H_
#define _GADGET_POSPROXY_H_

#include <gadget/gadgetConfig.h>

#include <cmath>
#include <vector>
#include <boost/shared_ptr.hpp>

#include <gmtl/Matrix.h>
#include <gmtl/MatrixOps.h>
#include <gmtl/Output.h>

#include <gadget/Type/Proxy.h>
#include <gadget/Type/Position.h>
#include <gadget/Filter/Position/PositionFilter.h>
#include <gadget/Type/Position/PositionUnitConversion.h>
#include <gadget/Type/PositionData.h>
#include <gadget/Type/PositionProxyPtr.h>


namespace gadget
{

/** \class PositionProxy PositionProxy.h gadget/Type/PositionProxy.h
 *
 * A proxy class to positional devices, used by the Input Manager.
 *
 * A position proxy always points to a positional device and unit number within
 * that device.  The Input Manager can therefore keep an array of these
 * around and treat them as positional devices that only return a single
 * sub-device's amount of data (one gmtl::Matrix44f).
 *
 * @see gagdet::Position
 */
class GADGET_CLASS_API PositionProxy : public TypedProxy<Position>
{
public:
   /** @since 2.1.1 */
   typedef TypedProxy<Position> base_type;

protected:
   PositionProxy(const std::string& deviceName = "UnknownPosition",
                 const int unitNum = -1);

public:
   /**
    * Creates a PositionProxy instance and returns it wrapped in a
    * PositionProxyPtr object.
    *
    * @since 1.3.7
    */
   static PositionProxyPtr create(const std::string& deviceName = "UnknownPosition",
                                  const int unitNum = -1);

   virtual ~PositionProxy();

   /**
    * Updates the proxy's copy of the data.
    * Copies the device data to local storage and transforms it if necessary.
    */
   virtual void updateData();

   /**
    * Applies the position filters configured for use by this object to the
    * given data.
    *
    * @return The transformed position data is returned.
    *
    * @since 2.1.4
    */
   const PositionData applyFilters(const PositionData& posData) const;

   /**
    * Gets the positional data within the device pointed to by this proxy as a
    * matrix using feet as the units.
    *
    * @return The position of the device as a matrix.
    */
   virtual const gmtl::Matrix44f getData() const
   {
      return getData(PositionUnitConversion::ConvertToFeet);
   }

   /**
    * Gets the positional data within the device pointed to by this proxy as a
    * matrix. For example, getData(3.28f) will return a matrix in feet.
    *
    * @param scaleFactor Factor to convert from meters to the desired units.
    *
    * @return The position of the device as a matrix.
    */
   const gmtl::Matrix44f getData(const float scaleFactor) const;

   /**
    * Applies the given scale factor to the given transformation matrix.
    *
    * @param xform       The source transformation matrix.
    * @param scaleFactor The scale factor to apply.
    * @param result      Storage for the transformed matrix.
    *
    * @since 2.1.4
    */
   void applyScaleFactor(const gmtl::Matrix44f& xform, const float scaleFactor,
                         gmtl::Matrix44f& result) const;

   static std::string getElementType();

   bool config(jccl::ConfigElementPtr element);

private:
   gmtl::Matrix44f   mPosMatrix_feet;                 /**< Cached version of data in feet */

   typedef boost::shared_ptr<PositionFilter> PositionFilterPtr;
   std::vector<PositionFilterPtr> mPositionFilters;   /**< The active filters that are to be used */
};

} // End of gadget namespace

#endif
