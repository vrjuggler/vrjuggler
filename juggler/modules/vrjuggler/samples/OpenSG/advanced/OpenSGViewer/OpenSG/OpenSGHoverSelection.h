#ifndef OPENSGGRABCONTROLLER_H
#define OPENSGGRABCONTROLLER_H

#include <View/GrabController.h>
#include <View/OpenSG/OpenSGSubSystem.h>

#include <OpenSG/OSGMatrix.h>
#include <OpenSG/OSGNode.h>
#include <OpenSG/OSGGeoPropPositions.h>
#include <OpenSG/OSGGeoPropPtrs.h>
#include <OpenSG/OSGTransform.h>

#include <OpenSG/OSGSimpleMaterial.h>

#include <Common/HelperMacros.h>
#include <gmtl/Matrix.h>

class OpenSGHoverSelection : public GrabController
{
public:
   OpenSGGrabController()
      : mInitialized(false), mCurHightlightState(false), mWandRepAttached(false),
        mWandRepTransNode(osg::NullFC), mWandRepTrans(osg::NullFC)
   {;}

   virtual ~OpenSGGrabController()
   {;}

   /** Initialization method
   * Called by creator when the control should be initialized.
   * @pre Entity exists and is set for view
   * @post GFX specific data structures are created and integrated
   * @note Classes MUST call their parent class's init function first thing
   *       to guarantee correct initialization and dependencies satisfied.
   */
   virtual void init();

   /** Update method.
   * Called when the controller and viewer should be updated
   */
   virtual void preUpdate(Traverser* traverser);

   /** Check for intersection
   * @returns true If the point is within the boundign volume
   *                 of the associated view.
   */
   virtual bool intersect(gmtl::Matrix44f wandWorldPos);

   /** Draw debugging geometry */
   void drawDebugGeom();

protected:
   /** Set the current state of the highlight */
   void setHighlightState(bool newState);

   /** Set the current state of the grabbed */
   void setHighlightColor(float r, float b, float g);

   /** Initialize the selection geometry */
   void initSelectionGeom();

   /** Update the highlight geometry */
   void updateHighlightGeom();

protected:
   /** Initialize the wand representation */
   void initWandRep();

   /** Update the virtual rep of the wand */
   void updateWandRep(osg::Matrix osg_wand_pos);

   /** Attach a wand representation to the base dataspace graph */
   void attachWandRep();

protected:     // ** Visual Rep stuff **//
   bool  mInitialized;     /**< Has everything been initialized */
   bool  mCurHightlightState; /**< The current highlighting state */

   osg::NodePtr               mHighlightNode;         /**< The geometry for the highlight box */
   osg::GeoPositions3fPtr     mHighlightPoints;       /**< The point geometry for the box */
   osg::SimpleMaterialPtr     mHighlightMaterial;     /**< The material for the highlight geometry */


   bool                 mWandRepAttached;
   osg::NodePtr         mWandRepTransNode;
   osg::TransformPtr    mWandRepTrans;
};

#endif //OPENSGGRABCONTROLLER_H

