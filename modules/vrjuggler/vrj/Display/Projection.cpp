/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998, 1999, 2000 by Iowa State University
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
 * -----------------------------------------------------------------
 * File:          $RCSfile$
 * Date modified: $Date$
 * Version:       $Revision$
 * -----------------------------------------------------------------
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

#include <vjConfig.h>
#include <Kernel/vjProjection.h>

float vjProjection::mNearDist = 0.1f;
float vjProjection::mFarDist = 10000.0f;


//: Helper to the frustum apex and corners in model coordinates
//!NOTE: The normal frustum is in camera (clip) coordinates
//+      and the model is in model (eye) coordinates.
//+      The matrix viewMat transforms from eye to clip
void vjProjection::getFrustumApexAndCorners(vjVec3& apex, vjVec3& ur, vjVec3& lr, vjVec3& ul, vjVec3& ll)
{
   vjMatrix view_mat_inv;
   view_mat_inv.invert(mViewMat);   // Get the inverse matrix

   //float near_dist = mFocusPlaneDist;
   // User like triangles to get the params for the focus surface
   float mult_factor = mFocusPlaneDist/mFrustum[vjFrustum::VJ_NEAR];
   float bot = mFrustum[vjFrustum::VJ_BOTTOM]*mult_factor;
   float left = mFrustum[vjFrustum::VJ_LEFT]*mult_factor;
   float top = mFrustum[vjFrustum::VJ_TOP]*mult_factor;
   float right = mFrustum[vjFrustum::VJ_RIGHT]*mult_factor;

   // Create points in clip space
   vjVec3 apexClip(0.0f, 0.0f, 0.0f);
   vjVec3 urClip(right, top, -mFocusPlaneDist);
   vjVec3 lrClip(right, bot, -mFocusPlaneDist);
   vjVec3 ulClip(left, top, -mFocusPlaneDist);
   vjVec3 llClip(left, bot, -mFocusPlaneDist);

   apex.xformFull(view_mat_inv, apexClip);
   ur.xformFull(view_mat_inv, urClip);
   lr.xformFull(view_mat_inv, lrClip);
   ul.xformFull(view_mat_inv, ulClip);
   ll.xformFull(view_mat_inv, llClip);
}


std::ostream& vjProjection::outStream(std::ostream& out)
{
   out << "eye: ";
   switch(mEye)
   {
   case vjProjection::LEFT:
      out << "Left";
      break;
   case vjProjection::RIGHT:
      out << "Right";
      break;
   }
   out << "  Frustum: " << mFrustum;
   return out;
}

VJ_IMPLEMENT(std::ostream&) operator<<(std::ostream& out, vjProjection& proj)
{
   return proj.outStream(out);
}
