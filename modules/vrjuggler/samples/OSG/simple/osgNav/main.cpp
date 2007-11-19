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
// Test Osg VR Juggler program
//
// main.cpp
//----------------------------------------

#include <cstdlib>
#include <OsgNav.h>

// --- Lib Stuff --- //
#include <vrj/Kernel/Kernel.h>

//#define OSG_USE_IO_DOT_H


int main(int argc, char* argv[])
{
   vrj::Kernel* kernel = vrj::Kernel::instance();  // Get the kernel
   OsgNav* application = new OsgNav(kernel, argc, argv);             // Instantiate an instance of the app

   if ( argc <= 2 )
   {
      // display some usage info (holding the user by the hand stuff)
      //  this will probably go away once the kernel becomes separate
      //  and can load application plugins.
      std::cout<<"\n"<<std::flush;
      std::cout<<"\n"<<std::flush;

      std::cout << "Usage: " << argv[0]
         << " modelname vjconfigfile[0] vjconfigfile[1] ... vjconfigfile[n]\n"
         << std::endl << std::endl;

      std::exit(1);
   }

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

   return 0;
}
