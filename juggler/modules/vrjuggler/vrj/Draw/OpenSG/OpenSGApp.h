/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2002 by Iowa State University
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
/** @todo Clear out the headers that aren't actually needed */
#include <OpenSG/OSGBaseTypes.h>
#include <OpenSG/OSGWindow.h>
#include <OpenSG/OSGDrawAction.h>
#include <OpenSG/OSGRenderAction.h>
#include <OpenSG/OSGNode.h>
#include <OpenSG/OSGWindow.h>
#include <OpenSG/OSGImageForeground.h>
#include <OpenSG/OSGTransform.h>
#include <OpenSG/OSGGeometry.h>
#include <OpenSG/OSGGeoPropPtrs.h>
#include <OpenSG/OSGSimpleMaterial.h>
#include <OpenSG/OSGMatrixCamera.h>
#include <OpenSG/OSGDirectionalLight.h>
#include <OpenSG/OSGGroup.h>
#include <OpenSG/OSGPassiveWindow.h>

#include <OpenSG/OSGMatrixUtility.h>
#include <OpenSG/OSGConfig.h>
#include <OpenSG/OSGBaseFunctions.h>
#include <OpenSG/OSGBaseTypes.h>
#include <OpenSG/OSGImageFileType.h>
#include <OpenSG/OSGSolidBackground.h>
#include <OpenSG/OSGViewport.h>
#include <OpenSG/OSGLogoData.h>
#include <OpenSG/OSGThread.h>
#include <OpenSG/OSGMPBase.h>
#include <OpenSG/OSGGeometry.h>

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
      OSG::MatrixCameraPtr       mCamera;
      OSG::TransformPtr          mCameraCartTransform;   /**< Do I really need all this cart stuff */
      OSG::NodePtr               mCameraCartNode;

      bool                       mContextThreadInitialized;
      OSG::ExternalThread*       mOsgThread;
   };

public:
    OpenSGApp(vrj::Kernel* kern)
       : GlApp(kern)
    {;}

    virtual ~OpenSGApp()
    {;}

    /** Scene initialization function
    * User code for initializing the OpenSG scene should be placed here
    */
    virtual void initScene() = 0;

    /** Get the OpenSG Scene root
    */
    virtual OSG::NodePtr getSceneRoot() = 0;

    // Initialize
    // Make sure to call initScene if you override this function
    virtual void init();

    /** Initialize OpenSG.
    * User overrided functions MUST call OpenSGApp::apiInit()
    */
    virtual void apiInit();

    /** GL Drawing functions
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
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   }


protected:
     vrj::GlContextData<context_data>  mContextData;  /**< OpenSG context data */
};

// Handle any initialization needed before API
inline void OpenSGApp::init()
{
   vprDEBUG(vprDBG_ALL,0) << "OpenSGNav::init: Called.\n" << vprDEBUG_FLUSH;

   // XXX: Complete initialization
   // if(!osgInitAlreadyCalled())
   OSG::osgInit(0,0);                  // Binds to primordial thread

   // Work around to disable display list caching
   OSG::FieldContainerPtr pProto = OSG::Geometry::getClassType().getPrototype();
   OSG::GeometryPtr pGeoProto = OSG::GeometryPtr::dcast(pProto);

   if(pGeoProto != OSG::NullFC)
   {
       pGeoProto->setDlistCache(false);
   }

   OSG_MAIN_ASPECT_ID = OSG::Thread::getAspect();   // Gets the base aspect id to use
}

inline void OpenSGApp::apiInit()
{
   vprDEBUG(vprDBG_ALL,0) << "OpenSGNav::initAPI: Called.\n" << vprDEBUG_FLUSH;

   this->initScene();
   vprASSERT(getSceneRoot().getCPtr() != NULL);    // I don't know if this is even valid
}

/** Called once per context at context creation */
inline void OpenSGApp::contextInit()
{
   vprDEBUG(vprDBG_ALL,0) << "OpenSGNav::contextInit: Called.\n" << vprDEBUG_FLUSH;

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
   c_data->mWin = OSG::PassiveWindow::create();
   c_data->mCameraCartNode = OSG::Node::create();
   c_data->mCameraCartTransform = OSG::Transform::create();
   c_data->mCamera = OSG::MatrixCamera::create();

   // Setup the cart, set internal transform node
   OSG::beginEditCP(c_data->mCameraCartNode);
      c_data->mCameraCartNode->setCore(c_data->mCameraCartTransform);
   OSG::endEditCP(c_data->mCameraCartNode);

   // Setup the camera
   OSG::beginEditCP(c_data->mCamera);
      c_data->mCamera->setBeacon (c_data->mCameraCartNode);
      c_data->mCamera->setNear   (0.1);
      c_data->mCamera->setFar    (10000);
   OSG::endEditCP(c_data->mCamera);

   // Could actually make one of these per thread instead of context.
   c_data->mRenderAction = OSG::RenderAction::create();
   c_data->mRenderAction->setAutoFrustum(false);         // Turn off auto frustum
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
   context_data* c_data = &(*mContextData);

   vrj::GlDrawManager* drawMan = dynamic_cast<vrj::GlDrawManager*> ( this->getDrawManager() );
   vprASSERT(drawMan != NULL);
   vrj::GlUserData* userData = drawMan->currentUserData();

   // Copy the matrix
   vrj::Projection* project = userData->getProjection();
   vrj::Frustum vrj_frustum = userData->getProjection()->mFrustum;

   float* vj_proj_view_mat = project->mViewMat.mData;
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
      c_data->mCamera->setProjectionMatrix( full_view_matrix );         // Set view matrix
      c_data->mCamera->setModelviewMatrix( OSG::Matrix::identity() );   // Set projection matrix
   OSG::endEditCP(c_data->mCamera);

   // --- Trigger the draw --- //
   c_data->mRenderAction->setWindow(c_data->mWin.getCPtr());
   c_data->mRenderAction->setViewport(NULL);                   // Don't know why a matrix camera needs a viewport
   c_data->mRenderAction->setCamera(c_data->mCamera.getCPtr());
   c_data->mRenderAction->setFrustumCulling(false);    // Turn off culling for now because I don't yet trust the frustum setup

   // Push the matrix so that drawing after this is not affected by the scene graph
   glPushMatrix();
      c_data->mRenderAction->apply(getSceneRoot());       // Actually do the rendering
   glPopMatrix();
}

};

#endif
