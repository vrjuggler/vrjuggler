/*
 *  File:	    $RCSfile$
 *  Date modified:  $Date$
 *  Version:	    $Revision$
 *
 *
 *                                VR Juggler
 *                                    by
 *                              Allen Bierbaum
 *                             Christopher Just
 *                             Patrick Hartling
 *                            Carolina Cruz-Neira
 *                               Albert Baker
 *
 *                  Copyright (C) - 1997, 1998, 1999, 2000
 *              Iowa State University Research Foundation, Inc.
 *                            All Rights Reserved
 */


#include <vjConfig.h>
//#include <sys/types.h>

#include <Kernel/vjWallProjection.h>
#include <Math/vjMatrix.h>
#include <Math/vjVec3.h>
#include <Kernel/vjKernel.h>
#include <Input/vjPosition/vjPosition.h>
#include <Math/vjCoord.h>


//: Recalculate the projection matrix
// Uses a method that needs to know the distance in the screen plane
// from the origin (determined by the normal to the plane throught the
// origin) to the edges of the screen.
// This method can be used for any rectangular planar screen.
// By adjusting the wall rotation matrix, this method can be used for
// the general case of a rectangular screen in 3-space
void vjWallProjection::calcViewMatrix(vjMatrix& eyePos)
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


   vjCoord eye_coord(eyePos);
   vjVec3   eye_pos;             // Non-xformed pos
   eye_pos = eye_coord.pos;
   vjVec3   eye_xformed;         // Xformed position of eyes

   vjDEBUG(vjDBG_DISP_MGR,7) << "vjWallProjection::calcWallProjection:  Wall Proj:\n" << *this << endl << vjDEBUG_FLUSH;
   vjDEBUG(vjDBG_DISP_MGR,7) << "vjWallProjection::calcWallProjection:    Base eye:" << eye_coord.pos << endl << vjDEBUG_FLUSH;

   // Convert eye coords into the wall's coord system
   eye_xformed.xformFull(mWallRotationMatrix, eye_pos);

   vjDEBUG(vjDBG_DISP_MGR,7) << "vjWallProjection::calcWallProjection:    Xformed eye:" << eye_xformed << endl << vjDEBUG_FLUSH;

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
   frustum.set(-n_eye_to_left, n_eye_to_right,
                -n_eye_to_bottom, n_eye_to_top,
                near_dist, far_dist);

   mFocusPlaneDist = eye_to_screen;    // Needed for drawing

   vjDEBUG(vjDBG_DISP_MGR,7) << "vjWallProjection::calcWallProjection: \n\tFrustum: " << frustum << endl << vjDEBUG_FLUSH;
   vjDEBUG(vjDBG_DISP_MGR,7) << "vjWallProjection::calcWallProjection: B4 Trans:\n" << mWallRotationMatrix << endl << vjDEBUG_FLUSH;

   viewMat.postTrans(mWallRotationMatrix, -eye_pos[VJ_X], -eye_pos[VJ_Y], -eye_pos[VJ_Z]);
}



//
// Performer: (Zup Coords)
//	1st row is "right" direction
//	2nd Row defines the view direction.
//	3rd row defines up direction
/*
void vjWallProjection::setWallRotationMatrix()
{
   vjDEBUG(vjDBG_ALL,2) << "vjWallProjection::setWallRotationMatrix: Entering" << endl << vjDEBUG_FLUSH;

   switch (mSurface)
   {
   //////////////
   // R  V  U  //
   case FRONT:
      mWallRotationMatrix.makeIdent();
      break;
   case LEFT:
      mWallRotationMatrix.makeRot(-90.0f, vjVec3(0.0f, 1.0f, 0.0f));
      break;
   case RIGHT:
      mWallRotationMatrix.makeRot(90.0f, vjVec3(0.0f, 1.0f, 0.0f));
      break;
   case FLOOR:
      mWallRotationMatrix.makeRot(-90.0f, vjVec3(-1.0f, 0.0f, 0.0f));
      break;
   case CEILING:
      mWallRotationMatrix.makeRot(90.0f, vjVec3(-1.0f, 1.0f, 0.0f));
      break;
   case BACK:
      mWallRotationMatrix.makeRot(180, vjVec3(0.0f, 1.0f, 0.0f));
      break;
   }

   vjDEBUG(vjDBG_ALL,2) << "vjWallProjection::setWallRotationMatrix: Matrix" << endl << vjDEBUG_FLUSH;
   vjDEBUG(vjDBG_ALL,2) << mWallRotationMatrix << endl << vjDEBUG_FLUSH;
}
*/


ostream& vjWallProjection::outStream(ostream& out)
{
   out << "vjWallProjection:\n";
   out << "surface: ";

   out << endl;

   return vjProjection::outStream(out);
}


