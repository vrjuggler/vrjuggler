#include <gadget/Type/NetInput.h>

#include <gadget/Type/Proxy.h>

namespace gadget{


// constructor used when a device has been requested from us (and remote id has been given)
NetInput::NetInput(const std::string& device_name, Input* input_ptr){
   mAckInitialization = false;
   mNeedToResendRequest = false;
}

// constructor used when a device has been requested from us (and remote id has been given)
NetInput::NetInput(const std::string& device_name, Proxy* proxy_ptr){
   // specific NetInputs such as NetDigital, will allocate mData.
   mAckInitialization = false;
   mNeedToResendRequest = false;
}

// constructor used when we are going to request a remote device and receive data from it.
// a Proxy in the input manager will point to a NetInput in the Remote Input Manager.
// the NetInput will have "_NET_" appended to its name.
NetInput::NetInput(jccl::ConfigChunkPtr chunk){
   mAckInitialization = false;
   mNeedToResendRequest = false;
   mSrcName = (std::string) chunk->getProperty("name");
   mInstName = mSrcName;
   mInstName += "_NET_";
}

}  // end namespace gadget
