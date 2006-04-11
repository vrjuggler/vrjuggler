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

#ifndef _OSG_NAV_
#define _OSG_NAV_

#include <vrj/vrjConfig.h>

#include <iostream>
#include <iomanip>

#include <vrj/Draw/OGL/GlApp.h>

#include <gadget/Type/PositionInterface.h>
#include <gadget/Type/AnalogInterface.h>
#include <gadget/Type/DigitalInterface.h>

//OSG  includes
#include <osg/Math>
#include <osg/Geode>
#include <osg/GeoSet>
#include <osg/Material>
#include <osg/Vec3>
#include <osg/Matrix>
#include <osg/Transform>
#include <osg/MatrixTransform>

#include <osgUtil/SceneView>
#include <osgUtil/Optimizer>
#include <osgUtil/DisplayListVisitor>

#include <osgDB/ReadFile>
#include <math.h>

#include <vrj/Draw/OSG/OsgApp.h>


/**
 * Demonstration Open Scene Graph application class
 */
class OsgNav : public vrj::OsgApp
{
public:
   OsgNav(vrj::Kernel* kern) : vrj::OsgApp(kern)         // Initialize base class
   {
      mFileToLoad = std::string("");
   }

   virtual ~OsgNav()
   {
      /* Do nothing. */ ;
   }

   // Execute any initialization needed before the API is started<BR><BR>
   //
   // This is called once before OGL is initialized
   virtual void initScene()
   {
      //DeBugOut = Debug::getStream(0, 3, true, true, 1, true);

      //std::cout << "---------- App:init() ---------------" << std::endl;
      // Initialize devices
      std::string wand("VJWand");
      std::string vjhead("VJHead");
      std::string but0("VJButton0");
      std::string but1("VJButton1");
      std::string but2("VJButton2");
      std::string but3("VJButton3");
      std::string but4("VJButton4");
      std::string but5("VJButton5");

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

   void myInit();

   virtual osg::Group* getScene()
   {
      return mRootNode;
   }

   virtual void configSceneView(osgUtil::SceneView* newSceneViewer)
   {
      newSceneViewer->setDefaults();
      newSceneViewer->setBackgroundColor( osg::Vec4(0.0f, 0.0f, 0.0f, 0.0f) );
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
   //       draw();
   //        intraFrame();     // Drawing is happening while here
   //       sync();
   //        postFrame();      // Drawing is now done
   //
   //       UpdateTrackers();
   //  }
   //------------------------------------

   //: Function called after tracker update but before start of drawing<BR><BR>
   //
   // called once before every frame.
   virtual void preFrame();

   //: Function called after drawing has been triggered but BEFORE it completes<BR><BR>
   //
   // called once during each frame
   virtual void intraFrame()
   {
      // Put your intra frame computations here.
   }

   //: Function called before updating trackers but after the frame is drawn<BR><BR>
   //
   // called once after every frame
   virtual void postFrame()
   {
      // Put your post frame computations here.
   }

   void setModelFileName(std::string filename)
   {
      mFileToLoad = filename;
   }

private:
   osg::Vec3 mPos;
   double posInc;

   osg::Group* mRootNode;
   osg::Group* mNoNav;
   osg::MatrixTransform* mNavTrans;
   osg::MatrixTransform* mModelTrans;
   osg::Node* mModel;

   float speed;
   float inc;

   std::string mFileToLoad;

public:
   gadget::PositionInterface  mWand;     // the Wand
   gadget::PositionInterface  mHead;     // the head
   gadget::DigitalInterface   mButton0;
   gadget::DigitalInterface   mButton1;
   gadget::DigitalInterface   mButton2;
   gadget::DigitalInterface   mButton3;
   gadget::DigitalInterface   mButton4;
   gadget::DigitalInterface   mButton5;
};


#endif
