package org.vrjuggler.vrjconfig.commoneditors.positionaldeviceeditor;

import java.util.List;
import gleem.linalg.*;
import net.java.games.jogl.*;
import net.java.games.jogl.util.GLUT;

/** A cube of width, height, and depth 2, centered about the origin
    and aligned with the X, Y, and Z axes. */
public class Intersense extends MovableObject
{
   private static final Vec3f[] vertices = {
      // Front side
      new Vec3f(-1.8288f, 0.03f, 1.2192f+0.03f),
      new Vec3f(-1.8288f, -0.03f, 1.2192f+0.03f),
      new Vec3f(0f, -0.03f, 1.2192f+0.03f),
      new Vec3f(0f, 0.03f, 1.2192f+0.03f),
      // Back side
      new Vec3f(-1.8288f, 0.03f, 1.2192f-0.03f),
      new Vec3f(-1.8288f, -0.03f, 1.2192f-0.03f),
      new Vec3f(0f, -0.03f, 1.2192f-0.03f),
      new Vec3f(0f, 0.03f, 1.2192f-0.03f)
   };

   private static final int[] vertexIndices = {
      // Front face
      0, 1, 2, 3,
      // Right face
      3, 2, 6, 7,
      // Back face
      7, 6, 5, 4,
      // Left face
      4, 5, 1, 0,
      // Top face
      4, 0, 3, 7,
      // Bottom face
      1, 5, 6, 2
   };

   private static final Vec2f[] textureCoords = {
      // Front side
      new Vec2f(0, 0),
      new Vec2f(0, 1),
      new Vec2f(1, 1),
      new Vec2f(1, 0)
   };

   private static final int[] textureIndices = {
      // Front face
      0, 1, 2, 3,
      // Right face
      0, 1, 2, 3,
      // Back face
      0, 1, 2, 3,
      // Left face
      0, 1, 2, 3,
      // Top face
      0, 1, 2, 3,
      // Bottom face
      0, 1, 2, 3
   };

   private static Vec3f[] normals = {
      // Front face
      new Vec3f(0, 0, 1),
      // Right face
      new Vec3f(1, 0, 0),
      // Back face
      new Vec3f(0, 0, -1),
      // Left face
      new Vec3f(-1, 0, 0),
      // Top face
      new Vec3f(0, 1, 0),
      // Bottom face
      new Vec3f(0, -1, 0)
   };
   private static int[] normalIndices = {
      // Front face
      0, 0, 0, 0,
      // Right face
      1, 1, 1, 1,
      // Back face
      2, 2, 2, 2,
      // Left face
      3, 3, 3, 3,
      // Top face
      4, 4, 4, 4,
      // Bottom face
      5, 5, 5, 5
   };
  
   /** Texture to be displaed as the background. */
   private Texture mTexture = null;
   private GLUT glut = new GLUT();
      
   public Intersense(GL gl, GLU glu)
   {
      super();

      mTexture = new Texture(gl, glu, "org/vrjuggler/vrjconfig/commoneditors/positionaldeviceeditor/images/ert2.png");
   }

   /**
    * Render this Manipulator now using the given OpenGL
    * routines and assuming an OpenGL context is current.
    */
   public void render(GL gl, GLU glu)
   {
      boolean lightingOn = true;
      // FIXME: this is too expensive; figure out another way
      //  if (glIsEnabled(GL.GL_LIGHTING))
      //    lightingOn = true;

      if (lightingOn)
      {
         gl.glEnable(GL.GL_COLOR_MATERIAL);
         gl.glColorMaterial(GL.GL_FRONT_AND_BACK, GL.GL_AMBIENT_AND_DIFFUSE);
      }
      
      gl.glPushMatrix();
         float[] data = new float[16];
         mTransform.getColumnMajorData(data);
         gl.glMultMatrixf(data);
         
         //gl.glEnable(gl.GL_LIGHTING);
         
         mTexture.bind(gl);
         gl.glBegin(GL.GL_QUADS);
            gl.glColor3f(1.0f, 1.0f, 1.0f);
            int i = 0;
            while (i < vertexIndices.length)
            {
               Vec3f n0 = normals[normalIndices[i]];
               Vec3f v0 = vertices[vertexIndices[i]];
               Vec2f t0 = textureCoords[textureIndices[i]];
               gl.glNormal3f(n0.x(), n0.y(), n0.z());
               if (i < 4)
                  gl.glTexCoord2d(t0.x(), t0.y());
               gl.glVertex3f(v0.x(), v0.y(), v0.z());
               i++;

               Vec3f n1 = normals[normalIndices[i]];
               Vec3f v1 = vertices[vertexIndices[i]];
               Vec2f t1 = textureCoords[textureIndices[i]];
               gl.glNormal3f(n1.x(), n1.y(), n1.z());
               if (i < 4)
                  gl.glTexCoord2d(t1.x(), t1.y());
               gl.glVertex3f(v1.x(), v1.y(), v1.z());
               i++;

               Vec3f n2 = normals[normalIndices[i]];
               Vec3f v2 = vertices[vertexIndices[i]];
               Vec2f t2 = textureCoords[textureIndices[i]];
               gl.glNormal3f(n2.x(), n2.y(), n2.z());
               if (i < 4)
                  gl.glTexCoord2d(t2.x(), t2.y());
               gl.glVertex3f(v2.x(), v2.y(), v2.z());
               i++;

               Vec3f n3 = normals[normalIndices[i]];
               Vec3f v3 = vertices[vertexIndices[i]];
               Vec2f t3 = textureCoords[textureIndices[i]];
               gl.glNormal3f(n3.x(), n3.y(), n3.z());
               if (i < 4)
                  gl.glTexCoord2d(t3.x(), t3.y());
               gl.glVertex3f(v3.x(), v3.y(), v3.z());
               i++;
            }
         gl.glEnd();
         mTexture.unbind(gl);
         if (lightingOn)
            gl.glDisable(GL.GL_COLOR_MATERIAL);
      gl.glPopMatrix();
   }
}
