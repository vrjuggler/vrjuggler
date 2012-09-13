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

#include <vrj/Draw/OpenGL/Config.h>

#include <vector>
#include <gmtl/Vec.h>
#include <gmtl/Output.h>
#include <gmtl/VecOps.h>

#include <vrj/Draw/OpenGL/DrawWandFunctors.h>

#if defined(VPR_OS_Darwin) && defined(VRJ_USE_COCOA)
#  include <OpenGL/glu.h>
#else
#  include <GL/glu.h>
#endif

namespace vrj
{

namespace opengl
{

DrawConeWandFunctor::DrawConeWandFunctor()
   : mQuadObj((void *) gluNewQuadric())
{
   /* Do nothing. */ ;
}

DrawConeWandFunctor::~DrawConeWandFunctor()
{
   if ( NULL != mQuadObj )
   {
      gluDeleteQuadric((GLUquadricObj *) mQuadObj);
      mQuadObj = NULL;
   }
}

void DrawConeWandFunctor::draw(vrj::UserPtr)
{
   const float base = 0.2f;
   const float height = 0.6f;
   const int slices = 6;
   const int stacks = 1;

   glColor3f(0.0f, 1.0f, 0.0f);
   gluQuadricDrawStyle((GLUquadricObj *) mQuadObj, (GLenum) GLU_FILL);
   gluQuadricNormals((GLUquadricObj *) mQuadObj, (GLenum) GLU_SMOOTH);
   gluCylinder((GLUquadricObj *) mQuadObj, base, 0.0, height, slices, stacks);
}

void DrawRightAngleWandFunctor::draw(vrj::UserPtr)
{
   GLfloat const VertexData[] = {
       0.014f, -0.140f,  0.014f,
       0.014f,  0.028f,  0.014f,
      -0.014f,  0.028f,  0.014f,
      -0.014f, -0.140f,  0.014f,
       0.014f, -0.140f, -0.014f,
       0.014f,  0.000f, -0.014f,
      -0.014f, -0.140f, -0.014f,
      -0.014f,  0.000f, -0.014f,
       0.028f,  0.056f, -0.042f,
      -0.028f,  0.056f, -0.042f,
       0.028f,  0.042f, -0.042f,
      -0.028f,  0.042f, -0.042f,
   };

   GLfloat const FaceNormData[] = {
      0.000f,  0.000f,  1.000f,
      0.000f,  0.000f,  1.000f,
      1.000f,  0.000f,  0.000f,
      1.000f,  0.000f,  0.000f,
      0.000f,  0.000f, -1.000f,
      0.000f,  0.000f, -1.000f,
     -1.000f,  0.000f,  0.000f,
     -1.000f,  0.000f,  0.000f,
      0.000f, -1.000f,  0.000f,
      0.000f, -1.000f,  0.000f,
      0.000f,  0.894f,  0.447f,
      0.000f,  0.894f,  0.447f,
      0.000f,  0.000f, -1.000f,
      0.000f,  0.000f, -1.000f,
      0.816f, -0.408f,  0.408f,
      0.970f,  0.000f,  0.243f,
     -0.970f,  0.000f,  0.243f,
     -0.816f, -0.408f,  0.408f,
      0.000f, -0.554f, -0.831f,
      0.000f, -0.554f, -0.831f
   };

   GLushort const Indices[] = {
       0,  1,  2,
       0,  2,  3,
       4,  5,  1,
       4,  1,  0,
       6,  7,  5,
       6,  5,  4,
       3,  2,  7,
       3,  7,  6,
       6,  4,  0,
       6,  0,  3,
       2,  1,  8,
       2,  8,  9,
      10, 11,  9,
      10,  9,  8,
       1,  5, 10,
       1, 10,  8,
       2,  9, 11,
      11,  7,  2,
      10,  5,  7,
      10,  7, 11
   };

#if 0
   unsigned num_tris(20);

   // USED to generate normals
   static bool first_time(true);

   if(first_time)
   {
      first_time = false;

      unsigned j;       // vertex offset
      std::vector<gmtl::Vec3f> norms;

      // For each triangle
      for(unsigned i=0;i<num_tris;i++)
      {
         j = 3*Indices[(i*3)+0];
         gmtl::Vec3f v1(VertexData[j], VertexData[j+1], VertexData[j+2]);
         j = 3*Indices[(i*3)+1];
         gmtl::Vec3f v2(VertexData[j], VertexData[j+1], VertexData[j+2]);
         j = 3*Indices[(i*3)+2];
         gmtl::Vec3f v3(VertexData[j], VertexData[j+1], VertexData[j+2]);
         gmtl::Vec3f normal;
         normal = gmtl::cross(normal, gmtl::Vec3f(v2-v1), gmtl::Vec3f(v3-v1));
         norms.push_back(normal);
         std::cout << i << ": " << normal << std::endl;
      }
   }

   glPushAttrib(GL_LIGHTING_BIT);
   glDisable(GL_LIGHTING);
   glColor3f(1.0,0.0f,0.0f);
   glBegin(GL_LINES);
      for(unsigned i=0;i<20;i++)
      {
         gmtl::Vec3f norm;    norm.set(&(NormData[(i*3)]));
         //norm = norms[i];
         gmtl::Vec3f v0;      v0.set( &(VertexData[3*Indices[(i*3)+0]]));
         gmtl::Vec3f v1 = v0+norm;
         glVertex3fv(v0.getData());
         glVertex3fv(v1.getData());
      }
   glEnd();
   glPopAttrib();
#endif

   glColor3f(0.00f, 1.00f, 0.00f);

   glBegin(GL_TRIANGLES);
   for(unsigned i=0;i<20;i++)
   {
      glNormal3fv(&(FaceNormData[i*3]));
      glVertex3fv(&(VertexData[3*Indices[(i*3)+0]]));
      glVertex3fv(&(VertexData[3*Indices[(i*3)+1]]));
      glVertex3fv(&(VertexData[3*Indices[(i*3)+2]]));
   }
   glEnd();
}

} // End of opengl namespace

} // End of vrj namespace