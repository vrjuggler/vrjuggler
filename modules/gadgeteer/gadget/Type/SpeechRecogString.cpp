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

#include <gadget/gadgetConfig.h>
#include <boost/concept_check.hpp>
#include <vpr/IO/ObjectWriter.h>
#include <vpr/IO/ObjectReader.h>
#include <gadget/Type/SpeechRecogString.h>

namespace gadget
{

vpr::ReturnStatus SpeechRecogString::writeObject(vpr::ObjectWriter* writer)
{
   writer->beginTag(SpeechRecogString::getBaseType());
   //std::cout << "[Remote Input Manager] In SpeechRecogString write" << std::endl;
   SampleBuffer_t::buffer_t& stable_buffer = mStringSamples.stableBuffer();
   writer->beginAttribute(gadget::tokens::DataTypeAttrib);
      writer->writeUint16(MSG_DATA_SPEECH_RECOG_STRING);                               // Write out the data type so that we can assert if reading in wrong place
   writer->endAttribute();

   writer->beginAttribute(gadget::tokens::SampleBufferLenAttrib);
      writer->writeUint16(stable_buffer.size());                           // Write the # of vectors in the stable buffer
   writer->endAttribute();

   if ( !stable_buffer.empty() )
   {
      mStringSamples.lock();
      for ( unsigned j=0;j<stable_buffer.size();j++ )                               // For each vector in the stable buffer
      {
         writer->beginTag(gadget::tokens::BufferSampleTag);
         writer->beginAttribute(gadget::tokens::BufferSampleLenAttrib);
            writer->writeUint16(stable_buffer[j].size());                           // Write the # of StringDatas in the vector
         writer->endAttribute();
         for ( unsigned i=0;i<stable_buffer[j].size();i++ )                         // For each StringData in the vector
         {
            writer->beginTag(gadget::tokens::StringValue);
            writer->beginAttribute(gadget::tokens::TimeStamp);
               writer->writeUint64(stable_buffer[j][i].getTime().usec());           // Write Time Stamp vpr::Uint64
            writer->endAttribute();
            writer->writeString(stable_buffer[j][i].getString());  // Write SpeechRecogString Data(std::string)
            writer->endTag();
         }
         writer->endTag();
      }
      mStringSamples.unlock();
   }
   writer->endTag();

   return vpr::ReturnStatus::Succeed;
}

vpr::ReturnStatus SpeechRecogString::readObject(vpr::ObjectReader* reader)
{
      //std::cout << "[Remote Input Manager] In SpeechRecogString read" << std::endl;
   vprASSERT(reader->attribExists("rim.timestamp.delta"));
   vpr::Uint64 delta = reader->getAttrib<vpr::Uint64>("rim.timestamp.delta");

      // ASSERT if this data is really not String Data
   reader->beginTag(SpeechRecogString::getBaseType());
   reader->beginAttribute(gadget::tokens::DataTypeAttrib);
      vpr::Uint16 temp = reader->readUint16();
   reader->endAttribute();

   // XXX: Should there be error checking for the case when vprASSERT()
   // is compiled out?  -PH 8/21/2003
   vprASSERT(temp==MSG_DATA_SPEECH_RECOG_STRING && "[Remote Input Manager]Not SpeechRecogString Data");
   boost::ignore_unused_variable_warning(temp);

   std::vector<StringData> dataSample;

   unsigned numStringDatas;
   std::string value;
   vpr::Uint64 timeStamp;
   StringData temp_string_data;

   reader->beginAttribute(gadget::tokens::SampleBufferLenAttrib);
      unsigned numVectors = reader->readUint16();
   reader->endAttribute();

   //std::cout << "Stable SpeechRecogString Buffer Size: "  << numVectors << std::endl;
   mStringSamples.lock();
   for ( unsigned i=0;i<numVectors;i++ )
   {
      reader->beginTag(gadget::tokens::BufferSampleTag);
      reader->beginAttribute(gadget::tokens::BufferSampleLenAttrib);
         numStringDatas = reader->readUint16();
      reader->endAttribute();

      dataSample.clear();
      for ( unsigned j=0;j<numStringDatas;j++ )
      {
         reader->beginTag(gadget::tokens::StringValue);
         reader->beginAttribute(gadget::tokens::TimeStamp);
            timeStamp = reader->readUint64();    // read Time Stamp vpr::Uint64
         reader->endAttribute();
         value = reader->readString();           // read SpeechRecogString Data(std::string)
         reader->endTag();

         temp_string_data.setString(value);
         temp_string_data.setTime(vpr::Interval(timeStamp + delta,vpr::Interval::Usec));
         dataSample.push_back(temp_string_data);
      }
      mStringSamples.addSample(dataSample);
      reader->endTag();
   }
   mStringSamples.unlock();
   mStringSamples.swapBuffers();

   reader->endTag();

   return vpr::ReturnStatus::Succeed;
}

} // End of gadget namespace
