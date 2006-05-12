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
 *************** <auto-copyright.pl END do not edit this line> ***************/

#include <tweek/tweekConfig.h>

#include <cstdio>
#include <cstdlib>
#include <sstream>
#include <string>
#include <boost/concept_check.hpp>
#include <boost/bind.hpp>

#include <vpr/vpr.h>
#include <vpr/Util/Assert.h>
#include <vpr/Util/Debug.h>

#include <tweek/Util/Debug.h>
#include <tweek/Util/Version.h>
#include <tweek/CORBA/CorbaHelpers.h>
#include <tweek/Client/CorbaService.h>


namespace tweek
{

CorbaService::CorbaService(const std::string& nsHost, vpr::Uint16 nsPort,
                           const std::string& iiopVersion,
                           const std::string& subContextId)
   : mOrbThread(NULL)
   , mNsHost(nsHost)
   , mNsPort(nsPort)
   , mNameServiceURI("corbaloc:iiop:")
   , mSubContextId(subContextId)
{
   std::ostringstream port_stream;
   port_stream << nsPort;

   mNameServiceURI += iiopVersion;
   mNameServiceURI += std::string("@");
   mNameServiceURI += nsHost;
   mNameServiceURI += std::string(":");
   mNameServiceURI += port_stream.str();
   mNameServiceURI += std::string("/NameService");

   vprDEBUG(tweekDBG_CORBA, vprDBG_VERB_LVL)
      << "Naming Service URI: " << mNameServiceURI << std::endl
      << vprDEBUG_FLUSH;

   std::string tweek_ver = getVersionString();

   // Print out the Tweek version information.
   vprDEBUG(tweekDBG_CORBA, vprDBG_CRITICAL_LVL)
      << std::string(tweek_ver.length() + 14, '=') << std::endl
      << vprDEBUG_FLUSH;
   vprDEBUG(tweekDBG_CORBA, vprDBG_CRITICAL_LVL)
      << clrOutNORM(clrGREEN, "Tweek Client: ")
      << clrOutNORM(clrGREEN, tweek_ver) << clrRESET << std::endl
      << vprDEBUG_FLUSH;
   vprDEBUG(tweekDBG_CORBA, vprDBG_CRITICAL_LVL)
      << std::string(tweek_ver.length() + 14, '=') << std::endl
      << vprDEBUG_FLUSH;
}

vpr::ReturnStatus CorbaService::init(int& argc, char* argv[])
{
   vpr::ReturnStatus status;

   try
   {
      // Initialize the ORB.
      vprDEBUG(tweekDBG_CORBA, vprDBG_STATE_LVL)
         << "Initializing client ORB (using init string '"
         << TWEEK_ORB_VER_STRING << "')\n" << vprDEBUG_FLUSH;
      mORB = CORBA::ORB_init(argc, argv, TWEEK_ORB_VER_STRING);

      initRootPOA();

      try
      {
         mRootContext = tweek::getRootNamingContextByURI(mORB, mNameServiceURI);

         if ( ! CORBA::is_nil(mRootContext) )
         {
            vprDEBUG(tweekDBG_CORBA, vprDBG_CRITICAL_LVL)
               << "Got root context, now binding local context\n"
               << vprDEBUG_FLUSH;
            mLocalContext =
               tweek::bindLocalNamingContext(mRootContext,
                                             std::string("tweek"));
         }
      }
      catch (CORBA::ORB::InvalidName& ex)
      {
         boost::ignore_unused_variable_warning(ex);

         // This should not happen!
         vprDEBUG(vprDBG_ALL, vprDBG_CRITICAL_LVL)
            << "NameService name invalid in CorbaService::init!\n"
            << vprDEBUG_FLUSH;
      }

      vprDEBUG(tweekDBG_CORBA, vprDBG_STATE_LVL) << "Starting ORB thread\n"
                                                 << vprDEBUG_FLUSH;

      mOrbThread = new vpr::Thread(boost::bind(&CorbaService::run, this));
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
#ifdef TWEEK_USE_OMNIORB
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

void CorbaService::shutdown(bool waitForCompletion)
{
   if ( ! CORBA::is_nil(mRootPOA) )
   {
      // We want to etherialize objects (destroy registered servants).  This
      // will destroy all descendant POAs, so we do not have to worry about
      // them at all.
      mRootPOA->destroy(true, waitForCompletion);
   }

   if ( ! CORBA::is_nil(mORB) )
   {
      mORB->shutdown(waitForCompletion);
   }
}

std::list<tweek::SubjectManager_var> CorbaService::getSubjectManagerList()
{
   std::list<tweek::SubjectManager_var> mgr_list;

   const CORBA::ULong data_size(100);

   CosNaming::BindingList_var     binding_list;
   CosNaming::BindingIterator_var binding_iter;

   mLocalContext->list(data_size, binding_list, binding_iter);

   addSubjectManagers(binding_list, mgr_list);

   if ( ! CORBA::is_nil(binding_iter) )
   {
      while ( binding_iter->next_n(data_size, binding_list) )
      {
         addSubjectManagers(binding_list, mgr_list);
      }

      binding_iter->destroy();
   }

   return mgr_list;
}

PortableServer::ObjectId_var CorbaService::registerObject(PortableServer::ServantBase* servant,
                                                          const std::string& name)
{
   // XXX: Why is name unused?  It doesn't seem like it should even be a
   // parameter.
   boost::ignore_unused_variable_warning(name);

   PortableServer::ObjectId_var obj_id;

   try
   {
      obj_id = mRootPOA->activate_object(servant);
   }
   catch (PortableServer::POA::ServantAlreadyActive& activeEx)
   {
      boost::ignore_unused_variable_warning(activeEx);
   }
   catch (PortableServer::POA::WrongPolicy& policyEx)
   {
      boost::ignore_unused_variable_warning(policyEx);
   }

   return obj_id;
}

void CorbaService::unregisterObject(PortableServer::ObjectId_var id)
{
   // XXX: Is there a way to verify that the id is valid first?
//   if ( ! PortableServer::is_nil(id) )
//   {
      try
      {
         mRootPOA->deactivate_object(id);
      }
      catch (PortableServer::POA::ObjectNotActive& activeEx)
      {
         boost::ignore_unused_variable_warning(activeEx);
         vprDEBUG(tweekDBG_CORBA, vprDBG_CRITICAL_LVL)
            << "CorbaService::unregisterObject: Tried to deactivate an inactive object\n"
            << vprDEBUG_FLUSH;
      }
      catch (PortableServer::POA::WrongPolicy& policyEx)
      {
         boost::ignore_unused_variable_warning(policyEx);
         vprDEBUG(tweekDBG_CORBA, vprDBG_CRITICAL_LVL)
            << "CorbaService::unregisterObject: Wrong POA policy\n"
            << vprDEBUG_FLUSH;
      }
//   }
//   else
//   {
//      vprDEBUG(tweekDBG_CORBA, vprDBG_WARNING_LVL)
//         << clrOutNORM(clrYELLOW, "WARNING:")
//         << " Tried to unregister null ID in CorbaService::unregisterObject()!"
//         << std::endl << vprDEBUG_FLUSH;
//   }
}

vpr::ReturnStatus CorbaService::initRootPOA()
{
   vpr::ReturnStatus status;

   CORBA::Object_var obj;

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

   return status;
}

void CorbaService::addSubjectManagers(const CosNaming::BindingList& bindingList,
                                      std::list<tweek::SubjectManager_var>& mgrList)
{
   CosNaming::Binding binding;

   for ( CORBA::ULong i = 0; i < bindingList.length(); ++i )
   {
      binding = bindingList[i];

      // We do not care about anything that is a naming context.
      if ( CosNaming::ncontext != binding.binding_type )
      {
         const std::string subj_mgr_name("SubjectManager");
         bool substr_match(false);

#if defined(__GNUC__) && __GNUC__ == 2
         // XXX: This is a hack to deal with GCC 2.96 (and earlier?) having
         // a std::string::compare() method that does not match the actual
         // STL prototype.
         substr_match = (subj_mgr_name.compare(binding.binding_name[0].id, 0,
                                               subj_mgr_name.size()) == 0);
#else
         substr_match = (subj_mgr_name.compare(0, subj_mgr_name.size(),
                                               binding.binding_name[0].id));
#endif

         // Furthermore, we only care about SubjectManager* instances.
         if ( substr_match )
         {
            CosNaming::Name name_comp = binding.binding_name;

            try
            {
               CORBA::Object_var ref = mLocalContext->resolve(name_comp);
               tweek::SubjectManager_var mgr = tweek::SubjectManager::_narrow(ref);

               try
               {
                  // Filter out any references that we know are not usable.
                  if ( ! mgr->_non_existent() )
                  {
                     mgrList.push_back(mgr);
                  }
               }
               // In the Java equivalent of this method, CORBA::TRANSIENT
               // exceptions are sometimes thrown by the call to
               // tweek::SubjectManager::_non_existent() above.  I don't know
               // if they are also thrown here, but we might as well be safe.
               catch (CORBA::TRANSIENT ex)
               {
                  boost::ignore_unused_variable_warning(ex);
                  vprDEBUG(tweekDBG_CORBA, vprDBG_WARNING_LVL)
                     << "addSubjectManagers(): Caught CORBA::TRANSIENT "
                     << "exception thrown by _non_existent\n"
                     << vprDEBUG_FLUSH;
               }
               // XXX: Figure out what exception(s) can be thrown by
               // CORBA::Object::_non_existent()!
               catch (...)
               {
                  vprDEBUG(tweekDBG_CORBA, vprDBG_CRITICAL_LVL)
                     << "addSubjectManagers(): Caught unknown exception "
                     << "thrown by _non_existent\n"
                     << vprDEBUG_FLUSH;
               }
            }
            catch (CosNaming::NamingContext::InvalidName& nameEx)
            {
               boost::ignore_unused_variable_warning(nameEx);
               vprDEBUG(tweekDBG_CORBA, vprDBG_CRITICAL_LVL)
                  << "addSubjectManagers(): Tried to resolve invalid name\n"
                  << vprDEBUG_FLUSH;
            }
            catch (CosNaming::NamingContext::CannotProceed& proceedEx)
            {
               vprDEBUG(tweekDBG_CORBA, vprDBG_CRITICAL_LVL)
                  << "addSubjectManagers(): Cannot proceed with resolution of '"
                  << proceedEx.rest_of_name[0].id << "'\n" << vprDEBUG_FLUSH;
            }
            catch (CosNaming::NamingContext::NotFound& notFoundEx)
            {
               vprDEBUG(tweekDBG_CORBA, vprDBG_CRITICAL_LVL)
                  << "addSubjectManagers(): No binding for name '"
                  << notFoundEx.rest_of_name[0].id << "' " << vprDEBUG_FLUSH;

               switch ( notFoundEx.why )
               {
                  case CosNaming::NamingContext::missing_node:
                     vprDEBUG_CONT(tweekDBG_CORBA, vprDBG_CRITICAL_LVL)
                        << "(missing node)" << std::endl << vprDEBUG_FLUSH;
                     break;
                  case CosNaming::NamingContext::not_context:
                     vprDEBUG_CONT(tweekDBG_CORBA, vprDBG_CRITICAL_LVL)
                        << "(not a context)" << std::endl << vprDEBUG_FLUSH;
                     break;
                  case CosNaming::NamingContext::not_object:
                     vprDEBUG_CONT(tweekDBG_CORBA, vprDBG_CRITICAL_LVL)
                        << "(not an object)" << std::endl << vprDEBUG_FLUSH;
                     break;
               }
            }
         }
         else
         {
            vprDEBUG(tweekDBG_CORBA, vprDBG_CRITICAL_LVL)
               << "addSubjectManagers(): Skipping binding with name '"
               << binding.binding_name[0].id << "'\n" << vprDEBUG_FLUSH;
         }
      }
   }
}

void CorbaService::run()
{
   vprDEBUG(tweekDBG_CORBA, vprDBG_STATE_LVL) << "Server is running!\n"
                                              << vprDEBUG_FLUSH;

//   PortableServer::POAManager_var pman = mChildPOA->the_POAManager();
   PortableServer::POAManager_var pman = mRootPOA->the_POAManager();

   pman->activate();
   mORB->run();
//   mORB->destroy();

   vprDEBUG(tweekDBG_CORBA, vprDBG_STATE_LVL) << "Server has shut down\n"
                                              << vprDEBUG_FLUSH;
}

} // End of tweek namespace
