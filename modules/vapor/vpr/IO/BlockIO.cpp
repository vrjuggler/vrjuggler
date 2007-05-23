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
 * VR Juggler is (C) Copyright 1998-2007 by Iowa State University
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

#include <vpr/IO/Stats/BaseIOStatsStrategy.h>
#include <vpr/IO/Selector.h>
#include <vpr/IO/BlockIO.h>


namespace vpr
{

bool BlockIO::isReadBlocked(const vpr::Interval& timeout)
{
   bool is_blocked;
   vpr::Selector selector;
   vpr::IOSys::Handle handle;
   vpr::Uint16 num_events;

   handle = getHandle();
   selector.addHandle(handle);
   selector.setIn(handle, vpr::Selector::Read);

   // Test the handle to get its read state.
   selector.select(num_events, timeout);

   if ( num_events == 1 )
   {
      is_blocked = false;
   }
   else
   {
      is_blocked = true;
   }

   return is_blocked;
}

bool BlockIO::isWriteBlocked(const vpr::Interval& timeout)
{
   bool is_blocked;
   vpr::Selector selector;
   vpr::IOSys::Handle handle;
   vpr::Uint16 num_events;

   handle = getHandle();
   selector.addHandle(handle);
   selector.setIn(handle, vpr::Selector::Write);

   // Test the handle to get its write state.
   selector.select(num_events, timeout);

   if ( num_events == 1 )
   {
      is_blocked = false;
   }
   else
   {
      is_blocked = true;
   }

   return is_blocked;
}

void BlockIO::setIOStatStrategy(vpr::BaseIOStatsStrategy* strat)
{
   if ( NULL != mStatsStrategy )
   {
      delete mStatsStrategy;
   }

   mStatsStrategy = strat;

   if ( mStatsStrategy != NULL )           // If we have a non-NULL strategy
   {
      mStatsStrategy->setRealObject(this); // Tell it about us
   }
}

BlockIO::BlockIO()
   : mOpen(false)
   , mBlocking(true)
   , mStatsStrategy(NULL)
{
   /* Do nothing. */ ;
}

BlockIO::BlockIO(const std::string& name)
   : mName(name)
   , mOpen(false)
   , mBlocking(true)
   , mStatsStrategy(NULL)
{
   /* Do nothing. */ ;
}

BlockIO::BlockIO(const BlockIO& other)
   : mName(other.mName)
   , mOpen(other.mOpen)
   , mBlocking(other.mBlocking)
   , mStatsStrategy(NULL)
{
   /* Do nothing. */ ;
}

BlockIO::~BlockIO()
{
   if (NULL != mStatsStrategy)
   {
      delete mStatsStrategy;
   }
}

vpr::Uint32 BlockIO::read_s(void* buffer, const vpr::Uint32 length,
                            const vpr::Interval& timeout)
{
   vpr::Uint32 bytes(0);

   if ( mStatsStrategy != NULL )
   {
      mStatsStrategy->read_s(buffer, length, bytes, timeout);
   }
   else
   {
      bytes = read_i(buffer, length, timeout);
   }

   return bytes;
}

vpr::Uint32 BlockIO::readn_s(void* buffer, const vpr::Uint32 length,
                             const vpr::Interval& timeout)
{
   vpr::Uint32 bytes(0);

   if ( mStatsStrategy != NULL )
   {
      mStatsStrategy->readn_s(buffer, length, bytes, timeout);
   }
   else
   {
      bytes = readn_i(buffer, length, timeout);
   }

   return bytes;
}

vpr::Uint32 BlockIO::write_s(const void* buffer, const vpr::Uint32 length,
                             const vpr::Interval& timeout)
{
   vpr::Uint32 bytes(0);

   if ( mStatsStrategy != NULL )
   {
      mStatsStrategy->write_s(buffer, length, bytes, timeout);
   }
   else
   {
       bytes = write_i(buffer, length, timeout);
   }

   return bytes;
}

}
