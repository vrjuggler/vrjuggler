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

#ifndef _VRJ_OPENSG_NAV_GRAB_H_
#define _VRJ_OPENSG_NAV_GRAB_H_

#include <string>
#include <vector>

#include <OpenSG/OSGGeoPropPtrs.h>
#include <OpenSG/OSGNode.h>
#include <OpenSG/OSGSimpleMaterial.h>
#include <OpenSG/OSGTransform.h>
#include <OpenSG/OSGGL.h>

#include <gmtl/Matrix.h>

#include <gadget/Type/Position/PositionUnitConversion.h>
#include <gadget/Type/PositionInterface.h>
#include <gadget/Type/DigitalInterface.h>

#include <vrj/Draw/OpenSG/OpenSGApp.h>


class OpenSGNavGrab : public vrj::OpenSGApp
{
public:
   OpenSGNavGrab(vrj::Kernel* kern);

   virtual ~OpenSGNavGrab();

   /**
    * Adds the given file name to the list of files to be loaded.
    *
    * @pre initScene() has not been invoked by the VR Juggler kernel yet.
    */
   void addModelToLoad(const std::string& fileName)
   {
      mFilesToLoad.push_back(fileName);
   }

   /**
    * Replaces the current list of files to load with the given list.
    *
    * @pre initScene() has not been invoked by the VR Juggler kernel yet.
    */
   void setModelsToLoad(const std::vector<std::string>& fileNames);

   /** Handles any initialization needed before API. */
   virtual void init();
        
   /** Initialize the scene graph. */
   virtual void initScene();

   /** Returns the scene root for this application. */
   virtual OSG::NodePtr getScene()
   {
      return mSceneRoot;
   }

   virtual void contextInit();

   virtual void preFrame();

   /**
    * Returns the scale factor for this application.  For this sample, we
    * are handling everything in feet.
    */
   virtual float getDrawScaleFactor()
   {
      return gadget::PositionUnitConversion::ConvertToFeet;
   }

   virtual void reset();

private:
   struct GrabObject
   {
      GrabObject(OSG::NodePtr node, OSG::TransformPtr xform,
                 const OSG::Matrix& pos)
         : xformNode(node)
         , xformCore(xform)
         , homePos(pos)
      {
         ;
      }

      /** The transform node (parent) for the grabbable object. */
      OSG::NodePtr xformNode;

      /** The transform core for the grabbable object. */
      OSG::TransformPtr xformCore;

      /**
       * The original transformation for the grabbable object.  This is used
       * for resetting the scene.
       */
      const OSG::Matrix homePos;
   };

   /** Initialize GL state.  Hold-over from regular OpenGL apps. */
   void initGLState();

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
   GrabObject* makeGrabbable(OSG::NodePtr model, const OSG::Matrix& modelPos);

   void updateGrabbing(const gmtl::Matrix44f& wandMatrix);

   void updateNavigation(const gmtl::Matrix44f& wandMatrix);

   void initHighlight();

   void updateHighlight();

   std::vector<std::string> mFilesToLoad; /**< Name(s) of the file(s) to load */

   //   mSceneRoot:[mSceneTransform]
   //         |
   //   mLightNode:[DirectionalLight]
   //         |
   //   mLightBeacon:[Transform]
   //         |
   //   mModelRoot:[mModelGroup]
   OSG::NodePtr        mSceneRoot;       /**< The root of the scene */
   OSG::TransformPtr   mSceneTransform;  /**< Transform core */
   OSG::NodePtr        mModelRoot;       /**< Root of the loaded model */
   OSG::GroupPtr       mModelGroup;

   OSG::NodePtr  mLightNode;       /**< Light node to use */
   OSG::NodePtr  mLightBeacon;     /**< A beacon for the light */

   gadget::PositionInterface  mWandPos;     /**< The position of the wand */
   gadget::DigitalInterface   mButton0;     /**< Wand button 0 */
   gadget::DigitalInterface   mButton1;     /**< Wand button 1 */
   gadget::DigitalInterface   mButton2;     /**< Wand button 2 */

   float mVelocity;

   /** @name Grabbed object management */
   //@{
   std::vector<GrabObject*> mObjects;
   GrabObject*              mIntersectedObj;
   GrabObject*              mGrabbedObj;
   //@}

   /** @name Highlight node management */
   //@{
   OSG::Color3f           mIntersectColor;
   OSG::Color3f           mGrabColor;

   OSG::NodePtr           mHighlight;        /**< The node to highlight */
   OSG::NodePtr           mHighlightNode;    /**< The highlight node */
   OSG::GeoPositions3fPtr mHighlightPoints;  /**< Points for mHighlightNode */

   OSG::SimpleMaterialPtr mHighlightMaterial;
   //@}
};


#endif
