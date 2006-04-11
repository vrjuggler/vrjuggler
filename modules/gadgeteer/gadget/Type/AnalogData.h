/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2005 by Iowa State University
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

#ifndef _GADGET_ANALOG_DATA_H_
#define _GADGET_ANALOG_DATA_H_

#include <gadget/Type/InputData.h>

namespace gadget
{

/** \class AnalogData AnalogData.h gadget/Type/AnalogData.h
 *
 * InputData subclass for analog data.
 */
class AnalogData : public InputData
{
   // Hack around a GCC 3.3 bug on Mac OS X 10.3 that shows up with
   // boost::is_polymorphic.
#if defined(__MACH__) && defined(__APPLE_CC__) && defined(__GNUC__) && \
    __GNUC__ == 3 && __GNUC_MINOR__ == 3
   bool dummy_;
#endif

public:
   /** Constructor. */
   AnalogData ()
      : InputData()
      , mAnalogData(0.0f)
   {
      ;
   }

   AnalogData(float f)
      : InputData()
      , mAnalogData(f)
   {
      ;
   }

   float getAnalog() const
   {
      return mAnalogData;
   }

   void setAnalog(const float f)
   {
      mAnalogData = f;
   }

   AnalogData& operator= (const AnalogData& pd)
   {
      InputData::copy (pd);
      mAnalogData = pd.mAnalogData;
      return *this;
   }

   AnalogData& operator= (const float f)
   {
      mAnalogData = f;
      return *this;
   }

   bool operator== (const AnalogData& o) const
   {
      return mAnalogData == o.mAnalogData && mTimeStamp == o.mTimeStamp;
   }

   bool operator!= (const AnalogData& o) const
   {
      return ! (*this == o);
   }

/*
   operator float() const
   {
      return mAnalogData;
   }
*/

protected:
   float mAnalogData;
}; // class AnalogData

} // namespace gadget


#endif /* _GADGET_ANALOG_DATA_H_ */
