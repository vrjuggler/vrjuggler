/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2010 by Iowa State University
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
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

#ifndef _OSG_NAV_GRAB
#define _OSG_NAV_GRAB

#include <vrj/vrjConfig.h>

#include <iostream>
#include <iomanip>
#include <math.h>

#include <gadget/Type/Position/PositionUnitConversion.h>

#include <gadget/Type/PositionInterface.h>
#include <gadget/Type/AnalogInterface.h>
#include <gadget/Type/DigitalInterface.h>

//OSG  includes
#include <osg/Matrix>
#include <osg/Transform>
#include <osg/MatrixTransform>

#include <osgUtil/SceneView>

#include "nav.h"

#include <vrj/Draw/OSG/App.h>

/**
 * Demonstration Open Scene Graph application class
 */
class OsgNavGrab : public vrj::osg::App
{
public:
   OsgNavGrab(vrj::Kernel* kern, int& argc, char** argv);

   virtual ~OsgNavGrab()
   {
      /* Do nothing. */ ;
   }

   // Execute any initialization needed before the API is started<BR><BR>
   //
   // This is called once before OGL is initialized
   virtual void initScene();

   void myInit();

   virtual osg::Group* getScene()
   {
      return mRootNode;
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

   //: Function called after tracker update but before start of drawing<BR><BR>
   //
   // called once before every frame.
   virtual void preFrame();

   // Function called after ApplicationData syncronization but before draw()
   virtual void latePreFrame();
   
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

   /**
    * Returns the scale factor for this application.  For this sample, we
    * are handling everything in feet.
    */
   virtual float getDrawScaleFactor()
   {
      return gadget::PositionUnitConversion::ConvertToFeet;
   }

private:
   osg::Group*             mRootNode;
   osg::Group*             mNoNav;
   osg::MatrixTransform*   mNavTrans;
   osg::MatrixTransform*   mModelTrans;
   osg::Node*              mModel;

   OsgNavigator  mNavigator;       /** Navigation class */

   std::string mFileToLoad;

   vpr::Interval           mLastPreFrameTime;   /**< Time of the start of the last preframe */

   struct GrabObject
   {
      GrabObject(osg::Node* node, osg::MatrixTransform* xform,
            const osg::Matrix& pos)
         : xformNode(node)
         , xformCore(xform)
         , homePos(pos)
      {
         ;
      }

      /** The transform node (parent) for the grabbable object. */
      osg::Node* xformNode;

      /** The transform core for the grabbable object. */
      osg::MatrixTransform* xformCore;

      /**
      * The original transformation for the grabbable object.  This is used
      * for resetting the scene.
      */
      const osg::Matrix homePos;

      osg::Matrix xformStart;
      osg::Matrix xformSaved;
   };

   /**
    * Creates a GrabObject instance for the given model that is to be located
    * initially at the given position.
    *
    * @param model      The model (i.e., geometric object) that will be
    *                   grabbable.
    * @param modelPos   The starting position for the given model.
    *
    * @return A new GrabObject instance.  It is the responsibility of the
    *         caller to release the memory.
    */
   GrabObject* makeGrabbable(osg::Node* model, osg::MatrixTransform* modelPos);

   void updateGrabbing(const gmtl::Matrix44f& wandMatrix);

   /** @name Grabbed object management */
   //@{
   std::vector<GrabObject*> mObjects;
   GrabObject*              mIntersectedObj;
   GrabObject*              mGrabbedObj;
   //@}

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
