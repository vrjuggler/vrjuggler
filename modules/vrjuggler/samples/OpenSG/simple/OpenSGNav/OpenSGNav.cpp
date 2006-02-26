/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2006 by Iowa State University
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

#include <OpenSG/OSGMatrix.h>
#include <OpenSG/OSGSimpleGeometry.h>
#include <OpenSG/OSGDirectionalLight.h>
#include <OpenSG/OSGSceneFileHandler.h>

#include <vrj/vrjConfig.h>

#include <math.h>

#include <gmtl/Matrix.h>
#include <gmtl/MatrixOps.h>
#include <gmtl/Generate.h>
#include <gmtl/Vec.h>
#include <gmtl/Coord.h>
#include <gmtl/Xforms.h>

#include <vrj/Display/Projection.h>
#include <vrj/Display/CameraProjection.h>

#include <OpenSGNav.h>

// Handle any initialization needed before API
void OpenSGNav::init()
{
   vrj::OpenSGApp::init();      // Call base class init
}

/** Called once per context at context creation */
void OpenSGNav::contextInit()
{
   vrj::OpenSGApp::contextInit();
   initGLState();
}

void OpenSGNav::draw()
{
   // Call parent class first to render the scene graph
   vrj::OpenSGApp::draw();
}

void OpenSGNav::preFrame()
{
   const float inc_vel(0.005f);
   const float max_vel(0.5f);

   // Update velocity
   if(mButton0->getData() == gadget::Digital::ON)
   {
      mVelocity += inc_vel;
      std::cout << "vel: " << mVelocity << std::endl;
   }
   else if(mVelocity > 0)
   {
      std::cout << "vel: " << mVelocity << std::endl;
      mVelocity -= inc_vel;
   }

   // Restrict range
   if(mVelocity < 0)
   {
      mVelocity = 0;
   }
   if(mVelocity > max_vel)
   {
      mVelocity = max_vel;
   }

   if(mButton1->getData() == gadget::Digital::ON)
   {
      mVelocity = 0;
   }

   // Travel in model
   // - Find forward direction of wand
   // - Translate along that direction

   // Get the wand matrix
   // NOTE: We ask for the wand transformation matrix in the same units that
   // we are using for this application object.
   gmtl::Matrix44f wand_mat(mWandPos->getData(getDrawScaleFactor()));
   gmtl::Vec3f z_dir = gmtl::Vec3f(0.0f, 0.0f, mVelocity);
   gmtl::Vec3f trans = wand_mat * z_dir;

   OSG::Matrix trans_mat(OSG::Matrix::identity());
   trans_mat.setTranslate(trans[0], trans[1], trans[2]);

   OSG::beginEditCP(mSceneTransform);
      mSceneTransform->getMatrix().multLeft(trans_mat);
   OSG::endEditCP(mSceneTransform);
}

/** Initialize GL state. Hold over from regular OGL apps */
void OpenSGNav::initGLState()
{
   GLfloat light0_ambient[]  = { 0.1f,  0.1f,  0.1f, 1.0f };
   GLfloat light0_diffuse[]  = { 0.7f,  0.7f,  0.7f, 1.0f };
   GLfloat light0_specular[] = { 1.0f,  1.0f,  1.0f, 1.0f };
   GLfloat light0_position[] = { 0.5f, 0.75f, 0.75f, 0.0f };

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
   glEnable(GL_LIGHTING);
   glEnable(GL_LIGHT0);
   glEnable(GL_COLOR_MATERIAL);
   glShadeModel(GL_SMOOTH);

   // OpenSG does not handle this yet, being smart about it is not
   // that trivial...
   glEnable(GL_NORMALIZE);
}

void OpenSGNav::initScene()
{
   vprDEBUG_OutputGuard(vprDBG_ALL, vprDBG_CRITICAL_LVL,
                        "OpenSGNav::initScene() called.\n",
                        "OpenSGNav::initScene() exiting.\n");

   // --- Initialize vrj devices --- //
   std::string wand("VJWand");
   std::string but0("VJButton0");
   std::string but1("VJButton1");
   std::string but2("VJButton2");
   mWandPos.init(wand);
   mButton0.init(but0);
   mButton1.init(but1);
   mButton2.init(but2);

   // Load the model to use
   if (mFileToLoad ==  std::string("none"))
   {
      vprDEBUG(vprDBG_ALL, vprDBG_CRITICAL_LVL)
         << "[OpenSGNav::initScene()] No model specified; creating torus."
         << std::endl << vprDEBUG_FLUSH;
      mModelRoot = OSG::makeTorus(.5, 2, 16, 16);
   }
   else
   {
      vprDEBUG(vprDBG_ALL, vprDBG_CRITICAL_LVL)
         << "[OpenSGNav::initScene()] Loading '" << mFileToLoad << "' ..."
         << std::endl << vprDEBUG_FLUSH;
      mModelRoot =
         OSG::SceneFileHandler::the().read((OSG::Char8*) (mFileToLoad.c_str()));
   }

   // --- Light setup --- //
   // - Add directional light for scene
   // - Create a beacon for it and connect to that beacon
   mLightNode   = OSG::Node::create();
   mLightBeacon = OSG::Node::create();
   OSG::DirectionalLightPtr light_core = OSG::DirectionalLight::create();
   OSG::TransformPtr light_beacon_core = OSG::Transform::create();

   // Setup light beacon
   OSG::Matrix light_pos;
   light_pos.setTransform(OSG::Vec3f(2.0f, 5.0f, 4.0f));

   OSG::beginEditCP(light_beacon_core, OSG::Transform::MatrixFieldMask);
      light_beacon_core->setMatrix(light_pos);
   OSG::endEditCP(light_beacon_core, OSG::Transform::MatrixFieldMask);

   OSG::beginEditCP(mLightBeacon);
      mLightBeacon->setCore(light_beacon_core);
   OSG::endEditCP(mLightBeacon);

   // Setup light node
   OSG::addRefCP(mLightNode);
   OSG::beginEditCP(mLightNode);
      mLightNode->setCore(light_core);
      mLightNode->addChild(mLightBeacon);
   OSG::endEditCP(mLightNode);

   OSG::beginEditCP(light_core);
      light_core->setAmbient   (0.9, 0.8, 0.8, 1);
      light_core->setDiffuse   (0.6, 0.6, 0.6, 1);
      light_core->setSpecular  (1, 1, 1, 1);
      light_core->setDirection (0, 0, 1);
      light_core->setBeacon    (mLightNode);
   OSG::endEditCP(light_core);

   // --- Setup Scene -- //
   // add the loaded scene to the light node, so that it is lit by the light
   OSG::addRefCP(mModelRoot);
   OSG::beginEditCP(mLightNode);
      mLightNode->addChild(mModelRoot);
   OSG::endEditCP(mLightNode);

   // create the root part of the scene
   mSceneRoot = OSG::Node::create();
   mSceneTransform = OSG::Transform::create();

   // Set up the root node
   OSG::beginEditCP(mSceneRoot);
      mSceneRoot->setCore(mSceneTransform);
      mSceneRoot->addChild(mLightNode);
   OSG::endEditCP(mSceneRoot);
}
