/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998, 1999, 2000 by Iowa State University
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

#include <vpr/Util/Debug.h>
#include <gadget/Type/Analog.h>


namespace gadget
{

vpr::ReturnStatus Analog::writeObject(vpr::ObjectWriter* writer)
{
   //std::cout << "[Remote Input Manager] In Analog write" << std::endl;

   ////////////////////////////////////////////////////
   SampleBuffer_t::buffer_t& stable_buffer = mAnalogSamples.stableBuffer();
   writer->writeUint16(MSG_DATA_ANALOG);                                           // Write out the data type so that we can assert if reading in wrong place

   if ( !stable_buffer.empty() )
   {
      mAnalogSamples.lock();
      writer->writeUint16(stable_buffer.size());                                          // Write the # of vectors in the stable buffer
      for ( unsigned j=0;j<stable_buffer.size();j++ )                                            // For each vector in the stable buffer
      {
         writer->writeUint16(stable_buffer[j].size());                                   // Write the # of AnalogDatas in the vector
         //std::cout << "Analog Data Size: "  << stable_buffer.back().size() << std::endl;
         //std::cout << "ME: ";
         for ( unsigned i=0;i<stable_buffer[j].size();i++ )                                 // For each AnalogData in the vector
         {
            writer->writeFloat(stable_buffer[j][i].getAnalog());    // Write Analog Data(int)
            //std::cout << stable_buffer[j][i].getAnalog();
            writer->writeUint64(stable_buffer[j][i].getTime().usec());              // Write Time Stamp vpr::Uint64
         }
         //std::cout << std::endl;
      }
      mAnalogSamples.unlock();
   }
   else        // No data or request out of range, return default value
   {
      writer->writeUint16(0);
      vprDEBUG(vprDBG_ALL, vprDBG_WARNING_LVL)
         << "Warning: Analog::writeObject: Stable buffer is empty. If this is not the first write, then this is a problem.\n"
         << vprDEBUG_FLUSH;
   }

   return vpr::ReturnStatus::Succeed;
   ////////////////////////////////////////////////////
}

vpr::ReturnStatus Analog::readObject(vpr::ObjectReader* reader,
                                     vpr::Uint64* delta)
{
   vpr::Uint16 temp = reader->readUint16();
   vprASSERT(temp==MSG_DATA_ANALOG && "[Remote Input Manager]Not Analog Data");                          // ASSERT if this data is really not Analog Data
   std::vector<AnalogData> dataSample;

   unsigned numAnalogDatas;
   float value;
   vpr::Uint64 timeStamp;
   AnalogData temp_analog_data;

   unsigned numVectors = reader->readUint16();
   //std::cout << "Stable Analog Buffer Size: "  << numVectors << std::endl;
   mAnalogSamples.lock();
   for ( unsigned i=0;i<numVectors;i++ )
   {
      numAnalogDatas = reader->readUint16();
      //std::cout << "Analog Data Size: "    << numAnalogDatas << std::endl;
      dataSample.clear();
      //std::cout << "ME: ";
      for ( unsigned j=0;j<numAnalogDatas;j++ )
      {
         value = reader->readFloat();            //Write Analog Data(int)
         //std::cout << value;
         timeStamp = reader->readUint64();                       //Write Time Stamp vpr::Uint64
         temp_analog_data.setAnalog(value);
         temp_analog_data.setTime(vpr::Interval(timeStamp + *delta,vpr::Interval::Usec));
         dataSample.push_back(temp_analog_data);
      }
      //std::cout << std::endl;
      mAnalogSamples.addSample(dataSample);
   }
   mAnalogSamples.unlock();
   swapAnalogBuffers();
   return vpr::ReturnStatus::Succeed;
}

bool Analog::config(jccl::ConfigChunkPtr c)
{
   mMin = c->template getProperty<float>("min");
   mMax = c->template getProperty<float>("max");

   vprDEBUG(vprDBG_ALL,4) << " SimAnalog::config() min:" << mMin
                          << " max:" << mMax << "\n" << vprDEBUG_FLUSH;

   return true;
}

// XXX: Add a "sample" filter that does the normalization in here instead
// of in the driver.
AnalogData Analog::getAnalogData(int devNum = 0)
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
            << "Warning: Analog::getAnalogData: Stable buffer is empty. If this is not the first read, then this is a problem.\n"
            << vprDEBUG_FLUSH;
      }
      else
      {
         vprDEBUG(vprDBG_ALL, vprDBG_CONFIG_LVL)
            << "Warning: Analog::getAnalogData: Requested devNum is not in the range available.  May have configuration error\n"
            << vprDEBUG_FLUSH;
      }
      return mDefaultValue;
   }
}

// Gives a value that will range from [min() <= n <= max()].
// This returns a value that is normalized to the range of mMin <= n <= mMax
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

} // End of gadget namespace
