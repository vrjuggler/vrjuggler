/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2002 by Iowa State University
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
    * @return  the position of the device as a matrix
    */
   gmtl::Matrix44f* getData()
   {
      if(mStupified)
      {
         gmtl::identity(*mPositionData.getPosition());
      }

      return mPositionData.getPosition();
   }

    /** Gets the actual PositionData. */
    PositionData* getPosition()
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

   /// Gets the transform being using by this proxy.
   /*
   gmtl::Matrix44f& getTransform()
   { return mMatrixTransform; }
   */

   /**
    * Transforms the data in mPosData.
    *
    * @pre mPosData needs to have most recent data.
    * @post mPosData is transformed by the xform matrix.
    *       mPosData = old(mPosData).post(xformMatrix)
    *
    * @note This moves the wMr to the modifed reciever system wMmr
    *       where w = world, mr = world of the reciever, and r = reciever
    */
   void transformData()
   {
      //mPositionData.getPosition()->postMult(mMatrixTransform);
      //*(mPositionData.getPosition()) *= mMatrixTransform;   // post multiply
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

   std::vector<PositionFilter*>  mPositionFilters;    /**< The active filters that are to be used */
   
public:
   /** Sets the scale factor to be applied to all positional
   * data returned from position proxies in the system.
   *
   * Since gadgeteer uses Meters internally, this factor
   * is meant to convert from Meters to the units desired.
   */
   static void setScaleFactor(float scaleFactor)
   { sScaleFactor = scaleFactor; }
   
private:
   static float sScaleFactor;
};

} // End of gadget namespace

#endif
