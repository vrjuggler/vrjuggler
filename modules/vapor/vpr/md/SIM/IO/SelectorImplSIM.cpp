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
 * VR Juggler is (C) Copyright 1998-2006 by Iowa State University
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

#include <vpr/md/SIM/IO/SelectorImplSIM.h>
#include <vpr/md/SIM/IO/Socket/SocketImplSIM.h> /* This kind of sucks, but oh well ... */


namespace vpr
{

bool SelectorImplSIM::addHandle(vpr::IOSys::Handle handle, vpr::Uint16 mask)
{
   bool status;

   if ( getHandle(handle) == mPollDescs.end() )
   {
      SimPollDesc new_desc;
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

bool SelectorImplSIM::removeHandle(vpr::IOSys::Handle handle)
{
   bool status;
   std::vector<SimPollDesc>::iterator i = getHandle(handle);

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

bool SelectorImplSIM::setIn(vpr::IOSys::Handle handle, vpr::Uint16 mask)
{
   bool status;
   std::vector<SimPollDesc>::iterator i = getHandle(handle);

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

vpr::Uint16 SelectorImplSIM::getIn(vpr::IOSys::Handle handle)
{
   vpr::Uint16 flags;
   std::vector<SimPollDesc>::iterator i = getHandle(handle);

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

vpr::Uint16 SelectorImplSIM::getOut(vpr::IOSys::Handle handle)
{
   vpr::Uint16 flags;
   std::vector<SimPollDesc>::iterator i = getHandle(handle);

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

vpr::ReturnStatus SelectorImplSIM::select(vpr::Uint16& numWithEvents,
                                          const vpr::Interval timeout)
{
   vpr::ReturnStatus status;
   std::vector<SimPollDesc>::iterator i;
   bool has_event;

   numWithEvents = 0;

   for ( i = mPollDescs.begin(); i != mPollDescs.end(); ++i )
   {
      // We have to do this every time to insure that a previous event is not
      // "reselected".
      (*i).out_flags = 0;

      has_event = false;

      if ( (*i).in_flags & vpr::SelectorBase::Read ||
           (*i).in_flags & vpr::SelectorBase::Accept)
      {
         if ( (*i).fd->isReadReady().success() )
         {
            has_event = true;
            (*i).out_flags |= vpr::SelectorBase::Read;
         }
      }

      if ( (*i).in_flags & vpr::SelectorBase::Write )
      {
         if ( (*i).fd->isWriteReady().success() )
         {
            has_event = true;
            (*i).out_flags |= vpr::SelectorBase::Write;
         }
      }

      if ( (*i).in_flags & vpr::SelectorBase::Except )
      {
         if ( (*i).fd->inExceptState().success() )
         {
            has_event = true;
            (*i).out_flags |= vpr::SelectorBase::Except;
         }
      }

      if ( has_event )
      {
         numWithEvents++;
      }
   }

   if ( numWithEvents == 0 )
   {
      status.setCode(vpr::ReturnStatus::Timeout);
   }

   return status;
}

std::vector<SelectorImplSIM::SimPollDesc>::iterator
SelectorImplSIM::getHandle(vpr::IOSys::Handle handle)
{
   // XXX: Should probably be replaced by a map in the future for speed.

   for ( std::vector<SimPollDesc>::iterator i=mPollDescs.begin();
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
