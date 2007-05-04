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

#ifndef _VRJ_OSG_APP_H_
#define _VRJ_OSG_APP_H_

#include <vrj/vrjConfig.h>

#include <vpr/Sync/Mutex.h>
#include <vpr/Sync/Guard.h>

#include <gadget/Type/PositionInterface.h>

#include <vrj/Draw/OGL/GlApp.h>
#include <vrj/Draw/OGL/GlContextData.h>
#include <vrj/Draw/OGL/GlWindow.h>

#include <vrj/Display/CameraProjection.h>

#include <osg/Vec3>
#include <osg/Matrix>
#include <osg/Transform>
#include <osg/Group>

#include <osgUtil/SceneView>
#include <osgUtil/UpdateVisitor>


namespace vrj
{

/** \class OsgApp OsgApp.h vrj/Draw/OSG/OsgApp.h
 *
 * Encapsulates an Open Scene Graph (OSG) application.  This defines the base
 * class from which OSG-based application classes should be derived.  It makes
 * use of the OpenGL Draw Manager.
 *
 * @note This class makes use of gadget::PositionInterface internally and
 *       requires that a proxy or proxy alias named "VJHead" is defined in
 *       the VR Juggler configuration.
 *
 * @see vrj::GlApp
 */
class OsgApp : public GlApp
{
public:
   OsgApp(Kernel* kern = NULL)
      : GlApp(kern)
      , mFrameNumber(0)
   {
      ;
   }

   virtual ~OsgApp()
   {
      ;
   }

   /**
    * Initializes the scene.
    * Called by init() to initialize the scene.
    */
   virtual void initScene() = 0;

   /**
    * Gets the root of the scene to render.
    * Called each frame to get the current scene to render.
    */
   virtual osg::Group* getScene() = 0;

   /**
    * Returns the options to be passed to osgUtil::SceneView::setDefaults()
    * for each scene view that is configured. This is called by the default
    * implementation of vrj::OsgApp::configSceneView(). See
    * osgUtil::SceneView::Options for the available settings.
    *
    * @see configSceneView()
    *
    * @since 2.1.26
    */
   virtual osgUtil::SceneView::Options getSceneViewDefaults()
   {
      return osgUtil::SceneView::STANDARD_SETTINGS;
   }

   /**
    * Configures newly created scene viewers.
    * This is called immediately after a new scene viewer is created for a
    * context.  This is the place to configure application background colors
    * and other viewer-specific information.  Below are some suggestions on
    * how to configure an osgUtil::SceneView instance with lighting.
    *
    * For an application configure with a sky light:
    *
    * \code
    * osgUtil::SceneView::Options MyApp::getSceneViewDefaults()
    * {
    *    return osgUtil::SceneView::SKY_LIGHT;
    * }
    * \endcode
    *
    * For an application configured with a headlight:
    *
    * \code
    * osgUtil::SceneView::Options MyApp::getSceneViewDefaults()
    * {
    *    return osgUtil::SceneView::HEADLIGHT;
    * }
    * \endcode
    *
    * For an application configure with a user-defined light, there are
    * several steps.
    *
    * \code
    * // First, declare two member variables in your subclass of vrj::OsgApp
    * such as the following:
    * osg::ref_ptr<osg::Light> mLight0;
    * osg::ref_ptr<osg::LightSource> mLightSource0;
    *
    * // Then, in init() do something such as the following:
    * void MyApp::init()
    * {
    *    vrj::OsgApp::init();
    *
    *    mLight0 = new osg::Light();
    *    mLight0->setLightNum(0);
    *    mLight0->setAmbient(osg::Vec4f(0.36862f, 0.36842f, 0.36842f, 1.0f));
    *    mLight0->setDiffuse(osg::Vec4f(0.88627f, 0.88500f, 0.88500f, 1.0f));
    *    mLight0->setSpecular(osg::Vec4f(0.49019f, 0.48872f, 0.48872f, 1.0f));
    *    mLight0->setPosition(osg::Vec4f(10000.0f, 10000.0f, 10000.0f, 0.0f));
    *    mLight0->setDirection(osg::Vec3f(-1.0f, -1.0f, -1.0f));
    *
    *    mLightSource0 = new osg::LightSource();
    *    mLightSource0->setLight(mLight0.get());
    *    mLightSource0->setLocalStateSetModes(osg::StateAttribute::ON);
    *
    *    // Now that we know we have a root node add the default light to the
    *    // scene.
    *    this->getScene()->addChild( mLightSource0.get() );
    * }
    *
    * // Next, override vrj::OsgApp::getSceneViewDefaults() to change the
    * // option passed to osgUtil::SceneView::setDefaults().
    * osgUtil::SceneView::Options MyApp::getSceneViewDefaults()
    * {
    *    return osgUtil::SceneView::NO_SCENEVIEW_LIGHT;
    * }
    *
    * // Finally, set up the osgUtil::SceneView instance to use this light.
    * void MyApp::configSceneView(osgUtil::SceneView* newSceneViewer)
    * {
    *    vrj::OsgApp::configSceneView(newSceneViewer);
    *
    *    // add lights and turn on lighting
    *    newSceneViewer->getGlobalStateSet()->setAssociatedModes(
    *       mLight0.get(), osg::StateAttribute::ON
    *    );
    *    newSceneViewer->getGlobalStateSet()->setMode(GL_LIGHTING,
    *                                                 osg::StateAttribute::ON);
    * }
    * \endcode
    *
    * @post \p newSceneViewer is initialized.
    *
    * @see getSceneViewDefaults()
    */
   virtual void configSceneView(osgUtil::SceneView* newSceneViewer)
   {
      newSceneViewer->setDefaults(getSceneViewDefaults());
      newSceneViewer->init();
      newSceneViewer->setClearColor(osg::Vec4(0.0f, 0.0f, 0.0f, 0.0f));

      // Needed for stereo to work.
      newSceneViewer->setDrawBufferValue(GL_NONE);
   }

   /**
    * Function called after preFrame() and application-specific data
    * synchronization (in a cluster configuration) but before the start of a
    * new frame.
    *
    * If this method is overridden, it must be called as the last thing done
    * by the overriding version.  This calls update(), which is used to update
    * this application object's scene graph.
    *
    * @note Subclasses overriding this method \em must call this method
    *       implementation as the last step.
    *
    * @since 2.1.9
    */
   virtual void latePreFrame()
   {
      update();
   }

   /**
    * Function to set up and render the scene using OSG.  Override this
    * method with great care.  All the logic to handle multi-pipe rendering
    * and other VR Juggler features happens here.
    *
    * @pre OpenGL state has correct transformation and buffer selected.
    * @post The current scene has been drawn.
    */
   virtual void draw();

   /**
    * Application initialization function.
    * Execute any initialization needed before the graphics API is started.
    * If this method is overridden, it must be called by the overriding
    * version.  This calls initScene(), which is used to set up this
    * application object's scene graph.
    *
    * @note Derived classes MUST call base class version of this method.
    */
   virtual void init()
   {
      mUpdateVisitor = new osgUtil::UpdateVisitor();
      mFrameStamp    = new osg::FrameStamp();
      mUpdateVisitor->setFrameStamp(mFrameStamp.get());

      GlApp::init();

      mHead.init("VJHead");

      //Create the scene
      this->initScene();
   }

   /**
    * Function that is called immediately after a new context is created.
    * Use this function to create context-specific data structures such as
    * display lists and texture objects that are known to be required when
    * the context is created.
    *
    * @pre The OpenGL context has been set to the new context.
    * @post The application has completed context-specific initialization.
    */
   void contextInit();

   /**
    * Function that is called immediately before a context is closed.
    * Use the function to clean up any context-specific data structures.
    */
   virtual void contextClose()
   {
      ;
   }

   /**
    * Function that is called upon entry into the context before rendering.
    * This can be used to allocate context-specific data dynamically.
    *
    * @pre The OpenGL context has been set to the context for drawing.
    * @post The application object has executed any commands that need to be
    *       executed only once per context, per frame.
    *
    * @note This function can be used for things that need to happen
    *       every frame but only once per context.
    */
   virtual void contextPreDraw()
   {
      ;
   }

   /**
    * Function that is called once for each frame buffer of an OpenGL context.
    * This function is executed after contextInit() (if needed) but before
    * contextPreDraw().  It is called once per frame buffer (see note).
    *
    * @pre The OpenGL context has been set to the context for drawing.
    * @post The application object has executed any commands that need to be
    *        executed once per context, per buffer, per frame.
    *
    * @note This function is designed to be used when some task must be
    *       performed only once per frame buffer (i.e., once for the left
    *       buffer, once for the right buffer).  For example, the OpenGL clear
    *       color should be defined and glClear(GL_COLOR_BUFFER_BIT) should be
    *       called in this method.
    */
   virtual void bufferPreDraw()
   {
      ;
   }

   /**
    * Function that is called at the beginning of the drawing of each pipe.
    *
    * @pre The library is preparing to render all windows on a given pipe.
    * @post Any pre-pipe user calls have been done.
    *
    * @note Currently the OpenGL context is not set when this function is
    *       called.  This is a TEST function.  USE AT YOUR OWN RISK!!!
    */
   virtual void pipePreDraw()
   {
      ;
   }

protected:
   /**
    * Performs the update stage on the scene graph.  This function should be
    * called as the last thing that happens in latePreFrame(). If
    * latePreFrame() is not overridden, then this happens automatically.
    * Otherwise be sure to call vrj::OsgApp::latePreFrame() as the last thing
    * in application object's override of latePreFrame().
    *
    * @pre The library is preparing to switch from the serial preDraw stages
    *      to the parallel draw stages.
    * @post The scene graph update stage is complete and it is ready for cull
    *       and draw to be called in parallel.
    *
    * @note This function also takes care of time-based features in the scene
    *       graph.
    *
    * @since 2.1.9
    */
   void update()
   {
      ++mFrameNumber;

      // Update the frame stamp with information from this frame.
      mFrameStamp->setFrameNumber(mFrameNumber);
      mFrameStamp->setReferenceTime(mHead->getTimeStamp().secd());

      // Set up the time and frame number so time-dependent things (animations,
      // particle system) function correctly.
      // XXX: This may not be necessary.
      mUpdateVisitor->setTraversalNumber(mFrameNumber);

      // Update the scene by traversing it with the the update visitor which
      // will call all node update callbacks and animations. This is
      // equivalent to calling osgUtil::SceneView::update() but does not
      // require access to the context-specific osgUtil::SceneView instance.
      getScene()->accept(*mUpdateVisitor);
   }

   vrj::GlContextData< osg::ref_ptr<osgUtil::SceneView> > sceneViewer;

private:
   osg::ref_ptr<osg::NodeVisitor> mUpdateVisitor;
   osg::ref_ptr<osg::FrameStamp> mFrameStamp;

   int mFrameNumber;
   gadget::PositionInterface mHead;
   vpr::Mutex mSceneViewLock;
};

inline void OsgApp::contextInit()
{
   unsigned int unique_context_id = GlDrawManager::instance()->getCurrentContext();

   // --- Create new context specific scene viewer -- //
   osg::ref_ptr<osgUtil::SceneView> new_sv(new osgUtil::SceneView);
   this->configSceneView(new_sv.get());            // Configure the new viewer
   new_sv->getState()->setContextID(unique_context_id);
   // Add the tree to the scene viewer and set properties
   {
      vpr::Guard<vpr::Mutex> sv_guard(mSceneViewLock);
      new_sv->setSceneData(getScene());
   }

   (*sceneViewer) = new_sv;
}

inline void OsgApp::draw()
{
   glClear(GL_DEPTH_BUFFER_BIT);

   // Users have reported problems with OpenGL reporting stack underflow
   // problems when the texture attribute bit is pushed here, so we push all
   // attributes *except* GL_TEXTURE_BIT.
   glPushAttrib(GL_ALL_ATTRIB_BITS & ~GL_TEXTURE_BIT);
   glPushAttrib(GL_TRANSFORM_BIT);
   glPushAttrib(GL_VIEWPORT_BIT);

   glMatrixMode(GL_MODELVIEW);
   glPushMatrix();

   glMatrixMode(GL_PROJECTION);
   glPushMatrix();

   osg::ref_ptr<osgUtil::SceneView> sv;
   sv = (*sceneViewer);    // Get context specific scene viewer
   vprASSERT(sv.get() != NULL);

   // Set the timing information in the scene view.
   sv->setFrameStamp(mFrameStamp.get());

   GlDrawManager*    gl_manager;    /**< The openGL manager that we are rendering for. */
   gl_manager = GlDrawManager::instance();

   // Set the up the viewport (since OSG clears it out)
   float vp_ox, vp_oy, vp_sx, vp_sy;   // The float vrj sizes of the view ports
   int w_ox, w_oy, w_width, w_height;  // Origin and size of the window
   gl_manager->currentUserData()->getViewport()->getOriginAndSize(vp_ox, vp_oy, vp_sx, vp_sy);
   gl_manager->currentUserData()->getGlWindow()->getOriginSize(w_ox, w_oy, w_width, w_height);

   // compute unsigned versions of the viewport info (for passing to glViewport)
   unsigned ll_x = unsigned(vp_ox*float(w_width));
   unsigned ll_y = unsigned(vp_oy*float(w_height));
   unsigned x_size = unsigned(vp_sx*float(w_width));
   unsigned y_size = unsigned(vp_sy*float(w_height));

   //sv->setCalcNearFar(false);
   sv->setComputeNearFarMode(osgUtil::CullVisitor::DO_NOT_COMPUTE_NEAR_FAR);
   sv->setViewport(ll_x, ll_y, x_size, y_size);

   //Get the view matrix and the frustrum form the draw manager
   GlDrawManager* drawMan = dynamic_cast<GlDrawManager*>(this->getDrawManager());
   vprASSERT(drawMan != NULL);
   GlUserData* userData = drawMan->currentUserData();

   //Get the frustrum
   Projection* project = userData->getProjection();
   Frustum frustum = project->getFrustum();
   sv->setProjectionMatrixAsFrustum(frustum[Frustum::VJ_LEFT],
                                    frustum[Frustum::VJ_RIGHT],
                                    frustum[Frustum::VJ_BOTTOM],
                                    frustum[Frustum::VJ_TOP],
                                    frustum[Frustum::VJ_NEAR],
                                    frustum[Frustum::VJ_FAR]);

   // Copy the view matrix
   sv->setViewMatrix(osg::Matrix(project->getViewMatrix().mData));

   //Draw the scene
   // NOTE: It is not safe to call osgUtil::SceneView::update() here; it
   // should only be called by a single thread. The equivalent of calling
   // osgUtil::SceneView::update() is in vrj::OsgApp::update().
   sv->cull();
   sv->draw();

   glMatrixMode(GL_PROJECTION);
   glPopMatrix();

   glMatrixMode(GL_MODELVIEW);
   glPopMatrix();

   glPopAttrib();
   glPopAttrib();
   glPopAttrib();
}

}


#endif /* _VRJ_OSG_APP_H_ */
