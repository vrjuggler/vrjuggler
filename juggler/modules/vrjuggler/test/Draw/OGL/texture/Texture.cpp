
//////////////////////////////////////////////////////////////////
//
//                         -=     Texture     =-
//
// Definition: "Texture class."
//
///////////////// <auto-copyright BEGIN do not edit this line> /////////////////
//
//    $RCSfile$
//    $Date$
//    $Revision$
//    Copyright (C) 1998, 1999, 2000  Kevin Meinert, KevinMeinert@bigfoot.com
//
//    This library is free software; you can redistribute it and/or
//    modify it under the terms of the GNU Library General Public
//    License as published by the Free Software Foundation; either
//    version 2 of the License, or (at your option) any later version.
//
//    This library is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//    Library General Public License for more details.
//
//    You should have received a copy of the GNU Library General Public
//    License along with this library; if not, write to the Free
//    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
///////////////// <auto-copyright END do not edit this line> ///////////////////

//////////////////////////////////////////////////////////////////////////////
#include "Texture.h" // My Header

//	Constructor.
Texture::Texture() : model( MODULATE ), wrapT( REPEAT ), 
		    wrapS( REPEAT ), 
		    mImage(), 
		    filtering( NEAREST ), mData( NULL )
{
}

Texture::Texture( const Texture &texture ) : model( MODULATE ), wrapT( REPEAT ), 
		    wrapS( REPEAT ), 
		    mImage(), 
		    filtering( NEAREST ), mData( NULL )
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
