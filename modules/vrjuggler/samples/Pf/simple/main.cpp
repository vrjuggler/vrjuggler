/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998, 1999, 2000 by Iowa State University
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
 * -----------------------------------------------------------------
 * File:          $RCSfile$
 * Date modified: $Date$
 * Version:       $Revision$
 * -----------------------------------------------------------------
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

 // --- VR Juggler Stuff --- //
#include <vrj/Kernel/Kernel.h>
#include <simplePfApp.h>

void usage(char** argv)
{
   std::cout<<"Usage:\n";
   std::cout<<"      "<<argv[0]<<" <model_name> vjconfigfile[0] vjconfigfile[1] ... vjconfigfile[n]\n";
}

int main(int argc, char* argv[])
{
   vrj::Kernel* kernel = vrj::Kernel::instance(); // Declare a new Kernel
   simplePfApp* application = new simplePfApp();  // Delcare an instance of my application

   // --- CHECK FOR CORRECT ARGUMENTS ---- //
   usage(argv);

   if (argc < 2)
   {
      usage( argv );
      std::cout<<"\n\n[ERROR!!!] you must supply a model database (then config files)\n\n"<<std::flush;
      return 1;
   }
   else if (argc < 3)
   {
      std::cout<<"\n\n[ERROR!!!] you must supply config files after the model file...\n\n"<<std::flush;
      return 2;
   }

   // --- GET MODEL NAME --- //
   std::string model_filename = argv[1];
   std::cout << "Will load: " << model_filename.c_str() << "\n" << std::flush;

   // Load config files
   for ( int i = 2; i < argc; ++i )
   {
      kernel->loadConfigFile(argv[i]);
   }

   kernel->start();                            // Start the Kernel running

   // Configure the application
   application->setModel( model_filename );
   kernel->setApplication( application );    // Set up the kernel

   while (1)
   {
      usleep( 100000 );
   }
}


