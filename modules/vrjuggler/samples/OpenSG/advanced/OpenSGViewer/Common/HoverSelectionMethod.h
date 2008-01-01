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

#ifndef HOVER_SELECTION_METHOD_H_
#define HOVER_SELECTION_METHOD_H_

#include <Common/SelectionMethod.h>

#include <OpenSG/OSGMatrix.h>
#include <OpenSG/OSGNode.h>
#include <OpenSG/OSGGeoPropPositions.h>
#include <OpenSG/OSGGeoPropPtrs.h>
#include <OpenSG/OSGTransform.h>

#include <OpenSG/OSGSimpleMaterial.h>

#include <gmtl/Matrix.h>

#include <boost/bind.hpp>
#include <boost/function.hpp>


class HoverSelectionMethod : public SelectionMethod
{
public:
   HoverSelectionMethod()
      : mInitialized(false), mWandRepAttached(false)
   {;}

   virtual void updateSelection();

   virtual OSG::NodePtr getCurSelection()
   {  return mCurSelectedNode; }

   /** Update the visuals for the selection method */
   void updateVisualState();

   /** Initialization method
   * Called by creator when the control should be initialized.
   */
   virtual void init();   

   bool intersect(OSG::NodePtr node, gmtl::Matrix44f wandWorldPos);

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
   void updateWandRep(OSG::Matrix osg_wand_pos);

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

   OSG::NodePtr               mHighlightNode;         /**< The geometry for the highlight box */
   OSG::GeoPositions3fPtr     mHighlightPoints;       /**< The point geometry for the box */
   OSG::SimpleMaterialPtr     mHighlightMaterial;     /**< The material for the highlight geometry */


   bool                 mWandRepAttached;
   OSG::NodePtr         mWandRepTransNode;
   OSG::TransformPtr    mWandRepTrans;

};

#endif

