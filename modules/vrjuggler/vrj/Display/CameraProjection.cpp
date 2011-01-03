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

#include <boost/concept_check.hpp>

#include <gmtl/Output.h>
#include <gmtl/Generate.h>

#include <jccl/Config/ConfigElement.h>

#include <vrj/Display/Display.h>
#include <vrj/Display/Viewport.h>
#include <vrj/Util/Debug.h>
#include <vrj/Display/CameraProjection.h>


namespace vrj
{

CameraProjection::CameraProjection()
   : mVertFOV(60.0f)
{
   /* Do nothing. */ ;
}

CameraProjectionPtr CameraProjection::create()
{
   return CameraProjectionPtr(new CameraProjection());
}

void CameraProjection::config(jccl::ConfigElementPtr element)
{
   Projection::config(element);
}

/** Calculate the viewmatrix and frustum for the camera
* Calculates a perspective transform for the given settings.
* Auto-calculates aspect ratio from the current size of the window and viewport
*/
void CameraProjection::calcViewMatrix(const gmtl::Point3f& cameraPos,
                                      const float scaleFactor)
{
   boost::ignore_unused_variable_warning(scaleFactor);
   mViewMat = gmtl::makeTrans<gmtl::Matrix44f>(cameraPos);;

   vprDEBUG(vprDBG_ALL, vprDBG_DETAILED_LVL)
      << "calcView: Cam pos:" << cameraPos << std::endl << vprDEBUG_FLUSH;

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
