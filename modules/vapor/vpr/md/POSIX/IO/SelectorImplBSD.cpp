/****************** <VPR heading BEGIN do not edit this line> *****************
 *
 * VR Juggler Portable Runtime
 *
 * Original Authors:
 *   Allen Bierbaum, Patrick Hartling, Kevin Meinert, Carolina Cruz-Neira
 *
 * -----------------------------------------------------------------
 * File:          $RCSfile$
 * Date modified: $Date$
 * Version:       $Revision$
 * -----------------------------------------------------------------
 *
 ****************** <VPR heading END do not edit this line> ******************/

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
 *************** <auto-copyright.pl END do not edit this line> ***************/

#include <vpr/vprConfig.h>

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>

#ifdef HAVE_BSTRING_H
#  include <bstring.h>
#endif

#include <sys/time.h>
#include <errno.h>

#include <vpr/md/POSIX/IO/SelectorImplBSD.h>
#include <vpr/Util/Assert.h>


namespace vpr
{

/**
 * Adds the given handle to the selector.
 */
bool SelectorImplBSD::addHandle (vpr::IOSys::Handle handle, vpr::Uint16 mask)
{
   bool status;

   if ( getHandle(handle) == mPollDescs.end() )
   {
      BSDPollDesc new_desc;
      new_desc.fd        = handle;
      new_desc.in_flags  = mask;
      new_desc.out_flags = 0;

      mPollDescs.push_back(new_desc);
      status = true;
   }
   else
   {
      status = false;
   }

   return status;
}

/**
 * Removes a handle from the selector.
 */
bool SelectorImplBSD::removeHandle (vpr::IOSys::Handle handle)
{
   bool status;
   std::vector<BSDPollDesc>::iterator i = getHandle(handle);

   if ( mPollDescs.end() == i )
   {
      status = false;
   }
   else
   {
      mPollDescs.erase(i);
      status = true;
   }

   return status;
}

/**< Sets the event flags going in to the select to mask. */
bool SelectorImplBSD::setIn (vpr::IOSys::Handle handle, vpr::Uint16 mask)
{
   bool status;
   std::vector<BSDPollDesc>::iterator i = getHandle(handle);

   if ( mPollDescs.end() == i )
   {
      status = false;
   }
   else
   {
      (*i).in_flags = mask;
      status = true;
   }

   return status;
}

/**< Gets the current in flag mask. */
vpr::Uint16 SelectorImplBSD::getIn (vpr::IOSys::Handle handle)
{
   vpr::Uint16 flags;
   std::vector<BSDPollDesc>::iterator i = getHandle(handle);

   if ( mPollDescs.end() == i )
   {
      // XXX: This is VERY bad thing to do.  Need to have an error code instead
      flags = 0;
   }
   else
   {
      flags = (*i).in_flags;
   }

   return flags;
}

/**< Gets the current out flag mask. */
vpr::Uint16 SelectorImplBSD::getOut (vpr::IOSys::Handle handle)
{
   vpr::Uint16 flags;
   std::vector<BSDPollDesc>::iterator i = getHandle(handle);

   if ( mPollDescs.end() == i )
   {
      // XXX: This is VERY bad thing to do.  Need to have an error code instead
      flags = 0;
   }
   else
   {
      flags = (*i).out_flags;
   }

   return flags;
}

/**
 * Select.
 */
vpr::ReturnStatus SelectorImplBSD::select (vpr::Uint16& numWithEvents,
                                           const vpr::Interval timeout)
{
   vpr::ReturnStatus ret_val;
   int result, last_fd;
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
   result = ::select(last_fd + 1, &read_set, &write_set, &exception_set,
                     (timeout != vpr::Interval::NoTimeout) ? &timeout_obj : NULL);

   // D'oh!
   if ( -1 == result )
   {
      fprintf(stderr, "SelectorImplBSD::select: Error selecting: %s\n",
              strerror(errno));
      numWithEvents = 0;
      ret_val.setCode(ReturnStatus::Fail);
   }
   // Timeout.
   else if ( 0 == result )
   {
      numWithEvents = 0;
      ret_val.setCode(ReturnStatus::Timeout);
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

      numWithEvents = result;
   }

   return ret_val;
}

/**
 * Gets the index of the handle given.
 */
std::vector<SelectorImplBSD::BSDPollDesc>::iterator SelectorImplBSD::getHandle (int handle)
{
   // XXX: Should probably be replaced by a map in the future for speed

   for ( std::vector<BSDPollDesc>::iterator i=mPollDescs.begin();
       i != mPollDescs.end();++i )
   {
      if ( (*i).fd == handle )
         return i;
   }

   return mPollDescs.end();
}

}  // namespace vpr
