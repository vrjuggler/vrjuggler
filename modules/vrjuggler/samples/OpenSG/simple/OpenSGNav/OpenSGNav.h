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

#ifndef _VRJ_OPENSG_NAV_H_
#define _VRJ_OPENSG_NAV_H_


/*-----------------------------OpenSG includes--------------------------------*/
#include <OpenSG/OSGGL.h>
#include <OpenSG/OSGNode.h>
#include <OpenSG/OSGTransform.h>
/*----------------------------------------------------------------------------*/

#include <iostream>
#include <iomanip>

/*-----------------------------Juggler includes-------------------------------*/
#include <vrj/vrjConfig.h>
#include <vrj/Draw/OpenGL/App.h>
#include <vrj/Draw/OpenSG/App.h>

#include <gmtl/Matrix.h>
#include <gmtl/MatrixOps.h>
#include <gmtl/Generate.h>

#include <gadget/Type/Position/PositionUnitConversion.h>
#include <gadget/Type/PositionInterface.h>
#include <gadget/Type/AnalogInterface.h>
#include <gadget/Type/DigitalInterface.h>

#include <vrj/Draw/OpenGL/ContextData.h>



class OpenSGNav : public vrj::opensg::App
{
public:
   OpenSGNav(vrj::Kernel* kern)
      : vrj::opensg::App(kern)
      , mFileToLoad("")
      , mVelocity(0.0f)
   {
      std::cout << "OpenSGNav::OpenSGNav() called\n";
   }

   virtual ~OpenSGNav()
   {
      std::cout << "OpenSGNav::~OpenSGNav() called\n";
   }

   /** Handles any initialization needed before API. */
   virtual void init();

   /** Initialize the scene graph. */
   virtual void initScene();

   /** Returns the scene root for this application. */
   virtual OSG::NodePtr getScene()
   {
      return mSceneRoot;
   }

   virtual void draw();

   virtual void contextInit();

   virtual void preFrame();

   virtual float getDrawScaleFactor()
   {
      return gadget::PositionUnitConversion::ConvertToFeet;
   }

   virtual void exit();

   void setModelFileName(std::string filename)
   {
      std::cout << "[OpenSGNav::setModelFileName()] Set filename '"
                << filename << "'\n";
      mFileToLoad = filename;
   }

private:
   void initGLState();

private:
   std::string mFileToLoad;      /**< Filename of the file to load */

   //   mSceneRoot:[mSceneTransform]
   //         |
   //   mLightNode:[DirectionalLight]
   //         |
   //   mLightBeacon:[Transform]
   //         |
   //     mModelRoot
   OSG::NodeRefPtr        mSceneRoot;       /**< The root of the scene */
   OSG::TransformRefPtr   mSceneTransform;  /**< Transform core */
   OSG::NodeRefPtr        mModelRoot;       /**< Root of the loaded model */

   OSG::NodeRefPtr  mLightNode;       /**< Light node to use */
   OSG::NodeRefPtr  mLightBeacon;     /**< A beacon for the light */

public:
   gadget::PositionInterface  mWandPos;     /**< The position of the wand */
   gadget::DigitalInterface   mButton0;     /**< Wand button 0 */
   gadget::DigitalInterface   mButton1;     /**< Wand button 1 */
   gadget::DigitalInterface   mButton2;     /**< Wand button 2 */

   float mVelocity;
};


#endif
