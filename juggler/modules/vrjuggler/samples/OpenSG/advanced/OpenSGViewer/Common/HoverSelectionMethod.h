#ifndef HOVER_SELECTION_METHOD_H_
#define HOVER_SELECTION_METHOD_H_

#include <Common/SelectionMethod.h>

class HoverSelectionMethod : public SelectionMethod
{
public:
   virtual void updateSelection();

   virtual OSG::NodePtr getCurSelection()
   {  return mCurSelectedNode; }

   /** Update the visuals for the selection method */
   void updateVisualState();

   /** Initialization method
   * Called by creator when the control should be initialized.
   */
   virtual void init()
   {
      mSelectStateChanged = false;
      mLocalSelctionState = SelectionMethod::NotSelected;
      mOldLocalSelectionState = SelectionMethod::NotSelected;

      initSelectionGeom();          // Initialize the geometry node that we use for marking selection

      #ifdef DEBUG_DRAW_VIEW_CONTROLLER
      // Setup drawing of debug geometry
      boost::function<void> gl_callback = boost::bind(&OpenSGGrabController::drawDebugGeom, this);
      Viewer* base_viewer = DataSpace::instance()->getViewer();
      OpenSGViewer* osg_viewer = dynamic_cast<OpenSGViewer*>(base_viewer);
      vprASSERT(osg_viewer != NULL);
      
      osg_viewer->addGlCallback(gl_callback);      // Add callback for drawing debug geometry
      #endif
   }

   void initLocalGrabControllerData();

   /** Update the grab state */
   virtual void preUpdate(Traverser* traverser);

   bool intersect(OSG::NodePtr node, gmtl::Matrix44f wandWorldPos)

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

protected:
   /** Matrices for tracking transformation of grabbing.
   * See GrabController::update for description
   */
   gmtl::Matrix44f   mXform_wand_M_o;     /**< Xfrom from the wand to the object */

#ifdef DEBUG_DRAW_VIEW_CONTROLLER
   // Debug matrices
   gmtl::Matrix44f   mDraw_xform_vm_M_o;      // Desired position of object
   gmtl::Matrix44f   mDraw_cur_xform_vm_M_o;  // Desired position of object
   gmtl::Matrix44f   mDraw_cur_xform_vm_M_n;  /// Desired position of object
   gmtl::Matrix44f   mDraw_xform_vw_M_wand;
   gmtl::Matrix44f   mDraw_xform_vw_M_plat;
#endif

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


};

#endif

