
#include <gadget/RemoteInputManager/RecvBuffer.h>
#include <vrj/Util/Debug.h>
 
namespace gadget
{
  
// SwapBuffers
// only swaps the pointers to the two buffers in our system
void RecvBuffer::swapBuffers(){  // instead of recopying data containing half a message, we swap pointers
   char* temp = current_buffer;
   current_buffer = secondary_buffer;
   secondary_buffer = temp;
}


// Constructor
RecvBuffer::RecvBuffer(){
   current_buffer = buffer1;
   secondary_buffer = buffer2;
   read_offset = 0;        
   write_offset = 0;
}
    

// Mark a number of bytes as processed already

void RecvBuffer::markAsRead(int num_bytes){
   read_offset += num_bytes;
   if(read_offset > write_offset){
      vprDEBUG(vrjDBG_INPUT_MGR, vprDBG_HEX_LVL) << "RecvBuffer::markAsRead() read_offset > RIM_BUFFER_SIZE" << std::endl << vprDEBUG_FLUSH; 
      read_offset = write_offset;
      // note: this means readable_bytes = 0;
    }
}
    

// increaseReadableSize: this means we have just put more data in the buffer
// moves write ptr and keeps track of size

void RecvBuffer::increaseReadableSize(int num_bytes){
   if (num_bytes > 0) {                 // extra (debugging only?) check for bad parameters
      write_offset += num_bytes;
      if (write_offset > RIM_BUFFER_SIZE - 1)   // Make sure we didn't increase readable size too much
         write_offset = RIM_BUFFER_SIZE - 1;
   }
}


// use this after reading bytes to make more room for new data in the buffer.
void RecvBuffer::eraseAlreadyReadBytes(){
   vprASSERT( getReadableSize() >= 0);
   if(write_offset == 0)  // if we can write in position zero, nothing was able to be read
      return;
   // if there's any readable characters left, copy them to the other buffer.
   // strncpy(secondary_buffer, getReadPtr(), getReadableSize()); // Duh, can't use strncpy for raw data
   char* read_ptr = getReadPtr();
   for (int i = 0; i < getReadableSize(); i++) {
      secondary_buffer[i] = read_ptr[i];
   }
   swapBuffers();
   write_offset = getReadableSize(); // write_offset - read_offset
   read_offset = 0;
}

}   // end namespace gadget



