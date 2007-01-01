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

#include <vpr/IO/ObjectReader.h>
#include <vpr/IO/ObjectWriter.h>

#include <boost/concept_check.hpp>
#include <gmtl/Matrix.h>
#include <gmtl/Vec.h>
#include <gmtl/VecOps.h>
#include <gmtl/MatrixOps.h>
#include <gmtl/Generate.h>
#include <gmtl/Xforms.h>
#include <gmtl/EulerAngle.h>

#include <jccl/Config/ConfigElement.h>
#include <gadget/Type/Glove.h>
#include <gadget/Util/DeviceSerializationTokens.h>

namespace gadget
{

Glove::Glove()
{
   //Initialize the transforms for the default GloveData
   mDefaultValue.calcXforms();

   //By default there are 2 gloves
   mGlovePositions.resize(2);
}

bool Glove::config(jccl::ConfigElementPtr e)
{
   // For now, assume just 2 gloves (left & right)
   std::vector<std::string> positionProxyNames(2);
   positionProxyNames[0]=e->getProperty<std::string>("left_glove_position");
   positionProxyNames[1]=e->getProperty<std::string>("right_glove_position");

   mGlovePositions.resize(positionProxyNames.size());
   unsigned int i;
   for(i=0;i<positionProxyNames.size();i++)
   {
      if(positionProxyNames[i]!="")
      {
         mGlovePositions[i].init(positionProxyNames[i]);
      }
   }

   return true;
}

void Glove::writeObject(vpr::ObjectWriter* writer)
{
   SampleBuffer_t::buffer_t& stable_buffer = mGloveSamples.stableBuffer();

   writer->beginTag(Glove::getInputTypeName());
   writer->beginAttribute(gadget::tokens::DataTypeAttrib);
      writer->writeUint16(MSG_DATA_GLOVE);                               // Write out the data type so that we can assert if reading in wrong place
   writer->endAttribute();

   writer->beginAttribute(gadget::tokens::SampleBufferLenAttrib);
      writer->writeUint16(stable_buffer.size());         // Write the size of the stable buffer
   writer->endAttribute();

   if ( !stable_buffer.empty() )
   {
      mGloveSamples.lock();
      for ( unsigned j=0;j<stable_buffer.size();j++ )
      {
         writer->beginTag(gadget::tokens::BufferSampleTag);
         writer->beginAttribute(gadget::tokens::BufferSampleLenAttrib);
            writer->writeUint16(stable_buffer[j].size());            // Number of glove values for this entry
         writer->endAttribute();

         for ( unsigned i=0;i<stable_buffer[j].size();i++ )       // For each glove value
         {
            writer->beginTag(gadget::tokens::GloveValue);

            // TODO: If we switch the GloveData to only work with Matrix4x4s, then change this.
            for(unsigned component=0;component<GloveData::NUM_COMPONENTS;component++)
            {
               for(unsigned joint=0;joint<GloveData::NUM_JOINTS;joint++)
               {
                  float value=stable_buffer[j][i].mAngles[GloveData::NUM_COMPONENTS][GloveData::NUM_JOINTS];
                  writer->writeFloat(value);
               }
            }

            writer->beginAttribute(gadget::tokens::TimeStamp);
               writer->writeUint64(stable_buffer[j][i].getTime().usec());           // Write Time Stamp vpr::Uint64
            writer->endAttribute();
            writer->endTag();
         }
         writer->endTag();
      }
      mGloveSamples.unlock();
   }
   else       // No data or request out of range, return default value
   {
      vprDEBUG(vprDBG_ALL, vprDBG_WARNING_LVL) << "Warning: Glove::writeObject: Stable buffer is empty. If this is not the first write, then this is a problem.\n" << vprDEBUG_FLUSH;
   }
   writer->endTag();
}

void Glove::readObject(vpr::ObjectReader* reader)
{
   vprASSERT(reader->attribExists("rim.timestamp.delta"));
   vpr::Uint64 delta = reader->getAttrib<vpr::Uint64>("rim.timestamp.delta");

   reader->beginTag(Glove::getInputTypeName());
   reader->beginAttribute(gadget::tokens::DataTypeAttrib);
      vpr::Uint16 temp = reader->readUint16();
   reader->endAttribute();

   // XXX: Should there be error checking for the case when vprASSERT() is
   // compiled out?  -PH 8/21/2003
   vprASSERT(temp==MSG_DATA_GLOVE && "[Remote Input Manager]Not Glove Data");
   boost::ignore_unused_variable_warning(temp);

   std::vector<GloveData> dataSample;

   unsigned int numGloveDatas;
   vpr::Uint64 timeStamp;
   GloveData gloveData;

   reader->beginAttribute(gadget::tokens::SampleBufferLenAttrib);
      unsigned int numVectors = reader->readUint16();
   reader->endAttribute();

   mGloveSamples.lock();
   for ( unsigned int i=0;i<numVectors;i++ )
   {
      reader->beginTag(gadget::tokens::BufferSampleTag);
      reader->beginAttribute(gadget::tokens::BufferSampleLenAttrib);
         numGloveDatas = reader->readUint16();
      reader->endAttribute();

      dataSample.clear();

      for ( unsigned int j=0;j<numGloveDatas;j++ )
      {
         reader->beginTag(gadget::tokens::GloveValue);
         for(unsigned int component=0;component<GloveData::NUM_COMPONENTS;component++)
         {
            for(unsigned int joint=0;joint<GloveData::NUM_JOINTS;joint++)
            {
               gloveData.mAngles[component][joint]=reader->readFloat();
            }
         }

         reader->beginAttribute(gadget::tokens::TimeStamp);
            timeStamp = reader->readUint64();
         reader->endAttribute();
         reader->endTag();

         gloveData.setTime(vpr::Interval(timeStamp + delta,vpr::Interval::Usec));
         gloveData.calcXforms();
         dataSample.push_back(gloveData);
      }

      mGloveSamples.addSample(dataSample);
      reader->endTag();
   }

   mGloveSamples.unlock();
   swapGloveBuffers();
   reader->endTag();
}

/**
 * Returns a vector ponting "out" of the component.
 * Can be used for selection, etc.
 * Use getJointTransform to get the transformation matrix.
 */
gmtl::Vec3f Glove::getTipVector(GloveData::GloveComponent component, int devNum)
{
   gmtl::Vec3f y_axis(0.0f, 1.0f, 0.0f);
   gmtl::Vec3f ret_val(0.0f, 0.0f, 0.0f);

   ret_val = getTipTransform(component, devNum) * y_axis;   // Compute the vector direction
   return ret_val;
}

/**
 * Returns the transform matrix of the specified finger tip in world space
 * wTt = wTb bTj jTt
 */
gmtl::Matrix44f Glove::getTipTransform(GloveData::GloveComponent component,int devNum)
{
   gmtl::Matrix44f worldTdij;

   worldTdij=getJointTransform(component,GloveData::DIJ,devNum);

   // TODO: Fix this up
   gmtl::Matrix44f dijTtip;
   gmtl::setTrans(dijTtip,gmtl::Vec3f(0,0.5f / PositionUnitConversion::ConvertToInches,0));
   gmtl::postMult(worldTdij,dijTtip);

   return worldTdij;
}

/**
 * Returns the transform matrix of the specified joint in world space
 * wTj = wTb bTj
 */
gmtl::Matrix44f Glove::getJointTransform(GloveData::GloveComponent component, GloveData::GloveJoint joint,int devNum)
{
   gmtl::Matrix44f result;           // The returned matrix.
   gmtl::Matrix44f baseTdij;         // Transform from base to dig coord system

   const GloveData data=getGloveData(devNum);

   if(component==GloveData::WRIST)
   {
      gmtl::identity(baseTdij);      // No transform
   }
   else
   {
      baseTdij = data.getLocalTransformMatrix(component,GloveData::MPJ);

      if(joint>=GloveData::PIJ)
      {
         gmtl::postMult(baseTdij,data.getLocalTransformMatrix(component,GloveData::PIJ));       // mpjTpij

         if(joint>=GloveData::DIJ)
         {
            gmtl::postMult(baseTdij,data.getLocalTransformMatrix(component,GloveData::DIJ));    // pijTdij
         }
      }
   }

   // Compute return value: result = TIPw = wTb bTd dTt
   if(devNum<(int)mGlovePositions.size()){
      result=mGlovePositions[devNum]->getData();      // wTb
   }
   gmtl::postMult(result,baseTdij);                     // bTd

   return result;
}

GloveData Glove::getGloveData(int devNum = 0)
{
   SampleBuffer_t::buffer_t& stable_buffer = mGloveSamples.stableBuffer();

   if ( (!stable_buffer.empty()) && (stable_buffer.back().size() > (unsigned)devNum) )  // If have entry && devNum in range
   {
      return stable_buffer.back()[devNum];
   }
   else        // No data or request out of range, return default value
   {
      if ( stable_buffer.empty() )
      {
         vprDEBUG(vprDBG_ALL, vprDBG_WARNING_LVL) << "Warning: Digital::getGloveData: Stable buffer is empty. If this is not the first read, then this is a problem.\n" << vprDEBUG_FLUSH;
      }
      else
      {
         vprDEBUG(vprDBG_ALL, vprDBG_CONFIG_LVL) << "Warning: Digital::getGloveData: Requested devNum " << devNum << " is not in the range available.  May have configuration error\n" << vprDEBUG_FLUSH;
      }
      return mDefaultValue;
   }
}

/**
 * Utility function to convert a 10 size vector of DigitalData to GloveData
 */
std::vector<GloveData> Glove::getGloveDataFromDigitalData(const std::vector<DigitalData> &digitalData)
{
   assert(digitalData.size()>=10);

   std::vector<GloveData> gloveData=std::vector<GloveData>(2);

   for(unsigned int i=0;i<2;i++)
   {
      for(unsigned int j=0;j<GloveData::NUM_COMPONENTS-1;j++)
      {
         // Use funky indexing here to access the correct digitalData
         if( digitalData[i*(GloveData::NUM_COMPONENTS-1)+j].getDigital() == 1)
         {
            for(unsigned int k=0;k<GloveData::NUM_JOINTS;k++)
            {
               gloveData[i].mAngles[j][k]=gmtl::Math::PI_OVER_2;
            }
         }
         else
         {
            for(unsigned int k=0;k<GloveData::NUM_JOINTS;k++)
            {
               gloveData[i].mAngles[j][k]=0;
            }
         }
      }
   }

   gloveData[0].calcXforms();
   gloveData[1].calcXforms();

   return gloveData;
}

} // End of gadget namespace
