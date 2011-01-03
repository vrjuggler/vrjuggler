/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2011 by Iowa State University
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
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

package org.vrjuggler.vrjconfig.commoneditors.positionaldeviceeditor;

import java.util.List;
import gleem.linalg.*;
import net.java.games.jogl.*;
import net.java.games.jogl.util.GLUT;

/** A cube of width, height, and depth 2, centered about the origin
    and aligned with the X, Y, and Z axes. */
public class Ascension extends MovableObject
{
   private static final Vec3f[] vertices = {
      // Front side
      new Vec3f(-0.1525f, 0.1525f, 0.1525f),
      new Vec3f(-0.1525f, -0.1525f, 0.1525f),
      new Vec3f(0.1525f, -0.1525f, 0.1525f),
      new Vec3f(0.1525f, 0.1525f, 0.1525f),
      // Back side
      new Vec3f(-0.1525f, 0.1525f, -0.1525f),
      new Vec3f(-0.1525f, -0.1525f, -0.1525f),
      new Vec3f(0.1525f, -0.1525f, -0.1525f),
      new Vec3f(0.1525f, 0.1525f, -0.1525f)
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
      
   public Ascension(GL gl, GLU glu)
   {
      super();

      mTexture = new Texture(gl, glu, "org/vrjuggler/vrjconfig/commoneditors/positionaldeviceeditor/images/ert2.png");
   }
   
   static float BOUNDING_BOX_SIZE = 0.35f;
   //static double TRACKING_RANGE_RADIUS = 3.05;
   static double TRACKING_RANGE_RADIUS = 2.5;

   /**
    * Render this Manipulator now uMath.sing the given OpenGL
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
         
      gl.glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
      
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
        
         //gl.glEnable(gl.GL_BLEND);		    // Turn Blending On
         //gl.glDisable(gl.GL_DEPTH_TEST);         // Turn Depth Testing Off
         
         gl.glColor4f(0.0f, 1.0f, 0.0f, 1.0f);
         //glut.glutSolidSphere(glu, (double)BOUNDING_BOX_SIZE * 2.0f, 30, 30);
         gl.glRotatef(180.0f, 1.0f, 0.0f, 0.0f);
         glutSolidHemisphere(gl, glu, TRACKING_RANGE_RADIUS, 30, 30);
         
         //gl.glDisable(gl.GL_BLEND);		    // Turn Blending On
         //gl.glEnable(gl.GL_DEPTH_TEST);         // Turn Depth Testing Off
         
         if (lightingOn)
            gl.glDisable(GL.GL_COLOR_MATERIAL);
      gl.glPopMatrix();

   }

   private GLUquadric quadObj;
   private void quadObjInit(GLU glu)
   {
      if (quadObj == null)
      {
         quadObj = glu.gluNewQuadric();
      }
      if (quadObj == null)
      {
         throw new GLException("Out of memory");
      }
   }

   private void glutSolidHemisphere(GL gl, GLU glu, double radius, int slices, int stacks)
   {
      quadObjInit(glu);
      glu.gluQuadricDrawStyle(quadObj, GLU.GLU_FILL);
      glu.gluQuadricNormals(quadObj, GLU.GLU_SMOOTH);
      /* If we ever changed/used the texture or orientation state
         of quadObj, we'd need to change it to the defaults here
         with gluQuadricTexture and/or gluQuadricOrientation. */
      //glu.gluSphere(quadObj, radius, slices, stacks);
      gl.glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
      gluHemisphere(gl, glu, quadObj, radius, slices, stacks);
   }

   static double PI = 3.14159265358979323846;
  
   private void gluHemisphere(GL gl, GLU glu, GLUquadric qobj, double radius, int slices, int stacks)
   {
      double rho, drho, theta, dtheta;
      float x, y, z;
      float s, t, ds, dt;
      int i, j, imin, imax;
      boolean normals;
      float nsign;

      normals = true;
      nsign = 1.0f;
      /*
      if (qobj.Normals == glu.GLU_NONE)
      {
         normals = false;
      }
      else
      {
         normals = true;
      }
      if (qobj.Orientation == glu.GLU_INSIDE)
      {
         nsign = -1.0f;
      }
      else
      {
         nsign = 1.0f;
      }
      */

      drho = (PI/2) / (float) stacks;
      //drho = PI / (float) stacks;
      dtheta = 2.0f * PI / (float) slices;
      //dtheta = PI / (float) slices;

      double adjust = PI - (dtheta*slices);

      // draw stack lines
      // stack line at i==stacks-1 was missing here
      for (i = 1; i <= stacks; i++)
      {
         rho = i * drho;
         gl.glBegin(gl.GL_LINE_LOOP);
         for (j = 0; j < slices ; j++)
         {
            theta = j * dtheta;
            x = (float)(Math.cos(theta) * Math.sin(rho));
            y = (float)(Math.sin(theta) * Math.sin(rho));
            z = (float)(Math.cos(rho));
            if (normals)
            {
               gl.glNormal3f(x * nsign, y * nsign, z * nsign);
            }
            gl.glVertex3f((float)(x * radius), (float)(y * radius), (float)(z * radius));
         }
         gl.glEnd();
      }
      
      /* draw slice lines */
      for (j = 0; j < slices; j++)
      {
         theta = j * dtheta;
         gl.glBegin(gl.GL_LINE_STRIP);
         for (i = 0; i <= stacks; i++)
         {
            rho = i * drho;
            x = (float)(Math.cos(theta) * Math.sin(rho));
            y = (float)(Math.sin(theta) * Math.sin(rho));
            z = (float)(Math.cos(rho));
            if (normals)
            {
               gl.glNormal3f(x * nsign, y * nsign, z * nsign);
            }
            gl.glVertex3f((float)(x * radius), (float)(y * radius), (float)(z * radius));
         }
         gl.glEnd();
      }
      
      /*
      ds = 1.0f / slices;
      dt = 1.0f / stacks;
      t = 1.0f;			// because loop now runs from 0

      imin = 1;
      imax = stacks - 1;

      // draw intermediate stacks as quad strips
      for (i = imin; i < imax; i++)
      {
         rho = i * drho;
         gl.glBegin(gl.GL_QUAD_STRIP);
         s = 0.0f;
         for (j = 0; j <= slices; j++)
         {
            theta = (j == slices) ? 0.0f : j * dtheta;
            x = (float)(-Math.sin(theta) * Math.sin(rho));
            y = (float)(Math.cos(theta) * Math.sin(rho));
            z = (float)(nsign * Math.cos(rho));
            if (normals)
            {
               gl.glNormal3f(x * nsign, y * nsign, z * nsign);
            }
            gl.glVertex3f(x * (float)radius, y * (float)radius, z * (float)radius);
            x = (float)(-Math.sin(theta) * Math.sin(rho + drho));
            y = (float)(Math.cos(theta) * Math.sin(rho + drho));
            z = (float)(nsign * Math.cos(rho + drho));
            if (normals)
            {
               gl.glNormal3f(x * nsign, y * nsign, z * nsign);
            }
            s += ds;
            gl.glVertex3f(x * (float)radius, y * (float)radius, z * (float)radius);
         }
         gl.glEnd();
         t -= dt;
      }

      // draw -Z end as a triangle fan
      gl.glBegin(gl.GL_TRIANGLE_FAN);
      gl.glNormal3f(0.0f, 0.0f, -1.0f);
      gl.glVertex3f(0.0f, 0.0f, ((float)-radius) * nsign);
      rho = PI - drho;
      s = 1.0f;
      t = dt;
      for (j = slices; j >= 0; j--)
      {
         theta = (j == slices) ? 0.0f : (float)j * dtheta;
         x = (float)(-Math.sin(theta) * Math.sin(rho));
         y = (float)(Math.cos(theta) * Math.sin(rho));
         z = (float)(nsign * Math.cos(rho));
         if (normals)
         {
            gl.glNormal3f(x * nsign, y * nsign, z * nsign);
         }
         s -= ds;
         gl.glVertex3f(x * (float)radius, y * (float)radius, z * (float)radius);
      }
      gl.glEnd();
      */
   }
}
