/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2003 by Iowa State University
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

#include <math.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include <gmtl/Matrix.h>
#include <gmtl/MatrixOps.h>
#include <gmtl/Generate.h>
#include <gmtl/Vec.h>
#include <gmtl/Coord.h>
#include <gmtl/Xforms.h>

#include <vrj/Display/Projection.h>
#include <vrj/Display/CameraProjection.h>

#include <OpenSGNav.h>

#include <OpenSG/OSGMatrix.h>
#include <OpenSG/OSGSimpleGeometry.h>
#include <OpenSG/OSGDirectionalLight.h>
#include <OpenSG/OSGSceneFileHandler.h>


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
      velocity += inc_vel;
      std::cout << "vel: " << velocity << std::endl;
   }
   else if(velocity > 0)
   {
      std::cout << "vel: " << velocity << std::endl;
      velocity -= inc_vel;
   }

   // Restrict range
   if(velocity < 0)
   {
      velocity = 0;
   }
   if(velocity > max_vel)
   {
      velocity = max_vel;
   }

   if(mButton1->getData() == gadget::Digital::ON)
   {
      velocity = 0;
   }

   // Travel in model
   // - Find forward direction of wand
   // - Translate along that direction
   gmtl::Matrix44f wandMatrix;
   wandMatrix = mWandPos->getData();      // Get the wand matrix
   gmtl::Vec3f direction;
   gmtl::Vec3f Zdir = gmtl::Vec3f(0.0f, 0.0f, velocity);
   direction = wandMatrix * Zdir;

   osg::Matrix osg_trans_mat( osg::Matrix::identity());
   osg_trans_mat.setTranslate(direction[0], direction[1], direction[2]);
      
   osg::beginEditCP(mSceneTransform);
      mSceneTransform->getMatrix().multLeft(osg_trans_mat);
   osg::endEditCP(mSceneTransform);
}

/** Initialize GL state. Hold over from regular OGL apps */
void OpenSGNav::initGLState()
{
    // OpenSG does not handle this yet, being smart about it is not 
    // that trivial...
    
    glEnable(GL_NORMALIZE);
}

void OpenSGNav::initScene(void)
{
   vprDEBUG(vprDBG_ALL, vprDBG_CRITICAL_LVL) << "OpenSGNav::initScene: Called.\n" << vprDEBUG_FLUSH;
   
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
      std::cout << "OpenSGNave::initScene: No model specified!!! Loading torus.\n";
      mModelRoot = OSG::makeTorus(.5, 2, 16, 16);      
   }
   else
   {
      std::cout << "OpenSGNav::initScene: Loading [" << mFileToLoad.c_str() << "]\n";
      mModelRoot = OSG::SceneFileHandler::the().read((OSG::Char8 *)(mFileToLoad.c_str()));
   }

   // --- Light setup --- //
   // - Add directional light for scene
   // - Create a beacon for it and connect to that beacon
   mLightNode   = OSG::Node::create();
   mLightBeacon = OSG::Node::create();
   OSG::DirectionalLightPtr light_core = OSG::DirectionalLight::create();
   OSG::TransformPtr light_beacon_core = OSG::Transform::create();
   
   // Setup light beacon
   osg::Matrix light_pos;
   light_pos.setTransform(osg::Vec3f( 2.0f, 5.0f, 4.0f));
   
   osg::beginEditCP(light_beacon_core, OSG::Transform::MatrixFieldMask);
      light_beacon_core->setMatrix(light_pos);
   osg::endEditCP(light_beacon_core, osg::Transform::MatrixFieldMask);
   
   osg::beginEditCP(mLightBeacon);
      mLightBeacon->setCore(light_beacon_core);
   osg::endEditCP(mLightBeacon);
   
   // Setup light node
   osg::addRefCP(mLightNode);
   osg::beginEditCP(mLightNode);
      mLightNode->setCore(light_core);
      mLightNode->addChild(mLightBeacon);
   osg::endEditCP(mLightNode);
   
   osg::beginEditCP(light_core);
      light_core->setAmbient   (.9, .8, .8, 1);
      light_core->setDiffuse   ( 0.6,  0.6,  0.6, 1);
      light_core->setSpecular  ( 1,  1,  1, 1);
      light_core->setDirection ( 0,  0,  1);
      light_core->setBeacon    (mLightNode);
   osg::endEditCP(light_core);
   
   // --- Setup Scene -- //
   // add the loaded scene to the light node, so that it is lit by the light
   osg::addRefCP(mModelRoot);
   osg::beginEditCP(mLightNode);
      mLightNode->addChild(mModelRoot);
   osg::endEditCP(mLightNode);
   
   // create the root part of the scene
   mRootNode = OSG::Node::create();
   mSceneScale = OSG::Transform::create();
   mSceneRoot = OSG::Node::create();
   mSceneTransform = OSG::Transform::create();

   /*
   float scene_scale = 1.0f/gadget::PositionUnitConversion::ConvertToFeet;    // Scene uses feet as units

   osg::Matrix scene_scale_mat;
   scene_scale_mat.setScale(scene_scale);

   // Set the scale for the scene
   osg::beginEditCP(mSceneScale);
      mSceneScale->setMatrix(scene_scale_mat);
   osg::endEditCP(mSceneScale);
   */

   // Set the root node
   osg::beginEditCP(mRootNode);
      mRootNode->setCore(mSceneScale);
      mRootNode->addChild(mSceneRoot);
   osg::endEditCP(mRootNode);
   
   osg::beginEditCP(mSceneRoot);
      mSceneRoot->setCore(mSceneTransform);
      mSceneRoot->addChild(mLightNode);
   osg::endEditCP(mSceneRoot);
   
   // --- Add scene to root node --- //
   vprDEBUG(vprDBG_ALL, vprDBG_CRITICAL_LVL) << "OpenSGNav::initScene finished\n" << vprDEBUG_FLUSH;
}

