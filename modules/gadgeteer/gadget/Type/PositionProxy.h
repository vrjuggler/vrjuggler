/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998, 1999, 2000 by Iowa State University
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

#ifndef _VRJ_POSPROXY_H_
#define _VRJ_POSPROXY_H_

#include <vrj/vrjConfig.h>
#include <math.h>

#include <vrj/Input/Type/Position.h>
#include <vrj/Input/Type/Proxy.h>
#include <vrj/Math/Matrix.h>
#include <vrj/Input/Type/PosFilter.h>


namespace vrj
{

//-----------------------------------------------------------------------
//: A proxy class to positional devices, used by the InputManager.
//
//  A PosProxy always points to a positional device and subUnit number,
//  the inputgroup can therefore keep an array of these around and
//  treat them as positional devices which only return a single
//  subDevice's amount of data.  (one POS_DATA)
//
// See also: Position
//------------------------------------------------------------------------
//!PUBLIC_API:
class VJ_CLASS_API PosProxy : public TypedProxy<Position>
{
public:
   PosProxy() :  mUnitNum(-1), mETrans(false), mFilter(NULL)
   {;}

   virtual ~PosProxy() {}

   //: Update the proxy's copy of the data
   // Copy the device data to local storage, and transform it if necessary
   virtual void updateData() {
      if(!mStupified)
      {
         mPosData = *(mTypedDevice->getPosData(mUnitNum));
         mPosUpdateTime = *(mTypedDevice->getPosUpdateTime(mUnitNum));

         if(mETrans)
            transformData();

         // Filter the data if there is an active filters
         if(mFilter != NULL)
         {
            mPosData = mFilter->getPos(mPosData);
         }
      }
   }

    //: returns time of last update...
    TimeStamp* getUpdateTime () {
   return &mPosUpdateTime;
    }


   //: Set the transform for this PosProxy
   // Sets the transformation matrix to
   //    mMatrixTransform = M<sub>trans</sub>.post(M<sub>rot</sub>)
   //! NOTE: This means that to set transform, you specific the translation
   //+       followed by rotation that takes the device from where it physically
   //+       is in space to where you want it to be.
   void setTransform( float xoff, float yoff, float zoff,    // Translate
                      float xrot, float yrot, float zrot);   // Rotate

   //: Get the data
   Matrix* getData()
   {
      if(mStupified)
         mPosData.makeIdent();

      return &mPosData;
   }

   //: Return this device's subunit number
   int getUnit()
   { return mUnitNum; }

   //: Return the Position pointer held by this proxy
   Position* getPositionPtr()
   {
      if(!mStupified)
         return mTypedDevice;
      else
         return NULL;
   }

   //: Get the transform being using by this proxy
   Matrix& getTransform()
   { return mMatrixTransform; }

   //: Transform the data in mPosData
   //! PRE: mPosData needs to have most recent data
   //! POST: mPosData is transformed by the xform matrix
   //+       mPosData = old(mPosData).post(xformMatrix)
   //!NOTE: This moves the wMr to the modifed reciever system wMmr
   //+  where w = world, mr = world of the reciever, and r = reciever
   void transformData()
   { mPosData.postMult(mMatrixTransform); }

   static std::string getChunkType() { return "PosProxy"; }

   bool config(ConfigChunk* chunk);

   virtual Input* getProxiedInputDevice()
   {
      if(NULL == mTypedDevice)
         return NULL;

      Input* ret_val = dynamic_cast<Input*>(mTypedDevice);
      vprASSERT(ret_val != NULL);
      return ret_val;
   }

private:
   Matrix       mPosData;
   TimeStamp    mPosUpdateTime;
   Matrix       mMatrixTransform;        // reciever_t_modifiedReciever
   int            mUnitNum;
   bool           mETrans;                // Are transformation enabled;
   PosFilter*   mFilter;                // A possible position filter to use
};

};

#endif
