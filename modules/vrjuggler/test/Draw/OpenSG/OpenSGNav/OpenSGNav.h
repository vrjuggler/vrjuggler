#ifndef _OSG_NAV_
#define _OSG_NAV_

#include <iostream>
#include <iomanip>

/*-----------------------------Juggler includes-------------------------------*/
#include <vrj/vrjConfig.h>
#include <vrj/Draw/OGL/GlApp.h>
#include <vrj/Math/Matrix.h>

#include <gadget/Type/PositionInterface.h>
#include <gadget/Type/AnalogInterface.h>
#include <gadget/Type/DigitalInterface.h>

/*-----------------------------OpenSG includes--------------------------------*/
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
#include <OpenSG/OSGPerspectiveCamera.h>
#include <OpenSG/OSGDirectionalLight.h>

/*----------------------------------------------------------------------------*/

#include <vjOSGApp.h>

class OpenSGNav : public vjOSGApp
{
public:
    OpenSGNav(vjKernel* kern):vjOSGApp(kern)
    {
        std::cout << "OpenSGNav::OpenSGNav called\n";
        mFileToLoad = std::string("");
        thread1_initialized=false;
        thread2_initialized=false;
    }

    virtual ~OpenSGNav (void)
    {
        std::cout << "OpenSGNav::~OpenSGNav called\n";
        if (_action)
            delete _action;
    }

    virtual void initScene()
    {
        std::cout << "OpenSGNav::initScene called\n";
        std::string wand("VJWand");
        mWand.init(wand);
        //OSG::osgInit(0,0);
        initRenderer();
        myInit();
    }

    void myInit(void);
    void highlightChanged(void);
    void updateHighlight(void);
    void initRenderer();

    virtual OSG::RenderAction * getAction()
    {
        std::cout << "OpenSGNav::getAction called\n";
        return _action;
    }

    virtual void draw();

    virtual void contextInit()
    {
        vjOSGApp::contextInit();
        initGLState();
    }

    void bufferPreDraw();

    virtual void preFrame();

    virtual void intraFrame()
    {
        //std::cout << "OpenSGNav::intraFrame called\n";
        // Put your intra frame computations here.
    }

    virtual void postFrame()
    {
        //std::cout << "OpenSGNav::postFrame called\n";
        // Put your post frame computations here.
    }

    void setModelFileName(std::string filename)
    {
        std::cout << "OpenSGNav::setModelFileName called\n";
        mFileToLoad = filename;
    }

  private:
    void initGLState();

  private:

    float speed;

    std::string mFileToLoad;

    OSG::NodePtr              _highlight;
    OSG::NodePtr              _highlightNode;
    OSG::GeoPositions3fPtr    _highlightPoints;

    OSG::NodePtr              _root;

    OSG::ImageForegroundPtr   _foreground;
    OSG::NodePtr              _internalRoot;
    OSG::DirectionalLightPtr  _headlight;
    OSG::RenderAction *       _action;
    OSG::PerspectiveCameraPtr _camera;
    OSG::TransformPtr         _cart;

    static  OSG::SimpleMaterialPtr    _highlightMaterial;

    bool thread1_initialized,thread2_initialized;
    OSG::Matrix OSGMat;

  public:
    vjPosInterface   mWand;

    static OSG::UInt32 OSG_MAIN_ASPECT_ID;
};


#endif
