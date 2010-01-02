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

#include <vrj/vrjConfig.h>
#include <iostream>
#include <iomanip>
#include <string>
#include <boost/concept_check.hpp>

#include <gmtl/Matrix.h>
#include <gmtl/MatrixOps.h>
#include <gmtl/Xforms.h>
#include <gmtl/Output.h>

#include <vrj/Util/Debug.h>
#include <vrj/Display/Projection.h>


namespace vrj
{

float Projection::mNearDist = 0.1f;
float Projection::mFarDist = 10000.0f;

Projection::Projection()
   : mEye(LEFT)
   , mFocusPlaneDist(1.0f)
{
   ;
}

void Projection::config(jccl::ConfigElementPtr element)
{
   boost::ignore_unused_variable_warning(element);
}

/**
 * Helper to the frustum apex and corners in model coordinates.
 * @note The normal frustum is in camera (clip) coordinates
 *       and the model is in model (eye) coordinates.
 *       The matrix viewMat transforms from eye to clip.
 */
void Projection::getFrustumApexAndCorners(gmtl::Vec3f& apex,
                                          gmtl::Vec3f& ur, gmtl::Vec3f& lr,
                                          gmtl::Vec3f& ul, gmtl::Vec3f& ll)
   const
{
   gmtl::Matrix44f view_mat_inv;
   gmtl::invert(view_mat_inv, mViewMat);

   //vprDEBUG(vprDBG_ALL, vprDBG_HVERB_LVL)
   //   << "[vrj::Projection::getFrustumApexAndCorners()]" << std::endl;
   //vprDEBUG_NEXTnl(vprDBG_ALL, vprDBG_HVERB_LVL)
   //   << "View matrix:\n" << mViewMat << std::endl;
   //vprDEBUG_NEXTnl(vprDBG_ALL, vprDBG_HVERB_LVL)
   //   << "Inverse view matrix:\n" << view_mat_inv << std::endl
   //   << vprDEBUG_FLUSH;

   //const float near_dist = mFocusPlaneDist;
   // Use like triangles to get the params for the focus surface
   const float mult_factor = mFocusPlaneDist / mFrustum[Frustum::VJ_NEAR];
   const float bot         = mFrustum[Frustum::VJ_BOTTOM] * mult_factor;
   const float left        = mFrustum[Frustum::VJ_LEFT] * mult_factor;
   const float top         = mFrustum[Frustum::VJ_TOP] * mult_factor;
   const float right       = mFrustum[Frustum::VJ_RIGHT] * mult_factor;

   // Create points in clip space
   const gmtl::Point3f apex_clip(0.0f, 0.0f, 0.0f);
   const gmtl::Point3f ur_clip(right, top, -mFocusPlaneDist);
   const gmtl::Point3f lr_clip(right, bot, -mFocusPlaneDist);
   const gmtl::Point3f ul_clip(left, top, -mFocusPlaneDist);
   const gmtl::Point3f ll_clip(left, bot, -mFocusPlaneDist);

   apex = view_mat_inv * apex_clip;
   ur   = view_mat_inv * ur_clip;
   lr   = view_mat_inv * lr_clip;
   ul   = view_mat_inv * ul_clip;
   ll   = view_mat_inv * ll_clip;
}

std::ostream& Projection::outStream(std::ostream& out,
                                    const unsigned int indentLevel)
{
   const int pad_width_dot(20 - indentLevel);
   out.setf(std::ios::left);

   const std::string indent_text(indentLevel, ' ');

   out << indent_text << "vrj::Projection:\n";
   out << indent_text << std::setw(pad_width_dot) << "  Eye " << " ";

   switch(mEye)
   {
   case Projection::LEFT:
      out << "Left";
      break;
   case Projection::RIGHT:
      out << "Right";
      break;
   }
   out << std::endl;
   out << indent_text << std::setw(pad_width_dot)
       << "  Frustum " << " " << mFrustum;
   return out;
}

void Projection::setNearFar(const float nearVal, const float farVal)
{
   vprDEBUG(vprDBG_ALL,vprDBG_STATE_LVL)
      << clrOutNORM(clrCYAN, "[vrj::Projection::setNearFar()] ")
      << "near: " << nearVal << ", far:" << farVal << std::endl
      << vprDEBUG_FLUSH;
   mNearDist = nearVal;
   mFarDist = farVal;
}

void Projection::getNearFar(float& nearVal, float& farVal)
{
   nearVal = mNearDist;
   farVal = mFarDist;
}

VJ_IMPLEMENT(std::ostream&) operator<<(std::ostream& out, Projection& proj)
{
   return proj.outStream(out);
}

}
