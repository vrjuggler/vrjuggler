#ifndef GADGET_NET_DIGITAL_DATA_H
#define GADGET_NET_DIGITAL_DATA_H

#include <gadget/gadgetConfig.h>
#include <gadget/Type/NetData.h>
#include <gadget/Type/DigitalProxy.h>
#include <gadget/Type/DeviceInterface.h>
#include <gadget/RemoteInputManager/NetUtils.h>

namespace gadget{

// needs to multiply inherit because only vjDigital can be used with vjDigitalProxy
class GADGET_CLASS_API NetDigitalData : public NetData, public DigitalData{ // , public Digital{

protected:
   // short mDigitalData;  // needed to store digital value
   unsigned short mNetworkShortDigitalValue;
   char send_buffer[5];  // 2 bytes of code/id, 4 bytes of data, 1 byte for semicolon
   DigitalInterface mLocalSource;  // local source of data, only used in LocalNetInput  
   
public:
   NetDigitalData(VJ_NETID_TYPE local_id) : NetData(local_id){
      vprDEBUG(vrjDBG_INPUT_MGR, vprDBG_CONFIG_LVL) << "NetDigitalData: local_id = " << local_id << std::endl << vprDEBUG_FLUSH;
   }

   NetDigitalData(VJ_NETID_TYPE local_id, VJ_NETID_TYPE remote_id) : NetData(local_id,remote_id){
      vprDEBUG(vrjDBG_INPUT_MGR, vprDBG_CONFIG_LVL) << "NetDigitalData: local_id = " << local_id << ", remote_id = " << remote_id << std::endl << vprDEBUG_FLUSH;
   }

   void init(){
      mLocalSource.init(mDeviceName);
   }
   
   // When updating, the data shouldn't really be accessed
   virtual void updateFromLocalSource();

   // the recv_buffer should be exactly the same as the send_buffer: opcode and semicolon included
   virtual void updateFromRemoteSource(char* recv_buffer, int recv_buff_len);

   char* getDataPtr(){return send_buffer;}                   // used by NetConnection to send network data
   size_t getDataByteLength(){ return sizeof(send_buffer);}  // ditto
  
   /*  // dynamic cast moved to NetDigital
   virtual DigitalData* getDigitalData(int devNum = 0){
      return dynamic_cast<DigitalData*> (this); // return mDigitalData;  
   }
   */

   int getDigital() const
   { return mDigitalData; }

};

}  // end namespace gadget

#endif

