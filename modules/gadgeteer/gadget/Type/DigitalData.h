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

#ifndef _GADGET_DIGITAL_DATA_H_
#define _GADGET_DIGITAL_DATA_H_

#include <gadget/Type/InputData.h>

namespace gadget 
{

/** \class DigitalData DigitalData.h gadget/Type/DigitalData.h
 *
 * InputData subclass for digital data.
 */
class DigitalData : public InputData 
{
   // Hack around a GCC 3.3 bug on Mac OS X 10.3 that shows up with
   // boost::is_polymorphic.
#if defined(__MACH__) && defined(__APPLE_CC__) && defined(__GNUC__) && \
    __GNUC__ == 3 && __GNUC_MINOR__ == 3
   bool dummy_;
#endif

public:
   /** Constructor. */
   DigitalData()
      : InputData()
      , mDigitalData(0)
   {
      ;
   }

   DigitalData(const int d)
      : InputData()
      , mDigitalData(d)
   {
      ;
   }

   int getDigital() const
   {
      return mDigitalData;
   }

   void setDigital(const int d)
   {
      mDigitalData = d;
   }

   DigitalData& operator= (const DigitalData& pd)
   {
      InputData::copy (pd);
      mDigitalData = pd.mDigitalData;
      return *this;
   }

   DigitalData& operator= (const int data)
   {
      mDigitalData = data;
      return *this;
   }

   bool operator== (const DigitalData& o) const
   {
      return mDigitalData == o.mDigitalData && mTimeStamp == o.mTimeStamp;
   }

   bool operator!= (const DigitalData& o) const
   {
      return ! (*this == o);
   }

/*
   operator int() const
   {
      return mDigitalData;
   }
*/

protected:
   int mDigitalData;
}; // class DigitalData

} // namespace gadget


#endif /* _GADGET_DIGITAL_DATA_H_ */
