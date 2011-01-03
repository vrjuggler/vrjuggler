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
#include <cstdlib>
#include <string>

 // --- VR Juggler Stuff --- //
#include <vrj/Kernel/Kernel.h>
#include <simplePfApp.h>


using namespace vrj;

void usage(char** argv)
{
   std::cout << "Usage:\n"
             << "      " << argv[0]
             << " <model_name> vjconfigfile[0] vjconfigfile[1] ... vjconfigfile[n]"
             << std::endl;
}

int main(int argc, char* argv[])
{
   Kernel* kernel = Kernel::instance();    // Declare a new Kernel

   // Delcare an instance of my application
   simplePfApp* application = new simplePfApp();

   // --- CHECK FOR CORRECT ARGUMENTS ---- //
   if ( argc < 2 )
   {
      usage(argv);
      std::cout << "\n\n[ERROR!!!] you must supply a model database followed "
                << "by VR Juggler config files." << std::endl;
      return EXIT_FAILURE;
   }
   else if ( argc < 3 )
   {
      usage(argv);
      std::cout << "\n\n[ERROR!!!] you must supply VR Juggler config files "
                << "after the model file." << std::endl;
      return 2;
   }

   kernel->init(argc, argv);

   // --- GET MODEL NAME --- //
   std::string model_filename = argv[1];
   std::cout << "Will load: " << model_filename << "\n" << std::flush;

   // Load config files
   for ( int i = 2; i < argc; ++i )
   {
      kernel->loadConfigFile(argv[i]);
   }

   kernel->start();                        // Start the Kernel running

   // Configure the application
   application->setModel(model_filename);
   kernel->setApplication(application);    // Set up the kernel

   kernel->waitForKernelStop();

   delete application;

   // Tell Performer to clean up its resources.  A side effect of this call
   // is that the standard library exit() function will be called, so nothing
   // after this call to pfExit() will be executed.
   pfExit();

   return EXIT_SUCCESS;
}
