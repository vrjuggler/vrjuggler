/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2008 by Iowa State University
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

#ifndef _OSG_VIEWER_
#define _OSG_VIEWER_

/* ---- Feature list ----------

- Viewplatform
- Pointing grabbing
- Grabbing using pinch gloves
- Slicing using cubic mouse interfacing

--------------------------
*/                    

#include <iostream>
#include <iomanip>

/*-----------------------------OpenSG includes--------------------------------*/
#include <OpenSG/OSGGL.h>
#include <OpenSG/OSGNode.h>
#include <OpenSG/OSGTransform.h>
/*----------------------------------------------------------------------------*/

/*-----------------------------Juggler includes-------------------------------*/
#include <vrj/vrjConfig.h>
#include <vrj/Draw/OpenGL/App.h>
#include <vrj/Draw/OpenSG/App.h>
#include <vpr/Util/Singleton.h>

#include <gmtl/Matrix.h>
#include <gmtl/MatrixOps.h>
#include <gmtl/Generate.h>

#include <gadget/Type/PositionInterface.h>
#include <gadget/Type/AnalogInterface.h>
#include <gadget/Type/DigitalInterface.h>

#include <vrj/Draw/OpenGL/ContextData.h>

// Common viewer helper classes
#include <Common/User.h>
#include <Common/ViewPlatform.h>
#include <Common/SelectionMethod.h>


class OpenSGViewer : public vrj::opensg::App
{
public:
    OpenSGViewer(vrj::Kernel* kern)
       : vrj::opensg::App(kern)
       , velocity(0.0f)
    {
        std::cout << "OpenSGViewer::OpenSGViewer called\n";
        mFileToLoad = std::string("");
        mUser = User::instance();
    }

    virtual ~OpenSGViewer (void)
    {
        std::cout << "OpenSGViewer::~OpenSGViewer called\n";
    }

    // Handle any initialization needed before API
    virtual void init();
        
    /** Initialize the scene graph */
    virtual void initScene();

    /** Return the scene root for this application */
    virtual OSG::NodePtr getScene()
    { return mSceneRoot; }

    /** Return the root of the firtual world */
    virtual OSG::NodePtr getWorldRoot()
    { return mWorldRoot; }

    virtual void draw();

    virtual void contextInit();

    virtual void preFrame();

    void setModelFileName(std::string filename)
    {
        std::cout << "OpenSGViewer::setModelFileName: Set filename: [" << filename << "]\n";
        mFileToLoad = filename;
    }

    User* getUser()
    { return mUser; }

    OSG::NodePtr getCurSelectedNode()
    { return mSelectionMethod->getCurSelection(); }
    std::vector<OSG::NodePtr> getSelectableNodes()
    { return mSelectableNodes; }

public:     // RECONFIG STUFF
   virtual bool configCanHandle(jccl::ConfigElementPtr element)
   {  return configCanHandleElement(element); }

   // This function is so that others can query this object to
   // see if it can be configured with the given information
   static bool configCanHandleElement(jccl::ConfigElementPtr element)
   {
      const std::string element_type(element->getID());
      return (std::string("opensg_viewer_app") == element_type);         
   }
     
   //: Are any application dependencies satisfied
   virtual bool depSatisfied()
   { return true; }

   //! NOTE: Inherited from jccl::ConfigElementHandler
   virtual bool configAdd(jccl::ConfigElementPtr element);

   //! NOTE: INherited from jccl::ConfigElementHandler
   virtual bool configRemove(jccl::ConfigElementPtr element)
   { vprASSERT(false); return false; }

  private:
    void initGLState();

  private:
    std::string         mFileToLoad;      /**< Filename of the file to load */

    User*               mUser;            /**< The user that this is a viewer for */

    std::vector<OSG::NodePtr> mSelectableNodes; /**< List of selectable nodes */
    SelectionMethod*          mSelectionMethod; /**< The current selection method being used */

    //   mSceneRoot:[mSceneTransform]
    //         |
    //   mLightNode:[DirectionalLight]
    //         |
    //   mLightBeacon:[Transform]
    //         |
    //     mModelRoot
    OSG::NodePtr        mSceneRoot;       /**< The root of the scene */
    OSG::TransformPtr   mSceneTransform;  /**< Transform core */
    OSG::NodePtr        mWorldRoot;       /**< Root of the world */

    OSG::NodePtr        mLightNode;       /**< Light node to use */
    OSG::NodePtr        mLightBeacon;     /**< A beacon for the light */

  public:
    gadget::PositionInterface  mWandPos;     /**< The position of the wand */
    gadget::DigitalInterface   mButton0;     /**< Wand button 0 */
    gadget::DigitalInterface   mButton1;     /**< Wand button 1 */
    gadget::DigitalInterface   mButton2;     /**< Wand button 2 */
    float  velocity;
};


#endif
