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

#ifndef _GADGET_ACCEPTOR_H
#define _GADGET_ACCEPTOR_H

#include <gadget/gadgetConfig.h>
#include <gadget/AbstractNetworkManager.h>

#include <vpr/IO/Socket/InetAddr.h>

namespace gadget
{

class GADGET_CLASS_API Acceptor
{
private:
   /**
    * Hide copy constructor and assignment operator.
    */
   Acceptor(const Acceptor&)
   {;}
   void operator=(const Acceptor&)
   {;}
public:
   /**
    * Construct an empty TCP/IP acceptor.
    */
   Acceptor( AbstractNetworkManager* network);

   /**
    * Deconstruct object and release memory.
    */
   virtual ~Acceptor();

   /**
    * Start the listening thread on the given port.
    */
   bool startListening( const int& listen_port, bool accept_anonymous );

   /**
    * Stop the listen thread.
    */
   void shutdown();
private:
   /**
    * Control Loop used to continuously accept incoming connections.
    */
   void acceptLoop( void* nullParam );
private:
   AbstractNetworkManager*      mNetworkManager;     /**< Network we are accepting connections on. */
   vpr::Thread*                 mAcceptThread;       /**< Thread that listens for incoming connections. */
   vpr::InetAddr                mListenAddr;         /**< Address to listen for incoming connections on. */
   bool                         mAcceptAnonymous;
};

} // end namespace gadget

#endif /* _GADGET_ACCEPTOR_H */
