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

#include <gadget/Type/NetPosition.h>
#include <gadget/Util/Debug.h>

namespace gadget{

// constructor for a transmitting network device
NetPosition::NetPosition(const std::string& src_device_name, Input* input_ptr, VJ_NETID_TYPE local_device_id, VJ_NETID_TYPE rmt_device_id) : NetInput(src_device_name, input_ptr, local_device_id, rmt_device_id, 16 * sizeof(float) ){

   vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_CONFIG_LVL)
      << "NetPosition: local_device_id = " << local_device_id << std::endl
      << vprDEBUG_FLUSH;

   if(src_device_name.length() > 0){   // pointing to another device/proxy for data source
      mLocalSource.init(mDeviceName);
      mSendBuffer.resize(3 + DATA_TIME_SIZE);  // 2 bytes code/id, 16 bytes float data, 4 bytes timestamp, 1 byte semicolon
      vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_HEX_LVL)
         << "NetPosition: SendBuffer size: " << (3 + DATA_TIME_SIZE)
         << std::endl << vprDEBUG_FLUSH;
      mNetworkMatrices.resize(1);  // space for one data item
   }
   else{                              // use ourself as source of data
      // find the number of devices

      int num_elements = this->getPositionDataBuffer().size();
      if (num_elements <= 0)
         num_elements = 1;

      mNetworkMatrices.resize(num_elements);

      // allocate space for data
      mSendBuffer.resize(3 + DATA_TIME_SIZE * num_elements);  // 2 bytes of code/id, 16*4*n bytes of data, 1 byte for semicolon
      vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_HEX_LVL)
         << "NetPosition: SendBuffer size: "
         << (3 + DATA_TIME_SIZE * num_elements) << std::endl << vprDEBUG_FLUSH;
   }
}

// constructor for a transmitting network proxy
NetPosition::NetPosition(const std::string& src_device_name, Proxy* proxy_ptr, VJ_NETID_TYPE local_device_id, VJ_NETID_TYPE rmt_device_id ) : NetInput(src_device_name, proxy_ptr, local_device_id, rmt_device_id, 16 * sizeof(float))
{
   mLocalSource.init(mDeviceName);
   mSendBuffer.resize(3 + DATA_TIME_SIZE); // 2 bytes code/id, 16*4 bytes data, 4 byte timestamp, 1 byte for semicolon
   vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_HEX_LVL)
      << "NetPosition: SendBuffer size: " << (3 + DATA_TIME_SIZE )
      << std::endl << vprDEBUG_FLUSH;
   mNetworkMatrices.resize(1);  // a proxy only points to a single data item
}

// constructor for a receiving NetInput
NetPosition::NetPosition(jccl::ConfigChunkPtr chunk, VJ_NETID_TYPE local_device_id) : NetInput(chunk, local_device_id, 16 * sizeof(float)){
   mNetworkMatrices.resize(1);  // a proxy only points to a single data item
   mSendBuffer.resize(3 + DATA_TIME_SIZE * mNetworkMatrices.size()); // buffer not used when receiving, but its size is currently used to determine how much data to pull from the recv buffer
}

// When updating, the data shouldn't really be accessed
void NetPosition::updateFromLocalSource()
{
   vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_HEX_LVL)
      << "NetPosition: mSendBuffer.size() = " << mSendBuffer.size()
      << std::endl << vprDEBUG_FLUSH;
   vprASSERT(mNetworkMatrices.size() == 1);  // only single position can be transmitted by current code

   std::vector<PositionData>  pos_data_sample(mNetworkMatrices.size());

   // next 3 lines should only be executed when using mLocalSource (remote Proxies, not devices)
   //   (i.e. source of data is not contained in the Position portion of this object, but in another object/device).
   //   the lines are not crucial to send the net msg, but keep this object sync just in case it's used.
   if(mLocalSource->getPositionPtr() != NULL)
   {
      pos_data_sample[0] = ( *(mLocalSource->getPosition()) );  // proxies only have one input
      mPosSamples.lock();
      mPosSamples.addSample(pos_data_sample);
      mPosSamples.unlock();
      mPosSamples.swapBuffers();
   }
   else
   {
      for(unsigned int i = 0; i < mNetworkMatrices.size(); i++)
      {
         pos_data_sample[i] = this->getPositionData(i);
      }
   }

   for(unsigned int m = 0; m < mNetworkMatrices.size(); m++)
   {
      //**// vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_VERB_LVL) << "NetPosition: Sending : __________________________ : " << std::endl << *( pos_data_sample[m].getPosition() )  << std::endl << vprDEBUG_FLUSH;
      // convert local data to network data
      for(int f = 0; f < 16; f++)
      {
         mNetworkMatrices[m][f] = vj_htonf( (*(pos_data_sample[m].getPosition()))[f] ); // change it to network byte order
      }
      //**//vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_VERB_LVL) << "NetPosition: Sending : ntwk byte order --------------------------: " << std::endl <<  mNetworkMatrices[m] << std::endl << vprDEBUG_FLUSH;
   }

   ushortTo2Bytes((char*)(&(mSendBuffer[0])), htons(mRemoteId));  // put the 2 byte id in the buffer

   // copy the data into the buffer
   for(unsigned int n = 0; n < mNetworkMatrices.size();  n++)
   {
      for(unsigned int j = 0; j < DATA_SIZE; j++)  // one byte at a time
      {
         mSendBuffer[2 + DATA_TIME_SIZE*n + j ] = ( (char*)(mNetworkMatrices[n].getData()) )[j];
      }
      float net_float = vj_htonf(pos_data_sample[n].getTime().usecf());
      floatTo4Bytes((char*)(&(mSendBuffer[2 + DATA_TIME_SIZE*n + DATA_SIZE])), net_float);   // copy the timestamp
   }

   mSendBuffer[2 + DATA_TIME_SIZE * mNetworkMatrices.size()] = ';';
}

void NetPosition::updateFromRemoteSource(char* recv_buffer, int recv_buff_len){
   vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_HEX_LVL)
      << "NetPosition: mSendBuffer.size() = " << mSendBuffer.size()
      << std::endl << vprDEBUG_FLUSH;

   vprASSERT(mNetworkMatrices.size() == 1);  // only single position can be transmitted by current code

   std::vector<PositionData> pos_data_sample(mNetworkMatrices.size());

   if(recv_buff_len < (int)getDataByteLength()) // make sure the data length is long enough
   {
      vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_VERB_LVL)
         << "ERROR NetPosition: recv_buff_len is not big enough to receive message."
         << std::endl << vprDEBUG_FLUSH;
      vprASSERT(0);
   }

   // copy the data out of the buffer
   for(unsigned int k = 0; k < mNetworkMatrices.size(); k++)
   {
      //**//vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_VERB_LVL) << "NetPosition: PreReceiving : ntwk byte order --------------------------: " << std::endl << mNetworkMatrices[k] << std::endl << vprDEBUG_FLUSH;

      for(unsigned int j = 0; j < DATA_SIZE; j++) // byte by byte
      {
         ( (char*)( mNetworkMatrices[k].getData() ) )[j] = recv_buffer[2 + k*DATA_TIME_SIZE + j];
      }

      //**//vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_VERB_LVL) << "NetPosition: Receiving : ntwk byte order --------------------------: " << std::endl << mNetworkMatrices[k] << std::endl << vprDEBUG_FLUSH;

      float dev_time;
      bytes4ToFloat(dev_time, recv_buffer + 2 + DATA_TIME_SIZE*k + DATA_SIZE );
      vpr::Interval dev_time_interval;
      dev_time_interval.usecf(vj_ntohf(dev_time));
      pos_data_sample[k].setTime(dev_time_interval);

      // convert network data to local data
      for(int i = 0; i < 16; i++)
      {
         (*(pos_data_sample[k].getPosition()))[i] = vj_ntohf( mNetworkMatrices[k][i] ); // change to host byte order
      }
      //**//vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_VERB_LVL) << "NetPosition: Receiving : __________________________ : " << std::endl << *(pos_data_sample[k].getPosition()) << std::endl << vprDEBUG_FLUSH;
   }

   mPosSamples.lock();
   mPosSamples.addSample(pos_data_sample);
   mPosSamples.unlock();

   mPosSamples.swapBuffers();
   //**//vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_VERB_LVL) << "NetPosition: New Data available: -------------------------- : " << *( this->getPositionData(0).getPosition() ) << std::endl << vprDEBUG_FLUSH;
}


}  // end namespace gadget

