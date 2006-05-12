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
 *************** <auto-copyright.pl END do not edit this line> ***************/

/****************** <SNX heading BEGIN do not edit this line> *****************
 *
 * Juggler Juggler
 *
 * Original Authors:
 *   Kevin Meinert, Carolina Cruz-Neira
 *
 ****************** <SNX heading END do not edit this line> ******************/

#include <iostream>

#include <unistd.h>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/exception.hpp>

#include <snx/sonix.h> 


int main( int argc, char* argv[] )
{
   std::string filename, api;
   
   if (argc == 3)
   {
      api = argv[1];
      filename = argv[2];
   }
   else
   {
      std::cout<<"Usage: "<<argv[0]<<" apiname filename\n"<<std::flush;
      std::cout<<"       "<<argv[0]<<" OpenAL sample.wav\n"<<std::flush;
      std::cout<<"       "<<argv[0]<<" AudioWorks sample.aifc\n"<<std::flush;
      std::cout<<"       "<<argv[0]<<" Audiere sample.wav\n"<<std::flush;
      return 0;
   }

   try
   {
      boost::filesystem::path file_path(filename, boost::filesystem::native);
      if ( ! boost::filesystem::exists(file_path) )
      {
         std::cout << "File not found: " << filename << "\n" << std::flush;
         return 0;
      }
   }
   catch (boost::filesystem::filesystem_error& ex)
   {
      std::cerr << "File system exception caught while trying to load\n"
                << filename << std::endl;
      std::cerr << ex.what() << std::endl;
      return -1;
   }

   snx::SoundInfo si;
   si.filename = filename;
   si.datasource = snx::SoundInfo::FILESYSTEM;
      
   
   std::cout<<"sonix: \n" << std::flush;
  
   //snx.startAPI();
   std::cout<<"associate: \n" << std::flush;
   snx::sonix::instance()->configure( "kevin", si );
   
   std::cout<<"trigger: \n" << std::flush;
   snx::sonix::instance()->trigger( "kevin" );
   
   std::cout<<"sleep: \n" << std::flush;
   sleep( 1 );
   
   snx::sonix::instance()->changeAPI( api );
   
   std::cout<<"trigger: \n" << std::flush;
   snx::sonix::instance()->trigger( "kevin" );
   
   std::cout<<"sleep: \n" << std::flush;
   sleep( 3 );
   
   snx::sonix::instance()->changeAPI( "stub" );
   
   sleep( 1 );
   
   snx::sonix::instance()->changeAPI( api );
   
   std::cout<<"trigger: \n" << std::flush;
   snx::sonix::instance()->trigger( "kevin" );
   
   std::cout<<"sleep: \n" << std::flush;
   sleep( 3 );
   
   snx::sonix::instance()->changeAPI( "stub" );
   
   return 1;
}
