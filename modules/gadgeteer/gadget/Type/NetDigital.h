#ifndef GADGET_NET_DIGITAL_H
#define GADGET_NET_DIGITAL_H

#include <gadget/gadgetConfig.h>
#include <gadget/RemoteInputManager/NetUtils.h>
#include <gadget/Type/NetInput.h>
#include <gadget/Type/NetDigitalData.h>
#include <gadget/Type/DigitalData.h>

namespace gadget{

// The data is prepared and parsed from the network with NetDigitalData

class GADGET_CLASS_API NetDigital : public NetInput, public Digital{
protected:
  NetDigitalData* mDigitalData;
public:
  // the following constructor determines what kind of data is passed across the network.
  // In this case, mData will knows how to handle digital data since we allocate
  // a NetData object (NetDigitalData) that knows how to handle digital data.

  // a transmitting device
  NetDigital(const std::string& device_name, Input* input_ptr, VJ_NETID_TYPE local_device_id, VJ_NETID_TYPE rmt_device_id) : NetInput(device_name, input_ptr){
      mDigitalData = new NetDigitalData(local_device_id, rmt_device_id);
      mData = dynamic_cast<NetData*> (mDigitalData);  
      mData->setDeviceName(device_name);
      mData->init();
  }

  // a transmitting proxy
  NetDigital(const std::string& device_name, Proxy* proxy_ptr, VJ_NETID_TYPE local_device_id, VJ_NETID_TYPE rmt_device_id) : NetInput(device_name, proxy_ptr){
      mDigitalData = new NetDigitalData(local_device_id, rmt_device_id);
      mData = dynamic_cast<NetData*> (mDigitalData); 
      mData->setDeviceName(device_name);
      mData->init();
  }

  // a receiving NetInput
  NetDigital(jccl::ConfigChunkPtr chunk, VJ_NETID_TYPE local_device_id) : NetInput(chunk) {     
     mDigitalData = new NetDigitalData(local_device_id);     
     mData = dynamic_cast<NetData*> (mDigitalData);
  }
  
  ~NetDigital(){
      mData = NULL;
      delete mDigitalData;
  }

  // CONFIG related functions
  static std::string getChunkType() { return std::string("NetDigital"); }
  // bool config(jccl::ConfigChunkPtr c){ return baseConfig(c); }
  
  // Digital virtual function
  // virtual DigitalData* getDigitalData(int devNum = 0){ return dynamic_cast<DigitalData*> (mDigitalData); }
  virtual const DigitalData getDigitalData(int devNum = 0){ return * (dynamic_cast<DigitalData*> (mDigitalData)); }

};

/*
class NetDigitalProxy : public NetProxy, public DigitalProxy{
protected:
  NetDigitalData* mDigitalData;
public:
  // the following constructor determines what kind of data is passed across the network.
  // In this case, mData will knows how to handle digital data since we allocate
  // a NetData object (NetDigitalData) that knows how to handle digital data.
  NetDigitalProxy(){ mDigitalData = NULL; mData = NULL; }

  bool config(ConfigChunkPtr chunk)
   {
   vprDEBUG_BEGIN(vrjDBG_INPUT_MGR,3) << "----------- configuring NET DIGITAL proxy ----\n" << vprDEBUG_FLUSH;
   vprASSERT(((std::string)chunk->getType()) == "NetDigProxy");

   // m_unitNum = chunk->getProperty("unit");
   mDeviceName = (std::string)chunk->getProperty("device");
   refresh();
   return true;
   }


  // a transmitting device
  NetDigitalProxy(const std::string& device_name, Input* input_ptr, VJ_NETID_TYPE local_device_id, VJ_NETID_TYPE rmt_device_id) : NetProxy(device_name, input_ptr){
      mDigitalData = new NetDigitalData(local_device_id, rmt_device_id);
      mData = dynamic_cast<NetData*> (mDigitalData);  
      mData->setDeviceName(device_name);
      mData->init();
  }

  // a transmitting proxy
  NetDigitalProxy(const std::string& device_name, Proxy* proxy_ptr, VJ_NETID_TYPE local_device_id, VJ_NETID_TYPE rmt_device_id) : NetProxy(device_name, proxy_ptr){
      mDigitalData = new NetDigitalData(local_device_id, rmt_device_id);
      mData = dynamic_cast<NetData*> (mDigitalData); 
      mData->setDeviceName(device_name);
      mData->init();
  }

  // a receiving NetInput
  NetDigitalProxy(ConfigChunkPtr chunk, VJ_NETID_TYPE local_device_id) : NetProxy(chunk) {     
     mDigitalData = new NetDigitalData(local_device_id);     
     mData = dynamic_cast<NetData*> (mDigitalData);
  }
  
  ~NetDigitalProxy(){
      mData = NULL;
      delete mDigitalData;
  }

  // CONFIG related functions
  static std::string getChunkType() { return std::string("NetDigProxy"); }
  // bool config(ConfigChunkPtr c){ return baseConfig(c); }
  
  // Digital virtual function
  // virtual int getDigitalData(int devNum = 0){ return mDigitalData->getDigitalData(); }
  virtual int getDigitalData(int devNum = 0){ return 3; }
};
*/

}  // end namespace gadget

#endif
