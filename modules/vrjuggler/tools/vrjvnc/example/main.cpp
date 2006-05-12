/*********** <VRJ VNC auto-copyright.pl BEGIN do not edit this line> **********
 *
 * VRJ VNC is (C) Copyright 2003-2006 by Iowa State University
 *
 * Original Authors:
 *   Patrick Hartling, Allen Bierbaum
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU General Public
 * License along with this application; if not, write to the Free
 * Software Foundation, Inc., 59 Temple Place - Suite 330, Boston,
 * MA 02111-1307, USA.
 *
 ************ <VRJ VNC auto-copyright.pl END do not edit this line> **********/

//----------------------------------------
// Test OpenGL VR Juggler program
//
// main.cpp
//----------------------------------------
#include <iostream>
#include <stdlib.h>
#include <vncApp.h>

      // --- Lib Stuff --- //
#include <vrj/Kernel/Kernel.h>


int main(int argc, char* argv[])
{
   vrj::Kernel* kernel = vrj::Kernel::instance();           // Get the kernel
   vrjvnc::vncApp* application = new vrjvnc::vncApp(kernel);       // Instantiate an instance of the app

   if (argc <= 1)
   {
      // display some usage info (holding the user by the hand stuff)
      //  this will probably go away once the kernel becomes separate
      //  and can load application plugins.
      std::cout << "\n\n" << std::flush;
      std::cout << "Usage: " << argv[0]
                << " vjconfigfile[0] vjconfigfile[1] ... vjconfigfile[n]\n"
                << std::flush;
      std::cout << "\n\n" << std::flush;
      exit(1);
   }

   // Load any config files specified on the command line
   for ( int i = 1; i < argc; ++i )
   {
      kernel->loadConfigFile(argv[i]);
   }

   kernel->start();

   /*
   for ( int i = 0; i < 20; ++i )
   {
      vpr::System::usleep(50000);
      std::cout << i << "." << std::flush;
   }
   */

   kernel->setApplication(application);

   kernel->waitForKernelStop();

   return 0;
}
