
//////////////////////////////////////////////////////////////////////////////
//
//                         -=     Image class     =-
//
// Description: "an image, which contains it's image data"
// This code contributed from software located:
// http://www.bigfoot.com/~KevinMeinert/html/software/index.html
// -- Kevin Meinert kevinmeinert@bigfoot.com
//////////////////////////////////////////////////////////////////////////////
#include <math.h>
#include <iostream>
#include <assert.h>

#include "Image.h"
#include "defines.h" // min


//: Default constructor
Image::Image() : _data( NULL ), _xsize( 0 ), _ysize( 0 ), _bpp( 0 ), 
               _channels( 0 ), _name( "unnamed" ), _rowPadding( 0 ), 
               _rowAlignment( 1 ), _usingPointer( false )
{
}

//: Copy constructor		
Image::Image( const Image& image ) : _data(NULL), _xsize( 0 ), 
                              _ysize( 0 ), _bpp( 0 ), _channels( 0 ), 
                              _name( "unnamed" ), _rowPadding( 0 ), 
                              _rowAlignment( 1 ), _usingPointer( false )
{
	this->copy( image );
}

//: Data copy constructor
// - set the source pixel data (bytes), this must be a valid pointer, 
//   and must contain the data you specify in the next few fields.
// - set copyData to true to copy the data into this Image, otherwise 
//   Image just stores the pointer.
// - set bpp to the colordepth of your data: 8, 15, 16, 24, or 32 bit
// - set xsize and ysize to the size in pixels of your bitmap...
// - set channels to the number of channels in your data
// - specify your data's byte alignment (usually 1 if no row padding)
//  NOTE: memory will not be free'd on destruct, if copyData is false 
//        (copyData == false implies Image does not own the data)
Image::Image( unsigned char* bytes, const bool& copyData, 
              const unsigned int xsize, const unsigned int ysize, 
              const unsigned int bpp, const unsigned int channels, 
				  const unsigned int alignment )
{
	this->setData( bytes, copyData, xsize, ysize, bpp, channels, alignment );
}


//: Destructor
Image::~Image()
{
	this->free();
}

//: frees memory taken by current image.
void Image::free()
{
    // don't free memory if using an external pointer.
    if (_usingPointer == true)
	{
		_data = NULL;
		return;
	}
	
	if (_data != NULL)
		delete [] _data;

	_xsize = 0;
	_ysize = 0;
	_bpp = 0;
	_channels = 0;
	_data = NULL;
	_rowPadding = 0;
}

//: set the image memory yourself 
// - set the source pixel data, this must be valid pointer, 
//   and must contain the data you specify in the next few fields.
// - set copyData to true to copy the data into this Image, otherwise Image 
//   just stores the pointer.
// - set bpp to the colordepth of your data: 8, 15, 16, 24, or 32 bit
// - set xsize and ysize to the size in pixels of your bitmap...
// - set channels to the number of channels in your data
// - specify your data's byte alignment (usually 1 if no row padding)
//  NOTE: memory will not be free'd on destruct, if copyData is false 
//        (copyData == false implies Image does not own the data)
void Image::setData( unsigned char* bytes, const bool& copyData, 
                     const unsigned int xsize, const unsigned int ysize, 
                     const unsigned int bpp, const unsigned int channels, 
                     const unsigned int alignment)
{
	if (copyData == true)
	{
		this->reserve( xsize, ysize, bpp, channels, alignment );
		::memcpy( this->data(), bytes, this->size() );
	}

	else
	{
		this->free();
		_usingPointer = true;
		_data = bytes;
    
		_xsize = xsize;
		_ysize = ysize;
		_bpp = bpp;
		_channels = channels;
		_rowAlignment = alignment;
		_rowPadding = xsize % alignment;
    
		// don't pass in NULL to this function.
		assert(_data != NULL && "NULL data pointer" );
	}
}

//Allocate memory for the image
// - bpp is the colordepth of 8, 15, 16, 24, or 32 bit
// - xsize and ysize is the size in pixels of your bitmap...
// - pad is the number of bytes to attach to each row
void Image::reserve( const unsigned int xsize, const unsigned int ysize, 
                     const unsigned int bpp, const unsigned int channels, 
                     const unsigned int alignment )
{
	this->free();
    _usingPointer = false;

    //allocate memory.
	_rowPadding = xsize % alignment;
    switch( bpp )
    {
    // special case - 15bit is usually stored in a short
    case 15:
	    _data = new unsigned char[(xsize * ysize * 16) + (ysize * _rowPadding)];
		memset( _data, 0xbc, (xsize * ysize * 16) + (ysize * _rowPadding) );
	    break;
	    
    // normal cases
    case 8:
    case 16:
    case 24:
    case 32:
	    _data = new unsigned char[(xsize * ysize * bpp) + (ysize * _rowPadding)];
		memset( _data, 0xff, (xsize * ysize * bpp) + (ysize * _rowPadding) );
	    break;
    
    // unknown color depth.
    default:
	    assert(false && "Unknown color depth." );
	    _data = new unsigned char[(xsize * ysize * bpp) + (ysize * _rowPadding)];
		memset( _data, 0xfe, (xsize * ysize * bpp) + (ysize * _rowPadding) );
    }

	
	_rowAlignment = alignment;
    _xsize = xsize;
    _ysize = ysize;
    _bpp = bpp;
    _channels = channels;

    // oops, the data is NULL.
    assert(_data != NULL && "NULL data pointer" );
}


//: copy an image into another.
const Image & Image::copy( const Image& image )
{
	if (image.data() == NULL) return *this;

	this->reserve( image.width(), image.height(), image.depth(), 
                  image.channels(), image.rowAlignment() );

	// attempt a fast copy
	assert( image.size() == this->size() && 
           "Copy didn't reserve the correct amount of space in the image." );
	::memcpy( this->data(), image.data(), image.size() );

	return *this;
}


//: get a subimage of this image. (top=0,left=0,bottom>0,right>0)
//  allows you to cut out a square area of this image
//  give - the sub image coordinates, these must be within this image's 
//         pixel coordinates
//  returns - copies the specified sub image into the Image object that you 
//            provide.
void Image::getSubImage( const unsigned int& pixelLeft, 
                         const unsigned int& pixelTop, 
                         const unsigned int& pixelRight, 
                         const unsigned int& pixelBottom, 
                         Image& newImage ) const
{
	// make sure the client follows the function's requirements
	assert( pixelRight < this->width() && "right position is not within bounds" );
	assert( pixelBottom  < this->height() && "bottom position is not within bounds" );
	
	// set the dim's of the client's image.
	newImage.reserve( pixelRight - pixelLeft, pixelBottom - pixelTop, 
                     this->bpp(), this->channels() );


	int imageChannels = this->channels();
	int newImageChannels = newImage.channels();
	//int imageWidth = this->width();
	//int newImageWidth = newImage.width();
	//const unsigned char* const imagePtr = this->data();
	//unsigned char* newImagePtr = newImage.data();

	// copy the subimage from this image into newImage.
	for (unsigned int y = 0; y < newImage.height(); ++y)
	{
		// get an index to each row to be copied next.
		const unsigned char* const rowImage = this->row( y+pixelTop );
		unsigned char* rowNewData = newImage.row( y );

		// copy the row from image1 to image2
		for (unsigned int x = 0; x < newImage.width(); ++x)
		{
			// get the index to this image's pixel to copy from
			int imageX = (x + pixelLeft) * imageChannels;
			//int imageY = ((y + pixelTop) * imageWidth * imageChannels);// + ((y + pixelTop) * image.rowPadding());
			//const unsigned char* const rowImage = &( imagePtr[imageY] );
			
			// get the index to newImage's pixel to copy to
			int newImageX = x * newImageChannels;
			//int newImageY = y * newImageWidth * newImageChannels);// + (y + newImage.rowPadding());
			//unsigned char* rowNewData = &( newImagePtr[newImageY] );
			
			// copy the entire pixel.
			// TODO: this could be optimized...
			for (unsigned int c = 0; c < this->channels(); ++c)
			{
				rowNewData[newImageX + c] = rowImage[imageX + c];
			}
		}
	}
}

//: Get a pointer to the pixel data at one row.
// give - a row (between 0 and width()-1)
// cast this to a char* or a short* depending on the bpc() of the image. 
// if bpc()==1 then char*, if bpc()==2 then short* -- for index into one pixel CHANNEL
// if bpc()==0 and, depth()==16 or depth()==15 then short* -- for index into one PIXEL
// 15 bit image (555) rgb is typically:  0rrrrrgggggbbbbb
// 16 bit image (565) rgb is typically:  rrrrrggggggbbbbb
// 24 bit image (888) rgb is typically:  rrrrrrrrggggggggbbbbbbbb
unsigned char* Image::row( const int& whichRow )
{
	//assuming alignment is 0, find the byte which begins the [whichRow] row.
	const int pixelIndex		= whichRow * this->width();
	const int byteIndex			= pixelIndex * this->channels();
	
	//now consider padding (row alignment)
	const int numberOfRowPads	= whichRow * this->rowPadding();
	
	//offset the index by the padding to find the actual address of [whichRow]
	return &_data[ byteIndex + numberOfRowPads ];
}

//: Get a pointer to the pixel data at one row.
// give - a row (between 0 and width()-1)
// cast this to a char* or a short* depending on the bpc() of the image. 
// if bpc()==1 then char*, if bpc()==2 then short* -- for index into one pixel CHANNEL
// if bpc()==0 and, depth()==16 or depth()==15 then short* -- for index into one PIXEL
// 15 bit image (555) rgb is typically:  0rrrrrgggggbbbbb
// 16 bit image (565) rgb is typically:  rrrrrggggggbbbbb
// 24 bit image (888) rgb is typically:  rrrrrrrrggggggggbbbbbbbb
const unsigned char* Image::row( const int& whichRow ) const
{
	//assuming alignment is 0, find the byte which begins the [whichRow] row.
	const int pixelIndex		= whichRow * this->width();
	const int byteIndex		= pixelIndex * this->channels();
	
	//now consider padding (row alignment)
	const int numberOfRowPads	= whichRow * this->rowPadding();
	
	//offset the index by the padding to find the actual address of [whichRow]
	return &_data[ byteIndex + numberOfRowPads ];
}

void Image::blit( const Image& image, const int& xpos, const int& ypos )
{
	int wide = kev::Min( this->width(),  image.width()  );
	int high = kev::Min( this->height(), image.height() );

	assert( xpos >= 0 && "no support to blit image to a negetive position (yet)" );
	assert( ypos >= 0 && "no support to blit image to a negetive position (yet)" );

	// copy the subimage from this image into newImage.
	for (int y = 0; y < high; ++y)
	{
		// get an index to each row to be copied next.
		const unsigned char* rowSourceImage = image.row( y );
		unsigned char* const rowImage = this->row( y + ypos );
		
		// copy the row from image1 to image2
		for (int x = 0; x < wide; ++x)
		{
			// get the index to newImage's pixel to copy to
			int sourceImageX = x * image.channels();

			// get the index to this image's pixel to copy from
			int imageX = (x + xpos) * this->channels();
			
			// copy the entire pixel.
			// TODO: this could be optimized...
			for (unsigned int c = 0; c < this->channels(); ++c)
			{
				rowImage[imageX + c] = rowSourceImage[sourceImageX + c];
			}
		}
	}
}

//: align the bytes in each scanline to 'alignment' 
//  this will effectivly pad the end of each row with 
//  enough bytes so that the next row begins on a multiple of 'alignment' 
//  address some video cards render more efficiently with different byte 
//  alignments.
void Image::setRowAlignment( const unsigned int& alignment )
{
	_rowAlignment = alignment;
	_rowPadding = this->width() % alignment;

	Image temp;
	temp = *this;
	this->reserve( temp.width(), temp.height(), temp.bpp(), temp.channels(), 
                  _rowPadding );
	this->blit( temp, 0, 0 );
}

//: specify the 2 channels to swap.
//  copies the image while swapping the 2 channels
void Image::swapChannels( const Image& image, const unsigned int& channel1, 
                          const unsigned int& channel2 )
{
	if ( (image.width() != this->width()		) ||
		 (image.height() != this->height()		) ||
		 (image.bpp() != this->bpp()			) ||
		 (image.channels() != this->channels()  ) ||
		 (image.rowAlignment() != this->rowAlignment()) )
	{
		this->reserve( image.width(), image.height(), image.bpp(), 
                     image.channels(), image.rowAlignment() );
	}

	assert( image.size() == this->size() && "Can't swap channels" );
	assert( channel1 <= (this->channels()-1) && "you must specify a valid channel 1" );
	assert( channel2 <= (this->channels()-1) && "you must specify a valid channel 2" );
	
	if (channel1 == channel2)
		return;

	char temp1, temp2;
	for (unsigned int y = 0; y < this->height(); ++y)
	{
		const unsigned char* const rowSrc = image.row( y );
		unsigned char* rowDest = this->row( y );

		for (unsigned int x = 0; x < this->width() * this->channels(); x += this->channels())
		{
			temp1 = rowSrc[x + channel1];
			temp2 = rowSrc[x + channel2];
			rowDest[x + channel1] = temp2;
			rowDest[x + channel2] = temp1;
		}
	}
}

//: specify the 2 channels to swap.
void Image::swapChannels( const unsigned int& channel1, 
                          const unsigned int& channel2 )
{
	assert( channel1 <= (this->channels()-1) && "you must specify a valid channel 1" );
	assert( channel2 <= (this->channels()-1) && "you must specify a valid channel 2" );
	
	if (channel1 == channel2)
		return;

	char temp;
	for (unsigned int y = 0; y < this->height(); ++y)
	{
		unsigned char* row = this->row( y );

		for (unsigned int x = 0; x < this->width() * this->channels(); x += this->channels())
		{
			temp = row[x + channel1];
			row[x + channel1] = row[x + channel2];
			row[x + channel2] = temp;
		}
	}
}

void Image::asciioutput( bool useCarriageReturns ) const
{
   if (useCarriageReturns)
   {
      unsigned int bytesWide = this->width() * this->channels() * this->bpc() + this->rowPadding();
      for ( unsigned int te = 0; te < this->size(); ++te )
      {
         std::cout<<(int)this->data()[te]<<" ";

         if ( (te % bytesWide) == ((bytesWide)-1) )
         {
	         std::cout<<'\n';
         }
      }
   } 
   else 
   {
      for (unsigned int te = 0; te < this->size(); ++te)
      {
	      std::cout<<this->data()[te];
      }
   }
}

// some notes:
//IDEA: make a base class IImage which Image derives from.  
//      make a IImage subclass SubImage, which aggregates an IImage*.
//      SubImage will reference it's IImage* with calls to data(), row(), 
//      and pixel().
//BAD: wouldn't be able to pass SubImage's .data ptr to opengl, right???
