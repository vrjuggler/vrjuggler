
//////////////////////////////////////////////////////////////////////////////
//
//                         -=     SgiImport class     =-
//
// Description: "a class for loading of the SGI image format"
// This code contributed from software located:
// http://www.bigfoot.com/~KevinMeinert/html/software/index.html
// -- Kevin Meinert, kevinmeinert@bigfoot.com
//////////////////////////////////////////////////////////////////////////////
#ifndef SGIIMPORT_INCLUDED
#define SGIIMPORT_INCLUDED

#include <fstream.h>
#include "Image.h"
#include "ImageImporter.h"



/*
////////////////////////////////////////////////////////////////////////
// The general structure of an SGI image file is as shown below: 
// The header indicates whether the image is run length encoded (RLE). 
//     
//   If the image is not run length encoded, this is the structure: 
//   + The Header
//   + The Image Data      
//
//   If the image is run length encoded, this is the structure:
//   + The Header 	
//   + The Offset Tables  	
//   + The Image Data   
////////////////////////////////////////////////////////////////////////
*/

//RGB class definition
class SgiImporter : public ImageImporter
{	
public:
	//: Default constructor
	SgiImporter();		
		
	//: Destructor
	virtual ~SgiImporter();
		
	//: Load the file "filename"
	//  returns true or false for successful or unsuccessful
	//  returns an Image object
	virtual bool				import(const char* const filename, Image& image);

	//: a string that describes the files supported by this importer
	virtual const char* const	description() const;
	
protected:
	//: Read the SGI image header from an ifstream
	bool                    LoadHeader(ifstream &file);
	
	//: load the data part of the file
	//  this automatically determines (based on the header) 
	//  whether to call LoadData or Load RLE
	bool                    LoadData(ifstream &file, Image& image);
	
	//: read from an ifstream RLE data
	// header will tell you if the data is RLE or RGB encoded
	bool                    LoadRLE(ifstream &file, Image& image);
	
	//: read from an ifstream RGB data
	// header will tell you if the data is RLE or RGB encoded
	bool                    LoadRGB(ifstream &file, Image& image);
	
	//: Expand one row of RLE data into RGB data
	//  rlePtr - a pointer to one row of RLE data            <BR>
	//  rlePtr - a pointer to the destination RGB data array <BR>
	//  chanOffset - which channel are you unpacking.        <BR>
	//  NOTE: for SGI images, use 8-bit or 16-bit pointers   <BR>
	//  HINT: use unsigned char or unsigned short pointers   <BR>
	template<class Type>
	inline void channelExpandRow(int chanOffset, Type* rlePtr, Type* rgbPtr) 	    
	{
 		Type token = 0, count = 0, flag = 0, pixel = 0;
		unsigned long rlePos = 0, rgbPos = 0;

		while(1) 
		{
			//each group to be unpacked is preceded by a token
			// the first bit tells whether we are copying
			// one or many.
			// the last 7 bits tell the number we are copying.
			token = rlePtr[rlePos];
			count = (Type) (token & 0x7f);
			if ( count == 0) return;
			rlePos++;
			flag  = (Type) ((token & 0x80)>>7);
			
			//copy from the next <count> pixels
			if(flag == 1)    
			{
				while(count--)
				{
					rgbPtr[rgbPos+chanOffset] = rlePtr[rlePos];
					rlePos++;
					rgbPos += _ZSIZE;
				}
			} 
			
			//copy the next pixel <count> times
			if(flag == 0)
			{
				pixel = rlePtr[rlePos];
				rlePos++;
				while(count--) 
				{
					rgbPtr[rgbPos+chanOffset] = pixel;
					rgbPos += _ZSIZE;
				}
			}
		}
	}
protected:	
	//: 474 is the SGI magic number...
	unsigned short  _MAGIC;         
	
	//: storage type
	// Either RGB_RLE or RGB_VERBATIM
	unsigned char   _STORAGE;       
	
	//: Number of bytes per pixel channel == 1 or 2
	unsigned char   _BPC;           
	
	//1 for 1 color and 1 row, 
	//2 for 1 color and many rows, 
	//3 for many colors and rows
	unsigned short	_DIMENSION;	// Number of dimensions  
	
	//XSIZE - The width of the image in pixels 
 	//YSIZE - The height of the image in pixels 
 	//ZSIZE - The number of channels in the image.  
	// B/W (greyscale) images are stored as 2 dimensional images with an ZSIZE ofr 1.  
	// RGB color images are stored as 3 dimensional images with a ZSIZE of 3.  
	// RGBA color images are stored as a 3 dimensional image with a ZSIZE of 4.
	unsigned short	_XSIZE;		// X size in pixels 
	unsigned short	_YSIZE;		// Y size in pixels 
	unsigned short	_ZSIZE;		// Number of channels
	unsigned long	_PIXMIN ;       // Minimum pixel value that means full dark   (0)
	unsigned long	_PIXMAX;        // Maximum pixel value that means full bright (255)
    
	//0:  NORMAL - The data in the channels represent B/W values
 	//    for images with 1 channel, RGB values for images with 3
 	//    channels, and RGBA values for images with 4 channels.
 	//    Almost all the SGI image files are of this type.  
 	//1:  DITHERED - The image will have only 1 channel of data.
 	//    For each pixel, RGB data is packed into one 8 bit value.
 	//    3 bits are used for red and green, while blue uses 2 bits.
 	//    Red data is found in bits[2..0], green data in bits[5..3],
 	//    and blue data in bits[7..6].  This format is obsolete. 
 	//2:  SCREEN - The image will have only 1 channel of data.
 	//    This format was used to store color-indexed pixels.
 	//    To convert the pixel values into RGB values a colormap
 	//    must be used.  The appropriate color map varies from
 	//    image to image.  This format is obsolete. 
 	//3:  COLORMAP - The image is used to store a color map from
 	//    an SGI machine.  In this case the image is not displayable
 	//    in the conventional sense. 
	unsigned long  _COLORMAP;        //Colormap ID

	enum SgiImageConstants 
	{ 
		//: MAGIC Number for an SGI image
		RGB_MAGIC = 474,

		//: Header tag in SGI image - means image data is not compressed
		RGB_VERBATIM = 0,

		//: Header tag in SGI image - means image data is run-length encoded
		RGB_RLE = 1,

		//: Header tag in SGI image - The data in the channels represent B/W values
		//  for images with 1 channel, RGB values for images with 3   <BR>
		//  channels, and RGBA values for images with 4 channels.     <BR>
		//  Almost all the SGI image files are of this type.          <BR>
		RGB_NORMAL = 0,

		//: Header tag in SGI image - The image will have only 1 channel of data.
		//  For each pixel, RGB data is packed into one 8 bit value.  <BR>
		//  3 bits are used for red and green, while blue uses 2 bits.<BR>
		//  Red data is found in bits[2..0], green data in bits[5..3],<BR>
		//  and blue data in bits[7..6].  This format is obsolete.    <BR>
		RGB_DITHERED = 1,

		//: Header tag in SGI image - The image will have only 1 channel of data.
		//  This format was used to store color-indexed pixels.    <BR>
		//  To convert the pixel values into RGB values a colormap <BR>
		//  must be used.  The appropriate color map varies from   <BR>
		//  image to image.  This format is obsolete.              <BR>
		RGB_SCREEN = 2,

		//: The image is used to store a color map from
		// 	an SGI machine.  In this case the image is not displayable
		//	in the conventional sense.
		RGB_COLORMAP = 3
	};
};

#endif
