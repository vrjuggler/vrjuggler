/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998, 1999, 2000 by Iowa State University
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

#include <vprConfig.h>

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>

#ifdef HAVE_BSTRING_H
#include <bstring.h>
#endif

#include <sys/time.h>
#include <errno.h>

#include <md/POSIX/SelectorImpBSD.h>
#include <Utils/Assert.h>


namespace vpr {

//: Add the given handle to the selector
//! PRE: handle is a valid handle
//! POST: handle is added to the handle set, and initialized to a mask of
//+       no-events
bool
SelectorImpBSD::addHandle (IOSys::Handle handle) {
   bool status;

   if ( getHandle(handle) == mPollDescs.end() ) {
      BSDPollDesc new_desc;
      new_desc.fd        = handle;
      new_desc.in_flags  = 0;
      new_desc.out_flags = 0;

      mPollDescs.push_back(new_desc);
      status = true;
   }
   else {
      status = false;
   }

   return status;
}

//: Remove a handle from the selector
//! PRE: handle is in the selector
//! POST: handle is removed from the set of valid handles
bool
SelectorImpBSD::removeHandle (IOSys::Handle handle) {
   bool status;
   std::vector<BSDPollDesc>::iterator i = getHandle(handle);

   if ( mPollDescs.end() == i ) {
      status = false;
   }
   else {
      mPollDescs.erase(i);
      status = true;
   }

   return status;
}

//: Set the event flags going in to the select to mask
bool
SelectorImpBSD::setIn (IOSys::Handle handle, vpr::Uint16 mask) {
   bool status;
   std::vector<BSDPollDesc>::iterator i = getHandle(handle);

   if ( mPollDescs.end() == i ) {
      status = false;
   }
   else {
      (*i).in_flags = mask;
      status = true;
   }

   return status;
}

//: Get the current in flag mask
vpr::Uint16
SelectorImpBSD::getIn (IOSys::Handle handle) {
   vpr::Uint16 flags;
   std::vector<BSDPollDesc>::iterator i = getHandle(handle);

   if ( mPollDescs.end() == i ) {
      // XXX: This is VERY bad thing to do.  Need to have an error code instead
      flags = 0;
   }
   else {
      flags = (*i).in_flags;
   }

   return flags;
}

//: Get the current out flag mask
vpr::Uint16
SelectorImpBSD::getOut (IOSys::Handle handle) {
   vpr::Uint16 flags;
   std::vector<BSDPollDesc>::iterator i = getHandle(handle);

   if ( mPollDescs.end() == i ) {
      // XXX: This is VERY bad thing to do.  Need to have an error code instead
      flags = 0;
   }
   else {
      flags = (*i).out_flags;
   }

   return flags;
}

//: Select
//! ARGS: numWithEvents - Upon completion, this holds the number of items that
//+                       have events
//! ARGS: timeout - The number of msecs to select for (0 - don't wait)
Status
SelectorImpBSD::select (vpr::Uint16& numWithEvents, vpr::Uint16 timeout) {
   vpr::Status ret_val;
   int result;
   fd_set read_set, write_set, exception_set;
   std::vector<BSDPollDesc>::iterator i;
   struct timeval timeout_obj;

   // Zero everything out before doing anything else.
   FD_ZERO(&read_set);
   FD_ZERO(&write_set);
   FD_ZERO(&exception_set);

   for ( i = mPollDescs.begin(); i != mPollDescs.end(); i++ ) {
      if ( (*i).in_flags & SelectorBase::VPR_READ ) {
         FD_SET((*i).fd, &read_set);
      }

      if ( (*i).in_flags & SelectorBase::VPR_WRITE ) {
         FD_SET((*i).fd, &write_set);
      }

      if ( (*i).in_flags & SelectorBase::VPR_EXCEPT ) {
         FD_SET((*i).fd, &exception_set);
      }
   }

   // Apparently select(2) doesn't like if the microsecond member has a time
   // larger than 1 second, so if timeout (given in milliseconds) is larger
   // than 1000, we have to split it up between the seconds and microseconds
   // members.
   if ( timeout >= 1000 ) {
      timeout_obj.tv_sec  = timeout / 1000;
      timeout_obj.tv_usec = (timeout % 1000) * 1000000;
   }
   else {
      timeout_obj.tv_sec  = 0;
      timeout_obj.tv_usec = timeout * 1000;
   }

   // If timeout is 0, this will be the same as polling the descriptors.  To
   // get no timeout, NULL must be passed to select(2).
   result = ::select(mPollDescs.size(), &read_set, &write_set, &exception_set,
                     (timeout > 0) ? &timeout_obj : NULL);

   // D'oh!
   if ( -1 == result ) {
      fprintf(stderr, "SelectorImpBSD::select: Error selecting: %s\n",
              strerror(errno));
      numWithEvents = 0;
      ret_val.setCode(Status::Failure);
   }
   // Timeout.
   else if ( 0 == result ) {
      numWithEvents = 0;
      ret_val.setCode(Status::Timeout);
   }
   // We got one!
   else {
      for ( i = mPollDescs.begin(); i != mPollDescs.end(); i++ ) {
         if ( FD_ISSET((*i).fd, &read_set) ) {
            (*i).out_flags |= SelectorBase::VPR_READ;
         }

         if ( FD_ISSET((*i).fd, &write_set) ) {
            (*i).out_flags |= SelectorBase::VPR_WRITE;
         }

         if ( FD_ISSET((*i).fd, &exception_set) ) {
            (*i).out_flags |= SelectorBase::VPR_EXCEPT;
         }
      }

      numWithEvents = result;
   }

   return ret_val;
}

// Get the index of the handle given
//! RETURNS: .end() - Not found, else the index to the handle in mPollDescs
std::vector<SelectorImpBSD::BSDPollDesc>::iterator
SelectorImpBSD::getHandle (int handle) {
   // XXX: Should probably be replaced by a map in the future for speed

   for(std::vector<BSDPollDesc>::iterator i=mPollDescs.begin();
          i != mPollDescs.end();i++)
   {
      if((*i).fd == handle)
         return i;
   }

   return mPollDescs.end();
}

}  // namespace vpr
