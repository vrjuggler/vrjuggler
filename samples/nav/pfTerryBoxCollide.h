#ifndef TERRY_BOX_COLLIDE
#define TERRY_BOX_COLLIDE

#include <Performer/pf.h>
#include <Performer/pf/pfNode.h>
#include <Performer/pr.h>
#include <Performer/pr/pfGeoSet.h>
#include <Performer/pfutil.h>
#include <iostream.h>
#include <math.h>

///////////////////////////////////////////////////////////////////////////////////
// uses vectors to approximate a box or sphere volume collider
///////////////////////////////////////////////////////////////

class pfTerryBoxCollide : public pfTerryCollide
{
public:
   static const int COLLIDE_SEGMENTS;
   pfSegSet mCollideVolume;
   
   pfTerryBoxCollide( int isectMask = 0x01 );

   // set the intersect mask of this volume
   // needs to match every node tht you want to intersect with
   // read the performer manpage docs for more info 
   // (see pfNode, or pfGeoSet::isect function
   // NOTE: probably the same mask you used in nodeSetup
   void setup( const int& isectMask ); 
   
   // set the radius of the simulated box/sphere volume
   void setRadius(float radius);
   
   //: To collide with objects using the simulated volume
   //: routine, call collide(displacement vector, the whole
   //: world, your position).  
   //
   // You'll probably want to give one radius at the beginning of 
   // your application, but setRadius can be called over and over 
   // if you want your radius to change.  Make sure the radius is
   // larger than the fastest speed you can travel, otherwise
   // you may penetrate some walls.  The world node that you
   // provide represents your whole scene and is probably the
   // same node you passed to nodeSetup if you only
   // called it once.  This function returns >1 if there was
   // a collision and 0 otherwise.  If there is a collision,
   // the displacement vector will say what to offset your
   // position by to eliminate the collision.
   //
   // NOTE: before calling this function call setRadius(radius of volume) 
   //       first
   int collide( pfVec3 &bounce, pfNode *objNode, pfVec3 pos);
};

#endif
