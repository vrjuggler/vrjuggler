/****************** <VPR heading BEGIN do not edit this line> *****************
 *
 * VR Juggler Portable Runtime
 *
 * Original Authors:
 *   Allen Bierbaum, Patrick Hartling, Kevin Meinert, Carolina Cruz-Neira
 *
 * -----------------------------------------------------------------
 * File:          $RCSfile$
 * Date modified: $Date$
 * Version:       $Revision$
 * -----------------------------------------------------------------
 *
 ****************** <VPR heading END do not edit this line> ******************/

/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2005 by Iowa State University
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

#ifndef _VPR_FACTORY_H
#define _VPR_FACTORY_H

#include <map>


namespace vpr
{

/** \struct NullFactoryError Factory.h vpr/Util/Factory.h
 *
 * @param IdentifierType  The factory identifier type.
 * @param AbstractProduct The type of the product created by the factory.
 */
template<class IdentifierType, class AbstractProduct>
struct NullFactoryError
{
   static AbstractProduct* onUnknownType(IdentifierType)
   {
      return NULL;
   }
};

/**
 * Implements a useful little template function usable as a Creator in factory.
 */
template<class AbstractProduct, class ConcreteProduct>
AbstractProduct* CreateProduct()
{
   return (new ConcreteProduct);
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
   typename ProductCreator = AbstractProduct* (*)(),
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

   AbstractProduct* createObject(const IdentifierType& id)
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

} // namespace vpr


#endif
