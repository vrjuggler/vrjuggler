#include <vjConfig.h>
#include <sys/types.h>

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
void vjWallProjection::calcWallProjection(vjMatrix& eyePos)
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

   //vjDEBUG(0) << "vjWallProjection::calcWallProjection:  Wall Proj:\n" << *this << endl << vjDEBUG_FLUSH;
   //vjDEBUG(0) << "vjWallProjection::calcWallProjection:    Base eye:" << eye_coord.pos << endl << vjDEBUG_FLUSH;

   // Convert eye coords into the wall's coord system
   eye_xformed.xformFull(mWallRotationMatrix, eye_pos);

   //vjDEBUG(0) << "vjWallProjection::calcWallProjection:    Xformed eye:" << eye_xformed << endl << vjDEBUG_FLUSH;

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


   //vjDEBUG(0) << "vjWallProjection::calcWallProjection: \n\tFrustum: " << frustum << endl << vjDEBUG_FLUSH;

   //vjDEBUG(0) << "vjWallProjection::calcWallProjection: B4 Trans:\n" << mWallRotationMatrix << endl << vjDEBUG_FLUSH;
   viewMat.postTrans(mWallRotationMatrix, -eye_pos[VJ_X], -eye_pos[VJ_Y], -eye_pos[VJ_Z]);
}



//
// Performer: (Zup Coords)
//	1st row is "right" direction
//	2nd Row defines the view direction.
//	3rd row defines up direction
void vjWallProjection::setWallRotationMatrix()
{
   vjDEBUG(2) << "vjWallProjection::setWallRotationMatrix: Entering" << endl << vjDEBUG_FLUSH;

   switch (mSurface)
   {
   /************/
   /* R  V  U  */
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

   vjDEBUG(2) << "vjWallProjection::setWallRotationMatrix: Matrix" << endl << vjDEBUG_FLUSH;
   vjDEBUG(2) << mWallRotationMatrix << endl << vjDEBUG_FLUSH;
}



ostream& vjWallProjection::outStream(ostream& out)
{
   out << "vjWallProjection:\n";
   out << "surface: ";
   switch(mSurface)
   {
   case vjWallProjection::LEFT:
      out << "Left";
      break;
   case vjWallProjection::RIGHT:
      out << "Right";
      break;
   case vjWallProjection::FRONT:
      out << "Front";
      break;
   case vjWallProjection::FLOOR:
      out << "Floor";
      break;
   case vjWallProjection::BACK:
      out << "Back";
      break;
   case vjWallProjection::CEILING:
      out << "Ceiling";
      break;
   case vjWallProjection::USER:
      out << "User";
      break;
   }
   out << endl;

   return vjProjection::outStream(out);
}

////// OLD CODE ///////////////
/*
void vjWallProjection::calcWallProjection(vjMatrix& eyePos)
{
   vjDEBUG(3) << "vjWallProjection::calcWallProjection: Entering." << endl << vjDEBUG_FLUSH;

   register float dfront, dback, dleft, dright, dceiling, dfloor;
   register float Factor, Left, Right, Top, Bottom;
// Not Used //   float Xprime, Zprime, OrigXprime, OrigZprime;
   float eyeX, eyeY, eyeZ;
   float OrigY, OrigX, OrigZ;      // Dist from left/floor/front
   float vjWidth, vjHeight;
   float vjNear, vjFar;

   vjCoord eye_coord(eyePos);

   vjNear = 0.1f;
   vjFar = 1000.0f;

   vjWidth = 12.0f;
   vjHeight = 9.0f;

   // XXX: Hack: Coords
   OrigX = 6.0f;     // Dist from right
   OrigY = 0.0f;     // Dist from floor
   OrigZ = 6.0f;     // Dist from front

   //vjDEBUG(1) << "Trk EYE: " << eyePos.pos << endl << vjDEBUG_FLUSH;

#if 0 // ZUP_COORDS
   eyeX = eye_coord.pos[0];
   eyeY = -eye_coord.pos[2];
   eyeZ = eye_coord.pos[1];
#else
   eyeX = eye_coord.pos[0];
   eyeY = eye_coord.pos[1];
   eyeZ = eye_coord.pos[2];
#endif

      // XXX: Hack
   if(eyeZ == 0.0f)
      eyeZ = 0.001f;

   vjDEBUG(0) << "\n    EYE: " << eyeX
            << ", " << eyeY
            << ", " << eyeZ << endl << vjDEBUG_FLUSH;

#if 0 //ZUP_COORDS
   dfront = OrigY - eyeY;
   dleft = OrigX + eyeX;
   dfloor = OrigZ + eyeZ;
#else
   dfront = OrigZ + eyeZ;
   dleft = OrigX + eyeX;
   dfloor = OrigY + eyeY;
#endif

   dback = vjWidth - dfront;
   dright = vjWidth - dleft;
   dceiling = vjHeight - dfloor;

   vjDEBUG(2) << "\tdfront:" << dfront
           << "  dleft:" << dleft
           << "  dfloor:" << dfloor
           << "  dback:" << dback
           << "  dright:" << dright
           << "  dceiling:" << dceiling << endl << flush << vjDEBUG_FLUSH;

   // Use ratios and similar triangles to find the frustum
   // (inser4t Diagrom here>
   // Find frustum
   switch (surface)
   {
   case FRONT:
      Factor = vjNear / dfront;
      Left = -Factor * dleft;
      Right = Factor * dright;
      Bottom = -Factor * dfloor;
      Top = Factor * dceiling;
      break;
   case LEFT:
      Factor = vjNear / dleft;
      Left = -Factor * dback;
      Right = Factor * dfront;
      Bottom = -Factor * dfloor;
      Top = Factor * dceiling;
      break;
   case RIGHT:
      Factor = vjNear / dright;
      Left = -Factor * dfront;
      Right = Factor * dback;
      Bottom = -Factor * dfloor;
      Top = Factor * dceiling;
      break;
   case FLOOR:
      Factor = vjNear / dfloor;
      Left = -Factor * dleft;
      Right = Factor * dright;
      Bottom = -Factor * dback;
      Top = Factor * dfront;
      break;
   case CEILING:
      Factor = vjNear / dceiling;
      Left = -Factor * dleft;
      Right = Factor * dright;
      Bottom = -Factor * dfront;
      Top = Factor * dback;
      break;
   case BACK:
      Factor = vjNear / dback;
      Left = -Factor * dright;
      Right = Factor * dleft;
      Bottom = -Factor * dfloor;
      Top = Factor * dceiling;
      break;
   }

   vjDEBUG(2) << "Bottom, Left, Top, Right:" << Bottom << ", " << Left << ", " << Top << ", " <<  Right << endl << vjDEBUG_FLUSH;
   frustum.setBottomLeftTopRight(Bottom, Left, Top, Right);
   frustum.setNearFar(vjNear, vjFar);

	vjDEBUG(2) << "vjWallProjection::calcWallProjection: \n\tFrustum: " << frustum
       << endl << vjDEBUG_FLUSH;
	
   vjDEBUG(5) << "vjWallProjection::calcWallProjection: B4 Trans:\n" << wallRotationMatrix << endl << vjDEBUG_FLUSH;
   viewMat.postTrans(wallRotationMatrix, -eyeX, -eyeY, -eyeZ);
	vjDEBUG(5) << "                                      After:\n" << viewMat << endl << vjDEBUG_FLUSH;
}
*/

