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

#ifndef _GADGET_Packet_FACTORY_H_
#define _GADGET_Packet_FACTORY_H_
//#pragma once

#include <gadget/gadgetConfig.h>
//#include <gadget/Type/Input.h>
#include <cluster/Packets/Packet.h>
//#include <jccl/Config/ConfigChunkPtr.h>
#include <vpr/Util/Singleton.h>

#include <vpr/Util/Debug.h>
#include <vpr/Util/Assert.h>

namespace cluster
{

/**
 * Base class for virtual construction of devices.
 * Implementations of this class are registered with the device factory
 * for each device in the system.
 */
class PacketConstructorBase
{
public:
   /**
    * Constructor.
    * @post Device is registered.
    */
   PacketConstructorBase() {;}

   /** Creates the device. */
   virtual Packet* createPacket(Header* packet_head, vpr::SocketStream* stream)
   {
      vprDEBUG(vprDBG_ALL,0) << "ERROR: DeviceConstructorBase::createDevice: Should never be called" << vprDEBUG_FLUSH;
      return NULL;
   }

   /** Gets the string desc of the type of chunk we can create. */
   virtual vpr::Uint16  getBaseType()
   { return 0; }
};


/**
 * Object used for creating devices.
 * @note Singleton
 */
class GADGET_CLASS_API PacketFactory
{
private:
   // Singleton so must be private
   PacketFactory()
   {
      mConstructors = std::vector<PacketConstructorBase*>(0);
      vprASSERT(mConstructors.size() == 0);
   }

   // This should be replaced with device plugins.
   /**
    * @post Devices are loaded that the system knows about.
    */
   void loadKnownPackets();

public:
   void registerPacket(PacketConstructorBase* constructor);

   /**
    * Queries if the factory knows about the given device.
    * @pre chunk != NULL, chunk is a valid chunk.
    * @param chunk The chunk we are requesting about knowledge to create.
    * @return true if the factory knows how to create the device; false if not.
    */
   bool recognizePacket(vpr::Uint16 base_type);

   /**
    * Loads the specified device.
    * @pre recognizeDevice(chunk) == true
    * @param chunk The specification of the device to load.
    * @return NULL is returned if the device failed to load.
    *         Otherwise, a pointer to the loaded device is returned.
    */

   
   
   Packet* recvPacket(vpr::SocketStream* stream);

private:
   /**
    * Finds a constructor for the given device type.
    * @return -1 is returned if the constructor is not found.
    *         Otherwise, the index of the constructor is returned.
    */
   int   findConstructor(vpr::Uint16 base_type);

   void debugDump();


private:
   std::vector<PacketConstructorBase*> mConstructors;  /**<  List of the device constructors */

   vprSingletonHeaderWithInitFunc(PacketFactory, loadKnownDevices);
};


template <class DEV>
class PacketConstructor : public PacketConstructorBase
{
public:
   PacketConstructor()
   {
      vprASSERT(PacketFactory::instance() != NULL);
      PacketFactory::instance()->registerPacket(this);
   }

   Packet* createPacket(Header* packet_head, vpr::SocketStream* stream)
   {
      DEV* new_dev = new DEV(packet_head, stream);
      return new_dev;
   }

   virtual vpr::Uint16 getBaseType()
    { return DEV::getBaseType(); }

   /**
    * Invokes the global scope delete operator.  This is required for proper
    * releasing of memory in DLLs on Win32.
    */
   void operator delete(void* p)
   {
      ::operator delete(p);
   }

protected:
   /**
    * Deletes this object.  This is an implementation of the pure virtual
    * gadget::Input::destroy() method.
    */
   virtual void destroy()
   {
      delete this;
   }
};

} // end namespace gadget

#endif
