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

#ifndef _GADGET_STRING_H_
#define _GADGET_STRING_H_

#include <gadget/gadgetConfig.h>

#include <vector>
#include <boost/noncopyable.hpp>
#include <boost/signal.hpp>

#include <vpr/Util/SignalProxy.h>
#include <vpr/IO/SerializableObject.h>

#include <jccl/Config/ConfigElementPtr.h>

#include <gadget/Type/SampleBuffer.h>
#include <gadget/Type/StringData.h>
#include <gadget/Type/StringPtr.h>


namespace gadget
{

const unsigned short MSG_DATA_STRING = 430;

/** \class String String.h gadget/Type/String.h
 *
 * gadget::String is the abstract base class for devices that return
 * spoken commends.  Drivers for all such devices must derive from this
 * class (through gadget::InputMixer).  This is in addition to
 * gadget::Input.  gadget::Input provides pure virtual function constraints
 * in the following functions: startSampling(), stopSampling(), sample(),
 * and updateData().
 *
 * gadget::String adds the function getStringData() for retreiving the
 * received commands.  This is similar to the additions made
 * by gadget::Position and gadget::Analog.
 *
 * @note This interface should be considered as being in an "alpha" state.
 *       String-oriented device types are very new to Gadgeteer, and they
 *       have not have the same amount of time to mature as the other
 *       device types (which have been around since VR Juggler 1.0).  This
 *       interface may change in a future release of Gadgeteer (such as
 *       Version 1.2).
 *
 * @see Input, InputMixer
 */
class GADGET_CLASS_API String
   : public vpr::SerializableObject
   , boost::noncopyable
{
public:
   typedef SampleBuffer<StringData> SampleBuffer_t;
   typedef boost::signal<void (const std::vector<StringData>&)> add_signal_t;

public:
   /* Constructor/Destructors */
   String();

   /**
    * Creates a String instance and returns it wrapped in a
    * StringPtr object.
    *
    * @since 1.3.7
    */
   static StringPtr create();

   virtual ~String();

   virtual bool config(jccl::ConfigElementPtr)
   {
      return true;
   }

   /**
    * Gets the string data for the given devNum.
    *
    * @note If devNum is out of range, function will fail, possibly issuing
    *       an error to a log or console - but will not ASSERT.
    */
   const StringData& getStringData(const int devNum = 0) const;

   /**
    * Helper method to add a collection of string samples to the sample
    * buffers.  This MUST be called by all string devices to add new
    * samples.
    *
    * @post The given string samples are added to the buffers.
    *
    * @param stringSample A vector of StringData objects that represent the
    *                     newest samples taken.
    */
   void addStringSample(const std::vector<StringData>& stringSample);

   /**
    * Swaps the string data buffers.
    *
    * @post If the ready queue has values, then those values are copied from
    *       the ready queue to the stable queue.  If not, then stable queue
    *       is not changed.
    */
   void swapStringBuffers()
   {
      mStringSamples.swapBuffers();
   }

   const SampleBuffer_t::buffer_t& getStringDataBuffer()
   {
      return mStringSamples.stableBuffer();
   }

   /**
    * Returns the Input type name used to map into the BaseTypeFactory. In a 
    * device driver, do not override this method unless the device driver is
    * also going to be implementing a new core Input device type.
    */
   virtual std::string getInputTypeName()
   {
      return std::string("String");
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
    * @since 2.1.7
    */
   vpr::SignalProxy<add_signal_t> dataAdded()
   {
      return mDataAdded;
   }

private:
   add_signal_t mDataAdded;

   SampleBuffer_t mStringSamples;  /**< String samples */
   StringData     mDefaultValue;   /**< Default String value to return */
};

} // End of gadget namespace

#endif   /* _GADGET_STRING_H_ */
