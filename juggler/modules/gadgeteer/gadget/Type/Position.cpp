/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2002 by Iowa State University
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
#include <float.h>
#include <vpr/Util/Debug.h>
#include <jccl/Config/ConfigChunk.h>
#include <gadget/Type/Position.h>

#include <gmtl/Matrix.h>
#include <gmtl/Vec.h>
#include <gmtl/MatrixOps.h>
#include <gmtl/Generate.h>

#include <gadget/Filter/Position/PositionFilter.h>
#include <gadget/Filter/Position/PositionFilterFactory.h>


namespace gadget
{

Position::Position()
{;}

Position::~Position()
{;}


// Set up the transformation information
bool Position::config(jccl::ConfigChunkPtr c)
{
   // --- Configure filters --- //
   unsigned num_filters = c->getNum("position_filters");

   vprDEBUG( vprDBG_ALL, 0) << "Num filters: " << num_filters << std::endl << vprDEBUG_FLUSH;

   jccl::ConfigChunkPtr cur_filter;
   PositionFilter* new_filter = NULL;

   for(unsigned i=0;i<num_filters;++i)
   {
      cur_filter = c->getProperty<jccl::ConfigChunkPtr>("position_filters",i);
      vprASSERT(cur_filter.get() != NULL);

      std::string filter_chunk_desc = cur_filter->getDescToken();
      vprDEBUG( vprDBG_ALL, 0) << "   Filter [" << i << "]: Type:" << filter_chunk_desc << std::endl << vprDEBUG_FLUSH;

      new_filter = PositionFilterFactory::instance()->createObject(filter_chunk_desc);
      if(new_filter != NULL)
      {
         new_filter->config(cur_filter);
         mPositionFilters.push_back(new_filter);
      }
      else
      {
         vprDEBUG( vprDBG_ALL, 0) << "   NULL Filter!!!" << std::endl << vprDEBUG_FLUSH;
      }
   }

   return true;
}

vpr::ReturnStatus Position::writeObject(vpr::ObjectWriter* writer)
{
   //std::cout << "[Remote Input Manager] In Position write" << std::endl;
   static long iteration=0;

   SampleBuffer_t::buffer_t& stable_buffer = mPosSamples.stableBuffer();
   writer->writeUint16(MSG_DATA_POS);                               // Write out the data type so that we can assert if reading in wrong place

   if ( !stable_buffer.empty() )
   {
      mPosSamples.lock();
      writer->writeUint16(stable_buffer.size());
      for ( unsigned j=0;j<stable_buffer.size();j++ )
      {
         writer->writeUint16(stable_buffer[j].size());
         //std::cout << std::endl;
         for ( unsigned i=0;i<stable_buffer[j].size();i++ )
         {
            //RIP MATRIX
            gmtl::Matrix44f* PosMatrix = stable_buffer[j][i].getPosition();
            //float pos_data[16];
            const float* pos_data = PosMatrix->getData();
            //std::cout << "WRITE " << "Iteration: " << iteration << std::endl;
            iteration++;
            //std::cout << "WRITE " << pos_data[0] << pos_data[1] << pos_data[4] << pos_data[5] << std::endl;
            //std::cout << "WRITE " << pos_data[4] << pos_data[5] << pos_data[6] << pos_data[7] << std::endl;
            //std::cout << "WRITE " << pos_data[8] << pos_data[9] << pos_data[10] << pos_data[11] << std::endl;
            //std::cout << "WRITE " << pos_data[12] << pos_data[13] << pos_data[14] << pos_data[15] << std::endl;

            //NOTE: This uses the value 16 because we are using a 4x4 matrix
            //for the position dataAt this point there is not a good
            //way to get the row and column value of any given size matrix
            for ( int n=0;n<16;n++ )
            {
               writer->writeFloat(pos_data[n]);
            }
            writer->writeUint64(stable_buffer[j][i].getTime().usec());           // Write Time Stamp vpr::Uint64
            //std::cout << "j: " << j << "i: " << i << std::endl;
            //std::cout << "WRITE " << "TimeStamp: " << stable_buffer[j][i].getTime().usec()  << std::endl;
         }
         //std::cout << std::endl;
      }
      mPosSamples.unlock();
   }
   else        // No data or request out of range, return default value
   {
      if ( stable_buffer.empty() )
      {
         vprDEBUG(vprDBG_ALL, vprDBG_WARNING_LVL) << "Warning: Position::writeObject: Stable buffer is empty. If this is not the first write, then this is a problem.\n" << vprDEBUG_FLUSH;
      }
      writer->writeUint16(0);
   }

   return vpr::ReturnStatus::Succeed;
}


vpr::ReturnStatus Position::readObject(vpr::ObjectReader* reader, vpr::Uint64* delta)
{
   //std::cout << "[Remote Input Manager] In Position Read" << std::endl;
   //SampleBuffer_t::buffer_t& stable_buffer = mAnalogSamples.stableBuffer();
   static long iteration=0;

   vpr::Uint16 temp = reader->readUint16();
   vprASSERT(temp==MSG_DATA_POS && "[Remote Input Manager]Not Positional Data");
   std::vector<PositionData> dataSample;

   unsigned numPosDatas;
   vpr::Uint64 timeStamp;
   PositionData temp_pos_data;
   float pos_data[16];
   gmtl::Matrix44f PosMatrix;

   unsigned numVectors = reader->readUint16();
   mPosSamples.lock();
   for ( unsigned i=0;i<numVectors;i++ )
   {
      numPosDatas = reader->readUint16();
      dataSample.clear();
      //std::cout << std::endl;
      for ( unsigned j=0;j<numPosDatas;j++ )
      {
         //NOTE: This uses the value 16 because we are using a 4x4 matrix
         //for the position dataAt this point there is not a good
         //way to get the row and column value of any given size matrix
         for ( unsigned n=0;n<16;n++ )
         {
            pos_data[n] = reader->readFloat();
         }
         //PosMatrix.set(pos_data[0], pos_data[1], pos_data[2], pos_data[3],
         //              pos_data[4], pos_data[5], pos_data[6], pos_data[7],
         //              pos_data[8], pos_data[9], pos_data[10], pos_data[11],
         //              pos_data[12], pos_data[13], pos_data[14], pos_data[15]);
         PosMatrix.set(pos_data);
         //std::cout << "READ " << "Iteration: " << iteration << std::endl;
         iteration++;
         //std::cout << "READ " << pos_data[0] << pos_data[1] << pos_data[4] << pos_data[5] << std::endl;
         //std::cout << "READ " << pos_data[4] << pos_data[5] << pos_data[6] << pos_data[7] << std::endl;
         //std::cout << "READ " << pos_data[8] << pos_data[9] << pos_data[10] << pos_data[11] << std::endl;
         //std::cout << "READ " << pos_data[12] << pos_data[13] << pos_data[14] << pos_data[15] << std::endl;

         timeStamp = reader->readUint64();
         //std::cout << "READ " << "TimeStamp: " << timeStamp  << std::endl;
         //std::cout << "READ " << "    Delta: " << *delta  << std::endl;
         //std::cout << "READ " << "    After:" << (timeStamp + *delta) << std::endl;

         temp_pos_data.setPosition(PosMatrix);
         temp_pos_data.setTime(vpr::Interval(timeStamp + *delta,vpr::Interval::Usec));
         //RIP MATRIX
         //gmtl::Matrix44f* TestMatrix = temp_pos_data.getPosition();
         //const float* tpos_data = TestMatrix->getData();
         //std::cout << tpos_data[0] << tpos_data[1] << tpos_data[4] << tpos_data[5] << std::endl;
         //std::cout << tpos_data[4] << tpos_data[5] << tpos_data[6] << tpos_data[7] << std::endl;
         //std::cout << tpos_data[8] << tpos_data[9] << tpos_data[10] << tpos_data[11] << std::endl;
         //std::cout << tpos_data[12] << tpos_data[13] << tpos_data[14] << tpos_data[15] << std::endl;

         dataSample.push_back(temp_pos_data);
      }
      //std::cout << std::endl;
      mPosSamples.addSample(dataSample);
   }
   mPosSamples.unlock();
   swapPositionBuffers();

   return(vpr::ReturnStatus::Succeed);
}

void Position::addPositionSample(std::vector< PositionData > posSample)
{
   // Apply all the positional filters
   for(std::vector<PositionFilter*>::iterator i=mPositionFilters.begin(); i != mPositionFilters.end(); ++i)
   {
      (*i)->apply(posSample);
   }
   
   // Locks and then swaps the indices.
   mPosSamples.lock();
   mPosSamples.addSample(posSample);
   mPosSamples.unlock();
}


} // End of gadget namespace

