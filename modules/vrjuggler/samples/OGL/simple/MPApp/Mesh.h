#ifndef _MESH_H_
#define _MESH_H_

#include <iostream>
#include <vector>
#include <GL/gl.h>
#include <Math/vjVec3.h>
#include <vpr/System.h>

// Simple class to calculate a height-field type mesh
//
//
// Desc:
//    This is a mesh of triangles based on a height grid
//  The grid is of unit size (1.0f x 1.0f) with a variable height
//  on the z-axis.
//
// How to use:
//
//
//
//
//-------------------------------------------
class Mesh
{
public:
   //: Constructor
   Mesh()
   {
      setDimensions(50,50);   // Set initial dimensions
   }

   //: Render the mesh in OpenGL
   // Uses triangle strips to render the mesh
   //!PRE: compute() must have been called to update
   //      the mesh data for the given time
   inline void render();

   //: Recompute the mesh state
   //! ARGS: curTime - time variable for the mesh update
   //                   (No specific units)
   //! POST: The mesh vertices and normals are updated
   //          based on the time value and the current dimensions
   inline void compute(float curTime);

   //: Set the size of the grid
   // The size is reflected the next time that compute is called
   void setDimensions(int xDim, int yDim)
   {
      mXDim = xDim;
      mYDim = yDim;
   }

protected:
   // Get the height value for the given x and y coords
   inline float getHeight(float x, float y, float curTime);

private:
   int mXDim, mYDim;

   std::vector<std::vector<vjVec3> > mVerts;
   std::vector<std::vector<vjVec3> > mNormals;
};


void Mesh::render()
{

   for(int j=1;j<mYDim;j++)
   {
      glBegin(GL_TRIANGLE_STRIP);

      for(int i=0;i<mXDim;i++)
      {
         glNormal3fv(mNormals[i][j].vec);
         glVertex3fv(mVerts[i][j].vec);
         glNormal3fv(mNormals[i][j-1].vec);
         glVertex3fv(mVerts[i][j-1].vec);
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
         mVerts[i][j] = vjVec3(x,y,getHeight(x,y,curTime));
      }
   }

   // --- Compute the normals --- //
   vjVec3 total_normal;     // Total of all normals to average
   float  num_normals;        // Number of normals contributing
   vjVec3 v0,v1,temp_normal;
   vjVec3 local_vert;

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
            temp_normal = v0.cross(v1);
            total_normal += temp_normal;
            num_normals++;
         }
         // UPPER-RIGHT
         if((i<(mXDim-1))&&(j<(mYDim-1)))
         {
            v0 = mVerts[i+1][j] - local_vert;
            v1 = mVerts[i][j+1] - local_vert;
            temp_normal = v0.cross(v1);
            total_normal += temp_normal;
            num_normals++;
         }
         // LOWER-LEFT
         if((i>0)&&(j>0))
         {
            v0 = mVerts[i-1][j] - local_vert;
            v1 = mVerts[i][j-1] - local_vert;
            temp_normal = v0.cross(v1);
            total_normal += temp_normal;
            num_normals++;
         }
         // LOWER-RIGHT
         if((i<(mXDim-1))&&(j>0))
         {
            v0 = mVerts[i][j-1] - local_vert;
            v1 = mVerts[i+1][j] - local_vert;
            temp_normal = v0.cross(v1);
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
   const float amp2(0.015);
   const float var2 = y+curTime;

   const float phase3(20.0f);
   const float offset3(0.7f);
   const float amp3(0.010f);
   const float var3 = x+(2.0f*y)+curTime;

   float ret_val;

   ret_val = amp1*vpr::System::sin((phase1*var1)+offset1) +
             amp2*vpr::System::sin((phase2*var2)+offset2) +
             amp3*vpr::System::sin((phase3*var3)+offset3);

   return ret_val;
}

#endif

