/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2002 by Iowa State University
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

#ifndef _VRJ_WALL_PROJECTION_H_
#define _VRJ_WALL_PROJECTION_H_

#include <vrj/vrjConfig.h>
#include <vrj/Display/Projection.h>
//#include <vrj/Math/Coord.h>

namespace vrj
{

class Matrix;

/**
 * Wall specific class for viewport definitions.
 *
 * Responsible for storing and computing projection information of a surface
 * specified.
 *
 * XXX: This should be renamed SurfaceProjection
 *
 * @date 10-5-97
 */
class WallProjection : public Projection
{
public:
   WallProjection(gmtl::Matrix44f surfaceRot, float toScr,
                    float toRight, float toLeft,
                    float toTop, float toBottom)
   {
      //mType = Projection::SURFACE;

      mWallRotationMatrix = surfaceRot;
      mOriginToScreen = toScr;
      mOriginToRight = toRight;
      mOriginToLeft = toLeft;
      mOriginToTop = toTop;
      mOriginToBottom = toBottom;
   }

   /** Configures the projection using the chunk given. */
   virtual void config(jccl::ConfigChunkPtr chunk);

   /**
    * Recalculates the projection matrix.
    * @pre WallRotation matrix must be set correctly.
    *      mOrigin*'s must all be set correctly.
    * @post frustum has been recomputed for given eyePos.
    */
   virtual void calcViewMatrix(gmtl::Matrix44f& eyePos);

   /**
    * Calculates the frustum needed for the view matrix.
    * @note This function is called as part of calcViewMatrix.
    */
   virtual void calcViewFrustum(gmtl::Matrix44f& eyePos);

   std::ostream& outStream(std::ostream& out);

protected:
   /// Rotation of the screen
   gmtl::Matrix44f   mWallRotationMatrix;

   /// Screen configuration
   float mOriginToScreen, mOriginToRight, mOriginToLeft, mOriginToTop, mOriginToBottom;
};

};
#endif
