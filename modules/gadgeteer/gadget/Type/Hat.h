/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2011 by Iowa State University
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

#ifndef _GADGET_HAT_H_
#define _GADGET_HAT_H_

#include <gadget/gadgetConfig.h>

#include <vector>
#include <boost/noncopyable.hpp>
#include <boost/signals2/signal.hpp>

#include <vpr/Util/SignalProxy.h>
#include <vpr/IO/SerializableObject.h>

#include <jccl/Config/ConfigElementPtr.h>

#include <gadget/Type/HatData.h>
#include <gadget/Type/SampleBuffer.h>
#include <gadget/Type/HatPtr.h>


namespace gadget
{

const unsigned short MSG_DATA_HAT = 420;

/** \class Hat Hat.h gadget/Type/Hat.h
 *
 * Hat is the abstract base class from which devices with hat data
 * derive (through gadget::InputDevice). This is in addition to gadget::Input.
 * gadget::Input provides pure virtual function constraints in the following
 * functions: startSampling(), stopSampling(), sample(), and updateData().
 *
 * gadget::Hat adds the function getHatData() for retreiving the
 * received hat data. This is similar to the additions made by
 * gadget::Position and gadget::Analog.
 *
 * @see Input
 * @see InputDevice
 */
class GADGET_API Hat
   : public vpr::SerializableObject
   , protected HatState
   , private boost::noncopyable
{
public:
   typedef SampleBuffer<HatData> SampleBuffer_t;
   typedef boost::signals2::signal<void (const std::vector<HatData>&)>
      add_signal_t;

   /** @name Compatibility */
   //@{
   using HatState::CENTERED;
   using HatState::UP;
   using HatState::RIGHT;
   using HatState::DOWN;
   using HatState::LEFT;
   //@}

protected:
   /* Constructor/Destructors */
   Hat();

public:
   /**
    * Creates a Hat instance and returns it wrapped in a
    * HatPtr object.
    *
    * @since 1.3.7
    */
   static HatPtr create();

   virtual ~Hat();

   virtual bool config(jccl::ConfigElementPtr)
   {
      return true;
   }

   /**
    * Gets the hat data for the given devNum.
    *
    * @return Hat 0 or 1, if devNum makes sense.
    *         -1 is returned if function fails or if devNum is out of range.
    *
    * @note If devNum is out of range, function will fail, possibly issuing
    *       an error to a log or console - but will not ASSERT.
    */
   const HatData getHatData(int devNum = 0);

   /**
    * Helper method to add a collection of hat samples to the sample
    * buffers.  This MUST be called by all hat devices to add new
    * samples.
    *
    * @post The given hat samples are added to the buffers.
    *
    * @param hatSample A vector of HatData objects that represent the
    *                  newest samples taken.
    */
   void addHatSample(const std::vector<HatData>& hatSample);

   /**
    * Swaps the hat data buffers.
    *
    * @post If the ready queue has values, then those values are copied from
    *       the ready queue to the stable queue.  If not, then stable queue
    *       is not changed.
    */
   void swapHatBuffers()
   {
      mHatSamples.swapBuffers();
   }

   /**
    * Returns the current stable sample buffers for this device.
    */
   const SampleBuffer_t::buffer_t& getHatDataBuffer()
   {
      return mHatSamples.stableBuffer();
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

   /**
    * @since 2.1.6
    */
   vpr::SignalProxy<add_signal_t> dataAdded()
   {
      return mDataAdded;
   }

private:
   static const std::string sTypeName;

   add_signal_t mDataAdded;

   SampleBuffer_t    mHatSamples; /**< Hat samples */
   HatData       mDefaultValue;   /**< Default hat value to return */
};

} // End of gadget namespace

#endif   /* _GADGET_HAT_H_ */
