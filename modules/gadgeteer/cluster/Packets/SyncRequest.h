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

#ifndef _GADGET_RIM_SYNC_REQUEST_H
#define _GADGET_RIM_SYNC_REQUEST_H

#include <gadget/gadgetConfig.h>

#include <vpr/vprTypes.h>
#include <vpr/IO/BufferObjectReader.h>
#include <vpr/IO/BufferObjectWriter.h>
#include <vpr/IO/Socket/SocketStream.h>

#include <cluster/Packets/Header.h>
#include <cluster/Packets/Packet.h>

namespace cluster
{

/** \class SyncRequest SyncRequest.h cluster/Packets/SyncRequest.h
 *
 * Synchronization request packet.
 */
class GADGET_CLASS_API SyncRequest : public Packet
{
public:
   SyncRequest()
   {;}

   /**
    * Create a SyncRequest packet to request syncronization.
    *
    * @param host_name Hostname of the node requesting syncronization.
    * @param port      Listening port of the requesting machine.
    */
   SyncRequest(std::string& host_name, vpr::Uint16& port);

   /**
    * Serializes member variables into a data stream.
    */
   void serialize();

   /**
    * Parses the data stream into the local member variables.
    */
   virtual void parse(vpr::BufferObjectReader* reader);

   /**
    * Print the data to the screen in a readable form.
    */
   virtual void printData(int debug_level);

   /**
    * Return the type of this packet.
    */
   static vpr::Uint16 getPacketFactoryType()
   {
      return(Header::RIM_SYNC_REQ);
   }

   /**
    * Return the hostname of the node requesting syncronization.
    */
   std::string getHostname()
   {
      return mHostname;
   }

   /**
    * Return the listening port of the requesting machine.
    */
   vpr::Uint16 getPort()
   {
      return mPort;
   }

private:
   std::string mHostname;  /**< Hostname of the node requesting syncronization. */
   vpr::Uint16 mPort;      /**< Listening port of the requesting machine. */
};

}

#endif
