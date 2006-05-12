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

#include <tweek/CORBA/CorbaManager.h>
#include <vpr/System.h>
#include <vpr/Util/Debug.h>

#include <StringSubjectImpl.h>


/**
 * This application starts the CORBA server for the C++ side of the test.
 */
int main (int argc, char* argv[])
{
   tweek::CorbaManager mgr;

   // The first thing we have to do is initialize the Tweek CORBA Manager.
   // If this fails, we're out of luck.
   try
   {
      if ( mgr.init("cxx_client_test", argc, argv).success() )
      {
         vpr::ReturnStatus status;

         // Once the CORBA Manager is initialized, we need to create a
         // Subject Manager.  This will hold our StringSubject servant.
         try
         {
            status = mgr.createSubjectManager();

            // If we were able to create the Subject Manager, now we register
            // our objects with it.
            if ( status.success() )
            {
               // Add a test informational item to the Subject Manager.
               mgr.getSubjectManager()->addInfoItem("App Type",
                                                    "String subject server");

               // First, create real instances of the C++ object that will
               // be the CORBA servant.  This must be allocated on the heap.
               CxxClientTest::StringSubjectImpl* slider_subj =
                  new CxxClientTest::StringSubjectImpl();

               // Now we try to register the subjects and give them symbolic,
               // easy-to-remember names.
               try
               {
                  mgr.getSubjectManager()->registerSubject(slider_subj,
                                                           "StringSubject");
               }
               catch (...)
               {
                  vprDEBUG(vprDBG_ALL, vprDBG_CRITICAL_LVL)
                     << "Failed to register subject\n" << vprDEBUG_FLUSH;
               }

               // We are done with our pointer to the servant.
               slider_subj->_remove_ref();
            }
         }
         catch (CORBA::Exception& ex)
         {
            vprDEBUG(vprDBG_ALL, vprDBG_CRITICAL_LVL)
               << "Caught CORBA exception '" << ex._name()
               << "' when trying to register!\n" << vprDEBUG_FLUSH;
         }

         if ( ! status.success() )
         {
            vprDEBUG(vprDBG_ALL, vprDBG_CRITICAL_LVL)
               << "Failed to register Subject Manager instance\n"
               << vprDEBUG_FLUSH;
         }

         std::cout << "Press 'x' to exit" << std::endl;
         char input;

         // Loop forever so that we can act sort of like a server.
         while ( 1 )
         {
            std::cin >> input;
            if ( input == 'x' )
            {
               break;
            }
            else
            {
               vpr::System::msleep(100);
            }
         }
      }
      else
      {
         vprDEBUG(vprDBG_ALL, vprDBG_CRITICAL_LVL)
            << "CORBA failed to initialize\n" << vprDEBUG_FLUSH;
      }
   }
   catch (...)
   {
      vprDEBUG(vprDBG_ALL, vprDBG_CRITICAL_LVL)
         << "Caught an unknown exception!\n" << vprDEBUG_FLUSH;
   }

   vprDEBUG(vprDBG_ALL, vprDBG_CRITICAL_LVL) << "Exiting\n" << vprDEBUG_FLUSH;

   return 0;
}
