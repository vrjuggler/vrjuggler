// library of file utility functions, useful to IRIS Performer apps

#ifndef PERFORMER_FILEIO_UTILS
#define PERFORMER_FILEIO_UTILS

#include <string>
#include <assert.h>
#include "fileIO.h" // bool fileExists()

// use this class to store your filepath etc...
// if you don't use this then the pfFileIO funcs will set the path
// by default to "."
class pfFileIO
{
public:
   // == "." by default
   static std::string filePath;
   static void setFilePath( const std::string& filepath = "." )
   {
      filePath = filepath;
   }

   static void addFilePath( const std::string& filepath)
   {
      filePath += filepath;
   }

   //: Load geometry from File
   // trys to make performer load whatever filename is given
   // uses pfFileIO:filePath as the current path
   // uses pfdLoadFile to load the given filename
   // exits application on failure
   // outputs useful stuff, to let you know current status.
   static pfNode* loadFile( const std::string& filename );

   //: Load geometry from File, uses cached version if available
   // use this function just like the pfdLoadFile performer function.
   // this function automatically keeps track of optimised versions of the filename.
   // if there is no .pfb file, or the .pfb is older than filename, then
   // function generates a new pfb file.
   // if there is a current pfb file, then functino uses it instead.
   // TODO: add time stamp check - not implemented yet.
   static pfNode* autoloadFile( std::string fileName );

   //: Write node to File
   // stores the geometry under node, in file originalFltName.pbf
   // NOTE: don't specify the .pfb extension.
   static void writeOptimizedFile( pfNode* node, std::string originalFltName );

   //: get the name of the optimized file given the original name
   static std::string optimizedName( std::string originalFltName );
};



#endif
