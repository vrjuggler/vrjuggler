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

#ifndef _VRJ_CAMERA_PROJECTION_H_
#define _VRJ_CAMERA_PROJECTION_H_

#include <vrj/vrjConfig.h>
#include <vrj/Display/Projection.h>
#include <jccl/Config/ConfigChunk.h>

namespace vrj
{

   class Matrix;

//: Projection class that simply takes a matrix for the camera position
//
class CameraProjection : public Projection
{
public:
   CameraProjection()
   {
      mType = Projection::SIM;
   }

   virtual void config(jccl::ConfigChunkPtr chunk)
   {
      Projection::config(chunk);

      vprASSERT( ((std::string)chunk->getType() == std::string("simDisplay")) ||
                ((std::string)chunk->getType() == std::string("simViewport")) );

      // Set fov and make sure that it is not 0.0
      mVertFOV = chunk->getProperty("vert_fov");
      if(mVertFOV == 0.0f)
         mVertFOV = 60.0f;
   }

   /** Calculate the viewmatrix and frustum for the camera
   * Calculates a perspective transform for the given settings.
   * Auto-calculates aspect ratio from the current size of the window and viewport
   */
   virtual void calcViewMatrix(Matrix& cameraPos)
   {
      mViewMat = cameraPos;

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

      top = Math::tan( Math::deg2rad(mVertFOV/2.0f) ) * mNearDist;     // Calculate the top based on fovy
      right = aspect_ratio * top;

      // XXX: The frustum is not used
      mFrustum.set(-right, right, -top, top, mNearDist, mFarDist);
   }

   virtual std::ostream& outStream(std::ostream& out)
   {
      out << "vjCameraProjection:\n";
      return out;
   }

public:
   //float mAspectRatio;     // w/h
   float mVertFOV;         // The vertical field of view
};

};
#endif
