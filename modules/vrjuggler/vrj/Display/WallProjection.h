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
      enum
      {FRONT=10, FLOOR, BACK, CEILING, USER};
public:
   vjWallProjection()
   {
      mSurface = -1;
      mEye = -1;
      mType = vjProjection::SURFACE;

      mOriginToScreen = 0;
      mOriginToRight = 0;
      mOriginToLeft = 0;
      mOriginToTop = 0;
      mOriginToBottom = 0;
   }

   virtual void config(vjConfigChunk* chunk)
   {
      vjProjection::config(chunk);        // Call base class config first

      // Set the surface by name
      char* proj  = strdup(((std::string)chunk->getProperty("projectiontype")).c_str());
      vjASSERT(proj != NULL);    // Check for a seg fault error
      setSurface(proj);

      // Set all the screen pos params
      mOriginToScreen = chunk->getProperty("screenPos", 0);
      mOriginToLeft = chunk->getProperty("screenPos", 1);
      mOriginToRight = chunk->getProperty("screenPos", 2);
      mOriginToBottom = chunk->getProperty("screenPos", 3);
      mOriginToTop = chunk->getProperty("screenPos", 4);
   }

   void setSurface(char* projStr)
   {
      vjDEBUG(2) << "vjProjection::setSurface: Setting surface:" << projStr << endl << vjDEBUG_FLUSH;
      if (strcasecmp(projStr, "left") == 0)
         setSurface(vjWallProjection::LEFT);
      else if (strcasecmp(projStr, "right") == 0)
         setSurface(vjWallProjection::RIGHT);
      else if (strcasecmp(projStr, "front") == 0)
         setSurface(vjWallProjection::FRONT);
      else if (strcasecmp(projStr, "floor") == 0)
         setSurface(vjWallProjection::FLOOR);
      else if (strcasecmp(projStr, "back") == 0)
         setSurface(vjWallProjection::BACK);
      else if (strcasecmp(projStr, "ceiling") == 0)
         setSurface(vjWallProjection::CEILING);
      else if (strcasecmp(projStr, "user") == 0)
         setSurface(vjWallProjection::USER);
   }

   virtual void setSurface(int _surface)
   {
      mSurface = _surface;
      this->surfaceSet();
   }

   int   getSurface()
   { return mSurface;}


   virtual void calcViewMatrix(vjMatrix& eyePos)
   {
      calcWallProjection(eyePos);
   }

   ostream& outStream(ostream& out);

protected:

   //: Recalculate the projection matrix
   //!POST: frustum has been recomputed for given eyePos
   void calcWallProjection(vjMatrix& eyePos);

   void setWallRotationMatrix();

   virtual void surfaceSet()
   {
      setWallRotationMatrix();
      mWallRotationMatrixInv.invert(mWallRotationMatrix);
      vjDEBUG(3) << "vjWallProjection::surfaceSet: Set rotation matrix:\n" << mWallRotationMatrix << vjDEBUG_FLUSH;
   }

private:
   int mSurface;   //: The surface we are drawing on

   // Rotation of the screen
   vjMatrix   mWallRotationMatrix, mWallRotationMatrixInv;

   // Screen configuration
   float mOriginToScreen, mOriginToRight, mOriginToLeft, mOriginToTop, mOriginToBottom;
};
#endif
