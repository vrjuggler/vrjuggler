#include <gadget/Type/NetDigital.h>

#include <vrj/Util/Debug.h>
#include <jccl/Plugins/PerformanceMonitor/TimeStamp.h>

namespace gadget{

// constructor for a transmitting network device
NetDigital::NetDigital(const std::string& src_device_name, Input* input_ptr, VJ_NETID_TYPE local_device_id, VJ_NETID_TYPE rmt_device_id) : NetInput(src_device_name, input_ptr, local_device_id, rmt_device_id, sizeof(short)) {

   vprDEBUG(vrjDBG_INPUT_MGR, vprDBG_CONFIG_LVL) << "NetDigital: local_device_id = " << local_device_id << std::endl << vprDEBUG_FLUSH;

  if(src_device_name.length() > 0){   // pointing to another device/proxy for data source
     mLocalSource.init(mDeviceName);
     mSendBuffer.resize(3 + DATA_TIME_SIZE * 1);            // 2 bytes for code/id, 2 bytes data, 4 bytes timestamp, 1 byte for semicolon
     mNetworkShortDigitalValues.resize(1);  // space for one data item
  }
  else{                              // use ourself as source of digital data
     // find the number of devices

     int num_elements = this->getDigitalDataBuffer().size();
     if (num_elements <= 0)
        num_elements = 1;

     mNetworkShortDigitalValues.resize(num_elements);

     // allocate space for data
     mSendBuffer.resize(3 + DATA_TIME_SIZE * num_elements);  
     // 2 bytes for code/id, 2*n bytes for data, 4*n bytes of timestamp data, 1 byte for semicolon
  }
}

// constructor for a transmitting network proxy
NetDigital::NetDigital(const std::string& src_device_name, Proxy* proxy_ptr, VJ_NETID_TYPE local_device_id, VJ_NETID_TYPE rmt_device_id) : NetInput(src_device_name, proxy_ptr, local_device_id, rmt_device_id, sizeof(short)) {
   mLocalSource.init(mDeviceName);
   mSendBuffer.resize(3 + DATA_TIME_SIZE * 1);
   mNetworkShortDigitalValues.resize(1);  // a digital proxy only points to a single data item
}

// constructor for a receiving NetInput
NetDigital::NetDigital(jccl::ConfigChunkPtr chunk, VJ_NETID_TYPE local_device_id) : NetInput(chunk, local_device_id, sizeof(short)){ 
   mSendBuffer.resize(1); // not used;
   mNetworkShortDigitalValues.resize(1);  // a digital proxy only points to a single data item
   mSendBuffer.resize(3 + DATA_TIME_SIZE * mNetworkShortDigitalValues.size());
   // buffer not used when receiving, but its size (9) is currently used to determine how much data to pull from the recv buffer
}

void NetDigital::updateFromLocalSource(){
   vprASSERT(mNetworkShortDigitalValues.size() == 1);  // currently, only single digital can be transmitted at a time 
   std::vector<DigitalData>  digital_data_sample(mNetworkShortDigitalValues.size());

   // next 3 lines should only be executed when using mLocalSource (remote Proxies, not devices)
   //   (i.e. source of data is not contained in the Digital portion of this object, but in another object/device).
   //   the lines are not crucial to send the net msg, but keep this object sync just in case it's used.
   if(mLocalSource->getDigitalPtr() != NULL){         
      digital_data_sample[0] = *(mLocalSource->getDigitalData());  // proxies only have one digital input  `
      mDigitalSamples.lock();
      mDigitalSamples.addSample(digital_data_sample);
      mDigitalSamples.unlock();
      mDigitalSamples.swapBuffers();
   }
   else{
      for(unsigned int i = 0; i < mNetworkShortDigitalValues.size(); i++)
         digital_data_sample[i] = this->getDigitalData(i);
   }

   for(unsigned int k = 0; k < mNetworkShortDigitalValues.size(); k++){
      vprDEBUG(vrjDBG_INPUT_MGR, vprDBG_VERB_LVL) << "NetDigital: src_name: " << mSrcName << ",  DeviceName" << mDeviceName << ".  Sending : __________________________ : " << digital_data_sample[k].getDigital() << std::endl << vprDEBUG_FLUSH;
      mNetworkShortDigitalValues[k] = htons( (short)digital_data_sample[k].getDigital());           // convert to network format
   }

   ushortTo2Bytes((char*)(&(mSendBuffer[0])), htons(mRemoteId));  // put the 2 byte id in the buffer

   unsigned int j = 0;
   for( ; j < mNetworkShortDigitalValues.size(); j++){
      for(unsigned int i = 0; i < DATA_SIZE; i++)  // 
         mSendBuffer[2 + DATA_TIME_SIZE*j + i] = ((char*)(& (mNetworkShortDigitalValues[j]) ))[i];   // copy the number byte by byte
      float net_float = vj_htonf(digital_data_sample[j].getTime().usecs());
      floatTo4Bytes((char*)(&(mSendBuffer[2 + DATA_TIME_SIZE*j + DATA_SIZE])), net_float);   // copy the timestamp
   }

   mSendBuffer[2 + DATA_TIME_SIZE * j] = ';';                            // terminate the message

}


// the recv_buffer should be exactly the same as the mSendBuffer: opcode and semicolon included
void NetDigital::updateFromRemoteSource(char* recv_buffer, int recv_buff_len){
   if(recv_buff_len < (int)getDataByteLength()){  // make sure the data length is long enough
      vprDEBUG(vrjDBG_INPUT_MGR, vprDBG_VERB_LVL) << "ERROR NetDigital: recv_buff_len is not big enough to receive message." << std::endl << vprDEBUG_FLUSH;
      return;
   }

   std::vector<DigitalData> digital_data_sample(mNetworkShortDigitalValues.size());

   vprASSERT(mNetworkShortDigitalValues.size() == 1);

   // get the data out of the buffer
   for(unsigned int k = 0; k < mNetworkShortDigitalValues.size(); k++){
      for(unsigned int i = 0; i < DATA_SIZE; i++)
         ((char*)( &(mNetworkShortDigitalValues[k]) ))[i] = recv_buffer[2 + DATA_TIME_SIZE*k + i];

      float dev_time;
      bytes4ToFloat(dev_time, recv_buffer + 2 + DATA_TIME_SIZE*k + DATA_SIZE );
      digital_data_sample[k].setTime(jccl::TimeStamp(vj_ntohf(dev_time)));       // get timestamp from buffer

      // convert network data to local data
      digital_data_sample[k] = (short) ntohs(mNetworkShortDigitalValues[k]);
      vprDEBUG(vrjDBG_INPUT_MGR, vprDBG_VERB_LVL) << "NetDigital: src_name: " << mSrcName << ",  DeviceName" << mDeviceName << ".  Receiving : __________________________ : " << digital_data_sample[k].getDigital() << std::endl << vprDEBUG_FLUSH;
   }

   mDigitalSamples.lock();
   mDigitalSamples.addSample(digital_data_sample);
   mDigitalSamples.unlock();

   mDigitalSamples.swapBuffers();
   vprDEBUG(vrjDBG_INPUT_MGR, vprDBG_VERB_LVL) << "NetDigital: New Data available: -------------------------- : " << this->getDigitalData(0).getDigital() << std::endl << vprDEBUG_FLUSH;

}



}  // end namespace gadget

