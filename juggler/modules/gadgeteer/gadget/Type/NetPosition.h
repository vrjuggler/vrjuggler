#ifndef GADGET_NET_POS_H
#define GADGET_NET_POS_H

#include <gadget/gadgetConfig.h>
#include <gadget/RemoteInputManager/NetUtils.h>                                       
#include <gadget/Type/NetInput.h>
#include <gadget/Type/NetPositionData.h>
#include <gadget/Type/Input.h>
#include <gadget/Type/Proxy.h>

// File contains Declarations for Local and Remote Positional Net Devices

// These classes are used to uniquely identify LocalNetPosition and RemoteNetPosition devices
// They also specify which datatype is used with these devices

// The data is prepared and parsed from the network with NetPositionData
// The data is transferred over the network with LocalNet

namespace gadget{

class GADGET_CLASS_API NetPosition : public NetInput, public Position{
protected:
   NetPositionData* mPosData;
public:
   // a transmitting device
   NetPosition(const std::string& device_name, Input* input_ptr, VJ_NETID_TYPE local_device_id, VJ_NETID_TYPE rmt_device_id) : NetInput(device_name, input_ptr){
      mPosData = new NetPositionData(local_device_id, rmt_device_id);
      mData = dynamic_cast<NetData*> (mPosData);  
      mData->setDeviceName(device_name);
      mData->init();
   }

   // a transmitting proxy
   NetPosition(const std::string& device_name, Proxy* proxy_ptr, VJ_NETID_TYPE local_device_id, VJ_NETID_TYPE rmt_device_id) : NetInput(device_name, proxy_ptr){
      mPosData = new NetPositionData(local_device_id, rmt_device_id);
      mData = dynamic_cast<NetData*> (mPosData); 
      mData->setDeviceName(device_name);
      mData->init();
   }

   NetPosition(jccl::ConfigChunkPtr chunk, VJ_NETID_TYPE local_device_id) : NetInput(chunk) {
      mPosData = new NetPositionData(local_device_id);
      mData = dynamic_cast<NetData*> (mPosData);
   }

   ~NetPosition(){ mData = NULL; delete mPosData; }

   // CONFIG related functions
   static std::string getChunkType() { return std::string("NetPosition"); }
  
   // virtual PositionData* getPositionData(int devNum = 0){ return dynamic_cast<PositionData*> (mPosData); }
   virtual PositionData getPositionData(int devNum = 0){ return *(dynamic_cast<PositionData*> (mPosData)); }

};

}  // end namespace gadget

#endif

