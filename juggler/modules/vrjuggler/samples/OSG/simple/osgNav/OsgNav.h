/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998, 1999, 2000 by Iowa State University
 *
 * Original Authors:
 *  Allen Bierbaum, Christopher Just,
 *  Patrick Hartling, Kevin Meinert,
 *  Carolina Cruz-Neira, Albert Baker
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
//using namespace std;

#include <iomanip>

#include <vrj/Draw/OGL/GlApp.h>
#include <vrj/Math/Matrix.h>

#include <vrj/Input/Type/PosInterface.h>
#include <vrj/Input/Type/AnalogInterface.h>
#include <vrj/Input/Type/DigitalInterface.h>

//OSG  includes
#include <osg/Geode>
#include <osg/GeoSet>
#include <osg/Material>
#include <osg/Vec3>
#include <osg/Matrix>
#include <osg/Transform>

#include <osgUtil/SceneView>
#include <osgUtil/DisplayListVisitor>

#include <osgDB/ReadFile>
#include <math.h>

#include <vrj/Draw/OSG/OsgApp.h>

using namespace std;


//----------------------------------------------------
//: Demonstration OpenGL application class
//
// This application simply renders a box on the wand
//----------------------------------------------------
class OsgNav : public vrj::OsgApp
{
public:
    OsgNav(vrj::Kernel* kern)
        : vrj::OsgApp(kern)         // Initialize base class
    {
      mFileToLoad = std::string("");
   }

    virtual ~OsgNav (void) {
        /* Do nothing. */ ;
    }

    // Execute any initialization needed before the API is started<BR><BR>
    //
    // This is called once before OGL is initialized
    virtual void initScene()
    {
        //DeBugOut = Debug::getStream(0, 3, true, true, 1, true);

        //cout << "---------- App:init() ---------------" << endl;
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

    //: Function that is called immediately after a new OGL context is created
    // Initialize GL state here. Also used to create context specific information<BR>
    //
    // This is called once for each display (twice for each stereo display)
    virtual void contextInit()
    {
        initGLState();       // Initialize the GL state information. (lights, shading, etc)
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
    void initGLState();

private:
   osg::Vec3 mPos;
    double posInc;

    osg::Group* mRootNode;
    osg::Group* mNoNav;
    osg::Transform* mNavTrans;
   osg::Transform* mModelTrans;
   osg::Transform* mModelBaseXform;

   osg::Node* mModel;

    float speed;

   std::string mFileToLoad;

public:
    vrj::PosInterface    mWand;     // the Wand
    vrj::PosInterface    mHead;     // the head
    vrj::DigitalInterface   mButton0;
    vrj::DigitalInterface   mButton1;
    vrj::DigitalInterface   mButton2;
    vrj::DigitalInterface   mButton3;
    vrj::DigitalInterface   mButton4;
    vrj::DigitalInterface   mButton5;
};


#endif
