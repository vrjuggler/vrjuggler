// library of file utility functions

#include <stdlib.h>
#include <string>

//: true - 
inline bool fileExists( const char* const name )
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

inline bool fileExists( const std::string& name )
{
   return fileExists( name.data() );
}
