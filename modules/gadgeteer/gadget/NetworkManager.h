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

#ifndef _GADGET_NETWORK_MANAGER_H
#define _GADGET_NETWORK_MANAGER_H

#include <gadget/gadgetConfig.h>

#include <gadget/AbstractNetworkManager.h>
#include <gadget/Util/Debug.h>


namespace gadget
{

/** \class NetworkManager NetworkManager.h gadget/NetworkManager.h
 *
 * Network manager.  This is an implementation of
 * gadget::AbstractNetworkManager.
 */
template<typename A, typename C>
class NetworkManager : public AbstractNetworkManager
{
private:
   void operator= (const NetworkManager& na)
   {;}
public:
   NetworkManager()
      : AbstractNetworkManager(),
        mAcceptor(this)
   {;}
   virtual bool attemptConnect(Node* node)
   {
      return mConnector.attemptConnect(node);
   }
   virtual void startListening(int listen_port, bool accept_anonymous)
   {
      mAcceptor.startListening(listen_port, accept_anonymous);
   }
private:
   A mAcceptor;
   C mConnector;
};

} // End namespace gadget

#endif /* _GADGET_NETWORK_MANAGER_H */
