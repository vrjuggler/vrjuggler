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
   vrj::opensg::App::init();      // Call base class init
}

/** Called once per context at context creation */
void OpenSGNav::contextInit()
{
   vrj::opensg::App::contextInit();
   initGLState();
}

void OpenSGNav::draw()
{
   // Call parent class first to render the scene graph
   vrj::opensg::App::draw();
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

#if OSG_MAJOR_VERSION < 2
   CPEdit(mSceneTransform, OSG::Transform::MatrixFieldMask);
   mSceneTransform->getMatrix().multLeft(trans_mat);
#else
   mSceneTransform->editMatrix().multLeft(trans_mat);
#endif

   vrj::opensg::App::preFrame();
}

void OpenSGNav::exit()
{
   // Ensure that we release all the OSG::RefPtr<T> objects that we hold so
   // that OpenSG can shut down cleanly.
   mSceneRoot      = OSG::NullFC;
   mSceneTransform = OSG::NullFC;
   mModelRoot      = OSG::NullFC;
   mLightNode      = OSG::NullFC;
   mLightBeacon    = OSG::NullFC;

   vrj::opensg::App::exit();
}

/** Initialize GL state. Hold over from regular OGL apps */
void OpenSGNav::initGLState()
{
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
      const OSG::Char8* file = mFileToLoad.c_str();
#if OSG_MAJOR_VERSION < 2
      mModelRoot = OSG::SceneFileHandler::the().read(file);
#else
      mModelRoot = OSG::SceneFileHandler::the()->read(file);
#endif
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

#if OSG_MAJOR_VERSION < 2
   CPEdit(light_beacon_core, OSG::Transform::MatrixFieldMask);
   CPEdit(mLightBeacon, OSG::Node::CoreFieldMask);
   CPEdit(mLightNode, OSG::Node::CoreFieldMask | OSG::Node::ChildrenFieldMask);
   CPEditAll(light_core);
   CPEdit(mSceneRoot, OSG::Node::CoreFieldMask | OSG::Node::ChildrenFieldMask);
#endif

   light_beacon_core->setMatrix(light_pos);

   mLightBeacon->setCore(light_beacon_core);

   // Setup light node
   mLightNode->setCore(light_core);
   mLightNode->addChild(mLightBeacon);

   light_core->setAmbient  (0.9, 0.8, 0.8, 1);
   light_core->setDiffuse  (0.6, 0.6, 0.6, 1);
   light_core->setSpecular (1, 1, 1, 1);
   light_core->setDirection(0, 0, 1);
   light_core->setBeacon   (mLightNode);

   // --- Setup Scene -- //
   // add the loaded scene to the light node, so that it is lit by the light
   mLightNode->addChild(mModelRoot);

   // create the root part of the scene
   mSceneRoot = OSG::Node::create();
   mSceneTransform = OSG::Transform::create();

   // Set up the root node
   mSceneRoot->setCore(mSceneTransform);
   mSceneRoot->addChild(mLightNode);
}
