/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2011 by Iowa State University
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
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
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
    * @param pluginId       GUID of the ClusterPlugin that should handle this
    *                       packet.
    * @param id             GUID of the Device that we are acknowledging.
    * @param deviceName     Name of the device that we are acknowledging.
    * @param deviceBaseType Base type ID of the device that we are
    *                       acknowledging.
    * @param ack            Boolean determining if this is a positive (ACK)
    *                       or a negative (NACK) responce.
    */
   DeviceAck(const vpr::GUID& pluginId, const vpr::GUID& id,
             const std::string& deviceName, const vpr::Uint16 deviceBaseType,
             const bool ack);

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
                              const std::string& devTypeId,
                              const vpr::Uint16 deviceBaseType,
                              const bool ack);

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
      return Header::RIM_DEVICE_ACK;
   }

   /**
    * Return the GUID of the Device that we are acknowledging.
    */
   const vpr::GUID& getId() const
   {
      return mId;
   }

   /**
    * Return the name of the device that we are acknowledging.
    */
   const std::string& getDeviceName() const
   {
      return mDeviceName;
   }

   /**
    * Return the basetype of the device that we are acknowledging.
    */
   vpr::Uint16 getDeviceBaseType() const
   {
      return mDeviceBaseType;
   }

   /**
    * Return the hostname of the node the device is located on.
    */
   const std::string& getHostname() const
   {
      return mHostname;
   }

   /**
    * Return a boolean determining if this is a positive(ACK) or a
    * negative(NACK) responce.
    */
   bool getAck() const
   {
      return mAck;
   }

private:
   vpr::GUID   mId;              /**< GUID of the Device that we are acknowledging. */
   std::string mDeviceName;      /**< Name of the device that we are acknowledging. */
   vpr::Uint16 mDeviceBaseType;  /**< Base type of the device that we are acknowledging. */
   std::string mHostname;        /**< The hostname of the node that the device is on. */
   bool        mAck;             /**< Boolean determining if this is a positive(ACK) or a negative(NACK) responce. */
};
}

#endif




