#include <iostream>

#include <unistd.h>
#include "aj/AudioJuggler.h"    // interface
#include "aj/FileIO.h"

void usage( int argc, char* argv[] )
{
   std::cout<<"Usage: "<<argv[0]<<" apiname filename\n"<<std::flush;
   std::cout<<"       "<<argv[0]<<" OpenAL sample.wav\n"<<std::flush;
   std::cout<<"       "<<argv[0]<<" AudioWorks sample.aifc\n"<<std::flush;
}

int main( int argc, char* argv[] )
{
   char bok[] = "../../../Audio/wav/sample.wav";
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

   if (!ajFileIO::fileExists( filename.c_str() ))
   {
      std::cout << "File not found: " << filename << "\n" << std::flush;
      return 0;
   }
   
   aj::SoundInfo si;
   si.filename = filename;
   si.datasource = aj::SoundInfo::FILESYSTEM;
      
   
   std::cout<<"AudioJuggler: \n" << std::flush;
   AudioJuggler aj;
   
   //aj.startAPI();
   std::cout<<"associate: \n" << std::flush;
   aj.configure( "kevin", si );
   
   std::cout<<"trigger: \n" << std::flush;
   aj.trigger( "kevin" );
   
   std::cout<<"sleep: \n" << std::flush;
   sleep( 1 );
   
   aj.changeAPI( api );
   
   std::cout<<"trigger: \n" << std::flush;
   aj.trigger( "kevin" );
   
   std::cout<<"sleep: \n" << std::flush;
   sleep( 3 );
   
   aj.changeAPI( "stub" );
   
   return 1;
}


/*

#include "ajSoundFactory.h"
#include "ajSoundImplementation.h"
#include "ajAudioWorksSoundImplementation.h"  
#include "ajSoundInfo.h"
#include "ajOpenALSoundImplementation.h"      
#include "ajStubSoundImplementation.h"

*/
