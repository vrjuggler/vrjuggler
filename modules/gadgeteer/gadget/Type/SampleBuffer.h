/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2003 by Iowa State University
 *
 * Original Authors:
 *   Allen Bierbaum, Christopher Just,
 *   Patrick Hartling, Kevin Meinert,
 *   Carolina Cruz-Neira, Albert Baker
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 *
 * -----------------------------------------------------------------
 * File:          $RCSfile$
 * Date modified: $Date$
 * Version:       $Revision$
 * -----------------------------------------------------------------
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

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
*
* @param MAX_BUFFER_SIZE - The maximum allowable size of the buffer.  After it gets this large
*                          we will start throwing away old data.
*/
template <class DATA_TYPE, unsigned MAX_BUFFER_SIZE=5000>
class SampleBuffer
{
public:
   typedef std::vector< std::vector< DATA_TYPE > > buffer_t;

public:
   SampleBuffer()
   {
      /* Do nothing. */ ;
   }

   /** Add a new sample to the buffer
    * @pre Buffers must be locked before calling
    */
   void addSample(const std::vector< DATA_TYPE >& dataSample)
   {
      vprASSERT(mLock.test());                  // Verify that it is locked
      mReadyBuffer.push_back(dataSample);
      if(mReadyBuffer.size() > MAX_BUFFER_SIZE)
      {
         while(mReadyBuffer.size() > MAX_BUFFER_SIZE)
         { mReadyBuffer.erase(mReadyBuffer.begin()); }
      }
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
   // vpr::Mutex is not copyable, so neither are we.
   SampleBuffer(const SampleBuffer& b) {;}
   void operator=(const SampleBuffer& b) {;}

   buffer_t   mStableBuffer;
   buffer_t   mReadyBuffer;

   vpr::Mutex  mLock;   /**< Lock to protect the buffer */
};

} // end namespace gadget


#endif
