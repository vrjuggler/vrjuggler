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
#include <simplePfNavApp.h>

void usage(char** argv)
{
   std::cout<<"_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/\n"<<std::flush;
   std::cout<<"\n"<<std::flush;
   std::cout<<"Usage:\n";
   std::cout<<"      "<<argv[0]<<" modelfile.flt vjconfigfile[0] vjconfigfile[1] ... vjconfigfile[n] (for plib's SL library)\n";
   std::cout<<"_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/\n"<<std::flush;
}

// The app class we want to use
// We are required to have a default constructor
// so create a new class whose constructor configures the simplePfNavApp
//
class myPfNavApp : public simplePfNavApp
{
public:
   myPfNavApp()
   {
   }

   // Empty virtual destructor.
   virtual ~myPfNavApp (void) {
      /* Do nothing. */ ;
   }
};

int main(int argc, char* argv[])
{
   vrj::Kernel* kernel = vrj::Kernel::instance(); // Declare a new Kernel
   simplePfNavApp* application = new simplePfNavApp();  // Delcare an instance of my application

   std::string file_path( "" );
   const float dcs_scale( 1.0f );
   const vrj::Vec3 dcs_trans( 0.0f, 0.0f, 0.0f );
   const vrj::Vec3 dcs_rot(0.0,0.0,0.0);
   const vrj::Vec3 initial_pos( 0.0f, 0.0f, 0.0f );

   if (argc < 2)
   {
      usage( argv );
      std::cout<<"\n\n[ERROR!!!] you must supply a model database (then config files)\n\n"<<std::flush;
      return 1;
   }

   std::string model_filename = argv[1];
   std::cout<<"Will load: "<<model_filename.c_str()<<"\n"<<std::flush;

   if (argc < 3)
   {
      std::cout<<"\n\n[ERROR!!!] you must supply config files after the model file...\n\n"<<std::flush;
      return 2;
   }

   std::cout<<"==========\n"<<std::flush;
   for ( int i = 2; i < argc; ++i )
   {
      std::cout<<"Loading Config File: "<<argv[i]<<"\n"<<std::flush;
     kernel->loadConfigFile(argv[i]);
   }
   std::cout<<"===========\n"<<std::flush;

    kernel->start();

    // Configure that application
    application->addModel( simplePfNavApp::Model(model_filename,model_filename,dcs_scale,dcs_trans, dcs_rot,true));
    application->setFilePath( file_path );
    application->setInitialNavPos( initial_pos );

    kernel->setApplication( application );    // Set up the kernel

    //while(!kernel->done())
   while (1)
   {
      usleep( 100000 );
   }
}


