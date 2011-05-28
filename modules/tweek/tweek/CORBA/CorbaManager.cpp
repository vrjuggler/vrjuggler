/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2011 by Iowa State University
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
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

#include <tweek/tweekConfig.h>

#include <cstdlib>
#include <cstring>
#include <sstream>
#include <boost/bind.hpp>

#include <vpr/vpr.h>
#include <vpr/System.h>
#include <vpr/Util/Debug.h>
#include <vpr/Util/Assert.h>
#include <vpr/Util/GUID.h>

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
   : mAppName("unknown")
   , mOrbThread(NULL)
   , mRootPOA(PortableServer::POA::_nil())
   , mChildPOA(PortableServer::POA::_nil())
   , mInsPOA(PortableServer::POA::_nil())
   , mSubjectManager(NULL)
   , mBeanDeliverySubject(NULL)
{
   std::string tweek_ver = getVersionString();

   // Print out the Tweek version information.
   vprDEBUG(tweekDBG_CORBA, vprDBG_CRITICAL_LVL)
      << std::string(tweek_ver.length() + 14, '=') << std::endl
      << vprDEBUG_FLUSH;
   vprDEBUG(tweekDBG_CORBA, vprDBG_CRITICAL_LVL)
      << clrOutNORM(clrGREEN, "Tweek Server: ")
      << clrOutNORM(clrGREEN, tweek_ver) << clrRESET << std::endl
      << vprDEBUG_FLUSH;
   vprDEBUG(tweekDBG_CORBA, vprDBG_CRITICAL_LVL)
      << std::string(tweek_ver.length() + 14, '=') << std::endl
      << vprDEBUG_FLUSH;
}

CorbaManager::~CorbaManager()
{
   shutdown();

   if ( mOrbThread != NULL )
   {
      delete mOrbThread;
      mOrbThread = NULL;
   }
}

bool CorbaManager::init(const std::string& localID, int& argc, char** argv,
                        const std::string& nsHost, const vpr::Uint16& nsPort,
                        const std::string& iiopVersion)
{
   bool status(true);

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

      // We want to allow multiple IDs to the same object and retain the
      // references.  The latter is required if we wish to do explict
      // activation.
      status = createChildPOA(localID, PortableServer::MULTIPLE_ID, false);

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
            std::ostringstream uri_stream;
            uri_stream << "corbaloc:iiop:" << iiopVersion << "@" << nsHost
                       << ":" << nsPort << "/NameService";
            mRootContext = tweek::getRootNamingContextByURI(mORB,
                                                            uri_stream.str());
         }

         if ( ! CORBA::is_nil(mRootContext) )
         {
            mLocalContext = tweek::bindLocalNamingContext(mRootContext,
                                                          std::string("tweek"));

            // XXX: Still need to do something with user-specified context...
         }
      }
      catch (CORBA::ORB::InvalidName&)
      {
         // This should not happen!
         vprDEBUG(vprDBG_ALL, vprDBG_CRITICAL_LVL)
            << "NameService name invalid in CorbaManager::init!\n"
            << vprDEBUG_FLUSH;
      }

      vprDEBUG(tweekDBG_CORBA, vprDBG_STATE_LVL) << "Starting ORB thread\n"
                                                 << vprDEBUG_FLUSH;
      mOrbThread = new vpr::Thread(boost::bind(&CorbaManager::run, this));
   }
   catch (CORBA::SystemException& sysEx)
   {
      status = false;
      vprDEBUG(tweekDBG_CORBA, vprDBG_CRITICAL_LVL)
         << "Caught CORBA::SystemException during initialization\n"
         << vprDEBUG_FLUSH;
      printSystemException(sysEx, vprDBG_CRITICAL_LVL);
   }
   catch (CORBA::Exception&)
   {
      status = false;
      vprDEBUG(tweekDBG_CORBA, vprDBG_CRITICAL_LVL)
         << "Caught CORBA::Exception during initialization.\n"
         << vprDEBUG_FLUSH;
   }
#if defined(TWEEK_USE_OMNIORB)
   catch (omniORB::fatalException& fe)
   {
      status = false;
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
   catch (vpr::Exception& ex)
   {
      status = false;
      vprDEBUG(tweekDBG_CORBA, vprDBG_CRITICAL_LVL)
         << "Caught vpr::Exception during initialization.\n"
         << ex.what() << vprDEBUG_FLUSH;
   }
   catch(...)
   {
      vprDEBUG(tweekDBG_CORBA, vprDBG_CRITICAL_LVL)
         << "Caught unknown exception during initialization." << std::endl
         << vprDEBUG_FLUSH;
   }

   return status;
}

bool CorbaManager::initDirect(const std::string& localID, int& argc,
                              char** argv, const std::string& endPointAddr,
                              const vpr::Uint16 endPointPort)
{
   bool status(true);

   // Retrieve the application name from argv if argv is non-NULL.
   if ( NULL != argv )
   {
      mAppName = argv[0];
   }

   try
   {
      // Create a two-dimensional array of constant strings to be passed as
      // the options to CORBA::ORB_init(). The first column is the property
      // name, and the second is the property value. This will either contain
      // one non-empty row (row 0) identifying the GIOP end point address
      // ("endPoint"), or both rows will be empty. The last row has to have
      // both columns set to NULL to denote the end of the option list.
      const char* options[2][2] = { { NULL, NULL }, { NULL, NULL } };

      // If endPointPort is 0, then we will fall back on OMNIORB_CONFIG to
      // provide the GIOP end point setting.
      if ( endPointPort == 0 )
      {
#if defined(TWEEK_USE_OMNIORB)
         std::string temp;

         // Test to determine whether the OMNIORB_CONFIG environment variable
         // is set.
         if ( ! vpr::System::getenv("OMNIORB_CONFIG", temp) )
         {
            // If the user does not have the OMNIORB_CONFIG environment
            // variable set, then we have no way to get the GIOP end point
            // configured. As such, we have to return error status.
            vprDEBUG(vprDBG_ALL, vprDBG_CRITICAL_LVL)
               << clrOutBOLD(clrRED, "ERROR")
               << ": OMNIORB_CONFIG not set and no end point port specified!"
               << std::endl;
            vprDEBUG_NEXTnl(vprDBG_ALL, vprDBG_CRITICAL_LVL)
               << "       Cannot bind CORBA Manager to a specific end point.\n"
               << vprDEBUG_FLUSH;
            return false;
         }
#endif
      }
      // With the end point address and port number set through the method
      // parameters, we construct the endPoint string and assign it to the
      // first row in the options table.
      else
      {
         std::ostringstream end_point_stream;

         end_point_stream << "giop:tcp:" << endPointAddr << ":"
                          << endPointPort;

         options[0][0] = "endPoint";
         options[0][1] = strdup(end_point_stream.str().c_str());

         vprDEBUG(tweekDBG_CORBA, vprDBG_CONFIG_LVL)
            << "GIOP end piont: '" << options[0][1] << "'\n"
            << vprDEBUG_FLUSH;
      }

      // Initialize the ORB.
      vprDEBUG(tweekDBG_CORBA, vprDBG_STATE_LVL)
         << "Initializing ORB (using init string '" << TWEEK_ORB_VER_STRING
         << "')\n" << vprDEBUG_FLUSH;
      mORB = CORBA::ORB_init(argc, argv, TWEEK_ORB_VER_STRING, options);

      if ( options[0][1] != NULL )
      {
         // Yes, each of these could be done with a simple C-style cast, but
         // the C++ cast operations exist for a reason.
         //std::free(reinterpret_cast<void*>(const_cast<char*>(options[0][0])));
         std::free(reinterpret_cast<void*>(const_cast<char*>(options[0][1])));
         options[0][0] = NULL;
         options[0][1] = NULL;
      }

      status = createChildPOA(localID, PortableServer::UNIQUE_ID, true);

#if defined(TWEEK_USE_OMNIORB)
      if ( status )
      {
         CORBA::Object_var obj =
            mORB->resolve_initial_references("omniINSPOA");

         if ( ! CORBA::is_nil(obj) )
         {
            mInsPOA = PortableServer::POA::_narrow(obj);

            if ( ! CORBA::is_nil(mInsPOA) )
            {
               // status has to be true to have reached this point.
               mInsPOA->the_POAManager()->activate();

               vprDEBUG(tweekDBG_CORBA, vprDBG_STATE_LVL)
                  << "Starting ORB thread\n" << vprDEBUG_FLUSH;
               mOrbThread = new vpr::Thread(boost::bind(&CorbaManager::run,
                                                        this));
            }
            else
            {
               status = false;

               vprDEBUG(tweekDBG_CORBA, vprDBG_WARNING_LVL)
                  << "WARNING: Failed to narrow reference to omniORB INS POA.\n"
                  << vprDEBUG_FLUSH;
               vprDEBUG_NEXT(tweekDBG_CORBA, vprDBG_WARNING_LVL)
                  << "         Tweek CORBA services will not be available!\n"
                  << vprDEBUG_FLUSH;
            }
         }
         else
         {
            status = false;

            vprDEBUG(tweekDBG_CORBA, vprDBG_WARNING_LVL)
               << "WARNING: Failed to acquire reference to omniORB INS POA.\n"
               << vprDEBUG_FLUSH;
            vprDEBUG_NEXT(tweekDBG_CORBA, vprDBG_WARNING_LVL)
               << "         Tweek CORBA services will not be available!\n"
               << vprDEBUG_FLUSH;
         }
      }
#else
      vprDEBUG(tweekDBG_CORBA, vprDBG_CRTICAL_LVL)
         << "ERROR: Direct object connection support only works with omniORB\n"
         << vprDEBUG_FLUSH;
      status = false;
#endif
   }
   catch (CORBA::SystemException& sysEx)
   {
      status = false;
      vprDEBUG(tweekDBG_CORBA, vprDBG_CRITICAL_LVL)
         << "Caught CORBA::SystemException during initialization\n"
         << vprDEBUG_FLUSH;
      printSystemException(sysEx, vprDBG_CRITICAL_LVL);
   }
   catch (CORBA::Exception&)
   {
      status = false;
      vprDEBUG(tweekDBG_CORBA, vprDBG_CRITICAL_LVL)
         << "Caught CORBA::Exception during initialization.\n"
         << vprDEBUG_FLUSH;
   }
#if defined(TWEEK_USE_OMNIORB)
   catch (omniORB::fatalException& fe)
   {
      status = false;
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
   catch (vpr::Exception& ex)
   {
      status = false;
      vprDEBUG(tweekDBG_CORBA, vprDBG_CRITICAL_LVL)
         << "Caught vpr::Exception during initialization.\n"
         << ex.what() << vprDEBUG_FLUSH;
   }
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

bool CorbaManager::createSubjectManager()
{
   bool status(true);

   vprASSERT(mSubjectManager == NULL &&
             "Subject Manager already exists for this CORBA Manager!");

   const bool use_naming_service(CORBA::is_nil(mInsPOA));
   tweek::SubjectManager_var mgr_ptr;

   std::ostringstream id_stream;
   id_stream << "SubjectManager";

   // In the case that we do not have the omniORB INS POA, create a unique
   // name for the Subject Manager servant.
   if ( use_naming_service )
   {
      vprASSERT(! CORBA::is_nil(mRootContext) &&
                "No naming service available");
      vprASSERT(! CORBA::is_nil(mLocalContext) &&
                "No naming service available");

      vpr::GUID name_guid(vpr::GUID::generateTag);
      id_stream << "." << name_guid;
   }

   mSubjectManager = new SubjectManagerImpl(*this, id_stream.str());
   mSubjectManager->setApplicationName(mAppName);

   // Try to activate the given servant with our child POA before anyone tries
   // to use it.
   try
   {
      mChildPOA->activate_object(mSubjectManager);
   }
   // This will be raised if the IdUniqunessPolicy within our child POA is set
   // to UNIQUE_ID.
   catch (PortableServer::POA::ServantAlreadyActive&)
   {
      vprDEBUG(tweekDBG_CORBA, vprDBG_WARNING_LVL)
         << "WARNING: Servant already active within our POA\n"
         << vprDEBUG_FLUSH;
   }
   catch (PortableServer::POA::WrongPolicy&)
   {
      status = false;
      vprDEBUG(tweekDBG_CORBA, vprDBG_CRITICAL_LVL)
         << "Invalid policy used when activating Subject Manager object\n"
         << vprDEBUG_FLUSH;
   }

   // Only proceed if we were able to activate an object within the POA. If we
   // couldn't, then there is no point in performing any further registration.
   if ( status )
   {
      // If we do not have the omniORB INS POA, then we are registering the
      // new Subject Manager servant with the CORBA Naming Service.
      if ( use_naming_service )
      {
         // Try to add the mgr_ptr reference to the bound references known to
         // the Naming Service.
         try
         {
            const char* kind = "Object";

            vprDEBUG(tweekDBG_CORBA, vprDBG_VERB_LVL)
               << "Subject Manager ID: " << id_stream.str() << std::endl
               << vprDEBUG_FLUSH;

            // This gives us our reference from the POA to the servant that
            // was registered above. This does not perform object activation
            // because the object was activated above.
            mgr_ptr = mSubjectManager->_this();

            vprASSERT(! CORBA::is_nil(mgr_ptr) &&
                      "CORBA object not activated in POA");

            CosNaming::Name subj_mgr_name;
            subj_mgr_name.length(1);
            subj_mgr_name[0].id   = CORBA::string_dup(id_stream.str().c_str());
            subj_mgr_name[0].kind = CORBA::string_dup(kind);

            // Bind the Subject Manager reference and activate the object
            // within the POA. If a Subject Manager is already bound, the
            // exception thrown prevents either operation from happening. This
            // is correct since we only want one Subject Manager per address
            // space.
            try
            {
               mLocalContext->bind(subj_mgr_name, mgr_ptr);
            }
            catch (CosNaming::NamingContext::AlreadyBound&)
            {
               vprDEBUG(vprDBG_ALL, vprDBG_WARNING_LVL)
                  << "WARNING: Subject manager reference already bound!\n"
                  << vprDEBUG_FLUSH;
            }
         }
         catch (CORBA::COMM_FAILURE&)
         {
            status = false;
            vprDEBUG(vprDBG_ALL, vprDBG_WARNING_LVL)
               << "Unable to contact the naming service\n" << vprDEBUG_FLUSH;
         }
         catch (CORBA::SystemException&)
         {
            status = false;
            vprDEBUG(vprDBG_ALL, vprDBG_WARNING_LVL)
               << "Caught a CORBA::SystemException while using the naming service"
               << std::endl << vprDEBUG_FLUSH;
         }
      }
      // Create an identifier for the Subject Manager servant using its
      // (non-unique) name and activate it within the omniORB INS POA.
      else
      {
         PortableServer::ObjectId_var subj_mgr_id =
            PortableServer::string_to_ObjectId(id_stream.str().c_str());
         mInsPOA->activate_object_with_id(subj_mgr_id, mSubjectManager);
      }

      // If things were set up successfully with the Subject Manager servant,
      // we can register the one Subject that is always around: the default
      // Bean Delivery Subject. This Subject is available for easy delivery of
      // new Beans to observers.
      if ( status )
      {
         try
         {
            mBeanDeliverySubject = new BeanDeliverySubjectImpl();
            mSubjectManager->registerSubject(mBeanDeliverySubject,
                                             DELIVERY_SUBJECT_NAME.c_str());
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
   }

   return status;
}

bool CorbaManager::destroySubjectManager()
{
   bool status(false);

   // Only try to do destruction if there is a servant to destroy.
   if ( mSubjectManager != NULL )
   {
      mSubjectManager->_remove_ref();
      mSubjectManager = NULL;
   }
   else
   {
      status = false;
      vprDEBUG(tweekDBG_CORBA, vprDBG_WARNING_LVL)
         << "WARNING: No Subject Manager servant to destroy!\n"
         << vprDEBUG_FLUSH;
   }

   return status;
}

// ============================================================================
// Private methods.
// ============================================================================

bool CorbaManager::
createChildPOA(const std::string& localID,
               const PortableServer::IdUniquenessPolicyValue uniquePolicy,
               const bool bidirectional)
{
   bool status(true);
   CORBA::Object_var obj;
   CORBA::PolicyList policy_list;

   // Obtain a reference to the root POA.  The caller will have to catch any
   // thrown exceptions.
   vprDEBUG(tweekDBG_CORBA, vprDBG_STATE_LVL) << "Requesting Root POA\n"
                                              << vprDEBUG_FLUSH;
   obj      = mORB->resolve_initial_references("RootPOA");
   mRootPOA = PortableServer::POA::_narrow(obj);

   vprASSERT(! CORBA::is_nil(mRootPOA) && "Failed to get Root POA");

   PortableServer::IdUniquenessPolicy_var uniq_policy =
      mRootPOA->create_id_uniqueness_policy(uniquePolicy);
   PortableServer::ServantRetentionPolicy_var retain_policy =
      mRootPOA->create_servant_retention_policy(PortableServer::RETAIN);
   PortableServer::ThreadPolicy_var thread_policy =
      mRootPOA->create_thread_policy(PortableServer::ORB_CTRL_MODEL);

   if ( bidirectional )
   {
      policy_list.length(4);
   }
   else
   {
      policy_list.length(3);
   }

   policy_list[0] =
      PortableServer::IdUniquenessPolicy::_duplicate(uniq_policy);
   policy_list[1] =
      PortableServer::ServantRetentionPolicy::_duplicate(retain_policy);
   policy_list[2] =
      PortableServer::ThreadPolicy::_duplicate(thread_policy);

   if ( bidirectional )
   {
      CORBA::Any a;
      a <<= BiDirPolicy::BOTH;
      policy_list[3] =
         mORB->create_policy(BiDirPolicy::BIDIRECTIONAL_POLICY_TYPE, a);
   }

   std::string poa_name = "tweek_" + localID;

   PortableServer::POAManager_var pman = mRootPOA->the_POAManager();
   pman->activate();

   try
   {
      vprDEBUG(tweekDBG_CORBA, vprDBG_STATE_LVL)
         << "Creating child of root POA named " << poa_name << std::endl
         << vprDEBUG_FLUSH;
      mChildPOA = mRootPOA->create_POA(poa_name.c_str(), pman, policy_list);
   }
   catch (PortableServer::POA::AdapterAlreadyExists&)
   {
      status = false;
      vprDEBUG(vprDBG_ALL, vprDBG_WARNING_LVL)
         << "WARNING: Child POA named '" << poa_name << "' already exists!\n"
         << vprDEBUG_FLUSH;
   }
   catch (PortableServer::POA::InvalidPolicy&)
   {
      status = false;
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
   // NOTE: Do not put uses of vprDEBUG here. It can cause crashes on exit
   // as singletons are destroyed.
   mORB->run();
}

} // End of tweek namespace
