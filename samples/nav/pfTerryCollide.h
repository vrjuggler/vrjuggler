#ifndef TERRY_COLLIDE
#define TERRY_COLLIDE

#include <Performer/pf.h>
#include <Performer/pf/pfNode.h>
#include <Performer/pr.h>
#include <Performer/pr/pfGeoSet.h>
#include <Performer/pfutil.h>
#include <iostream.h>
#include <math.h>

///////////////////////////////////////////////////////////////////////////////////
// base class for terry's nav routines.
////////////////////////////////////////////////////////////////////////////////

class pfTerryCollide
{
public:
   enum Flag
   {
      COLLIDE_STATIC = 0,
      COLLIDE_DYNAMIC = 1
   };
      
   //: Setup the node you want to collide with using
   //: nodeSetup(node to collide with, COLLIDE_STATIC or
   //: COLLIDE_DYNAMIC, mask)
   //
   // Usually you'll pass a node representing your entire
   // world and a simple mask such as 0x01.  The two different
   // collide modes control caching.  Objects that will always
   // remain in the same place should be COLLIDE_STATIC.
   // This function is not necessary if you don't want to
   // distinguish different geometry using different masks.
   void nodeSetup( pfNode* node, int mode, int mask );
   
   // data structure to get intersection result back from pfHit::mQuery
   class pfHitQueryResult
   {
   public:
       int     flags;
       int     whichseg;
       pfVec3  point;
       pfVec3  normal;
   };
};

#endif
