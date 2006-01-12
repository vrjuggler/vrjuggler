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

#include <cstdio>
#include <vpr/vpr.h>
//#include <vpr/System.h>
#include <vpr/Util/Debug.h>
#include <vpr/Util/Assert.h>

#include <tweek/Util/Version.h>
#include <tweek/Util/Debug.h>
#include <tweek/idl/SubjectManager.h>
#include <tweek/CORBA/BeanDeliverySubjectImpl.h>
#include <tweek/CORBA/CorbaHelpers.h>
#include <tweek/CORBA/CorbaManager.h>


namespace tweek
{

const std::string CorbaManager::DELIVERY_SUBJECT_NAME("TweekBeanPusher");

CorbaManager::CorbaManager()
   : mAppName("unknown"), mOrbFunctor(NULL), mOrbThread(NULL),
     mSubjectManager(NULL), mBeanDeliverySubject(NULL)
{
   std::string tweek_ver = getVersionString();

   // Print out the Tweek version information.
   vprDEBUG(tweekDBG_CORBA, vprDBG_CRITICAL_LVL)
      << std::string(tweek_ver.length() + 14, '=') << std::endl
      << vprDEBUG_FLUSH;
   vprDEBUG(tweekDBG_CORBA, vprDBG_CRITICAL_LVL)
      << clrOutNORM(clrGREEN, "Tweek Server: ") << clrOutNORM(clrGREEN, tweek_ver)
      << clrRESET << std::endl << vprDEBUG_FLUSH;
   vprDEBUG(tweekDBG_CORBA, vprDBG_CRITICAL_LVL)
      << std::string(tweek_ver.length() + 14, '=') << std::endl
      << vprDEBUG_FLUSH;
}

vpr::ReturnStatus CorbaManager::init(const std::string& local_id, int& argc,
                                     char** argv, const std::string& nsHost,
                                     const vpr::Uint16& nsPort,
                                     const std::string& iiopVersion)
{
   vpr::ReturnStatus status;

   // Retrieve the application name from argv if argv is non-NULL.
   if ( NULL != argv )
   {
      mAppName = argv[0];
   }

   try
   {
      // Initialize the ORB.
      vprDEBUG(tweekDBG_CORBA, vprDBG_STATE_LVL)
         << "Initializing ORB (using init string '" << TWEEK_ORB_VER_STRING
         << "')\n" << vprDEBUG_FLUSH;
      mORB = CORBA::ORB_init(argc, argv, TWEEK_ORB_VER_STRING);

      status = createChildPOA(local_id);

      try
      {
         // If no Naming Service host is given in the nsHost parameter, we
         // will use the initial references method for getting the root
         // naming context.
         if ( nsHost == std::string("") )
         {
            mRootContext = tweek::getRootNamingContextByInitRef(mORB);
         }
         // Otherwise, we will use a corbaloc URI to get the reference to
         // the Naming Service.
         else
         {
            // Why isn't this conversion easier to do with std::string?
            char nsPort_str[6];
            std::sprintf(nsPort_str, "%hu", nsPort);

            std::string ns_uri("corbaloc:iiop:");
            ns_uri += iiopVersion;
            ns_uri += std::string("@");
            ns_uri += nsHost;
            ns_uri += std::string(":");
            ns_uri += nsPort_str;
            ns_uri += std::string("/NameService");

            mRootContext = tweek::getRootNamingContextByURI(mORB, ns_uri);
         }

         if ( ! CORBA::is_nil(mRootContext) )
         {
            mLocalContext = tweek::bindLocalNamingContext(mRootContext,
                                                          std::string("tweek"));

            // XXX: Still need to do something with user-specified context...
         }
      }
      catch (CORBA::ORB::InvalidName& ex)
      {
         boost::ignore_unused_variable_warning(ex);

         // This should not happen!
         vprDEBUG(vprDBG_ALL, vprDBG_CRITICAL_LVL)
            << "NameService name invalid in CorbaManager::init!\n"
            << vprDEBUG_FLUSH;
      }

      vprDEBUG(tweekDBG_CORBA, vprDBG_STATE_LVL) << "Starting ORB thread\n"
                                                 << vprDEBUG_FLUSH;
      mOrbFunctor = new vpr::ThreadRunFunctor<tweek::CorbaManager>(this);
      mOrbThread  = new vpr::Thread(mOrbFunctor);
   }
   catch (CORBA::SystemException& sysEx)
   {
      status.setCode(vpr::ReturnStatus::Fail);
      vprDEBUG(tweekDBG_CORBA, vprDBG_CRITICAL_LVL)
         << "Caught CORBA::SystemException during initialization\n"
         << vprDEBUG_FLUSH;
      printSystemException(sysEx, vprDBG_CRITICAL_LVL);
   }
   catch (CORBA::Exception&)
   {
      status.setCode(vpr::ReturnStatus::Fail);
      vprDEBUG(tweekDBG_CORBA, vprDBG_CRITICAL_LVL)
         << "Caught CORBA::Exception during initialization.\n"
         << vprDEBUG_FLUSH;
   }
#ifdef OMNIORB_VER
   catch (omniORB::fatalException& fe)
   {
      status.setCode(vpr::ReturnStatus::Fail);
      vprDEBUG(tweekDBG_CORBA, vprDBG_CRITICAL_LVL)
         << "Caught omniORB::fatalException:\n" << vprDEBUG_FLUSH;
      vprDEBUG_NEXT(tweekDBG_CORBA, vprDBG_CRITICAL_LVL)
         << "  file: " << fe.file() << std::endl << vprDEBUG_FLUSH;
      vprDEBUG_NEXT(tweekDBG_CORBA, vprDBG_CRITICAL_LVL)
         << "  line: " << fe.line() << std::endl << vprDEBUG_FLUSH;
      vprDEBUG_NEXT(tweekDBG_CORBA, vprDBG_CRITICAL_LVL)
         << "  mesg: " << fe.errmsg() << std::endl << vprDEBUG_FLUSH;
   }
#endif
   catch(...)
   {
      vprDEBUG(tweekDBG_CORBA, vprDBG_CRITICAL_LVL)
         << "Caught unknown exception during initialization." << std::endl
         << vprDEBUG_FLUSH;
   }

   return status;
}

void CorbaManager::shutdown(bool waitForCompletion)
{
   // If the Subject Manager exists, we need to deactivate it, remove it
   // from the Naming Service, and free its memory.
   if ( NULL != mSubjectManager )
   {
      if ( NULL != mBeanDeliverySubject )
      {
         try
         {
            mSubjectManager->unregisterSubject(DELIVERY_SUBJECT_NAME.c_str());
         }
         catch (CORBA::OBJECT_NOT_EXIST&)
         {
            /* Do nothing. */ ;
         }

         mBeanDeliverySubject->_remove_ref();
         mBeanDeliverySubject = NULL;
      }

      destroySubjectManager();
   }

   if ( ! CORBA::is_nil(mRootPOA) )
   {
      // We want to etherialize objects (destroy registered servants).  This
      // will destroy all descendant POAs, so we do not have to worry about
      // them at all.
      try
      {
         mRootPOA->destroy(true, waitForCompletion);
         mRootPOA = PortableServer::POA::_nil();
      }
      catch (CORBA::SystemException& ex)
      {
         vprDEBUG(tweekDBG_CORBA, vprDBG_CRITICAL_LVL)
            << "Caught CORBA::SystemException during root POA destruction\n"
            << vprDEBUG_FLUSH;
         printSystemException(ex, vprDBG_CRITICAL_LVL);
      }
      catch (...)
      {
         vprDEBUG(tweekDBG_CORBA, vprDBG_CRITICAL_LVL)
            << "Caught unknown exception during root POA destruction."
            << std::endl << vprDEBUG_FLUSH;
      }
   }

   if ( ! CORBA::is_nil(mORB) )
   {
      try
      {
         mORB->shutdown(waitForCompletion);
         mORB->destroy();
         mORB = CORBA::ORB::_nil();
      }
      catch (CORBA::SystemException& ex)
      {
         vprDEBUG(tweekDBG_CORBA, vprDBG_CRITICAL_LVL)
            << "Caught CORBA::SystemException during ORB shutdown\n"
            << vprDEBUG_FLUSH;
         printSystemException(ex, vprDBG_CRITICAL_LVL);
      }
      catch (...)
      {
         vprDEBUG(tweekDBG_CORBA, vprDBG_CRITICAL_LVL)
            << "Caught unknown exception during ORB shutdown." << std::endl
            << vprDEBUG_FLUSH;
      }
   }
}

vpr::ReturnStatus CorbaManager::createSubjectManager()
{
   vprASSERT(! CORBA::is_nil(mRootContext) && "No naming service available");
   vprASSERT(! CORBA::is_nil(mLocalContext) && "No naming service available");
   vpr::ReturnStatus status;

   tweek::SubjectManager_var mgr_ptr;

   vprASSERT(mSubjectManager == NULL && "Subject Manager already exists for this CORBA Manager!");
   mSubjectManager = new SubjectManagerImpl(*this);
   mSubjectManager->setApplicationName(mAppName);

   // Try to activate the given servant with our child POA before anyone tries
   // to use it.
   try
   {
      mChildPOA->activate_object(mSubjectManager);
      mSubjectManager->_add_ref();
   }
   // This will be raised if the IdUniqunessPolicy within our child POA is set
   // to UNIQUE_ID.
   catch (PortableServer::POA::ServantAlreadyActive& active_ex)
   {
      boost::ignore_unused_variable_warning(active_ex);
      vprDEBUG(tweekDBG_CORBA, vprDBG_WARNING_LVL)
         << "WARNING: Servant already active within our POA\n"
         << vprDEBUG_FLUSH;
   }
   catch (PortableServer::POA::WrongPolicy& policy_ex)
   {
      boost::ignore_unused_variable_warning(policy_ex);
      status.setCode(vpr::ReturnStatus::Fail);
      vprDEBUG(tweekDBG_CORBA, vprDBG_CRITICAL_LVL)
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
         // Construct the SubjectManager's name using its GUID so that it is
         // guaranteed to be unique.
         std::string id_str("SubjectManager.");
         id_str += mSubjectManager->getGUID().toString();
         mSubjectManager->setName(id_str);

         const char* kind = "Object";

         vprDEBUG(tweekDBG_CORBA, vprDBG_VERB_LVL)
            << "Subject Manager ID: " << id_str << std::endl << vprDEBUG_FLUSH;

         // This gives us our reference from the POA to the servant that was
         // registered above.  This does not perform object activation because
         // the object was activated above.
         mgr_ptr = mSubjectManager->_this();

         vprASSERT(! CORBA::is_nil(mgr_ptr) && "CORBA object not activated in POA");

         CosNaming::Name subj_mgr_name;
         subj_mgr_name.length(1);
         subj_mgr_name[0].id   = CORBA::string_dup(id_str.c_str());
         subj_mgr_name[0].kind = CORBA::string_dup(kind);

         // Bind the Subject Manager reference and activate the object within
         // the POA.  If a Subject Manager is already bound, the exceptoin
         // thrown prevents either operation from happening.  This is correct
         // since we only want one Subject Manager per address space.
         try
         {
            mLocalContext->bind(subj_mgr_name, mgr_ptr);

            // Now that everything is set up with the Subject Manager, we can
            // register the one Subject that is always around: the default Bean
            // Delivery Subject.  This Subject is available for easy delivery
            // of new Beans to observers.
            try
            {
               mBeanDeliverySubject = new BeanDeliverySubjectImpl();
               mSubjectManager->registerSubject(mBeanDeliverySubject,
                                                DELIVERY_SUBJECT_NAME.c_str());
               mBeanDeliverySubject->_add_ref();
            }
            catch (...)
            {
               delete mBeanDeliverySubject;
               mBeanDeliverySubject = NULL;
               vprDEBUG(tweekDBG_CORBA, vprDBG_WARNING_LVL)
                  << clrOutNORM(clrYELLOW, "WARNING")
                  << ": Failed to register Bean Delivery Subject\n"
                  << vprDEBUG_FLUSH;
            }
         }
         catch (CosNaming::NamingContext::AlreadyBound& ex)
         {
            boost::ignore_unused_variable_warning(ex);
            vprDEBUG(vprDBG_ALL, vprDBG_WARNING_LVL)
               << "WARNING: Subject manager reference already bound!\n"
               << vprDEBUG_FLUSH;
         }
      }
      catch (CORBA::COMM_FAILURE& ex)
      {
         boost::ignore_unused_variable_warning(ex);
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

vpr::ReturnStatus CorbaManager::destroySubjectManager()
{
   vpr::ReturnStatus status;

   // Only try to do destruction if there is a servant to destroy.
   if ( mSubjectManager != NULL )
   {
      mSubjectManager->_remove_ref();
      mSubjectManager = NULL;
   }
   else
   {
      status.setCode(vpr::ReturnStatus::Fail);
      vprDEBUG(tweekDBG_CORBA, vprDBG_WARNING_LVL)
         << "WARNING: No Subject Manager servant to destroy!\n"
         << vprDEBUG_FLUSH;
   }

   return status;
}

// ============================================================================
// Private methods.
// ============================================================================

vpr::ReturnStatus CorbaManager::createChildPOA(const std::string& local_id)
{
   vpr::ReturnStatus status;
   CORBA::Object_var obj;
   CORBA::PolicyList policy_list;

   // Obtain a reference to the root POA.  The caller will have to catch any
   // thrown exceptions.
   vprDEBUG(tweekDBG_CORBA, vprDBG_STATE_LVL) << "Requesting Root POA\n"
                                              << vprDEBUG_FLUSH;
   obj      = mORB->resolve_initial_references("RootPOA");
   mRootPOA = PortableServer::POA::_narrow(obj);

   vprASSERT(! CORBA::is_nil(mRootPOA) && "Failed to get Root POA");

   // We want to allow multiple IDs to the same object and retain the
   // references.  The latter is required if we wish to do explict activation.
   PortableServer::IdUniquenessPolicy_var uniq_policy =
      mRootPOA->create_id_uniqueness_policy(PortableServer::MULTIPLE_ID);
   PortableServer::ServantRetentionPolicy_var retain_policy =
      mRootPOA->create_servant_retention_policy(PortableServer::RETAIN);
   PortableServer::ThreadPolicy_var thread_policy =
      mRootPOA->create_thread_policy(PortableServer::ORB_CTRL_MODEL);

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
      vprDEBUG(tweekDBG_CORBA, vprDBG_STATE_LVL)
         << "Creating child of root POA named " << poa_name << std::endl
         << vprDEBUG_FLUSH;
      mChildPOA = mRootPOA->create_POA(poa_name.c_str(),
                                       PortableServer::POAManager::_nil(),
                                       policy_list);
   }
   catch (PortableServer::POA::AdapterAlreadyExists& ex)
   {
      boost::ignore_unused_variable_warning(ex);
      status.setCode(vpr::ReturnStatus::Fail);
      vprDEBUG(vprDBG_ALL, vprDBG_WARNING_LVL)
         << "WARNING: Child POA named '" << poa_name << "' already exists!\n"
         << vprDEBUG_FLUSH;
   }
   catch (PortableServer::POA::InvalidPolicy& ex)
   {
      boost::ignore_unused_variable_warning(ex);
      status.setCode(vpr::ReturnStatus::Fail);
      vprDEBUG(vprDBG_ALL, vprDBG_WARNING_LVL)
         << "WARNING: Failed to set IdUniquenessPolicy for child POA\n"
         << vprDEBUG_FLUSH;
   }

   uniq_policy->destroy();
   retain_policy->destroy();

   return status;
}

void CorbaManager::run()
{
   vprDEBUG(tweekDBG_CORBA, vprDBG_STATE_LVL) << "Server is running!\n"
                                              << vprDEBUG_FLUSH;

   PortableServer::POAManager_var pman = mChildPOA->the_POAManager();

   pman->activate();
   mORB->run();
//   mORB->destroy();

   vprDEBUG(tweekDBG_CORBA, vprDBG_STATE_LVL) << "Server has shut down\n"
                                              << vprDEBUG_FLUSH;
}

} // End of tweek namespace
