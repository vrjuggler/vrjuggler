/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2009 by Iowa State University
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

  // If grab pressed --> Store obj matrix, IsGrabbed
         if(does_isect && grab_edge_on)
         {
            // Store xform values that will be needed next frame
            gmtl::Matrix44f wand_M_vw;
            gmtl::invert(wand_M_vw, vw_M_wand);
            gmtl::Matrix44f xform_vw_M_o = traverser->getXformMat();

            // Store the stable wand to object xform
            mXform_wand_M_o =  wand_M_vw * xform_vw_M_o;

            mLocalSelectionState = IsSelected;
            mSelectStateChanged = true;
            vprDEBUG(vprDBG_ALL, vprDBG_WARNING_LVL) << "GrabController: IsSelected --> IsGrabbed.\n" << vprDEBUG_FLUSH;           
         }


         case IsSelected:
      {         
         // Update the model position
         // --- Move to new position --- //
         // - Calculate the new object position (nMo - node to object - for local coord space)
         // xform: vwMo = vm_M_plat * plat_M_wand * wand_M_o
         // update: vwMo = vwMn * nMo
         // Calculate the new desired obj position in the vw
         gmtl::Matrix44f xform_vw_M_o;
         xform_vw_M_o = vw_M_plat * plat_M_wand * mXform_wand_M_o;

         // --- Update the position (solve for nMo) --- //
         // Calculate node to vw xform (we already have node xform applied at this time)
         // nMvw = nMo * oMvw -- node xform = local * inv(obj)
         // nMo = nMvw * vwMo
         gmtl::Matrix44f xform_n_M_o, xform_n_M_vw;
         gmtl::Matrix44f cur_xform_oMvw(traverser->getXformMat());
         gmtl::invert(cur_xform_oMvw);

         xform_n_M_vw = getEntity()->getPos() * cur_xform_oMvw;
         xform_n_M_o = xform_n_M_vw * xform_vw_M_o;
         
#ifdef DEBUG_DRAW_VIEW_CONTROLLER
         mDraw_xform_vm_M_o =  xform_vw_M_o;
         gmtl::invert(mDraw_cur_xform_vm_M_n,xform_n_M_vw);
#endif

         //getEntity()->setPos(xform_n_M_o);      // Set the position
         
         // -- Handle State transitions --- //
         // Let go of grab --> IsSelected
         if(!grab_pressed)
         {
            mLocalSelectionState = IsHighlighted;
            mSelectStateChanged = true;           
         }         
      }      
      break;

