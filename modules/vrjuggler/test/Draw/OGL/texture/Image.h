
//////////////////////////////////////////////////////////////////////////////
//
//                         -=     Image class     =-
//
// Description: "an image, bitmap, pixmap or whatever"
// This code contributed from software located:
// http://www.bigfoot.com/~KevinMeinert/html/software/index.html
// -- Kevin Meinert, kevinmeinert@bigfoot.com
//////////////////////////////////////////////////////////////////////////////
#ifndef IMAGE_INCLUDED
#define IMAGE_INCLUDED

#include <string>
#include <iostream>

//: Image class
//  "an image: pixmap, bitmap, or whatever."
class Image
{	
// construction:
public:
	//: Default constructor
	Image();

	//: Copy constructor		
	Image(const Image& image);

	//: Data copy constructor
	// - set the source pixel data (bytes), this must be a valid pointer, 
	//   and must contain the data you specify in the next few fields.
	// - set copyData to true to copy the data into this Image, otherwise Image just stores the pointer.
	// - set bpp to the colordepth of your data: 8, 15, 16, 24, or 32 bit
	// - set xsize and ysize to the size in pixels of your bitmap...
	// - set channels to the number of channels in your data
	// - specify your data's byte alignment (usually 1 if no row padding)
	//  NOTE: memory will not be free'd on destruct, if copyData is false (copyData == false implies Image does not own the data)
	Image(unsigned char* bytes, const bool& copyData, const unsigned int xsize, const unsigned int ysize, const unsigned int bpp, const unsigned int channels, const unsigned int alignment = 1);


	//: Destructor
	~Image();

// operations:
public:	
	//: copy one image into another.
	const Image &	    operator=( const Image& image);

	//: copy an image into another.
	//  NOTE: when using an external pointer to data, 
	//        (with useExternalPointer()) copy() will 
	//        make a _new_ copy of all data.
	const Image &	    copy(const Image& image);

	//: Free's memory taken by current image.
	//  This has no effect when you use useExternalPointer() to set the data.
	void		    free();

	//: Allocate memory for the image (safe)
	// - bpp is the colordepth of 8, 15, 16, 24, or 32 bit
	// - xsize and ysize is the size in pixels of your bitmap...
	// - channels is usually 3 (for RGB) or 4 (for RGBA)
	// - alignment should be 1 normally, this aligns rows to multiple of 1 addresses.
	void		    reserve(const unsigned int xsize, const unsigned int ysize, const unsigned int bpp, const unsigned int channels, const unsigned int alignment = 1);

	//: set the image memory yourself 
	// - set the source pixel data (bytes), this must be a valid pointer, 
	//   and must contain the data you specify in the next few fields.
	// - set copyData to true to copy the data into this Image, otherwise Image just stores the pointer.
	// - set bpp to the colordepth of your data: 8, 15, 16, 24, or 32 bit
	// - set xsize and ysize to the size in pixels of your bitmap...
	// - set channels to the number of channels in your data
	// - specify your data's byte alignment (usually 1 if no row padding)
	//  NOTE: memory will not be free'd on destruct, if copyData is false (copyData == false implies Image does not own the data)
	void		    setData(unsigned char* bytes, const bool& copyData, const unsigned int xsize, const unsigned int ysize, const unsigned int bpp, const unsigned int channels, const unsigned int alignment = 1);

	//: set the name of the image.
	void		    setName( const char* const name );

	//: get a subimage of this image. (top=0,left=0,bottom>0,right>0)
	//  allows you to cut out a square area of this image
	//  give - the sub image coordinates, these must be within this image's pixel coordinates 
	//  returns - copies the specified sub image into the Image object that you provide.
	void			getSubImage(const unsigned int& pixelLeft, const unsigned int& pixelTop, const unsigned int& pixelRight, const unsigned int& pixelBottom, Image& newImage) const;

	void			asciioutput( bool useCarriageReturns ) const;

	//: specify the 2 channels to swap.
	void			swapChannels( const unsigned int& channel1, const unsigned int& channel2 );

	//: specify the 2 channels to swap.
	//  copies the image while swapping the 2 channels
	void			swapChannels( const Image& image, const unsigned int& channel1, const unsigned int& channel2 );

	//: blit an image to xpos,ypos 
	//  source = image
	//  dest = this
	//  NOTE: this is a memory blit, not a video memory blit.
	void			blit(const Image& image, const int& xpos, const int& ypos );

	//: align the bytes in each scanline to 'alignment' 
	//  this will effectivly pad the end of each row with 
	//  enough bytes so that the next row begins on a multiple of 'alignment' address
	//  some video cards render more efficiently with different byte alignments.
	void			setRowAlignment( const unsigned int& alignment );

// aliases:
public:
	//: Get the image width in pixels
	const unsigned int&		width() const;

	//: Get the image width in pixels
	unsigned int&			width();

	//: Get the image height in pixels
	const unsigned int&		height() const;

	//: Get the image height in pixels
	unsigned int&			height();

	//: Get color depth of current image
	//  Returns a number: 15, 16, 24, or 32.
	const unsigned int&		depth()const;

	//: Get number of bits per pixel
	//  Typical values include 8, 15, 16, 24, 32
	const unsigned int&		bpp() const;

	//: Get size of current image
	//  Returns a number in bytes.
	unsigned long			size()const;

	//: Get size of one pixel in bytes
	int						getBytesPerPixel() const;

	//: number of colors available in image.
	unsigned long           colors() const;
	
	//: Get a pointer to the pixel data.
	// cast this to a char* or a short* depending on the bpc() of the image. 
	// if bpc()==1 then char*, if bpc()==2 then short* -- for index into one pixel CHANNEL
	// if bpc()==0 and, depth()==16 or depth()==15 then short* -- for index into one PIXEL
	// 15 bit image (555) rgb is typically:  0rrrrrgggggbbbbb
	// 16 bit image (565) rgb is typically:  rrrrrggggggbbbbb
	// 24 bit image (888) rgb is typically:  rrrrrrrrggggggggbbbbbbbb
	unsigned char*			data();

	//: Get a pointer to the pixel data.
	// cast this to a char* or a short* depending on the bpc() of the image. 
	// if bpc()==1 then char*, if bpc()==2 then short* -- for index into one pixel CHANNEL
	// if bpc()==0 and, depth()==16 or depth()==15 then short* -- for index into one PIXEL
	// 15 bit image (555) rgb is typically:  0rrrrrgggggbbbbb
	// 16 bit image (565) rgb is typically:  rrrrrggggggbbbbb
	// 24 bit image (888) rgb is typically:  rrrrrrrrggggggggbbbbbbbb
	const unsigned char*	data() const;

	//: Get a pointer to the pixel data at one row.
	// give - a row (between 0 and width()-1)
	// cast this to a char* or a short* depending on the bpc() of the image. 
	// if bpc()==1 then char*, if bpc()==2 then short* -- for index into one pixel CHANNEL
	// if bpc()==0 and, depth()==16 or depth()==15 then short* -- for index into one PIXEL
	// 15 bit image (555) rgb is typically:  0rrrrrgggggbbbbb
	// 16 bit image (565) rgb is typically:  rrrrrggggggbbbbb
	// 24 bit image (888) rgb is typically:  rrrrrrrrggggggggbbbbbbbb
	unsigned char*			row( const int& row );

	//: Get a pointer to the pixel data at one row.
	// give - a row (between 0 and width()-1)
	// cast this to a char* or a short* depending on the bpc() of the image. 
	// if bpc()==1 then char*, if bpc()==2 then short* -- for index into one pixel CHANNEL
	// if bpc()==0 and, depth()==16 or depth()==15 then short* -- for index into one PIXEL
	// 15 bit image (555) rgb is typically:  0rrrrrgggggbbbbb
	// 16 bit image (565) rgb is typically:  rrrrrggggggbbbbb
	// 24 bit image (888) rgb is typically:  rrrrrrrrggggggggbbbbbbbb
	const unsigned char*	row( const int& row ) const;

	//: Get a pointer to the pixel data starting at x,y in the image
	// give - a row (between 0 and width()-1)
	// cast this to a char* or a short* depending on the bpc() of the image. 
	// if bpc()==1 then char*, if bpc()==2 then short* -- for index into one pixel CHANNEL
	// if bpc()==0 and, depth()==16 or depth()==15 then short* -- for index into one PIXEL
	// 15 bit image (555) rgb is typically:  0rrrrrgggggbbbbb
	// 16 bit image (565) rgb is typically:  rrrrrggggggbbbbb
	// 24 bit image (888) rgb is typically:  rrrrrrrrggggggggbbbbbbbb
	unsigned char*			pixel( const int& x, const int& y );

	//: Get a pointer to the pixel data starting at x,y in the image
	// give - a row (between 0 and width()-1)
	// cast this to a char* or a short* depending on the bpc() of the image. 
	// if bpc()==1 then char*, if bpc()==2 then short* -- for index into one pixel CHANNEL
	// if bpc()==0 and, depth()==16 or depth()==15 then short* -- for index into one PIXEL
	// 15 bit image (555) rgb is typically:  0rrrrrgggggbbbbb
	// 16 bit image (565) rgb is typically:  rrrrrggggggbbbbb
	// 24 bit image (888) rgb is typically:  rrrrrrrrggggggggbbbbbbbb
	const unsigned char*	pixel( const int& x, const int& y ) const;

	

	
	
// other aliases that may or may not be set (set them yourself)
public:
	//: Get number of bytes per pixel's channel
	//  Should return only '1' or '2' bytes
	unsigned int			bpc() const;

	//: Get number of color channels in the image
	//  Returns a number: '1' (BW), '3' (RGB), '4' (RGBA)
	const unsigned int&		channels()const;

	//: Get number of color channels in the image
	//  Returns a number: '1' (BW), '3' (RGB), '4' (RGBA)
	unsigned int&			channels();

	//: name of the image (if set)
	const char* 			name() const;

	//: size of row padding in bytes.
	const unsigned int&		rowPadding() const;

	//: row alignment, each row is a multiple of:
	const unsigned int&		rowAlignment() const;
	
protected:
	//: The image data
	unsigned char*          _data;		
	
	//: Number of bytes per pixel channel == 1 or 2
	//unsigned int            _bpc;           
	
	//XSIZE - The width of the image in pixels 
 	unsigned int            _xsize;		// X size in pixels 
	
	//YSIZE - The height of the image in pixels 
 	unsigned int            _ysize;		// Y size in pixels 

	unsigned int            _bpp;

	//ZSIZE - The number of channels in the image.  
	// B/W (greyscale) images are stored as 2 dimensional images with an ZSIZE ofr 1.  
	// RGB color images are stored as 3 dimensional images with a ZSIZE of 3.  
	// RGBA color images are stored as a 3 dimensional image with a ZSIZE of 4.
	unsigned int            _channels;		// Number of channels        

	std::string             _name;

	unsigned int			_rowPadding;
	unsigned int			_rowAlignment;

	//invert the image...
	/*void			negate()
	{
	    for(int x=0; x<_XSIZE*_YSIZE*_ZSIZE; x++)
	    {
		if(this->getBpc() == sizeof(char))
		    ((char*)_data)[x] = ~(((char*)_data)[x]);
		else
		    ((short*)_data)[x] = ~(((short*)_data)[x]);
	    }
	}*/
	
public:
	//unsigned int bindId;
	//bool isBound;
		
	//: is it using a pointer for the data?
	bool _usingPointer;
   
public:
   // whatever...
   void outputImageProperties() const;
};



//: copy one image into another.
inline const Image & Image::operator=( const Image& image)
{
	this->copy( image );
	return *this;
}

//: Get the image width in pixels
inline const unsigned int& Image::width() const
{
	return _xsize;
}
//: Get the image width in pixels
inline unsigned int& Image::width()	
{
	return _xsize;
}

//: Get the image height in pixels
inline const unsigned int& Image::height() const
{
	return _ysize;
}
//: Get the image height in pixels
inline unsigned int& Image::height()
{
	return _ysize;
}

//: Get number of bytes per pixel's channel
//  Should return only '1' or '2' bytes
inline unsigned int Image::bpc() const
{
	int bytesPerPixel = _bpp / 8;
	return bytesPerPixel / _channels;
	//return _bpc;
}

//: Get number of bits per pixel
//  Typical values include 8, 15, 16, 24, 32
inline const unsigned int&	Image::bpp() const
{
	return _bpp;
}

//: Get number of color channels in the image
//  Returns a number: '1' (BW), '3' (RGB), '4' (RGBA)
inline const unsigned int& Image::channels()const
{
	return _channels;
}

//: Get number of color channels in the image
//  Returns a number: '1' (BW), '3' (RGB), '4' (RGBA)
inline unsigned int& Image::channels()
{
	return _channels;
}

//: Get color depth of current image
//  Returns a number: 15, 16, 24, or 32.
inline const unsigned int& Image::depth()const
{
	return _bpp;
}

//: Get size of current image
//  Returns a number in bytes.
inline unsigned long Image::size()const
{
	if (_data == NULL) 
		return 0;
	else 
		return _xsize * _ysize * _channels * this->bpc() + (_ysize*_rowPadding);
}

//: Get a pointer to the pixel data.
// cast this to a char* or a short* depending on the bpc() of the image. 
// if bpc()==1 then char*, if bpc()==2 then short* -- for index into one pixel CHANNEL
// if bpc()==0 and, depth()==16 or depth()==15 then short* -- for index into one PIXEL
// 15 bit image rgb is typically:          -rrrrrgggggbbbbb
// 16 bit image rgb is typically:          rrrrrggggggbbbbb
// 24 bit image rgb is typically:  rrrrrrrrggggggggbbbbbbbb
inline unsigned char* Image::data()
{
	return _data;
}

//: Get a pointer to the pixel data.
// cast this to a char* or a short* depending on the bpc() of the image. 
// if bpc()==1 then char*, if bpc()==2 then short* -- for index into one pixel CHANNEL
// if bpc()==0 and, depth()==16 or depth()==15 then short* -- for index into one PIXEL
// 15 bit image rgb is typically:          -rrrrrgggggbbbbb
// 16 bit image rgb is typically:          rrrrrggggggbbbbb
// 24 bit image rgb is typically:  rrrrrrrrggggggggbbbbbbbb
inline const unsigned char* Image::data()const
{
	return _data;
}

//: Get a pointer to the pixel data starting at x,y in the image
// give - a row (between 0 and width()-1)
// cast this to a char* or a short* depending on the bpc() of the image. 
// if bpc()==1 then char*, if bpc()==2 then short* -- for index into one pixel CHANNEL
// if bpc()==0 and, depth()==16 or depth()==15 then short* -- for index into one PIXEL
// 15 bit image (555) rgb is typically:  0rrrrrgggggbbbbb
// 16 bit image (565) rgb is typically:  rrrrrggggggbbbbb
// 24 bit image (888) rgb is typically:  rrrrrrrrggggggggbbbbbbbb
inline unsigned char* Image::pixel( const int& x, const int& y )
{
	// must specify an X within the row, (otherwise increment y, and subtract width from x)
	//Assert( x < this->width(), "You must specify a column less than the width of the image." );

	return & (this->row( y )[ x * this->channels() * this->bpc() ]);
}

//: Get a pointer to the pixel data starting at x,y in the image
// give - a row (between 0 and width()-1)
// cast this to a char* or a short* depending on the bpc() of the image. 
// if bpc()==1 then char*, if bpc()==2 then short* -- for index into one pixel CHANNEL
// if bpc()==0 and, depth()==16 or depth()==15 then short* -- for index into one PIXEL
// 15 bit image (555) rgb is typically:  0rrrrrgggggbbbbb
// 16 bit image (565) rgb is typically:  rrrrrggggggbbbbb
// 24 bit image (888) rgb is typically:  rrrrrrrrggggggggbbbbbbbb
inline const unsigned char*	Image::pixel( const int& x, const int& y ) const
{
	// must specify an X within the row, (otherwise increment y, and subtract width from x)
	//Assert( x < this->width(), "You must specify a column less than the width of the image." );

	return & (this->row( y )[ x * this->channels() * this->bpc() ]);
}

//: name of the image (if set)
inline const char* Image::name() const
{
	return _name.data();
}

//: set the name of the image.
inline void Image::setName( const char* const name )
{
	_name = name;
	_name += '\0';
	_name[ strlen( name ) ] = '\0';
}

//: number of colors available in image.
inline unsigned long Image::colors() const
{
	switch( _bpp )
	{
		case 1: 
			return (unsigned long)2;
		case 4: 
			return (unsigned long)0x0000000f + 1;
		case 8: 
			return (unsigned long)0x000000ff + 1;
		case 15:
			return (unsigned long)0x0000efff + 1;
		case 16: 
			return (unsigned long)0x0000ffff + 1;
		case 24:
			return (unsigned long)0x00ffffff + 1;
		case 32:
			return (unsigned long)0xffffffff;
		default: 
			return 0;
	}
}

//: Get size of one pixel in bytes
inline int	Image::getBytesPerPixel() const
{
	switch( _bpp )
	{
		case 1: 
			return 0;//1.0f / 8.0f;
		case 4: 
			return 0;//0.5f;
		case 8: 
			return 1;
		case 15:
			return 2;
		case 16: 
			return 2;
		case 24:
			return 3;
		case 32:
			return 4;
		default: 
			//Assert(false, "Unsupported bit depth" );
			return 0;
	}
}

//: size of row padding in bytes.
inline const unsigned int& Image::rowPadding() const
{
	return _rowPadding;
}

//: row alignment, each row is a multiple of:
inline const unsigned int& Image::rowAlignment() const
{
	return _rowAlignment;
}

inline void Image::outputImageProperties() const
{
    std::cout<<"Image Properties:\n\n";
    std::cout<<"Name            - "<<this->name()<<"\n";
    std::cout<<"Color Depth     - "<<this->bpp()<<"-bit color\n";
    std::cout<<"Size            - "<<this->width()<<" x "<<this->height()<<"\n";
    std::cout<<"Channels        - "<<this->channels()<<" color channel(s).\n";
    std::cout<<"BPC             - "<<this->bpc()<<" byte(s) per channel\n";
    std::cout<<"Memory Alignent - "<<this->rowAlignment()<<"-byte boundary addressing\n";
}

#endif
