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

#include <OsgNav.h>

#include <gmtl/Vec.h>
#include <gmtl/Coord.h>
#include <gmtl/Xforms.h>
#include <gmtl/Math.h>

void OsgNav::preFrame()
{

//vprDEBUG(vprDBG_ALL,0) << "------- preFrame ------\n" << vprDEBUG_FLUSH;

   // -- Get wand info -- //
   gmtl::Matrix44f wandMatrix = mWand->getData();      // Get the wand matrix

   osg::Matrix osgWandMat;
   osgWandMat.set(wandMatrix.getData());


   if ( mButton0->getData() == gadget::Digital::ON )
   {
      // Speed up
      speed = speed + inc;
      std::cout << "speed: " << speed << std::endl;
   }
   if ( mButton1->getData() == gadget::Digital::ON )
   {
      // Stop
      speed = 0.0;
   }
   if ( mButton2->getData() == gadget::Digital::ON )
   {
      // Slow down
      speed = speed - inc;
      std::cout << "speed: " << speed << std::endl;
   }


   //Navigation
   gmtl::Vec3f direction;
   gmtl::Vec3f Zdir = gmtl::Vec3f(0.0f, 0.0f, speed);
   gmtl::xform(direction, wandMatrix, Zdir);
   mNavTrans->preMult(osg::Matrix::translate(direction[0], direction[1], direction[2]));
}

void OsgNav::bufferPreDraw()
{
   glClearColor(0.0, 0.0, 0.0, 0.0);
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void OsgNav::myInit()
{
   //
   //          /-- mNoNav
   // mRootNode
   //         \-- mNavTrans -- mModelTrans -- mModel

   //The top level nodes of the tree
   mRootNode = new osg::Group();
   mNoNav    = new osg::Group();
   mNavTrans = new osg::MatrixTransform();

   mRootNode->addChild( mNoNav );
   mRootNode->addChild( mNavTrans );

   //Load the model
   std::cout << "Attempting to load file: " << mFileToLoad << "... ";
   mModel = osgDB::readNodeFile(mFileToLoad);
   std::cout << "done." << std::endl;

   // Transform node for the model
   mModelTrans  = new osg::MatrixTransform();
   //This can be used if the model orientation needs to change
   mModelTrans->preMult( osg::Matrix::rotate( gmtl::Math::deg2Rad( -90.0f ), 1.0f, 0.0f, 0.0f) );
   
   // Add model to the transform
   mModelTrans->addChild(mModel);
   // Add the transform to the tree
   mNavTrans->addChild( mModelTrans );

   // run optimization over the scene graph
   osgUtil::Optimizer optimzer;
   optimzer.optimize(mRootNode);

   // traverse the scene graph setting up all osg::GeoSet's so they will use
   // OpenGL display lists.
   //osgUtil::DisplayListVisitor dlv(osgUtil::DisplayListVisitor::SWITCH_ON_DISPLAY_LISTS);
   //mRootNode->accept(dlv);
   
   // The initial speed for navigation is set to zero
   speed = 0.0f;
   // How much we should accelerat each frame the button is held
   inc = 0.005f;
}
