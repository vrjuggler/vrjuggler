/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2003 by Iowa State University
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

#ifndef _GADGET_ANALOG_H_
#define _GADGET_ANALOG_H_

#include <gadget/gadgetConfig.h>

#include <gadget/Type/AnalogData.h>
#include <gadget/Type/SampleBuffer.h>

#include <jccl/Config/ConfigChunk.h>
#include <vpr/IO/SerializableObject.h>


namespace gadget
{

const unsigned short MSG_DATA_ANALOG = 421;

/**
 * Analog is the abstract base class that devices with Analog data derive
 * from.
 *
 * Analog is the base class that analog devices must derive from.
 * Analog inherits from Input, so it has pure virtual function
 * constraints from Input in the following functions: StartSampling,
 * StopSampling, Sample, and UpdateData. <br> <br>
 * Analog adds one new pure virtual function, GetAnalogData for retreiving
 * the analog data, similar to the addition for Position and Digital.
 */
class GADGET_CLASS_API Analog : public vpr::SerializableObject
{
public:
   typedef gadget::SampleBuffer<AnalogData> SampleBuffer_t;

public:

   /**
    * Constructor.
    * @post Set device abilities.
    * @note Must be called from all derived classes.
    */
   Analog();

   virtual ~Analog();

   virtual vpr::ReturnStatus writeObject(vpr::ObjectWriter* writer);

   virtual vpr::ReturnStatus readObject(vpr::ObjectReader* reader);

   /**
    * Just call base class config.
    * @note Let constructor set device abilities.
    */
   virtual bool config(jccl::ConfigChunkPtr c);

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
   AnalogData getAnalogData(int devNum = 0);

   /** Helper method to add a sample to the sample buffers.
   * This MUST be called by all analog devices to add a new sample.
   * The data samples passed in will then be modified by any local filters.
   * @post Sample is added to the buffers and the local filters are run on that sample.
   */
   void addAnalogSample(const std::vector< AnalogData >& anaSample);

   /** Swap the analog data buffers.
    * @post If ready has values, then copy values from ready to stable
    *        if not, then stable keeps its old values
    */
   void swapAnalogBuffers();


   const SampleBuffer_t::buffer_t& getAnalogDataBuffer();

   virtual std::string getBaseType();

protected:
   /**
    * Gives a value that will range from [min() <= n <= max()].
    * This returns a value that is normalized to the range of mMin <= n <= mMax
    * if n < mMin or n > mMax, then result = mMin or mMax respectively.
    */
   void normalizeMinToMax(const float& plainJaneValue,
                          float& normedFromMinToMax);

protected:
   /**
    * Gets the minimum "real" data value (real == from hardware).
    * This value is used to normalize the return value of getAnalogData.
    * @note this function is not needed by an application author.
    */
   float getMin() const;
   float getMax() const;
   void setMin(float mIn);
   void setMax(float mAx);

private:
   float mMin, mMax;

private:
   SampleBuffer_t    mAnalogSamples;  /**< Position samples */
   AnalogData        mDefaultValue;   /**< Default analog value to return */
};

} // End of gadget namespace


#endif   /* _GADGET_ANALOG_H_ */
