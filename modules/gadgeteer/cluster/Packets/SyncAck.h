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

#ifndef _GADGET_RIM_SYNC_ACK_H
#define _GADGET_RIM_SYNC_ACK_H

#include <gadget/gadgetConfig.h>

#include <vpr/vprTypes.h>
#include <vpr/IO/BufferObjectReader.h>
#include <vpr/IO/BufferObjectWriter.h>
#include <vpr/IO/Socket/SocketStream.h>

#include <cluster/Packets/Header.h>
#include <cluster/Packets/Packet.h>

namespace cluster
{

/** \class SyncAck SyncAck.h cluster/Packets/SyncAck.h
 *
 * Synchronization acknowledgement packet.
 */
class GADGET_CLASS_API SyncAck : public Packet
{
public:
   SyncAck()
   {;}

   /**
    * Create a ApplicationDataAck packet to acknowledge a
    * ApplicationDataRequest.
    *
    * @param host_name The hostname of the node acknowledging the SyncRequest.
    * @param port      The port of the node acknowledging the SyncRequest.
    * @param ack       Boolean determining if this is a positive (ACK) or a
    *                  negative (NACK) responce.
    */
   SyncAck(const std::string& host_name, const vpr::Uint16& port,
           const bool ack);

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
      return(Header::RIM_SYNC_ACK);
   }

   /**
    * Returns the hostname of the node acknowledging the SyncRequest.
    */
   std::string getHostname()
   {
      return mHostname;
   }

   /**
    * Returns the port of the node acknowledging the SyncRequest.
    */
   vpr::Uint16 getPort()
   {
      return mPort;
   }

   /**
    * Returns a boolean determining if this is a positive(ACK) or a
    * negative(NACK) responce.
    */
   bool getAck()
   {
      return mAck;
   }

private:
   std::string mHostname;  /**< Hostname of the node acknowledging the SyncRequest. */
   vpr::Uint16 mPort;      /**< Listening port of the node acknowledging the SyncRequest. */
   bool        mAck;       /**< Boolean determining if this is a positive(ACK) or a negative(NACK) responce. */
};

}


#endif
