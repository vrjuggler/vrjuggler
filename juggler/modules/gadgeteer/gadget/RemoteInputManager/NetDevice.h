#ifndef GADGET_NET_DEVICE_H
#define GADGET_NET_DEVICE_H


#include <gadget/Type/Input.h>
#include <gadget/RemoteInputManager/NetUtils.h>
#include <vpr/IO/ObjectWriter.h>

namespace gadget {

class GADGET_CLASS_API NetDevice
{
protected:
   Input*         mRealInput;            /**< Pointer to the real/virtual device */     
   std::string    mDeviceName;           /**< Name of the device */
   std::string    mSrcName;              /**< Name of the local device */     //<<<<SIMPLIFY
   int            mNumDependencies;
   bool           mAckInitialization;
   bool           mWaitingForData;
   bool           mNeedToResendRequest;

   vpr::ObjectWriter*   mObjectWriter;

   VJ_NETID_TYPE  mLocalId;
   VJ_NETID_TYPE  mRemoteId;
public:
  /** 
   * Creates a new NetDevice that is used to manage the transmission of the
   * device's data.
   *
   * @param src_device_name   The name of the local device.
   * @param input_ptr         A pointer to the local device.
   * @param local_device_id   A unique identifier for this NetDevice
   * @param rmt_device_id     A unique identifier for the remote NetDevice
   *
   * @pre   The local machine must have this real device attached locally.
   * @post  An object that is responsible for the information regarding a
   *        device shared on a cluster
   */
   NetDevice(const std::string& src_device_name, Input* input_ptr, VJ_NETID_TYPE local_device_id, VJ_NETID_TYPE rmt_device_id)
   {
      mRealInput = input_ptr;
      mNumDependencies = 0;
      mLocalId = local_device_id;
      mRemoteId = rmt_device_id;
      mDeviceName = src_device_name;
      mSrcName = src_device_name;
      mAckInitialization = false;
      mNeedToResendRequest = false;
      std::vector<vpr::Uint8>* temp = new std::vector<vpr::Uint8>(100);
      mObjectWriter = new vpr::ObjectWriter(temp);
   }
  
  /** 
   * Creates a new NetDevice that is used to manage the reception of a remote
   * device's data.
   *
   * @param device_name   The name of the remote device.
   * @param local_device_id   A unique identifier for this NetDevice
   *
   * @pre   A remote machin in the cluster must have the real device.
   * @post  An object that is responsible for the information regarding a
   *        device shared on a cluster
   */
   NetDevice(std::string dev_name, VJ_NETID_TYPE local_device_id)
   {
      mRealInput = NULL;
      mNumDependencies = 0;
      mLocalId = local_device_id;
      mRemoteId = 0;
      mAckInitialization = false;
      mNeedToResendRequest = false;
      mSrcName = dev_name;
   }
   ~NetDevice()
   {;}
   
   void NetDevice::updateFromLocalSource()
   {
      mObjectWriter->getData()->clear();
      mObjectWriter->setCurPos(0);
      mRealInput->writeObject(mObjectWriter);
   }

   vpr::ObjectWriter* getObjectWriter()
   {
      return mObjectWriter;
   }

  /**
   *  Adds a dependency for the NetDevice
   */
   void addDependency()
   { mNumDependencies++; }
  
  /**
   *  Removes a dependency for the NetDevice
   */
   void removeDependency()
   { mNumDependencies--; }
   
  /**
   * Returns the number of dependencies for this NetDevice
   * @return   An integer representing the number of dependencies
   */
   int getDependencies()
   { return (mNumDependencies); }

  /**
   * Set its pointer to the real device, or the "virtual device" which
   * is of the identical BaseType
   */
   void setRealDevice(Input* new_device)
   { mRealInput = new_device; }
   
  /**
   *  Returns a pointer to the RealDevice, or the "virtual device" which
   * is of the identical BaseType
   */
   Input* getRealDevice()
   { return(mRealInput); }
   
  /**
   *  Returns the unique id for this NetDevice.
   */
   VJ_NETID_TYPE getLocalId() const
   { return mLocalId; }
   
  /**
   *  Returns the unique id for the NetDevice on the remote device.
   */
   VJ_NETID_TYPE getRemoteId() const
   { return mRemoteId; }
   
  /**
   *  Sets the unique id for the NetDevice on the remote device.
   */
   void setRemoteId(VJ_NETID_TYPE remote_id)
   { mRemoteId = remote_id; }
   
  /**
   *  Set a flag that says that it has been Initialized
   */
   void setWasInitializedTrue()
   { mAckInitialization = true; }

  /**
   *  Returns true if it has been initialized
   */
   bool getWasInitialized() const
   { return mAckInitialization; }
   
  /**
   *  Sets a flag that to make RIM try to request this device again
   */
   void setNeedToResendRequestFlag()
   { mNeedToResendRequest = true; }
   
  /**
   *  Erase a flag that would make RIM try to request this device again
   */
   void clearNeedToResendRequestFlag()
   { mNeedToResendRequest = false; }
   
  /**
   *  Returns true if RIM needs to request this device again
   */
   bool needsToResendRequest()
   { return mNeedToResendRequest; }
   
  /**
   *  Returns the name of the device we are managing.
   */
   std::string getSourceName() const
   { return mSrcName; }
};

} // end namespace gadget

#endif




