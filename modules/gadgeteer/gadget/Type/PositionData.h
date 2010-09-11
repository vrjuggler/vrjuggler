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

#ifndef _GADGET_POSITION_DATA_H_
#define _GADGET_POSITION_DATA_H_

#include <gadget/Type/InputData.h>
#include <gmtl/Matrix.h>
#include <gmtl/MatrixOps.h>


namespace gadget
{

/** \class PositionData PositionData.h gadget/Type/PositionData.h
 *
 * InputData subclass for positional data.
 */
class PositionData : public InputData
{
public:
   /** Constructor. */
   PositionData()
      : InputData()
   {
      ;
   }

   /**
    * @deprecated Use getValue() for forward compatibility with the next major
    *             relesae of Gadgeteer.
    */
   const gmtl::Matrix44f& getPosition() const
   {
      return mPosData;
   }

   /**
    * @deprecated Use setValue() for forward compatibility with the next major
    *             relesae of Gadgeteer.
    */
   void setPosition(const gmtl::Matrix44f& posMatrix)
   {
      mPosData = posMatrix;
   }

   /**
    * @name Forward Compatibility
    *
    * These methods are for forward compatibility with gadget::DigitalData<T>,
    * a type that will be used as a replacement for gadget::PositionData in
    * the next major release of Gadgeteer.
    */
   //@{
   /**
    * @since 2.0.0
    */
   const gmtl::Matrix44f& getValue() const
   {
      return getPosition();
   }

   /**
    * @since 2.0.0
    */
   void setValue(const gmtl::Matrix44f& posMatrix)
   {
      setPosition(posMatrix);
   }
   //@}

   PositionData& operator= (const PositionData& pd)
   {
      InputData::copy (pd);
      mPosData = pd.mPosData;
      return *this;
   }

   bool operator== (const PositionData& o) const
   {
      return mPosData == o.mPosData && mTimeStamp == o.mTimeStamp;
   }

   bool operator!= (const PositionData& o) const
   {
      return ! (*this == o);
   }

public:
   gmtl::Matrix44f mPosData;
}; // class PositionData

} // namespace gadget


#endif /* _GADGET_POSITION_DATA_H_ */
