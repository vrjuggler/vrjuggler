/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2010 by Iowa State University
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

#include <cstdlib>
#include <vrj/Kernel/Kernel.h>
#include "NavGrabApp.h"


int main(int argc, char* argv[])
{
   vrj::Kernel* kernel     = vrj::Kernel::instance(); // Get the kernel
   NavGrabApp* application = new NavGrabApp(kernel); // Get app instance

   kernel->init(argc, argv);

   // Load any config files specified on the command line
   for ( int i = 1; i < argc; ++i )
   {
      std::string arg_str(argv[i]);
      if(arg_str == std::string("test"))  // enable testing
      {
         application->initTesting();      // Initialize the testing
      }
      else
      {
         kernel->loadConfigFile(argv[i]);
      }
   }

   kernel->start();

   kernel->setApplication(application);               // Set application

      //- Kernel load global config  -- Environment variable
      //- App KernelConfig function
      //- Load user local config

   kernel->waitForKernelStop();

   delete application;

   return EXIT_SUCCESS;
}
