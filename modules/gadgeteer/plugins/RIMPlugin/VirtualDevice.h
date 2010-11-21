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
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

#ifndef CLUSTER_VIRTUAL_DEVICE_H
#define CLUSTER_VIRTUAL_DEVICE_H

#include <cluster/PluginConfig.h>
#include <boost/noncopyable.hpp>
#include <gadget/Util/Debug.h>
#include <gadget/Type/Input.h>
#include <plugins/RIMPlugin/VirtualDevicePtr.h>

namespace cluster
{

/** \class VirtualDevice VirtualDevice.h gadget/VirtualDevice.h
 *
 * Virtual input device type.
 */
class GADGET_CLUSTER_PLUGIN_CLASS_API VirtualDevice
   : boost::noncopyable
{
protected:
   /**
    * Construct a new "virtual" device that represents a remote input device.
    */
   VirtualDevice(const std::string& name, const vpr::GUID& localId,
                 const std::string& baseType, const std::string& hostname,
                 gadget::InputPtr device);

public:
   /**
    * Deconstructor.
    */
   virtual ~VirtualDevice();

   /**
    * Creates a VirtualDevice instance and returns it wrapped in a
    * VirtualDevicePtr object.
    *
    * @since 1.3.7
    */
   static VirtualDevicePtr create(const std::string& name, const vpr::GUID& id,
                                  const std::string& baseType,
                                  const std::string& hostname,
                                  gadget::InputPtr device);


   /**
    * Print debug information.
    */
   void debugDump(int debug_level);

   /**
    * Get the name of the remote device that we represent.
    */
   std::string getName() const
   {
      return mName;
   }

   /**
    * Return the hostname for the machine that is connected to the real device.
    */
   std::string getRemoteHostname() const
   {
      return mRemoteHostname;
   }

   /**
    * Get the local id used to identify this device.
    */
   vpr::GUID getLocalId() const
   {
      return mId;
   }

   /**
    * Return a pointer to the low level input device.
    */
   gadget::InputPtr getDevice() const
   {
      return mDevice;
   }

private:
   std::string          mName;                     /**< VirtualDevice name */
   std::string          mBaseType;
   std::string          mRemoteHostname;
   vpr::GUID            mId;
   gadget::InputPtr     mDevice;
};

} // end namespace cluster

#endif
