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

#include <gadget/gadgetConfig.h>

#include <boost/concept_check.hpp>

#include <gmtl/Math.h>

#include <vpr/IO/ObjectWriter.h>
#include <vpr/IO/ObjectReader.h>
#include <vpr/Util/Debug.h>

#include <jccl/Config/ConfigElement.h>

#include <gadget/Util/DeviceSerializationTokens.h>
#include <gadget/Type/Analog.h>


namespace gadget
{

Analog::Analog()
   : mMin(0.0f)
   , mMax(1.0f)
{
   /* Do nothing. */ ;
}

Analog::~Analog()
{
   /* Do nothing. */ ;
}

AnalogPtr Analog::create()
{
   return AnalogPtr(new Analog());
}

void Analog::writeObject(vpr::ObjectWriter* writer)
{
   //std::cout << "[Remote Input Manager] In Analog write" << std::endl;

   SampleBuffer_t::buffer_t& stable_buffer = mAnalogSamples.stableBuffer();
   writer->beginTag(Analog::getInputTypeName());
   writer->beginAttribute(tokens::DataTypeAttrib);
      writer->writeUint16(MSG_DATA_ANALOG);                                   // Write out the data type so that we can assert if reading in wrong place
   writer->endAttribute();

   if ( !stable_buffer.empty() )
   {
      mAnalogSamples.lock();
      writer->beginAttribute(tokens::SampleBufferLenAttrib);
         writer->writeUint16(stable_buffer.size());                           // Write the # of vectors in the stable buffer
      writer->endAttribute();
      for (unsigned int j = 0; j < stable_buffer.size(); ++j)                   // For each vector in the stable buffer
      {
         writer->beginTag(tokens::BufferSampleTag);
         writer->beginAttribute(tokens::BufferSampleLenAttrib);
            writer->writeUint16(stable_buffer[j].size());                     // Write the # of AnalogDatas in the vector
         writer->endAttribute();
         for (unsigned int i = 0; i < stable_buffer[j].size(); ++i)           // For each AnalogData in the vector
         {
            writer->beginTag(tokens::AnalogValue);
            writer->beginAttribute(tokens::TimeStamp);
               writer->writeUint64(stable_buffer[j][i].getTime().usec());        // Write Time Stamp vpr::Uint64
            writer->endAttribute();
            writer->writeFloat(stable_buffer[j][i].getValue());               // Write Analog Data(int)
            writer->endTag();
         }
         writer->endTag();
      }
      mAnalogSamples.unlock();
   }
   else        // No data or request out of range, return default value
   {
      writer->beginTag(tokens::BufferSampleTag);
         writer->beginAttribute(tokens::BufferSampleLenAttrib);
            writer->writeUint16(0);
         writer->endAttribute();
      writer->endTag();
      vprDEBUG(vprDBG_ALL, vprDBG_WARNING_LVL)
         << "WARNING: [gadget::Analog::writeObject()] Stable buffer is empty.  "
         << "If this is not the first write, then this is a problem.\n"
         << vprDEBUG_FLUSH;
   }
   writer->endTag();
}

void Analog::readObject(vpr::ObjectReader* reader)
{
   vprASSERT(reader->attribExists("rim.timestamp.delta"));
   vpr::Uint64 delta = reader->getAttrib<vpr::Uint64>("rim.timestamp.delta");

   reader->beginTag(Analog::getInputTypeName());
   reader->beginAttribute(tokens::DataTypeAttrib);
      vpr::Uint16 temp = reader->readUint16();
   reader->endAttribute();

   // ASSERT if this data is really not Analog Data
   // XXX: Should there be error checking for the case when vprASSERT is
   // compied out?  -PH 8/21/2003
   vprASSERT(temp==MSG_DATA_ANALOG && "[Remote Input Manager] Not Analog Data");
   boost::ignore_unused_variable_warning(temp);

   std::vector<AnalogData> data_sample;

   unsigned int num_analog_values;
   float value;
   vpr::Uint64 time_stamp;
   AnalogData temp_analog_data;
   reader->beginAttribute(tokens::SampleBufferLenAttrib);
      const unsigned int num_vectors = reader->readUint16();
   reader->endAttribute();

   mAnalogSamples.lock();
   for (unsigned int i = 0; i < num_vectors; ++i)
   {
      reader->beginTag(tokens::BufferSampleTag);
      reader->beginAttribute(tokens::BufferSampleLenAttrib);
         num_analog_values = reader->readUint16();
      reader->endAttribute();

      data_sample.clear();

      for (unsigned int j = 0; j < num_analog_values; ++j)
      {
         reader->beginTag(tokens::AnalogValue);
         reader->beginAttribute(tokens::TimeStamp);
            time_stamp = reader->readUint64(); // Write Time Stamp vpr::Uint64
         reader->endAttribute();
         value = reader->readFloat();       // Write Analog Data(int)
         reader->endTag();

         temp_analog_data.setValue(value);
         temp_analog_data.setTime(vpr::Interval(time_stamp + delta,
                                                vpr::Interval::Usec));
         data_sample.push_back(temp_analog_data);
      }

      // Emit the dataAdded signal.
      mDataAdded(data_sample);

      mAnalogSamples.addSample(data_sample);
      reader->endTag();
   }
   mAnalogSamples.unlock();
   swapAnalogBuffers();

   reader->endTag();
}

bool Analog::config(jccl::ConfigElementPtr element)
{
   mMin = element->getProperty<float>("min");
   mMax = element->getProperty<float>("max");

   vprDEBUG(vprDBG_ALL, vprDBG_VERB_LVL)
      << "[gadget::Analog::config()] min:" << mMin
      << " max:" << mMax << "\n" << vprDEBUG_FLUSH;

   return true;
}

// XXX: Add a "sample" filter that does the normalization in here instead
// of in the driver.
AnalogData Analog::getAnalogData(int devNum)
{
   SampleBuffer_t::buffer_t& stable_buffer = mAnalogSamples.stableBuffer();

   if ( (!stable_buffer.empty()) &&
        (stable_buffer.back().size() > (unsigned)devNum) )  // If Have entry && devNum in range
   {
      return stable_buffer.back()[devNum];
   }
   else        // No data or request out of range, return default value
   {
      if ( stable_buffer.empty() )
      {
         vprDEBUG(vprDBG_ALL, vprDBG_WARNING_LVL)
            << "WARNING: [gadget::Analog::getAnalogData()] "
            << "Stable buffer is empty.  If this is not the first read, "
            << "then this is a problem.\n" << vprDEBUG_FLUSH;
      }
      else
      {
         vprDEBUG(vprDBG_ALL, vprDBG_CONFIG_LVL)
            << "WARNING: [gadget::Analog::getAnalogData()] "
            << "Requested devNum (" << devNum << ") is not within the "
            << "available range.  This is probably a configuration error.\n"
            << vprDEBUG_FLUSH;
      }
      return mDefaultValue;
   }
}

void Analog::addAnalogSample(const std::vector<AnalogData>& anaSample)
{
   // Emit the data added signal for the given sample.
   mDataAdded(anaSample);

   // Locks and then swaps the indices.
   mAnalogSamples.lock();
   mAnalogSamples.addSample(anaSample);
   mAnalogSamples.unlock();
}

void Analog::swapAnalogBuffers()
{
   mAnalogSamples.swapBuffers();
}

const Analog::SampleBuffer_t::buffer_t& Analog::getAnalogDataBuffer()
{
   return mAnalogSamples.stableBuffer();
}

// Given a value that will range from [min() <= n <= max()].
// This returns a value that is normalized to [0,1]
// if n < mMin or n > mMax, then result = mMin or mMax respectively.
float Analog::normalize(const float rawData) const
{
   // First, clamp the raw data so that min <= value <= max.
   const float min_value(getMin());
   const float max_value(getMax());
   const float value(gmtl::Math::clamp(rawData, min_value, max_value));

   // Slide everything to 0.0 (subtract all by min_value), and then divide
   // by max to get normalized value.
   const float tmax(max_value - min_value);
   const float tvalue(value - min_value);

   // since [tmin/tmax...tmax/tmax] == [0.0f...1.0f], the normalized value
   // will be value/tmax
   return tvalue / tmax;
}

void Analog::setMin(const float minValue)
{
   mMin = minValue;
}

void Analog::setMax(const float maxValue)
{
   mMax = maxValue;
}

} // End of gadget namespace
