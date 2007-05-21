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

#include <gadget/gadgetConfig.h>
#include <float.h>
#include <boost/concept_check.hpp>
#include <vpr/Util/Debug.h>
#include <vpr/IO/ObjectReader.h>
#include <vpr/IO/ObjectWriter.h>
#include <jccl/Config/ConfigElement.h>
#include <gadget/Type/Position.h>

#include <gmtl/Matrix.h>
#include <gmtl/Vec.h>
#include <gmtl/MatrixOps.h>
#include <gmtl/Generate.h>

#include <gadget/Filter/Position/PositionFilter.h>
#include <gadget/Filter/Position/PositionFilterFactory.h>

#include <gadget/Util/DeviceSerializationTokens.h>


namespace gadget
{

Position::Position()
{;}

Position::~Position()
{
   for (std::vector<PositionFilter*>::iterator itr = mPositionFilters.begin();
        itr != mPositionFilters.end(); itr++)
   {
      if (NULL != *itr)
      {
         delete *itr;
         *itr = NULL;
      }
   }
   mPositionFilters.clear();
}

PositionPtr Position::create()
{
   return PositionPtr(new Position());
}

// Set up the transformation information
bool Position::config(jccl::ConfigElementPtr e)
{
   // --- Configure filters --- //
   unsigned num_filters = e->getNum("position_filters");

   vprDEBUG_OutputGuard(vprDBG_ALL, vprDBG_VERB_LVL,
                        std::string("Position::config: ") + e->getName() +
                           std::string(":") + e->getID() +
                           std::string("\n"),
                        std::string("Position::config: done.\n") );

   vprDEBUG(vprDBG_ALL, vprDBG_VERB_LVL)
      << "Num filters: " << num_filters << std::endl << vprDEBUG_FLUSH;

   jccl::ConfigElementPtr cur_filter;
   PositionFilter* new_filter = NULL;

   for(unsigned i=0;i<num_filters;++i)
   {
      cur_filter = e->getProperty<jccl::ConfigElementPtr>("position_filters",i);
      vprASSERT(cur_filter.get() != NULL);

      std::string filter_id = cur_filter->getID();
      vprDEBUG( vprDBG_ALL, vprDBG_VERB_LVL)
         << "   Filter [" << i << "]: Type:" << filter_id
         << std::endl << vprDEBUG_FLUSH;

      new_filter = PositionFilterFactory::instance()->createObject(filter_id);
      if(new_filter != NULL)
      {
         if (new_filter->config(cur_filter))
         {
            mPositionFilters.push_back(new_filter);
         }
         else
         {
            vprDEBUG( vprDBG_ERROR, vprDBG_CONFIG_STATUS_LVL )
               << "   Filter [" << i << "]: Type:" << filter_id
               << " configuration failed; " << filter_id << " will NOT "
               << "be loaded.\n"
               << vprDEBUG_FLUSH;

            delete new_filter;
            new_filter = NULL;
         }
      }
      else
      {
         vprDEBUG(vprDBG_ALL, vprDBG_VERB_LVL)
            << "   NULL Filter!!!" << std::endl << vprDEBUG_FLUSH;
      }
   }

   return true;
}

void Position::writeObject(vpr::ObjectWriter* writer)
{
   SampleBuffer_t::buffer_t& stable_buffer = mPosSamples.stableBuffer();

   writer->beginTag(Position::getInputTypeName());
   writer->beginAttribute(gadget::tokens::DataTypeAttrib);
      writer->writeUint16(MSG_DATA_POS);                               // Write out the data type so that we can assert if reading in wrong place
   writer->endAttribute();

   writer->beginAttribute(gadget::tokens::SampleBufferLenAttrib);
      writer->writeUint16(stable_buffer.size());         // Write the size of the stable buffer
   writer->endAttribute();

   if ( !stable_buffer.empty() )
   {
      mPosSamples.lock();
      for ( unsigned j=0;j<stable_buffer.size();j++ )
      {
         writer->beginTag(gadget::tokens::BufferSampleTag);
         writer->beginAttribute(gadget::tokens::BufferSampleLenAttrib);
            writer->writeUint16(stable_buffer[j].size());            // Number of pos values for this entry
         writer->endAttribute();

         for ( unsigned i=0;i<stable_buffer[j].size();i++ )       // For each pos value
         {
            gmtl::Matrix44f* PosMatrix = &(stable_buffer[j][i].mPosData);
            const float* pos_data = PosMatrix->getData();

            writer->beginTag(gadget::tokens::PosValue);
            for ( int n=0;n<16;n++ )
            {  writer->writeFloat(pos_data[n]); }

            writer->beginAttribute(gadget::tokens::TimeStamp);
               writer->writeUint64(stable_buffer[j][i].getTime().usec());           // Write Time Stamp vpr::Uint64
            writer->endAttribute();
            writer->endTag();
         }
         writer->endTag();
      }
      mPosSamples.unlock();
   }
   else       // No data or request out of range, return default value
   {
      vprDEBUG(vprDBG_ALL, vprDBG_WARNING_LVL) << "Warning: Position::writeObject: Stable buffer is empty. If this is not the first write, then this is a problem.\n" << vprDEBUG_FLUSH;
   }
   writer->endTag();
}


void Position::readObject(vpr::ObjectReader* reader)
{
   vprASSERT(reader->attribExists("rim.timestamp.delta"));
   vpr::Uint64 delta = reader->getAttrib<vpr::Uint64>("rim.timestamp.delta");

   reader->beginTag(Position::getInputTypeName());
   reader->beginAttribute(gadget::tokens::DataTypeAttrib);
      vpr::Uint16 temp = reader->readUint16();
   reader->endAttribute();

   // XXX: Should there be error checking for the case when vprASSERT() is
   // compiled out?  -PH 8/21/2003
   vprASSERT(temp==MSG_DATA_POS && "[Remote Input Manager]Not Positional Data");
   boost::ignore_unused_variable_warning(temp);

   std::vector<PositionData> dataSample;

   unsigned numPosDatas;
   vpr::Uint64 timeStamp;
   PositionData temp_pos_data;
   float pos_data[16];
   gmtl::Matrix44f PosMatrix;

   reader->beginAttribute(gadget::tokens::SampleBufferLenAttrib);
      unsigned numVectors = reader->readUint16();
   reader->endAttribute();

   mPosSamples.lock();
   for ( unsigned i=0;i<numVectors;i++ )
   {
      reader->beginTag(gadget::tokens::BufferSampleTag);
      reader->beginAttribute(gadget::tokens::BufferSampleLenAttrib);
         numPosDatas = reader->readUint16();
      reader->endAttribute();

      dataSample.clear();

      for ( unsigned j=0;j<numPosDatas;j++ )
      {
         reader->beginTag(gadget::tokens::PosValue);
         //NOTE: This uses the value 16 because we are using a 4x4 matrix
         //for the position dataAt this point there is not a good
         //way to get the row and column value of any given size matrix
         for ( unsigned n=0;n<16;n++ )
         { pos_data[n] = reader->readFloat(); }
         PosMatrix.set(pos_data);

         reader->beginAttribute(gadget::tokens::TimeStamp);
            timeStamp = reader->readUint64();
         reader->endAttribute();
         reader->endTag();

         temp_pos_data.setPosition(PosMatrix);
         temp_pos_data.setTime(vpr::Interval(timeStamp + delta,vpr::Interval::Usec));
         dataSample.push_back(temp_pos_data);
      }
      mPosSamples.addSample(dataSample);
      reader->endTag();
   }
   mPosSamples.unlock();
   swapPositionBuffers();
   reader->endTag();
}

void Position::addPositionSample(std::vector< PositionData > posSample)
{
   // Apply all the positional filters
   for(std::vector<PositionFilter*>::iterator i = mPositionFilters.begin(); i != mPositionFilters.end(); ++i)
   {
      (*i)->apply(posSample);
   }

   // Locks and then swaps the indices.
   mPosSamples.lock();
   mPosSamples.addSample(posSample);
   mPosSamples.unlock();
}


} // End of gadget namespace

