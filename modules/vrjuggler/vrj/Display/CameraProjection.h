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
#include <vrj/Config/ConfigChunk.h>

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

   virtual void config(ConfigChunk* chunk)
   {
      Projection::config(chunk);

      vprASSERT( ((std::string)chunk->getType() == std::string("simDisplay")) ||
                ((std::string)chunk->getType() == std::string("simViewport")) );

      // Set fov and make sure that it is not 0.0
      mVertFOV = chunk->getProperty("vert_fov");
      if(mVertFOV == 0.0f)
         mVertFOV = 60.0f;
      mAspectRatio = chunk->getProperty("aspect_ratio");
      if(mAspectRatio == 0.0f)
         mAspectRatio = 1.0f;
   }

   virtual void calcViewMatrix(Matrix& cameraPos)
   {
      mViewMat = cameraPos;
      // XXX: The frustum is not used
      mFrustum.set(-0.6f, 0.6f, -0.6f, 0.6f, mNearDist, mFarDist);
   }

   virtual std::ostream& outStream(std::ostream& out)
   {
      out << "vjCameraProjection:\n";
      return out;
   }

public:
   float mAspectRatio;     // w/h
   float mVertFOV;         // The vertical field of view
};

};
#endif
