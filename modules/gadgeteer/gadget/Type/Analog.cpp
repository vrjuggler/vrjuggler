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

#include <gadget/gadgetConfig.h>

#include <boost/concept_check.hpp>
#include <vpr/IO/ObjectWriter.h>
#include <vpr/IO/ObjectReader.h>
#include <vpr/Util/Debug.h>
#include <gadget/Type/Analog.h>
#include <gadget/Util/DeviceSerializationTokens.h>


namespace gadget
{

Analog::Analog()
   : mMin(0.0f)
   , mMax(0.0f)
{
   /* Do nothing. */ ;
}

Analog::~Analog() throw ()
{
   /* Do nothing. */ ;
}

void Analog::writeObject(vpr::ObjectWriter* writer) throw (vpr::IOException)
{
   //std::cout << "[Remote Input Manager] In Analog write" << std::endl;

   SampleBuffer_t::buffer_t& stable_buffer = mAnalogSamples.stableBuffer();
   writer->beginTag(Analog::getInputTypeName());
   writer->beginAttribute(gadget::tokens::DataTypeAttrib);
      writer->writeUint16(MSG_DATA_ANALOG);                                   // Write out the data type so that we can assert if reading in wrong place
   writer->endAttribute();

   if ( !stable_buffer.empty() )
   {
      mAnalogSamples.lock();
      writer->beginAttribute(gadget::tokens::SampleBufferLenAttrib);
         writer->writeUint16(stable_buffer.size());                           // Write the # of vectors in the stable buffer
      writer->endAttribute();
      for ( unsigned j = 0; j < stable_buffer.size(); ++j )                   // For each vector in the stable buffer
      {
         writer->beginTag(gadget::tokens::BufferSampleTag);
         writer->beginAttribute(gadget::tokens::BufferSampleLenAttrib);
            writer->writeUint16(stable_buffer[j].size());                     // Write the # of AnalogDatas in the vector
         writer->endAttribute();
         for ( unsigned i = 0;i < stable_buffer[j].size(); ++i )              // For each AnalogData in the vector
         {
            writer->beginTag(gadget::tokens::AnalogValue);
            writer->beginAttribute(gadget::tokens::TimeStamp);
               writer->writeUint64(stable_buffer[j][i].getTime().usec());        // Write Time Stamp vpr::Uint64
            writer->endAttribute();
            writer->writeFloat(stable_buffer[j][i].getAnalog());              // Write Analog Data(int)
            writer->endTag();
         }
         writer->endTag();
      }
      mAnalogSamples.unlock();
   }
   else        // No data or request out of range, return default value
   {
      writer->beginTag(gadget::tokens::BufferSampleTag);
         writer->beginAttribute(gadget::tokens::BufferSampleLenAttrib);
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

void Analog::readObject(vpr::ObjectReader* reader) throw (vpr::IOException)
{
   vprASSERT(reader->attribExists("rim.timestamp.delta"));
   vpr::Uint64 delta = reader->getAttrib<vpr::Uint64>("rim.timestamp.delta");

   reader->beginTag(Analog::getInputTypeName());
   reader->beginAttribute(gadget::tokens::DataTypeAttrib);
      vpr::Uint16 temp = reader->readUint16();
   reader->endAttribute();

   // ASSERT if this data is really not Analog Data
   // XXX: Should there be error checking for the case when vprASSERT is
   // compied out?  -PH 8/21/2003
   vprASSERT(temp==MSG_DATA_ANALOG && "[Remote Input Manager] Not Analog Data");
   boost::ignore_unused_variable_warning(temp);

   std::vector<AnalogData> dataSample;

   unsigned numAnalogDatas;
   float value;
   vpr::Uint64 timeStamp;
   AnalogData temp_analog_data;
   reader->beginAttribute(gadget::tokens::SampleBufferLenAttrib);
      unsigned numVectors = reader->readUint16();
   reader->endAttribute();

   mAnalogSamples.lock();
   for ( unsigned i = 0; i < numVectors; ++i )
   {
      reader->beginTag(gadget::tokens::BufferSampleTag);
      reader->beginAttribute(gadget::tokens::BufferSampleLenAttrib);
         numAnalogDatas = reader->readUint16();
      reader->endAttribute();

      dataSample.clear();

      for ( unsigned j = 0; j < numAnalogDatas; ++j )
      {
         reader->beginTag(gadget::tokens::AnalogValue);
         reader->beginAttribute(gadget::tokens::TimeStamp);
            timeStamp = reader->readUint64();                  // Write Time Stamp vpr::Uint64
         reader->endAttribute();
         value = reader->readFloat();                       // Write Analog Data(int)
         reader->endTag();

         temp_analog_data.setAnalog(value);
         temp_analog_data.setTime(vpr::Interval(timeStamp + delta,vpr::Interval::Usec));
         dataSample.push_back(temp_analog_data);
      }

      mAnalogSamples.addSample(dataSample);
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

std::string Analog::getInputTypeName()
{
   return std::string("Analog");
}

// Given a value that will range from [min() <= n <= max()].
// This returns a value that is normalized to [0,1]
// if n < mMin or n > mMax, then result = mMin or mMax respectively.
void Analog::normalizeMinToMax(const float& plainJaneValue,
                               float& normedFromMinToMax)
{
   float value = plainJaneValue;

   // first clamp the value so that min<=value<=max
   if ( value < mMin ) value = mMin;
   if ( value > mMax ) value = mMax;

   // slide everything to 0.0 (subtract all by mMin)
   // Then divide by max to get normalized value
   float tmax( mMax - mMin),
         tvalue(value - mMin);

   // since [tmin/tmax...tmax/tmax] == [0.0f...1.0f], the normalized value will be value/tmax
   normedFromMinToMax = tvalue / tmax;
}

float Analog::getMin() const
{
   return mMin;
}

float Analog::getMax() const
{
   return mMax;
}

void Analog::setMin(float mIn)
{
   mMin = mIn;
}

void Analog::setMax(float mAx)
{
   mMax = mAx;
}

} // End of gadget namespace
