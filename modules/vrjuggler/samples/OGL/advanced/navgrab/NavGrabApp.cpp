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

#include <gmtl/Point.h>
#include <gmtl/Matrix.h>
#include <gmtl/Containment.h>
#include <gmtl/Generate.h>
#include <gmtl/Output.h>

#include <gadget/InputManager.h>
#include <gadget/InputLogger.h>

#include <string>
#include <iostream>

#include "NavGrabApp.h"
#include <TestCases/FailureTestCase.h>
#include <TestCases/SphereTestCase.h>

void NavGrabApp::init()
{
   vrj::opengl::App::init();

   mHead.init("VJHead");
   mWand.init("VJWand");

   mForwardButton.init("VJButton0");
   mRotateButton.init("VJButton1");
   mGrabButton.init("VJButton2");
   
   // Uncomment these lines when setting up logger tests
   //mDumpStateButton.init("VJButton2");
   //mLoggerPlayButton.init("LoggerPlayButton");
}

void NavGrabApp::contextInit()
{
   initGLState();
}

void NavGrabApp::preFrame()
{
   // Update the grabbing state
   updateGrabbing();
   
   // Update the state of navigation
   updateNavigation();

   // Update the logger state information
   updateLogger();
}

void NavGrabApp::updateGrabbing()
{
   // Get wand matrix in the virtual world coordinate system
   // wand_vw = vw_M_w * wand_w
   gmtl::Matrix44f vw_M_w;
   gmtl::invert(vw_M_w, mNavMatrix);      // Nav matrix is: w_M_vw.  So invert it
   gmtl::Matrix44f wand_matrix = vw_M_w * mWand->getData();

   // Get the point in space where the wand is located.
   gmtl::Point3f wand_pt = gmtl::makeTrans<gmtl::Point3f>(wand_matrix);

   // Check for intersection with the wand and our shapes.
   mSphereIsect = gmtl::isInVolume(mSphere, wand_pt);
   mCubeIsect   = gmtl::isInVolume(mCube, wand_pt);

   // If button 0 is pressed, check to see if we are grabbing anything.
   if ( mGrabButton->getData() == gadget::Digital::ON )
   {
      // If we are intersecting with the sphere or already have it grabbed
      // update its center to match the center of the wand.
      if ( mSphereIsect || mSphereSelected )
      {
         mSphereSelected = true;
         mSphere.setCenter(wand_pt);
      }
      else
      {
         mSphereSelected = false;
      }

      // If we are intersecting with the cube or grabbing it
      // update its center to match the center of the wand.
      if ( mCubeIsect || mCubeSelected )
      {
         mCubeSelected = true;
         
         gmtl::Point3f corner_offset(mCubeEdgeLength, mCubeEdgeLength, mCubeEdgeLength);
         mCube.setMin(wand_pt - corner_offset);   // Bottom rear left corner
         mCube.setMax(wand_pt + corner_offset);   // Top front right corner
      }
      else
      {
         mCubeSelected = false;
      }
   }
   // Nothing can be selected when the button state toggles from on to off.
   else if ( mGrabButton->getData() == gadget::Digital::TOGGLE_OFF )
   {
      mSphereSelected = false;
      mCubeSelected   = false;
   }
}

void NavGrabApp::updateNavigation()
{
   gmtl::Matrix44f wand_matrix = mWand->getData();      // Get the wand matrix

   // Update navigation
   // - Find forward direction of wand
   // - Translate along that direction
   float velocity(0.05f);
   float rotation(0.0f);
   if(mForwardButton->getData())
   {
      gmtl::Vec3f z_dir = gmtl::Vec3f(0.0f, 0.0f, velocity);
      gmtl::Vec3f dir(wand_matrix * z_dir);
      gmtl::preMult(mNavMatrix, gmtl::makeTrans<gmtl::Matrix44f>(dir));
   }   

   if(mRotateButton->getData())
   {
      const float rot_scale(0.01f);
      float y_rot = gmtl::makeYRot<float, 4, 4>(wand_matrix);
      rotation = -1.0f * y_rot * rot_scale;
      gmtl::preMult(mNavMatrix,
                    gmtl::makeRot<gmtl::Matrix44f>(gmtl::EulerAngleXYZf(0.0f,rotation,0.0f)));
   }

   // ---- RESET ---- //
   // If the reset button is pressed, reset the state of the application.
   // This button takes precedence over all others.
   if ( mForwardButton->getData() && mRotateButton->getData())
   {
      this->reset();
   }

}

void NavGrabApp::updateLogger()
{
   // If the dump state button has just been pressed, dump the current state
   // information.
   if ( mDumpStateButton->getData() == gadget::Digital::TOGGLE_ON )
   {
      dumpState();
   }

   // Playing of logs
   // Check logger play button
   if(mLoggerPlayButton->getData() == gadget::Digital::TOGGLE_ON)
   {
      std::cout << "\n\n------ Log Play Button hit ----\n" << std::flush;
      gadget::InputManager* input_mgr = gadget::InputManager::instance();
      gadget::InputLoggerPtr logger = input_mgr->getInputLogger();

      std::string log_filename;
      std::cout << "Enter filename to load:" << std::flush;
      std::cin >> log_filename;
      std::cout << std::endl << "Using file:" << log_filename << std::endl;

      vprASSERT(logger.get() != NULL);
      logger->load(log_filename);
      logger->play();
   }

   // Sleep several frames before starting up the logger
   if(mFramesToSleep != 0)
   {
      mFramesToSleep--;
      std::cout << "mFramesToSleep: " << mFramesToSleep << std::endl;
   }
   else if((NULL != mTestRunner) && (mTestRunner->getState() == vrj::test::TestRunner::Processing) )
   {
      mTestRunner->processTests();
   }
}

void NavGrabApp::bufferPreDraw()
{
   glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
   glClear(GL_COLOR_BUFFER_BIT);
}

void NavGrabApp::draw()
{
   glClear(GL_DEPTH_BUFFER_BIT);

   glMatrixMode(GL_MODELVIEW);

   glPushMatrix();
      // Move world for navigation
      glMultMatrixf(mNavMatrix.mData);

      drawSphere(mSphere, mSphereIsect, mSphereSelected);
      drawCube(mCube, mCubeIsect, mCubeSelected);
      drawFloor();
   glPopMatrix();
}

void NavGrabApp::reset()
{
   // Initialize the sphere.  This creates a sphere with radius 1 and center
   // (2.5, 3, -2.5).
   gmtl::Point3f sphere_center = gmtl::Point3f(2.5f, 3.0f, -2.5f);
   mSphere.setCenter(sphere_center);
   mSphere.setRadius(1.0f);

   // Initialize the cube.  This creates a cube with sides of length 2 centered
   // at (-2.5, 3, -2.5).
   mCubeEdgeLength = 0.75f;
   gmtl::Point3f cube_center(-2.5, 3, -2.5);
   gmtl::Point3f corner_offset(mCubeEdgeLength, mCubeEdgeLength, mCubeEdgeLength);
   mCube.setMin(cube_center - corner_offset);   // Bottom rear left corner
   mCube.setMax(cube_center + corner_offset);   // Top front right corner
   mCube.setEmpty(false);
   
   mSphereSelected = false;
   mCubeSelected   = false;

   gmtl::identity(mNavMatrix);      // Set Nav matrix to identity
}

void NavGrabApp::dumpState()
{
   vprDEBUG(vprDBG_ALL, vprDBG_CRITICAL_LVL)
      << "Sphere -- center: " << mSphere.getCenter() << " ## radius: "
      << mSphere.getRadius() << std::endl << vprDEBUG_FLUSH;
   vprDEBUG(vprDBG_ALL, vprDBG_CRITICAL_LVL)
      << "Cube -- min: " << mCube.getMin() << " ## max: " << mCube.getMax()
      << std::endl << vprDEBUG_FLUSH;
}

/** Initialize the test runner and the associated tests.
* This also sets up the runner to start processing tests
*/
void NavGrabApp::initTesting()
{
   mTestRunner = new vrj::test::TestRunner;

   mTestRunner->addTest( new SphereTestCase );
   mTestRunner->addTest(new FailureTestCase);
      
   mTestRunner->initialize(this);
}


void NavGrabApp::initShapes()
{
   // Set to initial shape positions
   reset();

   // Allocate a new quadric that will be used to render the sphere.
   mSphereQuad = gluNewQuadric();
}

void NavGrabApp::initGLState()
{
   GLfloat light0_ambient[]  = { 0.1f,  0.1f,  0.1f, 1.0f };
   GLfloat light0_diffuse[]  = { 0.8f,  0.8f,  0.8f, 1.0f };
   GLfloat light0_specular[] = { 1.0f,  1.0f,  1.0f, 1.0f };
   GLfloat light0_position[] = { 0.0f, 0.75f, 0.75f, 0.0f };

   GLfloat mat_ambient[]   = { 0.7f, 0.7f, 0.7f, 1.0f };
   GLfloat mat_diffuse[]   = { 1.0f, 0.5f, 0.8f, 1.0f };
   GLfloat mat_specular[]  = { 1.0f, 1.0f, 1.0f, 1.0f };
   GLfloat mat_shininess[] = { 50.0f };
//   GLfloat mat_emission[]  = { 1.0f, 1.0f, 1.0f, 1.0f };
   GLfloat no_mat[]        = { 0.0f, 0.0f, 0.0f, 1.0f };

   glLightfv(GL_LIGHT0, GL_AMBIENT, light0_ambient);
   glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);
   glLightfv(GL_LIGHT0, GL_SPECULAR, light0_specular);
   glLightfv(GL_LIGHT0, GL_POSITION, light0_position);

   glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
   glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
   glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
   glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
   glMaterialfv(GL_FRONT, GL_EMISSION, no_mat);

   glEnable(GL_DEPTH_TEST);
   glEnable(GL_NORMALIZE);
   glEnable(GL_LIGHTING);
   glEnable(GL_LIGHT0);
   glEnable(GL_COLOR_MATERIAL);
   glShadeModel(GL_SMOOTH);
}

void NavGrabApp::drawSphere(const gmtl::Spheref& sphere,
                            const bool& intersected, const bool& selected)
{
   gmtl::Point3f sphere_center = sphere.getCenter();

   glPushMatrix();
      // Set the rendering color based on the state of the sphere with respect
      // to the wand.
      if ( selected )
      {
         glColor3f(1.0f, 1.0f, 0.0f);
      }
      else if ( intersected )
      {
         glColor3f(1.0f, 0.0f, 0.5f);
      }
      else
      {
         glColor3f(1.0f, 0.0f, 0.0f);
      }

      // Rener the sphere.
      glTranslatef(sphere_center[0], sphere_center[1], sphere_center[2]);
      gluSphere(mSphereQuad, sphere.getRadius(), 15, 15);
   glPopMatrix();
}

void NavGrabApp::drawCube(const gmtl::AABoxf& cube, const bool& intersected,
                          const bool& selected)
{
   // Define the normals for the cube faces.
   static GLdouble normals[6][3] = {
      { -1.0, 0.0, 0.0 }, { 0.0, 1.0, 0.0 }, { 1.0, 0.0, 0.0 },
      { 0.0, -1.0, 0.0 }, { 0.0, 0.0, 1.0 }, { 0.0, 0.0, -1.0 }
   };

   // Define the array indices for the cube faces.  These will be used to
   // access values in the v array declared below.
   static GLint faces[6][4] = {
      { 0, 1, 2, 3 }, { 3, 2, 6, 7 }, { 7, 6, 5, 4 },
      { 4, 5, 1, 0 }, { 5, 6, 2, 1 }, { 7, 4, 0, 3 }
   };

   gmtl::Point3f cube_min = cube.getMin();
   gmtl::Point3f cube_max = cube.getMax();

   // Define the vertices based on the min and max points on the cube.
   GLdouble v[8][3];
   v[0][0] = v[1][0] = v[2][0] = v[3][0] = cube_min[0];
   v[4][0] = v[5][0] = v[6][0] = v[7][0] = cube_max[0];
   v[0][1] = v[1][1] = v[4][1] = v[5][1] = cube_min[1];
   v[2][1] = v[3][1] = v[6][1] = v[7][1] = cube_max[1];
   v[0][2] = v[3][2] = v[4][2] = v[7][2] = cube_min[2];
   v[1][2] = v[2][2] = v[5][2] = v[6][2] = cube_max[2];

   // Set the rendering color based on the state of the cube with respect to
   // the wand.
   if ( selected )
   {
      glColor3f(0.0f, 1.0f, 1.0f);
   }
   else if ( intersected )
   {
      glColor3f(0.5f, 0.0f, 1.0f);
   }
   else
   {
      glColor3f(0.0f, 0.0f, 1.0f);
   }

   // Draw the cube.
   for ( GLint i = 0; i < 6; ++i )
   {
      glBegin(GL_QUADS);
         glNormal3dv(&normals[i][0]);
         glVertex3dv(&v[faces[i][0]][0]);
         glVertex3dv(&v[faces[i][1]][0]);
         glVertex3dv(&v[faces[i][2]][0]);
         glVertex3dv(&v[faces[i][3]][0]);
      glEnd();
   }
}

void NavGrabApp::drawFloor()
{
   glPushAttrib(GL_LINE_BIT | GL_POLYGON_BIT);
      glPushMatrix();
         glPolygonMode(GL_FRONT, GL_LINE);
         glLineWidth(2);
         glColor3f(0.0f, 0.9f, 0.3f);

         glBegin(GL_LINES);
            for ( int z = -25; z < 25; ++z )
            {
               for ( int x = -25; x < 25; ++x )
               {
                  // Draw the line along the Z axis.
                  glVertex3f((float) x, 0.01f, (float) z);
                  glVertex3f((float) x, 0.01f, (float) z + 1.0f);

                  // Draw the line along the X axis.
                  glVertex3f((float) x, 0.01f, (float) z);
                  glVertex3f((float) x + 1.0f, 0.01f, (float) z);
               }
            }
         glEnd();
      glPopMatrix();
   glPopAttrib();
}

