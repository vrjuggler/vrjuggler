#include <gadget/Type/NetInput.h>

#include <gadget/Type/Proxy.h>

namespace gadget{


// constructor used when a device has been requested from us (and remote id has been given)
NetInput::NetInput(const std::string& src_device_name, Input* input_ptr, VJ_NETID_TYPE local_device_id, VJ_NETID_TYPE rmt_device_id, uint p_data_size) : DATA_SIZE(p_data_size), DATA_TIME_SIZE(p_data_size + sizeof(float)){
   mLocalId = local_device_id;
   mRemoteId = rmt_device_id;
   mDeviceName = src_device_name;
   mAckInitialization = false;
   mNeedToResendRequest = false;
}

// constructor used when a device has been requested from us (and remote id has been given)
NetInput::NetInput(const std::string& src_device_name, Proxy* proxy_ptr, VJ_NETID_TYPE local_device_id, VJ_NETID_TYPE rmt_device_id, uint p_data_size) : DATA_SIZE(p_data_size), DATA_TIME_SIZE(p_data_size + sizeof(float)){
   mLocalId = local_device_id;
   mRemoteId = rmt_device_id;
   mDeviceName = src_device_name;
   mAckInitialization = false;
   mNeedToResendRequest = false;
}

// constructor used when we are going to request a remote device and receive data from it.
// a Proxy in the input manager will point to a NetInput in the Remote Input Manager.
// the NetInput will have "_NET_" appended to its name.
NetInput::NetInput(jccl::ConfigChunkPtr chunk, VJ_NETID_TYPE local_device_id, uint p_data_size) : DATA_SIZE(p_data_size), DATA_TIME_SIZE(p_data_size + sizeof(float)){
   mLocalId = local_device_id;
   mRemoteId = 0;
   mAckInitialization = false;
   mNeedToResendRequest = false;
   mSrcName = chunk->getFullName();
   mInstName = mSrcName;
   mInstName += "_NET_";
}

}  // end namespace gadget
