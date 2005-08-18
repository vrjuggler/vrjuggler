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
 * Date modified: $Date$
 * Version:       $Revision$
 * -----------------------------------------------------------------
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

#include <gadget/gadgetConfig.h>
#include <gadget/Connector.h>
#include <gadget/Node.h>

#include <vpr/IO/Socket/SocketStream.h>
#include <vpr/Util/ReturnStatus.h>

#include <cluster/Packets/Packet.h>
#include <cluster/Packets/ConnectionAck.h>

namespace gadget
{

vpr::ReturnStatus Connector::attemptConnect( Node* node )
{
   // - Try to connect to remote host
   // - If successful
   //
   //   - Lock pending list
   //     - If hostname exists in pending list
   //       - Remove Node from pending list
   //   - Unlock pending list
   //
   //   - If Node not connected
   //     - Set socket stream
   //     - Set NEWCONNECTION
   
   vprASSERT( Node::PENDING == node->getStatus() &&
              "Can not connect to a node that is not pending." );

   vpr::SocketStream* sock_stream;
   vpr::InetAddr inet_addr;

   vprDEBUG( gadgetDBG_NET_MGR, vprDBG_VERB_LVL )
      << clrOutBOLD( clrBLUE, "[Connector]" )
      << " Attempting to connect to: " << node->getName()
      << std::endl << vprDEBUG_FLUSH;
      
   try
   {
      // Set the address that we want to connect to
      inet_addr.setAddress( node->getHostname(), node->getPort() );
   }
   catch (vpr::IOException& ex)
   {
      vprDEBUG( gadgetDBG_NET_MGR, vprDBG_CRITICAL_LVL )
         << clrOutBOLD( clrBLUE, "[Connector]" )
         << clrOutBOLD( clrRED, " ERROR:" )
         << " Failed to set address" << std::endl << vprDEBUG_FLUSH;
      return vpr::ReturnStatus::Fail;
   }

   // Create a new socket stream to this address
   sock_stream = new vpr::SocketStream( vpr::InetAddr::AnyAddr, inet_addr );

   try
   {
      // If we can successfully open the socket and connect to the server
      sock_stream->open();
      sock_stream->connect();

      vprDEBUG( gadgetDBG_NET_MGR, vprDBG_CONFIG_STATUS_LVL )
         << clrOutBOLD( clrBLUE, "[Connector]" )
         << " Successfully connected to: "
         << node->getHostname() <<":"<< node->getPort()
         << std::endl << vprDEBUG_FLUSH;
         
      sock_stream->setNoDelay( true );
      vpr::SocketStream* old_stream = node->getSockStream();
      node->setSockStream( sock_stream );

      vprDEBUG( gadgetDBG_NET_MGR, vprDBG_CONFIG_STATUS_LVL )
         << clrOutBOLD( clrBLUE, "[Connector]" )
         << " Waiting for connection ACK/NACK"
         << std::endl << vprDEBUG_FLUSH;
      
      cluster::Packet* temp_packet = node->recvPacket();
      cluster::ConnectionAck* ack_packet = dynamic_cast<cluster::ConnectionAck*>( temp_packet );
      node->setSockStream( old_stream );

      vprDEBUG( gadgetDBG_NET_MGR, vprDBG_CONFIG_STATUS_LVL )
         << clrOutBOLD( clrBLUE,"[Connector]" )
         << " Recieved" << ( ack_packet->getAck() ? " an Ack" : " a NACK" )
         << std::endl << vprDEBUG_FLUSH;
     
      vprASSERT( NULL != ack_packet && "Dynamic cast failed, must not be a ConnectionAck packet." );
      
      ack_packet->printData( vprDBG_CONFIG_STATUS_LVL );
      
      if ( ack_packet->getAck() )
      {
         //   - Lock pending list
         //     - If hostname exists in pending list
         //       - Remove Node from pending list
         //   - Unlock pending list
      
         vprASSERT( Node::PENDING == node->getStatus() && "Trying to connect to a node that is not pending" );
         node->setStatus( Node::NEWCONNECTION );

         //   - If Node not connected
         //     - Set socket stream
         //     - Set NEWCONNECTION

         vprASSERT( Node::NEWCONNECTION == node->getStatus() && "Should be NEWCONNECTION." );
         
         node->setSockStream( sock_stream );
         node->setStatus( Node::NEWCONNECTION );

         // XXX: We need to fix this in the near future.
         //ClusterDelta cluster_delta;
         //vpr::Interval temp;
         //temp = cluster_delta.getClusterDelta( getSockStream() );
         //mDelta = temp.getBaseVal();
      }
      else
      {
         node->setSockStream( NULL );
         delete sock_stream; 
         return vpr::ReturnStatus::Fail;
      }
   }
   catch (vpr::IOException& ex)
   {
      delete sock_stream;
      
      vprDEBUG( gadgetDBG_NET_MGR, vprDBG_VERB_LVL )
         << clrOutBOLD( clrBLUE, "[Connector]" )
         << clrOutBOLD( clrRED, " ERROR:" )
         << " Could not connect to Node: "
         << node->getHostname() << " : " << node->getPort()
         << std::endl << vprDEBUG_FLUSH;
         
      return vpr::ReturnStatus::Fail;
   }
   
   return vpr::ReturnStatus::Succeed;
}

} // end namespace gadget
