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
 * -----------------------------------------------------------------
 * File:          $RCSfile$
 * Date modified: $Date$
 * Version:       $Revision$
 * -----------------------------------------------------------------
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

#ifndef _GADGET_RIM_PACKET_H
#define _GADGET_RIM_PACKET_H

#include <vpr/vprTypes.h>
#include <vpr/Util/ReturnStatus.h>

#include <vpr/IO/BufferObjectReader.h>
#include <vpr/IO/BufferObjectWriter.h>
#include <vpr/IO/Socket/SocketStream.h>

#include <gadget/gadgetConfig.h>
#include <cluster/ClusterException.h>

namespace cluster
{
   class Header;
   class ClusterNode;

class Packet
{
public:
   /**
    * Create a new empty packet
    */
   Packet();
   
   /**
    * Copy the given data, and parse the header 
    */
   Packet(std::vector<vpr::Uint8>* data);
   
   /**
    * Directly read the needed header data from socket(blocking), and parse the header 
    */
   Packet(vpr::SocketStream* stream);

   virtual ~Packet();
   
   void recv(Header* packet_head, vpr::SocketStream* stream) throw(cluster::ClusterException);
   virtual vpr::ReturnStatus send(vpr::SocketStream* socket);
   void dump();
   
   virtual bool action(ClusterNode* node) = 0;
   virtual void printData(int debug_level) = 0;

protected:
   Header* mHeader;
   
   // Needed by all derived classes
   vpr::BufferObjectReader* mPacketReader;	
   vpr::BufferObjectWriter* mPacketWriter;
   std::vector<vpr::Uint8> mData;
};
}

#endif
