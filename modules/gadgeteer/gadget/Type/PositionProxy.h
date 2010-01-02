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
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
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
#include <vector>
#include <math.h>

#include <gadget/Type/Position.h>
#include <gadget/Type/Proxy.h>
#include <gadget/Filter/Position/PositionFilter.h>
#include <gadget/Type/Position/PositionUnitConversion.h>
#include <gadget/Type/PositionData.h>
#include <gadget/Type/PositionProxyPtr.h>

#include <gmtl/Matrix.h>
#include <gmtl/MatrixOps.h>
#include <gmtl/Output.h>

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

   /** Returns time of last update. */
   virtual vpr::Interval getTimeStamp() const;

   /**
    * Gets the positional data within the device pointed to by this proxy as a
    * matrix.  For example, getData(3.28f) will return a matrix in feet.
    *
    * @param scaleFactor Factor to convert from meters to the desired units.
    *                    This paramter is optional, and it defaults to the
    *                    conversion from meters to feet.
    *
    * @return The position of the device as a matrix.
    */
   const gmtl::Matrix44f
   getData(const float scaleFactor = PositionUnitConversion::ConvertToFeet)
      const;
   
   /** Gets the actual PositionData. */
   const PositionData* getPositionData() const
   {
      return &mPositionData;
   }

   /// Returns this device's subunit number.
   int getUnit() const
   {
      return mUnitNum;
   }

   /// Returns a pointer to the gadget::Position object held by this proxy.
   const PositionPtr getPositionPtr() const
   {
      if(mStupefied || NULL == mTypedDevice.get())
      {
         return PositionPtr();
      }
      else
      {
         return mTypedDevice;
      }
   }

   static std::string getElementType();

   bool config(jccl::ConfigElementPtr element);

private:
   PositionData      mPositionData;
   int               mUnitNum;

   gmtl::Matrix44f   mPosMatrix_feet;                 /**< Cached version of data in feet */

   std::vector<PositionFilter*>  mPositionFilters;    /**< The active filters that are to be used */
};

} // End of gadget namespace

#endif
