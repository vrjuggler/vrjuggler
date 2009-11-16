/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2009 by Iowa State University
 *
 * Original Authors:
 *   Allen Bierbaum, Christopher Just,
 *   Patrick Hartling, Kevin Meinert,
 *   Carolina Cruz-Neira, Albert Baker
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

package org.vrjuggler.vrjconfig.commoneditors.positionaldeviceeditor;

import java.awt.*;
import java.awt.event.*;
import java.awt.image.*;
import java.io.*;
import java.nio.*;
import java.util.*;
import javax.imageio.*;
import javax.swing.*;

import net.java.games.jogl.*;
import net.java.games.jogl.util.*;

/**
 * Stores an image to be used as a texture in OpenGL.
 *
 * @author      Marie Defurio
 */
public class Texture
{
   /** Texture ID that OpenGL associates with this texture. */
   private int mTextureId;

   /**
    * Load the image to be used as a texture and set the
    * correct OpenGL parameters.
    *
    * @author      Marie Defurio
    * @param gl         reference to OpenGL C interface
    * @param glu        reference to GLU C interface
    * @param file       location of image to load
    * @see net.java.games.jogl.GL
    * @see net.java.games.jogl.GLU
    */
   public Texture(GL gl, GLU glu, String file)
   {
      // Load the texture.
      mTextureId = genTexture(gl);
      gl.glBindTexture(gl.GL_TEXTURE_2D, mTextureId);
      BufferedImage img = readPNGImage(file);
      makeRGBTexture(gl, glu, img, gl.GL_TEXTURE_2D, true);

      gl.glTexParameterf(gl.GL_TEXTURE_2D, gl.GL_TEXTURE_WRAP_S, gl.GL_REPEAT);
      gl.glTexParameterf(gl.GL_TEXTURE_2D, gl.GL_TEXTURE_WRAP_T, gl.GL_REPEAT);
      gl.glTexParameterf(gl.GL_TEXTURE_2D, gl.GL_TEXTURE_MAG_FILTER, gl.GL_LINEAR);
      gl.glTexParameterf(gl.GL_TEXTURE_2D, gl.GL_TEXTURE_MIN_FILTER, gl.GL_LINEAR);
   }
  
   /**
    * Clean up by deleting texture from OpenGL engine.
    *
    * @author      Marie Defurio
    * @param gl         reference to OpenGL C interface
    */
   public void clean(GL gl)
   {
      int[] tmp = new int[1];
      tmp[0] = mTextureId;
      gl.glDeleteTextures(1, tmp);
   }

   /**
    * Bind this texture before use.
    *
    * @author      Marie Defurio
    * @param gl         reference to OpenGL C interface
    */
   public void bind(GL gl)
   {
      gl.glBindTexture(gl.GL_TEXTURE_2D, mTextureId);
   }

   /**
    * Unbind this texture after use.
    *
    * @author      Marie Defurio
    * @param gl         reference to OpenGL C interface
    */
   public void unbind(GL gl)
   {
      gl.glBindTexture(gl.GL_TEXTURE_2D, 0);
   }
  
   /**
    * Generate a availible texture ID.
    *
    * @author      Marie Defurio
    * @param gl         reference to OpenGL C interface
    */
   private int genTexture(GL gl)
   {
      int[] tmp = new int[1];
      gl.glGenTextures(1, tmp);
      return tmp[0];
   }

   /**
    * Load a PNG image file into memory.
    *
    * @author      Marie Defurio
    * @param resource_name      file to load into memory
    */
   private BufferedImage readPNGImage(String resource_name)
   {
      try
      {
         // Note: use of BufferedInputStream works around 4764639/4892246
         //XXX: Get out of jar
         BufferedImage img = ImageIO.read(new BufferedInputStream(
               getClass().getClassLoader().getResourceAsStream(resource_name)));
         //BufferedImage img = ImageIO.read(new BufferedInputStream(resource_name));
         if (img == null)
         {
            throw new RuntimeException("Error reading resource " + resource_name);
         }
         return img;
      }
      catch (IOException e)
      {
         throw new RuntimeException(e);
      }
   }

   /**
    * Convert the PNG image into a usable format and give the data to the OpenGl engine.
    *
    * @author      Marie Defurio
    * @param gl         reference to OpenGL C interface
    * @param glu        reference to GLU C interface
    * @param img        image to be loaded
    * @param target     OpenGL texture ID to associate with this texture
    * @param mipmapped  should we use mipmapping
    */
   private void makeRGBTexture(GL gl, GLU glu, BufferedImage img, int target, boolean mipmapped)
   {
      ByteBuffer dest = null;
      switch (img.getType())
      {
         case BufferedImage.TYPE_3BYTE_BGR:
         case BufferedImage.TYPE_CUSTOM:
         {
            byte[] data = ((DataBufferByte) img.getRaster().getDataBuffer()).getData();
            dest = ByteBuffer.allocateDirect(data.length);
            dest.order(ByteOrder.nativeOrder());
            dest.put(data, 0, data.length);
            break;
         }
         case BufferedImage.TYPE_INT_RGB:
         {
            int[] data = ((DataBufferInt) img.getRaster().getDataBuffer()).getData();
            dest = ByteBuffer.allocateDirect(data.length * BufferUtils.SIZEOF_INT);
            dest.order(ByteOrder.nativeOrder());
            dest.asIntBuffer().put(data, 0, data.length);
            break;
         }
         default:
            throw new RuntimeException("Unsupported image type " + img.getType());
      }

      /*
      if (mipmapped)
      {
         System.out.println("H: " + img.getHeight() + " W: " + img.getWidth());
         glu.gluBuild2DMipmaps(target, gl.GL_RGB8, img.getWidth(), img.getHeight(), gl.GL_RGB,
                               gl.GL_UNSIGNED_BYTE, dest);
      }
      else
      {
      */
         System.out.println("H: " + img.getHeight() + " W: " + img.getWidth());
         gl.glTexImage2D(target, 0, gl.GL_RGB, img.getWidth(), img.getHeight(), 0,
                         gl.GL_RGB, gl.GL_UNSIGNED_BYTE, dest);
      //}
   }
}
