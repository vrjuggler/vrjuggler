#include <collidor.h>
#include <Math/vjVec3.h>
#include <Math/vjMatrix.h>

#ifndef _PLANE_COLLIDOR_H_
#define _PLANE_COLLIDOR_H_

class planeCollidor : public collidor
{
public:
   planeCollidor()
   {
      mHeight = 0;
   }

   bool testMove(vjVec3 curPos, vjVec3 delta, vjVec3& correction, bool curPosWithDelta = false);

private:
   float mHeight;    // Height of the plane
};


bool planeCollidor::testMove(vjVec3 curPos, vjVec3 delta, vjVec3& correction, bool curPosWithDelta)
{
   correction.set(0,0,0);

   vjVec3 target_pos = curPos+delta;
   if(target_pos[1] < mHeight)
   {
      correction[1] = (mHeight-target_pos[1]);  // Get it back up there
      setDidCollide(true);
      return true;
   }
   else
   {
      setDidCollide(false);
      return false;
   }

}


#endif
