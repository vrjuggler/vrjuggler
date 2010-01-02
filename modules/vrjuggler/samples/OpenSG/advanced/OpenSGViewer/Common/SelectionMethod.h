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

#ifndef SELECTION_METHOD_H_
#define SELECTION_METHOD_H_

#include <OpenSG/OSGMatrix.h>
#include <OpenSG/OSGNode.h>
#include <OpenSG/OSGGeoPropPositions.h>
#include <OpenSG/OSGGeoPropPtrs.h>
#include <OpenSG/OSGTransform.h>

#include <gmtl/Matrix.h>
#include <gmtl/Generate.h>
#include <gmtl/Output.h>
#include <gmtl/Vec.h>


class OpenSGViewer;


class SelectionMethod
{
public:
   enum SelectionState { NotSelected=0,   /**< The entity is not currently selected (no isect) */ 
                         IsHighlighted=1, /**< The entity is highlighted, but not selected (yet) */
                         IsSelected=2     /**< The entity is currently selected (does isect */
                        };
public:
   SelectionMethod()
      : mViewer(NULL)
   {;}

   virtual void init()
   {
      mSelectStateChanged = false;
      mSelectionState = SelectionMethod::NotSelected;
      mOldSelectionState = SelectionMethod::NotSelected;
   }

   virtual void updateSelection() = 0;

   virtual gmtl::Matrix44f getSelectionPoint()
   { return mSelectionPoint_vw; }

   virtual OSG::NodePtr getCurSelection()
   { return mCurSelectedNode; }

   /** Is a node currently selected */
   virtual bool isSelected()
   { return mSelectionState == IsSelected; }

   /** Was a node previously selected */
   virtual bool wasSelected()
   { return mOldSelectionState == IsSelected; }

   virtual void setViewer(OpenSGViewer* viewer)
   { mViewer = viewer; }

protected:
   OSG::NodePtr   mCurSelectedNode;
   
   bool           mSelectStateChanged;       /**< True if the state changed. Needed for rendering and sound */   
   SelectionState mSelectionState;      /**< The selection state for the local grabber */
   SelectionState mOldSelectionState;   /**< Needed for some triggering of state changing events */

   gmtl::Matrix44f mSelectionPoint_vw;       /**< The point of selection in vw coordinated */
   OpenSGViewer*   mViewer;                  /**< The viewer that we are connected to */
};

#endif

