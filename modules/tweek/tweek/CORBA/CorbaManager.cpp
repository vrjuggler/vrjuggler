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
#include <vpr/Util/Assert.h>

#include <tweek/CORBA/SubjectManager.h>
#include <tweek/CORBA/CorbaManager.h>


namespace tweek
{

void CorbaManager::init (int argc, char** argv)
{
   CORBA::Object_var obj;

   m_orb = CORBA::ORB_init(argc, argv, "omniORB3");
   obj   = m_orb->resolve_initial_references("RootPOA");
   m_poa = PortableServer::POA::_narrow(obj);

   try
   {
      CORBA::Object_var name_obj;

      name_obj       = m_orb->resolve_initial_references("NameService");
      m_root_context = CosNaming::NamingContext::_narrow(name_obj);

      if ( CORBA::is_nil(m_root_context) )
      {
         vprDEBUG(vprDBG_ALL, vprDBG_CRITICAL_LVL)
            << "Failed to narrow. CorbaManager::init\n" << vprDEBUG_FLUSH;
      }
   }
   catch (CORBA::ORB::InvalidName& ex)
   {
      vprDEBUG(vprDBG_ALL, vprDBG_CRITICAL_LVL)
         << "Service required invalid.  CorbaManager::init\n" << vprDEBUG_FLUSH;
   }

   vpr::ThreadMemberFunctor<CorbaManager>* corba_run =
      new vpr::ThreadMemberFunctor<CorbaManager>(this, &CorbaManager::run);

   m_my_thread = new vpr::Thread(corba_run);
}

void CorbaManager::registerSubjectManager (tweek::SubjectManagerImpl* mgr)
{
   vprASSERT(! CORBA::is_nil(m_root_context) && "No naming service available");

   tweek::SubjectManager_ptr mgr_ptr;
   const char* id   = "SubjectManager";
   const char* kind = "Object";
   CosNaming::Name context_name;

   mgr_ptr = mgr->_this();

   context_name.length(1);
   context_name[0].id   = CORBA::string_dup(id);
   context_name[0].kind = CORBA::string_dup(kind);

   // Bind the Subject Manager reference and activate the object within the
   // POA.  If a Subject Manager is already bound, the exceptoin thrown
   // prevents either operation from happening.  This is correct since we
   // only want one Subject Manager per address space.
   try
   {
      m_root_context->bind(context_name, mgr_ptr);
      m_subj_mgr_id = m_poa->activate_object(mgr);
   }
   catch (CosNaming::NamingContext::AlreadyBound& ex)
   {
      vprDEBUG(vprDBG_ALL, vprDBG_WARNING_LVL)
         << "WARNING: Subject manager reference already bound!\n"
         << vprDEBUG_FLUSH;
   }
}

// ============================================================================
// Private methods.
// ============================================================================

void CorbaManager::run(void* args)
{
   vprDEBUG(vprDBG_ALL, vprDBG_STATE_LVL) << "Server is running!\n"
                                          << vprDEBUG_FLUSH;

   PortableServer::POAManager_var pman = m_poa->the_POAManager();

   pman->activate();
   m_orb->run();
   m_orb->destroy();
}

} // End of tweek namespace
