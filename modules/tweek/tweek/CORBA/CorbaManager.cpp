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

#include <vpr/vpr.h>
#include <vpr/System.h>
#include <vpr/Util/Debug.h>
#include <vpr/Util/Assert.h>

#include <tweek/CORBA/SubjectManager.h>
#include <tweek/CORBA/CorbaManager.h>


namespace tweek
{

vpr::ReturnStatus CorbaManager::init (const std::string& local_id, int& argc,
                                      char** argv)
{
   vpr::ReturnStatus status;

   try
   {
      // Initialize the ORB.
      vprDEBUG(vprDBG_ALL, vprDBG_STATE_LVL) << "Initializing ORB\n"
                                             << vprDEBUG_FLUSH;
      m_orb = CORBA::ORB_init(argc, argv, "omniORB3");

      status = createChildPOA(local_id);

      try
      {
         status = initNamingService("NameService", local_id);
      }
      catch (CORBA::ORB::InvalidName& ex)
      {
         // This should not happen!
         vprDEBUG(vprDBG_ALL, vprDBG_CRITICAL_LVL)
            << "NameService name invalid in CorbaManager::init!\n"
            << vprDEBUG_FLUSH;
      }

      vprDEBUG(vprDBG_ALL, vprDBG_STATE_LVL) << "Starting ORB thread\n"
                                             << vprDEBUG_FLUSH;
      vpr::ThreadMemberFunctor<CorbaManager>* corba_run =
         new vpr::ThreadMemberFunctor<CorbaManager>(this, &CorbaManager::run);

      m_my_thread = new vpr::Thread(corba_run);
   }
   catch (CORBA::SystemException&)
   {
      status.setCode(vpr::ReturnStatus::Fail);
      vprDEBUG(vprDBG_ALL, vprDBG_CRITICAL_LVL)
         << "Caught CORBA::SystemException\n" << vprDEBUG_FLUSH;
   }
   catch (CORBA::Exception&)
   {
      status.setCode(vpr::ReturnStatus::Fail);
      vprDEBUG(vprDBG_ALL, vprDBG_CRITICAL_LVL)
         << "Caught CORBA::Exception.\n" << vprDEBUG_FLUSH;
   }
   catch (omniORB::fatalException& fe)
   {
      status.setCode(vpr::ReturnStatus::Fail);
      vprDEBUG(vprDBG_ALL, vprDBG_CRITICAL_LVL)
         << "Caught omniORB::fatalException:\n" << vprDEBUG_FLUSH;
      vprDEBUG_NEXT(vprDBG_ALL, vprDBG_CRITICAL_LVL)
         << "  file: " << fe.file() << endl << vprDEBUG_FLUSH;
      vprDEBUG_NEXT(vprDBG_ALL, vprDBG_CRITICAL_LVL)
         << "  line: " << fe.line() << endl << vprDEBUG_FLUSH;
      vprDEBUG_NEXT(vprDBG_ALL, vprDBG_CRITICAL_LVL)
         << "  mesg: " << fe.errmsg() << endl << vprDEBUG_FLUSH;
   }
   catch(...)
   {
      cerr << "Caught unknown exception." << endl;
   }

   return status;
}

void CorbaManager::shutdown (bool wait_for_completion)
{
   if ( ! CORBA::is_nil(m_root_poa) )
   {
      // We want to etherialize objects (destroy registered servants).  This
      // will destroy all descendant POAs, so we do not have to worry about
      // them at all.
      m_root_poa->destroy(true, wait_for_completion);
   }

   if ( ! CORBA::is_nil(m_orb) )
   {
      m_orb->shutdown(wait_for_completion);
   }
}

vpr::ReturnStatus CorbaManager::createSubjectManager ()
{
   vprASSERT(! CORBA::is_nil(m_root_context) && "No naming service available");
   vprASSERT(! CORBA::is_nil(m_local_context) && "No naming service available");
   vpr::ReturnStatus status;

   tweek::SubjectManager_ptr mgr_ptr;

   vprASSERT(m_subj_mgr == NULL && "Subject Manager already exists for this CORBA Manager!");
   m_subj_mgr = new SubjectManagerImpl(*this);

   // Try to activate the given servant with our child POA before anyone tries
   // to use it.
   try
   {
      m_subj_mgr_id = m_child_poa->activate_object(m_subj_mgr);
   }
   // This will be raised if the IdUniqunessPolicy within our child POA is set
   // to UNIQUE_ID.
   catch (PortableServer::POA::ServantAlreadyActive& active_ex)
   {
      vprDEBUG(vprDBG_ALL, vprDBG_WARNING_LVL)
         << "WARNING: Servant already active within our POA\n"
         << vprDEBUG_FLUSH;
   }
   catch (PortableServer::POA::WrongPolicy& policy_ex)
   {
      status.setCode(vpr::ReturnStatus::Fail);
      vprDEBUG(vprDBG_ALL, vprDBG_CRITICAL_LVL)
         << "Invalid policy used when activating Subject Manager object\n"
         << vprDEBUG_FLUSH;
   }

   // Only proceed if we were able to activate an object within the POA.  If
   // we couldn't, there is no point in registering anything with the naming
   // service.
   if ( status.success() )
   {
      // Try to add the mgr_ptr reference to the bound references known to the
      // naming service.
      try
      {
         const char* id   = "SubjectManager";
         const char* kind = "Object";
         CosNaming::Name context_name;

         // This gives us our reference from the POA to the servant that was
         // registered above.  This does not perform object activation because
         // the object was activated above.
         mgr_ptr = m_subj_mgr->_this();

         vprASSERT(! CORBA::is_nil(mgr_ptr) && "CORBA object not activated in POA");

         context_name.length(1);
         context_name[0].id   = CORBA::string_dup(id);
         context_name[0].kind = CORBA::string_dup(kind);

         // Bind the Subject Manager reference and activate the object within
         // the POA.  If a Subject Manager is already bound, the exceptoin
         // thrown prevents either operation from happening.  This is correct
         // since we only want one Subject Manager per address space.
         try
         {
            m_local_context->bind(context_name, mgr_ptr);
         }
         catch (CosNaming::NamingContext::AlreadyBound& ex)
         {
            vprDEBUG(vprDBG_ALL, vprDBG_WARNING_LVL)
               << "WARNING: Subject manager reference already bound!\n"
               << vprDEBUG_FLUSH;
         }
      }
      catch (CORBA::COMM_FAILURE& ex)
      {
         status.setCode(vpr::ReturnStatus::Fail);
         vprDEBUG(vprDBG_ALL, vprDBG_WARNING_LVL)
            << "Unable to contact the naming service\n" << vprDEBUG_FLUSH;
      }
      catch (CORBA::SystemException&)
      {
         status.setCode(vpr::ReturnStatus::Fail);
         vprDEBUG(vprDBG_ALL, vprDBG_WARNING_LVL)
            << "Caught a CORBA::SystemException while using the naming service"
            << std::endl << vprDEBUG_FLUSH;
      }
   }

   return status;
}

// ============================================================================
// Private methods.
// ============================================================================

vpr::ReturnStatus CorbaManager::createChildPOA (const std::string& local_id)
{
   vpr::ReturnStatus status;
   CORBA::Object_var obj;
   CORBA::PolicyList policy_list;

   // Obtain a reference to the root POA.  The caller will have to catch any
   // thrown exceptions.
   vprDEBUG(vprDBG_ALL, vprDBG_STATE_LVL) << "Requesting Root POA\n"
                                          << vprDEBUG_FLUSH;
   obj        = m_orb->resolve_initial_references("RootPOA");
   m_root_poa = PortableServer::POA::_narrow(obj);

   vprASSERT(! CORBA::is_nil(m_root_poa) && "Failed to get Root POA");

   // We want to allow multiple IDs to the same object and retain the
   // references.  The latter is required if we wish to do explict activation.
   PortableServer::IdUniquenessPolicy_var uniq_policy =
      m_root_poa->create_id_uniqueness_policy(PortableServer::MULTIPLE_ID);
   PortableServer::ServantRetentionPolicy_var retain_policy =
      m_root_poa->create_servant_retention_policy(PortableServer::RETAIN);
   PortableServer::ThreadPolicy_var thread_policy =
      m_root_poa->create_thread_policy(PortableServer::ORB_CTRL_MODEL);

   policy_list.length(3);
   policy_list[0] =
      PortableServer::IdUniquenessPolicy::_duplicate(uniq_policy);
   policy_list[1] =
      PortableServer::ServantRetentionPolicy::_duplicate(retain_policy);
   policy_list[2] =
      PortableServer::ThreadPolicy::_duplicate(thread_policy);

   std::string poa_name = "tweek_" + local_id;

   try
   {
      vprDEBUG(vprDBG_ALL, vprDBG_STATE_LVL)
         << "Creating child of root POA named " << poa_name << std::endl
         << vprDEBUG_FLUSH;
      m_child_poa = m_root_poa->create_POA(poa_name.c_str(),
                                           PortableServer::POAManager::_nil(),
                                           policy_list);
   }
   catch (PortableServer::POA::AdapterAlreadyExists& ex)
   {
      status.setCode(vpr::ReturnStatus::Fail);
      vprDEBUG(vprDBG_ALL, vprDBG_WARNING_LVL)
         << "WARNING: Child POA named '" << poa_name << "' already exists!\n"
         << vprDEBUG_FLUSH;
   }
   catch (PortableServer::POA::InvalidPolicy& ex)
   {
      status.setCode(vpr::ReturnStatus::Fail);
      vprDEBUG(vprDBG_ALL, vprDBG_WARNING_LVL)
         << "WARNING: Failed to set IdUniquenessPolicy for child POA\n"
         << vprDEBUG_FLUSH;
   }

   uniq_policy->destroy();
   retain_policy->destroy();

   return status;
}

/**
 *
 * @post The root context is retrieved through m_orb, and a sub-context is
 *       created for use within this memory space.
 */
vpr::ReturnStatus CorbaManager::initNamingService (const std::string& ref_name,
                                                   const std::string& local_id)
{
   CORBA::Object_var name_obj;
   vpr::ReturnStatus status;

   // If the user does not have the OMNIORB_CONFIG environment variable set,
   // there will most likely be problems finding and/or contacting the
   // Naming Service.  To that end, print a warning saying as much when the
   // variable is not set.
   std::string temp;
   if ( vpr::System::getenv("OMNIORB_CONFIG", temp).failure() )
   {
      vprDEBUG(vprDBG_ALL, vprDBG_CRITICAL_LVL)
         << clrOutBOLD(clrRED, "WARNING: OMNIORB_CONFIG not set!  Expect problems contacting the Naming Service\n")
         << vprDEBUG_FLUSH;
   }

   vprDEBUG(vprDBG_ALL, vprDBG_STATE_LVL) << "Requesting Name Service\n"
                                          << vprDEBUG_FLUSH;
   name_obj       = m_orb->resolve_initial_references(ref_name.c_str());
   m_root_context = CosNaming::NamingContext::_narrow(name_obj);

   if ( CORBA::is_nil(m_root_context) )
   {
      status.setCode(vpr::ReturnStatus::Fail);
      vprDEBUG(vprDBG_ALL, vprDBG_CRITICAL_LVL)
         << "Failed to narrow Naming Service root context\n"
         << vprDEBUG_FLUSH;
   }
   else
   {
      std::string id("tweek");
      std::string kind("context");
      CosNaming::Name tweek_context_name;

//      id.append(local_id);

      tweek_context_name.length(1);
      tweek_context_name[0].id   = CORBA::string_dup(id.c_str());
      tweek_context_name[0].kind = CORBA::string_dup(kind.c_str());

      try
      {
         m_local_context = m_root_context->bind_new_context(tweek_context_name);
      }
      catch (CosNaming::NamingContext::AlreadyBound& ex)
      {
         CORBA::Object_var temp_obj;

         temp_obj        = m_root_context->resolve(tweek_context_name);
         m_local_context = CosNaming::NamingContext::_narrow(temp_obj);

         if ( CORBA::is_nil(m_local_context) )
         {
            status.setCode(vpr::ReturnStatus::Fail);
            vprDEBUG(vprDBG_ALL, vprDBG_CRITICAL_LVL)
               << "Failed to narrow Naming Service local Tweek context\n"
               << vprDEBUG_FLUSH;
         }
      }
   }

   return status;
}

void CorbaManager::run(void* args)
{
   vprDEBUG(vprDBG_ALL, vprDBG_STATE_LVL) << "Server is running!\n"
                                          << vprDEBUG_FLUSH;

   PortableServer::POAManager_var pman = m_child_poa->the_POAManager();

   pman->activate();
   m_orb->run();
   m_orb->destroy();

   vprDEBUG(vprDBG_ALL, vprDBG_STATE_LVL) << "Server has shut down\n"
                                          << vprDEBUG_FLUSH;
}

} // End of tweek namespace
