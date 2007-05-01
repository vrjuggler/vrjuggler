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

#ifndef _GADGET_REACTOR_H_
#define _GADGET_REACTOR_H_

#include <gadget/gadgetConfig.h>

#include <vector>

#include <vpr/IO/Selector.h>
#include <gadget/NodePtr.h>

namespace gadget
{

class Node;

class Reactor
{
public:
   void addNode(gadget::NodePtr node);

   void removeNode(gadget::NodePtr node);

   std::vector<gadget::NodePtr> getReadyNodes(const vpr::Interval& timeout);

   vpr::Uint16 getNumHandles() const
   {
      return mSelector.getNumHandles();
   }

private:
   vpr::Selector mSelector;
   std::map<vpr::IOSys::Handle, gadget::NodePtr> mDemuxTable;
};

}


#endif /* _GADGET_REACTOR_H_ */
