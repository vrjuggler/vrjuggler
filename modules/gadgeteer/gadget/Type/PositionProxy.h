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
// positional proxy class
//
////////////////////////////////////////////////////////////////////////

#ifndef _GADGET_POSPROXY_H_
#define _GADGET_POSPROXY_H_

#include <gadget/gadgetConfig.h>
#include <math.h>

#include <gadget/Type/Position.h>
#include <gadget/Type/Proxy.h>
#include <gadget/Filter/Position/PositionFilter.h>
#include <gadget/Type/Position/PositionUnitConversion.h>
#include <gadget/Type/PositionData.h>

#include <gmtl/Matrix.h>
#include <gmtl/MatrixOps.h>
#include <gmtl/Output.h>

namespace gadget
{

/**
 * A proxy class to positional devices, used by the InputManager.
 * A PositionProxy always points to a positional device and subUnit number,
 * the inputgroup can therefore keep an array of these around and
 * treat them as positional devices which only return a single
 * subDevice's amount of data.  (one POS_DATA)
 */
class GADGET_CLASS_API PositionProxy : public TypedProxy<Position>
{
public:
   PositionProxy() :  mUnitNum(-1)
   {;}

   virtual ~PositionProxy() {}

   /**
    * Updates the proxy's copy of the data.
    * Copies the device data to local storage, and transform it if necessary.
    */
   virtual void updateData();

   /// Returns time of last update.
   vpr::Interval getTimeStamp()
   {
      return mPositionData.getTime();
   }

   /**
    * Gets the positional data within the device pointed to by this proxy as a
    * matrix.
    *
    * @param scaleFactor - factor to convert from meters to the desired units
    * @return  the position of the device as a matrix
    * @example getData(3.28f) will return a matrix in feet
    */
   gmtl::Matrix44f getData(float scaleFactor = gadget::PositionUnitConversion::ConvertToFeet );
   
   /** Gets the actual PositionData. */
    PositionData* getPositionData()
    {
        return &mPositionData;
    }

   /// Returns this device's subunit number.
   int getUnit()
   { return mUnitNum; }

   /// Return the Position pointer held by this proxy.
   Position* getPositionPtr()
   {
      if(!mStupified)
         return mTypedDevice;
      else
         return NULL;
   }

   static std::string getChunkType() { return "PosProxy"; }

   bool config(jccl::ConfigChunkPtr chunk);

   virtual Input* getProxiedInputDevice()
   {
      if ((NULL == mTypedDevice) || (mStupified))
      {
         return NULL;
      }

      Input* ret_val = dynamic_cast<Input*>(mTypedDevice);
      vprASSERT(ret_val != NULL);
      return ret_val;
   }

private:
   PositionData      mPositionData;
   int               mUnitNum;

   gmtl::Matrix44f   mPosMatrix_feet;                 /**< Cached version of data in feet */

   std::vector<PositionFilter*>  mPositionFilters;    /**< The active filters that are to be used */
};

} // End of gadget namespace

#endif
