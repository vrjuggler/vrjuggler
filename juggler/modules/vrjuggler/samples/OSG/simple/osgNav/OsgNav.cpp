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
#include "remotenav/Subject/RemoteNavSubjectImpl.h"

OsgNav::OsgNav(vrj::Kernel* kern, int& argc, char** argv) : vrj::OsgApp(kern)
{
   //
   //          /-- mNoNav
   // mRootNode
   //         \-- mNavTrans -- mModelTrans -- mModel

   //The top level nodes of the tree
   mRootNode = new osg::Group();
   mNoNav    = new osg::Group();
   mNavTrans = new osg::MatrixTransform();
   mFileToLoad = std::string("");
   
   // We need to create a new navigater before calling initTweek because we pass
   // it to the RemoteNavSubjectImpl.
   mNavigater = new OsgNavigater();
   
   //Initialize tweek, which registers a new RemoteNavSubjectImpl with the
   //nameserver.
   initTweek( argc, argv );
}

void OsgNav::preFrame()
{
   static vpr::Interval last_time;

   vpr::Interval cur_time = mWand->getTimeStamp();
   vpr::Interval diff_time(cur_time-last_time);
      
   float time_delta = diff_time.secf();

   // Cluster debug code.
   // std::cout << "CLUSTER Delta: " << diff_time.getBaseVal() << std::endl;
   // std::cout << "CLUSTER Current: " << cur_time.getBaseVal() << "Last: " << mLastTimeStamp.getBaseVal() << "\n" << std::endl;
   
   last_time = cur_time;
   

   //vprDEBUG(vprDBG_ALL, vprDBG_CRITICAL_LVL) << "------- preFrame ------\n" << vprDEBUG_FLUSH;

   // Get wand data 
   gmtl::Matrix44f wandMatrix = mWand->getData();      // Get the wand matrix

   // If we are pressing button 1 then translate in the direction the wand is
   // pointing.
   if ( mButton0->getData() == gadget::Digital::ON )
   {
      gmtl::Vec3f direction;
      gmtl::Vec3f Zdir = gmtl::Vec3f(0.0f, 0.0f, 10.0f);
      gmtl::xform(direction, wandMatrix, Zdir);

      mNavigater->setVelocity(direction); 
   } // Make sure to reset the velocity when we stop pressing the button.
   else if ( mButton0->getData() == gadget::Digital::TOGGLE_OFF)
   {
      mNavigater->setVelocity(gmtl::Vec3f(0.0, 0.0, 0.0)); 
   }
   
   // If we are pressing button 2 then rotate in the direction the wand is
   // pointing.
   if ( mButton2->getData() == gadget::Digital::ON )
   {
      mNavigater->setRotationalVelocity(mWand->getData());
   } // Make sure to reset the rotational velocity when we stop pressing the button.
   else if(mButton2->getData() == gadget::Digital::TOGGLE_OFF)
   {
      mNavigater->setRotationalVelocity(gmtl::Matrix44f());
   }
   
   // Update the navigation using the time delta between
   mNavigater->update(time_delta);

   // Update the scene graph
   osg::Matrix osg_current_matrix;
   osg_current_matrix.set(mNavigater->getCurPos().getData());
   mNavTrans->setMatrix(osg_current_matrix);
}

void OsgNav::bufferPreDraw()
{
   glClearColor(0.0, 0.0, 0.0, 0.0);
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void OsgNav::myInit()
{

   //vpr::GUID new_guid("d6be4359-e8cf-41fc-a72b-a5b4f3f29aa2");
   //std::string hostname = "crash";
   //mMyData.init(new_guid, hostname);
   
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

void OsgNav::initTweek( int& argc, char* argv[] )
{
#ifdef TWEEK_HAVE_CXX
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
                  new RemoteNavSubjectImpl(mNavigater);

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

