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

#ifndef _VJ_OPENSG_APP__
#define _VJ_OPENSG_APP__

#include <vrj/vrjConfig.h>
#include <iostream>
#include <iomanip>

#include <vrj/Draw/OGL/GlContextData.h>

/*-----------------------------OpenSG includes--------------------------------*/
#include <OpenSG/OSGConfig.h>
#include <OpenSG/OSGThread.h>
#include <OpenSG/OSGRenderAction.h>
#include <OpenSG/OSGMatrixCamera.h>
#include <OpenSG/OSGPassiveWindow.h>
#include <OpenSG/OSGPassiveViewport.h>
#include <OpenSG/OSGPassiveBackground.h>
#include <OpenSG/OSGMatrixUtility.h>

/*----------------------------------------------------------------------------*/

#include <vrj/Draw/OGL/GlApp.h>

namespace
{
   OSG::UInt32 OSG_MAIN_ASPECT_ID;     /**< Local scope variable */
};

namespace vrj
{

class OpenSGApp : public vrj::GlApp
{
public:
   struct context_data
   {
      context_data()
        : mRenderAction(NULL),
          mContextThreadInitialized(false),
          mOsgThread(NULL)
      {;}

      OSG::RenderAction*         mRenderAction;    /**< The render action for the scene */
      OSG::PassiveWindowPtr      mWin;             /**< passive window to render with (the context) */
      OSG::PassiveViewportPtr    mViewport;        /**< passive viewport to render with (the context) */
      OSG::PassiveBackgroundPtr  mBackground;      /**< passive background to render with (the context) */
      OSG::MatrixCameraPtr       mCamera;

      bool                       mContextThreadInitialized;
      OSG::ExternalThread*       mOsgThread;
   };

public:
   OpenSGApp(vrj::Kernel* kern)
      : GlApp(kern)
   {;}
   
   virtual ~OpenSGApp()
   {;}
   
   /** Scene initialization function.
   * User code for initializing the OpenSG scene should be placed here.
   */
   virtual void initScene() = 0;
   
   /** Get the OpenSG Scene root.
   * @return NodePtr to the root of the scene to render.
   */
   virtual OSG::NodePtr getSceneRoot() = 0;
   
   /** Initialize OpenSG for drawing.
   * If overridden, MUST call this method
   */
   virtual void init();
   
   /** Initialize OpenSG.
   * Make sure to call initScene if you override this function
   * User overrided functions MUST call OpenSGApp::apiInit()
   */
   virtual void apiInit();
   
   /** GL Drawing functions.
   * If user code overrides these functions, it MUST call this method
   */
   //@{
   virtual void contextInit();
   virtual void contextPreDraw();
   virtual void contextPostDraw();
   virtual void draw();
   //@}
   
   /** Called once per frame, per buffer (basically context)
   *  need so that we can use subviewports */
   virtual void bufferPreDraw()
   {
      glClearColor(0.0, 0.0, 0.0, 0.0);
      glClear(GL_COLOR_BUFFER_BIT);
   }

protected:
   vrj::GlContextData<context_data>  mContextData;  /**< OpenSG context data */
};

// Handle any initialization needed before API
inline void OpenSGApp::init()
{
   vprDEBUG(vprDBG_ALL, vprDBG_VERB_LVL) << "OpenSGApp::init: Called.\n" << vprDEBUG_FLUSH;

   GlApp::init();

   // XXX: Complete initialization
   // if(!osgInitAlreadyCalled())
   OSG::osgInit(0,0);                  // Binds to primordial thread

#if 0
   // Work around to disable display list caching
   OSG::FieldContainerPtr pProto = OSG::Geometry::getClassType().getPrototype();
   OSG::GeometryPtr pGeoProto = OSG::GeometryPtr::dcast(pProto);

   if(pGeoProto != OSG::NullFC)
   {
       pGeoProto->setDlistCache(false);
   }
#endif

   OSG_MAIN_ASPECT_ID = OSG::Thread::getAspect();   // Gets the base aspect id to use
}

inline void OpenSGApp::apiInit()
{
   vprDEBUG(vprDBG_ALL, vprDBG_VERB_LVL) << "OpenSGApp::initAPI: Called.\n" << vprDEBUG_FLUSH;

   this->initScene();
   vprASSERT(getSceneRoot() != OSG::NullFC);
}

/** Called once per context at context creation */
inline void OpenSGApp::contextInit()
{
   vprDEBUG(vprDBG_ALL, vprDBG_VERB_LVL) << "OpenSGApp::contextInit: Called.\n" << vprDEBUG_FLUSH;

   context_data* c_data = &(*mContextData);  // Context specific data. Should be one copy per context

   // Check for thread initialized
   // This will only happen for the first initialized context per pipe
   if(!c_data->mContextThreadInitialized)
   {
      c_data->mContextThreadInitialized = true;

      char thread_name_buffer[255];
      sprintf(thread_name_buffer, "vprThread:%d", vpr::Thread::self()->getTID());
      c_data->mOsgThread = OSG::ExternalThread::get(thread_name_buffer);
      if(!(c_data->mOsgThread->isInitialized()))
      {
         c_data->mOsgThread->initialize(OSG_MAIN_ASPECT_ID);     // XXX: In future this might need to be different thread
      }
   }

   // Allocate OpenSG stuff
   c_data->mWin        = OSG::PassiveWindow::create();
   c_data->mViewport   = OSG::PassiveViewport::create();
   c_data->mBackground = OSG::PassiveBackground::create();
   c_data->mCamera     = OSG::MatrixCamera::create();

   // Setup the viewport
   OSG::beginEditCP(c_data->mViewport);
   	c_data->mViewport->setLeft(0);
   	c_data->mViewport->setRight(1);
   	c_data->mViewport->setBottom(0);
   	c_data->mViewport->setTop(1);
   	c_data->mViewport->setCamera(c_data->mCamera);
   	c_data->mViewport->setBackground(c_data->mBackground);
   OSG::endEditCP  (c_data->mViewport);
   
   // Setup the Window
   OSG::beginEditCP(c_data->mWin);
   	c_data->mWin->addPort(c_data->mViewport);
   OSG::endEditCP  (c_data->mWin);

   // Setup the camera
   OSG::beginEditCP(c_data->mCamera);
      c_data->mCamera->setNear(0.1);
      c_data->mCamera->setFar (10000);
   OSG::endEditCP(c_data->mCamera);

   // Could actually make one of these per thread instead of context.
   c_data->mRenderAction = OSG::RenderAction::create();
   // c_data->mRenderAction->setAutoFrustum(false);         // Turn off auto frustum

   // Initialize OpenSG's OpenGL state

   c_data->mWin->init();
}

inline void OpenSGApp::contextPreDraw()
{
   context_data* c_data = &(*mContextData);
   c_data->mWin->frameInit();
}

inline void OpenSGApp::contextPostDraw()
{
   context_data* c_data = &(*mContextData);
   c_data->mWin->frameExit();
}

inline void OpenSGApp::draw()
{
   glClear(GL_DEPTH_BUFFER_BIT);

   context_data* c_data = &(*mContextData);

   vrj::GlDrawManager* drawMan = dynamic_cast<vrj::GlDrawManager*> ( this->getDrawManager() );
   vprASSERT(drawMan != NULL);
   vrj::GlUserData* userData = drawMan->currentUserData();

   // Copy the matrix
   vrj::Projection* project = userData->getProjection();
   vrj::Frustum vrj_frustum = userData->getProjection()->getFrustum();

   const float* vj_proj_view_mat = project->getViewMatrix().mData;
   OSG::Matrix frustum_matrix, view_xform_mat;
   view_xform_mat.setValue(vj_proj_view_mat);

   OSG::MatrixFrustum(frustum_matrix, vrj_frustum[vrj::Frustum::VJ_LEFT], vrj_frustum[vrj::Frustum::VJ_RIGHT],
                                      vrj_frustum[vrj::Frustum::VJ_BOTTOM], vrj_frustum[vrj::Frustum::VJ_TOP],
                                      vrj_frustum[vrj::Frustum::VJ_NEAR], vrj_frustum[vrj::Frustum::VJ_FAR]);

   OSG::Matrix full_view_matrix = frustum_matrix;
   full_view_matrix.mult(view_xform_mat);   // Compute complete projection matrix

   // Setup the camera
   OSG::beginEditCP(c_data->mCamera);
      c_data->mCamera->setNear(vrj_frustum[vrj::Frustum::VJ_NEAR]);
      c_data->mCamera->setFar(vrj_frustum[vrj::Frustum::VJ_FAR]);
      c_data->mCamera->setProjectionMatrix( frustum_matrix );  // Set projection matrix
      c_data->mCamera->setModelviewMatrix( view_xform_mat );   // Set modelview matrix
   OSG::endEditCP(c_data->mCamera);

   // Setup the viewport
   OSG::beginEditCP(c_data->mViewport);
   	c_data->mViewport->setRoot(getSceneRoot());
   OSG::endEditCP  (c_data->mViewport);

   // --- Trigger the draw --- //  

   // Push the matrix so that drawing after this is not affected by the scene graph
   glMatrixMode(GL_PROJECTION);
   glPushMatrix();
   glMatrixMode(GL_MODELVIEW);
   glPushMatrix();   
	c_data->mWin->render(c_data->mRenderAction);
   glPopMatrix();
   glMatrixMode(GL_PROJECTION);
   glPopMatrix();
   glMatrixMode(GL_MODELVIEW);
   
   FINFO(("Frame done on Window %lx.\n", c_data->mWin.getCPtr() ));
}

};

#endif
