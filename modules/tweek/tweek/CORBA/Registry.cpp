/***************** <Tweek heading BEGIN do not edit this line> ****************
 * Tweek
 *
 * -----------------------------------------------------------------
 * File:          $RCSfile$
 * Date modified: $Date$
 * Version:       $Revision$
 * -----------------------------------------------------------------
 ***************** <Tweek heading END do not edit this line> *****************/

/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998, 1999, 2000, 2001 by Iowa State University
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

#include <tweek/tweekConfig.h>

#include <vpr/Util/Debug.h>

#include <tweek/CORBA/Registry.h>


namespace tweek
{

//Constructor
Registry::Registry() : m_my_thread(NULL)
{
   /* Do nothing. */ ;
}

//init() Get CORBA ready to go
void Registry::initCORBA()
{
   int temp = 0;

   orb = CORBA::ORB_init(temp, 0, "omniORB2");
   boa = orb->BOA_init(temp, 0, "omniORB2_BOA");

   CosNaming::NamingContext_var rootContext;

   try
   {
      CORBA::Object_var initServ;

      initServ    = orb->resolve_initial_references("NameService");
      rootContext = CosNaming::NamingContext::_narrow(initServ);

      if ( CORBA::is_nil(rootContext) )
      {
         vprDEBUG(vprDBG_ALL, vprDBG_CRITICAL_LVL)
            << "Failed to narrow. Registry::init\n" << vprDEBUG_FLUSH;
      }
   }
   catch (CORBA::ORB::InvalidName& ex)
   {
      vprDEBUG(vprDBG_ALL, vprDBG_CRITICAL_LVL)
         << "Service required invalid.  Registry::init\n" << vprDEBUG_FLUSH;
   }

   try
   {
      CosNaming::Name contextName;
      contextName.length(1);
      contextName[0].id=CORBA::string_dup("test");
      contextName[0].kind=CORBA::string_dup("my_context");

      try
      {
         testContext = rootContext->bind_new_context(contextName);
      }
      catch (CosNaming::NamingContext::AlreadyBound& ex)
      {
         CORBA::Object_var tmpobj;

         tmpobj      = rootContext->resolve(contextName);
         testContext = CosNaming::NamingContext::_narrow(tmpobj);

         if ( CORBA::is_nil(testContext) )
         {
            vprDEBUG(vprDBG_ALL, vprDBG_CRITICAL_LVL)
               << "Failed to narrow. Registry::init\n" << vprDEBUG_FLUSH;
         }
      }
   }
   catch (CORBA::COMM_FAILURE& ex)
   {
      vprDEBUG(vprDBG_ALL, vprDBG_CRITICAL_LVL)
         << "COMM_FAILURE: no NameService found.  Registry::init\n"
         << vprDEBUG_FLUSH;
   }
   catch (omniORB::fatalException& ex)
   {
      throw;
   }
   catch (...)
   {
   }

   start();
}

//regInterface() Bind interface to the running name service
void Registry::addInterface (CORBA::Object_ptr obj, std::string& objectId,
                             std::string& objectKind)
{
   CosNaming::Name objectName;
   objectName.length(1);
   objectName[0].id   = CORBA::string_dup(objectId.c_str());
   objectName[0].kind = CORBA::string_dup(objectKind.c_str());

   try
   {
      testContext->bind(objectName,obj);
   }
   catch (CosNaming::NamingContext::AlreadyBound& ex)
   {
      // XXX: Is this really what we want to do?
      testContext->rebind(objectName,obj);
   }
}

//getInterface() Get a reference to a reference not bound by the server
CORBA::Object_var Registry::getInterface (std::string& objectId,
                                          std::string& objectKind)
{
   CORBA::Object_var obj;
   int temp = 0;

   // Boy, does this suck.
#if OMNIORB_VER == 2
   CORBA::ORB_ptr orb = CORBA::ORB_init(temp, 0, "omniORB2");
#elif OMNIORB_VER == 3
   CORBA::ORB_ptr orb = CORBA::ORB_init(temp, 0, "omniORB3");
#endif

   CosNaming::NamingContext_var rootContext;
   CORBA::Object_var initServ;

   initServ    = orb->resolve_initial_references("NameService");
   rootContext = CosNaming::NamingContext::_narrow(initServ);

   CosNaming::Name name;
   name.length(1);
   name[0].id   = CORBA::string_dup(objectId.c_str());
   name[0].kind = CORBA::string_dup(objectKind.c_str());

   CORBA::Object_ptr objptr;

   objptr = rootContext->resolve(name);
   obj    = objptr;

   return obj;
}

// ============================================================================
// Private methods.
// ============================================================================

void Registry::start()
{
   vpr::ThreadMemberFunctor<Registry>* corba_run =
      new vpr::ThreadMemberFunctor<Registry>(this, &Registry::run);

   m_my_thread = new vpr::Thread(corba_run);
}

//run() Makes the BOA ready to serve its objects
void Registry::run(void* args)
{
   vprDEBUG(vprDBG_ALL, vprDBG_STATE_LVL) << "Server is running!\n"
                                          << vprDEBUG_FLUSH;
   boa->impl_is_ready();
}

} // End of tweek namespace
