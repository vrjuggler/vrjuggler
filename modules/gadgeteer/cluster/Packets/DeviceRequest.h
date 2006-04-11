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

#ifndef _GADGET_RIM_DEVICE_REQUEST_H
#define _GADGET_RIM_DEVICE_REQUEST_H

#include <gadget/gadgetConfig.h>

#include <vpr/vprTypes.h>
#include <vpr/IO/BufferObjectReader.h>
#include <vpr/IO/BufferObjectWriter.h>
#include <vpr/IO/Socket/SocketStream.h>

#include <cluster/Packets/Header.h>                                                       
#include <cluster/Packets/Packet.h>

namespace cluster
{

class GADGET_CLASS_API DeviceRequest : public Packet
{
public:
   DeviceRequest()
   {;}

   /**
    * Create a DeviceRequest packet to request a remote device.
    *
    * @param plugin_guid -GUID of the ClusterPlugin that should handle this packet.
    * @param device_name -Name of the remote device being requested.    
    */
   DeviceRequest(const vpr::GUID& plugin_guid, const std::string& device_name);
   
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
       return(Header::RIM_DEVICE_REQ);
   }

   /**
    * Return the name of the remote device being requested.
    */
   std::string getDeviceName() { return mDeviceName; }
private:
   std::string mDeviceName;   /**< The name of the remote device being requested */
};
}

#endif




