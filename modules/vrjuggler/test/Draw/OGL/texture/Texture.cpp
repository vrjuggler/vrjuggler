
//////////////////////////////////////////////////////////////////////////////
//
//                          -=     Texture     =-
//
// Description: "Texture class: aggregates Image and other texturing parameters"
// This code contributed from software located:
// http://www.bigfoot.com/~KevinMeinert/html/software/index.html
// -- Kevin Meinert, kevinmeinert@bigfoot.com
//////////////////////////////////////////////////////////////////////////////
#include "Texture.h" // My Header

//	Constructor.
Texture::Texture() : model( MODULATE ), 
		    wrapS( REPEAT ), wrapT( REPEAT ), 
		    filtering( NEAREST ), mImage(), 
		    mData( NULL )
{
}

Texture::Texture( const Texture &texture ) : model( MODULATE ), 
		    wrapS( REPEAT ), wrapT( REPEAT ), 
		    filtering( NEAREST ), mImage(), 
		    mData( NULL )
{
    // reference the image.
    this->setImage( texture.mImage );
	
    wrapS   = texture.wrapS; //repeat,clamp
    wrapT   = texture.wrapT; //repeat,clamp
    model   = texture.model; //blend,modulate,decal
    filtering = texture.filtering;
}

//: set the image of the texture.
// if this is NULL, then texture will not render or compile.
// Must do this before compile(), or render()
void Texture::setImage( const Image& image )
{
   mImage = image;
}
