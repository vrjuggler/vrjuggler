#include <gadget/Type/NetDigitalData.h>

#include <vrj/Util/Debug.h>

namespace gadget{

// we use shorts instead of ints, since ints vary in size

// When updating, the data shouldn't really be accessed
void NetDigitalData::updateFromLocalSource(){
   // convert local data to network data
   // mDigitalData = mLocalSource->getDigitalData(mUnit);
   mDigitalData = mLocalSource->getData();
   vprDEBUG(vrjDBG_INPUT_MGR, vprDBG_VERB_LVL) << "NetDigitalData: Sending : __________________________ : " << mDigitalData << std::endl << vprDEBUG_FLUSH;
   mNetworkShortDigitalValue = htons( (short)mDigitalData );           // convert to network format
            
   shortTo2Bytes(send_buffer, htons(mRemoteId));  // put the 2 byte id in the buffer
      
   for(unsigned int i = 0; i < sizeof(short); i++)
      send_buffer[2 + i] = ((char*)(&mNetworkShortDigitalValue))[i];   // copy the number byte by byte

   send_buffer[4] = ';';                                             // terminate the message
      
}

// the recv_buffer should be exactly the same as the send_buffer: opcode and semicolon included
void NetDigitalData::updateFromRemoteSource(char* recv_buffer, int recv_buff_len){
   if(recv_buff_len < (int)getDataByteLength())  // make sure the data length is long enough 
      return;
   // get the data out of the buffer
   for(unsigned int i = 0; i < sizeof(short); i++)
      ((char*)(&mNetworkShortDigitalValue))[i] = recv_buffer[2 + i];
   // convert network data to local data
   mDigitalData = (short) ntohs(mNetworkShortDigitalValue);
   vprDEBUG(vrjDBG_INPUT_MGR, vprDBG_VERB_LVL) << "NetDigitalData: Receiving : __________________________ : " << mDigitalData << std::endl << vprDEBUG_FLUSH;
   //C std::cout << "NetDigital: update: " << mDigitalData << std::endl;
}

}  // end namespace gadget

