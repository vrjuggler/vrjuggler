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
// positional tracker base class
//
////////////////////////////////////////////////////////////////////////
#ifndef _GADGET_ANALOG_H_
#define _GADGET_ANALOG_H_

#include <gadget/gadgetConfig.h>

#include <gadget/Type/AnalogData.h>
#include <gadget/Type/SampleBuffer.h>

#include <jccl/Config/ConfigChunk.h>
#include <vpr/Util/Debug.h>


namespace gadget
{

/**
 * Analog is the abstract base class that devices with digital data derive
 * from.
 *
 * Analog is the base class that digital devices must derive from.
 * Analog inherits from Input, so it has pure virtual function
 * constraints from Input in the following functions: StartSampling,
 * StopSampling, Sample, and UpdateData. <br> <br>
 *
 * Analog adds one new pure virtual function, GetAnalogData for retreiving
 * the digital data, similar to the addition for Position and Digital.
 */
class Analog
{
public:
   typedef gadget::SampleBuffer<AnalogData> SampleBuffer_t;

public:

   /**
    * Constructor.
    * @post Set device abilities.
    * @note Must be called from all derived classes.
    */
   Analog() : mMin( 0.0f ), mMax( 1.0f )
   {;}

   virtual ~Analog() {}

   /**
    * Just call base class config.
    * @note Let constructor set device abilities.
    */
   virtual bool config(jccl::ConfigChunkPtr c)
   {
      mMin = c->getProperty<float>("min");
      mMax = c->getProperty<float>("max");

      vprDEBUG(vprDBG_ALL,4) << " SimAnalog::config() min:" << mMin
                                                 << " max:" << mMax << "\n" << vprDEBUG_FLUSH;

      return true;
   }

   /* new pure virtual functions */
   /**
    * Returns "analog data".
    * Gee, that's ambiguous especially on a discrete system such as a digital
    * computer....
    *
    * @pre Give the device number you wish to access.
    * @post Returns a value that ranges from 0.0f to 1.0f.
    *
    * @note For example, if you are sampling a potentiometer, and it returns
    *       reading from 0, 255.  This function will normalize those values
    *       (using Analog::normalizeMinToMax()).  For another example, if
    *       your potentiometer's turn radius is limited mechanically to return,
    *       say, the values 176 to 200 (yes this is really low res), this
    *       function will still return 0.0f to 1.0f.
    * @note To specify these min/max values, you must set in your Analog (or
    *       analog device) config file the field "min" and "max".  By default
    *       (if these values do not appear), "min" and "max" are set to 0.0f
    *       and 1.0f respectivly.
    * @note TO ALL ANALOG DEVICE DRIVER WRITERS, you *must* normalize your
    *       data using Analog::normalizeMinToMax().
    */
   // XXX: Add a "sample" filter that does the normalization in here instead
   // of in the driver.
   AnalogData getAnalogData(int devNum = 0)
   {
      SampleBuffer_t::buffer_t& stable_buffer = mAnalogSamples.stableBuffer();

      if((!stable_buffer.empty()) &&
         (stable_buffer.back().size() > (unsigned)devNum))  // If Have entry && devNum in range
      {
         return stable_buffer.back()[devNum];
      }
      else        // No data or request out of range, return default value
      {
         if(stable_buffer.empty())
         {
            vprDEBUG(vprDBG_ALL, vprDBG_WARNING_LVL) << "Warning: Analog::getAnalogData: Stable buffer is empty. If this is not the first read, then this is a problem.\n" << vprDEBUG_FLUSH;
         }
         else
         {
            vprDEBUG(vprDBG_ALL, vprDBG_CONFIG_LVL) << "Warning: Analog::getAnalogData: Requested devNum is not in the range available.  May have configuration error\n" << vprDEBUG_FLUSH;
         }
         return mDefaultValue;
      }
   }

   const SampleBuffer_t::buffer_t& getAnalogDataBuffer()
   {
      return mAnalogSamples.stableBuffer();
   }

protected:
   /**
    * Gives a value that will range from [min() <= n <= max()].
    * This returns a value that is normalized to the range of mMin <= n <= mMax
    * if n < mMin or n > mMax, then result = mMin or mMax respectively.
    */
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
   /**
    * Gets the minimum "real" data value (real == from hardware).
    * This value is used to normalize the return value of getAnalogData.
    * @note this function is not needed by an application author.
    */
   float getMin() const { return mMin; }
   float getMax() const { return mMax; }
   void setMin( float mIn ) { mMin = mIn; }
   void setMax( float mAx ) { mMax = mAx; }

private:
   float mMin, mMax;

protected:
   SampleBuffer_t    mAnalogSamples;  /**< Position samples */
   AnalogData        mDefaultValue;   /**< Default analog value to return */
};


};

#endif   /* _GADGET_ANALOG_H_ */
