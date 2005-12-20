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

#include <tweek/tweekConfig.h>

#include <string>
#include <vpr/vpr.h>
#include <vpr/System.h>
#include <vpr/Util/Assert.h>
#include <vpr/Util/Debug.h>

#include <tweek/Util/Debug.h>
#include <tweek/CORBA/CorbaHelpers.h>

namespace tweek
{

CosNaming::NamingContext_var getRootNamingContextByInitRef(CORBA::ORB_ptr orb)
{
#ifdef OMNIORB_VER
   // If the user does not have the OMNIORB_CONFIG environment variable set,
   // there will most likely be problems finding and/or contacting the
   // Naming Service.  To that end, print a warning saying as much when the
   // variable is not set.
   std::string temp;
   if ( vpr::System::getenv("OMNIORB_CONFIG", temp).failure() )
   {
      vprDEBUG(vprDBG_ALL, vprDBG_CRITICAL_LVL)
         << clrOutBOLD(clrYELLOW, "WARNING: OMNIORB_CONFIG not set!")
         << "  Expect problems contacting the Naming Service\n"
         << vprDEBUG_FLUSH;
   }
#endif

   CORBA::Object_var name_obj;
   CosNaming::NamingContext_var root_context;

   vprDEBUG(tweekDBG_CORBA, vprDBG_STATE_LVL) << "Requesting Name Service\n"
                                              << vprDEBUG_FLUSH;
   name_obj     = orb->resolve_initial_references("NameService");
   root_context = CosNaming::NamingContext::_narrow(name_obj);

   if ( CORBA::is_nil(root_context) )
   {
      vprDEBUG(vprDBG_ALL, vprDBG_CRITICAL_LVL)
         << "Failed to narrow Naming Service root context\n"
         << vprDEBUG_FLUSH;
   }

   return root_context;
}

CosNaming::NamingContext_var getRootNamingContextByURI(CORBA::ORB_ptr orb,
                                                       const std::string& nameServiceURI)
{
   CORBA::Object_var name_obj;
   CosNaming::NamingContext_var root_context;

   vprDEBUG(tweekDBG_CORBA, vprDBG_STATE_LVL)
      << "Requesting Name Service (" << nameServiceURI << ")\n"
      << vprDEBUG_FLUSH;
   name_obj     = orb->string_to_object(nameServiceURI.c_str());
   root_context = CosNaming::NamingContext::_narrow(name_obj);

   if ( CORBA::is_nil(root_context) )
   {
      vprDEBUG(vprDBG_ALL, vprDBG_CRITICAL_LVL)
         << "Failed to narrow Naming Service root context\n"
         << vprDEBUG_FLUSH;
   }

   return root_context;
}

CosNaming::NamingContext_var bindLocalNamingContext(CosNaming::NamingContext_ptr parentContext,
                                                    const std::string& localId)
{
   vprASSERT(! CORBA::is_nil(parentContext) && "Root context is invalid");

   std::string kind("context");
   CosNaming::Name tweek_context_name;

   tweek_context_name.length(1);
   tweek_context_name[0].id   = CORBA::string_dup(localId.c_str());
   tweek_context_name[0].kind = CORBA::string_dup(kind.c_str());

   CosNaming::NamingContext_var local_context;

   try
   {
      local_context = parentContext->bind_new_context(tweek_context_name);
   }
   catch (CosNaming::NamingContext::AlreadyBound& ex)
   {
      CORBA::Object_var temp_obj;

      temp_obj      = parentContext->resolve(tweek_context_name);
      local_context = CosNaming::NamingContext::_narrow(temp_obj);

      if ( CORBA::is_nil(local_context) )
      {
         vprDEBUG(vprDBG_ALL, vprDBG_CRITICAL_LVL)
            << "Failed to narrow Naming Service to local Tweek context\n"
            << vprDEBUG_FLUSH;
      }
   }

   return local_context;
}

void printSystemException(const CORBA::SystemException& ex,
                          const int debugLevel)
{
#ifdef TWEEK_USE_OMNIORB
   vprDEBUG_NEXT(tweekDBG_CORBA, debugLevel)
      << "Name: " << ex._name() << std::endl << vprDEBUG_FLUSH;
   vprDEBUG_NEXT(tweekDBG_CORBA, debugLevel)
      << "Minor code: " << ex.NP_minorString() << " (" << ex.minor()
      << ")" << std::endl << vprDEBUG_FLUSH;
   vprDEBUG_NEXT(tweekDBG_CORBA, debugLevel)
      << "Completed: " << vprDEBUG_FLUSH;
#else
   vprDEBUG(tweekDBG_CORBA, debugLevel) << "Name: " << ex._name()
                                        << ", minor code: " << ex.minor()
                                        << ", completed: " << vprDEBUG_FLUSH;
#endif

   switch ( ex.completed() )
   {
      case CORBA::COMPLETED_YES:
         vprDEBUG_CONT(tweekDBG_CORBA, debugLevel) << "YES" << std::endl
                                                   << vprDEBUG_FLUSH;
         break;
      case CORBA::COMPLETED_NO:
         vprDEBUG_CONT(tweekDBG_CORBA, debugLevel) << "NO" << std::endl
                                                   << vprDEBUG_FLUSH;
         break;
      case CORBA::COMPLETED_MAYBE:
         vprDEBUG_CONT(tweekDBG_CORBA, debugLevel) << "MAYBE" << std::endl
                                                   << vprDEBUG_FLUSH;
         break;
   }
}

} // End of tweek namespace
