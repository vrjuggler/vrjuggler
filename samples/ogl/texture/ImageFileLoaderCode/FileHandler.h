// This code contributed from software located:
// http://www.bigfoot.com/~KevinMeinert/html/software/index.html
// -- Kevin Meinert, kevinmeinert@bigfoot.com
//////////////////////////////////////////////////////////////////////////////
#ifndef FILE_HANDLER
#define FILE_HANDLER

// Abstract base class for such types as handlers/exporters of image and geometry data.
// Implements common attributes that a file handler would need, 
//     such as "can it deal with this filename?", 
//     "what is the description of this handler?",
//     and "what file extensions are supported by this handler?"
#include <vector>
#include <string>
class FileHandler
{
// construction
public:
	//: virtual destructor
	virtual ~FileHandler() {}

// methods
public:
	//: test if this handler can deal with a file called filename.  
	//  this implementation of this function only tests the file extension.
	//  NOTE: derived classes may want to overload this to actually test the file
	//        header for majic number, valid data, etc...
	virtual bool				canHandle( const char* const filename ) const;

	//: a string that describes the files supported by this handler
	virtual const char* const	description() const = 0;

	//: the extension supported by this handler.  i.e. bmp, rgb, pcx
	//  give - an index number for handler that support multiple file extensions
	//  returns - the extension supported by this handler.
	const char* const			extension( const int& index ) const;

	//: returns - the number of extensions supported
	int							numberOfExtensionsSupported() const;

protected:
	//: add an extension descriptor
	//  usually this is only used by the handler's constructor to add extension descriptions.
	//  give - a file extension such as bmp, rgb, pcx
	void						addExtension( const char* const ex );

private:
	std::vector<std::string>	_extensions;
};



#endif
