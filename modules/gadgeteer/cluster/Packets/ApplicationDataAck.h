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

#ifndef _GADGET_USER_DATA_ACK_H
#define _GADGET_USER_DATA_ACK_H

#include <gadget/gadgetConfig.h>

#include <vpr/vprTypes.h>
#include <vpr/IO/BufferObjectReader.h>
#include <vpr/IO/BufferObjectWriter.h>
#include <vpr/IO/Socket/SocketStream.h>

#include <cluster/Packets/Header.h>
#include <cluster/Packets/Packet.h>

namespace cluster
{

/** \class ApplicationDataAck ApplicationDataAck.h cluster/Packets/ApplicationDataAck.h
 *
 * Application-specific data acknowledgement packet.
 */
class GADGET_CLASS_API ApplicationDataAck : public Packet
{
public:
   ApplicationDataAck()
   {;}

   /**
    * Create a ApplicationDataAck packet to acknowledge a
    * ApplicationDataRequest.
    *
    * @param plugin_guid GUID of the ClusterPlugin that should handle this
    *                    packet.
    * @param id          GUID of the ApplicationData object that we are
    *                    acknowledging.
    * @param ack         Boolean determining if this is a positive (ACK) or a
    *                    negative (NACK) responce.
    */
   ApplicationDataAck(const vpr::GUID& plugin_guid,
                      const vpr::GUID& id,
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
       return(Header::RIM_APPDATA_ACK);
   }

   /**
    * Return the GUID of the ApplicationData object being acknoledged
    */
   vpr::GUID getId() { return mId; }

   /**
    * Return a boolean determining if this is a positive(ACK) or a
    * negative(NACK) responce.
    */
   bool getAck()
   {
      return mAck;
   }

private:
   vpr::GUID   mId;  /**< GUID for the ApplicationData object being acknoledged. */
   bool        mAck; /**< Boolean determining if this is a positive(ACK) or a negative(NACK) responce. */
};

}

#endif
