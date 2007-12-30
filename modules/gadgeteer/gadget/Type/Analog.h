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
#include <boost/noncopyable.hpp>

#include <vpr/IO/SerializableObject.h>
#include <jccl/Config/ConfigElement.h>

#include <gadget/Type/AnalogData.h>
#include <gadget/Type/AnalogPtr.h>
#include <gadget/Type/SampleBuffer.h>


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
 * @note To specify the min/max values for an anlog device, the required "min"
 *       and "max" properties of a config elment type derived from "analog"
 *       must be set. By default (i.e., if these values do not appear), "min"
 *       and "max" are set to 0.0f and 1.0f respectively.
 *
 * @note For version 1.3.18 and newer, data normalization is performed in
 *       gadget::AnalogProxy::updateData(). Analog device driver authors no
 *       longer need to perform data normalization.
 *
 * @see Input, InputMixer
 */
class GADGET_CLASS_API Analog
   : public vpr::SerializableObject
   , boost::noncopyable
{
public:
   typedef gadget::SampleBuffer<AnalogData> SampleBuffer_t;

protected:
   /**
    * Constructor.
    * @post Set device abilities.
    * @note Must be called from all derived classes.
    */
   Analog();

public:
   /**
    * Creates a Analog instance and returns it wrapped in a
    * AnalogPtr object.
    *
    * @since 1.3.7
    */
   static AnalogPtr create();

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

   virtual std::string getInputTypeName()
   {
      return std::string("Analog");
   }

   /**
    * Given a value that will range from [min() <= n <= max()], this returns
    * a value that is normalized to the range [0,1].
    *
    * @param rawData A non-normalized data value read from this analog device.
    *
    * @return The normalized form of \p rawData in the range [0,1].
    *
    * @note This method was renamed from normalizeMinToMax() (a protected
    *       method) in version 1.3.18. Analog device drivers are no longer
    *       responsible for normalizing data. Instead, that operation is
    *       performed "on demand" either by a method such as
    *       gadget::AnalogProxy::updateData() or as desired by user-level code
    *       that retrieves the full analog sample buffer collection.
    *
    * @since 1.3.18
    *
    * @see getMin()
    * @see setMin()
    * @see getMax()
    * @see setMax()
    * @see getAnalogDataBuffer()
    */
   float normalize(const float rawData) const;

   /**
    * @name Data Bounds Interface
    *
    * These methods can be used for data normalization.
    */
   //@{
   float getMin() const
   {
      return mMin;
   }

   float getMax() const
   {
      return mMax;
   }
   //@}

protected:
   void setMin(const float minValue);
   void setMax(const float maxValue);

private:
   float mMin;
   float mMax;

   SampleBuffer_t    mAnalogSamples;  /**< Position samples */
   AnalogData        mDefaultValue;   /**< Default analog value to return */
};

} // End of gadget namespace


#endif   /* _GADGET_ANALOG_H_ */
