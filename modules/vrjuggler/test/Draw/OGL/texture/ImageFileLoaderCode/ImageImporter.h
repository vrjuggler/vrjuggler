// This code contributed from software located:
// http://www.bigfoot.com/~KevinMeinert/html/software/index.html
// -- Kevin Meinert, kevinmeinert@bigfoot.com
//////////////////////////////////////////////////////////////////////////////
#ifndef IMAGE_IMPORTER_INCLUDED
#define IMAGE_IMPORTER_INCLUDED

#include "Image.h"
#include "FileHandler.h"

// Abstract file handler for importation of Image objects from a file.
class ImageImporter : public FileHandler
{
// construction
public:
	//: virtual destructor
	virtual		~ImageImporter() {}

// methods
public:
	//: Load the file "filename"
	//  returns true or false for successful or unsuccessful
	//  returns an Image object
	virtual bool import(const char* const filename, Image& image) = 0;
};

//////////////////////////////
// image importer properties.
//////////////////////////////
#include <iostream.h>
inline void outputImporterProperties( ImageImporter& ii )
{
	cout<<"Importer Type:      "<<ii.description()<<"\n"<<flush;
	cout<<"Related Extensions: ";
	for (int k = 0; k < ii.numberOfExtensionsSupported(); ++k)
	{
		cout<<'.'<<ii.extension(k)<<' ';
	}
	cout<<"\n"<<flush;
}

#endif
