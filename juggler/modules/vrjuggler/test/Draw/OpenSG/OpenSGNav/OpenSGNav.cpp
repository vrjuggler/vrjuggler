#include <vrj/vrjConfig.h>

#include <math.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include <vrj/Math/Vec3.h>
#include <vrj/Math/Coord.h>

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

OSG::SimpleMaterialPtr OpenSGNav::_highlightMaterial;
OSG::UInt32 OpenSGNav::OSG_MAIN_ASPECT_ID=0;
typedef OSG::ExternalThread OSGET;

void OpenSGNav::draw()
{
    std::cout << "OpenSGNav::draw called\n";
    vrj::GlDrawManager* drawMan = dynamic_cast<vrj::GlDrawManager*> ( this->getDrawManager() );
    vprASSERT(drawMan != NULL);
    vrj::GlUserData* userData = drawMan->currentUserData();

    // Copy the matrix
    vrj::Projection* project = userData->getProjection();
    float * vj_proj_view_mat = project->mViewMat.getFloatPtr();
    OSGMat.setValue(vj_proj_view_mat);

/*	//Get the frustrum
	vjFrustum frustum = project->mFrustum;
	
    //Reset the camera
    the_cam->home();

    //Set the frustrum (this is set with the matrix below)
    float near_val = frustum[vjFrustum::VJ_NEAR];
    the_cam->setFrustum(frustum[vjFrustum::VJ_LEFT]*near_val,   frustum[vjFrustum::VJ_RIGHT]*near_val,
                      frustum[vjFrustum::VJ_BOTTOM]*near_val,  frustum[vjFrustum::VJ_TOP]*near_val,
                      frustum[vjFrustum::VJ_NEAR],             frustum[vjFrustum::VJ_FAR]);
	
    //Set the look at
    the_cam->attachTransform(osg::Camera::MODEL_TO_EYE, &osgMat);*/
    OSGMat.setValue(1.0f,0.0f,0.0f,0.0f,
                    0.0f,1.0f,0.0f,0.0f,
                    0.0f,0.0f,1.0f,0.0f,
                    0.0f,0.0f,0.0f,1.0f);

    _cart->getSFMatrix()->setValue(OSGMat);
    updateHighlight();
    //vjOSGApp::draw();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    glTranslatef(-1.5f,0.0f,6.0f);
    glBegin(GL_TRIANGLES);
    glVertex3f(0.0f,1.0f,0.0f);
    glVertex3f(-1.0f,-1.0f,0.0f);
    glVertex3f(1.0f,-1.0f,0.0f);
    glEnd();
}

void OpenSGNav::preFrame()
{
    //std::cout << "OpenSGNav::preFrame called\n";
    //move the model around
}

void OpenSGNav::bufferPreDraw()
{
    if (!thread2_initialized)
    {
       OSG::ExternalThread *new_thread = OSG::ExternalThread::get("OSGExternalThread2");
       new_thread->initialize(OSG_MAIN_ASPECT_ID);
       thread2_initialized=true;
    }
    std::cout << "OpenSGNav::preFrame called\n";
}

void OpenSGNav::initGLState()
{
    std::cout << "OpenSGNav::initGLState called\n";
    glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );
    glPixelStorei( GL_PACK_ALIGNMENT, 1 );

    glDepthFunc( GL_LEQUAL );
    glEnable( GL_DEPTH_TEST );


/*   GLfloat light0_ambient[] = { .2,  .2,  .2,  1.0};
   GLfloat light0_diffuse[] = { 1.0,  1.0,  1.0,  1.0};
   GLfloat light0_specular[] = { 1.0,  1.0,  1.0,  1.0};
   GLfloat light0_position[] = {6.0, 6,0, 6.0, 1.0};

   glLightfv(GL_LIGHT0, GL_AMBIENT,  light0_ambient);
   glLightfv(GL_LIGHT0, GL_DIFFUSE,  light0_diffuse);
   glLightfv(GL_LIGHT0, GL_SPECULAR,  light0_specular);
   glLightfv(GL_LIGHT0, GL_POSITION,  light0_position);

   glEnable(GL_DEPTH_TEST);
   glEnable(GL_NORMALIZE);
   glEnable(GL_LIGHTING);
   glEnable(GL_LIGHT0);
   glDisable(GL_COLOR_MATERIAL);
   glShadeModel(GL_SMOOTH);

   glMatrixMode(GL_MODELVIEW);    */
}

void OpenSGNav::initRenderer()
{
    if (!thread1_initialized)
    {
        OSG::ExternalThread *new_thread=OSG::ExternalThread::get("OSGExternalThread1");
        new_thread->initialize(OSG_MAIN_ASPECT_ID);
        thread1_initialized=true;
    }
    std::cout << "OpenSGNav::initRenderer called\n";
    // the connection between GLUT and OpenSG
//    GLUTWindowPtr gwin= GLUTWindow::create();
//    gwin->setWinID(winid);
//    gwin->init();

//    _win = gwin;
    _win = OSG::PassiveWindow::create();
}

void OpenSGNav::myInit(void)
{
    std::cout << "OpenSGNav::myInit called\n";
    if (1)
    {
        std::cout << "OpenSGNav::myInit beforetorusmake\n";
        _root = OSG::makeTorus(.5, 2, 16, 16);
        std::cout << "OpenSGNav::myInit aftertorusmake\n";
    }
    else
    {
       std::cout << "OpenSGNav::myInit: Loading [" << mFileToLoad.c_str() << "]\n";
       _root = OSG::SceneFileHandler::the().read((OSG::Char8 *)(mFileToLoad.c_str()));
    }
    std::cout << "OpenSGNav::myInit before RenderAction::create()\n";
    //the rendering action
    _action = OSG::RenderAction::create();
    std::cout << "OpenSGNav::myInit after RenderAction::create()\n";

    //the camera and light beacon
    OSG::NodePtr cartN = OSG::Node::create();
    _cart = OSG::Transform::create();

    std::cout << "OpenSGNav::myInit before bECP\n";
    OSG::beginEditCP(cartN);
    cartN->setCore(_cart);
    OSG::endEditCP(cartN);
    std::cout << "OpenSGNav::myInit after bECP\n";
    // the headlight
    _internalRoot = OSG::Node::create();
    _headlight = OSG::DirectionalLight::create();

    OSG::addRefCP(_internalRoot);
    OSG::beginEditCP(_internalRoot);
    _internalRoot->setCore(_headlight);
    _internalRoot->addChild(cartN);
    OSG::endEditCP(_internalRoot);

    OSG::beginEditCP(_headlight);
    _headlight->setAmbient   (.3, .3, .3, 1);
    _headlight->setDiffuse   ( 1,  1,  1, 1);
    _headlight->setSpecular  ( 1,  1,  1, 1);
    _headlight->setDirection ( 0,  0,  1);
    _headlight->setBeacon    (cartN);
    OSG::endEditCP(_headlight);

    //the camera
    _camera = OSG::PerspectiveCamera::create();
    OSG::beginEditCP(_camera);
    _camera->setBeacon (cartN);
    _camera->setFov    (OSG::deg2rad(60.));
    _camera->setNear   (0.1);
    _camera->setFar    (10000);
    OSG::endEditCP(_camera);

    //need a viewport?
    if (_win->getPort().getSize() == 0)
    {
        OSG::SolidBackgroundPtr bg = OSG::SolidBackground::create();
        OSG::beginEditCP(bg);
        bg->setColor(OSG::Color3f(0, 0, 0));
        OSG::endEditCP(bg);

        _foreground = OSG::ImageForeground::create();

        OSG::ViewportPtr vp = OSG::Viewport::create();
        OSG::beginEditCP(vp);
        vp->setCamera(_camera);
        vp->setRoot(_internalRoot);
        vp->setSize(0, 0, 1, 1);
        vp->setBackground(bg);
        vp->getForegrounds().addValue(_foreground);
        OSG::endEditCP(vp);

        OSG::beginEditCP(_win);
        _win->addPort(vp);
        OSG::endEditCP(_win);
    }
    OSG::addRefCP(_root);
    _internalRoot->addChild(_root);
    _highlight = _root;
    highlightChanged();
    std::cout << "OpenSGNav::myInit finished\n";
}

void OpenSGNav::highlightChanged(void)
{
    std::cout << "OpenSGNav::highlightChanged called\n";
    // init as needed
    if(_highlightMaterial == OSG::NullFC)
    {
        _highlightMaterial = OSG::SimpleMaterial::create();

        OSG::beginEditCP(_highlightMaterial);
        _highlightMaterial->setDiffuse (OSG::Color3f(0,1,0));
        _highlightMaterial->setLit     (false);
        OSG::endEditCP(_highlightMaterial);
    }
    if(_highlightNode == OSG::NullFC)
    {
        OSG::GeoPTypesPtr type = OSG::GeoPTypesUI8::create();
        OSG::beginEditCP(type);
        type->addValue(GL_LINE_STRIP);
        type->addValue(GL_LINES);
        OSG::endEditCP(type);

        OSG::GeoPLengthsPtr lens = OSG::GeoPLengthsUI32::create();
        OSG::beginEditCP(lens);
        lens->addValue(10);
        lens->addValue(6);
        OSG::endEditCP(lens);

        OSG::GeoIndicesUI32Ptr index = OSG::GeoIndicesUI32::create();
        OSG::beginEditCP(index);
        index->getFieldPtr()->addValue(0);
        index->getFieldPtr()->addValue(1);
        index->getFieldPtr()->addValue(3);
        index->getFieldPtr()->addValue(2);
        index->getFieldPtr()->addValue(0);
        index->getFieldPtr()->addValue(4);
        index->getFieldPtr()->addValue(5);
        index->getFieldPtr()->addValue(7);
        index->getFieldPtr()->addValue(6);
        index->getFieldPtr()->addValue(4);

        index->getFieldPtr()->addValue(1);
        index->getFieldPtr()->addValue(5);
        index->getFieldPtr()->addValue(2);
        index->getFieldPtr()->addValue(6);
        index->getFieldPtr()->addValue(3);
        index->getFieldPtr()->addValue(7);
        OSG::endEditCP(index);

        _highlightPoints = OSG::GeoPositions3f::create();
        OSG::beginEditCP(_highlightPoints);
        _highlightPoints->addValue(OSG::Pnt3f(-1, -1, -1));
        _highlightPoints->addValue(OSG::Pnt3f( 1, -1, -1));
        _highlightPoints->addValue(OSG::Pnt3f(-1,  1, -1));
        _highlightPoints->addValue(OSG::Pnt3f( 1,  1, -1));
        _highlightPoints->addValue(OSG::Pnt3f(-1, -1,  1));
        _highlightPoints->addValue(OSG::Pnt3f( 1, -1,  1));
        _highlightPoints->addValue(OSG::Pnt3f(-1,  1,  1));
        _highlightPoints->addValue(OSG::Pnt3f( 1,  1,  1));
        OSG::endEditCP(_highlightPoints);

        OSG::GeometryPtr geo=OSG::Geometry::create();
        OSG::beginEditCP(geo);
        geo->setTypes     (type);
        geo->setLengths   (lens);
        geo->setIndices   (index);
        geo->setPositions (_highlightPoints);
        geo->setMaterial  (_highlightMaterial);
        OSG::endEditCP(geo);
        OSG::addRefCP(geo);

        _highlightNode = OSG::Node::create();
        OSG::beginEditCP(_highlightNode);
        _highlightNode->setCore(geo);
        OSG::endEditCP(_highlightNode);
        OSG::addRefCP(_highlightNode);
    }

    // attach the hightlight node to the root if the highlight is active
    if(_highlight != OSG::NullFC)
    {
        if(_highlightNode->getParent() == OSG::NullFC)
        {
            OSG::beginEditCP(_internalRoot);
            _internalRoot->addChild(_highlightNode);
            OSG::endEditCP(_internalRoot);
        }
    }
    else
    {
        if(_highlightNode->getParent() != OSG::NullFC)
        {
            OSG::beginEditCP(_internalRoot);
            _internalRoot->subChild(_highlightNode);
            OSG::endEditCP(_internalRoot);
        }

    }
    // update the highlight geometry
    updateHighlight();
}

void OpenSGNav::updateHighlight(void)
{
    std::cout << "OpenSGNav::updateHighlight called\n";
    if(_highlight == OSG::NullFC)
        return;

    // calc the world bbox of the highlight object
    OSG::DynamicVolume vol;
    _highlight->getWorldVolume(vol);

    OSG::Pnt3f min,max;
    vol.getBounds(min, max);

    OSG::beginEditCP(_highlightPoints);
    _highlightPoints->setValue(OSG::Pnt3f(min[0], min[1], min[2]), 0);
    _highlightPoints->setValue(OSG::Pnt3f(max[0], min[1], min[2]), 1);
    _highlightPoints->setValue(OSG::Pnt3f(min[0], max[1], min[2]), 2);
    _highlightPoints->setValue(OSG::Pnt3f(max[0], max[1], min[2]), 3);
    _highlightPoints->setValue(OSG::Pnt3f(min[0], min[1], max[2]), 4);
    _highlightPoints->setValue(OSG::Pnt3f(max[0], min[1], max[2]), 5);
    _highlightPoints->setValue(OSG::Pnt3f(min[0], max[1], max[2]), 6);
    _highlightPoints->setValue(OSG::Pnt3f(max[0], max[1], max[2]), 7);
    OSG::endEditCP(_highlightPoints);

    OSG::beginEditCP(_highlightNode->getCore(), OSG::Geometry::PositionsFieldMask);
    OSG::endEditCP  (_highlightNode->getCore(), OSG::Geometry::PositionsFieldMask);
}
