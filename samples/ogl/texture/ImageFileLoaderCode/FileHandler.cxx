
//////////////////////////////////////////////////////////////////////////////
//
//                         -=     FileHandler class     =-
//
// Description: ""
// This code contributed from software located:
// http://www.bigfoot.com/~KevinMeinert/html/software/index.html
// -- Kevin Meinert, kevinmeinert@bigfoot.com
//////////////////////////////////////////////////////////////////////////////
#include "defines.h" // min max
#include "CFileIO.h"  //to check if a file exists.

#include "FileHandler.h"

//: test if a file is supported by this importer
bool FileHandler::canHandle( const char* const filename ) const
{
	bool canload = false;

	for (int x = 0; x < _extensions.size(); ++x)
	{
		// we need a non-const version of filename...
		char* file = new char[::strlen(filename)+1];
		::strcpy( file, filename );

		// see if .ext exists in the string "filename"
		std::string fileExt = CFileIO::fileExtension( file );
		std::string currentExt = _extensions[x];

		int amountToCompare = kev::min(fileExt.size(), currentExt.size());
		#ifdef WIN32
		if (strnicmp( fileExt.data(), currentExt.data(), amountToCompare ) == 0)
			canload = true;
		#else
		if (strncasecmp( fileExt.data(), currentExt.data(), amountToCompare ) == 0)
			canload = true;
		#endif
		
		// cleanup
		delete []file;
	}

	return canload;
}

//: the extension supported by this importer.  i.e. bmp, rgb, pcx
//  give - an index number for importers that support multiple file extensions
//  returns - the extension supported by this importer.
const char* const FileHandler::extension( const int& index ) const
{
	return _extensions[index].data();
}

//: returns - the number of extensions supported
int FileHandler::numberOfExtensionsSupported() const
{
	return _extensions.size();
}

//: add an extension descriptor
//  usually this is only used by the importer's constructor to add extension descriptions.
//  give - a file extension such as bmp, rgb, pcx
void FileHandler::addExtension( const char* const ex )
{
	_extensions.push_back( std::string(ex) );
}
