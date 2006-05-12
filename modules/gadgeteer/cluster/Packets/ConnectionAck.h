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

#ifndef _GADGET_RIM_CONNECTION_ACK_H
#define _GADGET_RIM_CONNECTION_ACK_H

#include <gadget/gadgetConfig.h>

#include <vpr/vprTypes.h>
#include <vpr/IO/BufferObjectReader.h>
#include <vpr/IO/BufferObjectWriter.h>
#include <vpr/IO/Socket/SocketStream.h>

#include <cluster/Packets/Header.h>
#include <cluster/Packets/Packet.h>

namespace cluster
{
   /** \class ConnectionAck ConnectionAck.h cluster/Packets/ConnectionAck.h
    *
    * Connection acknowledgement packet.
    */
   class GADGET_CLASS_API ConnectionAck : public Packet
   {
   public:
      ConnectionAck()
      {;}

      /**
       * Create a ConnectionAck packet to acknowledge a ConnectionRequest.
       *
       * @param host_name The hostname of the machine that is acknowledging
       *                  the connection.
       * @param port      The port that the acknowledging machine is listening
       *                  on.
       * @param ack       Boolean determining if this is a positive (ACK) or a
       *                  negative (NACK) responce.
       */
      ConnectionAck(std::string host_name, vpr::Uint16 port, bool ack);

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
         return(Header::RIM_CONNECTION_ACK);
      }

      /**
       * Return the hostname of the machine that is acknowledging the
       * connection.
       */
      std::string getHostname()
      {
         return mHostname;
      }

      /**
       * Return the port that the acknowledging machine is listening on.
       */
      vpr::Uint16 getPort()
      {
         return mPort;
      }

      /**
       * Return a boolean determining if this is a positive(ACK) or a
       * negative(NACK) responce.
       */
      bool getAck()
      {
         return mAck;
      }

   private:
      std::string mHostname;  /**< The hostname of the machine that is acknowledging the connection. */
      vpr::Uint16 mPort;      /**< The port that the acknowledging machine is listening on. */
      bool        mAck;       /**< Boolean determining if this is a positive(ACK) or a negative(NACK) responce. */
   };
}


#endif
