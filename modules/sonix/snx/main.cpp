#include "aj.h"    // interface
#include "CFileIO.h"
#include "unistd.h"

int main( int argc, char* argv[] )
{
   std::cout<<"exists: "<<CFileIO::fileExists( argv[1] ) << "\n" << std::flush;

   /*
   if (CFileIO::fileExists( argv[1] ))
   {
      std::cout<<CFileIO::fileSize( argv[1] ) << "\n" << std::flush;
   
      std::vector<unsigned char> data;
      CFileIO::fileLoad( argv[1], data );
      
      for (int x = 0; x < data.size(); ++x)
      {
         std::cout<< data[x] << std::flush;
      }      
   }
   */
         
   ajSoundInfo si;
   si.filename = argv[1];
   si.datasource = ajSoundInfo::FILESYSTEM;
      
   
   ajOpenALSoundImplementation al;
   
   al.startAPI();
   al.associate( "kevin", si );
   
   al.trigger( "kevin" );
   
   sleep( 3 );
   al.shutdownAPI();
   
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
