/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2010 by Iowa State University
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

#ifndef _MESH_H_
#define _MESH_H_

#include <iostream>
#include <vector>

#if defined(__APPLE__)
#  include <OpenGL/gl.h>
#else
#  include <GL/gl.h>
#endif

#include <gmtl/Vec.h>
#include <gmtl/VecOps.h>
#include <vpr/System.h>

using namespace gmtl;
using namespace vrj;

/**
 * Simple class to calculate a height-field type mesh.
 * This is a mesh of triangles based on a height grid.  The grid is of unit
 * size (1.0f x 1.0f) with a variable height on the z-axis.
 */
class Mesh
{
public:
   /// Constructor
   Mesh()
   {
      setDimensions(50,50);   // Set initial dimensions
   }

   /**
    * Renders the mesh in OpenGL.  Uses triangle strips to render the mesh.
    *
    * @pre compute() must have been called to update the mesh data for the
    * given time.
    */
   inline void render();

   /**
    * Recomputes the mesh state.
    *
    * @post The mesh vertices and normals are updated based on the time value
    *       and the current dimensions.
    *
    * @param curTime time variable for the mesh update (no specific units).
    */
   inline void compute(float curTime);

   /**
    * Sets the size of the grid.  The size is reflected the next time that
    * compute is called.
    */
   void setDimensions(int xDim, int yDim)
   {
      mXDim = xDim;
      mYDim = yDim;
   }

protected:
   /// Gets the height value for the given x and y coords
   inline float getHeight(float x, float y, float curTime);

private:
   int mXDim, mYDim;

   std::vector<std::vector<Vec3f> > mVerts;
   std::vector<std::vector<Vec3f> > mNormals;
};


void Mesh::render()
{

   for(int j=1;j<mYDim;j++)
   {
      glBegin(GL_TRIANGLE_STRIP);

      for(int i=0;i<mXDim;i++)
      {
         glNormal3fv(mNormals[i][j].mData);
         glVertex3fv(mVerts[i][j].mData);
         glNormal3fv(mNormals[i][j-1].mData);
         glVertex3fv(mVerts[i][j-1].mData);
      }

      glEnd();
   }

}

void Mesh::compute(float curTime)
{

   // NOTE: This implemenation is meant to be clean and understandable
   //       it does NOT have optimal performance

   int i,j;             // Loop index variables
   float x,y;           // Used to store current x and y coords we are working on

   // --- Verify mesh is large enough --- //
   mVerts.resize(mXDim);
   mNormals.resize(mXDim);
   for(i=0;i<mXDim;i++)
   {
      mVerts[i].resize(mYDim);
      mNormals[i].resize(mYDim);
   }

   // Update height values of the grid
   for(i=0;i<mXDim;i++)
   {
      x = (1.0f/float(mXDim))*float(i);     // Compute current x coord
      for(j=0;j<mYDim;j++)
      {
         y = (1.0f/float(mYDim))*float(j);
         mVerts[i][j] = Vec3f(x,y,getHeight(x,y,curTime));
      }
   }

   // --- Compute the normals --- //
   Vec3f total_normal;     // Total of all normals to average
   float  num_normals;        // Number of normals contributing
   Vec3f v0,v1,temp_normal;
   Vec3f local_vert;

   for(i=0;i<mXDim;i++)
   {
      for(j=0;j<mYDim;j++)
      {
         total_normal.set(0.0,0.0,0.0);
         num_normals = 0;
         local_vert = mVerts[i][j];

         // UPPER-LEFT
         if((i>0)&&(j<(mYDim-1)))
         {
            v0 = mVerts[i][j+1] - local_vert;
            v1 = mVerts[i-1][j] - local_vert;
            cross(temp_normal, v0, v1);
            total_normal += temp_normal;
            num_normals++;
         }
         // UPPER-RIGHT
         if((i<(mXDim-1))&&(j<(mYDim-1)))
         {
            v0 = mVerts[i+1][j] - local_vert;
            v1 = mVerts[i][j+1] - local_vert;
            cross(temp_normal, v0, v1);
            total_normal += temp_normal;
            num_normals++;
         }
         // LOWER-LEFT
         if((i>0)&&(j>0))
         {
            v0 = mVerts[i-1][j] - local_vert;
            v1 = mVerts[i][j-1] - local_vert;
            cross(temp_normal, v0, v1);
            total_normal += temp_normal;
            num_normals++;
         }
         // LOWER-RIGHT
         if((i<(mXDim-1))&&(j>0))
         {
            v0 = mVerts[i][j-1] - local_vert;
            v1 = mVerts[i+1][j] - local_vert;
            cross(temp_normal, v0, v1);
            total_normal += temp_normal;
            num_normals++;
         }

         mNormals[i][j] = (total_normal/num_normals);
      }
   }
}

// Get the height value for the given x and y coords
float Mesh::getHeight(float x, float y, float curTime)
{
   const float phase1(20.5f);
   const float offset1(1.2f);
   const float amp1(0.025f);
   const float var1 = x+curTime;

   const float phase2(20.6f);
   const float offset2(0.3f);
   const float amp2(0.015f);
   const float var2 = y+curTime;

   const float phase3(20.0f);
   const float offset3(0.7f);
   const float amp3(0.010f);
   const float var3 = x+(2.0f*y)+curTime;

   float ret_val;

   ret_val = amp1*Math::sin((phase1*var1)+offset1) +
             amp2*Math::sin((phase2*var2)+offset2) +
             amp3*Math::sin((phase3*var3)+offset3);

   return ret_val;
}

#endif

