
//////////////////////////////////////////////////////////////////////////////
//
//                          -=     Texture     =-
//
// Description: "Texture class: aggregates Image and other texturing parameters"
// This code contributed from software located:
// http://www.bigfoot.com/~KevinMeinert/html/software/index.html
// -- Kevin Meinert, kevinmeinert@bigfoot.com
//////////////////////////////////////////////////////////////////////////////
#ifndef TEXTURE_INCLUDED
#define TEXTURE_INCLUDED

#include <assert.h>

#include "Image.h"

//: Texture node.
class Texture
{
public:
    Texture();
    Texture( const Texture &texture );
    virtual ~Texture()
    {
    }
    
    enum Model 
    {
	 //The texture color is multiplied by the surface color
         MODULATE, 
	 
	 //The texture color replaces the surface color
         DECAL, 
	 
	 //Blends between the surface color and current geometry's color
         BLEND
    };
    
    enum Filtering
    {
	    NEAREST,
	    LINEAR,
	    MIPMAP_NEAREST, 
	    MIPMAP_LINEAR
    };

    enum Wrap 
    {
    //Repeats texture outside 0-1 texture coordinate range
         REPEAT, 
	 
	 //Clamps texture coordinates to lie within 0-1 range
         CLAMP
    };

    //: set the image of the texture.
    // if this is NULL, then texture will not render or compile.
    // Must do this before compile(), or render()
    void			setImage( const Image& image );
    
    //: an alias to this texture's image.
    Image& 			image();
    const Image&  image() const;
    
    //:Specifies how to map texture onto surface.
    // MODULATE, DECAL, or BLEND
    Model			model;
    
    //: Specify how texture is wrapped onto surface
    // REPEAT, or CLAMP
    Wrap			wrapS;
    Wrap			wrapT;
    
    //WARNING: set this before requesting to use texture object.
    Filtering			filtering;
    
    // unused...
   //mutable unsigned int bindId;
	//mutable bool isBound;

   void setUserData( void* data ) 
   { 
      mData = data; 
   }
   void* userData() 
   { 
      assert( mData != NULL && "you must setUserData(..) before using userData()" ); 
      return mData; 
   }
protected:
	Image			mImage;
   void*       mData;
};

//: an alias to this texture's image.
inline Image&			Texture::image() { return mImage; }
inline const Image& 	Texture::image() const { return mImage; }

#endif
