#include <gadget/Type/NetPositionData.h>

namespace gadget{
   
// When updating, the data shouldn't really be accessed
void NetPositionData::updateFromLocalSource(){
   // vprDEBUG(vrjDBG_INPUT_MGR, vprDBG_CRITICAL_LVL) << "NetPositionData: Sending : __________________________ : " << std::endl << mPosData << std::endl << vprDEBUG_FLUSH;
   // convert local data to network data      
   mPosData = *(vrj::Matrix*)(mLocalSource->getData());       // copy position matrix
   for(int k = 0; k < 16; k++){
      mNetworkMatrix.getFloatPtr()[k] = vj_htonf( mPosData.getFloatPtr()[k] ); // change it to network byte order
   }      
            
   ushortTo2Bytes(send_buffer, htons(mRemoteId));  // put the 2 byte id in the buffer
      
   // copy the number byte by byte into the buffer
   for(unsigned int j = 0; j < sizeof(float) * 16; j++)
      send_buffer[2 + j] = ((char*)(&mNetworkMatrix))[j];   

   send_buffer[3 + 16*4 - 1] = ';';
}

void NetPositionData::updateFromRemoteSource(char* recv_buffer, int recv_buff_length){
   // copy the number byte by byte out of the buffer
   for(unsigned int j = 0; j < sizeof(float) * 16; j++)
      ((char*)(&mNetworkMatrix))[j] = recv_buffer[2 + j];   

   // convert network data to local data
   for(int i = 0; i < 16; i++){
      mPosData.getFloatPtr()[i] = vj_ntohf( mNetworkMatrix.getFloatPtr()[i] ); // change it to host byte order
   }

   // std::cout << "NetPos: update: " << std::endl << mPosData << std::endl;
   vprDEBUG(vrjDBG_INPUT_MGR, vprDBG_VERB_LVL) << "NetPositionData: Receiving : __________________________ : " << std::endl << mPosData << std::endl << vprDEBUG_FLUSH;
}

}  // end namespace gadget

