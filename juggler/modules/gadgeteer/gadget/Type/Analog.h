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
#ifndef _VRJ_ANALOG_H_
#define _VRJ_ANALOG_H_

#include <vrj/vrjConfig.h>
#include <vrj/Config/ConfigChunk.h>
#include <vrj/Util/Debug.h>

namespace vrj
{

//-----------------------------------------------------------------------------
//: Analog is the abstract base class that devices with digital data derive
//+ from.
//
//  Analog is the base class that digital devices must derive from.
//  Analog inherits from Input, so it has pure virtual function
//  constraints from Input in the following functions: StartSampling,
//  StopSampling, Sample, and UpdateData. <br> <br>
//
//  Analog adds one new pure virtual function, GetAnalogData for retreiving
//  the digital data, similar to the addition for Position and Digital.
//!PUBLIC_API:
//-----------------------------------------------------------------------------
class Analog
{
public:

   //: Constructor
   //! POST: Set device abilities
   //! NOTE: Must be called from all derived classes
   Analog() : mMin( 0.0f ), mMax( 1.0f )
   {;}

   virtual ~Analog() {}

   // Just call base class config
   //! NOTE: Let constructor set device abilities
   virtual bool config(ConfigChunk* c)
   {
      mMin = static_cast<float>( c->getProperty("min") );
      mMax = static_cast<float>( c->getProperty("max") );

      vprDEBUG(vprDBG_ALL,4)<<"*** SimAnalog::config() min:"<<mMin<<" max:"<<mMax<<"\n"<< vprDEBUG_FLUSH;

      return true;
   }

   /* new pure virtual functions */
   //: Return "analog data"..
   //  Gee, that's ambiguous especially on a discrete system such as a digital computer....
   //
   //! PRE: give the device number you wish to access.
   //! POST: returns a value that ranges from 0.0f to 1.0f
   //! NOTE: for example, if you are sampling a potentiometer, and it returns reading from
   //        0, 255 - this function will normalize those values (using Analog::normalizeMinToMax())
   //        for another example, if your potentiometer's turn radius is limited mechanically to return
   //        say, the values 176 to 200 (yes this is really low res), this function will still return
   //        0.0f to 1.0f.
   //! NOTE: to specify these min/max values, you must set in your Analog (or analog device) config
   //        file the field "min" and "max".  By default (if these values do not appear),
   //        "min" and "max" are set to 0.0f and 1.0f respectivly.
   //! NOTE: TO ALL ANALOG DEVICE DRIVER WRITERS, you *must* normalize your data using
   //        Analog::normalizeMinToMax()
   virtual float getAnalogData(int devNum = 0) = 0;

protected:
   // give a value that will range from [min() <= n <= max()]
   // return a value that is normalized to the range of mMin <= n <= mMax
   // if n < mMin or n > mMax, then result = mMin or mMax respectively.
   void normalizeMinToMax( const float& plainJaneValue, float& normedFromMinToMax )
   {
      float value = plainJaneValue;

      // first clamp the value so that min<=value<=max
      if (value < mMin) value = mMin;
      if (value > mMax) value = mMax;

      // slide everything to 0.0 (subtract all by mMin)
      // Then divide by max to get normalized value
      float tmax( mMax - mMin),
            tvalue(value - mMin);

      // since [tmin/tmax...tmax/tmax] == [0.0f...1.0f], the normalized value will be value/tmax
      normedFromMinToMax = tvalue / tmax;
   }


protected:
   // get the minimum "real" data value (real == from hardware)
   // this value is used to normalize the return value of getAnalogData
   // NOTE: this function is not needed by an application author.
   float getMin() const { return mMin; }
   float getMax() const { return mMax; }
   void setMin( float mIn ) { mMin = mIn; }
   void setMax( float mAx ) { mMax = mAx; }

private:
   float mMin, mMax;
};


};

#endif   /* _VJ_ANALOG_H_ */
