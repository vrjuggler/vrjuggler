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

#include <vrj/Draw/OGL/GlContextData.h>

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
#include <OpenSG/OSGGroup.h>

/*----------------------------------------------------------------------------*/

#include <vjOSGApp.h>

struct context_data
{
   context_data()
     : mRenderAction(NULL),
       mContextThreadInitialized(false),
       mOsgThread(NULL)
   {;}

   OSG::RenderAction*         mRenderAction;    /**< The render action for the scene */
   OSG::PassiveWindowPtr      mWin;             /**< passive window to render with (the context) */
   OSG::PerspectiveCameraPtr  mCamera;
   OSG::TransformPtr          mCameraCartTransform;
   OSG::NodePtr               mCameraCartNode;

   bool                       mContextThreadInitialized;
   OSG::ExternalThread*       mOsgThread;
};


class OpenSGNav : public vrj::OpenSGApp
{
public:
    OpenSGNav(vrj::Kernel* kern)
       : OpenSGApp(kern)
    {
        std::cout << "OpenSGNav::OpenSGNav called\n";
        mFileToLoad = std::string("");
    }

    virtual ~OpenSGNav (void)
    {
        std::cout << "OpenSGNav::~OpenSGNav called\n";
    }

    // Handle any initialization needed before API
    virtual void init();

    virtual void apiInit()
    {
       vrj::OpenSGApp::apiInit();

       vprDEBUG(vprDBG_ALL,0) << "OpenSGNav::initAPI: Called.\n" << vprDEBUG_FLUSH;

       this->initScene();
       vprASSERT(getSceneRoot().getCPtr() != NULL);    // I don't know if this is even valid
    }

    virtual void initScene();
    virtual OSG::NodePtr getSceneRoot()
    { return mSceneRoot; }

    void initRenderer();

    virtual void draw();

    virtual void contextInit();
    virtual void contextPreDraw();
    virtual void contextPostDraw();

    void bufferPreDraw();

    virtual void preFrame();

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
    OSG::NodePtr              mSceneRoot;       /**< The root of the scene */
    OSG::NodePtr              mLightNode;       /**< The light node */
    OSG::NodePtr              mLightCart;       /**< The cart for the light */

    // App class stuff //
    //OSG::NodePtr              mRoot;            /**< A pointer to the scene graph root*/

    //OSG::ImageForegroundPtr   _foreground;
    //OSG::NodePtr              _internalRoot;
    //OSG::DirectionalLightPtr  _headlight;


    vrj::GlContextData<context_data>  mContextData;

    /* OSG::Matrix OSGMat; */

  public:
    gadget::PositionInterface   mWand;

    static OSG::UInt32 OSG_MAIN_ASPECT_ID;
};


#endif
