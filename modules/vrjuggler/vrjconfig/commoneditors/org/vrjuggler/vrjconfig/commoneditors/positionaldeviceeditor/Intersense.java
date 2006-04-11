/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2005 by Iowa State University
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
 * -----------------------------------------------------------------
 * File:          $RCSfile$
 * Date modified: $Date$
 * Version:       $Revision$
 * -----------------------------------------------------------------
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

package org.vrjuggler.vrjconfig.commoneditors.positionaldeviceeditor;

import java.util.List;
import gleem.linalg.*;
import net.java.games.jogl.*;
import net.java.games.jogl.util.GLUT;

/** A cube of width, height, and depth 2, centered about the origin
    and aligned with the X, Y, and Z axes. */
public class Intersense extends MovableObject
{
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

         // Draw Axis
         gl.glLineWidth(2.0f);
         gl.glDisable(gl.GL_LIGHTING);
         Vec3f x_axis = new Vec3f(1.0f, 0.0f, 0.0f);
         Vec3f y_axis = new Vec3f(0.0f, 1.0f, 0.0f);
         Vec3f z_axis = new Vec3f(0.0f, 0.0f, 1.0f);
         Vec3f origin = new Vec3f(0.0f, 0.0f, 0.0f);

         gl.glBegin(gl.GL_LINES);
            gl.glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
            gl.glVertex3f(origin.x(), origin.y(), origin.z());
            gl.glVertex3f(x_axis.x(), x_axis.y(), x_axis.z());

            gl.glColor4f(0.0f, 1.0f, 0.0f, 1.0f);
            gl.glVertex3f(origin.x(), origin.y(), origin.z());
            gl.glVertex3f(y_axis.x(), y_axis.y(), y_axis.z());

            gl.glColor4f(0.0f, 0.0f, 1.0f, 1.0f);
            gl.glVertex3f(origin.x(), origin.y(), origin.z());
            gl.glVertex3f(z_axis.x(), z_axis.y(), z_axis.z());
         gl.glEnd();
         gl.glEnable(gl.GL_LIGHTING);
         
         //gl.glEnable(gl.GL_LIGHTING);
         
         mTexture.bind(gl);

         /** Standard 6ft 4ft 6ft Fixed Frame
          * MCC
          * MCF1,0.0000,1.2192,0.0000,0.00,0.00,1.00,215
          * MCF2,-0.9144,1.2192,0.0000,0.00,0.00,1.00,216
          * MCF3,-1.8288,1.2192,0.0000,0.00,0.00,1.00,217
          *
          * MCF4,0.0000,0.0000,0.0000,0.00,0.00,1.00,209
          * MCF5,-0.9144,0.0000,0.0000,0.00,0.00,1.00,210
          * MCF6,-1.8288,0.0000,0.0000,0.00,0.00,1.00,211
          *
          * MCF7,-0.3048,0.6096,0.0000,0.00,0.00,1.00,527
          * MCF8,-0.9144,0.6096,0.0000,0.00,0.00,1.00,528
          * MCF9,-1.5240,0.6096,0.0000,0.00,0.00,1.00,529
          * MCe
          */
         drawBox(gl, new Vec3f(-1.8288f,1.2192f-0.03f,-0.03f), new Vec3f(-0.0f,1.2192f+0.03f,0.03f), gl.GL_QUADS);
         drawBox(gl, new Vec3f(-1.8288f,-0.03f,-0.03f), new Vec3f(-0.0f,0.03f,0.03f), gl.GL_QUADS);
         drawBox(gl, new Vec3f(-1.5240f,0.6096f-0.03f,-0.03f), new Vec3f(-0.3048f,0.6096f+0.03f,0.03f), gl.GL_QUADS);
         mTexture.unbind(gl);
         if (lightingOn)
            gl.glDisable(GL.GL_COLOR_MATERIAL);
      gl.glPopMatrix();
   }

   private static final float[][] boxNormals = {
      {-1.0f, 0.0f, 0.0f},
      {0.0f, 1.0f, 0.0f},
      {1.0f, 0.0f, 0.0f},
      {0.0f, -1.0f, 0.0f},
      {0.0f, 0.0f, 1.0f},
      {0.0f, 0.0f, -1.0f}
   };
   private static final int[][] boxFaces = {
      {0, 1, 2, 3},
      {3, 2, 6, 7},
      {7, 6, 5, 4},
      {4, 5, 1, 0},
      {5, 6, 2, 1},
      {7, 4, 0, 3}
   };
   
   private float[][] boxVertices = null;
   private void drawBox(GL gl, Vec3f start, Vec3f end, int type)
   {
      float[][] v = new float[8][];
      for (int i = 0; i < 8; i++)
      {
         v[i] = new float[3];
      }
      v[0][0] = v[1][0] = v[2][0] = v[3][0] = start.x();
      v[4][0] = v[5][0] = v[6][0] = v[7][0] = end.x();
      v[0][1] = v[1][1] = v[4][1] = v[5][1] = start.y();
      v[2][1] = v[3][1] = v[6][1] = v[7][1] = end.y();
      v[0][2] = v[3][2] = v[4][2] = v[7][2] = start.z();
      v[1][2] = v[2][2] = v[5][2] = v[6][2] = end.z();
      
      float[][] n = boxNormals;
      int[][] faces = boxFaces;
      for (int i = 5; i >= 0; i--)
      {
         gl.glBegin(type);
         gl.glNormal3fv(n[i]);
         gl.glVertex3fv(v[faces[i][0]]);
         gl.glVertex3fv(v[faces[i][1]]);
         gl.glVertex3fv(v[faces[i][2]]);
         gl.glVertex3fv(v[faces[i][3]]);
         gl.glEnd();
      }
   }
}
