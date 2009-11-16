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
 * VR Juggler is (C) Copyright 1998-2009 by Iowa State University
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

#include <stdio.h>
#include <string.h>
#include <algorithm>
#include <unistd.h>
#include <sys/types.h>

#ifdef HAVE_BSTRING_H
#  include <bstring.h>
#endif

#include <sys/time.h>
#include <errno.h>

#include <vpr/IO/TimeoutException.h>
#include <vpr/md/POSIX/IO/SelectorImplBSD.h>
#include <vpr/Util/Assert.h>


namespace vpr
{

/**
 * Adds the given handle to the selector.
 */
bool SelectorImplBSD::addHandle(const vpr::IOSys::Handle handle,
                                const vpr::Uint16 mask)
{
   if ( getHandle(handle) != mPollDescs.end() )
   {
      return false;
   }
   
   BSDPollDesc new_desc;
   new_desc.fd        = handle;
   new_desc.in_flags  = mask;
   new_desc.out_flags = 0;

   mPollDescs.push_back(new_desc);

   return true;
}

/**
 * Removes a handle from the selector.
 */
bool SelectorImplBSD::removeHandle(const vpr::IOSys::Handle handle)
{
   std::vector<BSDPollDesc>::iterator i = getHandle(handle);

   if ( mPollDescs.end() == i )
   {
      return false;
   }

   mPollDescs.erase(i);
   return true;
}

/**< Sets the event flags going in to the select to mask. */
bool SelectorImplBSD::setIn(const vpr::IOSys::Handle handle,
                            const vpr::Uint16 mask)
{
   std::vector<BSDPollDesc>::iterator i = getHandle(handle);

   if ( mPollDescs.end() == i )
   {
      return false;
   }

   (*i).in_flags = mask;

   return true;
}

/**< Gets the current in flag mask. */
vpr::Uint16 SelectorImplBSD::getIn(const vpr::IOSys::Handle handle) const
{
   std::vector<BSDPollDesc>::const_iterator i = getHandle(handle);

   if ( mPollDescs.end() == i )
   {
      // XXX: This is VERY bad thing to do.  Need to have an error code instead
      return 0;
   }
   return (*i).in_flags;
}

/**< Gets the current out flag mask. */
vpr::Uint16 SelectorImplBSD::getOut(const vpr::IOSys::Handle handle) const
{
   std::vector<BSDPollDesc>::const_iterator i = getHandle(handle);

   if ( mPollDescs.end() == i )
   {
      // XXX: This is VERY bad thing to do.  Need to have an error code instead
      return 0;
   }
   
   return (*i).out_flags;
}

/**
 * Select.
 */
void SelectorImplBSD::select (vpr::Uint16& numWithEvents,
                              const vpr::Interval& timeout)
{
   int num_events, last_fd;
   fd_set read_set, write_set, exception_set;
   std::vector<BSDPollDesc>::iterator i;
   struct timeval timeout_obj;

   // Zero everything out before doing anything else.
   FD_ZERO(&read_set);
   FD_ZERO(&write_set);
   FD_ZERO(&exception_set);

   last_fd = -1;

   for ( i = mPollDescs.begin(); i != mPollDescs.end(); ++i )
   {
      (*i).out_flags = 0;

      if ( (*i).in_flags & SelectorBase::Read )
      {
         FD_SET((*i).fd, &read_set);
      }

      if ( (*i).in_flags & SelectorBase::Write )
      {
         FD_SET((*i).fd, &write_set);
      }

      if ( (*i).in_flags & SelectorBase::Except )
      {
         FD_SET((*i).fd, &exception_set);
      }

      // Find the highest-valued file descriptor.
      if ( last_fd < (*i).fd )
      {
         last_fd = (*i).fd;
      }
   }

   if ( timeout == vpr::Interval::NoWait )
   {
      timeout_obj.tv_sec  = 0;
      timeout_obj.tv_usec = 0;
   }
   else
   {
      // Apparently select(2) doesn't like if the microsecond member has a
      // time larger than 1 second, so if timeout (given in milliseconds) is
      // larger than 1000, we have to split it up between the seconds and
      // microseconds members.
      if ( timeout.msec() >= 1000 )
      {
         timeout_obj.tv_sec  = timeout.msec() / 1000;
         timeout_obj.tv_usec = (timeout.msec() % 1000) * 1000000;
      }
      else
      {
         timeout_obj.tv_sec  = 0;
         timeout_obj.tv_usec = timeout.msec() * 1000;
      }
   }

   // If timeout is 0, this will be the same as polling the descriptors.  To
   // get no timeout, NULL must be passed to select(2).
   num_events = ::select(last_fd + 1, &read_set, &write_set, &exception_set,
                     (timeout != vpr::Interval::NoTimeout) ? &timeout_obj : NULL);

   // D'oh!
   if ( -1 == num_events )
   {
      fprintf(stderr, "SelectorImplBSD::select: Error selecting: %s\n",
              strerror(errno));
      numWithEvents = 0;

      throw IOException("SelectorImplBSD::select: Error selecting: "
         + std::string(strerror(errno)), VPR_LOCATION);
   }
   // Timeout.
   // XXX: As documented in FileHandleImplUNIX::isReadable(), if timeout
   //      is equal to vpr::Interval::NoWait the file handle may be ready
   //      for reading even though select returned 0. This is vauge in the
   //      documentation because it says that calling select with a timeout
   //      of NULL will return immediately.
   else if ( 0 == num_events )
   {
      numWithEvents = 0;
      throw TimeoutException("Timeout occured while selecting.", VPR_LOCATION);
   }
   // We got one!
   else
   {
      for ( i = mPollDescs.begin(); i != mPollDescs.end(); ++i )
      {
         if ( FD_ISSET((*i).fd, &read_set) )
         {
            (*i).out_flags |= SelectorBase::Read;
         }

         if ( FD_ISSET((*i).fd, &write_set) )
         {
            (*i).out_flags |= SelectorBase::Write;
         }

         if ( FD_ISSET((*i).fd, &exception_set) )
         {
            (*i).out_flags |= SelectorBase::Except;
         }
      }

      numWithEvents = num_events;
   }
}

struct HandlePred
{
   HandlePred(const int handle)
      : mHandle(handle)
   {
   }

   bool operator()(const SelectorImplBSD::BSDPollDesc& d)
   {
      return d.fd == mHandle;
   }

   const int mHandle;
};

// Gets the index of the handle given.
std::vector<SelectorImplBSD::BSDPollDesc>::iterator
SelectorImplBSD::getHandle(const int handle)
{
   // XXX: Should probably be replaced by a map in the future for speed

   HandlePred pred(handle);
   return std::find_if(mPollDescs.begin(), mPollDescs.end(), pred);
}

// Gets the index of the handle given (const version).
std::vector<SelectorImplBSD::BSDPollDesc>::const_iterator
SelectorImplBSD::getHandle(const int handle) const
{
   // XXX: Should probably be replaced by a map in the future for speed

   HandlePred pred(handle);
   return std::find_if(mPollDescs.begin(), mPollDescs.end(), pred);
}

}  // namespace vpr
