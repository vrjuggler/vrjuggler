/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998, 1999, 2000 by Iowa State University
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
//vjProxyFactory* vjProxyFactory::mInstance = NULL;
//vjSingletonImp( vjProxyFactory ); // kevin
vjSingletonImpWithInitFunc( vjProxyFactory, loadKnownProxies );

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
   vjDEBUG(vjDBG_INPUT_MGR,1) << "vjProxyFactory: Constructor registered for: "
              << std::setiosflags(std::ios::right) << std::setw(25) << std::setfill(' ') << constructor->getChunkType().c_str() << std::resetiosflags(std::ios::right)
              //<< "   :" << (void*)constructor
              << " type: " << typeid(*constructor).name() << std::endl
              << vjDEBUG_FLUSH;
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
              << typeid(*constructor).name() << std::endl << vjDEBUG_FLUSH;
   new_dev = constructor->createProxy(chunk);
   return new_dev;
}

int vjProxyFactory::findConstructor(vjConfigChunk* chunk)
{
   std::string chunk_type;
   chunk_type = (std::string)chunk->getType();
   for(unsigned i=0;i<mConstructors.size();i++)
   {
      if(mConstructors[i]->getChunkType() == chunk_type)
         return i;
   }

   return -1;
}
