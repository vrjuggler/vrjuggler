/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2005 by Iowa State University
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
 * Date modified: $Date: 2005-01-01 14:59:16 -0600 (Sat, 01 Jan 2005) $
 * Version:       $Revision: 16524 $
 * -----------------------------------------------------------------
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

#include <gadget/gadgetConfig.h>

#include <gadget/Node.h>
#include <gadget/Reactor.h>


namespace gadget
{

void Reactor::addNode(gadget::NodePtr node)
{
   vpr::IOSys::Handle handle = node->getSockStream()->getHandle();

   if ( mDemuxTable.find(handle) == mDemuxTable.end() )
   {
      mDemuxTable[handle] = node;
      mSelector.addHandle(handle);
      mSelector.setIn(handle, vpr::Selector::Read);
   }
}

void Reactor::removeNode(gadget::NodePtr node)
{
   vpr::IOSys::Handle handle = node->getSockStream()->getHandle();

   typedef std::map<vpr::IOSys::Handle, gadget::NodePtr>::iterator iter_t;
   iter_t i = mDemuxTable.find(handle);

   if ( i != mDemuxTable.end() )
   {
      mDemuxTable.erase(i);
      mSelector.removeHandle(handle);
   }
}

void Reactor::getReadyNodes(std::vector<gadget::NodePtr>& readyList,
                            const vpr::Interval& timeout)
{
   vpr::Uint16 num_events(0);
   mSelector.select(num_events, timeout);

   if ( num_events > 0 )
   {
      readyList.reserve(num_events);

      for ( vpr::Uint16 i = 0; i < mSelector.getNumHandles(); ++i )
      {
         vpr::IOSys::Handle h         = mSelector.getHandle(i);
         const vpr::Uint16 event_mask = mSelector.getOut(h);

         if ( 0 != event_mask )
         {
            readyList.push_back(mDemuxTable[h]);
         }
      }
   }
}

}
