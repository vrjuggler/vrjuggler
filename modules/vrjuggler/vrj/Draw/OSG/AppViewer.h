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
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

#ifndef _VRJ_OSG_APPVIEWER_H_
#define _VRJ_OSG_APPVIEWER_H_

#include <vrj/vrjConfig.h>

#include <vpr/Sync/Mutex.h>
#include <vpr/Sync/Guard.h>

#include <vrj/Draw/OpenGL/App.h>
#include <vrj/Draw/OpenGL/ContextData.h>
#include <vrj/Draw/OpenGL/Window.h>

#include <vrj/Display/CameraProjection.h>

#include <vrj/Kernel/Kernel.h>
#include <vrj/Kernel/User.h>

#include <osgViewer/Viewer>

namespace vrj
{
   
namespace osg
{

/** \class vrj::osg::AppViewer AppViewer.h vrj/Draw/OSG/AppViewer.h
 *
 * This class is an alternative class to the standard OpenSceneGraph
 * application class vrj::osg::App which uses an instance of the
 * osgViewer::Viewer instead of the deprecated osgUtil::SceneView
 * class for rendering. If you wish to use all the functionality
 * embedded within osgViewer, then this class defines the base class
 * from which your application class should be derived. It makes use
 * of the OpenGL Draw Manager.
 *
 * @since 2.3.19
 *
 * @see vrj::opengl::App
 */
class AppViewer : public vrj::opengl::App
{
public:
	AppViewer(Kernel* kern = NULL)
		: vrj::opengl::App(kern)
	{
      // Tell osg::Referenced to use thread-safe reference counting. We do
      // this here for all applications and all configurations though it is
      // only strictly necessary when VR Juggler is configured to use
      // multi-threaded rendering.
      ::osg::Referenced::setThreadSafeReferenceCounting(true);
	}

	virtual ~AppViewer()
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
	virtual ::osg::Group* getScene() = 0;

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
		// Create the osgViewer instance
		mViewer = new osgViewer::Viewer;
		mViewer->setThreadingModel(osgViewer::Viewer::SingleThreaded);

		// Call the parent init method
		vrj::opengl::App::init();

		// Create the scene
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
		// Clear the color buffer
		glClear(GL_COLOR_BUFFER_BIT);
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
	 * Otherwise be sure to call vrj::osg::App::latePreFrame() as the last
	 * thing in application object's override of latePreFrame().
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
		// Update the viewer time stamp
		const double head_time(mKernel->getUsers()[0]->getHeadPosProxy()->getTimeStamp().secd());
		mViewer->advance(head_time);

		// Only update everything attached to the viewer if there
		// is actually work to do
		if (mViewer->done())
			return;

		mViewer->eventTraversal();
	}

	vrj::opengl::ContextData< ::osg::ref_ptr<osgViewer::Viewer> > mContextViewer;
	::osg::ref_ptr<osgViewer::Viewer> mViewer;

private:
	int mFrameNumber;
	vpr::Mutex mSceneViewLock;
};

inline void AppViewer::contextInit()
{
	// --- Create new context specific osgViewer::Viewer for rendering -- //

   // Create the osgViewer instance
   ::osg::ref_ptr<osgViewer::Viewer> contextViewer = new osgViewer::Viewer;
   contextViewer->setThreadingModel(osgViewer::Viewer::SingleThreaded);

	// Set up osgViewer::GraphicsWindowEmbedded for this context
	::osg::ref_ptr< ::osg::GraphicsContext::Traits > traits = new ::osg::GraphicsContext::Traits;
	::osg::ref_ptr<osgViewer::GraphicsWindowEmbedded> graphicsWindow =
		new osgViewer::GraphicsWindowEmbedded(traits.get());
	contextViewer->getCamera()->setGraphicsContext(graphicsWindow.get());

	// Set the unique context id
	const unsigned int unique_context_id =
		vrj::opengl::DrawManager::instance()->getCurrentContext();
	contextViewer->getCamera()->getGraphicsContext()->getState()->setContextID(unique_context_id);

	// Add the tree to the scene viewer and set properties
	vpr::Guard<vpr::Mutex> sv_guard(mSceneViewLock);
	contextViewer->setSceneData(getScene());

	// Keep our pointer around to the context viewer for rendering purposes
	(*mContextViewer) = contextViewer;
}

inline void AppViewer::draw()
{
	// Clear the depth buffer
	glClear(GL_DEPTH_BUFFER_BIT);

	// Grab the context specific osgViewer
   ::osg::ref_ptr<osgViewer::Viewer> contextViewer;
   contextViewer = (*mContextViewer);
   vprASSERT(contextViewer.get() != NULL);

	// Must force the updateTraversal in the draw method for terrain database paging to
	// work correctly for each context.
	const double head_time(mKernel->getUsers()[0]->getHeadPosProxy()->getTimeStamp().secd());
	contextViewer->advance(head_time);
	if (!contextViewer->done())
		contextViewer->updateTraversal();

	// Grab the OpenGL Draw Manager that we are rendering for.
	// Then grab the user data and extra the viewport information.
	vrj::opengl::DrawManager* gl_manager =
		dynamic_cast<vrj::opengl::DrawManager*>(this->getDrawManager());
	vprASSERT(gl_manager != NULL);
	vrj::opengl::UserData* user_data = gl_manager->currentUserData();

	// Set the up the viewport (since OSG clears it out)
	float vp_ox, vp_oy, vp_sx, vp_sy;   // The float vrj sizes of the view ports
	int w_ox, w_oy, w_width, w_height;  // Origin and size of the window
	user_data->getViewport()->getOriginAndSize(vp_ox, vp_oy, vp_sx, vp_sy);
	user_data->getGlWindow()->getOriginSize(w_ox, w_oy, w_width, w_height);

	// Compute unsigned versions of the viewport info (for passing to glViewport)
	const unsigned int ll_x = static_cast<unsigned int>(vp_ox * static_cast<float>(w_width));
	const unsigned int ll_y = static_cast<unsigned int>(vp_oy * static_cast<float>(w_height));
	const unsigned int x_size = static_cast<unsigned int>(vp_sx * static_cast<float>(w_width));
	const unsigned int y_size = static_cast<unsigned int>(vp_sy * static_cast<float>(w_height));

	// Set the actual viewport
	contextViewer->getCamera()->setViewport(ll_x, ll_y, x_size, y_size);

	// Set the frustrum
	vrj::ProjectionPtr project = user_data->getProjection();
	vrj::Frustum frustum = project->getFrustum();
	contextViewer->getCamera()->setProjectionMatrixAsFrustum(frustum[vrj::Frustum::VJ_LEFT],
																		      frustum[vrj::Frustum::VJ_RIGHT],
																	      	frustum[vrj::Frustum::VJ_BOTTOM],
																		      frustum[vrj::Frustum::VJ_TOP],
																		      frustum[vrj::Frustum::VJ_NEAR],
																		      frustum[vrj::Frustum::VJ_FAR]);

	// Set the view matrix
	contextViewer->getCamera()->setViewMatrix(::osg::Matrix(project->getViewMatrix().mData));	

	// Use the osgViewer to handle all culling and drawing
	contextViewer->renderingTraversals();
}

} // End of osg namespace

} // End of vrj namespace

#endif /* _VRJ_OSG_APPVIEWER_H_ */
