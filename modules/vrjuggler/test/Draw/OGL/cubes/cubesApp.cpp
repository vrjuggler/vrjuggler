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
#include <vrj/Draw/OpenGL/DrawManager.h>
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

GLfloat cubesApp::sVertexData[] = {
      1.0f, 1.0f, 1.0f, 1.0f,-1.0f, 1.0f, 1.0f,-1.0f,-1.0f, 1.0f, 1.0f,-1.0f,
      1.0f, 1.0f, 1.0f, 1.0f, 1.0f,-1.0f,-1.0f, 1.0f,-1.0f,-1.0f, 1.0f, 1.0f,
      1.0f, 1.0f, 1.0f,-1.0f, 1.0f, 1.0f,-1.0f,-1.0f, 1.0f, 1.0f,-1.0f, 1.0f,
     -1.0f,-1.0f,-1.0f,-1.0f,-1.0f, 1.0f,-1.0f, 1.0f, 1.0f,-1.0f, 1.0f,-1.0f,
     -1.0f,-1.0f,-1.0f, 1.0f,-1.0f,-1.0f, 1.0f,-1.0f, 1.0f,-1.0f,-1.0f, 1.0f,
     -1.0f,-1.0f,-1.0f,-1.0f, 1.0f,-1.0f, 1.0f, 1.0f,-1.0f, 1.0f,-1.0f,-1.0f,
   };

 GLfloat cubesApp::sNormalData[] = {
      1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
      0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
      0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
     -1.0f, 0.0f, 0.0f,-1.0f, 0.0f, 0.0f,-1.0f, 0.0f, 0.0f,-1.0f, 0.0f, 0.0f,
      0.0f,-1.0f, 0.0f, 0.0f,-1.0f, 0.0f, 0.0f,-1.0f, 0.0f, 0.0f,-1.0f, 0.0f,
      0.0f, 0.0f,-1.0f, 0.0f, 0.0f,-1.0f, 0.0f, 0.0f,-1.0f, 0.0f, 0.0f,-1.0f
   };

GLushort cubesApp::sIndices[] = {
      0,  1,  2,  0,  2,  3,
      4,  5,  6,  4,  6,  7,
      8,  9,  10, 8,  10, 11,
      12, 13, 14, 12, 14, 15,
      16, 17, 18, 16, 18, 19,
      20, 21, 22, 20, 22, 23
   };

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
   const std::string& sVertexSource = " \
      uniform vec4 uTranslation;\n \
      attribute vec4 aVertexCoord;\n \
      attribute vec3 aVertexNormal;\n \
      uniform vec4 uVertexColor;\n \
      varying vec4 vFragColor;\n \
      void main(void) {\n \
         mat3 mNormalMatrix;\n \
         mNormalMatrix[0] = gl_ModelViewMatrix[0].xyz;\n \
         mNormalMatrix[1] = gl_ModelViewMatrix[1].xyz;\n \
         mNormalMatrix[2] = gl_ModelViewMatrix[2].xyz;\n \
         vec3 mNormal = normalize(mNormalMatrix * aVertexNormal);\n \
         vec3 mLightDir = vec3(0.0, 0.0, 1.0);\n \
         float mfDot = max(0.5, dot(mNormal, mLightDir));\n \
         vFragColor.rgb = uVertexColor.rgb * mfDot;\n \
         vFragColor.a = uVertexColor.a;\n \
         mat4 mMVPMatrix = gl_ProjectionMatrix * gl_ModelViewMatrix;\n \
         gl_Position = mMVPMatrix * (aVertexCoord + uTranslation);\n \
      }\n";

   const std::string& sFragmentSource = " \
      varying vec4 vFragColor;\n \
      void main(void) {\n \
         gl_FragColor = vFragColor;\n \
      }\n";

   vrj::opengl::ExtensionLoaderGL& gl =
         vrj::opengl::DrawManager::instance()->getGL();
   std::string vertexSource = sVertexSource;
   std::string fragmentSource = sFragmentSource;

   // Add matrix uniforms if using the core profile
   if (gl.isCoreProfile())
   {
      vertexSource = " \
         uniform mat4 gl_ModelViewMatrix;\n \
         uniform mat4 gl_ProjectionMatrix;\n" + vertexSource;
   }

   mProgram = gl.createProgram(vertexSource, fragmentSource);
   if (mProgram != 0)
   {
      if (gl.isCoreProfile())
      {
         muMVMatrixHandle = gl.GetUniformLocation(mProgram, "gl_ModelViewMatrix");
         muPMatrixHandle = gl.GetUniformLocation(mProgram, "gl_ProjectionMatrix");
      }

      muTranslationHandle = gl.GetUniformLocation(mProgram, "uTranslation");
      maVertexCoordHandle = gl.GetAttribLocation(mProgram, "aVertexCoord");
      maVertexNormalHandle = gl.GetAttribLocation(mProgram, "aVertexNormal");
      muVertexColorHandle = gl.GetUniformLocation(mProgram, "uVertexColor");
   
      if (gl.GenVertexArrays != NULL)
         gl.GenVertexArrays(1, &mVertexArrayBufferID);
      gl.GenBuffers(1, &mVertexCoordBufferID);
      gl.GenBuffers(1, &mVertexNormalBufferID);
      gl.GenBuffers(1, &mIndexBufferID);

      gl.BindBuffer(GL_ARRAY_BUFFER, mVertexCoordBufferID);
      gl.BufferData(GL_ARRAY_BUFFER, 72 * sizeof(GLfloat), &sVertexData[0], GL_STATIC_DRAW);
      gl.BindBuffer(GL_ARRAY_BUFFER, mVertexNormalBufferID);
      gl.BufferData(GL_ARRAY_BUFFER, 72 * sizeof(GLfloat), &sNormalData[0], GL_STATIC_DRAW);
      gl.BindBuffer(GL_ARRAY_BUFFER, 0);
      gl.BindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBufferID);
      gl.BufferData(GL_ELEMENT_ARRAY_BUFFER, 36 * sizeof(GLushort), &sIndices[0], GL_STATIC_DRAW);
      gl.BindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
   }

   VPR_PROFILE_GUARD("cubesApp::contextInit");
   initGLState();
}

//: Called immediately upon closing an OpenGL context
// (called for every window that is closed)
// put your opengl deallocation here...
void cubesApp::contextClose()
{
   VPR_PROFILE_GUARD("cubesApp::contextClose");
}

// Draw the scene.  A bunch of boxes of differing color.
void cubesApp::myDraw(vrj::UserPtr user)
{
   VPR_PROFILE_GUARD_HISTORY("cubesApp::myDraw", 10);
   vprDEBUG(vprDBG_ALL, vprDBG_HVERB_LVL) << "\n--- myDraw() ---\n" << vprDEBUG_FLUSH;

   static const float SCALE = 100;
   static const float INCR = 0.1f;

   vrj::opengl::ExtensionLoaderGL& gl =
         vrj::opengl::DrawManager::instance()->getGL();
   if (mProgram != 0)   // OpenGL 3.2+ compatible rendering
   {
      gl.UseProgram(mProgram);
      if (mVertexArrayBufferID != 0)
         gl.BindVertexArray(mVertexArrayBufferID);
      
      gl.BindBuffer(GL_ARRAY_BUFFER, mVertexCoordBufferID);
      gl.VertexAttribPointer(maVertexCoordHandle, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
      gl.EnableVertexAttribArray(maVertexCoordHandle);

      gl.BindBuffer(GL_ARRAY_BUFFER, mVertexNormalBufferID);
      gl.VertexAttribPointer(maVertexNormalHandle, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
      gl.EnableVertexAttribArray(maVertexNormalHandle);

      gl.PushMatrix();
      gmtl::Matrix44f nav_matrix = mUserData[user->getId()]->mNavMatrix;
      gl.MultMatrix(nav_matrix);

      if (gl.isCoreProfile())
      {
         gl.UniformMatrix4fv(muMVMatrixHandle, 1, false, gl.getModelViewMatrix().mData);
         gl.UniformMatrix4fv(muPMatrixHandle, 1, false, gl.getProjectionMatrix().mData);
      }

      gl.BindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBufferID);

      for (float x=0;x<1;x += INCR)
      {
         for (float y=0;y<1; y += INCR)
         {
            for (float z=0;z<1;z += INCR)
            {
               gl.Uniform4f(muTranslationHandle, (x-0.5)*SCALE, (y-0.5)*SCALE, (z-0.5)*SCALE, 0);
               gl.Uniform4f(muVertexColorHandle, x, y, z, 1.0f);
               gl.DrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, 0);  
            }
         }
      }

      gl.PopMatrix();
      gl.DisableVertexAttribArray(maVertexCoordHandle);
      gl.DisableVertexAttribArray(maVertexNormalHandle);
      gl.BindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
      gl.BindBuffer(GL_ARRAY_BUFFER, 0);
      if (mVertexArrayBufferID != 0)
        gl.BindVertexArray(0);
      gl.UseProgram(0);
   }
   else  // OpenGL 1.1 compatible rendering
   {
      glEnableClientState(GL_VERTEX_ARRAY);
      glVertexPointer(3, GL_FLOAT, 0, (const GLvoid*)sVertexData);
      glEnableClientState(GL_NORMAL_ARRAY);
      glNormalPointer(GL_FLOAT, 0, (const GLvoid*)sNormalData);

      gl.PushMatrix();
      gmtl::Matrix44f nav_matrix = mUserData[user->getId()]->mNavMatrix;
      gl.MultMatrix(nav_matrix);

      //---- Main box loop -----///
      for (float x=0;x<1;x += INCR)
      {
         for (float y=0;y<1; y += INCR)
         {
            for (float z=0;z<1;z += INCR)
            {
               gl.PushMatrix();
               gl.Translate((x-0.5)*SCALE, (y-0.5)*SCALE, (z-0.5)*SCALE);
               glColor3f(x, y, z);     // Set the Color
               gl.DrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, (const GLvoid*)sIndices);
               gl.PopMatrix();     
            }
         }
      }

      gl.PopMatrix();
      glDisableClientState(GL_VERTEX_ARRAY);
      glDisableClientState(GL_NORMAL_ARRAY);
   }
}

void cubesApp::initGLState()
{
   vrj::opengl::ExtensionLoaderGL& gl =
         vrj::opengl::DrawManager::instance()->getGL();

   if (mProgram == 0)
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

      gl.Enable(GL_DEPTH_TEST);
      gl.Enable(GL_LIGHTING);
      gl.Enable(GL_LIGHT0);
      gl.Enable(GL_COLOR_MATERIAL);
      glShadeModel(GL_SMOOTH);
   }
   else
      gl.Enable(GL_DEPTH_TEST);

   gl.MatrixMode(GL_MODELVIEW);
}
