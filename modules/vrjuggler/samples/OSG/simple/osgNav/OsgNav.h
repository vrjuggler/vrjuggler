/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2007 by Iowa State University
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

#ifndef _OSG_NAV_
#define _OSG_NAV_

#include <vrj/vrjConfig.h>

#include <iostream>
#include <iomanip>
#include <math.h>

#include <gadget/Type/PositionInterface.h>
#include <gadget/Type/AnalogInterface.h>
#include <gadget/Type/DigitalInterface.h>

//OSG  includes
#include <osg/Matrix>
#include <osg/Transform>
#include <osg/MatrixTransform>

#include <osgUtil/SceneView>

#ifdef TWEEK_HAVE_CXX
#include <tweek/CORBA/CorbaManager.h>
#endif

#include "nav.h"

#include <vrj/Draw/OSG/App.h>

/**
 * Demonstration Open Scene Graph application class.
 */
class OsgNav : public vrj::osg::App
{
public:
   OsgNav(vrj::Kernel* kern, int& argc, char** argv);

   virtual ~OsgNav()
   {
      /* Do nothing. */ ;
   }

   /**
    * Execute any initialization needed before the API is started.
    *
    * This is called once before OSG is initialized.
    */
   virtual void initScene();

   void myInit();

   void initTweek(int& argc, char* argv[]);

   virtual osg::Group* getScene()
   {
      return mRootNode.get();
   }

   virtual void configSceneView(osgUtil::SceneView* newSceneViewer)
   {
      vrj::osg::App::configSceneView(newSceneViewer);

      newSceneViewer->getLight()->setAmbient(osg::Vec4(0.3f,0.3f,0.3f,1.0f));
      newSceneViewer->getLight()->setDiffuse(osg::Vec4(0.9f,0.9f,0.9f,1.0f));
      newSceneViewer->getLight()->setSpecular(osg::Vec4(1.0f,1.0f,1.0f,1.0f));
   }

   void bufferPreDraw();

   // ----- Drawing Loop Functions ------
   //
   //  The drawing loop will look similar to this:
   //
   //  while (drawing)
   //  {
   //        preFrame();
   //       <Application Data Syncronization>
   //        latePreFrame();
   //       draw();
   //        intraFrame();     // Drawing is happening while here
   //       sync();
   //        postFrame();      // Drawing is now done
   //
   //       UpdateTrackers();
   //  }
   //------------------------------------

   /**
    * Function called after tracker update but before start of drawing.
    *
    * Called once before every frame.
    */
   virtual void preFrame();

   /** Function called after ApplicationData syncronization but before draw() */
   virtual void latePreFrame();
   
   /**
    * Function called after drawing has been triggered but BEFORE it
    * completes.
    *
    * Called once during each frame.
    */
   virtual void intraFrame()
   {
      // Put your intra frame computations here.
   }

   /**
    * Function called before updating trackers but after the frame is drawn.
    *
    * Called once after every frame.
    */
   virtual void postFrame()
   {
      // Put your post frame computations here.
   }

   void setModelFileName(std::string filename)
   {
      mFileToLoad = filename;
   }

private:
   osg::ref_ptr<osg::Group>           mRootNode;
   osg::ref_ptr<osg::Group>           mNoNav;
   osg::ref_ptr<osg::MatrixTransform> mNavTrans;
   osg::ref_ptr<osg::MatrixTransform> mModelTrans;
   osg::ref_ptr<osg::Node>            mModel;

   OsgNavigator  mNavigator;       /**< Navigation class */

   std::string mFileToLoad;

   /** Time of the start of the last preframe */
   vpr::Interval mLastPreFrameTime;

#ifdef TWEEK_HAVE_CXX
   tweek::CorbaManager mCorbaManager;
#endif

public:
   gadget::PositionInterface  mWand;     /**< the wand */
   gadget::PositionInterface  mHead;     /**< the head */
   gadget::DigitalInterface   mButton0;
   gadget::DigitalInterface   mButton1;
   gadget::DigitalInterface   mButton2;
   gadget::DigitalInterface   mButton3;
   gadget::DigitalInterface   mButton4;
   gadget::DigitalInterface   mButton5;
};


#endif
