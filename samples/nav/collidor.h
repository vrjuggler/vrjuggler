#include <Math/vjVec3.h>

#ifndef _COLLIDOR_H_
#define _COLLIDOR_H_

//: Base class for collision detection
//
// All it does is test movements to see if they are valid.
//  Any motion (nav) model can make use of a collidor to
// check for collisions and to modify the motion
//
class collidor
{
public:

   //: test a movement
   //!ARGS: curPos - The current position we are at
   //!ARGS: delta - The amount we want to move
   //!ARGS: correction - The amount to correct the movement so that we do not collide
   //!ARGS: curPosWithDelta - Does the curPos already have delta added in?
   //!RETURNS: true - There was a hit.
   virtual bool testMove(vjVec3 curPos, vjVec3 delta, vjVec3& correction, bool curPosWithDelta = false) = 0;

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
