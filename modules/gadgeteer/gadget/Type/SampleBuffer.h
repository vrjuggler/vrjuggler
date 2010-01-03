/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2010 by Iowa State University
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
 *************** <auto-copyright.pl END do not edit this line> ***************/

#ifndef _GADGET_SAMPLE_BUFFER_H_
#define _GADGET_SAMPLE_BUFFER_H_

#include <gadget/gadgetConfig.h>

#include <vector>
#include <boost/noncopyable.hpp>

#include <vpr/Util/Assert.h>
#include <vpr/Sync/Guard.h>
#include <vpr/Sync/Mutex.h>


namespace gadget
{

/** \class SampleBuffer SampleBuffer.h gadget/Type/SampleBuffer.h
 *
 * Buffer class for data from input devices. Instances of this class store
 * typed samples in a first-in, first-out (FIFO) queue with a maximum size.
 * The most recent sample is always at the \em end of the queue.
 *
 * There are two buffers (each a vector of vectors of samples): the "ready"
 * and the "stable" samples:
 *
 *   -# Stable: Samples that the user-level code access. There is one entry
 *              in this buffer for each frame of the application.
 *   -# Ready:  Samples that have been completed and could be swapped over to
 *              current.
 *
 * The first index level of the stable vector of vectors is for each frame of
 * the application. The Input Manager tells each device to swap its ready
 * buffers into its stable buffers once per frame. Thus, for each frame, a new
 * set of stable buffers is added for each device.
 *
 * The second index level of both buffers is for the collection of input units
 * on a given device. For example, a three-button mouse without a scroll
 * wheel or roller ball will have three digital input units (one for each
 * button) and two analog input units (one for the X and one for the Y
 * dimensions of movement).
 *
 * Thus, a basic mouse driver would utilize two instantiations of this type:
 * gadget::SampleBuffer<gadget::DigitalData,5000> and
 * gadget::SampleBuffer<gadget::AnalogData,5000> (assuming that the default
 * maximum buffer size is used). Depending on the time of querying (and
 * whether the device is properly producing input data) there would be between
 * 0 and 5000 first-level indices for both sample buffer instances. The
 * second-level vector would have exactly 3 indices of gadget::DigitalData
 * objects and exactly 2 indices of gadget::AnalogData objects.
 *
 * As a simple example of how this type is used, see the methods
 * gadget::Digital::addDigitalSample(), gadget::Digital::swapDigitalBuffers(),
 * and gadget::Digital::getDigitalData(). The first two methods are invoked by
 * all \em subclasses of gadget::Digital (drivers for devices that supply
 * digital input). The third is used by gadget::DigtialProxy::updateData() to
 * get the most recent stable samples for a given digital device unit.
 *
 * @note The buffers can be empty at the start, but after the first cycle
 *       (first time stable gets values) the Stable buffer must have at least
 *       one sample.
 *
 * @tparam DATA_TYPE       The type of the samples stored by the instanation
 *                         of this template type.
 * @tparam MAX_BUFFER_SIZE The maximum allowable size of the buffers. After
 *                         the buffer reaches this size, old samples are
 *                         discarded from the \em front of the queue.
 *
 * @see gadget::InputManager::updateAllProxies()
 * @see gadget::Proxy::updateData()
 * @see gadget::InputManager::updateAllDevices()
 * @see gadget::Input::updateData()
 */
template <class DATA_TYPE, unsigned MAX_BUFFER_SIZE=5000>
class SampleBuffer : private boost::noncopyable
{
public:
   typedef std::vector< std::vector< DATA_TYPE > > buffer_t;

public:
   SampleBuffer()
   {
      /* Do nothing. */ ;
   }

   /**
    * Adds a new sample to the buffer.
    * @pre Buffers must be locked before calling.
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

   /**
    * Swaps the data buffers.
    *
    * @post If ready has values, then copy values from ready to stable
    *       if not, then stable keeps its old values.
    * @note This means that until the first sample, StableBuffer is possibly
    *       empty.
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
   {
      mLock.acquire();
   }

   void unlock()
   {
      mLock.release();
   }

   buffer_t& stableBuffer()
   {
      return mStableBuffer;
   }

   const buffer_t& stableBuffer() const
   {
      return mStableBuffer;
   }

protected:
   buffer_t   mStableBuffer;
   buffer_t   mReadyBuffer;

   vpr::Mutex  mLock;   /**< Lock to protect the buffer */
};

} // end namespace gadget


#endif
