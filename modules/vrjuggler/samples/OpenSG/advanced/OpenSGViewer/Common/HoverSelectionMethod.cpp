/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2007 by Iowa State University
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

#include <Common/HoverSelectionMethod.h>

#include <OpenSG/OSGBaseTypes.h>
#include <OpenSG/OSGTransform.h>
#include <OpenSG/OSGGeometry.h>
#include <OpenSG/OSGGeoPropPtrs.h>
#include <OpenSG/OSGSimpleGeometry.h>

#include <OpenSG/OSGDynamicVolume.h>
#include <OpenSG/OSGVector.h>

#include <gmtl/Generate.h>
#include <gmtl/External/OpenSGConvert.h>
#include <gmtl/Output.h>
#include <gmtl/Vec.h>

#include <Common/User.h>
#include <OpenSGViewer.h>


namespace
{
   void drawAxis(const float& len)
   {
      glPushMatrix();
         gmtl::Vec3f x_axis(len,0.0f,0.0f); gmtl::Vec3f y_axis(0.0f, len, 0.0f);
         gmtl::Vec3f z_axis(0.0f, 0.0f, len); gmtl::Vec3f origin(0.0f, 0.0f, 0.0f);
         glBegin(GL_LINES);
            glVertex3fv(origin.mData); glVertex3fv(x_axis.mData);
            glVertex3fv(origin.mData); glVertex3fv(y_axis.mData);
            glVertex3fv(origin.mData); glVertex3fv(z_axis.mData);
         glEnd();
      glPopMatrix();
   }
}

void HoverSelectionMethod::init()
{
   SelectionMethod::init();

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

void HoverSelectionMethod::updateSelection()
{
   vprASSERT(mInitialized && "updateSelection called before selection method was initialized");

   //   vwMo - Virtual World to object xform,
   //   vwMi - virtual world to "interface" (wand),
   //   iMo - Interface to object
   //   vwMn - virtual world to object containing node,
   //   nMo - Node to object (local object xform)
   //   Interface to object is estabilished at "grab time" and from then on while grabbed, wMo is calculated
   //
   // @todo Take into account the fact that the object is under and xform itself.
   //
   // Transformations with _Mo_ are old values from previous frame
   //
   // Transformation to new position
   // vwMo = vm_M_plat * plat_M_wand * wand_Mo_plat * plat_Mo_vw * vw_Mo_o
   //

   User* cur_user = mViewer->getUser();
   std::vector<OSG::NodePtr> selectable_nodes = mViewer->getSelectableNodes();

   bool grab_pressed = ( cur_user->getButton(1)->getData() == gadget::Digital::ON);
   bool grab_edge_on = ( cur_user->getButton(1)->getData() == gadget::Digital::TOGGLE_ON );
      
   gmtl::Matrix44f vw_M_plat = cur_user->getXform_vwMrw();     // Get the xform from vw to real world (platform)

   gmtl::Matrix44f plat_M_wand, vw_M_wand;
   plat_M_wand = cur_user->getWandPos()->getData();      // Get the wand matrix
   vw_M_wand = vw_M_plat * plat_M_wand;                   // Get the wand matrix in virtual world coords

#ifdef DEBUG_DRAW_VIEW_CONTROLLER
   mDraw_xform_vw_M_wand = vw_M_wand;
   mDraw_xform_vw_M_plat = vw_M_plat;
   mDraw_cur_xform_vm_M_o = traverser->getXformMat();
#endif

   // Test for first intersection (or intersection against current node)
   // for (each selectable node)
   // - If isects
   bool           does_isect(false);
   OSG::NodePtr   cur_node(mCurSelectedNode);
   
   if(cur_node != OSG::NullFC)
   {
       does_isect = this->intersect(cur_node, vw_M_wand);
   }
   else
   {
      std::vector<OSG::NodePtr>::iterator node_i = selectable_nodes.begin();
      while((!does_isect) && (node_i != selectable_nodes.end()))
      {
         cur_node = *node_i;
         does_isect = this->intersect(cur_node, vw_M_wand);
         ++node_i;
      }
   }
      
   if(does_isect)
   {
      mCurSelectedNode = cur_node;
   }
   else
   {
      mCurSelectedNode = OSG::NullFC;
   }

   // -- UPDATE LOCAL STATE --- //
   // Initially no states have changed
   mSelectStateChanged = false;        // Intially state has not changed
   mOldSelectionState = mSelectionState;

   // Switch on selection state
   switch (mSelectionState)
   {
   case NotSelected:
      {
         if(does_isect)
         {
            vprDEBUG(vprDBG_ALL, vprDBG_CONFIG_LVL) << "GrabController: Intersection with object: " << std::endl << vprDEBUG_FLUSH;

            mSelectionState = IsHighlighted;            
            mSelectStateChanged = true;   // Now in selected state
            vprDEBUG(vprDBG_ALL, vprDBG_WARNING_LVL) << "GrabController: NotSelected --> IsSelected.\n" << vprDEBUG_FLUSH;
         }         
      }
      break;
   case IsHighlighted:
      {
         // If we don't isect any more --> NotSelected
         if(!does_isect)
         {
            mSelectionState = NotSelected;
            mSelectStateChanged = true;
            vprDEBUG(vprDBG_ALL, vprDBG_WARNING_LVL) << "GrabController: IsSelected --> NotSelected.\n" << vprDEBUG_FLUSH;
         }

         // If grab pressed --> Store obj matrix, IsGrabbed
         if(does_isect && grab_edge_on)
         {
            // Store xform values that will be needed next frame
            gmtl::Matrix44f wand_M_vw;
            gmtl::invert(wand_M_vw, vw_M_wand);
            mSelectionPoint_vw = wand_M_vw;
            
            mSelectionState = IsSelected;
            mSelectStateChanged = true;
            vprDEBUG(vprDBG_ALL, vprDBG_WARNING_LVL) << "GrabController: IsSelected --> IsGrabbed.\n" << vprDEBUG_FLUSH;           
         }
      }
      break;
   case IsSelected:
      {         
         // -- Handle State transitions --- //
         // Let go of grab --> IsSelected
         if(!grab_pressed)
         {
            mSelectionState = IsHighlighted;
            mSelectStateChanged = true;           
         }         
      }      
      break;
   default:
      vprASSERT(false && "mSelectionState is in an unknown state, this is bad.");
   }

   // Update the visuals based upon this information
   updateVisualState();
   
}


void HoverSelectionMethod::updateVisualState()
{
   // Check for state change to visual rep
   if(mSelectStateChanged)    // Only update the visual rep when the state changes
   {
      // First determine if highlighting should even be on
      if(NotSelected == mSelectionState)
      { 
         vprDEBUG(vprDBG_ALL, vprDBG_WARNING_LVL) << "Setting highlight state false: (NotSelected == mSelectionState)\n" << vprDEBUG_FLUSH;
         setHighlightState(false); 
      }
      else
      {  
         vprDEBUG(vprDBG_ALL, vprDBG_WARNING_LVL) << "Setting highlight state true: !((NotSelected == mSelectionState) || (UnSelected == mRemoteState))\n" << vprDEBUG_FLUSH;
         setHighlightState(true); 
         
         float red(0.0f), green(0.0f), blue(0.0f);

         if(IsHighlighted == mSelectionState)
         {  blue = 1.0f; }
         else if(IsSelected == mSelectionState)
         {  green = 0.5f; }         
      
         vprDEBUG(vprDBG_ALL, vprDBG_WARNING_LVL) << "Setting highlight color: [" << red << "," 
                                                   << green << "," << blue << "]\n" << vprDEBUG_FLUSH;
         setHighlightColor(red,green,blue);
      }

   }   

   if(mSelectionState != NotSelected)
   {
      //vprDEBUG(vprDBG_ALL, vprDBG_WARNING_LVL) << "Updating hightlight geometry.\n" << vprDEBUG_FLUSH;
      updateHighlightGeom();        // Update the geometry (only does this when selected)
   }
}

/** Check for intersection
* @returns true If the point is within the boundign volume
*                 of the associated view.
*/
bool HoverSelectionMethod::intersect(OSG::NodePtr node, gmtl::Matrix44f wandWorldPos)
{
   // XXX: The bounding volume returned from getWorldVolume is in the platform (real world)
   //      coordinate frame, so we need to transform wandWorldPos in that coordinate
   //      frame from the vw coordinate frame it starts in
   gmtl::Matrix44f plat_M_virt =  mViewer->getUser()->viewPlatform().getTransform_platMvirt();
   gmtl::Matrix44f plat_M_wand = plat_M_virt * wandWorldPos;

   OSG::Matrix osg_wand_pos;
   gmtl::set(osg_wand_pos, plat_M_wand);
   OSG::Pnt3f osg_wand_pos_pnt(0.0f, 0.0f, 0.0f);
   osg_wand_pos.multFullMatrixPnt(osg_wand_pos_pnt);

   OSG::DynamicVolume bounding_volume;
   node->getWorldVolume(bounding_volume);

   bool does_isect = bounding_volume.intersect(osg_wand_pos_pnt);

   // --- Debugging
   OSG::Matrix osg_wand_pos_vw;
   gmtl::set(osg_wand_pos_vw, wandWorldPos);
   updateWandRep(osg_wand_pos_vw);        // Update the wand pos rep

   OSG::Pnt3f min_pnt, max_pnt;
   bounding_volume.getBounds(min_pnt, max_pnt);

   gmtl::Vec3f wand_pos_pnt;
   wand_pos_pnt = gmtl::makeTrans<gmtl::Vec3f>(plat_M_wand);
   //std::cout << "Wand pos [plat]: " << wand_pos_pnt << std::endl;
   //std::cout << "Bounding vol (plat): \n" << std::flush;
   //bounding_volume.dump();

   return does_isect;
}


/** Set the current state of the selection */
void HoverSelectionMethod::setHighlightState(bool newState)
{
   if(!mInitialized)
   {
      initSelectionGeom();
   }

   // If trying to switch to same state, just return
   if(mCurHightlightState == newState)
   {  return; }

   mCurHightlightState = newState;

   // Parent on world root for now
   OSG::NodePtr vw_root = mViewer->getWorldRoot();  // Get the base root node of the vw

   if(newState == true)    // Set to highlighted
   {
      vprASSERT(mHighlightNode->getParent() == OSG::NullFC);      // We should not have parent

      OSG::beginEditCP(vw_root);
         vw_root->addChild(mHighlightNode);
      OSG::endEditCP(vw_root);
   }
   else                    // Set not highlighted
   {
      if(mHighlightNode->getParent() != OSG::NullFC)
      {
         OSG::beginEditCP(vw_root);
            vw_root->subChild(mHighlightNode);
         OSG::endEditCP(vw_root);
      }
   }
}

/** Set the current state of the grabbed */
void HoverSelectionMethod::setHighlightColor(float r, float g, float b)
{
   vprASSERT (mHighlightMaterial != OSG::NullFC);

   // Set color based on state
   OSG::beginEditCP(mHighlightMaterial);
      mHighlightMaterial->setDiffuse (OSG::Color3f(r,g,b));     
   OSG::endEditCP(mHighlightMaterial);
}


/*! Update the highlight for a moved object. Does not handle changing the
    object, this is done by highlightChanged().
 */
void HoverSelectionMethod::updateHighlightGeom(void)
{
   //vprASSERT(mSelected);
   vprASSERT(mHighlightNode != OSG::NullFC);
   vprASSERT(mCurSelectedNode != OSG::NullFC);

    // calc the world bbox of the highlight object
    OSG::DynamicVolume vol;
    mCurSelectedNode->getWorldVolume(vol);

    // We have to transform the volume back into virtual world coord system (instead of platform coord system)
    // because this bounding volume is drawn as a child of the root OSG node (which has the platform xform on it)
    gmtl::Matrix44f vw_M_plat =  mViewer->getUser()->viewPlatform().getTransform_virtMplat();
    OSG::Matrix osg_vw_M_plat;
    gmtl::set(osg_vw_M_plat, vw_M_plat);

    vol.transform(osg_vw_M_plat);      // Transform back into virtual world

    OSG::Pnt3f min_pt,max_pt;
    vol.getBounds(min_pt, max_pt);

    OSG::beginEditCP(mHighlightPoints);
       mHighlightPoints->setValue(OSG::Pnt3f(min_pt[0], min_pt[1], min_pt[2]), 0);
       mHighlightPoints->setValue(OSG::Pnt3f(max_pt[0], min_pt[1], min_pt[2]), 1);
       mHighlightPoints->setValue(OSG::Pnt3f(min_pt[0], max_pt[1], min_pt[2]), 2);
       mHighlightPoints->setValue(OSG::Pnt3f(max_pt[0], max_pt[1], min_pt[2]), 3);
       mHighlightPoints->setValue(OSG::Pnt3f(min_pt[0], min_pt[1], max_pt[2]), 4);
       mHighlightPoints->setValue(OSG::Pnt3f(max_pt[0], min_pt[1], max_pt[2]), 5);
       mHighlightPoints->setValue(OSG::Pnt3f(min_pt[0], max_pt[1], max_pt[2]), 6);
       mHighlightPoints->setValue(OSG::Pnt3f(max_pt[0], max_pt[1], max_pt[2]), 7);
    OSG::endEditCP(mHighlightPoints);

    OSG::beginEditCP(mHighlightNode->getCore(), OSG::Geometry::PositionsFieldMask);
    OSG::endEditCP  (mHighlightNode->getCore(), OSG::Geometry::PositionsFieldMask);
}


/** Initialize the selection geometry */
void HoverSelectionMethod::initSelectionGeom()
{
   vprASSERT(mInitialized == false && "Tried to initialize twice");
      
   mInitialized = true;

   // init as needed
   if (mHighlightMaterial == OSG::NullFC)
   {
      mHighlightMaterial = OSG::SimpleMaterial::create();

      OSG::beginEditCP(mHighlightMaterial);
         mHighlightMaterial->setDiffuse (OSG::Color3f(0,1,0));
         mHighlightMaterial->setLit     (false);
      OSG::endEditCP(mHighlightMaterial);
   }
   if (mHighlightNode == OSG::NullFC)
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
         // Line strip
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

         // Lines
         index->getFieldPtr()->addValue(1);
         index->getFieldPtr()->addValue(5);
         index->getFieldPtr()->addValue(2);
         index->getFieldPtr()->addValue(6);
         index->getFieldPtr()->addValue(3);
         index->getFieldPtr()->addValue(7);
      OSG::endEditCP(index);

      mHighlightPoints = OSG::GeoPositions3f::create();
      OSG::beginEditCP(mHighlightPoints);
         mHighlightPoints->addValue(OSG::Pnt3f(-1, -1, -1));
         mHighlightPoints->addValue(OSG::Pnt3f( 1, -1, -1));
         mHighlightPoints->addValue(OSG::Pnt3f(-1,  1, -1));
         mHighlightPoints->addValue(OSG::Pnt3f( 1,  1, -1));
         mHighlightPoints->addValue(OSG::Pnt3f(-1, -1,  1));
         mHighlightPoints->addValue(OSG::Pnt3f( 1, -1,  1));
         mHighlightPoints->addValue(OSG::Pnt3f(-1,  1,  1));
         mHighlightPoints->addValue(OSG::Pnt3f( 1,  1,  1));
      OSG::endEditCP(mHighlightPoints);

      OSG::GeometryPtr geo=OSG::Geometry::create();
      OSG::beginEditCP(geo);
         geo->setTypes     (type);
         geo->setLengths   (lens);
         geo->setIndices   (index);
         geo->setPositions (mHighlightPoints);
         geo->setMaterial  (mHighlightMaterial);
      OSG::endEditCP(geo);
      OSG::addRefCP(geo);

      mHighlightNode = OSG::Node::create();
      OSG::beginEditCP(mHighlightNode);
         mHighlightNode->setCore(geo);
      OSG::endEditCP(mHighlightNode);
      OSG::addRefCP(mHighlightNode);
   }

   initWandRep();
}

void HoverSelectionMethod::initWandRep()
{
   // --- Initialize wand rep --- //
   // Create the geometyr
   OSG::NodePtr wand_rep_geode = OSG::Node::create();
   OSG::GeometryPtr wand_geo = OSG::makeSphereGeo(3,0.10f);

   OSG::beginEditCP(wand_rep_geode);
      wand_rep_geode->setCore(wand_geo);
   OSG::endEditCP(wand_rep_geode);

   // Create the transform node
   mWandRepTransNode = OSG::Node::create();
   mWandRepTrans = OSG::Transform::create();
   vprASSERT(mWandRepTrans != OSG::NullFC);

   OSG::beginEditCP(mWandRepTransNode);
      mWandRepTransNode->setCore(mWandRepTrans);
      mWandRepTransNode->addChild(wand_rep_geode);
   OSG::endEditCP(mWandRepTransNode);

   // Set material
   OSG::SimpleMaterialPtr geo_material = OSG::SimpleMaterial::create();

   OSG::beginEditCP(geo_material);
   {
      geo_material->setAmbient      (OSG::Color3f(0.2,0.2,0.2));
      geo_material->setDiffuse      (OSG::Color3f(0.8,0.1,0.6));
      geo_material->setEmission     (OSG::Color3f(0.0,0.0,0.0));
      geo_material->setSpecular     (OSG::Color3f(1.0,1.0,1.0));
      geo_material->setShininess    (10.0f);
      geo_material->setTransparency (0.0f);
      geo_material->setColorMaterial(GL_NONE);      // Turns off the object prim colors
   }
   OSG::endEditCP  (geo_material);

   // assign the material to the geometry
   OSG::beginEditCP(wand_geo, OSG::Geometry::MaterialFieldMask );
   {
      wand_geo->setMaterial(geo_material);
   }
   OSG::endEditCP  (wand_geo, OSG::Geometry::MaterialFieldMask );
}

void HoverSelectionMethod::attachWandRep()
{
   // Attach wand rep to root (world) coord for debugging
   OSG::NodePtr vworld_root = mViewer->getWorldRoot();     // Get the world root to attach to

   OSG::beginEditCP(vworld_root);
      vworld_root->addChild(mWandRepTransNode);
   OSG::endEditCP(vworld_root);
}


/*! Update the highlight for a moved object. Does not handle changing the
 object, this is done by highlightChanged().
 */
void HoverSelectionMethod::updateWandRep(OSG::Matrix osg_wand_pos)
{
   if(!mWandRepAttached)
   {
      mWandRepAttached = true;
      attachWandRep();
   }

   vprASSERT(mWandRepTrans != OSG::NullFC);

   OSG::beginEditCP(mWandRepTrans);
      mWandRepTrans->getMatrix().setValue(osg_wand_pos);
   OSG::endEditCP(mWandRepTrans);
}


/*
void HoverSelectionMethod::drawDebugGeom()
{
#ifdef DEBUG_DRAW_VIEW_CONTROLLER
   glPushMatrix();
      glColor3f(1.0f, 0.0f, 0.0f);
      glMultMatrixf( mDraw_xform_vm_M_o.getData());
      drawAxis(20.0f);
   glPopMatrix();

   glPushMatrix();
      glColor3f(0.0f, 1.0f, 0.0f);
      glMultMatrixf( mDraw_cur_xform_vm_M_o.getData());
      drawAxis(20.0f);
   glPopMatrix();

   glPushMatrix();
      glColor3f(0.0f, 0.0f, 1.0f);
      glMultMatrixf( mDraw_cur_xform_vm_M_n.getData());
      drawAxis(20.0f);
   glPopMatrix();

   glPushMatrix();
      glColor3f(0.0f, 1.0f, 1.0f);
      glMultMatrixf( mDraw_xform_vw_M_wand.getData());
      drawAxis(20.0f);
   glPopMatrix();

   glPushMatrix();
      glColor3f(1.0f, 0.0f, 1.0f);
      glMultMatrixf( mDraw_xform_vw_M_plat.getData());
      drawAxis(20.0f);
   glPopMatrix();
#endif
}
*/


