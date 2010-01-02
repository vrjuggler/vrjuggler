/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2010 by Iowa State University
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

#ifndef _GADGET_RIM_DEVICE_ACK_H
#define _GADGET_RIM_DEVICE_ACK_H

#include <gadget/gadgetConfig.h>

#include <vpr/vprTypes.h>
#include <vpr/IO/BufferObjectReader.h>
#include <vpr/IO/BufferObjectWriter.h>
#include <vpr/IO/Socket/SocketStream.h>

#include <cluster/Packets/Header.h>
#include <cluster/Packets/Packet.h>
#include <cluster/Packets/DeviceAckPtr.h>

namespace cluster
{

/** \class DeviceAck DeviceAck.h cluster/Packets/DeviceAck.h
 *
 * Device acknowledgement packet.
 */
class GADGET_CLASS_API DeviceAck
   : public Packet
{
protected:
   DeviceAck();

   /**
    * Create a DeviceAck packet to acknowledge a ApplicationDataRequest.
    *
    * @param plugin_id        GUID of the ClusterPlugin that should handle this
    *                         packet.
    * @param id               GUID of the Device that we are acknowledging.
    * @param device_name      Name of the device that we are acknowledging.
    * @param device_base_type Basetype of the device that we are acknowledging.
    * @param ack              Boolean determining if this is a positive (ACK)
    *                         or a negative (NACK) responce.
    */
   DeviceAck(const vpr::GUID& pluginId, const vpr::GUID& id,
             const std::string& deviceName,
             const std::string& deviceBaseType, bool ack);

public:
   /**
    * Creates a DeviceAck instance and returns it wrapped in a
    * DeviceAckPtr object.
    *
    * @since 1.3.7
    */
   static DeviceAckPtr create();

   /**
    * Creates a DeviceAck instance and returns it wrapped in a
    * DeviceAckPtr object.
    *
    * @since 1.3.7
    */
   static DeviceAckPtr create(const vpr::GUID& pluginId, const vpr::GUID& id,
                              const std::string& deviceName,
                              const std::string& deviceBaseType, bool ack);

   virtual ~DeviceAck();

   /**
    * Serializes member variables into a data stream.
    */
   void serialize();

   /**
    * Parses the data stream into the local member variables.
    */
   virtual void parse();

   /**
    * Print the data to the screen in a readable form.
    */
   virtual void printData(int debugLevel) const;

   /**
    * Return the type of this packet.
    */
   static vpr::Uint16 getPacketFactoryType()
   {
      return(Header::RIM_DEVICE_ACK);
   }

   /**
    * Return the GUID of the Device that we are acknowledging.
    */
   vpr::GUID getId()
   {
      return mId;
   }

   /**
    * Return the name of the device that we are acknowledging.
    */
   std::string getDeviceName()
   {
      return mDeviceName;
   }

   /**
    * Return the basetype of the device that we are acknowledging.
    */
   std::string getDeviceBaseType()
   {
      return mDeviceBaseType;
   }

   /**
    * Return the hostname of the node the device is located on.
    */
   std::string getHostname()
   {
      return mHostname;
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
   vpr::GUID   mId;              /**< GUID of the Device that we are acknowledging. */
   std::string mDeviceName;      /**< Name of the device that we are acknowledging. */
   std::string mDeviceBaseType;  /**< Basetype of the device that we are acknowledging. */
   std::string mHostname;        /**< The hostname of the node that the device is on. */
   bool        mAck;             /**< Boolean determining if this is a positive(ACK) or a negative(NACK) responce. */
};
}

#endif




