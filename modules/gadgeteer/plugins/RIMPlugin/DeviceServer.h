/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2007 by Iowa State University
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

#ifndef _CLUSTER_DEVICE_SERVER_H
#define _CLUSTER_DEVICE_SERVER_H

#include <cluster/PluginConfig.h>

#include <boost/noncopyable.hpp>
#include <gadget/Type/InputPtr.h>
#include <cluster/Packets/DataPacketPtr.h>
#include <plugins/RIMPlugin/DeviceServerPtr.h>

namespace vpr
{
   class BufferObjectWriter;
}

namespace cluster
{
   class Node;
   
/** \class DeviceServer DeviceServer.h gadget/DeviceServer.h
 *
 * Device server class.
 */
class GADGET_CLUSTER_PLUGIN_CLASS_API DeviceServer
   : boost::noncopyable
{
protected:
   /**
    * Create a new DeviceServer.
    *
    * @param name      Name of the device that we are sharing.
    * @param device    Pointer to the device that we are sharing.
    * @param plugin_id GUID that should be placed at the beginning of 
    *                  each data packet so that the receiver knows which 
    *                  plugin the data is coming from.
    */
   DeviceServer(const std::string& name, gadget::InputPtr device,
                const vpr::GUID& pluginId);

public:
   /**
    * Creates a DeviceServer instance and returns it wrapped in a
    * DeviceServerPtr object.
    *
    * @since 1.3.7
    */
   static DeviceServerPtr create(const std::string& name, gadget::InputPtr device,
                                 const vpr::GUID& pluginGuid);
   /**
    */
   virtual ~DeviceServer();

   /**
    */
   void send() const;

   /**
    */
   void updateLocalData();

   /**
    */
   void debugDump(int debugLevel) const;

   /**
    */
   std::string getName() const
   {
      return mName;
   }

   /**
    */
   vpr::GUID getId() const
   {
      return mId;
   }

private:
   std::string                         mName;   /**< DeviceServer name */
   vpr::GUID                           mId;                    /**< GUID for shared device */
   vpr::GUID                           mPluginGUID;
   
   gadget::InputPtr                    mDevice;
   cluster::DataPacketPtr              mDataPacket;
};

} // end namespace cluster

#endif /*_CLUSTER_DEVICE_SERVER_H*/
