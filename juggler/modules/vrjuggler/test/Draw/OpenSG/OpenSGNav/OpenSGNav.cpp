#include <vrj/vrjConfig.h>

#include <math.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include <vrj/Math/Vec3.h>
#include <vrj/Math/Coord.h>
#include <vrj/Display/Projection.h>
#include <vrj/Display/CameraProjection.h>

#include <OpenSGNav.h>

#include <OpenSG/OSGConfig.h>
#include <OpenSG/OSGBaseFunctions.h>
#include <OpenSG/OSGBaseTypes.h>
#include <OpenSG/OSGImageFileType.h>
#include <OpenSG/OSGSolidBackground.h>
#include <OpenSG/OSGViewport.h>
#include <OpenSG/OSGLogoData.h>
#include <OpenSG/OSGThread.h>
#include <OpenSG/OSGMPBase.h>
#include <OpenSG/OSGMatrixUtility.h>

OSG::UInt32 OpenSGNav::OSG_MAIN_ASPECT_ID=0;

// Handle any initialization needed before API
void OpenSGNav::init()
{
   vrj::OpenSGApp::init();      // Call base class init

   vprDEBUG(vprDBG_ALL,0) << "OpenSGNav::init: Called.\n" << vprDEBUG_FLUSH;

   // XXX: Complete
   // if(!osgInitAlreadyCalled())
   OSG::osgInit(0,0);                  // Binds to primordial thread
   OpenSGNav::OSG_MAIN_ASPECT_ID = OSG::Thread::getAspect();   // Gets the base aspect id to use
}

void OpenSGNav::contextInit()
{
   vprDEBUG(vprDBG_ALL,0) << "OpenSGNav::contextInit: Called.\n" << vprDEBUG_FLUSH;

   context_data* c_data = &(*mContextData);

   // Check for thread initialized
   // This will only happen for the first initialized context per pipe
   if(!c_data->mContextThreadInitialized)
   {
      c_data->mContextThreadInitialized = true;

      char thread_name_buffer[255];
      sprintf(thread_name_buffer, "vprThread:%d", vpr::Thread::self()->getTID());
      c_data->mOsgThread = OSG::ExternalThread::get(thread_name_buffer);
      c_data->mOsgThread->initialize(OSG_MAIN_ASPECT_ID);     // XXX: In future this might need to be different thread
   }

   // Allocate OpenSG stuff
   c_data->mWin = OSG::PassiveWindow::create();
   c_data->mCameraCartNode = OSG::Node::create();
   c_data->mCameraCartTransform = OSG::Transform::create();
   c_data->mCamera = OSG::PerspectiveCamera::create();

   // Setup the cart, set internal transform node
   OSG::beginEditCP(c_data->mCameraCartNode);
      c_data->mCameraCartNode->setCore(c_data->mCameraCartTransform);
   OSG::endEditCP(c_data->mCameraCartNode);

   // Setup the camera
   OSG::beginEditCP(c_data->mCamera);
      c_data->mCamera->setBeacon (c_data->mCameraCartNode);
      c_data->mCamera->setFov    (OSG::deg2rad(60.));
      c_data->mCamera->setNear   (0.1);
      c_data->mCamera->setFar    (10000);
   OSG::endEditCP(c_data->mCamera);

   // Could actually make one of these per thread instead of context.
   c_data->mRenderAction = OSG::RenderAction::create();
   c_data->mRenderAction->setAutoFrustum(false);         // Turn off auto frustum

   //vjOSGApp::contextInit();
   initGLState();
}

void OpenSGNav::contextPreDraw()
{
   context_data* c_data = &(*mContextData);
   c_data->mWin->frameInit();
}

void OpenSGNav::contextPostDraw()
{
   context_data* c_data = &(*mContextData);
   c_data->mWin->frameExit();
}


void OpenSGNav::draw()
{
   context_data* c_data = &(*mContextData);

   vrj::GlDrawManager* drawMan = dynamic_cast<vrj::GlDrawManager*> ( this->getDrawManager() );
   vprASSERT(drawMan != NULL);
   vrj::GlUserData* userData = drawMan->currentUserData();

   // Copy the matrix
   vrj::Projection* project = userData->getProjection();
   vrj::Frustum vrj_frustum = userData->getProjection()->mFrustum;

   float * vj_proj_view_mat = project->mViewMat.getFloatPtr();
   OSG::Matrix frustum_matrix, view_xform_mat;
   view_xform_mat.setValue(vj_proj_view_mat);
   osg::MatrixFrustum(frustum_matrix, vrj_frustum[vrj::Frustum::VJ_LEFT], vrj_frustum[vrj::Frustum::VJ_RIGHT],
                                      vrj_frustum[vrj::Frustum::VJ_BOTTOM], vrj_frustum[vrj::Frustum::VJ_TOP],
                                      vrj_frustum[vrj::Frustum::VJ_NEAR], vrj_frustum[vrj::Frustum::VJ_FAR]);

   // ---- SETUP CAMERA --- //
   // Get the viewport
   vrj::Viewport* vp = userData->getViewport();
   vrj::CameraProjection* sim_cam_proj(NULL);

   switch(vp->getType())
   {
   case vrj::Viewport::SURFACE:
      vprASSERT(false);
      /*
      the_cam->home();
      the_cam->setAdjustAspectRatioMode(osg::Camera::ADJUST_NONE);      // Tell it not to adjust the aspect ratio at all

      //Set the frustrum (this is set with the matrix below)
      //float near_val = frustum[Frustum::VJ_NEAR];
      the_cam->setFrustum(frustum[Frustum::VJ_LEFT],   frustum[Frustum::VJ_RIGHT],
                          frustum[Frustum::VJ_BOTTOM],  frustum[Frustum::VJ_TOP],
                          frustum[Frustum::VJ_NEAR],             frustum[Frustum::VJ_FAR]);
      */
      break;

   case vrj::Viewport::SIM:
      sim_cam_proj = dynamic_cast<vrj::CameraProjection*>(project);
      vprASSERT(sim_cam_proj != NULL && "Trying to use non-camera projection for simulator");
      c_data->mCamera->setNear(vrj_frustum[vrj::Frustum::VJ_NEAR]);
      c_data->mCamera->setFar(vrj_frustum[vrj::Frustum::VJ_FAR]);
      c_data->mCamera->setFov(sim_cam_proj->mVertFOV);
      break;

   default:
      vprASSERT(false);
      break;
   }

   // --- Trigger the draw --- //
   c_data->mRenderAction->setWindow(c_data->mWin.getCPtr());
   c_data->mRenderAction->setViewport(NULL);
   c_data->mRenderAction->setCamera(c_data->mCamera.getCPtr());
   c_data->mRenderAction->setFrustumCulling(false);    // Turn off culling for now because I don't trust the frustum setup

   c_data->mRenderAction->apply(getSceneRoot());                // Actually do the rendering

   //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
     // Set material color
   float onyx_red = 59.0f/255.0f;
   float onyx_blue = 57.0f/255.0f;
   GLfloat mat_ambient[] = { onyx_red, 0.0, onyx_blue, 1.0};
   GLfloat mat_diffuse[] = { onyx_red, 0.0, onyx_blue, 1.0};
   GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0};
   GLfloat mat_shininess[] = { 50.0};

   glMaterialfv( GL_FRONT, GL_AMBIENT, mat_ambient );
   glMaterialfv( GL_FRONT,  GL_DIFFUSE, mat_diffuse );
   glMaterialfv( GL_FRONT, GL_SPECULAR, mat_specular );
   glMaterialfv( GL_FRONT,  GL_SHININESS, mat_shininess );
   //glColor4f(1.0, 1.0, 1.0, 1.0);

   //glLoadIdentity();
   glPushMatrix();
      glTranslatef(-1.5f,0.0f,6.0f);
       glBegin(GL_TRIANGLES);
         glVertex3f(0.0f,1.0f,0.0f);
         glVertex3f(-1.0f,-1.0f,0.0f);
         glVertex3f(1.0f,-1.0f,0.0f);
       glEnd();
   glPopMatrix();
}

void OpenSGNav::preFrame()
{
    //std::cout << "OpenSGNav::preFrame called\n";
    //move the model around
}

void OpenSGNav::bufferPreDraw()
{
   glClearColor(0.0, 0.0, 0.0, 0.0);
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void OpenSGNav::initGLState()
{
    std::cout << "OpenSGNav::initGLState called\n";
    /*
    glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );
    glPixelStorei( GL_PACK_ALIGNMENT, 1 );
    */

    GLfloat light0_ambient[] = { .2,  .2,  .2,  1.0};
   GLfloat light0_diffuse[] = { 1.0,  1.0,  1.0,  1.0};
   GLfloat light0_specular[] = { 1.0,  1.0,  1.0,  1.0};
   GLfloat light0_position[] = {6.0, 6,0, 6.0, 1.0};

   glLightfv(GL_LIGHT0, GL_AMBIENT,  light0_ambient);
   glLightfv(GL_LIGHT0, GL_DIFFUSE,  light0_diffuse);
   glLightfv(GL_LIGHT0, GL_SPECULAR,  light0_specular);
   glLightfv(GL_LIGHT0, GL_POSITION,  light0_position);

   glEnable(GL_DEPTH_TEST);
   glEnable(GL_NORMALIZE);
   //glEnable(GL_LIGHTING);
   //glEnable(GL_LIGHT0);
   //glEnable(GL_COLOR_MATERIAL);
   glShadeModel(GL_SMOOTH);

   glMatrixMode(GL_MODELVIEW);

}

void OpenSGNav::initScene(void)
{
   vprDEBUG(vprDBG_ALL,0) << "OpenSGNav::initScene: Called.\n" << vprDEBUG_FLUSH;

   std::string wand("VJWand");
   mWand.init(wand);

   // Load a graph
   if (mFileToLoad ==  std::string("none"))
    {
        std::cout << "OpenSGNav::initScene beforetorusmake\n";
        mSceneRoot = OSG::makeTorus(.5, 2, 16, 16);
        std::cout << "OpenSGNav::myInit aftertorusmake\n";
    }
    else
    {
       std::cout << "OpenSGNav::initScene: Loading [" << mFileToLoad.c_str() << "]\n";
       mSceneRoot = OSG::SceneFileHandler::the().read((OSG::Char8 *)(mFileToLoad.c_str()));
    }
    std::cout << "OpenSGNav::initScene before RenderAction::create()\n";

    // -- light node --> light cart
    //the camera and light beacon
    mLightNode = OSG::Node::create();
    mLightCart = OSG::Node::create();
    OSG::DirectionalLightPtr light_core = OSG::DirectionalLight::create();
    OSG::TransformPtr cart_core = OSG::Transform::create();

    osg::Matrix light_pos;
    light_pos.setTransform(osg::Vec3f( 2.0f, 5.0f, 4.0f));

   osg::beginEditCP(cart_core, OSG::Transform::MatrixFieldMask);
   {
       cart_core->setMatrix(light_pos);
   }
   osg::endEditCP(cart_core, osg::Transform::MatrixFieldMask);

    // Setup light cart node
    osg::beginEditCP(mLightCart);
      mLightCart->setCore(cart_core);
    osg::endEditCP(mLightCart);

    osg::addRefCP(mLightNode);
    osg::beginEditCP(mLightNode);
      mLightNode->setCore(light_core);
      mLightNode->addChild(mLightCart);
    osg::endEditCP(mLightNode);

    osg::beginEditCP(light_core);
      light_core->setAmbient   (.9, .8, .8, 1);
      light_core->setDiffuse   ( 0.6,  0.6,  0.6, 1);
      light_core->setSpecular  ( 1,  1,  1, 1);
      light_core->setDirection ( 0,  0,  1);
      light_core->setBeacon    (mLightNode);
    osg::endEditCP(light_core);

    osg::addRefCP(mSceneRoot);
    osg::beginEditCP(mSceneRoot);
      mSceneRoot->addChild(mLightNode);
    osg::endEditCP(mSceneRoot);

    // --- Add scene to root node --- //

    std::cout << "OpenSGNav::initScene finished\n";
}

