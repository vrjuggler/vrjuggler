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
*
* ASSERTION: The buffers can be empty at the start, but after the first cycle 
*            (first time stable gets values) the Stable buffer must have at least 
*            one sample.
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
    * @post If ready has values, then copy values from ready to stable 
    *        if not, then stable keeps its old values
    * @note This means that until the first sample, StableBuffer is possibly empty.
    */
   void swapBuffers()
   {
   vpr::Guard<vpr::Mutex>  guard(mLock);
      
      if(!mReadyBuffer.empty())            // If Ready buffer has data
      {
         mStableBuffer = mReadyBuffer;       // Copy over the ready values
      }
      
      mReadyBuffer.clear();      
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
