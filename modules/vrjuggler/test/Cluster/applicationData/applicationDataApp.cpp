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

#if defined(WIN32) || defined(WIN64)
#  include <windows.h>
#endif

#include <iostream>

#if defined(__APPLE__)
#  include <OpenGL/gl.h>
#  include <OpenGL/glu.h>
#else
#  include <GL/gl.h>
#  include <GL/glu.h>
#endif

#include <gmtl/Generate.h>
#include <gadget/Node.h>
#include <cluster/ClusterManager.h>
#include <cluster/ClusterNetwork.h>

#include "applicationDataApp.h"


namespace vrjTest
{

// Utility function to draw a cube
void drawbox(GLdouble x0, GLdouble x1, GLdouble y0, GLdouble y1,
             GLdouble z0, GLdouble z1, GLenum type);


applicationDataApp::applicationDataApp(vrj::Kernel* kernel)
   : vrj::opengl::App(kernel) 
{}

applicationDataApp::~applicationDataApp()
{}

void applicationDataApp::init()
{
   //std::cout << "---------- App:init() ---------------" << std::endl;
   // Initialize devices
   mWand.init("VJWand");
   mHead.init("VJHead");
   mButton0.init("VJButton0");
   mButton1.init("VJButton1");
   mButton2.init("VJButton2");
   mButton3.init("VJButton3");
   mButton4.init("VJButton4");
   mButton5.init("VJButton5");
   
   vpr::GUID new_guid("d6be4359-e8cf-41fc-a72b-a5b4f3f29aa2");
   mMyData.init(new_guid);
}

void applicationDataApp::contextInit()
{
   // Initialize the GL state information. (lights, shading, etc)
   initGLState();
}

void applicationDataApp::preFrame()
{
/*    std::cout  << "Wand Buttons:"
           << " 0:" << mButton0->getData()
           << " 1:" << mButton1->getData()
           << " 2:" << mButton2->getData()
           << " 3:" << mButton3->getData()
           << " 4:" << mButton4->getData()
           << " 5:" << mButton5->getData() << std::endl;
*/
   mHeadHistory.push_back(gmtl::makeTrans<gmtl::Vec3f>(mHead->getData()));
   if (mMyData.isLocal())
   {
      if (0 != mButton0->getData())
      {
         mMyData->drawBool = true;
      }
      else
      {
         mMyData->drawBool = false;
      }
   }
}

void applicationDataApp::bufferPreDraw()
{
   glClearColor(0.0, 0.0, 0.0, 0.0);
   glClear(GL_COLOR_BUFFER_BIT);
}

void applicationDataApp::draw()
{
   glClear(GL_DEPTH_BUFFER_BIT);
   //std::cout << "\n--- myDraw() ---\n";

//   std::cout << "HeadPos:" << gmtl::makeTrans<gmtl::Vec3f>(mHead->getData()) << "\t"
//             << "WandPos:" << gmtl::makeTrans<gmtl::Vec3f>(mWand->getData()) << std::endl;

   glMatrixMode(GL_MODELVIEW);

      // -- Draw box on wand --- //
   gmtl::Matrix44f wandMatrix = mWand->getData();      // Get the wand matrix

   glPushMatrix();
      // cout << "wand:\n" << *wandMatrix << endl;
      glPushMatrix();
         glMultMatrixf(wandMatrix.mData);  // Push the wand matrix on the stack

         //glColor3f(1.0f, 0.0f, 1.0f);
         float wand_color[3];
         wand_color[0] = wand_color[1] = wand_color[2] = 0.0f;
         if (mButton0->getData() == gadget::Digital::ON)
         {
            wand_color[0] += 0.5f;
         }
         if (mButton1->getData() == gadget::Digital::ON)
         {
            wand_color[1] += 0.5f;
         }
         if (mButton2->getData() == gadget::Digital::ON)
         {
            wand_color[2] += 0.5f;
         }
         if (mButton3->getData() == gadget::Digital::ON)
         {
            wand_color[0] += 0.5f;
         }
         if (mButton4->getData() == gadget::Digital::ON)
         {
            wand_color[1] += 0.5f;
         }
         if (mButton5->getData() == gadget::Digital::ON)
         {
            wand_color[2] += 0.5f;
         }
         glColor3fv(wand_color);
         glScalef(0.25f, 0.25f, 0.25f);
         drawCube();
         glPopMatrix();

         // A little laser pointer
         glLineWidth(5.0f);

         // Draw Axis
         glDisable(GL_LIGHTING);
         glPushMatrix();
            glMultMatrixf(wandMatrix.mData);    // Goto wand position

            gmtl::Vec3f x_axis(7.0f,0.0f,0.0f);
            gmtl::Vec3f y_axis(0.0f, 7.0f, 0.0f);
            gmtl::Vec3f z_axis(0.0f, 0.0f, 7.0f);
            gmtl::Vec3f origin(0.0f, 0.0f, 0.0f);

            glBegin(GL_LINES);
               glColor3f(1.0f, 0.0f, 0.0f);
               glVertex3fv(origin.mData);
               glVertex3fv(x_axis.mData);

               glColor3f(0.0f, 1.0f, 0.0f);
               glVertex3fv(origin.mData);
               glVertex3fv(y_axis.mData);

               glColor3f(0.0f, 0.0f, 1.0f);
               glVertex3fv(origin.mData);
               glVertex3fv(z_axis.mData);
            glEnd();
         glPopMatrix();
         glEnable(GL_LIGHTING);
      glPopMatrix();

      // Draw the head history
      glLineWidth(1.0f);
      glPushMatrix();
         glBegin(GL_LINES);
            glColor3f(0.0f, 0.8f, 0.8f);

            for(unsigned i=0; i<mHeadHistory.size(); ++i)
            {
               glVertex3fv(mHeadHistory[i].mData);
            }
         glEnd();
      glPopMatrix();

      if (mMyData->drawBool == true)
      {
         drawNetwork();
      }
//   vpr::System::msleep(2000);
}

void applicationDataApp::drawNetwork()
{
   glPushMatrix();

   glLineWidth(3.0f);
	
   cluster::ClusterNetwork* network = cluster::ClusterManager::instance()->getNetwork();
	
   std::vector<gadget::NodePtr>::iterator begin = network->getNodesBegin();
   std::vector<gadget::NodePtr>::iterator end = network->getNodesEnd();
   std::vector<gadget::NodePtr>::iterator i;
    
			// Draw the local machine
   glColor3f(0.75f, 0.75f, 0.0f);
   drawbox(-0.25, 0.25, -0.25, 0.25, -0.25, 0.25, GL_QUADS);
		
   for (i = begin ; i != end ; i++)
   {
      glRotatef(360/network->getNumNodes(), 0.0, 1.0, 0.0);
      if ( (*i)->isConnected() )
      {
         glColor3f(0.0f, 1.0f, 0.0f);
	 glBegin(GL_LINES);
	    glVertex3f(0.0f, 0.0f, 0.0f);
	    glVertex3f(1.0f, 0.0f, 0.0f);
	 glEnd();		  
      }
      else
      {
         glColor3f(0.5f, 0.5f, 0.5f);
      }
      glPushMatrix();
      glTranslatef(1.0f, 0.0f, 0.0f);
      drawbox(-0.25, 0.25, -0.25, 0.25, -0.25, 0.25, GL_QUADS);
      glPopMatrix();
   }

   glLineWidth(1.0f);
   glPopMatrix();      
}

void applicationDataApp::initGLState()
{
   GLfloat light0_ambient[] = { 0.1f,  0.1f,  0.1f,  1.0f};
   GLfloat light0_diffuse[] = { 0.7f,  0.7f,  0.7f,  1.0f};
   GLfloat light0_specular[] = { 1.0f,  1.0f,  1.0f,  1.0f};
   GLfloat light0_position[] = {0.5f, 0.75f, 0.75f, 0.0f};

   GLfloat mat_ambient[] = { 0.7f, 0.7f,  0.7f,  1.0f };
   GLfloat mat_diffuse[] = { 1.0f,  0.5f,  0.8f,  1.0f };
   GLfloat mat_specular[] = { 1.0,  1.0,  1.0,  1.0};
   GLfloat mat_shininess[] = { 50.0};
//   GLfloat mat_emission[] = { 1.0,  1.0,  1.0,  1.0};
   GLfloat no_mat[] = { 0.0,  0.0,  0.0,  1.0};

   glLightfv(GL_LIGHT0, GL_AMBIENT,  light0_ambient);
   glLightfv(GL_LIGHT0, GL_DIFFUSE,  light0_diffuse);
   glLightfv(GL_LIGHT0, GL_SPECULAR,  light0_specular);
   glLightfv(GL_LIGHT0, GL_POSITION,  light0_position);

   glMaterialfv( GL_FRONT, GL_AMBIENT, mat_ambient );
   glMaterialfv( GL_FRONT,  GL_DIFFUSE, mat_diffuse );
   glMaterialfv( GL_FRONT, GL_SPECULAR, mat_specular );
   glMaterialfv( GL_FRONT,  GL_SHININESS, mat_shininess );
   glMaterialfv( GL_FRONT,  GL_EMISSION, no_mat);

   glEnable(GL_DEPTH_TEST);
   glEnable(GL_NORMALIZE);
   glEnable(GL_LIGHTING);
   glEnable(GL_LIGHT0);
   glEnable(GL_COLOR_MATERIAL);
   glShadeModel(GL_SMOOTH);
}

void applicationDataApp::drawCube()
{
   drawbox(-0.5, 0.5, -0.5, 0.5, -0.5, 0.5, GL_QUADS);
}

/** Utility function for drawing a cube. */
void drawbox(GLdouble x0, GLdouble x1, GLdouble y0, GLdouble y1,
             GLdouble z0, GLdouble z1, GLenum type)
{
   static GLdouble n[6][3] = {
      {-1.0, 0.0, 0.0}, {0.0, 1.0, 0.0}, {1.0, 0.0, 0.0},
      {0.0, -1.0, 0.0}, {0.0, 0.0, 1.0}, {0.0, 0.0, -1.0}
   };
   static GLint faces[6][4] = {
      { 0, 1, 2, 3}, { 3, 2, 6, 7}, { 7, 6, 5, 4},
      { 4, 5, 1, 0}, { 5, 6, 2, 1}, { 7, 4, 0, 3}
   };
   GLdouble v[8][3], tmp;
   GLint i;

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
      glBegin(type);
         glNormal3dv(&n[i][0]);
         glVertex3dv(&v[faces[i][0]][0]);
         glNormal3dv(&n[i][0]);
         glVertex3dv(&v[faces[i][1]][0]);
         glNormal3dv(&n[i][0]);
         glVertex3dv(&v[faces[i][2]][0]);
         glNormal3dv(&n[i][0]);
         glVertex3dv(&v[faces[i][3]][0]);
      glEnd();
   }
}


}
