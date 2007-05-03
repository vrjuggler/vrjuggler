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

#ifndef _GADGET_DIGITAL_H_
#define _GADGET_DIGITAL_H_

#include <gadget/gadgetConfig.h>

#include <vector>
#include <boost/concept_check.hpp>   /* for ignore_unused_variable_warning */
#include <boost/noncopyable.hpp>

#include <vpr/IO/SerializableObject.h>
#include <jccl/Config/ConfigElementPtr.h>
#include <gadget/Type/DigitalData.h>
#include <gadget/Type/SampleBuffer.h>
#include <gadget/Type/DigitalPtr.h>

namespace gadget
{

const unsigned short MSG_DATA_DIGITAL = 420;

/** \class Digital Digital.h gadget/Type/Digital.h
 *
 * Digital is the abstract base class from which devices with digital data
 * derive (through gadget::InputMixer).  This is in addition to
 * gadget::Input.  gadget::Input provides pure virtual function constraints
 * in the following functions: startSampling(), stopSampling(), sample(),
 * and updateData().
 *
 * gadget::Digital adds the function getDigitalData() for retreiving the
 * received digital data.  This is similar to the additions made by
 * gadget::Position and gadget::Analog.
 *
 * @see Input, InputMixer
 */
class GADGET_CLASS_API Digital
   : public vpr::SerializableObject
   , boost::noncopyable
{
public:
   typedef gadget::SampleBuffer<DigitalData> SampleBuffer_t;

public:
   /**
    * Enum for the state of the digital buttons.
    * Used in DigitalProxy.
    */
   enum State
   {
      OFF = 0,       /**< Device is in the "off" state. */
      ON = 1,        /**< Device is in the "on" state. */
      TOGGLE_ON = 2, /**< Device was in the "off" state and has changed to
                          "on" since the last frame. */
      TOGGLE_OFF = 3 /**< Device was in the "on" state and has changed to
                          "off" since the last frame. */
   };

protected:
   /* Constructor/Destructors */
   Digital();

public:
   /**
    * Creates a Digital instance and returns it wrapped in a
    * DigitalPtr object.
    *
    * @since 1.3.7
    */
   static DigitalPtr create();

   virtual ~Digital();

   virtual bool config(jccl::ConfigElementPtr e)
   {
      boost::ignore_unused_variable_warning(e);
      return true;
   }

   /**
    * Gets the digital data for the given devNum.
    *
    * @return Digital 0 or 1, if devNum makes sense.
    *         -1 is returned if function fails or if devNum is out of range.
    *
    * @note If devNum is out of range, function will fail, possibly issuing
    *       an error to a log or console - but will not ASSERT.
    */
   const DigitalData getDigitalData(int devNum = 0);

   /**
    * Helper method to add a collection of digital samples to the sample
    * buffers.  This MUST be called by all digital devices to add new
    * samples.
    *
    * @post The given digital samples are added to the buffers.
    *
    * @param digSample A vector of DigitalData objects that represent the
    *                  newest samples taken.
    */
   void addDigitalSample(const std::vector< DigitalData >& digSample)
   {
      // Locks and then swaps the indices.
      mDigitalSamples.lock();
      mDigitalSamples.addSample(digSample);
      mDigitalSamples.unlock();
   }

   /**
    * Swaps the digital data buffers.
    *
    * @post If the ready queue has values, then those values are copied from
    *       the ready queue to the stable queue.  If not, then stable queue
    *       is not changed.
    */
   void swapDigitalBuffers()
   {
      mDigitalSamples.swapBuffers();
   }

   /**
    * Returns the current stable sample buffers for this device.
    */
   const SampleBuffer_t::buffer_t& getDigitalDataBuffer()
   {
      return mDigitalSamples.stableBuffer();
   }
   virtual std::string getInputTypeName()
   {
      return std::string("Digital");
   }

   /**
    * Serializes this object into the given object writer.
    *
    * @param writer The object writer to which this object will be
    *               serialized.
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

private:
   SampleBuffer_t    mDigitalSamples; /**< Digital samples */
   DigitalData       mDefaultValue;   /**< Default digital value to return */
};

} // End of gadget namespace

#endif   /* _GADGET_DIGITAL_H_ */
