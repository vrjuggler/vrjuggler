/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2005 by Iowa State University
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

#include <vrj/vrjConfig.h>

#include <jccl/Config/ConfigElement.h>

#include <gmtl/Output.h>
#include <gmtl/Generate.h>

#include <vrj/Display/Display.h>
#include <vrj/Display/CameraProjection.h>

#include <boost/concept_check.hpp>


namespace vrj
{

void CameraProjection::config(jccl::ConfigElementPtr element)
{
   Projection::config(element);
}

/** Calculate the viewmatrix and frustum for the camera
* Calculates a perspective transform for the given settings.
* Auto-calculates aspect ratio from the current size of the window and viewport
*/
void CameraProjection::calcViewMatrix(gmtl::Matrix44f& cameraPos, const float scaleFactor)
{
   boost::ignore_unused_variable_warning(scaleFactor);
   mViewMat = cameraPos;

   gmtl::Vec3f camera_trans = gmtl::makeTrans<gmtl::Vec3f>(cameraPos);
   vprDEBUG(vprDBG_ALL, vprDBG_DETAILED_LVL) << "calcView: Cam pos:" << camera_trans << std::endl << vprDEBUG_FLUSH;

   int win_xo, win_yo, win_xs, win_ys;  // origin and size of display window
   float vp_xo, vp_yo, vp_xs, vp_ys;  // origin and size of viewport
   float width, height;
   float aspect_ratio;

   getViewport()->getDisplay()->getOriginAndSize(win_xo,win_yo, win_xs, win_ys);
   getViewport()->getOriginAndSize( vp_xo, vp_yo, vp_xs, vp_ys );
   width = float(win_xs) * vp_xs;
   height = float(win_ys) * vp_ys;

   aspect_ratio = (width/height);

   // Calculate frustum
   float top, right;
   top = gmtl::Math::tan( gmtl::Math::deg2Rad(mVertFOV/2.0f) ) * mNearDist;     // Calculate the top based on fovy
   right = aspect_ratio * top;

   // Calculate the frustum to use for the camera
   mFrustum.set(-right, right, -top, top, mNearDist, mFarDist);
}

std::ostream& CameraProjection::outStream(std::ostream& out,
                                          const unsigned int indentLevel)
{
//   const int pad_width_dot(20 - indentLevel);
   out.setf(std::ios::left);

   const std::string indent_text(indentLevel, ' ');

   out << indent_text << "vrj::CameraProjection:\n";

   return Projection::outStream(out, indentLevel);
}

}
