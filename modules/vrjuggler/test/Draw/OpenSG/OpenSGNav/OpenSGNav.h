#ifndef _OSG_NAV_
#define _OSG_NAV_

#include <iostream>
#include <iomanip>

/*-----------------------------Juggler includes-------------------------------*/
#include <vrj/vrjConfig.h>
#include <vrj/Draw/OGL/GlApp.h>
#include <gmtl/Matrix.h>
#include <gmtl/MatrixOps.h>
#include <gmtl/Generate.h>

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
#include <OpenSG/OSGMatrixCamera.h>
#include <OpenSG/OSGDirectionalLight.h>
#include <OpenSG/OSGGroup.h>

/*----------------------------------------------------------------------------*/

#include <vrj/Draw/OpenSG/OpenSGApp.h>



class OpenSGNav : public vrj::OpenSGApp
{
public:
    OpenSGNav(vrj::Kernel* kern)
       : vrj::OpenSGApp(kern)
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


    virtual void initScene();
    virtual OSG::NodePtr getSceneRoot()
    { return mSceneRoot; }

    void initRenderer();

    virtual void draw();

    virtual void contextInit();

    virtual void preFrame();

    void setModelFileName(std::string filename)
    {
        std::cout << "OpenSGNav::setModelFileName called\n";
        mFileToLoad = filename;
    }

  private:
    void initGLState();

  private:
    std::string   mFileToLoad;

    OSG::NodePtr        mSceneRoot;       /**< The root of the scene */
    OSG::TransformPtr   mSceneTransform;  /**< Transform core */
    OSG::NodePtr        mModelRoot;       /**< Root of the loaded model */

    OSG::NodePtr  mLightNode;       /**< The light node */
    OSG::NodePtr  mLightCart;       /**< The cart for the light */

  public:
    gadget::PositionInterface   mWand;
    gadget::DigitalInterface   mButton0;
    gadget::DigitalInterface   mButton1;
    gadget::DigitalInterface   mButton2;
    float  velocity;
};


#endif
