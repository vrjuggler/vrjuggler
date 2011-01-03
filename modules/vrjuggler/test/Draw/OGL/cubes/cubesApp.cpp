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

#include <iostream>
#include <math.h>

#include <gmtl/Quat.h>
#include <gmtl/QuatOps.h>
#include <gmtl/Vec.h>
#include <gmtl/Generate.h>
#include <gmtl/Output.h>

#include <vrj/Draw/OpenGL/UserData.h>
#include <vrj/Display/Viewport.h>
#include <vrj/Kernel/User.h>
#include <vpr/Perf/ProfileManager.h>

#include <cubesApp.h>


// ----------------------------------------------------------------------------
// UserData methods.
// ----------------------------------------------------------------------------

// Update the navigation matrix
//
// Uses a quaternion to do rotation in the environment
void UserData::updateNavigation()
{
   VPR_PROFILE_GUARD("UserData::updateNavigation");
   gmtl::EulerAngleXYZf xyzAngles;
   gmtl::Vec3f xyzTrans;

   // Cur*Transform = New Location
   gmtl::Matrix44f transform, transformIdent;
   gmtl::Quatf     source_rot, goal_rot, slerp_rot;

   gmtl::identity(transformIdent);

   gmtl::Matrix44f wand_matrix = mWand->getData();
   gmtl::setRot( xyzAngles, wand_matrix );

   vprDEBUG(vprDBG_ALL, vprDBG_DETAILED_LVL)
      << "===================================\n" << vprDEBUG_FLUSH;
   vprDEBUG(vprDBG_ALL, vprDBG_DETAILED_LVL) << "Wand:\n" << wand_matrix
                                             << std::endl << vprDEBUG_FLUSH;
//   vprDEBUG(vprDBG_ALL, vprDBG_DETAILED_LVL) << "Wand XYZ: " << xyzAngles << std::endl
//                        << vprDEBUG_FLUSH;

   gmtl::set(goal_rot, wand_matrix); // Create the goal rotation quaternion

   if(transformIdent != wand_matrix)  // If we don't have two identity matrices
   {
      gmtl::slerp(slerp_rot, 0.05f, source_rot, goal_rot); // Transform part way there
      gmtl::set(transform, slerp_rot);      // Create the transform matrix to use
   }
   else
   {
      gmtl::identity(transform);
   }

   vprDEBUG(vprDBG_ALL, vprDBG_DETAILED_LVL) << "Transform:\n" << transform
                                             << std::endl << vprDEBUG_FLUSH;
   gmtl::setRot(xyzAngles, transform);
//   vprDEBUG(vprDBG_ALL, vprDBG_DETAILED_LVL) << "Transform XYZ: " << xyzAngles
//                                             << std::endl << vprDEBUG_FLUSH;

   vprDEBUG(vprDBG_ALL, vprDBG_DETAILED_LVL) << "Nav:\n" << mNavMatrix
                                             << std::endl << std::endl
                                             << vprDEBUG_FLUSH;

   // ----- Translation ------- //
   const float velocity_inc = 0.001f;

   // Update velocity
   if ( mIncVelocityButton->getData() )
   {
      mCurVelocity += velocity_inc;
   }
   else if ( mDecVelocityButton->getData() )
   {
      mCurVelocity -= velocity_inc;
   }
   else if ( mStopButton->getData() )
   {
      mCurVelocity = 0.0f;
   }

   if ( mIncVelocityButton->getData() || mDecVelocityButton->getData() )
   {
      vprDEBUG(vprDBG_ALL, vprDBG_DETAILED_LVL)
         << "Velocity: " << mCurVelocity << std::endl << vprDEBUG_FLUSH;
   }

   if ( mIncVelocityButton->getData() == gadget::Digital::TOGGLE_ON )
   {
      vprDEBUG(vprDBG_ALL, vprDBG_STATE_LVL) << "-- Toggle ON --" << std::endl
                                             << vprDEBUG_FLUSH;
   }

   if ( mIncVelocityButton->getData() == gadget::Digital::TOGGLE_OFF )
   {
      vprDEBUG(vprDBG_ALL, vprDBG_STATE_LVL) << "-- Toggle OFF --" << std::endl
                                             << vprDEBUG_FLUSH;
   }

   if ( mIncVelocityButton->getData() == gadget::Digital::ON )
   {
      vprDEBUG(vprDBG_ALL, vprDBG_STATE_LVL) << "-- ON --" << std::endl
                                             << vprDEBUG_FLUSH;
   }

//   if ( mIncVelocityButton->getData() == Digital::OFF )
//   {
//      vprDEBUG(vprDBG_ALL,) << "-- OFF --" << std::endl << vprDEBUG_FLUSH;
//   }

   // Find direction vector
   gmtl::Vec3f forward(0.0f, 0.0f, -1.0f);
   forward *= mCurVelocity;

   gmtl::Matrix44f rot_mat, local_xform;
   gmtl::invert(rot_mat, transform);

   local_xform = gmtl::makeTrans<gmtl::Matrix44f>(gmtl::Vec3f(0.0f, 0.0f,
                                                              mCurVelocity));
   gmtl::postMult(local_xform, rot_mat);

   gmtl::preMult(mNavMatrix, local_xform);

   gmtl::setRot(xyzAngles, local_xform );
   gmtl::setTrans(xyzTrans, local_xform);
//   vprDEBUG(vprDBG_ALL, vprDBG_DETAILED_LVL) << "Transform   Rot: " << xyzAngles << std::endl
//                        << vprDEBUG_FLUSH;
   vprDEBUG(vprDBG_ALL, vprDBG_DETAILED_LVL)
      << "Transform Trans: " << xyzTrans << std::endl << vprDEBUG_FLUSH;
   vprDEBUG(vprDBG_ALL, vprDBG_DETAILED_LVL)
      << "-------------------------------------------" << std::endl
      << vprDEBUG_FLUSH;
}

void UserData::updateShapeSetting()
{
   VPR_PROFILE_GUARD("UserData::updateShapeSetting");
   if ( mToggleButton->getData() == gadget::Digital::TOGGLE_ON )
   {
      // Toggle back and forth between cone and cube rendering.
      mShapeSetting = ((mShapeSetting == CUBE) ? CONE : CUBE);
   }
}

// ----------------------------------------------------------------------------
// cubesApp methods.
// ----------------------------------------------------------------------------

// Execute any initialization needed before the API is started.  Put device
// initialization here.
void cubesApp::init()
{
   VPR_PROFILE_GUARD("cubesApp::init");
   vrj::opengl::App::init();

   vprDEBUG(vprDBG_ALL, vprDBG_VERB_LVL) << "---------- cubes:App:init() ---------------"
                        << std::endl << vprDEBUG_FLUSH;
   // Request user list
   const std::vector<vrj::UserPtr>& users = mKernel->getUsers();
   size_t num_users = users.size();
   vprASSERT(num_users > 0);      // Make sure that we actually have users defined

   UserData* new_user=NULL;
   mUserData = std::vector<UserData*>(num_users);

   switch (num_users)
   {
   case (2):
      new_user = new UserData(users[1],"VJWand1", "VJButton0_1", "VJButton1_1",
                              "VJButton2_1", "VJButton3_1");
      mUserData[1] = new_user;
      vprASSERT(users[1]->getId() == 1);
   case (1):
      new_user = new UserData(users[0],"VJWand", "VJButton0", "VJButton1",
                              "VJButton2", "VJButton3");
      mUserData[0] = new_user;
      vprASSERT(users[0]->getId() == 0);
      break;
   default:
      vprDEBUG(vprDBG_ALL, vprDBG_CRITICAL_LVL)
         << clrOutNORM(clrRED, "ERROR:") << " Bad number of users."
         << vprDEBUG_FLUSH;
      exit();
      break;
   }
}

// Called immediately upon opening a new OpenGL context.  This is called once
// for every display window that is opened.  Put OpenGL resource allocation
// here.
void cubesApp::contextInit()
{
   *mConeQuad = gluNewQuadric();
   *mBaseQuad = gluNewQuadric();

   VPR_PROFILE_GUARD("cubesApp::contextInit");
   // Generate some random lists.  NOTE: Needed for testing/debugging only!
   mDlDebugData->maxIndex = rand()%50;
   mDlDebugData->dlIndex = glGenLists(mDlDebugData->maxIndex);

   // Generate the display list to be used for the cube faces.  This is the
   // important one.
   mDlCubeData->dlIndex = glGenLists(1);

   // Generate the display list to be used for the cones.
   mDlConeData->dlIndex = glGenLists(1);

   // Verify that we got a valid display list index.
   if ( glIsList(mDlCubeData->dlIndex) == GL_FALSE )
   {
      vprASSERT(false && "glGenLists() returned an invalid display list!");
   }
   else
   {
      glNewList(mDlCubeData->dlIndex, GL_COMPILE);
         drawbox(-1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f, GL_QUADS);
      glEndList();

      glNewList(mDlConeData->dlIndex, GL_COMPILE);
         drawCone(1.1f, 2.0f, 20, 10);
      glEndList();

      vprDEBUG(vprDBG_ALL, vprDBG_VERB_LVL)
         << "Created cube DL:" << mDlCubeData->dlIndex << std::endl
         << vprDEBUG_FLUSH;
      vprDEBUG(vprDBG_ALL, vprDBG_VERB_LVL)
         << "Created cone DL:" << mDlConeData->dlIndex << std::endl
         << vprDEBUG_FLUSH;
      std::cerr << "created displays lists:" << mDlDebugData->maxIndex + 1
                << std::endl;

      initGLState();
   }
}

//: Called immediately upon closing an OpenGL context
// (called for every window that is closed)
// put your opengl deallocation here...
void cubesApp::contextClose()
{
   gluDeleteQuadric(*mConeQuad);
   gluDeleteQuadric(*mBaseQuad);

   VPR_PROFILE_GUARD("cubesApp::contextClose");
   // Deallocate the random display lists used for debugging.
   if ( glIsList(mDlDebugData->dlIndex) == GL_TRUE )
   {
      vprDEBUG(vprDBG_ALL, vprDBG_VERB_LVL)
         << "Deallocating " << mDlDebugData->maxIndex << " display lists\n"
         << vprDEBUG_FLUSH;
      glDeleteLists(mDlDebugData->dlIndex, mDlDebugData->maxIndex);
   }

   // Deallocate the cube face geometry data from the video hardware.
   if ( glIsList(mDlCubeData->dlIndex) == GL_TRUE )
   {
      glDeleteLists(mDlCubeData->dlIndex, 1);
   }

   // Deallocate the cone geometry data from the video hardware.
   if ( glIsList(mDlConeData->dlIndex) == GL_TRUE )
   {
      vprDEBUG(vprDBG_ALL, vprDBG_VERB_LVL)
         << "Deallocating cone display list\n" << vprDEBUG_FLUSH;
      glDeleteLists(mDlConeData->dlIndex, 1);
   }
}

// Draw the scene.  A bunch of boxes of
// differing color and stuff.
// NOTE: This draw routine places extreme stress on VR Juggler's
//       vrj::pengl::ContextData class (by repeatedly dereferencing it to
//       access a display list index).  As such, performance of this method
//       will suffer on multipipe configurations.
//       DO NOT IMITATE THIS CODE.
void cubesApp::myDraw(vrj::UserPtr user)
{
   VPR_PROFILE_GUARD_HISTORY("cubesApp::myDraw", 10);
   vprDEBUG(vprDBG_ALL, vprDBG_HVERB_LVL) << "\n--- myDraw() ---\n" << vprDEBUG_FLUSH;

   static const float SCALE = 100;
   //static const float SCALE = 10;
   static const float INCR = 0.1f;

   /*
   vrj::opengl::UserData* user_data =
      vrj::opengl::DrawManager::instance()->currentUserData();
   int cur_eye = user_data->getProjection()->getEye();

   if(cur_eye == vrj::Projection::LEFT)
   {
      glColor3f(1.0f, 0.0f, 0.0f);
   }
   else if(cur_eye == vrj::Projection::RIGHT)
   {
      glColor3f(0.0f, 0.0f, 1.0f);
   }
   else
   {
      glColor3f(0.0f, 1.0f, 0.0f);
   }
   */

   ContextTimingData* timing_data = &(*mContextTiming);
   int dlist_index(0);

   glPushMatrix();
         // --- Push on Navigation matrix for the user --- //

      gmtl::Matrix44f nav_matrix = mUserData[user->getId()]->mNavMatrix;
      glMultMatrixf(nav_matrix.mData);


      //---- Main box loop -----///
      for (float x=0;x<1;x += INCR)
      {
         for (float y=0;y<1; y += INCR)
         {
            for (float z=0;z<1;z += INCR)
            {
               glColor3f(x, y, z);     // Set the Color
               glPushMatrix();
               glTranslatef( (x-0.5)*SCALE, (y-0.5)*SCALE, (z-0.5)*SCALE);

               switch (mUserData[user->getId()]->getShapeSetting())
               {
                  case UserData::CONE:
                     timing_data->dlist_wait.startSample();
                     dlist_index = mDlConeData->dlIndex;
                     timing_data->dlist_wait.stopSample();
                     break;
                  case UserData::CUBE:
                  default:
                     timing_data->dlist_wait.startSample();
                     dlist_index = mDlCubeData->dlIndex;
                     timing_data->dlist_wait.stopSample();
                     break;
               }

               glCallList(dlist_index);

               glPopMatrix();
            }
         }
      }


     /*
         // --- Draw corner boxes --- //
      for(float x=0;x<=1.0;x++)
         for(float y=0.0;y<=1.0;y++)
            for(float z=0.0;z<=1.0;z++)
            {
               //glColor3f(x,y,z);    // Set the color
               glColor3f(1.0, 0.0,  0.0f);
                  glPushMatrix();
               {
                  glTranslatef( (x-0.5)*SCALE, (y-0.0)*SCALE, (z-0.5)*SCALE);
                  glScalef(2.0f, 2.0f, 2.0f);
                  drawCube();
               }
               glPopMatrix();
            }
     */

   glPopMatrix();
}

void cubesApp::initGLState()
{
   GLfloat light0_ambient[] = { .2f,  .2f,  .2f,  1.0f };
   GLfloat light0_diffuse[] = { 1.0,  1.0,  1.0,  1.0 };
   GLfloat light0_specular[] = { 1.0,  1.0,  1.0,  1.0};
   GLfloat light0_position[] = {2000.0, 1000.0, 100.0, 1.0};
   GLfloat light0_direction[] = {-100, -100.0, -100.0};

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
   glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, light0_direction);

   glMaterialfv( GL_FRONT, GL_AMBIENT, mat_ambient );
   glMaterialfv( GL_FRONT,  GL_DIFFUSE, mat_diffuse );
   glMaterialfv( GL_FRONT, GL_SPECULAR, mat_specular );
   glMaterialfv( GL_FRONT,  GL_SHININESS, mat_shininess );
   glMaterialfv( GL_FRONT,  GL_EMISSION, no_mat);

   glEnable(GL_DEPTH_TEST);
   //glEnable(GL_NORMALIZE);
   glEnable(GL_LIGHTING);
   glEnable(GL_LIGHT0);
   glEnable(GL_COLOR_MATERIAL);
   glShadeModel(GL_SMOOTH);

   glMatrixMode(GL_MODELVIEW);
}

void cubesApp::drawbox(GLdouble x0, GLdouble x1, GLdouble y0, GLdouble y1,
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
