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

#include <vrj/vjConfig.h>
#include <vrj/Display/Projection.h>

namespace vrj
{
   
float Projection::mNearDist = 0.1f;
float Projection::mFarDist = 10000.0f;


//: Helper to the frustum apex and corners in model coordinates
//!NOTE: The normal frustum is in camera (clip) coordinates
//+      and the model is in model (eye) coordinates.
//+      The matrix viewMat transforms from eye to clip
void Projection::getFrustumApexAndCorners(Vec3& apex, Vec3& ur, Vec3& lr, Vec3& ul, Vec3& ll)
{
   Matrix view_mat_inv;
   view_mat_inv.invert(mViewMat);   // Get the inverse matrix

   //float near_dist = mFocusPlaneDist;
   // User like triangles to get the params for the focus surface
   float mult_factor = mFocusPlaneDist/mFrustum[Frustum::VJ_NEAR];
   float bot = mFrustum[Frustum::VJ_BOTTOM]*mult_factor;
   float left = mFrustum[Frustum::VJ_LEFT]*mult_factor;
   float top = mFrustum[Frustum::VJ_TOP]*mult_factor;
   float right = mFrustum[Frustum::VJ_RIGHT]*mult_factor;

   // Create points in clip space
   Vec3 apexClip(0.0f, 0.0f, 0.0f);
   Vec3 urClip(right, top, -mFocusPlaneDist);
   Vec3 lrClip(right, bot, -mFocusPlaneDist);
   Vec3 ulClip(left, top, -mFocusPlaneDist);
   Vec3 llClip(left, bot, -mFocusPlaneDist);

   apex.xformFull(view_mat_inv, apexClip);
   ur.xformFull(view_mat_inv, urClip);
   lr.xformFull(view_mat_inv, lrClip);
   ul.xformFull(view_mat_inv, ulClip);
   ll.xformFull(view_mat_inv, llClip);
}


std::ostream& Projection::outStream(std::ostream& out)
{
   out << "eye: ";
   switch(mEye)
   {
   case Projection::LEFT:
      out << "Left";
      break;
   case Projection::RIGHT:
      out << "Right";
      break;
   }
   out << "  Frustum: " << mFrustum;
   return out;
}

void Projection::setNearFar(float near_val, float far_val)
{
   vjDEBUG(vjDBG_ALL,vjDBG_STATE_LVL) << clrOutNORM(clrCYAN,"vjProjection::setNearFar:")
                           << "near: " << near_val << " far:" << far_val
                           << std::endl << vjDEBUG_FLUSH;
   mNearDist = near_val;
   mFarDist = far_val;
}

VJ_IMPLEMENT(std::ostream&) operator<<(std::ostream& out, Projection& proj)
{
   return proj.outStream(out);
}

};
