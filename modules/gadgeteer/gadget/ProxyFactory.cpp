/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2006 by Iowa State University
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

#include <gadget/gadgetConfig.h>

#include <iomanip>
#include <typeinfo>

#include <jccl/Config/ConfigElement.h>

#include <gadget/Type/AnalogProxy.h>
#include <gadget/Type/DigitalProxy.h>
#include <gadget/Type/PositionProxy.h>
#include <gadget/Type/GloveProxy.h>
#include <gadget/Type/GestureProxy.h>
#include <gadget/Type/KeyboardMouseProxy.h>
#include <gadget/Type/CommandProxy.h>
#include <gadget/Type/StringProxy.h>
#include <gadget/Util/Debug.h>

#include <gadget/ProxyFactory.h>


namespace gadget
{

// Initialize the singleton ptr
vprSingletonImpWithInitFunc( ProxyFactory, loadKnownProxies );

ProxyConstructorBase::ProxyConstructorBase()
   : boost::enable_shared_from_this<ProxyConstructorBase>()
{
   /* Do nothing. */ ;
}

ProxyConstructorBase::~ProxyConstructorBase()
{
   /* Do nothing. */ ;
}

template<typename PROXY>
boost::shared_ptr<ProxyConstructorBase> ProxyConstructor<PROXY>::create()
{
   boost::shared_ptr< ProxyConstructor<PROXY> > px(new ProxyConstructor());
   ProxyFactory::instance()->registerProxy(px);
   return px;
}

ProxyFactory::~ProxyFactory()
{
   mConstructors.clear();
}

// Register all the proxies that I know about
void ProxyFactory::loadKnownProxies()
{
   ProxyConstructor<AnalogProxy>::create();
   ProxyConstructor<DigitalProxy>::create();
   ProxyConstructor<PositionProxy>::create();
   ProxyConstructor<GloveProxy>::create();
//   ProxyConstructor<GestureProxy>::create();
   ProxyConstructor<KeyboardMouseProxy>::create();
   ProxyConstructor<CommandProxy>::create();
   ProxyConstructor<StringProxy>::create();

   jccl::DependencyManager::instance()->registerChecker(&mDepChecker);
}

void ProxyFactory::registerProxy(boost::shared_ptr<ProxyConstructorBase> constructor)
{
   mConstructors.push_back(constructor);     // Add the constructor to the list
   vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_CONFIG_LVL)
      << "gadget::ProxyFactory: Constructor registered for: "
      << std::setiosflags(std::ios::right) << std::setw(25)
      << std::setfill(' ') << constructor->getElementType()
      << std::resetiosflags(std::ios::right)
      //<< "   :" << (void*)constructor
      << " type: " << typeid(*constructor).name() << std::endl
      << vprDEBUG_FLUSH;
}

// Simply query all proxy constructors registered looking
// for one that knows how to load the proxy
bool ProxyFactory::recognizeProxy(jccl::ConfigElementPtr element)
{
   return ! (findConstructor(element) == -1);
}

/**
 * Loads the specified proxy.
 */
Proxy* ProxyFactory::loadProxy(jccl::ConfigElementPtr element)
{
   vprASSERT(recognizeProxy(element));

   int index = findConstructor(element);

   Proxy* new_proxy;
   boost::shared_ptr<ProxyConstructorBase> constructor = mConstructors[index];

   vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_STATE_LVL)
      << "[gadget::ProxyFactory::loadProxy] Loading proxy: "
      << element->getID() << "  with: "
      << typeid(*constructor).name() << std::endl << vprDEBUG_FLUSH;
   new_proxy = constructor->createProxy(element);
   return new_proxy;
}

int ProxyFactory::findConstructor(jccl::ConfigElementPtr element)
{
   std::string element_type(element->getID());

   for(unsigned i=0;i<mConstructors.size();i++)
   {
      if(mConstructors[i]->getElementType() == element_type)
      {
         return i;
      }
   }

   return -1;
}

} // End of gadget namespace
