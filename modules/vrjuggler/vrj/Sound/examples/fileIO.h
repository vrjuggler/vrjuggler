#ifndef FILE___IO_STUFF
#define FILE___IO_STUFF

#include <stdlib.h>
#include <string>
#include <vector>

// use this class to store your filepath etc...
// if you don't use this then the pfFileIO funcs will set the path
// by default to "."
class fileIO
{
public:
   // == "." by default
   // this is in the form of: 
   //     /home/users/me
   // or: ../../hi-man
   // or: ./rgbfiles
   static void addFilePath( const std::string& filepath = "." )
   {
      mPaths.push_back( filepath );
   }

   static bool fileExists( const char* const name );
   static bool fileExists( const std::string& name );
   static bool fileExistsResolvePath( const char* const filename, std::string& realPath );
   static bool fileExistsResolvePath( const std::string& filename, std::string& realPath );
   static std::string resolvePathForName( const char* const filename );
   static std::vector<std::string> mPaths;  
};




#endif
