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
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
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

std::vector<gadget::NodePtr> Reactor::getReadyNodes(const vpr::Interval& timeout)
{
   vpr::Uint16 num_events(0);
   mSelector.select(num_events, timeout);

   std::vector<gadget::NodePtr> ready_nodes;

   if ( num_events > 0 )
   {
      ready_nodes.reserve(num_events);
      vpr::Uint16 event_mask;
      const vpr::Uint16 num_handles = mSelector.getNumHandles();
      for ( vpr::Uint16 i = 0; i < num_handles; ++i )
      {
         vpr::IOSys::Handle h = mSelector.getHandle(i);
         event_mask = mSelector.getOut(h);

         if ( 0 != event_mask )
         {
            ready_nodes.push_back(mDemuxTable[h]);
         }
      }
   }

   return ready_nodes;
}

}
