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

#ifndef _GADGET_PROXY_FACTORY_H_
#define _GADGET_PROXY_FACTORY_H_
//#pragma once

#include <gadget/gadgetConfig.h>
#include <vector>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>

#include <vpr/Util/Singleton.h>
#include <jccl/Config/ConfigElementPtr.h>
#include <jccl/RTRC/DependencyManager.h>

#include <gadget/ProxyDepChecker.h>
#include <gadget/Type/Proxy.h>


namespace gadget
{

/** \class ProxyConstructorBase ProxyFactory.h gadget/ProxyFactory.h
 *
 * Base class for virtual construction of proxies.
 * Implementations of this class are registered with the proxy factory
 * for each proxy type in the system.
 */
class ProxyConstructorBase
   : public boost::enable_shared_from_this<ProxyConstructorBase>
{
protected:
   /**
    * Constructor.
    * @post We have been registered with the proxy factory.
    */
   ProxyConstructorBase();

public:
   virtual ~ProxyConstructorBase();

   /**
    * Creates the proxy.
    * @return NULL if the proxy failed creation or configuration.
    */
   virtual ProxyPtr createProxy(jccl::ConfigElementPtr element) const = 0;

   /** Gets the string definition of the type of element we can create. */
   virtual std::string getElementType() const = 0;
};


/** \class ProxyConstructor ProxyFactory.h gadget/ProxyFactory.h
 *
 * Type-specific proxy constructor.
 */
template <class PROXY>
class ProxyConstructor : public ProxyConstructorBase
{
public:
   static boost::shared_ptr<ProxyConstructorBase> create();

   virtual ~ProxyConstructor()
   {
      /* Do nothing. */ ;
   }

   /**
    * Creates the proxy.
    * @return NULL if proxy failed creation or configuration.
    */
   ProxyPtr createProxy(jccl::ConfigElementPtr element) const
   {
      // Create new proxy
      ProxyPtr new_proxy = PROXY::create();
      bool success = new_proxy->config(element);  // Attempt to configure it
                                                  // config calls inputmgr registrator

      if(success)          // Configured succesfully
      {
         return new_proxy;
      }
      else                 // Failed
      {
         return ProxyPtr();
      }
   }

   std::string getElementType() const
   {
      return PROXY::getElementType();
   }

protected:
   ProxyConstructor()
      : ProxyConstructorBase()
   {
      /* Do nothing. */ ;
   }
};


/** \class ProxyFactory ProxyFactory.h gadget/ProxyFactory.h
 *
 * Object used for creating proxies.
 * @note Singleton
 */
class ProxyFactory
{
private:
   /** Singleton so must be private. */
   ProxyFactory() {;}

   ~ProxyFactory();

   /**
    * Registers the proxies that the system knows about.
    * @post All known proxies are registered with this factory.
    */
   void loadKnownProxies();

public:
   /**
    * Registers a proxy constructor object with this factory.  In
    * Gadgeteer 1.0, the type of \p constructor was a raw pointer to a
    * gadget::ProxyConstructorBase object.
    *
    * @param constructor The proxy constructor object that will be used later
    *                    to create a specialization of gadget::Proxy.
    *
    * @see loadProxy()
    *
    * @since 1.1.2
    */
   void registerProxy(boost::shared_ptr<ProxyConstructorBase> constructor);

   /**
    * Queries if the factory knows about the given proxy.
    *
    * @pre \p element != NULL, element is a valid config element.
    * @param element The element we are requesting about knowledge to create.
    * @return true if the factory knows how to create the proxy; false if not.
    */
   bool recognizeProxy(jccl::ConfigElementPtr element);

   /**
    * Loads the specified proxy.
    *
    * @pre recognizeDevice(element) == true.
    * @param element The specification of the proxy to load.
    * @return NULL is returned if the proxy failed to load.
    *         Otherwise, a pointer to the loaded proxy is returned.
    */
   ProxyPtr loadProxy(jccl::ConfigElementPtr element);

private:

   /**
    * Finds a constructor for the given proxy type.
    * @return -1 is returned if the constructor is not found.
    *         Otherwise, the index of the constructor is returned.
    */
   int findConstructor(jccl::ConfigElementPtr element);

private:
   ProxyDepChecker mDepChecker;

   /** List of all the proxy constructors. */
   std::vector< boost::shared_ptr<ProxyConstructorBase> > mConstructors;

   vprSingletonHeaderWithInitFunc(ProxyFactory,loadKnownProxies);
};

} // end namespace

#endif
