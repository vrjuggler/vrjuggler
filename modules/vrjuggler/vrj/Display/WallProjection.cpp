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

#include <vrj/vrjConfig.h>
//#include <sys/types.h>

#include <vrj/Display/WallProjection.h>
#include <vrj/Kernel/Kernel.h>

#include <vrj/Math/Matrix.h>
#include <vrj/Math/Vec3.h>
#include <vrj/Math/Vec4.h>


#include <vrj/Input/Type/Position.h>
#include <vrj/Math/Coord.h>
#include <vrj/Util/Debug.h>

#include <vrj/Config/ConfigChunk.h>

namespace vrj
{


// Just call the base class constructor
void WallProjection::config(ConfigChunk* chunk)
{
   vprASSERT( ((std::string)chunk->getType() == std::string("surfaceDisplay")) ||
             ((std::string)chunk->getType() == std::string("surfaceViewport")) );

   Projection::config(chunk);        // Call base class config first
}

//: Recalculate the projection matrix
// Uses a method that needs to know the distance in the screen plane
// from the origin (determined by the normal to the plane throught the
// origin) to the edges of the screen.
// This method can be used for any rectangular planar screen.
// By adjusting the wall rotation matrix, this method can be used for
// the general case of a rectangular screen in 3-space
//
//! PRE: WallRotation matrix must be set correctly
//! PRE: mOrigin*'s must all be set correctly
void WallProjection::calcViewMatrix(Matrix& eyePos)
{
   calcViewFrustum(eyePos);

   Coord eye_coord(eyePos);
   Vec3   eye_pos;             // Non-xformed pos
   eye_pos = eye_coord.pos;

   // Need to post translate to get the view matrix at the position of the eye
   mViewMat.postTrans(mWallRotationMatrix, -eye_pos[VJ_X], -eye_pos[VJ_Y], -eye_pos[VJ_Z]);
}


//: Recalculate the view frustum
// Uses a method that needs to know the distance in the screen plane
// from the origin (determined by the normal to the plane throught the
// origin) to the edges of the screen.
// This method can be used for any rectangular planar screen.
// By adjusting the wall rotation matrix, this method can be used for
// the general case of a rectangular screen in 3-space
//
//! PRE: WallRotation matrix must be set correctly
//! PRE: mOrigin*'s must all be set correctly
void WallProjection::calcViewFrustum(Matrix& eyePos)
{
   float near_dist, far_dist;
   near_dist = mNearDist;
   far_dist = mFarDist;

   // Distance measurements from eye to screen/edges
   // Distance to edges is from the point on the screen plane
   // where a normal line would go through the origin.
   float eye_to_screen, eye_to_right, eye_to_left, eye_to_top, eye_to_bottom;

   // Distances in near plane, in near plane from origin.  (Similar to above)
   float n_eye_to_right, n_eye_to_left, n_eye_to_top, n_eye_to_bottom;


   Coord eye_coord(eyePos);
   Vec3   eye_pos;             // Non-xformed pos
   eye_pos = eye_coord.pos;
   Vec3   eye_xformed;         // Xformed position of eyes

   vjDEBUG(vjDBG_DISP_MGR,7)
      << "vjWallProjection::calcWallProjection:  Wall Proj:\n" << *this
      << std::endl << vjDEBUG_FLUSH;
   vjDEBUG(vjDBG_DISP_MGR,7)
      << "vjWallProjection::calcWallProjection:    Base eye:" << eye_coord.pos
      << std::endl << vjDEBUG_FLUSH;

   // Convert eye coords into the wall's coord system
   eye_xformed.xformFull(mWallRotationMatrix, eye_pos);

   vjDEBUG(vjDBG_DISP_MGR,7)
      << "vjWallProjection::calcWallProjection:    Xformed eye:" << eye_xformed
      << std::endl << vjDEBUG_FLUSH;

   // Compute dist from eye to screen/edges
   eye_to_screen = mOriginToScreen + eye_xformed[VJ_Z];
   eye_to_right = mOriginToRight - eye_xformed[VJ_X];
   eye_to_left = mOriginToLeft + eye_xformed[VJ_X];
   eye_to_top = mOriginToTop - eye_xformed[VJ_Y];
   eye_to_bottom = mOriginToBottom + eye_xformed[VJ_Y];

   // Find dists on near plane using similar triangles
   float near_distFront = near_dist/eye_to_screen;      // constant factor
   n_eye_to_left = eye_to_left*near_distFront;
   n_eye_to_right = eye_to_right*near_distFront;
   n_eye_to_top = eye_to_top*near_distFront;
   n_eye_to_bottom = eye_to_bottom*near_distFront;

   // Set frustum and calulcate the matrix
   mFrustum.set(-n_eye_to_left, n_eye_to_right,
                -n_eye_to_bottom, n_eye_to_top,
                near_dist, far_dist);

   mFocusPlaneDist = eye_to_screen;    // Needed for drawing

   vjDEBUG(vjDBG_DISP_MGR,7)
      << "vjWallProjection::calcWallProjection: \n\tFrustum: " << mFrustum
      << std::endl << vjDEBUG_FLUSH;
   vjDEBUG(vjDBG_DISP_MGR,7)
      << "vjWallProjection::calcWallProjection: B4 Trans:\n"
      << mWallRotationMatrix << std::endl << vjDEBUG_FLUSH;
}


std::ostream& WallProjection::outStream(std::ostream& out)
{
   out << "vjWallProjection: ";

   return Projection::outStream(out);
}


};
