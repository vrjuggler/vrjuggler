#ifndef GADGET_NET_POSITIONAL_DATA_H
#define GADGET_NET_POSITIONAL_DATA_H

#include <gadget/gadgetConfig.h>
#include <gadget/Type/NetData.h>
#include <gadget/Type/PositionInterface.h>
#include <gadget/Type/Position.h>
#include <gadget/RemoteInputManager/NetUtils.h>
#include <vpr/System.h>
#include <vrj/Math/Matrix.h>

namespace gadget{

class GADGET_CLASS_API NetPositionData : public NetData, public PositionData{ 

protected:
  // Position mPosData; // vrj::Matrix mPosData;
  vrj::Matrix mNetworkMatrix;  // used by local device before sending across network

  PositionInterface mLocalSource;  // local source of data, only used in LocalNetInput
  char send_buffer[3 + 16 * sizeof(float)];  // 2 bytes of code/id, 16 floats of data (16*4 bytes), 1 byte for semicolon

public:
   NetPositionData(VJ_NETID_TYPE local_id) : NetData(local_id){
      vprDEBUG(vrjDBG_INPUT_MGR, vprDBG_STATE_LVL) << "NetPositionData: local_id = " << local_id << std::endl << vprDEBUG_FLUSH;
   }
   NetPositionData(VJ_NETID_TYPE local_id, VJ_NETID_TYPE remote_id) : NetData(local_id,remote_id){
      vprDEBUG(vrjDBG_INPUT_MGR, vprDBG_STATE_LVL) << "NetPositionData: local_id = " << local_id << ", remote_id = " << remote_id << std::endl << vprDEBUG_FLUSH;
   }
   
   // only used locally
   void init(){   
      mLocalSource.init(mDeviceName);
   }
   
   // When updating, the data shouldn't really be accessed
   virtual void updateFromLocalSource();

   virtual void updateFromRemoteSource(char* recv_buffer, int recv_buff_length);

   // char* getDataPtr(){return (char*)mNetworkMatrix.getFloatPtr();}  // used by NetConnection to send data
   // size_t getDataByteLength(){ return 16 * sizeof(float);}    // ditto

   char* getDataPtr(){return send_buffer;}                      // used by NetConnection to send data
   size_t getDataByteLength(){ return sizeof(send_buffer);}    // ditto
  
   // virtual PositionData getPositionData(int devNum = 0){
   //   return mPosData;
   // }

};

}  // end namespace gadget

#endif

