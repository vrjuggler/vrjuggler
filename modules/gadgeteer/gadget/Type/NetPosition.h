#ifndef GADGET_NET_POS_H
#define GADGET_NET_POS_H

#include <gadget/gadgetConfig.h>
#include <gadget/RemoteInputManager/NetUtils.h>                                       
#include <gadget/Type/NetInput.h>
#include <gadget/Type/Position.h>
#include <gadget/Type/PositionInterface.h>
#include <gadget/Type/PositionData.h>

namespace gadget{

class GADGET_CLASS_API NetPosition : public NetInput, public Position{
protected:
   std::vector <vrj::Matrix>  mNetworkMatrices;  // holds data in network byte order
   PositionInterface mLocalSource;  // local source of data, only used in transmitting data.

public:
   // constructor for a transmitting device
   NetPosition(const std::string& src_device_name, Input* input_ptr, VJ_NETID_TYPE local_device_id, VJ_NETID_TYPE rmt_device_id);

   // constructor for a transmitting proxy
   NetPosition(const std::string& src_device_name, Proxy* proxy_ptr, VJ_NETID_TYPE local_device_id, VJ_NETID_TYPE rmt_device_id); 

   // constructor for a receiving NetInput
   NetPosition(jccl::ConfigChunkPtr chunk, VJ_NETID_TYPE local_device_id);

   // CONFIG related functions
   static std::string getChunkType() { return std::string("NetPosition"); }
  
   // Overriding NetInput virtual functions
   virtual void updateFromLocalSource();
   virtual void updateFromRemoteSource(char* recv_buffer, int recv_buff_len);
   virtual char* getDataPtr(){ return (char*) &(mSendBuffer[0]); }
   virtual int getDataByteLength() const { return mSendBuffer.size(); }
   virtual VJ_NETID_TYPE getLocalId() const { return mLocalId; }
   virtual VJ_NETID_TYPE getRemoteId() const { return mRemoteId; }
   virtual void setRemoteId(VJ_NETID_TYPE remote_id){ mRemoteId = remote_id; }
};

}  // end namespace gadget

#endif

