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

#ifndef _VRJ_CAMERA_PROJECTION_H_
#define _VRJ_CAMERA_PROJECTION_H_

#include <vrj/vrjConfig.h>
#include <vrj/Display/Projection.h>
#include <jccl/Config/ConfigElementPtr.h>

#include <gmtl/Matrix.h>
#include <gmtl/Vec.h>


namespace vrj
{

/** \class CameraProjection CameraProjection.h vrj/Display/CameraProjection.h
 *
 * Projection class that simply takes a matrix for the camera position.
 */
class VJ_CLASS_API CameraProjection : public Projection
{
public:
   CameraProjection() : mVertFOV(60.0f)
   {
      //mType = Projection::SIM;
   }

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
   virtual void calcViewMatrix(const gmtl::Matrix44f& cameraPos,
                               const float scaleFactor);

   virtual std::ostream& outStream(std::ostream& out,
                                   const unsigned int indentLevel = 0);

public:
   //float mAspectRatio;     // w/h
   float mVertFOV;         // The vertical field of view
};

}

#endif
