#ifndef _VJ_WALL_PROJECTION_
#define _VJ_WALL_PROJECTION_

#include <vjConfig.h>
#include <Kernel/vjProjection.h>
#include <Input/vjPosition/vjPosition.h>
#include <Math/vjCoord.h>

class vjMatrix;

//-----------------------------------------------------------------
//: Wall specific class for viewport definitions.
//
//	 Responsible for storing and computing projection
//  information of Wall specific projections.
//
// @author Allen Bierbaum
//  Date: 10-5-97
//----------------------------------------------------------------
class vjWallProjection : public vjProjection
{
public:
   vjWallProjection(vjMatrix surfaceRot, float toScr,
                    float toRight, float toLeft,
                    float toTop, float toBottom)
   {
      mType = vjProjection::SURFACE;

      mWallRotationMatrix = surfaceRot;
      mOriginToScreen = toScr;
      mOriginToRight = toRight;
      mOriginToLeft = toLeft;
      mOriginToTop = toTop;
      mOriginToBottom = toBottom;
   }

   virtual void config(vjConfigChunk* chunk)
   {
      vjProjection::config(chunk);        // Call base class config first
   }

   //: Recalculate the projection matrix
   //!POST: frustum has been recomputed for given eyePos
   virtual void calcViewMatrix(vjMatrix& eyePos);

   ostream& outStream(ostream& out);

private:
   // Rotation of the screen
   vjMatrix   mWallRotationMatrix;

   // Screen configuration
   float mOriginToScreen, mOriginToRight, mOriginToLeft, mOriginToTop, mOriginToBottom;
};
#endif
