#ifndef _RENDER_TEXTURE_H_
#define _RENDER_TEXTURE_H_

//////////////////////////////////////////////////////////////////////////////
//
//                          -=     renderTexture     =-
//
// Description: "OpenGL texture rendering code for the Texture type"
// This code contributed from software located:
// http://www.bigfoot.com/~KevinMeinert/html/software/index.html
// -- Kevin Meinert, kevinmeinert@bigfoot.com
//////////////////////////////////////////////////////////////////////////////
#include "Texture.h" // the Texture type
#include "defines.h" // min

// a suite of OpenGL texture routines for use with the Texture type.
//
// load   - send the data to the graphics hardware
// bind   - create an opengl texture object
// unbind - delete an opengl texture object
class tex
{
public:
   // load texture data to hardware, making the texture current
   // NOTE: does not set any glEnable()s, use tex::render() instead of this function.
   // NOTE: this function results in slower performance than using the combination 
   //       of tex::bind() and tex::render()
   // NOTE: if you don't use the tex::bind() and tex::render() combination, then 
   //       tex::render() will call tex::load() after setting a lot of glEnable()s
   static bool load( const Texture& texture, int mipmapLevelOfDetail = 0, int bordersize = 0 );
   
   // is a Texture already an OpenGL texture object?
   // return true if texObjectID is a valid OpenGL texture object...
   static bool isBound( const Texture& texture, const int& texObjectID );
   
   // destroy an OpenGL texture object currently associated with a Texture
   // texObjectID: must be valid (not -1)
   static void unbind( Texture& texture, int& texObjectID );
   
   // create an OpenGL texture object from a Texture
   // give: your Texture, and texture object resource ID
   // texObjectID must == -1 (to mean unused tex object)
   static void bind( Texture& texture, int& texObjectID, int mipmapLevelOfDetail = 0, int bordersize = 0 );

   static void render( const Texture& texture, const int& texObjectID );
};


#endif /* _RENDER_TEXTURE_H_ */
