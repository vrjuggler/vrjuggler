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
 * -----------------------------------------------------------------
 * File:          $RCSfile$
 * Date modified: $Date$
 * Version:       $Revision$
 * -----------------------------------------------------------------
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

#include <gadget/gadgetConfig.h>
#include <gadget/Acceptor.h>
#include <gadget/Node.h>
#include <gadget/Util/Debug.h>

#include <cluster/Packets/ConnectionAck.h>
#include <cluster/Packets/Packet.h>
#include <cluster/ClusterDelta.h>

#include <vpr/IO/Socket/SocketStream.h>

#include <boost/concept_check.hpp>

namespace gadget
{
   Acceptor::Acceptor( AbstractNetworkManager* network)
      : mNetworkManager( network ), mAcceptThread( NULL )
   {;}

   Acceptor::~Acceptor()
   {
      shutdown();
   }

   bool Acceptor::startListening( const int& listen_port, bool accept_anonymous )
   {
      mAcceptAnonymous = accept_anonymous;

      // If we haven't already started the listening thread
      if ( mAcceptThread == NULL )
      {
         // If the listen port is valid
         if ( listen_port > 0 )
         {
            mListenAddr.setPort( listen_port );

            vprDEBUG( gadgetDBG_NET_MGR, vprDBG_CONFIG_LVL )
               << clrOutBOLD( clrMAGENTA, "[Acceptor]" )
               << "Starting the listening thread...\n" << vprDEBUG_FLUSH;

               // Start a thread to monitor port
            vpr::ThreadMemberFunctor<Acceptor>* memberFunctor =
               new vpr::ThreadMemberFunctor<Acceptor>( this, &Acceptor::acceptLoop, NULL );

            mAcceptThread = new vpr::Thread( memberFunctor );
            vprASSERT( mAcceptThread->valid() );
            return true;
         }
         else
         {
            vprDEBUG( gadgetDBG_NET_MGR, vprDBG_CONFIG_LVL )
               << clrOutBOLD( clrMAGENTA, "[Acceptor]" )
               << "startListening() Can NOT listen on port: "
               << listen_port << "\n" << vprDEBUG_FLUSH;
            return false;
         }
      }
      else
      {
         vprDEBUG( gadgetDBG_NET_MGR, vprDBG_CONFIG_LVL )
               << clrOutBOLD( clrMAGENTA,"[Acceptor]" )
               << "startListening() Listening thread already active."
               << std::endl << vprDEBUG_FLUSH;
         return false;
      }
   }

   void Acceptor::acceptLoop( void* nullParam )
   {
      
      // - Create a socket to listen for incoming connections
      // - Wait for new connections
      //   - If successful
      //     - Lock Node list
      //       - If hostname does not exist in list
      //         - Add new Node
      //     - Unlock Node list
      //         
      //     - Lock pending list
      //       - If hostname exists in pending list
      //         - Remove Node from pending list
      //     - Unlock pending list
      //
      //     - Lock Node list
      //       - If Node is not connected
      //         - Set socket stream
      //         - Set NEWCONNECTION
      //     - Unlock Node list
      //
      //     - Get Cluster Delta
      //   - Else close and delete socket
      
      boost::ignore_unused_variable_warning( nullParam );

      // Create a socket to listen for incoming connections
      vpr::SocketStream sock( mListenAddr, vpr::InetAddr::AnyAddr );
      
      // Open in server mode.
      try
      {
         // Open server enabling reuse of bound address.
         sock.openServer(true);

         vprDEBUG( gadgetDBG_NET_MGR, vprDBG_CONFIG_LVL )
         << clrOutBOLD( clrMAGENTA, "[Acceptor]" )
         << " Listening on Port: " << mListenAddr.getPort()
         << std::endl << vprDEBUG_FLUSH;
      }
      catch (vpr::IOException&)
      {
         vprDEBUG( gadgetDBG_NET_MGR, vprDBG_CRITICAL_LVL )
            << clrSetBOLD( clrRED ) << "[Acceptor]"
            << " Unable to open listening socket on port: "
            << mListenAddr.getPort() << std::endl
            << clrRESET << vprDEBUG_FLUSH;

         exit(0);
      }

      // Create a socketstream for new socket
      vpr::SocketStream* client_sock = new vpr::SocketStream();

      // Wait for an incoming connection.
      bool mRunning = true;

      while ( mRunning )
      {
         try
         {
            // Wait for a connection request.
            sock.accept( *client_sock, vpr::Interval::NoTimeout );

            vprDEBUG(gadgetDBG_NET_MGR, vprDBG_CONFIG_LVL)
               << clrOutBOLD(clrMAGENTA, "[Acceptor]")
               << " Receiving a connection on Port: "
               << mListenAddr.getPort() << std::endl << vprDEBUG_FLUSH;

            // Optimize new socket for low latency communication
            client_sock->setNoDelay( true );

            // Get the hostname and port of the requesting host.
            const std::string remote_host_name(
               client_sock->getRemoteAddr().getHostname()
            );
            vpr::Uint16 port = client_sock->getRemoteAddr().getPort();

            vprDEBUG( gadgetDBG_NET_MGR, vprDBG_CONFIG_LVL )
               << clrOutBOLD( clrMAGENTA, "[Acceptor]" )
               << " Received from: " << remote_host_name
               << ":" << port << std::endl << vprDEBUG_FLUSH;

            Node* remote_node = mNetworkManager->getNodeByHostname( remote_host_name );

            if ( NULL == remote_node )
            {
               vprDEBUG( gadgetDBG_NET_MGR,vprDBG_VERB_LVL )
                  << clrOutBOLD( clrMAGENTA,"[Acceptor]" )
                  << " Nodes not configured yet."
                  << std::endl << vprDEBUG_FLUSH;
               
               client_sock->close();
               delete client_sock;
               client_sock = new vpr::SocketStream;
            }
            else
            {
               // Get address information about local host.
               const vpr::InetAddr local = vpr::InetAddr::getLocalHost();
               const std::string local_hostname = local.getHostname();
               cluster::ConnectionAck* temp = NULL;

               if( remote_node->getStatus() == Node::CONNECTED || 
                   remote_node->getStatus() == Node::NEWCONNECTION )
               {
                  vprDEBUG( gadgetDBG_NET_MGR,vprDBG_STATE_LVL )
                     << clrOutBOLD( clrMAGENTA, "[Acceptor]" )
                     << " Node is already connected, no need to respond to request." 
                     << std::endl << vprDEBUG_FLUSH;

                  temp = new cluster::ConnectionAck( local_hostname, mListenAddr.getPort(), false );
               }
               else if ( Node::PENDING == remote_node->getStatus() )
               {
                  vprASSERT( NULL != remote_node && "Remote node must nut be equal to NULL" );
               
                  vprDEBUG( gadgetDBG_NET_MGR, vprDBG_STATE_LVL )
                     << clrOutBOLD( clrMAGENTA, "[Acceptor]")
                     << " Pending Node exists, we must decide which"
                     << " side should initiate the connection."
                     << std::endl << vprDEBUG_FLUSH;
                  
                  // Get address values to test.
                  vpr::Uint32 remote_value = client_sock->getRemoteAddr().getAddressValue();
                  vpr::Uint32 local_value = local.getAddressValue();
                  
                  vprDEBUG( gadgetDBG_NET_MGR, vprDBG_STATE_LVL )
                     << "Remote: " << remote_value
                     << std::endl << vprDEBUG_FLUSH;
                  
                  vprDEBUG( gadgetDBG_NET_MGR, vprDBG_STATE_LVL )
                     << "Local: " << local_value
                     << std::endl << vprDEBUG_FLUSH;

                  // If Node has an address value less than mine
                  if ( remote_value < local_value )
                  {
                     vprDEBUG( gadgetDBG_NET_MGR, vprDBG_STATE_LVL )
                        << "Result: (remote address < local address)"
                        << " Create NACK" << std::endl << vprDEBUG_FLUSH;
                     
                     // Create NACK
                     temp = new cluster::ConnectionAck( local_hostname, mListenAddr.getPort(), false );
                  }
                  else
                  {
                     vprDEBUG( gadgetDBG_NET_MGR, vprDBG_STATE_LVL )
                        << "Result: (remote address >= local address)"
                        << "Create ACK" << std::endl << vprDEBUG_FLUSH;

                     temp = new cluster::ConnectionAck( local_hostname, mListenAddr.getPort(), true );
                 }
               }
               else
               {
                  vprDEBUG( gadgetDBG_NET_MGR, vprDBG_STATE_LVL )
                     << clrOutBOLD( clrMAGENTA, "[Acceptor]" )
                     << " Node is not pending, create ACK."
                     << std::endl << vprDEBUG_FLUSH;
                     
                  // Create an ACK since we do not depend on it
                  temp = new cluster::ConnectionAck( local_hostname, mListenAddr.getPort(), true );
               }
                  
               vprDEBUG( gadgetDBG_NET_MGR, vprDBG_STATE_LVL )
                  << clrOutBOLD( clrMAGENTA,"[Acceptor]" )
                  << " Set SockStream and send responce."
                  << std::endl << vprDEBUG_FLUSH;
               
               vpr::SocketStream* old_stream = remote_node->getSockStream();
               remote_node->setSockStream( client_sock );
               remote_node->send( temp );
               remote_node->setSockStream( old_stream );

               if ( temp->getAck() )
               {
                  vprDEBUG( gadgetDBG_NET_MGR,vprDBG_STATE_LVL )
                     << clrOutBOLD( clrMAGENTA,"[Acceptor]" )
                     << " Set new Node as a NEWCONNECTION."
                     << std::endl << vprDEBUG_FLUSH;

                  remote_node->setSockStream( client_sock );
                  
                  // Since we have just recieved a new connection,
                  // set the connected status as so. We are not
                  // in a fully connected state until the begining
                  // of the next frame in Acceptor::updateNewConnetions()
                  // this is becuase we only want to start using a
                  // new connection at the start of a new frame.
                  remote_node->setStatus( Node::NEWCONNECTION );
                  // Print the new state information about this node.
                  remote_node->debugDump( vprDBG_CONFIG_LVL );
                  
                  // XXX: We need to fix this in the near future.
                  //ClusterDelta cluster_delta;
                  //cluster_delta.clientClusterDelta(requesting_node->getSockStream());
               }
               else
               {
                  vprDEBUG( gadgetDBG_NET_MGR,vprDBG_STATE_LVL )
                     << clrOutBOLD( clrMAGENTA,"[Acceptor]" )
                     << " Delete unused client sock."
                     << std::endl << vprDEBUG_FLUSH;
                  
                  client_sock->close();
                  remote_node->setSockStream( NULL );
                  delete client_sock;
               }

               // We need to create a new SocketStream since the to
               // hold the value of the next recieved socketstream since
               // the old one is now being used by the new Node
               client_sock = new vpr::SocketStream;
            }
         }
         catch (vpr::IOException&)
         {
            // Should never happen since timeout is infinite
            client_sock->close();
            delete client_sock;
            client_sock = new vpr::SocketStream;
         }
      }   // end infinite while
   }

   void Acceptor::shutdown()
   {
      // Kill thread used to listen for incoming
      // connection requests

      // TODO: Make this actually shutdown the Accepting thread, this will require
      //       non blocking accept calls.
      if ( mAcceptThread )
      {
         mAcceptThread->kill();
         mAcceptThread = NULL;
      }
   }
} // end namespace gadget
