/*
 *  File:	    $RCSfile$
 *  Date modified:  $Date$
 *  Version:	    $Revision$
 *
 *
 *                                VR Juggler
 *                                    by
 *                              Allen Bierbaum
 *                             Christopher Just
 *                             Patrick Hartling
 *                            Carolina Cruz-Neira
 *                               Albert Baker
 *
 *                         Copyright  - 1997,1998,1999
 *                Iowa State University Research Foundation, Inc.
 *                            All Rights Reserved
 */


#ifndef _VJ_PROXY_FACTORY_H_
#define _VJ_PROXY_FACTORY_H_
#pragma once

#include <vjConfig.h>
#include <Input/InputManager/vjProxy.h>
#include <Config/vjConfigChunk.h>

//: Base class for virtual construction of proxies
// Implementations of this class are registered with the proxy factory
// for each proxy tyep in the system
class vjProxyConstructorBase
{
public:
   //: Constructor
   //! POST: We have been registered with the proxy factory
   vjProxyConstructorBase() {;}

   //: Create the proxy
   virtual vjProxy* createProxy(vjConfigChunk* chunk) const = 0;

   //: Get the string desc of the type of chunk we can create
   virtual std::string    getChunkType() const = 0;
};


template <class PROXY>
class vjProxyConstructor : public vjProxyConstructorBase
{
public:
   vjProxyConstructor();

   vjProxy* createProxy(vjConfigChunk* chunk) const
   {
      PROXY* new_proxy = new PROXY;             // Create new proxy
      bool success = new_proxy->config(chunk);  // Attempt to configure it

      if(success)          // Configured succesfully
         return new_proxy;
      else                 // Failed
      {
         delete new_proxy;
         return NULL;
      }
   }

   std::string   getChunkType() const
   { return PROXY::getChunkType(); }
};


//: Object used for creating proxies
//!NOTE: Singleton
class vjProxyFactory
{
private:
   // Singleton so must be private
   vjProxyFactory() {;}

   //: register the proxies that the system knows about
   //! POST: all known proxies are registered with this factory
   void loadKnownProxies();

public:
   void registerProxy(vjProxyConstructorBase* constructor);

   //: Query if the factory knows about the given proxy
   //!PRE: chunk != NULL, chunk is a valid chunk
   //!ARGS: chunk - chunk we are requesting about knowledge to create
   //!RETURNS: true - factory knows how to create the proxy
   //+          false - factory does not know how to create the proxy
   bool recognizeProxy(vjConfigChunk* chunk);

   //: Load the specified proxy
   //!PRE: recognizeDevice(chunk) == true
   //!ARGS: chunk - specification of the proxy to load
   //!RETURNS: null - Proxy failed to load
   //+         other - Pointer to the loaded proxy
   vjProxy* loadProxy(vjConfigChunk* chunk);

private:

   //: Find a constructor for the given proxy type
   //!RETURNS: -1 - Not found
   //+            - Index of the constructorck
   int   findConstructor(vjConfigChunk* chunk);

private:
   std::vector<vjProxyConstructorBase*> mConstructors;   //: List of the proxy constructors


public:     // ------ SINGLETON ----- ///
   //: Return singleton instance of the class
   static vjProxyFactory* instance()
   {
      if(mInstance == NULL)
      {
         mInstance = new vjProxyFactory();
         mInstance->loadKnownProxies();
      }
      return mInstance;
   }

private:
   static vjProxyFactory* mInstance;
};


#endif
