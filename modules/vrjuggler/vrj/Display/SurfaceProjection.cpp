/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2011 by Iowa State University
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
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

#include <vrj/vrjConfig.h>

#include <iomanip>
#include <sstream>
#include <string>

#include <gmtl/Vec.h>
#include <gmtl/Matrix.h>
#include <gmtl/MatrixOps.h>
#include <gmtl/Generate.h>
#include <gmtl/Output.h>
#include <gmtl/Xforms.h>

#include <vpr/Util/Assert.h>
#include <jccl/Config/ConfigElement.h>

#include <vrj/Util/Debug.h>
#include <vrj/Display/DisplayExceptions.h>
#include <vrj/Display/SurfaceProjection.h>


namespace vrj
{

SurfaceProjection::SurfaceProjection(const gmtl::Point3f& llCorner,
                                     const gmtl::Point3f& lrCorner,
                                     const gmtl::Point3f& urCorner,
                                     const gmtl::Point3f& ulCorner)
   : mLLCorner(llCorner)
   , mLRCorner(lrCorner)
   , mURCorner(urCorner)
   , mULCorner(ulCorner)
{
   /* Do nothing. */ ;
}

SurfaceProjectionPtr SurfaceProjection::create(const gmtl::Point3f& llCorner,
                                               const gmtl::Point3f& lrCorner,
                                               const gmtl::Point3f& urCorner,
                                               const gmtl::Point3f& ulCorner)
{
   return SurfaceProjectionPtr(new SurfaceProjection(llCorner, lrCorner,
                                                     urCorner, ulCorner));
}

void SurfaceProjection::validateCorners()
{
   const gmtl::Vec3f bot_side   = mLRCorner - mLLCorner;
   const gmtl::Vec3f diag       = mULCorner - mLRCorner;
   const gmtl::Vec3f right_side = mURCorner - mLRCorner;

   gmtl::Vec3f norm1, norm2;
   gmtl::cross(norm1, bot_side, diag);
   gmtl::cross(norm2, bot_side, right_side);
   gmtl::normalize(norm1);
   gmtl::normalize(norm2);

   if ( ! gmtl::isEqual(norm1, norm2, 1e-4f) )
   {
      std::ostringstream msg_stream;
      msg_stream << "The corners " << mLLCorner << ", " << mLRCorner << ", "
                 << mURCorner << ", " << mULCorner
                 << " do not form a valid surface";
      throw InvalidSurfaceException(msg_stream.str());
   }
}

void SurfaceProjection::updateCorners(const gmtl::Point3f& llCorner,
                                      const gmtl::Point3f& lrCorner,
                                      const gmtl::Point3f& urCorner,
                                      const gmtl::Point3f& ulCorner)
{
   mLLCorner = llCorner;
   mLRCorner = lrCorner;
   mURCorner = urCorner;
   mULCorner = ulCorner;

   validateCorners();
}

// Just call the base class constructor
void SurfaceProjection::config(jccl::ConfigElementPtr element)
{
   vprASSERT(element->getID() == "surface_viewport");

   Projection::config(element);        // Call base class config first
}

// Recalculates the projection matrix. This uses a method that needs to
// know the distance in the screen plane from the origin (determined by the
// normal to the plane throught the origin) to the edges of the screen.
// This method can be used for any rectangular planar screen. By adjusting
// the wall rotation matrix, this method can be used for the general case
// of a rectangular screen in 3-space
void SurfaceProjection::calcViewMatrix(const gmtl::Matrix44f&,
                                       const gmtl::Point3f& eyePoint,
                                       const float scaleFactor)
{
   calculateOffsets();
   calcViewFrustum(eyePoint, scaleFactor);

   // Need to post translate to get the view matrix at the position of the
   // eye.
   mViewMat = m_surface_M_base * gmtl::makeTrans<gmtl::Matrix44f>(-eyePoint);
}

// Calculates the view frustum needed for the view matrix. This uses a
// method that needs to know the distance in the screen plane from the
// origin (determined by the normal to the plane through the origin) to the
// edges of the screen. This method can be used for any rectangular planar
// screen. By adjusting the wall rotation matrix, this method can be used
// for the general case of a rectangular screen in 3-space.
void SurfaceProjection::calcViewFrustum(const gmtl::Point3f& eyePos,
                                        const float scaleFactor)
{
   const float near_dist = mNearDist;
   const float far_dist = mFarDist;

   // Distance measurements from eye to screen/edges
   // Distance to edges is from the point on the screen plane
   // where a normal line would go through the origin.
   float eye_to_screen, eye_to_right, eye_to_left, eye_to_top, eye_to_bottom;

   // Compute transformed eye position
   // - Converts eye coords into the surface's coord system
   // Xformed position of eyes
   const gmtl::Point3f eye_surface = m_surface_M_base * eyePos;

   vprDEBUG(vrjDBG_DISP_MGR, vprDBG_HEX_LVL)
      << "[vrj::SurfaceProjection::calcViewFrustum()] Base eye: "
      << eyePos << std::endl
      << vprDEBUG_FLUSH;
   vprDEBUG_NEXT(vrjDBG_DISP_MGR, vprDBG_HEX_LVL)
      << "                                            Transformed eye: "
      << eye_surface << std::endl << vprDEBUG_FLUSH;

   // Compute dist from eye to screen/edges
   // Take into account scale since all origin to anythings are in meters
   eye_to_screen = (scaleFactor * mOriginToScreen) + eye_surface[gmtl::Zelt];
   eye_to_right  = (scaleFactor * mOriginToRight) - eye_surface[gmtl::Xelt];
   eye_to_left   = (scaleFactor * mOriginToLeft) + eye_surface[gmtl::Xelt];
   eye_to_top    = (scaleFactor * mOriginToTop) - eye_surface[gmtl::Yelt];
   eye_to_bottom = (scaleFactor * mOriginToBottom) + eye_surface[gmtl::Yelt];

   // Distances in near plane, in near plane from origin.  (Similar to above)
   // Find dists on near plane using similar triangles
   const float near_dist_front = near_dist / eye_to_screen; // constant factor
   const float n_eye_to_left   = eye_to_left * near_dist_front;
   const float n_eye_to_right  = eye_to_right * near_dist_front;
   const float n_eye_to_top    = eye_to_top * near_dist_front;
   const float n_eye_to_bottom = eye_to_bottom * near_dist_front;

   // Set frustum and calculate the matrix.
   mFrustum.set(-n_eye_to_left, n_eye_to_right, -n_eye_to_bottom,
                n_eye_to_top, near_dist, far_dist);

   mFocusPlaneDist = eye_to_screen;    // Needed for drawing

   vprDEBUG(vrjDBG_DISP_MGR, vprDBG_HEX_LVL)
      << "[vrj::SurfaceProjection::calcViewFrustum()]" << std::endl
      << vprDEBUG_FLUSH;
   vprDEBUG_NEXT(vrjDBG_DISP_MGR, vprDBG_HEX_LVL)
      << "\tFrustum: " << mFrustum << std::endl << vprDEBUG_FLUSH;
}

std::ostream&
SurfaceProjection::outStream(std::ostream& out,
                             const unsigned int indentLevel) const
{
//   const int pad_width_dot(20 - indentLevel);
   out.setf(std::ios::left);

   const std::string indent_text(indentLevel, ' ');

   out << indent_text << "vrj::SurfaceProjection:\n";

   return Projection::outStream(out, indentLevel);
}

void SurfaceProjection::calculateOffsets()
{
   calculateSurfaceRotation();

   m_base_M_surface = mSurfaceRotation;
   gmtl::invert(m_surface_M_base, m_base_M_surface);

   calculateCornersInBaseFrame();

   mOriginToScreen = -mxLLCorner[gmtl::Zelt];
   mOriginToRight  = mxLRCorner[gmtl::Xelt];
   mOriginToLeft   = -mxLLCorner[gmtl::Xelt];
   mOriginToTop    = mxURCorner[gmtl::Yelt];
   mOriginToBottom = -mxLRCorner[gmtl::Yelt];
}

void SurfaceProjection::calculateSurfaceRotation()
{
   // Find the base vectors for the surface axis (in terms of the base coord
   // system) with z out, x to the right, and y up.
   gmtl::Vec3f x_base(mLRCorner - mLLCorner);
   gmtl::Vec3f y_base(mURCorner - mLRCorner);
   gmtl::Vec3f z_base;
   gmtl::cross(z_base, x_base, y_base);

   // They must be normalized.
   gmtl::normalize(x_base);
   gmtl::normalize(y_base);
   gmtl::normalize(z_base);

   // Calculate the surfaceRotMat using law of cosines.
   mSurfaceRotation = gmtl::makeDirCos<gmtl::Matrix44f>(x_base, y_base,
                                                        z_base);
}

void SurfaceProjection::calculateCornersInBaseFrame()
{
   mxLLCorner = m_surface_M_base * mLLCorner;
   mxLRCorner = m_surface_M_base * mLRCorner;
   mxURCorner = m_surface_M_base * mURCorner;
   mxULCorner = m_surface_M_base * mULCorner;

   // Verify that they are all in the same x,y plane
   vprDEBUG(vprDBG_ALL, vprDBG_HVERB_LVL) << std::setprecision(10)
                                          << mxLLCorner[gmtl::Zelt] << " "
                                          << mxLRCorner[gmtl::Zelt] << " "
                                          << mxURCorner[gmtl::Zelt] << " "
                                          << mxULCorner[gmtl::Zelt] << "\n"
                                          << vprDEBUG_FLUSH;

#ifdef VJ_DEBUG
   // Use 1e-4f here, otherwise the floating point error can get big enough to
   // mess this up for tracked surfaces.
   vprASSERT(
      gmtl::Math::isEqual(mxLLCorner[gmtl::Zelt], mxLRCorner[gmtl::Zelt], 1e-4f) &&
      gmtl::Math::isEqual(mxURCorner[gmtl::Zelt], mxULCorner[gmtl::Zelt], 1e-4f) &&
      gmtl::Math::isEqual(mxLLCorner[gmtl::Zelt], mxULCorner[gmtl::Zelt], 1e-4f)
   );
#endif
}

}
