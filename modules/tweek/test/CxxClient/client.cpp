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

#include <iostream>
#include <string>

#include <vpr/vpr.h>
#include <vpr/Util/Debug.h>
#include <tweek/Client/CorbaService.h>
#include <tweek/idl/SubjectManager.h>

#include <StringObserverImpl.h>


/**
 * Using the given CORBA Service object, we will try to choose a Subject
 * Manager for use with this client application.  The user will be presented
 * with choices of valid Subject Manager references.
 */
static tweek::SubjectManager_var chooseSubjectManager(tweek::CorbaService& corbaService)
{
   tweek::SubjectManager_var subj_mgr;

   // Request all the active Subject Manager references.
   std::list<tweek::SubjectManager_var> mgrs =
      corbaService.getSubjectManagerList();

   std::list<tweek::SubjectManager_var>::iterator cur_mgr;

   // Iterate over all the tweek::SubjectManager references we have.
   // XXX: This one-way iterative approach to choosing the reference is not
   // very good.  However, it should serve to demonstrate the basic idea.
   for ( cur_mgr = mgrs.begin(); cur_mgr != mgrs.end(); ++cur_mgr )
   {
      try
      {
         // It is not entirely safe to assume that *cur_mgr is still valid at
         // this point, even though it was valid when the mgrs list was
         // constructed.  Hence, we test it again now.
         if ( ! (*cur_mgr)->_non_existent() )
         {
            std::string response;
            const std::string proceed("y");

            tweek::SubjectManager::SubjectManagerInfoList_var mgr_info =
               (*cur_mgr)->getInfo();

            std::cout << "\nSubject Manager information:" << std::endl;

            // Loop over the information items and print each key/value pair
            // to the screen.
            for ( CORBA::ULong i = 0; i < mgr_info->length(); ++i )
            {
               std::cout << "\t" << mgr_info[i].key  << " = "
                         << mgr_info[i].value << std::endl;
            }

            std::cout << "Use this Subject Manager (y/n)? ";
            std::cin >> response;

            if ( proceed == response )
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

   // Create the local CORBA Service using the Naming Service URI information
   // we just collected.
   tweek::CorbaService corba_service(ns_host, ns_port, iiop_ver);

   try
   {
      // Attempt to initialize the CORBA Service.
      if ( corba_service.init(argc, argv).success() )
      {
         // This will hold the reference to the Subject Manager we use.
         tweek::SubjectManager_var subj_mgr =
            chooseSubjectManager(corba_service);

         // Verify that we actually got a Subject Manager reference back
         // from chooseSubjectManager.
         if ( ! CORBA::is_nil(subj_mgr) )
         {
            // Request the Subject with which we will communicate.  This
            // hard-coded Subject name is not necessarily a good thing.
            tweek::Subject_var subj = subj_mgr->getSubject("StringSubject");

            // If the Subject Manager knows about the Subject named above,
            // then we are good to go.
            if ( ! CORBA::is_nil(subj) )
            {
               StringObserverImpl* string_observer(NULL);
               PortableServer::ObjectId_var observer_id;

               try
               {
                  // Attempt to narrow subj to the more specific reference type
                  // CxxClientTest::StringSubject_var.  If this fails, an
                  // exception will be thrown and caught below.
                  CxxClientTest::StringSubject_var string_subj =
                     CxxClientTest::StringSubject::_narrow(subj);

                  // Request the current value before we create the Observer.
                  // In this way, we can see the persistent state maintained
                  // by the Subject.
                  char* cur_value = string_subj->getValue();
                  std::cout << "Current string value is '" << cur_value << "'"
                            << std::endl;
                  delete cur_value;

                  // Create our Observer servant.
                  string_observer = new StringObserverImpl(string_subj);

                  // Register the newly created servant with our ORB's POA.
                  observer_id = corba_service.registerObject(string_observer,
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
               }
               // XXX: Make this more specific.
               catch (...)
               {
                  vprDEBUG(vprDBG_ALL, vprDBG_CRITICAL_LVL)
                     << "Caught an unknown exception during object interaction!\n"
                     << vprDEBUG_FLUSH;
               }

               // We're done, so now we have to clean up after ourselves.
               // The order of operations here is important.
               if ( NULL != string_observer )
               {
                  string_observer->detach();
                  corba_service.unregisterObject(observer_id);
                  delete string_observer;
               }
            }
         }
         // We did not get a Subject Manager reference back for some reason.
         else
         {
            vprDEBUG(vprDBG_ALL, vprDBG_CRITICAL_LVL)
               << "No Subject Manager chosen--exiting.\n" << vprDEBUG_FLUSH;
         }
      }
      // The CORBA Service initialization failed.
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
