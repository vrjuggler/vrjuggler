#ifndef GADGET_NET_DIGITAL_H
#define GADGET_NET_DIGITAL_H

#include <gadget/gadgetConfig.h>
#include <gadget/RemoteInputManager/NetUtils.h>
#include <gadget/Type/NetInput.h>
#include <gadget/Type/DigitalData.h>

namespace gadget{

class GADGET_CLASS_API NetDigital : public NetInput, public Digital{
protected:
   std::vector <unsigned short> mNetworkShortDigitalValues;  // holds data in network byte order
   DigitalInterface mLocalSource;  // local source of data, only used in transmitting data

public:
   // the following constructor determines what kind of data is passed across the network.
   // In this case, mData will knows how to handle digital data since we allocate

   // constructor for a transmitting network device
   NetDigital(const std::string& src_device_name, Input* input_ptr, VJ_NETID_TYPE local_device_id, VJ_NETID_TYPE rmt_device_id);

   // constructor for a transmitting network proxy
   NetDigital(const std::string& src_device_name, Proxy* proxy_ptr, VJ_NETID_TYPE local_device_id, VJ_NETID_TYPE rmt_device_id);

   // constructor for a receiving NetInput
   NetDigital(jccl::ConfigChunkPtr chunk, VJ_NETID_TYPE local_device_id);
  
   ~NetDigital(){
      delete[] mSendBuffer;
   } 

   // CONFIG related functions
   static std::string getChunkType() { return std::string("NetDigital"); }

   // Overriding NetInput virtual functions
   virtual void updateFromLocalSource();
   virtual void updateFromRemoteSource(char* recv_buffer, int recv_buff_len);
   virtual char* getDataPtr(){ return mSendBuffer; }
   virtual int getDataByteLength() const { return sizeof(mSendBuffer); }
   virtual VJ_NETID_TYPE getLocalId() const { return mLocalId; }
   virtual VJ_NETID_TYPE getRemoteId() const { return mRemoteId; }
   virtual void setRemoteId(VJ_NETID_TYPE remote_id){ mRemoteId = remote_id; }
};

}  // end namespace gadget

#endif
