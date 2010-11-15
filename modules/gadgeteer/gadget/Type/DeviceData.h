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

#ifndef _GADGET_DEVICE_DATA_H_
#define _GADGET_DEVICE_DATA_H_

#include <gadget/gadgetConfig.h>

#include <gadget/Type/InputData.h>


namespace gadget
{

/** \class DeviceData DeviceData.h gadget/Type/DeviceData.h
 *
 * Generic storage for input read from Gadgeteer device drivers.
 *
 * @since 2.1.1
 */
template<typename T>
class DeviceData : public InputData
{
public:
   typedef T data_type;

   /** @name Constructors */
   //@{
   /**
    * Default constructor.
    *
    * @post The value held by this object is initialized using its default
    *       initializer/constructor.
    */
   DeviceData()
   {
      /* Do nothing. */ ;
   }

   /**
    * Value constructor.
    *
    * @post \c mValue holds a copy of \p value.
    *
    * @param value The value to use for initializing the data held by this
    *              object.
    */
   DeviceData(const T& value)
      : mValue(value)
   {
      /* Do nothing. */ ;
   }
   //@}

   /** @name Value Access */
   //@{
   /**
    * Returns a mutable reference to the value held by this object.
    */
   T& editValue()
   {
     return mValue;
   }

   /**
    * Returns the value held by this object.
    */
   const T& getValue() const
   {
      return mValue;
   }

   /**
    * Sets the value held by this object.
    */
   void setValue(const T& value)
   {
      mValue = value;
   }
   //@}

   /** @name Assignment Operators */
   //@{
   DeviceData& operator=(const DeviceData& other)
   {
      InputData::copy(other);
      mValue = other.mValue;
      return *this;
   }

   DeviceData& operator=(const T& value)
   {
      mValue = value;
      return *this;
   }
   //@}

   /** @name Equality Operators */
   //@{
   bool operator==(const DeviceData& other) const
   {
      return mValue == other.mValue && InputData::isEqual(other);
   }

   bool operator!=(const DeviceData& other) const
   {
      return ! (*this == other);
   }
   //@}

private:
   T mValue;
};

}


#endif /* _GADGET_DEVICE_DATA_H_ */
