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

#include <vpr/Util/Debug.h>
#include <vpr/IO/ObjectWriter.h>
#include <vpr/IO/ObjectReader.h>
#include <gadget/Util/DeviceSerializationTokens.h>

#include <gadget/Type/Digital.h>


namespace gadget
{

const std::string Digital::sTypeName("Digital");

Digital::Digital()
{;}

Digital::~Digital()
{;}

DigitalPtr Digital::create()
{
   return DigitalPtr(new Digital());
}

const DigitalData Digital::getDigitalData(int devNum)
{
   SampleBuffer_t::buffer_t& stable_buffer = mDigitalSamples.stableBuffer();

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
            << "WARNING: [gadget::Digital::getDigitalData()] "
            << "Stable buffer is empty.  If this is not the first "
            << "read, then this is a problem.\n" << vprDEBUG_FLUSH;
      }
      else
      {
         vprDEBUG(vprDBG_ALL, vprDBG_CONFIG_LVL)
            << "WARNING: [gadget::Digital::getDigitalData()] "
            << "Requested devNum (" << devNum
            << ") is not in the range available.  "
            << "This is probably a configuration error.\n"
            << vprDEBUG_FLUSH;
      }
      return mDefaultValue;
   }
}

void Digital::addDigitalSample(const std::vector<DigitalData>& digSample)
{
   // Emit the data added signal for the given sample.
   mDataAdded(digSample);

   // Locks and then swaps the indices.
   mDigitalSamples.lock();
   mDigitalSamples.addSample(digSample);
   mDigitalSamples.unlock();
}

void Digital::writeObject(vpr::ObjectWriter* writer)
{
   writer->beginTag(sTypeName);
   //std::cout << "[Remote Input Manager] In Digital write" << std::endl;
   SampleBuffer_t::buffer_t& stable_buffer = mDigitalSamples.stableBuffer();
   writer->beginAttribute(tokens::DataTypeAttrib);
      writer->writeUint16(MSG_DATA_DIGITAL);                               // Write out the data type so that we can assert if reading in wrong place
   writer->endAttribute();

   writer->beginAttribute(tokens::SampleBufferLenAttrib);
      writer->writeUint16(stable_buffer.size());                           // Write the # of vectors in the stable buffer
   writer->endAttribute();

   if (! stable_buffer.empty())
   {
      mDigitalSamples.lock();
      for (unsigned int j = 0; j < stable_buffer.size(); ++j)                               // For each vector in the stable buffer
      {
         writer->beginTag(tokens::BufferSampleTag);
         writer->beginAttribute(tokens::BufferSampleLenAttrib);
            writer->writeUint16(stable_buffer[j].size());                           // Write the # of DigitalDatas in the vector
         writer->endAttribute();
         for (unsigned int i = 0; i < stable_buffer[j].size(); ++i)                         // For each DigitalData in the vector
         {
            writer->beginTag(tokens::DigitalValue);
            writer->beginAttribute(tokens::TimeStamp);
               writer->writeUint64(stable_buffer[j][i].getTime().usec());           // Write Time Stamp vpr::Uint64
            writer->endAttribute();
            writer->writeUint32((vpr::Uint32)stable_buffer[j][i].getValue());  // Write Digital Data(int)
            writer->endTag();
         }
         writer->endTag();
      }
      mDigitalSamples.unlock();
   }
   writer->endTag();
}

void Digital::readObject(vpr::ObjectReader* reader)
{
      //std::cout << "[Remote Input Manager] In Digital read" << std::endl;
   vprASSERT(reader->attribExists("rim.timestamp.delta"));
   vpr::Uint64 delta = reader->getAttrib<vpr::Uint64>("rim.timestamp.delta");

      // ASSERT if this data is really not Digital Data
   reader->beginTag(sTypeName);
   reader->beginAttribute(tokens::DataTypeAttrib);
      vpr::Uint16 temp = reader->readUint16();
   reader->endAttribute();

   // XXX: Should there be error checking for the case when vprASSERT()
   // is compiled out?  -PH 8/21/2003
   vprASSERT(temp==MSG_DATA_DIGITAL && "[Remote Input Manager]Not Digital Data");
   boost::ignore_unused_variable_warning(temp);

   std::vector<DigitalData> data_sample;

   unsigned int num_digital_values;
   vpr::Uint32 value;
   vpr::Uint64 time_stamp;
   DigitalData temp_digital_data;

   reader->beginAttribute(tokens::SampleBufferLenAttrib);
      const unsigned int num_vectors(reader->readUint16());
   reader->endAttribute();

   //std::cout << "Stable Digital Buffer Size: "  << num_vectors << std::endl;
   mDigitalSamples.lock();
   for (unsigned int i = 0; i < num_vectors; ++i)
   {
      reader->beginTag(tokens::BufferSampleTag);
      reader->beginAttribute(tokens::BufferSampleLenAttrib);
         num_digital_values = reader->readUint16();
      reader->endAttribute();

      data_sample.clear();
      for (unsigned int j = 0; j < num_digital_values; ++j)
      {
         reader->beginTag(tokens::DigitalValue);
         reader->beginAttribute(tokens::TimeStamp);
            time_stamp = reader->readUint64();   // read Time Stamp vpr::Uint64
         reader->endAttribute();
         value = reader->readUint32();           // read Digital Data(int)
         reader->endTag();

         temp_digital_data.setValue(static_cast<DigitalState::State>(value));
         temp_digital_data.setTime(vpr::Interval(time_stamp + delta,
                                   vpr::Interval::Usec));
         data_sample.push_back(temp_digital_data);
      }

      // Emit the dataAdded signal.
      mDataAdded(data_sample);

      mDigitalSamples.addSample(data_sample);
      reader->endTag();
   }
   mDigitalSamples.unlock();
   mDigitalSamples.swapBuffers();

   reader->endTag();
}

} // End of gadget namespace
