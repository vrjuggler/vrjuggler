/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998, 1999, 2000, 2001, 2002
 *   by Iowa State University
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

#ifndef _VJ_OSG_APP_
#define _VJ_OSG_APP_

#include <vjConfig.h>

#include <Kernel/GL/vjGlApp.h>
#include <Kernel/vjCameraProjection.h>
#include <Kernel/GL/vjGlContextData.h>

#include <osg/Vec3>
#include <osg/Matrix>
#include <osg/Transform>
#include <osg/Group>

#include <osgUtil/SceneView>


//-----------------------------------------------------------
//: vjOsgApp: Encapulates an open scene graph application.
//
// PURPOSE:
//
// See also: vjGlApp
//
//-------------------------------------------------------------
//!PUBLIC_API:
class  vjOsgApp : public vjGlApp
{
public:
   vjOsgApp(vjKernel* kern) : vjGlApp(kern)
   {;}

   virtual ~vjOsgApp() {;}

   virtual void initScene() = 0;
   virtual osg::Group* getScene() = 0;

   /**
   * Configure newly created scene viewers
   * This is called immediately after a new scene viewer is created for a context
   * This is the place to configure application background colors and other viewer
   * specific information
   */
   virtual void configSceneView(osgUtil::SceneView* newSceneViewer)
   {
      newSceneViewer->setDefaults();
      newSceneViewer->setBackgroundColor( osg::Vec4(0.0f, 0.0f, 0.0f, 0.0f) );
   }

   //: Function to draw the scene
   // Override this function with the user draw routine
   //!PRE: OpenGL state has correct transformation and buffer selected
   //!POST: The current scene has been drawn
   virtual void draw();

   // Initialize
   // Make sure to call initScene if you override this function
   virtual void init()
   {
      this->initScene();
   }

   //: Function that is called immediately after a new context is created
   //  Use this function to create context specific data structures.
   //  i.e. Display lists, Texture objects, etc.
   //! PRE: The ogl context has been set to the new context
   //! POST: Application has completed in initialization the user wishes
   virtual void contextInit();

   //: Function that is called immediately before a context is closed
   // Use the function to clean up any context data structures
   virtual void contextClose()
   {;}

   //: Function that is called upon entry into the context for a draw
   //! PRE: The ogl context has been set to the context for drawing
   //! POST: User application has executed any commands that need
   //+   to only be executed once per context, per frame
   //! NOTE: This function can be used for things that need to happen
   //+       every frame, but only once per context
   //+  <br> Ex: Dynamically Create display lists
   virtual void contextPreDraw()
   {;}  

   //: Function that is called at the beginning of the drawing of each pipe
   //!PRE: The library is preparing to render all windows on a given pipe
   //!POST: Any pre-pipe user calls have been done
   //!NOTE: Currently the OGL context is not set when this function is called <br>
   //+      This is a TEST function.  USE AT YOUR OWN RISK!!!
   virtual void pipePreDraw()
   {;}

protected:
   vjGlContextData< osgUtil::SceneView* > sceneViewer;
   vjGlContextData< bool > first;
};

inline void vjOsgApp::contextInit()
{
   unsigned int unique_context_id = vjGlDrawManager::instance()->getCurrentContext();

   // --- Create new context specific scene viewer -- //
   osgUtil::SceneView* new_sv = new osgUtil::SceneView;
   this->configSceneView(new_sv);            // Configure the new viewer
   new_sv->getState()->setContextID(unique_context_id);
   (*sceneViewer) = new_sv;
   *first = true;
}

inline void vjOsgApp::draw()
{
   osgUtil::SceneView (*sv)(NULL);
   sv = (*sceneViewer);    // Get context specific scene viewer
   vjASSERT( sv != NULL);   

   // XXX: The following is a hack to deal with a bug in OSG 0.8.45 that
   // causes problems with stereo rendering.
   if ( true == *first )
   {
      // Add the tree to the scene viewer
      sv->setSceneData(getScene());
      sv->setCalcNearFar(false);

      //Take care of the view port (this does not work)
      GLint view[4];
      glGetIntegerv(GL_VIEWPORT, view);   //Get the view port that juggler sets
      sv->setViewport(view[0],view[1],view[2],view[3]);
      *first = false;
   }

   //Get the view matrix and the frustrum form the draw manager
   vjGlDrawManager* drawMan = dynamic_cast<vjGlDrawManager*> ( this->getDrawManager() );
   vjASSERT(drawMan != NULL);
   vjGlUserData* userData = drawMan->currentUserData();
	
   // Copy the matrix
   vjProjection* project = userData->getProjection();
   float* vj_proj_view_mat = project->mViewMat.getFloatPtr();
   osg::Matrix osgMat;
   osgMat.set(	vj_proj_view_mat );

   //Get the frustrum
   vjFrustum frustum = project->mFrustum;

   //Reset the camera
   osg::Camera* the_cam = sv->getCamera();
   vjCameraProjection (*sim_cam_proj)(NULL);   

   switch(project->getType())
   {
   case vjProjection::SURFACE:
      the_cam->home();
      the_cam->setAdjustAspectRatioMode(osg::Camera::ADJUST_NONE);      // Tell it not to adjust the aspect ratio at all
   
      //Set the frustrum (this is set with the matrix below)
      //float near_val = frustum[Frustum::VJ_NEAR];
      the_cam->setFrustum(frustum[vjFrustum::VJ_LEFT],   frustum[vjFrustum::VJ_RIGHT],
                          frustum[vjFrustum::VJ_BOTTOM], frustum[vjFrustum::VJ_TOP],
                          frustum[vjFrustum::VJ_NEAR],   frustum[vjFrustum::VJ_FAR]);
      break;

   case vjProjection::SIM:
      sim_cam_proj = dynamic_cast<vjCameraProjection*>(project);
      vjASSERT(sim_cam_proj != NULL && "Trying to use non-camera projection for simulator");
      the_cam->home();
      the_cam->setAdjustAspectRatioMode(osg::Camera::ADJUST_HORIZONTAL);
      //chan->setNearFar(frustum[Frustum::VJ_NEAR], frustum[Frustum::VJ_FAR]);
      //chan->setFOV(0.0f, cam_proj->mVertFOV);
      the_cam->setPerspective( sim_cam_proj->mVertFOV, sim_cam_proj->mAspectRatio,
                               frustum[vjFrustum::VJ_NEAR], frustum[vjFrustum::VJ_FAR]);
      break;

   default:
      vjASSERT(false);
      break;
   }

   //Set the look at
   // NOTE: This is on the wrong stack !!!!
   the_cam->attachTransform(osg::Camera::MODEL_TO_EYE, &osgMat);
		
   //Draw the scene
   sv->app();
   sv->cull();
   sv->draw();
}

#endif

