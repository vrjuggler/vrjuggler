#ifndef GADGET_NET_DATA_H
#define GADGET_NET_DATA_H

#include <gadget/gadgetConfig.h>
#include <gadget/RemoteInputManager/NetUtils.h>
#include <gadget/Type/Input.h>

namespace gadget{

class GADGET_CLASS_API NetData{
protected:
    std::string mDeviceName;
    VJ_NETID_TYPE mLocalId;  // unsigned short
    VJ_NETID_TYPE mRemoteId;
public:    
   NetData(VJ_NETID_TYPE local_id){     
      mRemoteId = 0;
      mLocalId = local_id;
   }

   NetData(VJ_NETID_TYPE local_id, VJ_NETID_TYPE remote_id){     
      mRemoteId = remote_id;
      mLocalId = local_id;
   }
   virtual ~NetData(){;}
   
   // used when other functions want to send this data
   virtual char* getDataPtr(){return 0;}
   virtual size_t getDataByteLength(){return 0;}

   // called once per frame to allow it to refresh it's data
   virtual bool setLocalSource(Input* device){return 0;}
   virtual void updateFromLocalSource(){;}
   virtual void updateFromRemoteSource(char* recv_buffer, int recv_buff_length){;}
   virtual void setDeviceName(const std::string& deviceName){ mDeviceName = deviceName; }
   // device name should be set first
   virtual void init(){;}  // for initializing the device interface, if possible 

   void setLocalId(VJ_NETID_TYPE local_id){ mLocalId = local_id; }
   void setRemoteId(VJ_NETID_TYPE remote_id){ mRemoteId = remote_id; }
   VJ_NETID_TYPE getLocalId(){ return mLocalId; }
   VJ_NETID_TYPE getRemoteId(){ return mRemoteId; }
   
};

}  // end namespace gadget

#endif

