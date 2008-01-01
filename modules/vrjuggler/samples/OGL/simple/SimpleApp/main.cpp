/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2008 by Iowa State University
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

#include <cstdlib>
#include <simpleApp.h>

// --- Lib Stuff --- //
#include <vrj/Kernel/Kernel.h>


using namespace vrj;

int main(int argc, char* argv[])
{
   // Allocate the kernel object and the application object
   Kernel* kernel = Kernel::instance();           // Get the kernel
   simpleApp* application = new simpleApp();          // Instantiate an instance of the app

#if ! defined(VRJ_USE_COCOA)
   // IF not args passed to the program
   //    Display usage information and exit
   if (argc <= 1)
   {
      std::cout << "\n\n";
      std::cout << "Usage: " << argv[0] << " vjconfigfile[0] vjconfigfile[1] ... vjconfigfile[n]" << std::endl;
      std::exit(EXIT_FAILURE);
   }
#endif

   kernel->init(argc, argv);

   // Load any config files specified on the command line
   for( int i = 1; i < argc; ++i )
   {
      kernel->loadConfigFile(argv[i]);
   }

   // Start the kernel running
   kernel->start();

   // Give the kernel an application to execute
   kernel->setApplication(application);

   // Keep thread alive and waiting
   kernel->waitForKernelStop();

   delete application;

   return EXIT_SUCCESS;
}
