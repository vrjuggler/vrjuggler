#include <iostream>
#include <string>

#include <vpr/vpr.h>
#include <vpr/Util/Debug.h>
#include <tweek/Client/CorbaService.h>
#include <tweek/CORBA/SubjectManager.h>

#include <StringObserverImpl.h>


static tweek::SubjectManager_var chooseSubjectManager(tweek::CorbaService& corbaService)
{
   tweek::SubjectManager_var subj_mgr;

   // Request all the active Subject Manager references.
   std::list<tweek::SubjectManager_var> mgrs =
      corbaService.getSubjectManagerList();

   std::list<tweek::SubjectManager_var>::iterator cur_mgr;

   for ( cur_mgr = mgrs.begin(); cur_mgr != mgrs.end(); ++cur_mgr )
   {
      try
      {
         if ( ! (*cur_mgr)->_non_existent() )
         {
            std::string response;

            tweek::SubjectManager::SubjectManagerInfoList_var mgr_info =
               (*cur_mgr)->getInfo();

            std::cout << "\nSubject Manager information:" << std::endl;

            for ( CORBA::ULong i = 0; i < mgr_info->length(); ++i )
            {
               std::cout << "\t" << mgr_info[i].key  << " = "
                         << mgr_info[i].value << std::endl;
            }

            std::cout << "Use this Subject Manager (y/n)? ";
            std::cin >> response;

            if ( response == std::string("y") )
            {
               subj_mgr = *cur_mgr;
               break;
            }
         }
      }
      // XXX: Make this more specific.
      catch (...)
      {
         vprDEBUG(vprDBG_ALL, vprDBG_CRITICAL_LVL)
            << "Caught an unknown exception in chooseSubjectManager loop\n"
            << vprDEBUG_FLUSH;
      }
   }

   return subj_mgr;
}

int main(int argc, char* argv[])
{
   std::string ns_host, iiop_ver;
   vpr::Uint16 ns_port;

   std::cout << "Naming Service host: ";
   std::cin >> ns_host;

   std::cout << "Naming Service port (usually 2809): ";
   std::cin >> ns_port;

   std::cout << "IIOP version (usually 1.0): ";
   std::cin >> iiop_ver;

   tweek::CorbaService corba_service(ns_host, ns_port, iiop_ver);

   try
   {
      if ( corba_service.init(argc, argv).success() )
      {
         vpr::ReturnStatus status;

         // This will hold the reference to the Subject Manager we use.
         tweek::SubjectManager_var subj_mgr =
            chooseSubjectManager(corba_service);

         if ( ! CORBA::is_nil(subj_mgr) )
         {
            tweek::Subject_var subj = subj_mgr->getSubject("StringSubject");

            if ( ! CORBA::is_nil(subj) )
            {
               try
               {
                  CxxClientTest::StringSubject_var string_subj =
                     CxxClientTest::StringSubject::_narrow(subj);

                  StringObserverImpl* string_observer =
                     new StringObserverImpl(string_subj);

                  PortableServer::ObjectId_var observer_id =
                     corba_service.registerObject(string_observer,
                                                   "StringObserver");

                  // This could be done in the StringObserverImpl constructor,
                  // but we do it here in this example just to make it clear
                  // that it is important.
                  string_subj->attach(string_observer->_this());

                  const std::string exit_string("Q");
                  std::string cur_string;

                  for ( ;; )
                  {
                     std::cout << "Enter a string (Q to quit): ";
                     std::cin >> cur_string;

                     if ( exit_string != cur_string )
                     {
                        string_subj->setValue(cur_string.c_str());
                     }
                     else
                     {
                        break;
                     }
                  }

                  // We're done, so now we have to clean up after ourselves.
                  corba_service.unregisterObject(observer_id);

                  // XXX: This causes a bus error.  Why?
                  delete string_observer;
               }
               // XXX: Make this more specific.
               catch (...)
               {
               }
            }
         }
         else
         {
            vprDEBUG(vprDBG_ALL, vprDBG_CRITICAL_LVL)
               << "No Subject Manager chosen--exiting.\n" << vprDEBUG_FLUSH;
         }
      }
      else
      {
         vprDEBUG(vprDBG_ALL, vprDBG_CRITICAL_LVL)
            << "CORBA Service failed to initialize\n" << vprDEBUG_FLUSH;
      }
   }
   catch (...)
   {
      vprDEBUG(vprDBG_ALL, vprDBG_CRITICAL_LVL)
         << "Caught an unknown exception!\n" << vprDEBUG_FLUSH;
   }

   return 0;
}
