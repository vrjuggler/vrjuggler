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
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

#ifndef _VRJ_CAMERA_PROJECTION_H_
#define _VRJ_CAMERA_PROJECTION_H_

#include <vrj/vrjConfig.h>

#include <gmtl/Matrix.h>
#include <gmtl/Vec.h>

#include <jccl/Config/ConfigElementPtr.h>
#include <vrj/Display/Projection.h>
#include <vrj/Display/CameraProjectionPtr.h>


namespace vrj
{

/** \class CameraProjection CameraProjection.h vrj/Display/CameraProjection.h
 *
 * Projection class that simply takes a matrix for the camera position.
 */
class VJ_CLASS_API CameraProjection : public Projection
{
protected:
   CameraProjection();

public:
   /**
    * @since 2.3.4
    */
   static CameraProjectionPtr create();

   virtual void config(jccl::ConfigElementPtr element);

   void setVerticalFOV(const float fov)
   {
      mVertFOV = fov;
   }

   /**
    * Calculates the view matrix and frustum for the camera.
    * Calculates a perspective transform for the given settings.
    * Auto-calculates aspect ratio from the current size of the window and
    * viewport.
    */
   virtual void calcViewMatrix(const gmtl::Point3f& cameraPos,
                               const float scaleFactor);

   virtual std::ostream& outStream(std::ostream& out,
                                   const unsigned int indentLevel = 0);

public:
   //float mAspectRatio;     // w/h
   float mVertFOV;         // The vertical field of view
};

}

#endif
