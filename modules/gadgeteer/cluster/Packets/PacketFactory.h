/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2009 by Iowa State University
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

#ifndef _CLUSTER_PACKET_FACTORY_H_
#define _CLUSTER_PACKET_FACTORY_H_

#include <gadget/gadgetConfig.h>
#include <string>
#include <boost/smart_ptr.hpp>
#include <vpr/Util/Factory.h>
#include <vpr/Util/Singleton.h>
#include <cluster/Packets/Packet.h>

/**
 * Registers a creator for the Packet base class.
 *
 * @pre Requires that the method std::string getPacketFactoryType() be defined for
 *      class PacketType.
 *
 * Ex: CLUSTER_REGISTER_CLUSTER_PACKET_CREATOR(ConnectionAck)
 */
#define CLUSTER_REGISTER_CLUSTER_PACKET_CREATOR(PacketType) \
class PacketType; \
const bool reg_ctr_ ## PacketType = \
   cluster::PacketFactory::instance()-> \
      registerCreator(PacketType::getPacketFactoryType(), \
                      cluster::CreateProduct<cluster::Packet, PacketType>);

namespace cluster
{

/** \struct NullFactoryError PacketFactory.h cluster/Packets/PacketFactory.h
 *
 * @param IdentifierType  The factory identifier type.
 * @param AbstractProduct The type of the product created by the factory.
 */
template<class IdentifierType, class AbstractProduct>
struct NullFactoryError
{
   static boost::shared_ptr<AbstractProduct> onUnknownType(IdentifierType)
   {
      return boost::shared_ptr<AbstractProduct>();
   }
};

/**
 * Implements a useful little template function usable as a Creator in factory.
 */
template<class AbstractProduct, class ConcreteProduct>
boost::shared_ptr<AbstractProduct> CreateProduct()
{
   return (ConcreteProduct::create());
}

/** \class Factory Factory.h vpr/Util/Factory.h
 *
 * Implements generic Factory pattern.
 *
 * @param AbstractProduct    The base class for the hierarchy for the object
 *                           factory.
 * @param IndentifierType    The ID type for indexing the creators (must be
 *                           sortable).
 * @param ProductCreator     The callable entity that creates objects.  It
 *                           must support <tt>AbstractProduct* operator()</tt>.
 *                           For example, functions, functors, and classes are
 *                           valid types to use for this parameter.  The
 *                           default type is a simple function.
 * @param FactoryErrorPolicy The handler for failed lookups.  It must support
 *                           the following:
 * \code
 * FactoryErrorImpl<IdentifierType, AbstractProduct> fErrorImpl;
 * AbstractProduct* p = fErrorImpl.onUnknownType(id)
 * \endcode
 */
template<
   class AbstractProduct,
   class IdentifierType,
   typename ProductCreator = boost::shared_ptr<AbstractProduct> (*)(),
      class FactoryErrorPolicy = NullFactoryError<IdentifierType, AbstractProduct>
>
class Factory : public FactoryErrorPolicy
{
public:
   bool registerCreator(const IdentifierType& id, ProductCreator creator)
   {
      // XXX: It would probably be better to use CreatorMap::value_type here.
      std::pair<const IdentifierType, ProductCreator> p = std::make_pair(id, creator);
      return mCreatorMap.insert(p).second;
   }

   bool unregisterCreator(const IdentifierType& id)
   {
      return (mCreatorMap.erase(id) == 1);   // return (num_erased == 1)
   }
   bool isRegistered(const IdentifierType& id)
   {
      return ( mCreatorMap.find(id) != mCreatorMap.end());
   }

   boost::shared_ptr<AbstractProduct> createObject(const IdentifierType& id)
   {
      typename CreatorMap::const_iterator i = mCreatorMap.find(id);
      if(i != mCreatorMap.end())
      {
         return (i->second)();
      }

      return onUnknownType(id);     // Calls template method from FactoryErrorPolicy<>
   }

protected:
   typedef std::map<IdentifierType, ProductCreator> CreatorMap;
   CreatorMap mCreatorMap;
};

/** \class PacketFactory PacketFactory.h cluster/Packets/PacketFactory.h
 *
 * Cluster packet factory.
 */
class GADGET_CLASS_API PacketFactory :
   public Factory<Packet, vpr::Uint16>
{
public:
   vprSingletonHeader(PacketFactory);
};

} // End of cluster namespace

#endif
