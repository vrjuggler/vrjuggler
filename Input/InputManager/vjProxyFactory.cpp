/*
 *  File:          $RCSfile$
 *  Date modified: $Date$
 *  Version:       $Revision$
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
 *                  Copyright (C) - 1997, 1998, 1999, 2000
 *              Iowa State University Research Foundation, Inc.
 *                            All Rights Reserved
 */


#include <vjConfig.h>
#include <Input/InputManager/vjProxyFactory.h>

#include <Input/InputManager/vjAnalogProxy.h>
#include <Input/InputManager/vjDigitalProxy.h>
#include <Input/InputManager/vjPosProxy.h>
#include <Input/InputManager/vjGloveProxy.h>
#include <Input/InputManager/vjGestureProxy.h>
#include <Input/InputManager/vjKeyboardProxy.h>

#include <typeinfo>


// Initialize the singleton ptr
vjProxyFactory* vjProxyFactory::mInstance = NULL;

template <class PROXY>
vjProxyConstructor<PROXY>::vjProxyConstructor()
{
   vjProxyFactory::instance()->registerProxy(this);
}


// Register all the proxies that I know about
void vjProxyFactory::loadKnownProxies()
{
   vjProxyConstructor<vjAnalogProxy>* analog_proxy = new vjProxyConstructor<vjAnalogProxy>;
   vjProxyConstructor<vjDigitalProxy>* digital_proxy = new vjProxyConstructor<vjDigitalProxy>;
   vjProxyConstructor<vjPosProxy>* pos_proxy = new vjProxyConstructor<vjPosProxy>;
   vjProxyConstructor<vjGloveProxy>* glove_proxy = new vjProxyConstructor<vjGloveProxy>;
   vjProxyConstructor<vjGestureProxy>* gesture_proxy = new vjProxyConstructor<vjGestureProxy>;
   vjProxyConstructor<vjKeyboardProxy>* keyboard_proxy = new vjProxyConstructor<vjKeyboardProxy>;
}

void vjProxyFactory::registerProxy(vjProxyConstructorBase* constructor)
{
   mConstructors.push_back(constructor);     // Add the constructor to the list
   vjDEBUG(vjDBG_INPUT_MGR,1) << "vjProxyFactory::registerProxy: Constructor registered for: "
              << constructor->getChunkType().c_str()
              << "   :" << (void*)constructor
              << " type:" << typeid(*constructor).name() << endl << vjDEBUG_FLUSH;
}

// Simply query all proxy constructors registered looking
// for one that knows how to load the proxy
bool vjProxyFactory::recognizeProxy(vjConfigChunk* chunk)
{
   if(findConstructor(chunk) == -1)
      return false;
   else
      return true;
}

//: Load the specified proxy
//!PRE: recognizeProxy(chunk) == true
//!ARGS: chunk - specification of the proxy to load
//!RETURNS: null - Proxy failed to load
//+         other - Pointer to the loaded proxy
vjProxy* vjProxyFactory::loadProxy(vjConfigChunk* chunk)
{

   vjASSERT(recognizeProxy(chunk));

   int index = findConstructor(chunk);

   vjProxy* new_dev;
   vjProxyConstructorBase* constructor = mConstructors[index];

   vjDEBUG(vjDBG_INPUT_MGR,3) << "vjProxyFactory::loadProxy: Loading proxy: "
              << chunk->getType() << "  with: "
              << typeid(*constructor).name() << endl << vjDEBUG_FLUSH;
   new_dev = constructor->createProxy(chunk);
   return new_dev;
}

int vjProxyFactory::findConstructor(vjConfigChunk* chunk)
{
   std::string chunk_type;
   chunk_type = (std::string)chunk->getType();
   for(int i=0;i<mConstructors.size();i++)
   {
      if(mConstructors[i]->getChunkType() == chunk_type)
         return i;
   }

   return -1;
}
