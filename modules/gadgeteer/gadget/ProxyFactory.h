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

#ifndef _GADGET_PROXY_FACTORY_H_
#define _GADGET_PROXY_FACTORY_H_
//#pragma once

#include <gadget/gadgetConfig.h>
#include <vector>
#include <gadget/Type/Proxy.h>
#include <jccl/Config/ConfigChunkPtr.h>
#include <vpr/Util/Singleton.h>
#include <jccl/RTRC/DependencyManager.h>


namespace gadget
{

/**
 * Base class for virtual construction of proxies.
 * Implementations of this class are registered with the proxy factory
 * for each proxy tyep in the system.
 */
class ProxyConstructorBase
{
public:
   /**
    * Constructor.
    * @post We have been registered with the proxy factory.
    */
   ProxyConstructorBase() {;}

   /**
    * Creates the proxy.
    * @return NULL if the proxy failed creation or configuration.
    */
   virtual Proxy* createProxy(jccl::ConfigChunkPtr chunk) const = 0;

   /** Gets the string desc of the type of chunk we can create. */
   virtual std::string    getChunkType() const = 0;
};


template <class PROXY>
class ProxyConstructor : public ProxyConstructorBase
{
public:
   ProxyConstructor();

   /**
    * Creates the proxy.
    * @return NULL if proxy failed creation or configuration.
    */
   Proxy* createProxy(jccl::ConfigChunkPtr chunk) const
   {
      PROXY* new_proxy = new PROXY;             // Create new proxy
      bool success = new_proxy->config(chunk);  // Attempt to configure it
                                                // config calls inputmgr registrator

      if(success)          // Configured succesfully
      {
         return new_proxy;
      }
      else                 // Failed
      {
  //XXX//       delete new_proxy;
         return NULL;
      }
   }

   std::string   getChunkType() const
   { return PROXY::getChunkType(); }
};


/**
 * Object used for creating proxies.
 * @note Singleton
 */
class ProxyFactory
{
private:
   // Singleton so must be private
   ProxyFactory() {;}

   /**
    * Registers the proxies that the system knows about.
    * @post All known proxies are registered with this factory.
    */
   void loadKnownProxies();

public:
   void registerProxy(ProxyConstructorBase* constructor);

   /**
    * Queries if the factory knows about the given proxy.
    *
    * @pre chunk != NULL, chunk is a valid chunk.
    * @param chunk The chunk we are requesting about knowledge to create.
    * @return true if the factory knows how to create the proxy; false if not.
    */
   bool recognizeProxy(jccl::ConfigChunkPtr chunk);

   /**
    * Loads the specified proxy.
    *
    * @pre recognizeDevice(chunk) == true.
    * @param chunk The specification of the proxy to load.
    * @return NULL is returned if the proxy failed to load.
    *         Otherwise, a pointer to the loaded proxy is returned.
    */
   Proxy* loadProxy(jccl::ConfigChunkPtr chunk);

private:

   /**
    * Finds a constructor for the given proxy type.
    * @return -1 is returned if the constructor is not found.
    *         Otherwise, the index of the constructor is returned.
    */
   int   findConstructor(jccl::ConfigChunkPtr chunk);

private:
   std::vector<ProxyConstructorBase*> mConstructors;   /**<  List of the proxy constructors */


   vprSingletonHeaderWithInitFunc(ProxyFactory,loadKnownProxies);
};

} // end namespace

#endif
