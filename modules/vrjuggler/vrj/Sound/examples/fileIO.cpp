#include "fileIO.h"

std::vector<std::string> fileIO::mPaths;

//: true - 
bool fileIO::fileExists( const char* const name )
{
	FILE* file = ::fopen( name, "r" );
	if (file == NULL)
	{
		return false;
	}

	else
	{
		::fclose( file );
		return true;
	}
}

bool fileIO::fileExists( const std::string& name )
{
   return fileExists( name.c_str() );
}

bool fileIO::fileExistsResolvePath( const char* const filename, std::string& realPath )
{
   realPath = resolvePathForName( filename );
   return fileExists( realPath.c_str() );
}

bool fileIO::fileExistsResolvePath( const std::string& filename, std::string& realPath )
{
   return fileExistsResolvePath( filename.c_str(), realPath );
}

std::string fileIO::resolvePathForName( const char* const filename )
{
   for (int x = 0; x < fileIO::mPaths.size(); ++x)
   {
      std::string slash = "/";
      std::string temp  = fileIO::mPaths[x] + slash + filename;
      
      // if this path works, then return it.
      if (fileExists( temp ))
      {
         cout<<"Fixed path: "<<temp.c_str()<<"\n"<<flush;
         return temp;
      }
   }
   
   // couldn't find any that matched, so just return the filename.
   cout<<"Did not fix path: "<<filename<<"\n"<<flush;
   return filename;
}
