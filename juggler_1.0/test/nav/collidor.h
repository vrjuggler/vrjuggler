#include <Math/vjVec3.h>

#ifndef _COLLIDOR_H_
#define _COLLIDOR_H_

//: Base class for collision detection
//
class collidor
{
public:
   virtual bool testMove(vjVec3 curPos, vjVec3 delta, vjVec3& correction) = 0;

   // Did it collide in the last frame
   // API specific functions could return more info about the collision
   bool didCollide()
   { return mDidCollide; }

protected:
   void setDidCollide(bool val)
   { mDidCollide = val; }

   bool mDidCollide;
};

#endif
