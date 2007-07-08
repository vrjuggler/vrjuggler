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

#ifndef _VJ_OPENSG_APP_H_
#define _VJ_OPENSG_APP_H_

#include <vrj/vrjConfig.h>
#include <sstream>

#include <vpr/Util/Debug.h>
#include <vrj/Draw/OpenGL/ContextData.h>

/*-----------------------------OpenSG includes--------------------------------*/
#include <OpenSG/OSGConfig.h>
#include <OpenSG/OSGThread.h>
#include <OpenSG/OSGMatrixCamera.h>
#include <OpenSG/OSGPassiveWindow.h>
#include <OpenSG/OSGPassiveViewport.h>
#include <OpenSG/OSGPassiveBackground.h>
#include <OpenSG/OSGMatrixUtility.h>

#if OSG_MAJOR_VERSION >= 2
#include <OpenSG/OSGRenderTraversalAction.h>
#else
#include <OpenSG/OSGRenderAction.h>
#endif

/*----------------------------------------------------------------------------*/

#include <vrj/Draw/OpenGL/App.h>


namespace vrj
{

namespace opensg
{

/** \class vrj::opensg::App OpenSGApp.h vrj/Draw/OpenSG/OpenSGApp.h
 *
 * Base type for OpenSG application objects.
 *
 * @note This class was renamed from vrj::OpenSGApp in VR Juggler 2.3.11.
 */
class App : public vrj::opengl::App
{
public:
   struct context_data
   {
      context_data()
        : mRenderAction(NULL)
        , mContextThreadInitialized(false)
        , mOsgThread(NULL)
      {
         ;
      }
#if OSG_MAJOR_VERSION >= 2
      OSG::RenderTraversalAction* mRenderAction;    /**< The render trav action for the scene */
#else
      OSG::RenderAction*         mRenderAction;    /**< The render action for the scene */
#endif

      OSG::PassiveWindowPtr      mWin;             /**< passive window to render with (the context) */
      OSG::PassiveViewportPtr    mViewport;        /**< passive viewport to render with (the context) */
      OSG::PassiveBackgroundPtr  mBackground;      /**< passive background to render with (the context) */
      OSG::MatrixCameraPtr       mCamera;

      bool                       mContextThreadInitialized;
      OSG::ExternalThread*       mOsgThread;
   };

public:
   App(vrj::Kernel* kern = NULL)
      : vrj::opengl::App(kern)
   {
      ;
   }

   virtual ~App()
   {
      ;
   }

   /**
    * Scene initialization function.
    * User code for initializing the OpenSG scene should be placed here.
    */
   virtual void initScene() = 0;

   /**
    * Get the OpenSG Scene root.
    * @return NodePtr to the root of the scene to render.
    */
   virtual OSG::NodePtr getScene() = 0;

   /**
    * Initializes OpenSG for drawing.
    * If overridden, the overriding method MUST call this method.
    */
   virtual void init();

   /**
    * Initializes OpenSG.
    * Make sure to call initScene if you override this function.
    * If a derived class overrides this method, the overriding function MUST
    * call vrj::opensg::App::apiInit().
    */
   virtual void apiInit();

   /**
    * Shuts down OpenSG.  If overridden, the overriding method must call this
    * method.
    */
   virtual void exit();

   /**
    * For OpenSG 1.x, this method does nothing. For OpenSG 2, it ensures that
    * all scene graph changes have been committed prior to rendering. If
    * overridden, the overriding method must call this method. It is
    * recommended that the invocation of this method be the last step in the
    * override of latePreFrame().
    *
    * @since 2.3.8
    */
   virtual void latePreFrame()
   {
#if OSG_MAJOR_VERSION >= 2
      OSG::commitChanges();
#endif
   }

   /**
    * OpenGL Drawing functions.
    * If user code overrides these functions, the overriding functions MUST
    * call these methods.
    */
   //@{
   virtual void contextInit();
   virtual void contextPreDraw();
   virtual void contextPostDraw();
   virtual void draw();
   //@}

   /**
    * Called once per frame, per buffer (basically context).
    * This is needed so that we can use subviewports.
    */
   virtual void bufferPreDraw()
   {
      glClearColor(0.0, 0.0, 0.0, 0.0);
      glClear(GL_COLOR_BUFFER_BIT);
   }

protected:
   /** OpenSG context-specific data. */
   vrj::opengl::ContextData<context_data> mContextData;

   OSG::UInt32 OSG_MAIN_ASPECT_ID;
};

// Handle any initialization needed before API
inline void App::init()
{
   vprDEBUG_OutputGuard(vprDBG_ALL, vprDBG_VERB_LVL,
                        "vrj::opensg::App::init() entered.\n",
                        "vrj::opensg::App::init() exited.\n");

   vrj::opengl::App::init();

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

#if OSG_MAJOR_VERSION >= 2
   OSG_MAIN_ASPECT_ID = OSG::Thread::getCurrentAspect();   // Gets the base aspect id to use
#else
   OSG_MAIN_ASPECT_ID = OSG::Thread::getAspect();   // Gets the base aspect id to use
#endif
}

inline void App::apiInit()
{
   vprDEBUG_OutputGuard(vprDBG_ALL, vprDBG_VERB_LVL,
                        "vrj::opensg::App::apiInit() entered.\n",
                        "vrj::opensg::App::apiInit() exited.\n");

   this->initScene();
   vprASSERT(getScene() != OSG::NullFC);
}

inline void App::exit()
{
   vprDEBUG_OutputGuard(vprDBG_ALL, vprDBG_VERB_LVL,
                        "vrj::opensg::App::exit() entered.\n",
                        "vrj::opensg::App::exit() exited.\n");

   OSG::osgExit();
}

/** Called once per context at context creation */
inline void App::contextInit()
{
   vprDEBUG_OutputGuard(vprDBG_ALL, vprDBG_VERB_LVL,
                        "vrj::opensg::App::contextInit() entered.\n",
                        "vrj::opensg::App::contextInit() exited.\n");

   context_data* c_data = &(*mContextData);  // Context specific data. Should be one copy per context

   // Check for thread initialized
   // This will only happen for the first initialized context per pipe
   if(!c_data->mContextThreadInitialized)
   {
      c_data->mContextThreadInitialized = true;

      std::ostringstream thread_name;
      thread_name << "vprThread:" << vpr::Thread::self()->getTID();
      c_data->mOsgThread = OSG::ExternalThread::get(thread_name.str().c_str());

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

#if OSG_MAJOR_VERSION < 2
   // Create container pointer editors for the field containers modified
   // below.
   OSG::CPEditor vp_editor(c_data->mViewport);
   OSG::CPEditor win_editor(c_data->mWin);
   OSG::CPEditor camera_editor(c_data->mCamera);
#endif

   // Set up the viewport.
   c_data->mViewport->setLeft(0);
   c_data->mViewport->setRight(1);
   c_data->mViewport->setBottom(0);
   c_data->mViewport->setTop(1);
   c_data->mViewport->setCamera(c_data->mCamera);
   c_data->mViewport->setBackground(c_data->mBackground);

   // Set up the Window.
   c_data->mWin->addPort(c_data->mViewport);

   // Set up the camera.
   c_data->mCamera->setNear(0.1);
   c_data->mCamera->setFar (10000);

   // Could actually make one of these per thread instead of context.
#if OSG_MAJOR_VERSION >= 2
   c_data->mRenderAction = OSG::RenderTraversalAction::create();
#else
   c_data->mRenderAction = OSG::RenderAction::create();
#endif
   // c_data->mRenderAction->setAutoFrustum(false);         // Turn off auto frustum

   // Initialize OpenSG's OpenGL state

   c_data->mWin->init();

#if OSG_MAJOR_VERSION >= 2
   OSG::commitChanges();
#endif
}

inline void App::contextPreDraw()
{
   context_data* c_data = &(*mContextData);
   c_data->mWin->frameInit();
}

inline void App::contextPostDraw()
{
   context_data* c_data = &(*mContextData);
   c_data->mWin->frameExit();
}

inline void App::draw()
{
   vprDEBUG_OutputGuard(vprDBG_ALL, vprDBG_HVERB_LVL,
                        "vrj::opensg::App::draw() entered.\n",
                        "vrj::opensg::App::draw() exited.\n");

   glClear(GL_DEPTH_BUFFER_BIT);
   glPushAttrib(GL_ALL_ATTRIB_BITS);

   context_data* c_data = &(*mContextData);

   vrj::opengl::DrawManager* draw_man =
      dynamic_cast<vrj::opengl::DrawManager*>(this->getDrawManager());
   vprASSERT(draw_man != NULL);
   vrj::opengl::UserData* userData = draw_man->currentUserData();

   // Copy the matrix
   vrj::ProjectionPtr project = userData->getProjection();
   const vrj::Frustum& vrj_frustum = userData->getProjection()->getFrustum();

   const float* vj_proj_view_mat = project->getViewMatrix().mData;
   OSG::Matrix frustum_matrix, view_xform_mat;
   view_xform_mat.setValue(vj_proj_view_mat);

   OSG::MatrixFrustum(frustum_matrix,
                      vrj_frustum[vrj::Frustum::VJ_LEFT],
                      vrj_frustum[vrj::Frustum::VJ_RIGHT],
                      vrj_frustum[vrj::Frustum::VJ_BOTTOM],
                      vrj_frustum[vrj::Frustum::VJ_TOP],
                      vrj_frustum[vrj::Frustum::VJ_NEAR],
                      vrj_frustum[vrj::Frustum::VJ_FAR]);

   OSG::Matrix full_view_matrix = frustum_matrix;
   full_view_matrix.mult(view_xform_mat);   // Compute complete projection matrix

#if OSG_MAJOR_VERSION < 2
   // Create container pointer editors for the field containers modified
   // below.
   OSG::CPEditor camera_editor(c_data->mCamera,
                               OSG::MatrixCamera::NearFieldMask |
                                  OSG::MatrixCamera::FarFieldMask |
                                  OSG::MatrixCamera::ProjectionMatrixFieldMask |
                                  OSG::MatrixCamera::ModelviewMatrixFieldMask);
   OSG::CPEditor vp_editor(c_data->mViewport, OSG::Viewport::RootFieldMask);
#endif

   // Set up the camera.
   c_data->mCamera->setNear(vrj_frustum[vrj::Frustum::VJ_NEAR]);
   c_data->mCamera->setFar(vrj_frustum[vrj::Frustum::VJ_FAR]);
   c_data->mCamera->setProjectionMatrix(frustum_matrix);  // Set projection matrix
   c_data->mCamera->setModelviewMatrix(view_xform_mat);   // Set modelview matrix

   // Set up the viewport.
   c_data->mViewport->setRoot(getScene());

   // --- Trigger the draw --- //

   // Push the matrix so that drawing after this is not affected by the scene
   // graph.
   glMatrixMode(GL_PROJECTION);
   glPushMatrix();
   glMatrixMode(GL_MODELVIEW);
   glPushMatrix();
      c_data->mWin->render(c_data->mRenderAction);
   glPopMatrix();
   glMatrixMode(GL_PROJECTION);
   glPopMatrix();
   glMatrixMode(GL_MODELVIEW);

   glPopAttrib();    // Pop the attribute store
#if OSG_MAJOR_VERSION < 2
   FINFO(("Frame done on Window %lx.\n", c_data->mWin.getCPtr() ));
#endif
}

} // End of opensg namespace

} // End of vrj namespace


#endif
