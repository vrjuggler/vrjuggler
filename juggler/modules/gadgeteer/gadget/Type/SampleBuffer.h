#ifndef _SAMPLE_BUFFER_H_
#define _SAMPLE_BUFFER_H_


#include <gadget/gadgetConfig.h>

#include <vpr/Sync/Mutex.h>
#include <vpr/Sync/Guard.h>
#include <vector>
      
namespace gadget
{


/** Buffer class for input data
*
* The SampleBuffer stores the readings from an input device.
*
* It consists of two buffers (vectors of vectors of samples) that hold
* The "stable" samples and the "ready" samples.
*
* Stable - Samples that the application is actually looking at
* Ready   - Samples that have been completed and could be swapped over to current
*/
template <class DATA_TYPE>
class SampleBuffer
{
public:
   typedef std::vector< std::vector< DATA_TYPE > > buffer_t;

public:
   /** Add a new sample to the buffer
    * @pre Buffers must be locked before calling
    */
   void addSample(const std::vector< DATA_TYPE >& dataSample)
   {
      vprASSERT(mLock.test());      // Verify that it is locked
      mReadyBuffer.push_back(dataSample);
   }

   /** Swap the data buffers
    * @post Values from ready are copied to stable and 
    *       ready keeps the last value it had (this keeps there from being a frame without data)
    */
   void swapBuffers()
   {
   vpr::Guard<vpr::Mutex>  guard(mLock);
      vprASSERT(!mReadyBuffer.empty());   // ASSERT: We actually have values in ready buffer

      mStableBuffer = mReadyBuffer;    // Copy over the ready values
      if(mStableBuffer.size() > 1)       // If we have more then one sample in buffer
      {
         mReadyBuffer.front() = mReadyBuffer.back();                       // Keep copy of most recent sample
         mReadyBuffer.erase(mReadyBuffer.begin()+1, mReadyBuffer.end());   // Erase all but the first value
      }

      vprASSERT(!mReadyBuffer.empty());      // ASSERT: We actually have values in ready buffer
      vprASSERT(!mStableBuffer.empty());     // ASSERT: We actually have values in stable buffer
   }
   
   void lock()
   {  mLock.acquire(); }
   
   void unlock()
   {  mLock.release(); }

   buffer_t& stableBuffer()
   { return mStableBuffer; }   

protected:
   buffer_t   mStableBuffer;
   buffer_t   mReadyBuffer;
   
   vpr::Mutex  mLock;   /**< Lock to protect the buffer */   
};

}; // end namespace gadget

#endif
