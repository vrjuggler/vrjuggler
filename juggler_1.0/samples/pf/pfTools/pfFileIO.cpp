// library of file utility functions, useful to IRIS Performer apps

// Performer includes
#include <Performer/pf/pfChannel.h>
#include <Performer/pf/pfEarthSky.h>
#include <Performer/pf/pfLightSource.h>
#include <Performer/pf/pfNode.h>
#include <Performer/pf/pfTraverser.h>
#include <Performer/pf/pfDCS.h>
#include <Performer/pfdu.h>
#include <Performer/pfutil.h>
#include <Performer/pf/pfSwitch.h>
#include <Performer/pr.h>

#include "pfFileIO.h" // my header.

std::string pfFileIO::filePath = ".";

// trys to make performer load whatever filename is given
// uses pfFileIO:filePath as the current path
// uses pfdLoadFile to load the given filename
// exits application on failure
// outputs useful stuff, to let you know current status.
pfNode* loadFile( const std::string& filename )
{
   pfdInitConverter( filename.data() );
   pfFilePath( pfFileIO::filePath.data() );
   pfNode* node = pfdLoadFile( filename.data() );

   if (node == NULL)
   {
      cout<<"COULDN'T FIND "<<filename.data()<<"\n"<<flush;
      exit(0);
   }
   
   return node;
}

// get the name of the optimized file given the original name
std::string optimizedName( std::string originalFltName )
{
   std::string cachedPreFix = ".cached";
   std::string cachedPostFix = ".pfb";
   return originalFltName + cachedPreFix + cachedPostFix;
}

// stores the geometry under node, in file "optimizedName"
// NOTE: usually pass in the name returned by optimizedName() function
void writeOptimizedFile( pfNode* node, std::string optimizedName )
{
   pfdStoreFile( node, optimizedName.data() );
}

// use this function just like the pfdLoadFile performer function.
// this function automatically keeps track of optimised versions of the filename.
// if there is no .pfb file, or the .pfb is older than filename, then
// function generates a new pfb file.
// if there is a current pfb file, then functino uses it instead.
// TODO: add time stamp check - not implemented yet.
pfNode* autoloadFltData( std::string fltFile )
{
   pfNode* node = NULL;
   
   std::string optimizedFltFile = optimizedName( fltFile );
   if (fileExists(optimizedFltFile))
   {
      cout<<"Loading "<<optimizedFltFile.data()<<"\n"<<flush;
      node = loadFile( optimizedFltFile );
   }

   else
   {
      cout<<"Loading "<<fltFile.data()<<"... "<<flush;
      node = loadFile( fltFile );
      cout<<"Caching to "<<optimizedFltFile.data()<<"...\n"<<flush;
      writeOptimizedFile( node, optimizedFltFile );
      // TODO: consider if the user has write access, 
      //       for demos, they may not.
      // one workaround is to always distribute this app with the pfb's.
   }
   
   assert( node != NULL );
   return node;
}
