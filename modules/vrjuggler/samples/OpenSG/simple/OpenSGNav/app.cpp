/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2009 by Iowa State University
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
#include <OpenSGNav.h>

#include <vrj/Kernel/Kernel.h>


int main(int argc, char* argv[])
{
   vrj::Kernel* kernel = vrj::Kernel::instance();
   OpenSGNav* application = new OpenSGNav(kernel);

#if ! defined(VRJ_USE_COCOA)
   if ( argc <= 2 )
   {
      std::cout << "\n\nUsage: " << argv[0]
                << " modelname vjconfigfile[0] vjconfigfile[1] ... vjconfigfile[n]\n"
                << std::flush;
      std::exit(EXIT_FAILURE);
   }
#endif

   kernel->init(argc, argv);

   application->setModelFileName(std::string(argv[1]));

   // Load any config files specified on the command line
   for ( int i = 2; i < argc; ++i )
   {
      kernel->loadConfigFile(argv[i]);
   }
    
   kernel->start();
   kernel->setApplication(application);
   kernel->waitForKernelStop();

   delete application;

   return EXIT_SUCCESS;
}
