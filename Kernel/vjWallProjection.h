#ifndef _VJ_WALL_PROJECTION_
#define _VJ_WALL_PROJECTION_

#include <config.h>
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
   vjWallProjection()
   {
      surface = -1;

      mOriginToScreen = 0;
      mOriginToRight = 0;
      mOriginToLeft = 0;
      mOriginToTop = 0;
      mOriginToBottom = 0;
   }

   virtual void config(vjConfigChunk* chunk)
   {
      vjProjection::config(chunk);        // Call base class config first
      mOriginToScreen = chunk->getProperty("screenPos", 0);
      mOriginToLeft = chunk->getProperty("screenPos", 1);
      mOriginToRight = chunk->getProperty("screenPos", 2);
      mOriginToBottom = chunk->getProperty("screenPos", 3);
      mOriginToTop = chunk->getProperty("screenPos", 4);  
   }

   virtual void calcViewMatrix(vjMatrix& eyePos)
   {
      calcWallProjection(eyePos);
   }

protected:

   //: Recalculate the projection matrix
   //!POST: frustum has been recomputed for given eyePos
   void calcWallProjection(vjMatrix& eyePos);

   void setWallRotationMatrix();

   virtual void surfaceSet()
   {
      setWallRotationMatrix();
      mWallRotationMatrixInv.invert(mWallRotationMatrix);
      vjDEBUG(3) << "vjWallProjection::surfaceSet: Set rotation matrix:\n" << mWallRotationMatrix;
   }

private:
   vjMatrix   mWallRotationMatrix, mWallRotationMatrixInv;

   // Screen configuration
   float mOriginToScreen, mOriginToRight, mOriginToLeft, mOriginToTop, mOriginToBottom;
};
#endif
