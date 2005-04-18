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

#include <OpenSGViewer.h>

#include <OpenSG/OSGMatrix.h>
#include <OpenSG/OSGSimpleGeometry.h>
#include <OpenSG/OSGDirectionalLight.h>
#include <OpenSG/OSGSceneFileHandler.h>

#include <Common/HoverSelectionMethod.h>


// Handle any initialization needed before API
void OpenSGViewer::init()
{
   vrj::OpenSGApp::init();      // Call base class init
   mUser->init();

   // Setup the selection and manipulation methods
   mSelectionMethod = new HoverSelectionMethod;

   mSelectionMethod->setViewer(this);
   mSelectionMethod->init();
}

/** Called once per context at context creation */
void OpenSGViewer::contextInit()
{
   vrj::OpenSGApp::contextInit();
   initGLState();
}

void OpenSGViewer::draw()
{
   // Call parent class first to render the scene graph
   vrj::OpenSGApp::draw();
}

void OpenSGViewer::preFrame()
{
   // Update the user (and viewplatform with navigation strategy)
   mUser->update();

   OSG::beginEditCP(mSceneTransform); 
      mSceneTransform->getMatrix().setValue( mUser->viewPlatform().getTransform_platMvirt().getData() );
   OSG::endEditCP(mSceneTransform);

   // Update manipulation
   mSelectionMethod->updateSelection();
}

/** Initialize GL state. Hold over from regular OGL apps */
void OpenSGViewer::initGLState()
{
    // OpenSG does not handle this yet, being smart about it is not 
    // that trivial...
    
    glEnable(GL_NORMALIZE);
}

void OpenSGViewer::initScene(void)
{
   vprDEBUG(vprDBG_ALL, vprDBG_CRITICAL_LVL) << "OpenSGViewer::initScene: Called.\n" << vprDEBUG_FLUSH;
   
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
      std::cout << "OpenSGViewere::initScene: No model specified!!! Loading torus.\n";
      mWorldRoot = OSG::makeTorus(.5, 2, 16, 16);      
   }
   else
   {
      std::cout << "OpenSGViewer::initScene: Loading [" << mFileToLoad.c_str() << "]\n";
      mWorldRoot = OSG::SceneFileHandler::the().read((OSG::Char8 *)(mFileToLoad.c_str()));
   }

   mSelectableNodes.push_back(mWorldRoot);

   // --- Light setup --- //
   // - Add directional light for scene
   // - Create a beacon for it and connect to that beacon
   mLightNode   = OSG::Node::create();
   mLightBeacon = OSG::Node::create();
   OSG::DirectionalLightPtr light_core = OSG::DirectionalLight::create();
   OSG::TransformPtr light_beacon_core = OSG::Transform::create();
   
   // Setup light beacon
   OSG::Matrix light_pos;
   light_pos.setTransform(OSG::Vec3f( 2.0f, 5.0f, 4.0f));
   
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
      light_core->setAmbient   (.9, .8, .8, 1);
      light_core->setDiffuse   ( 0.6,  0.6,  0.6, 1);
      light_core->setSpecular  ( 1,  1,  1, 1);
      light_core->setDirection ( 0,  0,  1);
      light_core->setBeacon    (mLightNode);
   OSG::endEditCP(light_core);
   
   // --- Setup Scene -- //
   // add the loaded scene to the light node, so that it is lit by the light
   OSG::addRefCP(mWorldRoot);
   OSG::beginEditCP(mLightNode);
      mLightNode->addChild(mWorldRoot);
   OSG::endEditCP(mLightNode);
   
   // create the root.
   mSceneRoot = OSG::Node::create();
   mSceneTransform = OSG::Transform::create();
   
   OSG::beginEditCP(mSceneRoot);
      mSceneRoot->setCore(mSceneTransform);
      mSceneRoot->addChild(mLightNode);
   OSG::endEditCP(mSceneRoot);
   
   // --- Add scene to root node --- //
   vprDEBUG(vprDBG_ALL, vprDBG_CRITICAL_LVL) << "OpenSGViewer::initScene finished\n" << vprDEBUG_FLUSH;
}

//! NOTE: Inherited from jccl::ConfigElementHandler
bool OpenSGViewer::configAdd(jccl::ConfigElementPtr element)
{
   return true;
}

