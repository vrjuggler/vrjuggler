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

#ifdef USESOUND
bool initSoundEngine( const std::string& arg, SoundEngine* &engine )
{
   std::string soundConfigFile;
   if (arg == "sl") // some hacky logic.
   {
      soundConfigFile = "sound.dat";
   }
   
   else if (arg == "aw")
   {
      soundConfigFile = "sound.adf";
   }
   
   else if (arg == "nosound")
   {
      soundConfigFile = "sound.dat";
   }   
   else
   {
      engine = NULL;
      cout<<"[soundengine] Invalid option\n"<<flush;
         
      return false; //user didn't specify an option
   }
   
   std::string soundConfigFileWithPath;
   if (fileIO::fileExistsResolvePath( soundConfigFile, soundConfigFileWithPath ))
   {
      engine = SoundFactory::newEngine( arg, soundConfigFileWithPath.c_str() );
      
      if ( engine == NULL)
      {
         cout<<"[soundengine] NULL engine\n"<<flush;
         return false;
      }      
   }
      
   else
   {
      cout<<"[soundengine] Couldn't find "<<soundConfigFile<<"\n"<<flush;
      engine = NULL;
      return false;
   }    
   
   cout<<"[soundengine] Good engine\n"<<flush;
   return true; //user specified an option
}
#endif

void main(int argc, char* argv[])
{
   vjKernel* kernel = vjKernel::instance(); // Declare a new Kernel
   simplePfNavApp* application = new simplePfNavApp(kernel);  // Delcare an instance of my application

   std::string file_path( "" );
   const float dcs_scale( 1.0f );
   const vjVec3 dcs_trans( 0.0f, 0.0f, 0.0f );
   const vjVec3 initial_pos( 0.0f, 0.0f, 0.0f );
    
   if (argc < 2)
   {
      usage( argv );
      cout<<"\n\n[ERROR!!!] you must supply at least a model database (then config files)\n\n"<<flush;
      return;
   }
   
   int a = 1; // it's here so i can add in the sound 
              // stuff until we have a complex pfNav :)
   
   #ifdef USESOUND
   cout<<"[soundengine] Kevin's Sound Stuff is enabled!  1st arg needs to be the engine\n"<<flush;
   if (initSoundEngine( argv[a], gSoundEngine ))
   {
      ++a;
   }
   else
   {
      // then this will definately work.
      initSoundEngine( "nosound", gSoundEngine );
   }
   #endif
   
   if (argc < a)
   {
      usage( argv );
      cout<<"\n\n[ERROR!!!] you must supply a model database (then config files)\n\n"<<flush;
      return;
   }

   std::string model_filename = argv[a];
   ++a;
   
   if (argc < a)
   {
      cout<<"\n\n[ERROR!!!] you must supply config files after the model file...\n\n"<<flush;
      return;
   }

    for ( int i = a; i < argc; i++ ) 
   {
     kernel->loadConfigFile(argv[i]);
   }
   
    kernel->start();

    // Configure that application
    application->setFileName( model_filename );
    application->setFilePath( file_path );
    application->setWorldDcsScale( dcs_scale );
    application->setWorldDcsTrans( dcs_trans );
    application->setInitialNavPos( initial_pos );

    kernel->setApplication( application );    // Set up the kernel

    //while(!kernel->done())
   while (1)
   {
      usleep( 100000 );
   }
}


