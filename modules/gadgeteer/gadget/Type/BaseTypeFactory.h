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

#ifndef _GADGET_BASE_TYPE_FACTORY_H_
#define _GADGET_BASE_TYPE_FACTORY_H_
//#pragma once

#include <gadget/gadgetConfig.h>
#include <vector>
#include <boost/concept_check.hpp>

#include <gadget/Type/Input.h>
#include <jccl/Config/ConfigElementPtr.h>
#include <vpr/Util/Assert.h>
#include <vpr/Util/Debug.h>
#include <vpr/Util/Singleton.h>

namespace gadget
{

/** \struct NullFactoryError BaseTypeFactory.h gadget/Type/BaseTypeFactor.y
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

/** \class Factory Factory.h vpr/Util/Factory.h
 *
 * Implements generic Factory pattern.
 *
 * @param AbstractProduct    The base class for the hierarchy for the object
 *                           factory.
 * @param IndentifierType    The ID type for indexing the creators (must be
 *                           sortable).
 * @param ProductCreator     The callable entity that creates objects. It must support 
 *                           <tt>boost::shared_ptr<AbstractProduct> operator()</tt>.
 *                           For example, functions, functors, and classes are
 *                           valid types to use for this parameter.  The
 *                           default type is a simple function.
 * @param FactoryErrorPolicy The handler for failed lookups.  It must support
 *                           the following:
 * \code
 * FactoryErrorImpl<IdentifierType, AbstractProduct> fErrorImpl;
 * boost::shared_ptr<AbstractProduct> p = fErrorImpl.onUnknownType(id)
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

/** \class BaseTypeFactory BaseTypeFactory.h gadget/Type/BaseTypeFactory.h
 *
 * Input base type factory.
 */
class GADGET_CLASS_API BaseTypeFactory :
   public gadget::Factory<Input, std::string>
{
public:
   vprSingletonHeaderWithInitFunc(BaseTypeFactory, hackLoadKnownDevices);
private:
   void hackLoadKnownDevices();
};


} // end namespace gadget

#endif
