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
// positional tracker base class
//
////////////////////////////////////////////////////////////////////////

#ifndef _GADGET_POSITION_H_
#define _GADGET_POSITION_H_

#include <gadget/gadgetConfig.h>
#include <typeinfo>
#include <gadget/Type/Input.h>
#include <gadget/Type/PositionData.h>
#include <gadget/Type/SampleBuffer.h>

#include <gmtl/Matrix.h>
#include <vpr/Util/Debug.h>
//#include <vpr/IO/ObjectReader.h>
//#include <vpr/IO/ObjectWriter.h>
#include <gadget/RemoteInputManager/SerializableDevice.h>


namespace gadget
{
   const unsigned short MSG_DATA_POS = 422;

/** Position is the abstract base class that devices with digital data derive from.
*
*  Position is the base class that digital devices must derive from.  Position
*  inherits from Input, so it has pure virtual function constraints from
*  Input in the following functions: StartSampling,StopSampling,Sample,
*  and UpdateData.
*
*  Position objects have the ability to convert from the tracker's coord system
*  to the Juggler coordinate system.
*
*  Position adds one new pure virtual function, GetPosData for retreiving
*  the positional data, similar to the addition for Analog and Digital.
*/
class GADGET_CLASS_API Position : public SerializableDevice
{
public:
   typedef gadget::SampleBuffer<PositionData> SampleBuffer_t;

public:
   /** Constructor */
   Position();

   /** Destructor */
   virtual ~Position();

   virtual bool config(jccl::ConfigChunkPtr c);


   /** Get Positional data. */
   PositionData getPositionData (int devNum = 0)
   {
      SampleBuffer_t::buffer_t& stable_buffer = mPosSamples.stableBuffer();

      if ((!stable_buffer.empty()) &&
          (stable_buffer.back().size() > (unsigned)devNum))  // If Have entry && devNum in range
      {
         return stable_buffer.back()[devNum];
      }
      else        // No data or request out of range, return default value
      {
         if(stable_buffer.empty())
         {
            vprDEBUG(vprDBG_ALL, vprDBG_WARNING_LVL) << "Warning: Position::getPositionData: Stable buffer is empty. If this is not the first read, then this is a problem.\n" << vprDEBUG_FLUSH;
         }
         else
         {
            vprDEBUG(vprDBG_ALL, vprDBG_CONFIG_LVL) << "Warning: Position::getPositionData: Requested devNum is not in the range available.  May have configuration error\n" << vprDEBUG_FLUSH;
         }

         return mDefaultValue;
      }
   }
    virtual std::string getBaseType()
    {
        return std::string("Position");
    }

   virtual vpr::ReturnStatus writeObject(vpr::ObjectWriter* writer);
   virtual vpr::ReturnStatus readObject(vpr::ObjectReader* reader, vpr::Uint64* delta);



   const SampleBuffer_t::buffer_t& getPositionDataBuffer()
   {
      return mPosSamples.stableBuffer();
   }

public:
   SampleBuffer_t    mPosSamples;   /**< Position samples */
   PositionData      mDefaultValue;   /**< Default analog value to return */

   gmtl::Matrix44f xformMat;   /**< The total xform matrix.  T*R  NOTE: Used to move from trk coord system to Juggler coord system */
   gmtl::Matrix44f rotMat;     /**< Only the rotation matrix */
};

} // End of gadget namespace

#endif
