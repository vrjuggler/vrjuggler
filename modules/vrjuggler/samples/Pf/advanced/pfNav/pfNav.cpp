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
 * -----------------------------------------------------------------
 * File:          $RCSfile$
 * Date modified: $Date$
 * Version:       $Revision$
 * -----------------------------------------------------------------
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

#include <iostream>
#include <string>

#include <gmtl/Vec.h>

 // --- VR Juggler Stuff --- //
#include <vrj/Kernel/Kernel.h>
#include <simplePfNavApp.h>

void usage(char** argv)
{
   std::cout << "_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/\n\n"
             << "Usage:\n"
             << "      " << argv[0] << " modelfile.flt bump_thresh "
             << "vjconfigfile[0] vjconfigfile[1] ... vjconfigfile[n]\n"
             << "_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/"
             << std::endl;
}

int main(int argc, char* argv[])
{
   vrj::Kernel* kernel = vrj::Kernel::instance(); // Declare a new Kernel

   // Delcare an instance of my application
   simplePfNavApp* application = new simplePfNavApp();

   std::string file_path("");
   const float dcs_scale(1.0f);
   const gmtl::Vec3f dcs_trans(0.0f, 0.0f, 0.0f);
   const gmtl::Vec3f dcs_rot(0.0f, 0.0f, 0.0f);
   const gmtl::Vec3f initial_pos(0.0f, 0.0f, 0.0f);

   if ( argc < 2 )
   {
      usage(argv);
      std::cout << "\n\n[ERROR!!!] you must supply a model database followed "
                << "by VR Juggler config files." << std::endl;
      return 1;
   }
   
   std::string model_filename = argv[1];
   std::cout << "Will load: " << model_filename << std::endl;

   if ( argc < 3 )
   {
      usage(argv);
      std::cout << "\n\n[ERROR!!!] you must supply VR Juggler config files "
                << "after the model file." << std::endl;
      return 2;
   }

   std::cout << "==========" << std::endl;
   for ( int i = 2; i < argc; ++i )
   {
      std::cout << "Loading Config File: " << argv[i] << std::endl;
      kernel->loadConfigFile(argv[i]);
   }

   std::cout << "===========" << std::endl;

   kernel->start();

   // Configure that application
   application->addModel(simplePfNavApp::Model(model_filename, model_filename,
                                               dcs_scale, dcs_trans, dcs_rot,
                                               true));
   application->setFilePath(file_path);
   application->setInitialNavPos(initial_pos);

   kernel->setApplication(application);    // Set up the kernel

   kernel->waitForKernelStop();

   delete application;

   // Tell Performer to clean up its resources.  A side effect of this call
   // is that the standard library exit() function will be called, so nothing
   // after this call to pfExit() will be executed.
   pfExit();

   return 0;
}
