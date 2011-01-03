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

#include <iostream>
#include <string>

#include <vpr/vpr.h>
#include <vpr/System.h>
#include <vpr/Thread/Thread.h>
#include <vpr/Util/Debug.h>

#include <tweek/CORBA/CorbaManager.h>

#include <SliderSubjectImpl.h>
#include <WhiteboardSubjectImpl.h>


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
      // Bind to the default network interface and list on port 12345.
      if ( mgr.initDirect("corba_test", argc, argv, "", 12345) )
      {
         bool status(false);

         // Once the CORBA Manager is initialized, we need to create a
         // Subject Manager.  This will hold our SliderSubject and our
         // WhiteboardSubject objects.
         try
         {
            status = mgr.createSubjectManager();

            // If we were able to create the Subject Manager, now we register
            // our objects with it.
            if ( status )
            {
               // Add a test informational item to the Subject Manager.
               mgr.getSubjectManager()->addInfoItem("My Value",
                                                    "This is a test");

               // First, create real instances of the C++ objects that will
               // be the CORBA servants.  These must be allocated on the heap.
               networktest::SliderSubjectImpl* slider_subj =
                  new networktest::SliderSubjectImpl();
               networktest::WhiteboardSubjectImpl* whiteboard_subj =
                  new networktest::WhiteboardSubjectImpl();

               // Now we try to register the subjects and give them symbolic,
               // easy-to-remember names.
               try
               {
                  mgr.getSubjectManager()->registerSubject(slider_subj,
                                                           "SliderSubject");
                  mgr.getSubjectManager()->registerSubject(whiteboard_subj,
                                                           "WhiteboardSubject");
               }
               catch (...)
               {
                  vprDEBUG(vprDBG_ALL, vprDBG_CRITICAL_LVL)
                     << "Failed to register subject\n" << vprDEBUG_FLUSH;
               }

               // We are done with our pointers to the servants.
               slider_subj->_remove_ref();
               whiteboard_subj->_remove_ref();
            }
         }
         catch (CORBA::Exception& ex)
         {
            vprDEBUG(vprDBG_ALL, vprDBG_CRITICAL_LVL)
               << "Caught CORBA exception '" << ex._name()
               << "' when trying to register!\n" << vprDEBUG_FLUSH;
         }

         if ( ! status )
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
