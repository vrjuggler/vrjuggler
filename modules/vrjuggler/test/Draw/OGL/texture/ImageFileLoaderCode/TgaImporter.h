
//////////////////////////////////////////////////////////////////////////////
//
//                         -=     TgaImport class     =-
//
// Description: "Importer for Targa files.
// This code contributed from software located:
// http://www.bigfoot.com/~KevinMeinert/html/software/index.html
// -- Kevin Meinert, kevinmeinert@bigfoot.com
//////////////////////////////////////////////////////////////////////////////
#ifndef TGA_IMPORTER_INCLUDED
#define TGA_IMPORTER_INCLUDED

#include "Image.h"
#include "ImageImporter.h"

class TgaImporter : public ImageImporter
{	
public:
	//: Default constructor
	TgaImporter();		
		
	//: Destructor
	virtual ~TgaImporter();
		
	//: Save the image as "filename"
	//  returns true or false for successful or unsuccessful
	virtual bool import( const char* const filename, Image& image );	
	
	//: a string that describes the files supported by this importer
	virtual const char* const	description() const;

	// Read, and expand, one row of RLE data from the filestream.
	// data       - pointer to buffer to recieve uncompressed data.
	// bufferSize - buffer size in bytes
	// bpp        - bytes per pixel (8, 16, 24, 32)
	// fp         - file pointer, must be an opened file, ::use fseek to position at valid start of RLE row.
	static int ReadRLERow( unsigned char* data, const int& bufferSize, const int& bpp, FILE *fp );
   
   void Alert( const bool& truth, const char* const str ) const {}
};



#endif
