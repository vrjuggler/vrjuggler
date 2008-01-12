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
 * VR Juggler is (C) Copyright 1998-2008 by Iowa State University
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

#include <vpr/vprConfig.h>

#include <cstdlib>
#include <sstream>
#include <new>
#include <algorithm>

#include <vpr/Util/Exception.h>
#include <vpr/IO/Socket/NoPushWriter.h>


namespace vpr
{

NoPushWriter::NoPushWriter(const NoPushAllocStrategy& strategy)
   : allocStrategy(strategy)
   , mBuffer(NULL)
   , mBufferSize(0)
   , mBufferUse(0)
{
   /* Do nothing. */ ;
}

NoPushWriter::NoPushWriter(const NoPushWriter& obj)
   : allocStrategy(obj.allocStrategy)
   , mBuffer(NULL)
   , mBufferSize(obj.mBufferSize)
   , mBufferUse(obj.mBufferUse)
{
   if ( NULL != obj.mBuffer )
   {
      mBuffer = std::malloc(mBufferSize);
      std::memcpy(mBuffer, obj.mBuffer, mBufferSize);
   }
}

NoPushWriter::~NoPushWriter()
{
   flush();
}

vpr::Uint32 NoPushWriter::write(const void* buffer, const vpr::Uint32 length)
{
   if ( NULL == mBuffer )
   {
      const size_t new_buffer_size = allocStrategy(0, length);

      if ( new_buffer_size < length )
      {
         std::ostringstream msg_stream;
         msg_stream << "Allocation of " << new_buffer_size
                    << " bytes for initial corking buffer is not "
                    << "larger than packet size " << length;
         throw vpr::Exception(msg_stream.str(), VPR_LOCATION);
      }

      mBufferSize = new_buffer_size;
      mBuffer     = std::malloc(mBufferSize);

      if ( NULL == mBuffer )
      {
         throw std::bad_alloc();
      }

      std::memset(mBuffer, 0, mBufferSize);
   }
   else if ( mBufferUse + length > mBufferSize )
   {
      const size_t new_buffer_size = allocStrategy(mBufferSize, length);

      if ( new_buffer_size <= mBufferSize )
      {
         std::ostringstream msg_stream;
         msg_stream << "Allocation of " << new_buffer_size
                    << " bytes for resizing of corking buffer is not "
                    << "larger than starting size " << mBufferSize;
         throw vpr::Exception(msg_stream.str(), VPR_LOCATION);
      }

      mBufferSize = new_buffer_size;
      mBuffer     = std::realloc(mBuffer, mBufferSize);

      if ( NULL == mBuffer )
      {
         throw std::bad_alloc();
      }
   }

   char* cork_buffer      = reinterpret_cast<char*>(mBuffer);
   const char* pkt_buffer = reinterpret_cast<const char*>(buffer);

   // Append the bytes from pkt_buffer to the end of cork_buffer.
   std::copy(pkt_buffer, pkt_buffer + length, &cork_buffer[mBufferUse]);
   mBufferUse += length;

   return length;
}

void NoPushWriter::flush()
{
   if ( NULL != mBuffer )
   {
      std::free(mBuffer);
      mBuffer     = NULL;
      mBufferUse  = 0;
      mBufferSize = 0;
   }
}

}
