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

#include <OsgNav.h>

#include <osg/Math>
#include <osg/Geode>
#include <osg/Material>
#include <osg/Vec3>
#include <osgUtil/Optimizer>
#include <osgDB/ReadFile>

#include <gmtl/Vec.h>
#include <gmtl/Coord.h>
#include <gmtl/Xforms.h>
#include <gmtl/Math.h>

#ifdef USE_REMOTE_NAV
#   include <remotenav/Subject/RemoteNavSubjectImpl.h>
#endif

OsgNav::OsgNav(vrj::Kernel* kern, int& argc, char** argv) : vrj::OsgApp(kern)
{
   mFileToLoad = std::string("");

#ifdef USE_REMOTE_NAV   
   // Initialize tweek and register RemoteNavSubjectImpl with the nameserver.
   initTweek( argc, argv );
#endif
}

void OsgNav::latePreFrame()
{
   gmtl::Matrix44f world_transform;
   gmtl::invertFull(world_transform, mNavigator.getCurPos());
   // Update the scene graph
   osg::Matrix osg_current_matrix;
   osg_current_matrix.set(world_transform.getData());
   mNavTrans->setMatrix(osg_current_matrix);
}

void OsgNav::preFrame()
{
   vpr::Interval cur_time = mWand->getTimeStamp();
   vpr::Interval diff_time(cur_time-mLastPreFrameTime);
   if (mLastPreFrameTime.getBaseVal() >= cur_time.getBaseVal())
   {  diff_time.secf(0.0f); }

   float time_delta = diff_time.secf();

   // Cluster debug code.
   // std::cout << "CLUSTER Delta: " << diff_time.getBaseVal() << std::endl;
   // std::cout << "CLUSTER Current: " << cur_time.getBaseVal() << "Last: " << mLastTimeStamp.getBaseVal() << "\n" << std::endl;

   mLastPreFrameTime = cur_time;

   //vprDEBUG(vprDBG_ALL, vprDBG_CRITICAL_LVL) << "------- preFrame ------\n" << vprDEBUG_FLUSH;

   // Get wand data
   gmtl::Matrix44f wandMatrix = mWand->getData();      // Get the wand matrix

   // If we are pressing button 1 then translate in the direction the wand is
   // pointing.
   if ( mButton0->getData() == gadget::Digital::ON )
   {
      gmtl::Vec3f direction;
      gmtl::Vec3f Zdir = gmtl::Vec3f(0.0f, 0.0f, -10.0f);
      gmtl::xform(direction, wandMatrix, Zdir);

      mNavigator.setVelocity(direction);
   }  // Make sure to reset the velocity when we stop pressing the button.
   else if ( mButton0->getData() == gadget::Digital::TOGGLE_OFF)
   {
      mNavigator.setVelocity(gmtl::Vec3f(0.0, 0.0, 0.0));
   }

   if ( mButton1->getData() == gadget::Digital::TOGGLE_ON)
   {
      std::cout << "Cur Pos: " << mNavigator.getCurPos() << std::endl;
   }

   // If we are pressing button 2 then rotate in the direction the wand is
   // pointing.
   if ( mButton2->getData() == gadget::Digital::ON )
   {
      gmtl::EulerAngleXYZf euler( 0.0f, gmtl::makeYRot(mWand->getData()), 0.0f );// Only allow Yaw (rot y)
      gmtl::Matrix44f rot_mat = gmtl::makeRot<gmtl::Matrix44f>( euler );
      mNavigator.setRotationalVelocity(rot_mat);
   } // Make sure to reset the rotational velocity when we stop pressing the button.
   else if(mButton2->getData() == gadget::Digital::TOGGLE_OFF)
   {
      mNavigator.setRotationalVelocity(gmtl::Matrix44f());
   }
   // Update the navigation using the time delta between
   mNavigator.update(time_delta);
}

void OsgNav::bufferPreDraw()
{
   glClearColor(0.0, 0.0, 0.0, 0.0);
   glClear(GL_COLOR_BUFFER_BIT);
}

void OsgNav::initScene()
{
   //DeBugOut = Debug::getStream(0, 3, true, true, 1, true);

   //std::cout << "---------- App:init() ---------------" << std::endl;
   // Initialize devices
   const std::string wand("VJWand");
   const std::string vjhead("VJHead");
   const std::string but0("VJButton0");
   const std::string but1("VJButton1");
   const std::string but2("VJButton2");
   const std::string but3("VJButton3");
   const std::string but4("VJButton4");
   const std::string but5("VJButton5");

   mWand.init(wand);
   mHead.init(vjhead);
   mButton0.init(but0);
   mButton1.init(but1);
   mButton2.init(but2);
   mButton3.init(but3);
   mButton4.init(but4);
   mButton5.init(but5);

   myInit();
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


   mNavigator.init();

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

   if(NULL == mModel)
   {
      std::cout << "ERROR: Could not load file: " << mFileToLoad << std::endl;
   }
   else
   {
      // Add model to the transform
      mModelTrans->addChild(mModel);
   }
   
   // Add the transform to the tree
   mNavTrans->addChild( mModelTrans );

   // run optimization over the scene graph
   osgUtil::Optimizer optimizer;
   optimizer.optimize(mRootNode);

   // traverse the scene graph setting up all osg::GeoSet's so they will use
   // OpenGL display lists.
   //osgUtil::DisplayListVisitor dlv(osgUtil::DisplayListVisitor::SWITCH_ON_DISPLAY_LISTS);
   //mRootNode->accept(dlv);
}

void OsgNav::initTweek(int& argc, char* argv[])
{
#ifdef USE_REMOTE_NAV
   std::cout << "\n\nSTARTING TWEEK INITIALIZATION!!\n\n" << std::flush;

   std::string name_context( "OSG_REMOTE_NAV_");

   try
   {
      if ( mCorbaManager.init(name_context, argc, argv).success() )
      {
         try
         {
            if ( mCorbaManager.createSubjectManager().success() )
            {
               RemoteNavSubjectImpl* remote_nav_interface =
                  new RemoteNavSubjectImpl(&mNavigator);

               mCorbaManager.getSubjectManager()->addInfoItem("OsgNav","RemoteNav");
               try
               {
                  mCorbaManager.getSubjectManager()->registerSubject(remote_nav_interface,
                                                                     "RemoteNavSubject");
               }
               catch (...)
               {
                  std::cout
                     << "Failed to register subject\n" << std::flush;
               }
            }
         }
         catch (CORBA::Exception& ex)
         {
            std::cout
               << "Caught an unknown CORBA exception when trying to register!\n"
               << std::flush;
         }
      }

   }
   catch (...)
   {
      std::cout
         << "Caught an unknown exception while initializing Tweek!\n" << std::flush;
   }

   std::cout << "\n\nDONE WITH TWEEK INITIALIZATION!!\n\n" << std::flush;
#else
   boost::ignore_unused_variable_warning(argc);
   boost::ignore_unused_variable_warning(argv);
#endif
}

