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
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

#include <vpr/vprConfig.h>
#include "boost/date_time/posix_time/posix_time.hpp"
#include <vpr/md/BOOST/IO/SelectorImplBOOST.h>
#include <vpr/md/BOOST/IO/Socket/SocketImplBOOST.h>
#include <vpr/IO/TimeoutException.h>

#include <vpr/System.h>


namespace vpr
{

bool SelectorImplBOOST::addHandle(vpr::IOSys::Handle handle, vpr::Uint16 mask)
{
   if ( getHandle(handle) != mPollDescs.end() )
   {
      return false;
   }

   BOOSTPollDesc new_desc;
   new_desc.fd        = handle;
   new_desc.in_flags  = mask;
   new_desc.out_flags = 0;

   mPollDescs.push_back(new_desc);
   //status = true;

   return true;
}

bool SelectorImplBOOST::removeHandle(vpr::IOSys::Handle handle)
{
   std::vector<BOOSTPollDesc>::iterator i = getHandle(handle);

   if ( mPollDescs.end() == i )
   {
      return false;
   }

   mPollDescs.erase(i);
   return true;
}

bool SelectorImplBOOST::setIn(vpr::IOSys::Handle handle, vpr::Uint16 mask)
{
   std::vector<BOOSTPollDesc>::iterator i = getHandle(handle);

   if ( mPollDescs.end() == i )
   {
      return  false;
   }

   (*i).in_flags = mask;

   return true;
}

vpr::Uint16 SelectorImplBOOST::getIn(vpr::IOSys::Handle handle)
{
   std::vector<BOOSTPollDesc>::iterator i = getHandle(handle);

   if ( mPollDescs.end() == i )
   {
      // XXX: This is VERY bad thing to do.  Need to have an error code instead
      return 0;
   }
   return (*i).in_flags;
}

vpr::Uint16 SelectorImplBOOST::getOut(const vpr::IOSys::Handle handle) const
{
   std::vector<BOOSTPollDesc>::const_iterator i = getHandle(handle);

   if ( mPollDescs.end() == i )
   {
      // XXX: This is VERY bad thing to do.  Need to have an error code instead
      return 0;
   }

   return (*i).out_flags;
}

void SelectorImplBOOST::select(vpr::Uint16& numWithEvents,
                               const vpr::Interval timeout)
{
   std::vector<BOOSTPollDesc>::iterator i;
   numWithEvents = 0;
   bool has_event;
   
   vpr::Interval start, now, local_timeout;
   local_timeout = timeout;
   start.setNowReal();
   if ( timeout == vpr::Interval::NoWait )
   {
      local_timeout.set(0, vpr::Interval::Unit::Sec);
   }
   else if (timeout == vpr::Interval::NoTimeout)
   {
      local_timeout.set(1000, vpr::Interval::Unit::Sec);
   }
   
   do
   {
      for ( i = mPollDescs.begin(); i != mPollDescs.end(); ++i )
      {
         // We have to do this every time to insure that a previous event is not
         // "reselected".
         (*i).out_flags = 0;

         has_event = false;

         if ( (*i).in_flags & vpr::SelectorBase::Read ||
            (*i).in_flags & vpr::SelectorBase::Accept)
         {
            if ( (*i).fd->availableBytes() > 0 )
            {
               has_event = true;
               (*i).out_flags |= vpr::SelectorBase::Read;
            }
         }

         if ( (*i).in_flags & vpr::SelectorBase::Write )
         {
            has_event = true;
            (*i).out_flags |= vpr::SelectorBase::Write;
         }

         if ( has_event )
         {
            numWithEvents++;
         }
      }

      now.setNowReal();
      if (numWithEvents > 0)
      {
         break;
      }
      vpr::System::msleep(10); //Don't busy-wait
   }
   while ((now.usec() - start.usec()) < local_timeout.usec());
   
   if ( numWithEvents == 0 )
   {
      numWithEvents = 0;
      throw TimeoutException("Timeout occured while selecting.", VPR_LOCATION);
   }
}

std::vector<SelectorImplBOOST::BOOSTPollDesc>::iterator
SelectorImplBOOST::getHandle(const vpr::IOSys::Handle handle)
{
   // XXX: Should probably be replaced by a map in the future for speed.

   for ( std::vector<BOOSTPollDesc>::iterator i=mPollDescs.begin();
         i != mPollDescs.end();
         ++i )
   {
      if ( (*i).fd == handle )
      {
         return i;
      }
   }

   return mPollDescs.end();
}

std::vector<SelectorImplBOOST::BOOSTPollDesc>::const_iterator
SelectorImplBOOST::getHandle(const vpr::IOSys::Handle handle) const
{
   // XXX: Should probably be replaced by a map in the future for speed.

   for ( std::vector<BOOSTPollDesc>::const_iterator i=mPollDescs.begin();
         i != mPollDescs.end();
         ++i )
   {
      if ( (*i).fd == handle )
      {
         return i;
      }
   }

   return mPollDescs.end();
}

} // End of vpr namespace
