#ifndef GADGET_RECV_BUFFER_H
#define GADGET_RECV_BUFFER_H

#include <gadget/gadgetConfig.h>

namespace gadget
{

static const int RIM_BUFFER_SIZE = 1024;

// A buffer for the Remote Input Manager to use in network communication.
// It buffers the data received from another juggler instance.
class GADGET_CLASS_API RecvBuffer{
protected:
  int connection_index;
  char buffer1[RIM_BUFFER_SIZE];
  char buffer2[RIM_BUFFER_SIZE];  // two buffers are necessary to avoid recopying when saving old info.
  char* current_buffer;
  char* secondary_buffer;
  int write_offset;
  int read_offset;
  // note: recv_buffer_length = write_offset - read_offset
  
  // only swaps the pointers to the two buffers in our system
  void swapBuffers();

public:
   RecvBuffer();
   
   // Mark a number of bytes as processed already
   void markAsRead(int num_bytes);

   char* getReadPtr(){ return current_buffer + read_offset; }
   char* getWritePtr(){ return current_buffer + write_offset; }
   int getReadableSize(){ return write_offset - read_offset; } 
   int getWriteableSize(){ return RIM_BUFFER_SIZE - write_offset; }
    
   // this means we have put more data in the buffer
   // moves write ptr and keeps track of size
   void increaseReadableSize(int num_bytes); 

    // use this after reading/procesing bytes to make more room for new data in the buffer.
    void eraseAlreadyReadBytes();
};

}   // namespace gadget


#endif
