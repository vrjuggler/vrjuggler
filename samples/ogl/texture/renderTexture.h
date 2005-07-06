#ifndef _RENDER_TEXTURE_H_
#define _RENDER_TEXTURE_H_

//////////////////////////////////////////////////////////////////////////////
//
//                          -=     renderTexture     =-
//
// Description: "OpenGL texture rendering code"
// This code contributed from software located:
// http://www.bigfoot.com/~KevinMeinert/html/software/index.html
// -- Kevin Meinert, kevinmeinert@bigfoot.com
//////////////////////////////////////////////////////////////////////////////
#include <GL/gl.h>
#include "Texture.h"
#include "defines.h" // min
class tex
{
public:
   // listID: listID must == -1 (to mean unused tex object)
   static void bind( Texture& texture, int& texObjectID, int mipmapLevelOfDetail = 0, int bordersize = 0 )
   {
      assert( texObjectID == -1 && "listID must == -1 (to mean unused tex object)" );

      // find out what dimension texture we're using
      int texDimension;
      if (texture.image().height() == 1 || texture.image().width() == 1)
         texDimension = GL_TEXTURE_1D;	
      else 
         texDimension = GL_TEXTURE_2D;

      // create the texture object, setting the texObjectID
      unsigned int id;
#ifdef GL_VERSION_1_1
         ::glGenTextures( 1, &id );
         ::glBindTexture( texDimension, id );
#else
         ::glGenTexturesEXT( 1, &id );
         ::glBindTextureEXT( texDimension, id );
#endif
      texObjectID = id;

      // load the texture into hardware that will be referenced by the texObjectID
      tex::load( texture, mipmapLevelOfDetail, bordersize );
   }

   // texObjectID: must be valid and != -1
   static void unbind( Texture& texture, int& texObjectID )
   {
      assert( texObjectID != -1 && "texture object already deleted" );

      unsigned int id( texObjectID );
#ifdef GL_VERSION_1_1
         ::glDeleteTextures( 1, &id );
#else
         ::glDeleteTexturesEXT( 1, &id );
#endif

      texObjectID = -1; // unused
   }

   // return true is texObjectID is valid...
   static bool isBound( const Texture& texture, const int& texObjectID )
   {
      if (texObjectID == -1)
         return false;
      
      unsigned int id( texObjectID );
#ifdef GL_VERSION_1_1
         return ::glIsTexture( id ) == GL_TRUE;
#else
         return ::glIsTextureEXT( id ) == GL_TRUE;
#endif
   }
   
   // load texture data to hardware, making the texture current
   // NOTE: does not set any glEnable()s, use tex::render() instead of this function.
   // NOTE: this function results in slower performance than using the combination 
   //       of tex::bind() and tex::render()
   // NOTE: if you don't use the tex::bind() and tex::render() combination, then 
   //       tex::render() will call tex::load() after setting a lot of glEnable()s
   static bool load( const Texture& texture, int mipmapLevelOfDetail = 0, int bordersize = 0 )
   {
      int format, type;
      const Image& image = texture.image();

      // set the way that gl reads the pixels.
      switch (image.channels())
      {
      case 1: format = GL_LUMINANCE; break;
      case 3: format = GL_RGB;  break;
      case 4: format = GL_RGBA; break;
      default: 
         return false;
      }

      // set the byte alignment of the pixels.   
      if (image.bpc() == 2)
         type = GL_UNSIGNED_SHORT;
      else
         type = GL_UNSIGNED_BYTE;   
      
      //make sure the alignment matches the pixel size in bytes
      ::glPixelStorei( GL_UNPACK_ALIGNMENT, image.bpc() );

      // fatal error. Nothing to render
      if( image.data() == NULL )
      return false; 

      // find out what dimension texture we're using
      int texDimension;
      if( image.height() == 1 || image.width() == 1 )
      {
         // handle one dimensional images...
         int length = kev::Max( image.width(), image.height() );
         if (texture.filtering != Texture::NEAREST ||
             texture.filtering != Texture::LINEAR)
         {
            ::gluBuild1DMipmaps( GL_TEXTURE_1D, 
                                 image.channels(), length, 
                                 format, type, image.data() );
         } 
         else 
         {
            ::glTexImage1D( GL_TEXTURE_1D, mipmapLevelOfDetail, 
                           image.channels(), length, 
                           bordersize, format, type, image.data() );
         }

         texDimension = GL_TEXTURE_1D;	
      } 

      else 
      {
         // other wise, handle 2D images.
         if (texture.filtering == Texture::MIPMAP_NEAREST ||
             texture.filtering == Texture::MIPMAP_LINEAR)
         {
            ::gluBuild2DMipmaps( GL_TEXTURE_2D, 
                                 image.channels(), image.width(), 
                                 image.height(), format, type, 
                                 image.data() );
         }
         else 
         {
            ::glTexImage2D( GL_TEXTURE_2D, mipmapLevelOfDetail, 
                           image.channels(), image.width(), 
                           image.height(), bordersize, format, 
                           type, image.data() );
         }

         texDimension = GL_TEXTURE_2D;
      }

      // figure out what filtering to use.
      // set the filtering for the texture...
      switch (texture.filtering)
      {
      case Texture::NEAREST:
         ::glTexParameteri( texDimension, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
         ::glTexParameteri( texDimension, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
      break;

      case Texture::LINEAR:
         ::glTexParameteri( texDimension, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
         ::glTexParameteri( texDimension, GL_TEXTURE_MIN_FILTER, GL_LINEAR );	
      break;

      case Texture::MIPMAP_NEAREST:
         ::glTexParameteri( texDimension, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
         ::glTexParameteri( texDimension, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST );
      break;

      case Texture::MIPMAP_LINEAR:
         ::glTexParameteri( texDimension, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
         ::glTexParameteri( texDimension, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
      break;
      default:
         assert(false);
      }

      return true;
   }
   
   //: render texture
   // will make current a texture that has been created as a texure object, or not...
   // and sets all relevent glEnable()s.  Using this, you shouldn't have to do anything 
   // else with the gl texture functions.
   //
   // texture: make sure it's set up properly with a valid Image object, etc...
   // texObjectID: if == -1 then function will load the texture.image() data each time
   //              you should use the texture bind function before calling render for
   //              better performance
   static void render( const Texture& texture, const int& texObjectID )
   {
      const Image& image = texture.image();
      
      // find out what dimension texture we're using
      int texDimension;
      int texBindDimension;
      if (image.height() == 1 || image.width() == 1 )
      {
         texDimension = GL_TEXTURE_1D;	
#ifdef GL_VERSION_1_1
            texBindDimension = GL_TEXTURE_BINDING_1D;
#else
            texBindDimension = GL_TEXTURE_1D_BINDING;
#endif
      }
      else 
      {
         texDimension = GL_TEXTURE_2D;
#ifdef GL_VERSION_1_1
            texBindDimension = GL_TEXTURE_BINDING_2D;
#else
            texBindDimension = GL_TEXTURE_2D_BINDING;
#endif
      }
      
      // enable texturing for this dimension if not already
      if (::glIsEnabled( texDimension ) == GL_FALSE)
         ::glEnable( texDimension );

      // if we're rendering the same texture as what is current, then quit
      int currentTexId;
      ::glGetIntegerv( texBindDimension, &currentTexId );
      if (currentTexId == texObjectID)
         return;
      
      // Disable auto texture coord generation
      // TODO: limitation, no way to enable...
      if (glIsEnabled( GL_TEXTURE_GEN_T ) == GL_FALSE)
         ::glDisable( GL_TEXTURE_GEN_T );
      if (glIsEnabled( GL_TEXTURE_GEN_S ) == GL_FALSE)
         ::glDisable( GL_TEXTURE_GEN_S );
      

	   // figure out which GL model the texture uses
      int tex_env_mode  = -1;
      glGetTexEnviv( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, &tex_env_mode );
      switch (texture.model)
      {
      case Texture::MODULATE: 
         if (tex_env_mode != GL_MODULATE)
            ::glTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );
         break;
      case Texture::DECAL: 
         if (tex_env_mode != GL_DECAL)
            ::glTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL );
         break;
      case Texture::BLEND: 
         if (tex_env_mode != GL_BLEND)
            ::glTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_BLEND );
         break;
      }
	   
	   // set the wrap mode the texture uses for 'S' if not set already
      int tex_parameter = -1;
      ::glGetTexParameteriv( texDimension, GL_TEXTURE_WRAP_S, &tex_parameter );
      switch (texture.wrapS)
      {
      case Texture::REPEAT:
         if (tex_parameter != GL_REPEAT)
            ::glTexParameteri( texDimension, GL_TEXTURE_WRAP_S, GL_REPEAT );
         break;
      case Texture::CLAMP: 
         if (tex_parameter != GL_CLAMP)
            ::glTexParameteri( texDimension, GL_TEXTURE_WRAP_S, GL_CLAMP );
         break;
      }
	   
      // set the wrap mode the texture uses for 'T' if not set already
      ::glGetTexParameteriv( texDimension, GL_TEXTURE_WRAP_T, &tex_parameter );
      switch (texture.wrapT)
      {
      case Texture::REPEAT: 
         if (tex_parameter != GL_REPEAT)
            ::glTexParameteri( texDimension, GL_TEXTURE_WRAP_T, GL_REPEAT );
         break;
      case Texture::CLAMP: 
         if (tex_parameter != GL_CLAMP)
            ::glTexParameteri( texDimension, GL_TEXTURE_WRAP_T,  GL_CLAMP );
         break;
      }
	   
      if (texObjectID != -1) 
      {
#ifdef GL_VERSION_1_1
            ::glBindTexture( texDimension, texObjectID );
#else
            ::glBindTextureEXT( texDimension, texObjectID );
#endif
      }               
      // if the texture object is invalid, 
      // just load the pixels from memory to the graphics hardware
      // this will be major-slow but, you'll get results...
      // NOTE: if texObjectID==-1 then, you should use the texture bind function 
      //       before rendering for better performance
      else 
      {
         tex::load( texture );
      }
   }  
};

#endif /* _RENDER_TEXTURE_H_ */
