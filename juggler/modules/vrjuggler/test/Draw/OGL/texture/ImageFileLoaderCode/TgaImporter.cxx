
//////////////////////////////////////////////////////////////////////////////
//
//                         -=     TgaImport class     =-
//
// Description: "Importer for Targa files.
// This code contributed from software located:
// http://www.bigfoot.com/~KevinMeinert/html/software/index.html
// -- Kevin Meinert, kevinmeinert@bigfoot.com
//////////////////////////////////////////////////////////////////////////////
#include <string>			// for std::string
#include "endian.h"     // for byteReverse
#include "CFileIO.h"		// needed to write out binary data with proper endianness.
#include "TgaHeader.h"	// the TGA header data structure

#include "TgaImporter.h"



///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////


//: Default constructor
TgaImporter::TgaImporter()
{
	this->addExtension( "tga" );
	this->addExtension( "vda" );
	this->addExtension( "icb" );
	this->addExtension( "vst" );
	this->addExtension( "win" );
}

//: Destructor
TgaImporter::~TgaImporter()
{
}

// (this was fun to write.)
// data       - pointer to buffer to recieve uncompressed data.
// bufferSize - buffer size in bytes
// bpp        - bytes per pixel (8, 16, 24, 32)
// fp         - file pointer, must be an opened file, ::use fseek to position at valid start of RLE row.
int TgaImporter::ReadRLERow( unsigned char* data, const int& bufferSize, const int& bpp, FILE *fp )
{
	unsigned int	value;  
	unsigned char	byte;   // for reading byte by byte.
	int		i;
	unsigned char*	q;
	int		n( bufferSize );
	static unsigned char rleBuf[RLEBUFSIZ];
		
	while ( n > 0 )
	{
		CFileIO::ReadByte( fp, byte );
		value = byte;

		if ( value & 0x80 )
		{
			value &= 0x7f;
			value++;
			n -= value * bpp;
			if ( n < 0 ) 
				return( -1 );
			if ( ::fread( rleBuf, 1, bpp, fp ) != bpp ) return( -1 );
			while ( value > 0 )
			{
				*data++ = rleBuf[0];
				if ( bpp > 1 ) *data++ = rleBuf[1];
				if ( bpp > 2 ) *data++ = rleBuf[2];
				if ( bpp > 3 ) *data++ = rleBuf[3];
				value--;
			}
		}
		else
		{
			value++;
			n -= value * bpp;
			if ( n < 0 ) 
				return( -1 );
			
			// Maximum for value is 128 so as long as RLEBUFSIZ
			// is at least 512, and bpp is not greater than 4
			// we can read in the entire raw packet with one operation.
			if ( fread( rleBuf, bpp, value, fp ) != value ) 
				return( -1 );
			for ( i = 0, q = rleBuf; i < (value * bpp); ++i ) *data++ = *q++;
		}
	}
	return( 0 );
}

//: Save the image as "filename"
//  returns true or false for successful or unsuccessful
bool TgaImporter::import(const char* const filename, Image& image)
{
   if (!CFileIO::fileExists( filename ))
   {
      cout<<"File doesn't exist!\n"<<flush;
      return false;
   }

	//if tga, then we need to load it
	// Open the file.
	TGAHeader header;
	FILE* fp = ::fopen( filename, "rb" );
	assert( fp != NULL && "fileExists passed, but fopen did not" );

	// It would be nice to be able to read in the entire structure with one fread, but hardware dependent
	// structure alignment precludes the simplistic approach. Instead, fill each field individually, 
	// and use routines that will allow code to execute on various hosts by recompilation with particular 
	// compiler flags.
	//
	// Start by reading the fields associated with the original TGA format.
	CFileIO::ReadByte( fp, header.idLength );
	CFileIO::ReadByte( fp, header.mapType );
	CFileIO::ReadByte( fp, header.imageType );
	CFileIO::ReadShort( CFileIO::LITTLE, fp, header.mapStart );
	CFileIO::ReadShort( CFileIO::LITTLE, fp, header.mapLength );
	CFileIO::ReadByte( fp, header.mapDepth );
	CFileIO::ReadShort( CFileIO::LITTLE, fp, header.xOrigin );
	CFileIO::ReadShort( CFileIO::LITTLE, fp, header.yOrigin );
	CFileIO::ReadShort( CFileIO::LITTLE, fp, header.imageWidth );
	CFileIO::ReadShort( CFileIO::LITTLE, fp, header.imageHeight );
	CFileIO::ReadByte( fp, header.pixelDepth );
	CFileIO::ReadByte( fp, header.imageDesc );

	//: currently only 8 and 24 and 32 bit is supported
	if (header.pixelDepth != 8 && header.pixelDepth != 24 && header.pixelDepth != 32)
	{
		Alert( false, "Importer only supports 24 and 32bit TGA." );   
		return false;
	}

	//: header.imageType = ???  (* == supported)
	//   ImageType	Datatype		Colormap	Encoding
	//   0			no image data	n			n
	//   1			colormapped		y			n
	// * 2			truecolor		n			n
	// * 3			monochrome		n			n
	//   9			colormapped		y			y
	// * 10			truecolor		n			y
	//   11			monochrome		n			y
	
	//: currently only truecolor or monochrome (compressed, and uncompressed) is supported
	if (header.imageType != 3 && header.imageType != 11 && //monochrome
		header.imageType != 2 && header.imageType != 10)   //truecolor
	{
		cout<<"Only support Truecolor (24 and 32bit) and monochrome (8bit) TGA's\n"<<flush;
		return false;
	}

	// TODO: useless?
	/*
	//: read the image id string.
	::memset( header.idString, 0, 256 );
	if ( header.idLength > 0 ) 
	{
		// read the string
		// NOTE: as long as idLength is char, it wont overflow the buffer.
		char* imageIdString = reinterpret_cast<char*>(header.idString);
		::fread( imageIdString, sizeof(char), header.idLength, fp );
	}
	*/

	// Extended header stuff
	/*
	// Seek to the end of the file
	// there may be 2 longs, and an 18 char string there. (26 bytes total)
	int xTgaFormat = 0;
	if ( ::fseek( fp, -26, SEEK_END ) != 0 ) 
	{
		Alert( false, "Error seeking to end of file for possible extension data." );
		return false;
	}

	// Read the first two longs (8 bytes)
	// get the offset to the xtended header (if it exists)
	header.extAreaOffset = ReadLong( CFileIO::LITTLE, fp );
	header.devDirOffset = ReadLong( CFileIO::LITTLE, fp );
	
	// Read the signature (18 bytes)
	// compare it to see if the file is the new (extended) TGA format.
	::fgets( header.signature, 18, fp );
	if (std::string( "TRUEVISION-XFILE." ) == header.signature)
		xTgaFormat = 1;
	else 
	{
		// Reset offset values since this is not a new TGA file
		header.extAreaOffset = 0L;
		header.devDirOffset = 0L;
	}

	//: if it's an extended TGA file, then read the extended header.
	if ( xTgaFormat && header.extAreaOffset )
	{
		if ( ReadExtendedTGA( fp, &header ) < 0 ) 
			return false;
	}
	*/

	// initialize the Image
	int bytesPerPixel = (header.pixelDepth + 7) >> 3;
	image.free();
	image.setName( filename );
	image.reserve( header.imageWidth - header.xOrigin, header.imageHeight - header.yOrigin, header.pixelDepth, bytesPerPixel );

	// figure out stats for the colormap info (if any), we'll need the length so we can find the pixel area.
	int colorMapLength = 0;
	if (header.mapType == 1)
	{
		int bytesPerPaletteElement = (header.mapDepth + 7) >> 3;
		colorMapLength = bytesPerPaletteElement * header.mapLength;
	}

	// Truecolor data starts sizeof(TGAOriginalHeader) from beginning. (18)
	// This is true for both RLE and uncompressed scanlines.
	int dataAreaOffset = 18 + header.idLength + colorMapLength;
	if ( ::fseek( fp, dataAreaOffset, SEEK_SET ) != 0 )
	{
		Alert( false, "Couldn't seek within the file to the image data portion" );
		return false;
	}

	//: Read Image Data --> truecolor/monochrome and compressed (easy)
	if ( header.imageType == 10 || header.imageType == 11 )
	{
		// allocate data to uncompress a scanline.
		int bCount = header.imageWidth * bytesPerPixel;

		//: process each scanline one by one.
		for ( int i = 0; i < header.imageHeight; ++i )
		{					
			//: expand one scanline of RLE data from the file...
			if (ReadRLERow( image.row( i ), bCount, bytesPerPixel, fp ) < 0)
			{
				// Indicate an error, but return true so that the user can at least see what is in the image
				// TODO: there may be times when the user will want this to fail, such as a texture loader 
				//       in a professional-grade simulation where you want to ensure your client wont see crap.
				Alert( false, "Error decompressing RLE data." );
				return true;
			}
		}
	} else

	//: Read Image Data --> truecolor/monochrome and UNcompressed
	if ( header.imageType == 2 || header.imageType == 3 )
	{
		int rowStart, columnStart;
		int onePixelSize = image.channels();

		//is the image stored upside down?
		bool isUpsideDown = ((header.imageDesc & (0x0001<<5)) == (0x0001<<5));
		
		//is the image stored flipped?
		bool isHorzFlipped = ((header.imageDesc & (0x0001<<4)) == (0x0001<<4));
		
		// Which row do we start at?
		// NOTE: this depends on if the image is upside down
		if ( isUpsideDown )
		{
			rowStart = image.height() - 1;
		}
		
		else
		{
			rowStart = 0;
		}
			
		// Where is the beginning of one row?
		// NOTE: this depends on if the image is flipped or not
		if ( isHorzFlipped )
		{
			columnStart = image.width() - 1;
		}
		
		else
		{
			columnStart = 0;
		}
		
		// Read the image in the correct order.
		// NOTE: y and x are just counters. 
		//       i and j actually traverse the image.
		int j = rowStart;
		for ( int y = 0; y < image.height(); ++y )
		{
			int i = columnStart;	
			for ( int x = 0; x < image.width(); ++x )
			{
				
				//: expand one scanline of RLE data from the file...
				if (::fread( image.pixel(i, j), sizeof(unsigned char), onePixelSize, fp ) != onePixelSize)
				{
					// Indicate an error, but return true so that the user can at least see what is in the image
					// TODO: there may be times when the user will want this to fail, such as a texture loader 
					//       in a professional-grade simulation where you want to ensure your client wont see crap.
					Alert( false, "Error reading image data." );
					return true;
				}
				
				if (isHorzFlipped) 
					--i;
				else
					++i;
			}
			
			if (isUpsideDown) 
				--j;
			else
				++j;
		}
	}

	// swap the Red and Blue channels, BGRA -> RGBA, or BGR -> RGB
	// NOTE: only do this if there is a red and blue channel.
	if (header.pixelDepth == 24 || header.pixelDepth == 32)
		image.swapChannels(0,2);

	// done with file, close it.
	assert( fp != NULL && "File pointer is NULL, right after using it? (Makes no sense)" );
	::fclose( fp );

	return true;
}

//: a string that describes the files supported by this importer
const char* const TgaImporter::description() const
{
	return "Truevision Graphics Adapter";
}
