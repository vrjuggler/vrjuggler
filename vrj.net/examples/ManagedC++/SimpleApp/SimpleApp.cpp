// VRJ.NET is (C) Copyright 2004 by Patrick Hartling
// Distributed under the GNU Lesser General Public License 2.1.  (See
// accompanying file COPYING.txt or http://www.gnu.org/copyleft/lesser.txt)

// File:          $RCSfile$
// Date modified: $Date$
// Version:       $Revision$

#using <vprnet.dll>
#using <gmtlnet.dll>

#include "SimpleApp.h"

using namespace Tao::OpenGl;


#ifdef WIN32
#pragma component(mintypeinfo, on)
#endif

void SimpleApp::init()
{
   mButton0->init(S"VJButton0");
   mButton1->init(S"VJButton1");
   mWand->init(S"VJWand");
}

void SimpleApp::preFrame()
{
   if ( mButton0->getProxy()->getData() != gadget::Digital::State::OFF )
   {
      mBoxGrabbed = true;
   }
   else
   {
      mBoxGrabbed = false;
   }

   if ( mButton1->getProxy()->getData() )
   {
      System::Console::WriteLine(S"Button 1 pressed");
   }
}

void SimpleApp::bufferPreDraw()
{
   Gl::glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
   Gl::glClear(Gl::GL_COLOR_BUFFER_BIT);
}

void SimpleApp::draw()
{
   Gl::glClear(Gl::GL_DEPTH_BUFFER_BIT);

   // --- Setup for drawing --- //
   Gl::glMatrixMode(Gl::GL_MODELVIEW);

   gmtl::Matrix44f* box_offset;

   // If the box is grabbed, position it in the same place as the wand.
   if ( mBoxGrabbed )
   {
      // -- Get Wand matrix --- //
      box_offset = mWand->getProxy()->getData();
   }
   // Otherwise, use a fixed position.
   else
   {
      // -- Create box offset matrix -- //
      gmtl::EulerAngleXYZf* euler_ang =
         new gmtl::EulerAngleXYZf(gmtl::Math::deg2Rad(-90.0f), gmtl::Math::deg2Rad(0.0f),
                                 gmtl::Math::deg2Rad(0.0f));
      box_offset = gmtl::Gmtl::makeRotMatrix44(euler_ang);
      gmtl::Gmtl::setTrans(box_offset, new gmtl::Vec3f(0.0f, 1.0f, 0.0f));
   }

   // --- Create a color for the box --- //
   float box_color[3] = { 0.7f, 0.7f, 0.7f };

   Gl::glPushMatrix();

      // --- Draw the box --- //
      Gl::glPushMatrix();
         Gl::glMultMatrixf(box_offset->getData());    // Push the wand offset matrix on the stack
         Gl::glColor3fv(box_color);
         Gl::glScalef(0.5f, 0.5f, 0.5f);
         drawCube();
      Gl::glPopMatrix();

      // Draw a coordinate axis "on" the box
      Gl::glLineWidth(2.0f);
      Gl::glDisable(Gl::GL_LIGHTING);
      Gl::glPushMatrix();
         Gl::glMultMatrixf(box_offset->getData());    // Go to box position

         gmtl::Vec3f* x_axis = new gmtl::Vec3f(7.0f,0.0f,0.0f);
         gmtl::Vec3f* y_axis = new gmtl::Vec3f(0.0f, 7.0f, 0.0f);
         gmtl::Vec3f* z_axis = new gmtl::Vec3f(0.0f, 0.0f, 7.0f);
         gmtl::Vec3f* origin = new gmtl::Vec3f(0.0f, 0.0f, 0.0f);

         Gl::glBegin(Gl::GL_LINES);
            Gl::glColor3f(1.0f, 0.0f, 0.0f);
            Gl::glVertex3fv(origin->getData());
            Gl::glVertex3fv(x_axis->getData());

            Gl::glColor3f(0.0f, 1.0f, 0.0f);
            Gl::glVertex3fv(origin->getData());
            Gl::glVertex3fv(y_axis->getData());

            Gl::glColor3f(0.0f, 0.0f, 1.0f);
            Gl::glVertex3fv(origin->getData());
            Gl::glVertex3fv(z_axis->getData());
         Gl::glEnd();
      Gl::glPopMatrix();
      Gl::glEnable(Gl::GL_LIGHTING);
   Gl::glPopMatrix();
}

void SimpleApp::initGLState()
{
   float light0_ambient[]  = { 0.1f,  0.1f,  0.1f, 1.0f };
   float light0_diffuse[]  = { 0.8f,  0.8f,  0.8f, 1.0f };
   float light0_specular[] = { 1.0f,  1.0f,  1.0f, 1.0f };
   float light0_position[] = { 0.0f, 0.75f, 0.75f, 0.0f };

   float mat_ambient[]   = { 0.7f, 0.7f, 0.7f, 1.0f };
   float mat_diffuse[]   = { 1.0f, 0.5f, 0.8f, 1.0f };
   float mat_specular[]  = { 1.0f, 1.0f, 1.0f, 1.0f };
   float mat_shininess[] = { 50.0f };
   float mat_emission[]  = { 1.0f, 1.0f, 1.0f, 1.0f };
   float no_mat[]        = { 0.0f, 0.0f, 0.0f, 1.0f };

   Gl::glLightfv(Gl::GL_LIGHT0, Gl::GL_AMBIENT, light0_ambient);
   Gl::glLightfv(Gl::GL_LIGHT0, Gl::GL_DIFFUSE, light0_diffuse);
   Gl::glLightfv(Gl::GL_LIGHT0, Gl::GL_SPECULAR, light0_specular);
   Gl::glLightfv(Gl::GL_LIGHT0, Gl::GL_POSITION, light0_position);

   Gl::glMaterialfv(Gl::GL_FRONT, Gl::GL_AMBIENT, mat_ambient);
   Gl::glMaterialfv(Gl::GL_FRONT, Gl::GL_DIFFUSE, mat_diffuse);
   Gl::glMaterialfv(Gl::GL_FRONT, Gl::GL_SPECULAR, mat_specular);
   Gl::glMaterialfv(Gl::GL_FRONT, Gl::GL_SHININESS, mat_shininess);
   Gl::glMaterialfv(Gl::GL_FRONT, Gl::GL_EMISSION, no_mat);

   Gl::glEnable(Gl::GL_DEPTH_TEST);
   Gl::glEnable(Gl::GL_NORMALIZE);
   Gl::glEnable(Gl::GL_LIGHTING);
   Gl::glEnable(Gl::GL_LIGHT0);
   Gl::glEnable(Gl::GL_COLOR_MATERIAL);
   Gl::glShadeModel(Gl::GL_SMOOTH);
}

/// Utility function for drawing a cube
void SimpleApp::drawbox(double x0, double x1, double y0, double y1,
                        double z0, double z1, int type)
{
   static double n[6][3] = {
      {-1.0, 0.0, 0.0}, {0.0, 1.0, 0.0}, {1.0, 0.0, 0.0},
      {0.0, -1.0, 0.0}, {0.0, 0.0, 1.0}, {0.0, 0.0, -1.0}
   };
   static int faces[6][4] = {
      { 0, 1, 2, 3}, { 3, 2, 6, 7}, { 7, 6, 5, 4},
      { 4, 5, 1, 0}, { 5, 6, 2, 1}, { 7, 4, 0, 3}
   };
   double v[8][3], tmp;
   int i;

   if (x0 > x1)
   {
      tmp = x0; x0 = x1; x1 = tmp;
   }
   if (y0 > y1)
   {
      tmp = y0; y0 = y1; y1 = tmp;
   }
   if (z0 > z1)
   {
      tmp = z0; z0 = z1; z1 = tmp;
   }
   v[0][0] = v[1][0] = v[2][0] = v[3][0] = x0;
   v[4][0] = v[5][0] = v[6][0] = v[7][0] = x1;
   v[0][1] = v[1][1] = v[4][1] = v[5][1] = y0;
   v[2][1] = v[3][1] = v[6][1] = v[7][1] = y1;
   v[0][2] = v[3][2] = v[4][2] = v[7][2] = z0;
   v[1][2] = v[2][2] = v[5][2] = v[6][2] = z1;

   for (i = 0; i < 6; i++)
   {
      Gl::glBegin(type);
         Gl::glNormal3dv(&n[i][0]);
         Gl::glVertex3dv(&v[faces[i][0]][0]);
         Gl::glNormal3dv(&n[i][0]);
         Gl::glVertex3dv(&v[faces[i][1]][0]);
         Gl::glNormal3dv(&n[i][0]);
         Gl::glVertex3dv(&v[faces[i][2]][0]);
         Gl::glNormal3dv(&n[i][0]);
         Gl::glVertex3dv(&v[faces[i][3]][0]);
      Gl::glEnd();
   }
}

#ifdef WIN32
#pragma component(mintypeinfo, off)
#endif
