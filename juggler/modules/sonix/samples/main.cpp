
/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998, 1999, 2000, 2001 by Iowa State University
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

/****************** <SNX heading BEGIN do not edit this line> *****************
 *
 * Juggler Juggler
 *
 * Original Authors:
 *   Kevin Meinert, Carolina Cruz-Neira
 *
 * -----------------------------------------------------------------
 * File:          $RCSfile$
 * Date modified: $Date$
 * Version:       $Revision$
 * -----------------------------------------------------------------
 *
 ****************** <SNX heading END do not edit this line> ******************/

#include <iostream>

#include <unistd.h>
#include "snx/sonix.h"    // interface
#include "snx/FileIO.h"

void usage( int argc, char* argv[] )
{
   std::cout<<"Usage: "<<argv[0]<<" apiname filename\n"<<std::flush;
   std::cout<<"       "<<argv[0]<<" OpenAL sample.wav\n"<<std::flush;
   std::cout<<"       "<<argv[0]<<" AudioWorks sample.aifc\n"<<std::flush;
}

int main( int argc, char* argv[] )
{
   char bok[] = "../../data/sample.wav";
   std::string filename, api;
   
   if (argc == 1 || argc == 2)
   {
      api = "OpenAL";
      filename = bok;
      usage( argc, argv );
   }
   if (argc == 2 || argc == 3)
   {
      api = argv[1];
   }
   if (argc == 3)
   {
      filename = argv[2];
   }
   if (argc > 3)
   {
      usage( argc, argv );
      return 0;
   }

   if (!snxFileIO::fileExists( filename.c_str() ))
   {
      std::cout << "File not found: " << filename << "\n" << std::flush;
      return 0;
   }
   
   snx::SoundInfo si;
   si.filename = filename;
   si.datasource = snx::SoundInfo::FILESYSTEM;
      
   
   std::cout<<"sonix: \n" << std::flush;
  
   //snx.startAPI();
   std::cout<<"associate: \n" << std::flush;
   sonix::instance()->configure( "kevin", si );
   
   std::cout<<"trigger: \n" << std::flush;
   sonix::instance()->trigger( "kevin" );
   
   std::cout<<"sleep: \n" << std::flush;
   sleep( 1 );
   
   sonix::instance()->changeAPI( api );
   
   std::cout<<"trigger: \n" << std::flush;
   sonix::instance()->trigger( "kevin" );
   
   std::cout<<"sleep: \n" << std::flush;
   sleep( 3 );
   
   sonix::instance()->changeAPI( "stub" );
   
   sleep( 1 );
   
   sonix::instance()->changeAPI( api );
   
   std::cout<<"trigger: \n" << std::flush;
   sonix::instance()->trigger( "kevin" );
   
   std::cout<<"sleep: \n" << std::flush;
   sleep( 3 );
   
   sonix::instance()->changeAPI( "stub" );
   
   return 1;
}


/*

#include "snxSoundFactory.h"
#include "snxSoundImplementation.h"
#include "snxAudioWorksSoundImplementation.h"  
#include "snxSoundInfo.h"
#include "snxOpenALSoundImplementation.h"      
#include "snxStubSoundImplementation.h"

*/
