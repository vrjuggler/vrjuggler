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

#ifndef _VRJ_WALL_PROJECTION_H_
#define _VRJ_WALL_PROJECTION_H_

#include <vrj/vrjConfig.h>
#include <vrj/Display/Projection.h>
//#include <vrj/Math/Coord.h>

namespace vrj
{

class Matrix;

//-----------------------------------------------------------------
//: Wall specific class for viewport definitions.
//
//  Responsible for storing and computing projection
//  information of a surface specified.
//
// XXX: This should be renamed SurfaceProjection
//
// @author Allen Bierbaum
//  Date: 10-5-97
//----------------------------------------------------------------
class WallProjection : public Projection
{
public:
   WallProjection(Matrix surfaceRot, float toScr,
                    float toRight, float toLeft,
                    float toTop, float toBottom)
   {
      mType = Projection::SURFACE;

      mWallRotationMatrix = surfaceRot;
      mOriginToScreen = toScr;
      mOriginToRight = toRight;
      mOriginToLeft = toLeft;
      mOriginToTop = toTop;
      mOriginToBottom = toBottom;
   }

   //: Configure the projection using the chunk given
   virtual void config(jccl::ConfigChunkPtr chunk);

   //: Recalculate the projection matrix
   //! PRE: WallRotation matrix must be set correctly
   //! PRE: mOrigin*'s must all be set correctly
   //!POST: frustum has been recomputed for given eyePos
   virtual void calcViewMatrix(Matrix& eyePos);

   //: Calculate the frustum needed for the view matrix
   //! NOTE: This function is called as part of calcViewMatrix
   virtual void calcViewFrustum(Matrix& eyePos);

   std::ostream& outStream(std::ostream& out);

protected:
   // Rotation of the screen
   Matrix   mWallRotationMatrix;

   // Screen configuration
   float mOriginToScreen, mOriginToRight, mOriginToLeft, mOriginToTop, mOriginToBottom;
};

};
#endif
