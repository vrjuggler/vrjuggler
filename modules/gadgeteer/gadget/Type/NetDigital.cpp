#include <gadget/Type/NetDigital.h>

#include <vrj/Util/Debug.h>

namespace gadget{

// constructor for a transmitting network device
NetDigital::NetDigital(const std::string& src_device_name, Input* input_ptr, VJ_NETID_TYPE local_device_id, VJ_NETID_TYPE rmt_device_id) : NetInput(src_device_name, input_ptr){

   vprDEBUG(vrjDBG_INPUT_MGR, vprDBG_CONFIG_LVL) << "NetDigital: local_device_id = " << local_device_id << std::endl << vprDEBUG_FLUSH;

  mLocalId = local_device_id;
  mRemoteId = rmt_device_id;
  mDeviceName = src_device_name;

  if(src_device_name.length() > 0){   // pointing to another device/proxy for data source
     mLocalSource.init(mDeviceName);
     mSendBuffer.resize(5);            // 2 bytes of code/id, 2 bytes of data, 1 byte for semicolon
     mNetworkShortDigitalValues.resize(1);  // space for one data item
  }
  else{                              // use ourself as source of digital data
     // find the number of devices

     int num_elements = this->getDigitalDataBuffer().size();
     if (num_elements <= 0)
        num_elements = 1;

     mNetworkShortDigitalValues.resize(num_elements);

     // allocate space for data
     mSendBuffer.resize(3 + sizeof(short) * num_elements);  // 2 bytes of code/id, 2*n bytes of data, 1 byte for semicolon
  }
}

// constructor for a transmitting network proxy
NetDigital::NetDigital(const std::string& src_device_name, Proxy* proxy_ptr, VJ_NETID_TYPE local_device_id, VJ_NETID_TYPE rmt_device_id) : NetInput(src_device_name, proxy_ptr){
   mLocalId = local_device_id;
   mRemoteId = rmt_device_id;
   mDeviceName = src_device_name;
   mLocalSource.init(mDeviceName);
   mSendBuffer.resize(5);
   mNetworkShortDigitalValues.resize(1);  // a digital proxy only points to a single data item
}

// constructor for a receiving NetInput
NetDigital::NetDigital(jccl::ConfigChunkPtr chunk, VJ_NETID_TYPE local_device_id) : NetInput(chunk) {
   mLocalId = local_device_id;
   mRemoteId = 0;
   mSendBuffer.resize(1); // not used;
   mNetworkShortDigitalValues.resize(1);  // a digital proxy only points to a single data item
   mSendBuffer.resize(3 + sizeof(short) * mNetworkShortDigitalValues.size()); // buffer not used when receiving, but its size (5) is currently used to determine how much data to pull from the recv buffer
}

void NetDigital::updateFromLocalSource(){
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

   shortTo2Bytes((char*)(&(mSendBuffer[0])), htons(mRemoteId));  // put the 2 byte id in the buffer

   unsigned int j = 0;
   for( ; j < mNetworkShortDigitalValues.size(); j++){
      for(unsigned int i = 0; i < sizeof(short); i++)
         mSendBuffer[2 + sizeof(short)*j + i] = ((char*)(& (mNetworkShortDigitalValues[j]) ))[i];   // copy the number byte by byte
   }

   // mSendBuffer[4] = ';';                                             // terminate the message
   mSendBuffer[2 + sizeof(short) * j] = ';';

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
      for(unsigned int i = 0; i < sizeof(short); i++)
         ((char*)( &(mNetworkShortDigitalValues[k]) ))[i] = recv_buffer[2 + sizeof(short)*k + i];

      // convert network data to local data
      // mDigitalData = (short) ntohs(mNetworkShortDigitalValue);
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

