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
#ifndef _VJ_POSPROXY_H_
#define _VJ_POSPROXY_H_

#include <vjConfig.h>
#include <math.h>

#include <Input/vjPosition/vjPosition.h>
#include <Input/InputManager/vjProxy.h>
#include <Math/vjMatrix.h>
#include <Input/Filter/vjPosFilter.h>

//-----------------------------------------------------------------------
//: A proxy class to positional devices, used by the vjInputManager.
//
//  A vjPosProxy always points to a positional device and subUnit number,
//  the inputgroup can therefore keep an array of these around and
//  treat them as positional devices which only return a single
//  subDevice's amount of data.  (one vjPOS_DATA)
//
// See also: vjPosition
//------------------------------------------------------------------------
//!PUBLIC_API:
class vjPosProxy : public vjProxy
{
public:
   vjPosProxy() : mPosPtr(NULL), mUnitNum(-1), mETrans(false), mFilter(NULL)
   {;}

   virtual ~vjPosProxy() {}


   //: Update the proxy's copy of the data
   // Copy the device data to local storage, and transform it if necessary
   void updateData() {
      mPosData = *(mPosPtr->getPosData(mUnitNum));
      vjTimeStamp* ts = mPosPtr->getPosUpdateTime(mUnitNum);
      if (ts)
          mPosUpdateTime = *ts;

      if(mETrans)
         transformData();

      // Filter the data if there is an active filters
      if(mFilter != NULL)
      {
         mPosData = mFilter->getPos(mPosData);
      }
   }

    //: returns time of last update...
    vjTimeStamp* getUpdateTime () {
   return &mPosUpdateTime;
    }


   //: Set the transform for this vjPosProxy
   // Sets the transformation matrix to
   //    mMatrixTransform = M<sub>trans</sub>.post(M<sub>rot</sub>)
   //! NOTE: This means that to set transform, you specific the translation
   //+       followed by rotation that takes the device from where it physically
   //+       is in space to where you want it to be.
   void setTransform( float xoff, float yoff, float zoff,    // Translate
                      float xrot, float yrot, float zrot);   // Rotate

   //: Set the vjPosProxy to now point to another device and subDevicenumber
   void set(vjPosition* posPtr, int unitNum);

   //: Get the data
   vjMatrix* getData()
   { return &mPosData; }

   //: Return this device's subunit number
   int getUnit()
   { return mUnitNum; }

   //: Return the vjPosition pointer held by this proxy
   vjPosition* getPositionPtr()
   { return mPosPtr; }

   //: Get the transform being using by this proxy
   vjMatrix& getTransform()
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

   bool config(vjConfigChunk* chunk);

   virtual vjInput* getProxiedInputDevice()
   {
      vjInput* ret_val = dynamic_cast<vjInput*>(mPosPtr);
      vjASSERT(ret_val != NULL);
      return ret_val;
   }

private:
   vjMatrix       mPosData;
   vjTimeStamp    mPosUpdateTime;
   vjMatrix       mMatrixTransform;        // reciever_t_modifiedReciever
   vjPosition*    mPosPtr;                 // Ptr to the position device
   int            mUnitNum;
   bool           mETrans;                // Are transformation enabled;
   vjPosFilter*   mFilter;                // A possible position filter to use
};

#endif
