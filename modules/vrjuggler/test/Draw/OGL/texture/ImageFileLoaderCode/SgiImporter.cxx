
//////////////////////////////////////////////////////////////////////////////
//
//                         -=     SgiImport class     =-
//
// Description: "a class for loading of the SGI image format"
// This code contributed from software located:
// http://www.bigfoot.com/~KevinMeinert/html/software/index.html
// -- Kevin Meinert, kevinmeinert@bigfoot.com
//////////////////////////////////////////////////////////////////////////////
#include <string>			// for std::string
#include <fstream.h>		// for ifstream
#include <math.h> // for pow

#include "endian.h" // for kev::byteReverse
#include "CFileIO.h" // for fileexists..

#include "SgiImporter.h"


//////////////////////////////////////////////
///  Public Members:
/////////////////////////////////////////////

//: Default constructor
SgiImporter::SgiImporter() : ImageImporter()
{
	this->addExtension( "rgb" );
	this->addExtension( "rgba" );
	this->addExtension( "sgi" );
	this->addExtension( "bw" );
}

//: Destructor
SgiImporter::~SgiImporter()
{
}

//: a string that describes the files supported by this importer
const char* const SgiImporter::description() const
{
	return "Sgi Image";
}

//: Load the file "filename"
//  returns true or false for successful or unsuccessful
//  returns an Image object
bool SgiImporter::import(const char* const filename, Image& image)
{
	assert( filename != NULL && "Give a valid string pointer, this one's NULL" );

   if (!CFileIO::fileExists( filename ))
   {
      cout<<"File doesn't exist!\n"<<flush;
      return false;
   }

	//check if the file is an rgb file.
	//TODO: remove this check to be more flexible...
	if( strstr(filename, ".sgi") || strstr(filename, ".rgb") || 
	    strstr(filename, ".rgba") || strstr(filename, ".bw") ) 
	{
		//if rgb, then we need to do the loading
		image.free();
		
		#ifdef WIN32
		ifstream hRGBFile( filename, ios::in | ios::nocreate | ios::binary, filebuf::openprot );
		#else
		ifstream hRGBFile( filename, ios::in | ios::nocreate, filebuf::openprot );
		#endif
		
		// why would ifstream mangle this pointer?  that violates const-ness. (you probably need to recompile clean)
		assert( filename != NULL && "This should never happen, ifstream's constructor mangled the filename address" );
		
		this->LoadHeader(hRGBFile);
		image.channels() = _ZSIZE;
		//image.bpc() = _BPC;
		image.setName( filename );
		if (!this->LoadData(hRGBFile, image)) 
		{
			//Log(0)
			cout << "Couldn't load image\n" << flush;
			hRGBFile.close();
			return false;
		}
		
		hRGBFile.close();
		return true;
	}

	//Log(0)
		cout << filename << ": Specify an rgb image type (extention must be sgi/rgb/rgba/bw)\n" << flush;
	return false;
}



//////////////////////////////////////////////
///  Protected Members:
/////////////////////////////////////////////


//Load data from an input file stream
bool SgiImporter::LoadData(ifstream &file, Image& image)
{
	if(_STORAGE == RGB_RLE)
		return this->LoadRLE(file, image);
	else return this->LoadRGB(file, image);
}
 	


//: Read the SGI image header from an ifstream
bool SgiImporter::LoadHeader(ifstream &file)
{
  	char filler[404];
	file.read((unsigned char *)&_MAGIC,	sizeof(unsigned short));
	file.read((unsigned char *)&_STORAGE,	sizeof(unsigned char) );
	file.read((unsigned char *)&_BPC,	sizeof(unsigned char) );
	file.read((unsigned char *)&_DIMENSION,	sizeof(unsigned short));
	file.read((unsigned char *)&_XSIZE,	sizeof(unsigned short));
	file.read((unsigned char *)&_YSIZE,	sizeof(unsigned short));
	file.read((unsigned char *)&_ZSIZE,	sizeof(unsigned short));
	file.read((unsigned char *)&_PIXMIN,	sizeof(unsigned long) );
	file.read((unsigned char *)&_PIXMAX,	sizeof(unsigned long) );
	file.read((unsigned char *) filler,	sizeof(unsigned long) );
	char IMAGENAME[80];
	file.read((unsigned char *) IMAGENAME,	sizeof(unsigned char) * 80);
	file.read((unsigned char *)&_COLORMAP,	sizeof(unsigned long) );
	file.read((unsigned char *) filler,	sizeof(unsigned char) * 404);

   // SGI format is big endian, 
   // so will need to swap bytes if system architecture is little endian.
   if (kev::littleEndian())
   {
	   //reverse anything that is not unsigned char size
	   kev::byteReverse(_MAGIC);
	   kev::byteReverse(_DIMENSION);
	   kev::byteReverse(_XSIZE);
	   kev::byteReverse(_YSIZE);
	   kev::byteReverse(_ZSIZE);
	   kev::byteReverse(_PIXMIN);
	   kev::byteReverse(_PIXMAX);
	   kev::byteReverse(_COLORMAP);
   }
	
	// Do some tests to make sure the header looks valid.
	
	if (_MAGIC != RGB_MAGIC)
	{
		//Log(0)
		cout<<"Not an rgb image according to the magic number. "<<_MAGIC<<"!="
			<<RGB_MAGIC<<"\n"<<flush;
		//Log(0)
		cout<<IMAGENAME<<" is not SGI format\n"<<flush;
		return false;
	} 
 
	if (_DIMENSION == 1 && 
		(_ZSIZE != 1 || _YSIZE != 1) )
	{
		//Log(0)
		cout<<"Error: Dimension is 1, you cannot have more than one color, or scanline\n"
			<<flush;
		//Log(0)
		cout<<" - Dimensions ="<<_XSIZE<<"x"<<_YSIZE<<":"<<_ZSIZE<<" colors\n"<<flush;
		return false;
	}

	if (_DIMENSION == 2 && _ZSIZE != 1 )
	{
		//Log(0)
		cout<<"Error: Dimension is 2, you cannot have more than one color\n"<<flush;
		//Log(0)
		cout<<" - Dimensions ="<<_XSIZE<<"x"<<_YSIZE<<":"<<_ZSIZE<<" colors\n"<<flush;
		return false;
	} 
	
	float twoToTheBPC8 = pow(2.0f, (_BPC * 8));

	if ( (_PIXMAX - _PIXMIN) > twoToTheBPC8 )
	{
		//Log(0)
		cout<<"Error: color channel ranges from "<<_PIXMIN<<" to "<<_PIXMAX<<"\n"<<flush;
		//Log(0)
		cout<<"       this conflicts with the total number of colors per channel\n"<<flush;
		//Log(0)
		cout<<"       specified in the header.  BitsPerChannel = "<< twoToTheBPC8 <<"\n"<<flush;
		return false;
	}

	if (_COLORMAP == RGB_SCREEN || _COLORMAP == RGB_DITHERED)
	{
		//Log(0)
		cout<<"Error: this rgb format is obsolete, don't use screen or "
			<<"dithered encoding\n"<<flush;
		return false;
	}
	if (_COLORMAP == RGB_COLORMAP)
		//Log(0)<<"Warning: Trying to load rgb image as a color map.\n"<<flush;

	return true;
}

//: read from an ifstream RLE data
// header will tell you if the data is RLE or RGB encoded
bool SgiImporter::LoadRLE(ifstream &file, Image& image)
{
    unsigned long *	startTable	= 0, *lengthTable = 0;
    unsigned long	channel		= 0, y=0;
    unsigned long	scanlines	= _YSIZE * _ZSIZE;
    unsigned long	rleLength	= 0;
    void *			rleData		= 0;
    unsigned long	rgbPos		= 0, rlePos = 0;
    
    //this->LoadHeader() should have positioned us at the 512th unsigned char in the file...
    //there is a table entry for each red row, green row, and blue row, (and alpha)
    //there are two tables... so there are 2 * numChannels entries in all.

    //now, read in <scanlines> number of longs for the 'start' OffsetTable
    startTable  = (unsigned long*)new unsigned char[scanlines * sizeof(unsigned long)];
    for (y = 0; y < scanlines; y++)
    {
	   file.read((unsigned char *) &startTable[y], sizeof(unsigned long));
	   
      // SGI format is big endian, 
      // so will need to swap bytes if system architecture is little endian.
      if (kev::littleEndian())
      {
         kev::byteReverse(startTable[y]);
      }
    }
    assert(!file.eof() && "Unexpected end of file" );
    
    //now, read in <scanlines> number of longs for the 'length' OffsetTable
    lengthTable = (unsigned long*)new unsigned char[scanlines * sizeof(unsigned long)];
    for (y = 0; y < scanlines; y++)
    {
	    file.read((unsigned char*) &lengthTable[y], sizeof(unsigned long));
	    // SGI format is big endian, 
       // so will need to swap bytes if system architecture is little endian.
       if (kev::littleEndian())
       {
	       kev::byteReverse(lengthTable[y]);
       }
      
       rleLength += lengthTable[y];
    }
    assert( !file.eof() && "Unexpected end of file" );
    
    //Size of rgb data in bytes...
    //unsigned long chanLength = _XSIZE * _YSIZE * _BPC;
    //unsigned long rgbLength = chanLength * _ZSIZE;
    
    
    
	rleData = (unsigned char *)new unsigned char[rleLength];
    file.read((unsigned char *)rleData, rleLength);

	// reserver space in the image.
	int bpp = _ZSIZE * _BPC * 8; // 8 bits per byte
	image.reserve( this->_XSIZE, this->_YSIZE, bpp, _ZSIZE );

    for (channel = 0; channel < _ZSIZE; channel++)
    {	
		for (y = 0; y < _YSIZE; y++)	
		{	
			//skip every XSIZE*ZSIZE pixels (there is a pixel every ZSIZE bytes)
			rgbPos = (y * (_XSIZE)) * _ZSIZE;
			
			//Go to the next row of pixals
			//use startTable's entry for the offset of this RLE'd row data   
			rlePos = startTable[y + channel * _YSIZE] 
				- (512 + scanlines*sizeof(long)*2);
			
			//expand the rle data for one row of rgb data
			if (_BPC == sizeof(unsigned char))
				channelExpandRow( channel, &((unsigned char*)rleData)[rlePos], 
					              &((unsigned char*)image.data())[rgbPos]);
			else 
				channelExpandRow( channel, &((unsigned short*)rleData)[rlePos], 
					              &((unsigned short*)image.data())[rgbPos]);
		}
    }
    
    delete rleData;
    delete startTable;
    delete lengthTable;
    return true;
}

//: read from an ifstream RGB data
// header will tell you if the data is RLE or RGB encoded
bool SgiImporter::LoadRGB(ifstream &file, Image& image)
{
    int x, y, channel;
    unsigned char *verbatimData;
    unsigned long rowStart=0;
    unsigned long whichPixel=0;
    unsigned long chanLength=0,dataPos=0,verbatimPos=0;
    if(!file.rdbuf()->is_open())
    {
	    //Log(0)
			cout<<"File is not open anymore.\n"<<flush;
	    return false;
    }
    // The Image Data (if not RLE) 
    // If the image is stored verbatim (without RLE), then image data directly
    // follows the 512 unsigned char header.  The data for each scanline of the first
    // channel is written first.  If the image has more than 1 channel, all
    // the data for the first channel is written, followed by the remaining
    // channels.  If the BPC value is 1, then each scanline is written as XSIZE
    // bytes.  If the BPC value is 2, then each scanline is written as XSIZE 
    // shorts.  These shorts are stored in the unsigned char order described above.  
	

    //Size of rgb data in bytes...
    chanLength = _XSIZE * _YSIZE * _BPC;
    unsigned long rgbLength = chanLength * _ZSIZE;

    //read in the verbatim data. 
    //in the file it will look like 
    // [rrrrrrrrr(etc..)ggggggggg(etc..)bbbbbbbbb(etc..)aaaaaaaaa(etc..)]
    //
    verbatimData = (unsigned char *) new unsigned char[rgbLength];
    file.read((unsigned char *)verbatimData, rgbLength);
    
    //interlace the verbatim data
    // in the end it should look like [rgbargbargbargba(etc..)]
    int bpp= _ZSIZE * _BPC * 8; // 8 bits per byte
	image.reserve( this->_XSIZE, this->_YSIZE, bpp, _ZSIZE );

    for (channel=0; channel < _ZSIZE; channel++) 
    {
		for (y=0; y<_YSIZE; y++) 
		{
			rowStart = (y * _XSIZE) * _BPC;
			for (x = 0; x < _XSIZE; x++)
			{ 
				whichPixel = (x) * _BPC;
				dataPos = (whichPixel+rowStart)*_ZSIZE + channel;
				verbatimPos = whichPixel + rowStart + channel * chanLength;
				image.data()[dataPos] = verbatimData[verbatimPos];
			} 		
		} 
    }
    delete verbatimData;
    return true;
}
