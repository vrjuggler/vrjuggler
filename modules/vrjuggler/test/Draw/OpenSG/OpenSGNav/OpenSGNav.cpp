/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2002 by Iowa State University
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

#include <OpenSG/OSGConfig.h>
#include <OpenSG/OSGBaseFunctions.h>
#include <OpenSG/OSGBaseTypes.h>
#include <OpenSG/OSGImageFileType.h>
#include <OpenSG/OSGSolidBackground.h>
#include <OpenSG/OSGViewport.h>
#include <OpenSG/OSGLogoData.h>
#include <OpenSG/OSGThread.h>
#include <OpenSG/OSGMPBase.h>
#include <OpenSG/OSGMatrixUtility.h>
#include <OpenSG/OSGGeometry.h>
#include <OpenSG/OSGSimpleGeometry.h>
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

   // --- DEBUGGING CODE ------------- //
   // - Draws a little white triangle on the screen - //
   //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
     // Set material color
   float onyx_red = 59.0f/255.0f;
   float onyx_blue = 57.0f/255.0f;
   GLfloat mat_ambient[] = { onyx_red, 0.0, onyx_blue, 1.0};
   GLfloat mat_diffuse[] = { onyx_red, 0.0, onyx_blue, 1.0};
   GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0};
   GLfloat mat_shininess[] = { 50.0};

   glMaterialfv( GL_FRONT, GL_AMBIENT, mat_ambient );
   glMaterialfv( GL_FRONT,  GL_DIFFUSE, mat_diffuse );
   glMaterialfv( GL_FRONT, GL_SPECULAR, mat_specular );
   glMaterialfv( GL_FRONT,  GL_SHININESS, mat_shininess );
   //glColor4f(1.0, 1.0, 1.0, 1.0);

   //glLoadIdentity();
   glPushMatrix();
      glTranslatef(-1.5f,0.0f,6.0f);
       glBegin(GL_TRIANGLES);
         glVertex3f(0.0f,1.0f,0.0f);
         glVertex3f(-1.0f,-1.0f,0.0f);
         glVertex3f(1.0f,-1.0f,0.0f);
       glEnd();
   glPopMatrix();
}

void OpenSGNav::preFrame()
{
   const float inc_vel(0.005f);
   const float max_vel(0.5f);

   // Update velocity
   if(mButton0->getData() == gadget::Digital::ON)
   {
     velocity += inc_vel;
     cout << "vel: " << velocity << std::endl;
   }
   else if(velocity > 0)
   {
      cout << "vel: " << velocity << std::endl;
      velocity -= inc_vel;
   }

   // Restrict range
   if(velocity < 0)
      { velocity = 0;}
   if(velocity > max_vel)
      { velocity = max_vel; }

   if(mButton1->getData() == gadget::Digital::ON)
   { velocity = 0; }

   // Travel in model
   // -- Get wand info -- //
   gmtl::Matrix44f wandMatrix;
   wandMatrix = (*mWand->getData());      // Get the wand matrix
   gmtl::Vec3f direction;
   gmtl::Vec3f Zdir = gmtl::Vec3f(0.0f, 0.0f, velocity);
   direction = wandMatrix * Zdir;

   osg::Matrix osg_trans_mat( osg::Matrix::identity());
   osg_trans_mat.setTranslate(direction[0], direction[1], direction[2]);
   //mNavTrans->preMult(osg::Matrix::translate(direction[0], direction[1], direction[2]));

   osg::beginEditCP(mSceneTransform);
      mSceneTransform->getMatrix().multLeft(osg_trans_mat);
   osg::endEditCP(mSceneTransform);
}

/** Initialize GL state. Hold over from regular OGL apps */
void OpenSGNav::initGLState()
{
    std::cout << "OpenSGNav::initGLState called\n";
    /*
    glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );
    glPixelStorei( GL_PACK_ALIGNMENT, 1 );
    */

    GLfloat light0_ambient[] = { .2,  .2,  .2,  1.0};
   GLfloat light0_diffuse[] = { 1.0,  1.0,  1.0,  1.0};
   GLfloat light0_specular[] = { 1.0,  1.0,  1.0,  1.0};
   GLfloat light0_position[] = {6.0, 6,0, 6.0, 1.0};

   glLightfv(GL_LIGHT0, GL_AMBIENT,  light0_ambient);
   glLightfv(GL_LIGHT0, GL_DIFFUSE,  light0_diffuse);
   glLightfv(GL_LIGHT0, GL_SPECULAR,  light0_specular);
   glLightfv(GL_LIGHT0, GL_POSITION,  light0_position);

   glEnable(GL_DEPTH_TEST);
   glEnable(GL_NORMALIZE);
   //glEnable(GL_LIGHTING);
   //glEnable(GL_LIGHT0);
   //glEnable(GL_COLOR_MATERIAL);
   glShadeModel(GL_SMOOTH);

   glMatrixMode(GL_MODELVIEW);

}

void OpenSGNav::initScene(void)
{
   vprDEBUG(vprDBG_ALL,0) << "OpenSGNav::initScene: Called.\n" << vprDEBUG_FLUSH;

   // --- Initialize wand --- //
   std::string wand("VJWand");
   std::string but0("VJButton0");
   std::string but1("VJButton1");
   std::string but2("VJButton2");
   mWand.init(wand);
   mButton0.init(but0);
   mButton1.init(but1);
   mButton2.init(but2);

   // Load a graph
   if (mFileToLoad ==  std::string("none"))
    {
        std::cout << "OpenSGNav::initScene beforetorusmake\n";
        mModelRoot = OSG::makeTorus(.5, 2, 16, 16);
        std::cout << "OpenSGNav::myInit aftertorusmake\n";
    }
    else
    {
       std::cout << "OpenSGNav::initScene: Loading [" << mFileToLoad.c_str() << "]\n";
       mModelRoot = OSG::SceneFileHandler::the().read((OSG::Char8 *)(mFileToLoad.c_str()));
    }
    std::cout << "OpenSGNav::initScene before RenderAction::create()\n";

    // -- light node --> light cart
    //the camera and light beacon
    mLightNode = OSG::Node::create();
    mLightCart = OSG::Node::create();
    OSG::DirectionalLightPtr light_core = OSG::DirectionalLight::create();
    OSG::TransformPtr cart_core = OSG::Transform::create();

    osg::Matrix light_pos;
    light_pos.setTransform(osg::Vec3f( 2.0f, 5.0f, 4.0f));

   osg::beginEditCP(cart_core, OSG::Transform::MatrixFieldMask);
   {
       cart_core->setMatrix(light_pos);
   }
   osg::endEditCP(cart_core, osg::Transform::MatrixFieldMask);

    // Setup light cart node
    osg::beginEditCP(mLightCart);
      mLightCart->setCore(cart_core);
    osg::endEditCP(mLightCart);

    osg::addRefCP(mLightNode);
    osg::beginEditCP(mLightNode);
      mLightNode->setCore(light_core);
      mLightNode->addChild(mLightCart);
    osg::endEditCP(mLightNode);

    osg::beginEditCP(light_core);
      light_core->setAmbient   (.9, .8, .8, 1);
      light_core->setDiffuse   ( 0.6,  0.6,  0.6, 1);
      light_core->setSpecular  ( 1,  1,  1, 1);
      light_core->setDirection ( 0,  0,  1);
      light_core->setBeacon    (mLightNode);
    osg::endEditCP(light_core);

    // --- Setup Scene -- //
    osg::addRefCP(mModelRoot);
    osg::beginEditCP(mModelRoot);
      mModelRoot->addChild(mLightNode);
    osg::endEditCP(mModelRoot);

    mSceneRoot = OSG::Node::create();
    mSceneTransform = OSG::Transform::create();

    osg::beginEditCP(mSceneRoot);
       mSceneRoot->setCore(mSceneTransform);
       mSceneRoot->addChild(mModelRoot);
    osg::endEditCP(mSceneRoot);

    // --- Add scene to root node --- //
    std::cout << "OpenSGNav::initScene finished\n";
}

