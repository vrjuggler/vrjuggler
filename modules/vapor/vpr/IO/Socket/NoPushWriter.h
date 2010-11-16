/****************** <VPR heading BEGIN do not edit this line> *****************
 *
 * VR Juggler Portable Runtime
 *
 * Original Authors:
 *   Allen Bierbaum, Patrick Hartling, Kevin Meinert, Carolina Cruz-Neira
 *
 ****************** <VPR heading END do not edit this line> ******************/

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
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

#ifndef _VPR_SOCKET_NO_PUSH_WRITER_H_
#define _VPR_SOCKET_NO_PUSH_WRITER_H_

#include <vpr/vprConfig.h>

#include <boost/function.hpp>

#include <vpr/vprTypes.h>


namespace vpr
{

/** \class NoPushWriter NoPushWriter.h vpr/IO/Socket/NoPushWriter.h
 *
 * A userland implementation of no-push or "corked" writing. This class is
 * only for use on platforms that do not impmlement corked TCP sockets.
 * Moreoever, it is designed for internal use only by VPR stream socket
 * implementations.
 *
 * Memory allocation of the internal buffer can be controlled using a
 * strategy. The job of the strategy is to determine how big the new buffer
 * should be based on the size of the current buffer and the additional
 * space required for a write operation.
 *
 * @see vpr::SocketStreamImplBSD
 * @see vpr::SocketStreamImplNSPR
 *
 * @since 2.1.9
 */
class VPR_CLASS_API NoPushWriter
{
public:
   /**
    * Constructor.
    */
   NoPushWriter(const NoPushAllocStrategy& strategy);

   /**
    * Copy constructor.
    *
    * @post If \p obj has an internal buffer, a copy of it is made.
    */
   NoPushWriter(const NoPushWriter& obj);

   /**
    * Ensures that the internal buffer is released.
    */
   ~NoPushWriter();

   /**
    * This appends the given buffer to the internal "corking" buffer.
    * \c mBuffer is allocated or resized as necessary using the allocation
    * strategy. Upon initial allocation, 0 is passed as the first argument to
    * the allocation strategy, and \p length is passed as the second argument.
    * For all other uses of the allocation strategy, \c mBuffferSize is passed
    * as the first argument and \p length as the second.
    *
    * @pre The socket is open for writing.
    * @post \c mBuffer points to a memory buffer large enough to contain the
    *       given data block and all previous data written to this object
    *       since the last time it was flushed.
    *
    * @param buffer A pointer to the buffer to be written.
    * @param length The length of the buffer.
    *
    * @return The number of bytes stored in the buffer is returned.
    *
    * @throw vpr::Exception
    *           Thrown if corking is enabled and, in the event that resizing
    *           the corking buffer is necessary, the new buffer allocation
    *           size returned by the allocation strategy is insufficiently
    *           large.
    * @throw std::bad_alloc
    */
   vpr::Uint32 write(const void* buffer, const vpr::Uint32 length);

   /**
    * Changes the strategy used to allocate and resize the internal buffer.
    *
    * @param strategy A callable object used to determine how much memory to
    *                 allocate when the internal buffer needs to grow.
    *
    * @see write()
    */
   void setAllocStrategy(const NoPushAllocStrategy& strategy)
   {
      allocStrategy = strategy;
   }

   /**
    * Returns a pointer to the current internal buffer.
    */
   const void* getBuffer() const
   {
      return mBuffer;
   }

   /**
    * Returns the total size of the internal buffer. This does not necessarily
    * reflect the use of that memory.
    *
    * @see getBufferUse()
    */
   size_t getBufferSize() const
   {
      return mBufferSize;
   }

   /**
    * Returns the amount of memory in the internal buffer that has been
    * consumed.
    */
   size_t getBufferUse() const
   {
      return mBufferUse;
   }

   /**
    * Releases the internal buffer.
    *
    * @post \c mBuffer is NULL and any memory that had been allocated is
    *       freed. \c mBufferSize and \c mBufferUse are 0.
    */
   void flush();

private:
   NoPushAllocStrategy allocStrategy;
   void*               mBuffer;       /**< Internal buffer for corked storage */
   size_t              mBufferSize;   /**< Total size of \c mBuffer */
   size_t              mBufferUse;    /**< Current usage of \c mBuffer */
};

}


#endif /* _VPR_SOCKET_NO_PUSH_WRITER_H_ */
