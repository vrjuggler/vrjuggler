#ifndef TERRY_POGOSTICK_COLLIDER
#define TERRY_POGOSTICK_COLLIDER

#include <Performer/pf.h>
#include <Performer/pf/pfNode.h>
#include <Performer/pr.h>
#include <Performer/pr/pfGeoSet.h>
#include <Performer/pfutil.h>
#include <iostream.h>
#include <math.h>

///////////////////////////////////////////////////////////////////////////////////
// Down vector "Pogostick" (or ground) collision
/////////////////////////////////

class pfTerryPogostickCollide : public pfTerryCollide
{
public:
   //: For collisions with the ground, use collide(
   //: displacement vector, the whole world, mask, your position
   //: length of collision vector).  
   //
   // The world and mask will
   // probably be the same ones you used in nodeSetup.
   // Make sure that the length of the vector is longer than
   // the rate at which you are falling or you might fall right
   // through the ground.
   bool collide( pfVec3 &bounce, pfNode *objNode, int mask, pfVec3 pos, float length );
   
   //: collideRide(displacement vector, position
   //: offset, rotation offset, the whole world, mask, your
   //: position) 
   //
   // is just like collide(...) but with two extra
   // parameters.  If you have animated geometry that you can
   // stand on, these parameters will tell you how much to
   // change your position and rotation by each frame to stay
   // on top of things.  If you want to stand up straight all
   // the time, you'll probably just want to use the heading
   // from the rotation offset.  The vector stores an hpr, but
   // the pitch and roll probably won't be useful.
   bool collideRide( pfVec3 &bounce, pfVec3 &ridexyz, pfVec3 &ridehpr, pfNode *objNode, int mask, pfVec3 pos, float length );
};
      

#endif
