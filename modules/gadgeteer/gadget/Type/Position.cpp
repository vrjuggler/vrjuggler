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


namespace gadget
{

// Set up the transformation information
bool Position::config(jccl::ConfigChunkPtr c)
{
  //vprDEBUG(vprDBG_ALL,0) << "vjPosition::config(jccl::ConfigChunkPtr)" << vprDEBUG_FLUSH;
  if ((c->getNum("translate") == 3) && (c->getNum("rotate") == 3))
  {
    // These are the transforms from the base tracker coord system

    float xt, yt, zt;
    xt = c->getProperty<float>("translate",0);
    yt = c->getProperty<float>("translate",1);
    zt = c->getProperty<float>("translate",2);

    // These values are specified in human-friendly degrees but must be passed
    // to GMTL as radians.
    float xr, yr, zr;
    xr = gmtl::Math::deg2Rad(c->getProperty<float>("rotate",0));
    yr = gmtl::Math::deg2Rad(c->getProperty<float>("rotate",1));
    zr = gmtl::Math::deg2Rad(c->getProperty<float>("rotate",2));

      // This makes a rotation matrix that moves a pt in
      // the device's coord system to the vj coord system.
      // ==> world_M_transmitter
    gmtl::EulerAngleXYZf euler( xr,yr,zr );
    rotMat = gmtl::makeRot<gmtl::Matrix44f>( euler );

    gmtl::identity(xformMat);
    gmtl::setTrans(xformMat, gmtl::Vec3f(xt, yt, zt) );
    gmtl::postMult(xformMat, rotMat);         // xformMat = T*R
  }

  return true;
}

	vpr::ReturnStatus Position::writeObject(vpr::ObjectWriter* writer)
	{
		//std::cout << "[Remote Input Manager] In Position write" << std::endl;
        
		SampleBuffer_t::buffer_t& stable_buffer = mPosSamples.stableBuffer();
		writer->writeUint16(MSG_DATA_POS);                               // Write out the data type so that we can assert if reading in wrong place
        
        if(!stable_buffer.empty())
		{
           mPosSamples.lock(); 
           writer->writeUint16(stable_buffer.size());
            for(unsigned j=0;j<stable_buffer.size();j++)
            {
               writer->writeUint16(stable_buffer[j].size());
               //std::cout << std::endl;
               for(unsigned i=0;i<stable_buffer[j].size();i++)
			   {
				  //RIP MATRIX
				  gmtl::Matrix44f* PosMatrix = stable_buffer[j][i].getPosition();
				  //float pos_data[16];
                  const float* pos_data = PosMatrix->getData();
                  //std::cout << pos_data[0] << pos_data[1] << pos_data[4] << pos_data[5] << std::endl;
                  //std::cout << pos_data[4] << pos_data[5] << pos_data[6] << pos_data[7] << std::endl;
                  //std::cout << pos_data[8] << pos_data[9] << pos_data[10] << pos_data[11] << std::endl;
                  //std::cout << pos_data[12] << pos_data[13] << pos_data[14] << pos_data[15] << std::endl;

				  //NOTE: This uses the value 16 because we are using a 4x4 matrix 
				  //for the position dataAt this point there is not a good 
				  //way to get the row and column value of any given size matrix
				  for(int n=0;n<16;n++)
				  {
					 writer->writeFloat(pos_data[n]);
				  }
                  writer->writeUint64(stable_buffer[j][i].getTime().usec());           // Write Time Stamp vpr::Uint64
			   } 
               //std::cout << std::endl;
            }
            mPosSamples.unlock();
		}
		else        // No data or request out of range, return default value
		{
			if(stable_buffer.empty())
			{
				vprDEBUG(vprDBG_ALL, vprDBG_WARNING_LVL) << "Warning: Position::writeObject: Stable buffer is empty. If this is not the first write, then this is a problem.\n" << vprDEBUG_FLUSH;
			}
			writer->writeUint16(0);
		}
		
      return vpr::ReturnStatus::Succeed;
	}


	vpr::ReturnStatus Position::readObject(vpr::ObjectReader* reader)
	{
	   //std::cout << "[Remote Input Manager] In Position Read" << std::endl;
       //SampleBuffer_t::buffer_t& stable_buffer = mAnalogSamples.stableBuffer();
       
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
       for(unsigned i=0;i<numVectors;i++)
	   {
		  numPosDatas = reader->readUint16();
          dataSample.clear();
          //std::cout << std::endl;
          for (unsigned j=0;j<numPosDatas;j++)
          {
            //NOTE: This uses the value 16 because we are using a 4x4 matrix 
			//for the position dataAt this point there is not a good 
			//way to get the row and column value of any given size matrix
			for(unsigned n=0;n<16;n++)
			{
				pos_data[n] = reader->readFloat();
			}
            //PosMatrix.set(pos_data[0], pos_data[1], pos_data[2], pos_data[3], 
            //              pos_data[4], pos_data[5], pos_data[6], pos_data[7], 
            //              pos_data[8], pos_data[9], pos_data[10], pos_data[11], 
            //              pos_data[12], pos_data[13], pos_data[14], pos_data[15]);
            PosMatrix.set(pos_data);
            //std::cout << pos_data[0] << pos_data[1] << pos_data[4] << pos_data[5] << std::endl;
            //std::cout << pos_data[4] << pos_data[5] << pos_data[6] << pos_data[7] << std::endl;
            //std::cout << pos_data[8] << pos_data[9] << pos_data[10] << pos_data[11] << std::endl;
            //std::cout << pos_data[12] << pos_data[13] << pos_data[14] << pos_data[15] << std::endl;

            timeStamp = reader->readUint64();
            temp_pos_data.setPosition(PosMatrix);
            temp_pos_data.setTime(vpr::Interval(timeStamp + mDelta,vpr::Interval::Usec));
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
       mPosSamples.swapBuffers();
       return(vpr::ReturnStatus::Succeed);

          
	}                            

Position::Position()
{
  //vprDEBUG(vprDBG_ALL,0) << "vjPosition::Position()" << vprDEBUG_FLUSH;
  gmtl::identity(xformMat);
}

Position::~Position()
{
    ;
}

};
