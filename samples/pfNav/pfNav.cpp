/*
 * VRJuggler
 *   Copyright (C) 1997,1998,1999,2000
 *   Iowa State University Research Foundation, Inc.
 *   All Rights Reserved
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
 */

#include <iostream.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

 // --- VR Juggler Stuff --- //
#include <Kernel/vjKernel.h>
#include <simplePfNavApp.h>

void usage(char** argv)
{
   cout<<"_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/\n"<<flush;
   cout<<"\n"<<flush;
   cout<<"Usage:\n";
   cout<<"      "<<argv[0]<<" modelfile.flt vjconfigfile[0] vjconfigfile[1] ... vjconfigfile[n] (for plib's SL library)\n";
   cout<<"_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/\n"<<flush;
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
};

void main(int argc, char* argv[])
{
   vjKernel* kernel = vjKernel::instance(); // Declare a new Kernel
   simplePfNavApp* application = new simplePfNavApp();  // Delcare an instance of my application

   std::string file_path( "" );
   //const float dcs_scale( 1.0f );
   //const vjVec3 dcs_trans( 0.0f, 0.0f, 0.0f );
   const vjVec3 initial_pos( 0.0f, 0.0f, 0.0f );

   if (argc < 2)
   {
      usage( argv );
      cout<<"\n\n[ERROR!!!] you must supply a model database (then config files)\n\n"<<flush;
      return;
   }

   std::string model_filename = argv[1];
   cout<<"Will load: "<<model_filename<<"\n"<<flush;
   
   if (argc < 3)
   {
      cout<<"\n\n[ERROR!!!] you must supply config files after the model file...\n\n"<<flush;
      return;
   }

   cout<<"==========\n"<<flush;
   for ( int i = 2; i < argc; ++i )
   {
      cout<<"Loading Config File: "<<argv[i]<<"\n"<<flush;
     kernel->loadConfigFile(argv[i]);
   }
 cout<<"===========\n"<<flush;
   
    kernel->start();

    // Configure that application
    application->addModel( model_filename );
    application->setFilePath( file_path );
    //application->setWorldDcsScale( dcs_scale );
    //application->setWorldDcsTrans( dcs_trans );
    application->setInitialNavPos( initial_pos );

    kernel->setApplication( application );    // Set up the kernel

    //while(!kernel->done())
   while (1)
   {
      usleep( 100000 );
   }
}


