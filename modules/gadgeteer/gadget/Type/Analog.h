/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2007 by Iowa State University
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

#ifndef _GADGET_ANALOG_H_
#define _GADGET_ANALOG_H_

#include <gadget/gadgetConfig.h>
#include <vector>

#include <gadget/Type/AnalogData.h>
#include <gadget/Type/SampleBuffer.h>

#include <jccl/Config/ConfigElement.h>
#include <vpr/IO/SerializableObject.h>


namespace gadget
{

const unsigned short MSG_DATA_ANALOG = 421;

/** \class Analog Analog.h gadget/Type/Analog.h
 *
 * Analog is the abstract base class from which devices returning analog data
 * must derive (through the use of gadget::InputMixer).  This is in addition
 * to gadget::Input.  gadget::Input provides pure virtual function constraints
 * in the following functions: startSampling(), stopSampling(), sample(), and
 * updateData().
 *
 * gadget::Analog adds the function getAnalogData() for retreiving the
 * received analog data.  This is similar to the additions made by
 * gadget::Position and gadget::Digital.
 *
 * @see Input, InputMixer
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

   /**
    * Serializes this object into the given object writer.
    *
    * @param writer The object writer to which this object will be serialized.
    *
    * @throw vpr::IOException is thrown if serialization fails.
    */
   virtual void writeObject(vpr::ObjectWriter* writer);

   /**
    * De-serializes this object.
    *
    * @param reader The object reader from which this object will be
    *               de-serialized.
    *
    * @throw vpr::IOException is thrown if de-serialization fails.
    */
   virtual void readObject(vpr::ObjectReader* reader);

   /**
    * Reads the minimum and maximum value configuration information for this
    * analog device.
    *
    * @post mMin and mMax are set to the values contained in the given config
    *       element.
    *
    * @param element The config element for an analog device.  It must derive
    *                from the base config element type 'analog'.
    */
   virtual bool config(jccl::ConfigElementPtr element);

   /**
    * Returns analog data.
    *
    * @post Returns a value that ranges from 0.0f to 1.0f.
    *
    * @param devNum Device unit number to access.
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

   /**
    * Helper method to add a collection of analog samples to the sample
    * buffers.  This MUST be called by all analog devices to add new samples.
    * The data samples passed in will then be modified by any local filters.
    *
    * @post The given analog samples are added to the buffers, and the local
    *       filters are run on the new samples.
    *
    * @param anaSample A vector of AnalogData objects that represent the
    *                  newest samples taken.
    */
   void addAnalogSample(const std::vector< AnalogData >& anaSample);

   /**
    * Swaps the analog data buffers.
    *
    * @post If the ready queue has values, then those values are copied from
    *       the ready queue to the stable queue.  If not, then stable queue
    *       is not changed.
    */
   void swapAnalogBuffers();

   /**
    * Returns the current stable sample buffers for this device.
    */
   const SampleBuffer_t::buffer_t& getAnalogDataBuffer();

   virtual std::string getInputTypeName();

protected:
   /**
    * Given a value that will range from [min() <= n <= max()].
    * This returns a value that is normalized to [0,1]
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

   // gadget::SampleBuffer<T> is not copyable, so neither are we.
   Analog(const gadget::Analog& d)
      : vpr::SerializableObject(d)
   {;}

   void operator=(const gadget::Analog&) {;}

private:
   float mMin, mMax;

private:
   SampleBuffer_t    mAnalogSamples;  /**< Position samples */
   AnalogData        mDefaultValue;   /**< Default analog value to return */
};

} // End of gadget namespace


#endif   /* _GADGET_ANALOG_H_ */
