#ifndef GADGET_NET_INPUT_H
#define GADGET_NET_INPUT_H

#include <gadget/gadgetConfig.h>
#include <jccl/Config/ConfigChunkPtr.h>
#include <gadget/Type/Input.h>
#include <gadget/Type/NetData.h>

namespace gadget{

class Proxy;

class GADGET_CLASS_API NetInput : public Input{
protected:
    std::string mSrcName;
    int num_dependencies;
    NetData* mData;    // can be removed when NetPosition is updated to SampleBuffers design
    bool mAckInitialization;
    bool mWaitingForData;
    bool mNeedToResendRequest;
    std::string mDeviceName;
    VJ_NETID_TYPE mLocalId;  // unsigned short
    VJ_NETID_TYPE mRemoteId;
    std::vector<char> mSendBuffer;  
   
   // constructor used when a device has been requested from us (and remote id has been given)
   NetInput(const std::string& device_name, Input* input_ptr);

   // constructor used when a device has been requested from us (and remote id has been given)
   NetInput(const std::string& device_name, Proxy* proxy_ptr);

   // constructor used when we are going to request a remote device and receive data from it.
   //    a Proxy in the input manager will point to a NetInput in the Remote Input Manager.
   //    the NetInput will have "_NET_" appended to its name.
   NetInput(jccl::ConfigChunkPtr chunk);

public:

    virtual ~NetInput(){;} // derived classes allocated mData, so they also delete mData

    // The following two functions help us keep track if this net_input is being used.
    // We just count them so we'll know when it's not being used anymore -- there's  
       // currently no reason to store pointers to each dependency, 
    void addDependency(){ num_dependencies++; }
    void removeDependency(){ num_dependencies--; }

    void setWasInitializedTrue(){ mAckInitialization = true; }
    bool getWasInitialized() const { return mAckInitialization; }

    void setNeedToResendRequestFlag() { mNeedToResendRequest = true; }
    void clearNeedToResendRequestFlag() { mNeedToResendRequest = false; }
    bool needsToResendRequest() { return mNeedToResendRequest ; }

    std::string getSourceName() const { return mSrcName; }

    virtual void updateFromLocalSource(){ mData->updateFromLocalSource(); }
    virtual void updateFromRemoteSource(char* recv_buffer, int recv_buff_len){ mData->updateFromRemoteSource(recv_buffer, recv_buff_len); }
    virtual char* getDataPtr(){ return mData->getDataPtr(); }
    virtual int getDataByteLength() const { return mData->getDataByteLength(); }
    virtual VJ_NETID_TYPE getLocalId() const { return mData->getLocalId(); }
    virtual VJ_NETID_TYPE getRemoteId() const { return mData->getRemoteId(); }
    virtual void setRemoteId(VJ_NETID_TYPE remote_id){ mData->setRemoteId(remote_id); }

    int sample(){return 1;}
    int startSampling(){return 1;}
    int stopSampling(){return 1;}
    void updateData(){;}
    void refresh(){;}

};

}  // end namespace gadget

#endif

