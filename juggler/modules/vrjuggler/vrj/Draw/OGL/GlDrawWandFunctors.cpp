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

#include <vrj/vrjConfig.h>

#include <boost/concept_check.hpp>

#include <vrj/Draw/OGL/GlDrawWandFunctors.h>

#include <gmtl/Vec.h>
#include <gmtl/Output.h>
#include <gmtl/VecOps.h>
#include <vector>

namespace vrj
{

GlDrawConeWandFunctor::GlDrawConeWandFunctor()
   : mQuadObj(gluNewQuadric())
{
}

void GlDrawConeWandFunctor::draw(vrj::User* user)
{
   boost::ignore_unused_variable_warning(user);

   const float base = 0.2f;
   const float height = 0.6f;
   const int slices = 6;
   const int stacks = 1;

   glColor3f(0.0f, 1.0f, 0.0f);
   gluQuadricDrawStyle(mQuadObj, (GLenum) GLU_FILL);
   gluQuadricNormals(mQuadObj, (GLenum) GLU_SMOOTH);
   gluCylinder(mQuadObj, base, 0.0, height, slices, stacks);
}

void GlDrawRightAngleWandFunctor::draw(vrj::User* user)
{
   boost::ignore_unused_variable_warning(user);

   static GLfloat VertexData[] = {
      0.0140000f, -0.140000f, 0.0140000f, 0.0140000f, 0.028000f, 0.0140000f, -0.0140000f, 0.028000f, 0.0140000f,
      -0.0140000f, -0.140000f, 0.0140000f, 0.0140000f, -0.140000f, -0.0140000f,
      0.0140000f, 0.00000f, -0.0140000f, 0.0140000f, 0.028000f, 0.0140000f,
      0.0140000f, -0.140000f, 0.0140000f, -0.0140000f, -0.140000f, -0.0140000f,
      -0.0140000f, 0.00000f, -0.0140000f, 0.0140000f, 0.00000f, -0.0140000f,
      0.0140000f, -0.140000f, -0.0140000f, -0.0140000f, -0.140000f, 0.0140000f,
      -0.0140000f, 0.028000f, 0.0140000f, -0.0140000f, 0.00000f, -0.0140000f,
      -0.0140000f, -0.140000f, -0.0140000f, -0.0140000f, -0.140000f, -0.0140000f,
      0.0140000f, -0.140000f, -0.0140000f, 0.0140000f, -0.140000f, 0.0140000f,
      -0.0140000f, -0.140000f, 0.0140000f, -0.0140000f, 0.028000f, 0.0140000f,
      0.0140000f, 0.028000f, 0.0140000f, 0.028000f, 0.056000f, -0.042000f,
      -0.028000f, 0.056000f, -0.042000f, 0.028000f, 0.042000f, -0.042000f,
      -0.028000f, 0.042000f, -0.042000f, -0.028000f, 0.056000f, -0.042000f,
      0.028000f, 0.056000f, -0.042000f, 0.0140000f, 0.028000f, 0.0140000f,
      0.0140000f, 0.00000f, -0.0140000f, 0.028000f, 0.042000f, -0.042000f,
      0.0140000f, 0.028000f, 0.0140000f, 0.028000f, 0.042000f, -0.042000f,
      0.028000f, 0.056000f, -0.042000f, -0.0140000f, 0.028000f, 0.0140000f,
      -0.028000f, 0.056000f, -0.042000f, -0.028000f, 0.042000f, -0.042000f,
      -0.028000f, 0.042000f, -0.042000f, -0.0140000f, 0.00000f, -0.0140000f,
      -0.0140000f, 0.028000f, 0.0140000f, 0.028000f, 0.042000f, -0.042000f,
      0.0140000f, 0.00000f, -0.0140000f, -0.0140000f, 0.00000f, -0.0140000f,
      -0.028000f, 0.042000f, -0.042000f
   };
   static GLuint Indices[] = {
      0, 1, 2,
      0, 2, 3,
      4, 5, 6,
      4, 6, 7,
      8, 9, 10,
      8, 10, 11,
      12, 13, 14,
      12, 14, 15,
      16, 17, 18,
      16, 18, 19,
      20, 21, 22,
      20, 22, 23,
      24, 25, 26,
      24, 26, 27,
      28, 29, 30,
      31, 32, 33,
      34, 35, 36,
      37, 38, 39,
      40, 41, 42,
      40, 42, 43
   };

   static GLfloat NormData[] = {
      0.0f, -0.0f, 0.004704f,
      0.0f, 0.0f, 0.004704f,
      0.00392f, 0.0f, 0.0f,
      0.004704f, 0.0f, 0.0f,
      0.0f, 0.0f, -0.00392f,
      0.0f, 0.0f, -0.00392f,
      0.004704f, 0.0f, 0.0f,
      0.00392f, 0.0f, 0.0f,
      0.0f, -0.000784f, 0.0f,
      0.0f, -0.000784f, 0.0f,
      0.0f, 0.001568f, 0.000784f,
      3.16352e-11f, 0.003136f, 0.001568f,
      0.0f, 0.0f, -0.000784f,
      0.0f, 0.0f, -0.000784f,
      0.00196f, -0.000392f, 0.000392f,
      0.000784f, 1.58176e-11f, 0.000196f,
      0.000784f, -1.58176e-11f, 0.000196f,
      0.00196f, -0.000392f, 0.000392f,
      4.84511e-11f, -0.000784f, -0.001176f,
      0.0f, -0.001568f, -0.002352f
   };

   unsigned num_tris(20);

#if 0
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

   glColor3f(0.00000f, 1.00000f, 0.00000f);

   glBegin(GL_TRIANGLES);
   for(unsigned i=0;i<20;i++)
   {
      glNormal3fv(&(NormData[i*3]));
      glVertex3fv(&(VertexData[3*Indices[(i*3)+0]]));
      glVertex3fv(&(VertexData[3*Indices[(i*3)+1]]));
      glVertex3fv(&(VertexData[3*Indices[(i*3)+2]]));
   }
   glEnd();
}

}
