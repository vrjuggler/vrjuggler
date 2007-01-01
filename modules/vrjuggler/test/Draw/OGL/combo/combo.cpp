/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2007 by Iowa State University
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

//----------------------------------------
// Test OpenGL VR Juggler program
//
//  Test switching GL applications.
//
// torus.cpp
//----------------------------------------
#include <torusApp.h>
#include <wandApp.h>
#include <cubesApp.h>

      // --- Lib Stuff --- //
#include <vrj/Kernel/Kernel.h>
#include <vrj/Display/Projection.h>
#include <vpr/System.h>

int main(int argc, char* argv[])
{
   vrj::Projection::setNearFar(0.01f, 10000.0f);

   vrj::Kernel* kernel = vrj::Kernel::instance();        // Get the kernel
   vrjTest::torusApp* torus_app = new vrjTest::torusApp(kernel);   // Declare an instance of the app
   vrjTest::wandApp*  wand_app  = new vrjTest::wandApp(kernel);
   cubesApp* cubes_app          = new cubesApp(kernel);

   if (argc <= 1)
   {
      // display some usage info (holding the user by the hand stuff)
      //  this will probably go away once the kernel becomes separate
      //  and can load application plugins.
      std::cout<<"\n"<<std::flush;
      std::cout<<"\n"<<std::flush;
      std::cout<<"Usage: "<<argv[0]
               <<" vjconfigfile[0] vjconfigfile[1] ... vjconfigfile[n]\n"
               <<std::flush;
      std::cout<<"\n"<<std::flush;
      std::cout<<"\n"<<std::flush;
      exit(1);
   }

   // Load any config files specified on the command line
   for ( int i = 1; i < argc; ++i )
   {
      kernel->loadConfigFile(argv[i]);
   }

   kernel->start();
      //- Kernel load global config  -- Environment variable
      //- App KernelConfig function
      //- Load user local config

   kernel->setApplication(torus_app);         // Set application

   // Run each application object three times.
   for ( int i = 0; i < 3; ++i )
   {
      vprDEBUG(vprDBG_ALL, vprDBG_CRITICAL_LVL)
         << "main loop pass #" << i << std::endl << vprDEBUG_FLUSH;
      vpr::System::sleep(5);
      kernel->setApplication(wand_app);
      vpr::System::sleep(5);
      kernel->setApplication(cubes_app);
      vpr::System::sleep(5);
      kernel->setApplication(torus_app);

      vpr::Thread::yield();
   }

   kernel->stop();
   kernel->waitForKernelStop();

   delete wand_app;
   delete cubes_app;
   delete torus_app;

   return 0;
}
