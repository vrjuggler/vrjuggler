/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2008 by Iowa State University
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

#include <vrj/vrjConfig.h>

#include <iostream>
#include <math.h>

#if defined(__APPLE__)
#  include <OpenGL/gl.h>
#  include <OpenGL/glu.h>
#else
#  include <GL/gl.h>
#  include <GL/glu.h>
#endif

#include <gmtl/Math.h>
#include <gmtl/Vec.h>
#include <gmtl/Matrix.h>
#include <gmtl/MatrixOps.h>
#include <gmtl/Generate.h>
#include <gmtl/Xforms.h>


#include <torusApp.h>

namespace vrjTest
{

static void doughnut(GLfloat r, GLfloat R, GLint nsides, GLint rings);

void torusApp::bufferPreDraw()
{
   glClearColor(0.0, 0.0, 0.0, 0.0);
   glClear(GL_COLOR_BUFFER_BIT);
}

// Draw the scene.  A bunch of boxes of differing color and stuff
void torusApp::draw()
{
   glClear(GL_DEPTH_BUFFER_BIT);

   //vrj::Coord wand_pos(*mWand->getData());
   gmtl::Matrix44f wand_pos(mWand->getData());
   gmtl::Vec4f wand_trans( gmtl::makeTrans<gmtl::Vec4f>(wand_pos) );

   // Set light position
   //GLfloat light0_position[] = { 6.0f, 6,0f, 6.0f, 1.0f };
   //glLightfv(GL_LIGHT0, GL_POSITION, light0_position);
   glLightfv(GL_LIGHT0, GL_POSITION, wand_trans.mData);

   // Set material color
   float onyx_red  = 59.0f / 255.0f;
   float onyx_blue = 57.0f / 255.0f;
   GLfloat mat_ambient[]   = { onyx_red, 0.0f, onyx_blue, 1.0f };
   GLfloat mat_diffuse[]   = { onyx_red, 0.0f, onyx_blue, 1.0f };
   GLfloat mat_specular[]  = { 1.0f, 1.0f, 1.0f, 1.0f };
   GLfloat mat_shininess[] = { 50.0f };

   glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
   glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
   glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
   glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

   glPushMatrix();

      glTranslatef(-6.0f, 0.0f, -6.0f);
      glRotatef(30, 0, 1, 0);         // Around Y
      glRotatef(-30, 1, 0, 0);        // Around X

      glRotatef(mTorusRotation, 1, 0, 0);        // Around X

      //doughnut(1.5f, 9.0f, 300, 300);
      //doughnut(1.5f, 6.0f, 120, 120);
      doughnut(1.5f, 3.0f, 30, 30);
      //doughnut(0.5f,1.0f,100,100);
   glPopMatrix();
//   vpr::System::msleep(2000);
}

void torusApp::initGLState()
{
   GLfloat light0_ambient[]  = { 0.2f, 0.2f, 0.2f, 1.0f };
   GLfloat light0_diffuse[]  = { 1.0f, 1.0f, 1.0f, 1.0f };
   GLfloat light0_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
   GLfloat light0_position[] = { 6.0f, 6.0f, 6.0f, 1.0f };

   glLightfv(GL_LIGHT0, GL_AMBIENT, light0_ambient);
   glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);
   glLightfv(GL_LIGHT0, GL_SPECULAR, light0_specular);
   glLightfv(GL_LIGHT0, GL_POSITION, light0_position);

   glEnable(GL_DEPTH_TEST);
   glEnable(GL_NORMALIZE);
   glEnable(GL_LIGHTING);
   glEnable(GL_LIGHT0);
   glDisable(GL_COLOR_MATERIAL);
   glShadeModel(GL_SMOOTH);

   glMatrixMode(GL_MODELVIEW);
}

// Draw a doughnut.
// Borrowed from GLUT
static void doughnut(GLfloat r, GLfloat R, GLint nsides, GLint rings)
{
   GLfloat theta, phi, theta1;
   GLfloat cosTheta, sinTheta;
   GLfloat cosTheta1, sinTheta1;
   GLfloat ringDelta, sideDelta;

   ringDelta = 2.0 * gmtl::Math::PI / rings;
   sideDelta = 2.0 * gmtl::Math::PI / nsides;

   theta    = 0.0f;
   cosTheta = 1.0f;
   sinTheta = 0.0f;

   for ( int i = rings - 1; i >= 0; --i )
   {
      theta1 = theta + ringDelta;
      cosTheta1 = cos(theta1);
      sinTheta1 = sin(theta1);
      glBegin(GL_QUAD_STRIP);
      phi = 0.0;
      for ( int j = nsides; j >= 0; --j )
      {
         GLfloat cosPhi, sinPhi, dist;

         phi += sideDelta;
         cosPhi = cos(phi);
         sinPhi = sin(phi);
         dist = R + r * cosPhi;

         glNormal3f(cosTheta1 * cosPhi, -sinTheta1 * cosPhi, sinPhi);
         glVertex3f(cosTheta1 * dist, -sinTheta1 * dist, r * sinPhi);
         glNormal3f(cosTheta * cosPhi, -sinTheta * cosPhi, sinPhi);
         glVertex3f(cosTheta * dist, -sinTheta * dist,  r * sinPhi);
      }
      glEnd();
      theta = theta1;
      cosTheta = cosTheta1;
      sinTheta = sinTheta1;
   }
}

} // namespace vrjTest
