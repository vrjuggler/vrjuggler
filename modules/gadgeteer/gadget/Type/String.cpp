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

#include <gadget/gadgetConfig.h>

#include <boost/concept_check.hpp>

#include <vpr/IO/ObjectWriter.h>
#include <vpr/IO/ObjectReader.h>
#include <vpr/Util/Debug.h>

#include <gadget/Util/DeviceSerializationTokens.h>
#include <gadget/Type/String.h>

namespace gadget
{

String::String() : mDefaultValue("")
{;}

StringPtr String::create()
{
   return StringPtr(new String());
}

String::~String()
{;}

const StringData& String::getStringData(const int devNum) const
{
   const SampleBuffer_t::buffer_t& stable_buffer =
      mStringSamples.stableBuffer();

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
            << "WARNING: [gadget::String::getStringData()] "
            << "Stable buffer is empty.  If this is not the first "
            << "read, then this is a problem.\n" << vprDEBUG_FLUSH;
      }
      else
      {
         vprDEBUG(vprDBG_ALL, vprDBG_CONFIG_LVL)
            << "WARNING: [gadget::String::getStringData()] "
            << "Requested devNum (" << devNum
            << ") is not in the range available.  "
            << "This is probably a configuration error.\n" << vprDEBUG_FLUSH;
      }
      return mDefaultValue;
   }
}

void String::addStringSample(const std::vector<StringData>& stringSample)
{
   // Emit the data added signal for the given sample.
   mDataAdded(stringSample);

   // Locks and then swaps the indices.
   mStringSamples.lock();
   mStringSamples.addSample(stringSample);
   mStringSamples.unlock();
}

void String::writeObject(vpr::ObjectWriter* writer)
{
   writer->beginTag(String::getInputTypeName());
   SampleBuffer_t::buffer_t& stable_buffer = mStringSamples.stableBuffer();
   writer->beginAttribute(tokens::DataTypeAttrib);
      writer->writeUint16(MSG_DATA_STRING);                               // Write out the data type so that we can assert if reading in wrong place
   writer->endAttribute();

   writer->beginAttribute(tokens::SampleBufferLenAttrib);
      writer->writeUint16(stable_buffer.size());                           // Write the # of vectors in the stable buffer
   writer->endAttribute();

   if ( !stable_buffer.empty() )
   {
      mStringSamples.lock();
      for (unsigned int j = 0; j < stable_buffer.size(); ++j)                               // For each vector in the stable buffer
      {
         writer->beginTag(tokens::BufferSampleTag);
         writer->beginAttribute(tokens::BufferSampleLenAttrib);
            writer->writeUint16(stable_buffer[j].size());                           // Write the # of StringDatas in the vector
         writer->endAttribute();
         for (unsigned int i = 0; i < stable_buffer[j].size(); ++i)                         // For each StringData in the vector
         {
            writer->beginTag(tokens::StringValue);
            writer->beginAttribute(tokens::TimeStamp);
               writer->writeUint64(stable_buffer[j][i].getTime().usec());           // Write Time Stamp vpr::Uint64
            writer->endAttribute();
            writer->writeString(stable_buffer[j][i].getValue());  // Write String Data(std::string)
            writer->endTag();
         }
         writer->endTag();
      }
      mStringSamples.unlock();
   }
   writer->endTag();
}

void String::readObject(vpr::ObjectReader* reader)
{
   vprASSERT(reader->attribExists("rim.timestamp.delta"));
   vpr::Uint64 delta = reader->getAttrib<vpr::Uint64>("rim.timestamp.delta");

      // ASSERT if this data is really not String Data
   reader->beginTag(String::getInputTypeName());
   reader->beginAttribute(tokens::DataTypeAttrib);
      vpr::Uint16 temp = reader->readUint16();
   reader->endAttribute();

   // XXX: Should there be error checking for the case when vprASSERT()
   // is compiled out?  -PH 8/21/2003
   vprASSERT(temp==MSG_DATA_STRING && "[Remote Input Manager]Not String Data");
   boost::ignore_unused_variable_warning(temp);

   std::vector<StringData> data_sapmle;

   unsigned int num_string_values;
   std::string value;
   vpr::Uint64 time_stamp;
   StringData temp_string_data;

   reader->beginAttribute(tokens::SampleBufferLenAttrib);
      const unsigned int num_vectors(reader->readUint16());
   reader->endAttribute();

   mStringSamples.lock();
   for ( unsigned int i = 0; i < num_vectors; ++i)
   {
      reader->beginTag(tokens::BufferSampleTag);
      reader->beginAttribute(tokens::BufferSampleLenAttrib);
         num_string_values = reader->readUint16();
      reader->endAttribute();

      data_sapmle.clear();
      for (unsigned int j = 0; j < num_string_values; ++j)
      {
         reader->beginTag(tokens::StringValue);
         reader->beginAttribute(tokens::TimeStamp);
            time_stamp = reader->readUint64();    // read Time Stamp vpr::Uint64
         reader->endAttribute();
         value = reader->readString();           // read String Data(std::string)
         reader->endTag();

         temp_string_data.setValue(value);
         temp_string_data.setTime(vpr::Interval(time_stamp + delta,
                                  vpr::Interval::Usec));
         data_sapmle.push_back(temp_string_data);
      }
      mStringSamples.addSample(data_sapmle);
      reader->endTag();
   }
   mStringSamples.unlock();
   mStringSamples.swapBuffers();

   reader->endTag();
}

} // End of gadget namespace
