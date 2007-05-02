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
   , mViewport(NULL)
   , mFocusPlaneDist(1.0f)
{
   ;
}

Projection::Projection(const Projection& p)
   : mViewMat(p.mViewMat)
   , mFrustum(p.mFrustum)
   , mEye(p.mEye)
   , mViewport(p.mViewport)
   , mFocusPlaneDist(p.mFocusPlaneDist)
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

   // vprDEBUG(vprDBG_ALL, vprDBG_HVERB_LVL) << "GetApex:\nview mat:\n" << mViewMat << "\nviewMatInv:\n" << view_mat_inv << std::endl << vprDEBUG_FLUSH;


   //float near_dist = mFocusPlaneDist;
   // Use like triangles to get the params for the focus surface
   float mult_factor = mFocusPlaneDist/mFrustum[Frustum::VJ_NEAR];
   float bot = mFrustum[Frustum::VJ_BOTTOM]*mult_factor;
   float left = mFrustum[Frustum::VJ_LEFT]*mult_factor;
   float top = mFrustum[Frustum::VJ_TOP]*mult_factor;
   float right = mFrustum[Frustum::VJ_RIGHT]*mult_factor;

   // Create points in clip space
   gmtl::Point3f apexClip(0.0f, 0.0f, 0.0f);
   gmtl::Point3f urClip(right, top, -mFocusPlaneDist);
   gmtl::Point3f lrClip(right, bot, -mFocusPlaneDist);
   gmtl::Point3f ulClip(left, top, -mFocusPlaneDist);
   gmtl::Point3f llClip(left, bot, -mFocusPlaneDist);

   apex = view_mat_inv * apexClip;
   ur = view_mat_inv * urClip;
   lr = view_mat_inv * lrClip;
   ul = view_mat_inv * ulClip;
   ll = view_mat_inv * llClip;
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

void Projection::setNearFar(float near_val, float far_val)
{
   vprDEBUG(vprDBG_ALL,vprDBG_STATE_LVL)
      << clrOutNORM(clrCYAN,"vjProjection::setNearFar:")
      << "near: " << near_val << " far:" << far_val << std::endl
      << vprDEBUG_FLUSH;
   mNearDist = near_val;
   mFarDist = far_val;
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
