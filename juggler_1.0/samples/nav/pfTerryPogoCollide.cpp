#include "pfTerryPogoCollide.h"

bool pfTerryPogoCollide::collide( pfVec3 &bounce, pfNode *objNode, int mask, pfVec3 pos, float length )
{
    pfHit **hit[1];
    pfSegSet segset;
    pfMatrix collidemat;
    
    // Make a ray looking "down" at terrain
    segset.segs[0].pos = pos;
    segset.segs[0].dir = pfVec3(0.0, 0.0, -1.0);
    segset.segs[0].length = length;

    segset.mode = PFTRAV_IS_PRIM|PFTRAV_IS_NORM|PFTRAV_IS_CULL_BACK;
    segset.userData = NULL;
    segset.isectMask = mask;
    segset.activeMask = 0x1;
    segset.bound = NULL;
    segset.discFunc = NULL;

   // Query for collision data
   if (objNode->isect(&segset, hit) > 0)
   {
      uint query[] = {PFQHIT_FLAGS, PFQHIT_SEGNUM, PFQHIT_POINT, NULL};
	   
      // get the intersection results.
      pfHitQueryResult isectResult;
	   hit[0][0]->mQuery( query, &isectResult );
      
	   if(isectResult.flags & PFHIT_XFORM)
      {    
         // correct for DCS transformations
	      hit[0][0]->query( PFQHIT_XFORM, &collidemat );
	      isectResult.point.xformPt( isectResult.point, collidemat );
	   }
	   pos.sub( isectResult.point, pos );
	   bounce.add( pos, pfVec3(0.0, 0.0, length) );
	   return true;
   }
   return false;
}



bool pfTerryPogoCollide::collideRide( pfVec3 &bounce, pfVec3 &ridexyz, pfVec3 &ridehpr, pfNode *objNode, int mask, pfVec3 pos, float length )
{
   pfHit** hit[1];
   pfSegSet segset;
   pfNode* newnode( NULL );
   pfVec3 originalpoint, oldpoint;
   pfCoord coord;
   pfMatrix collidemat;
   
   // TODO: remove statics.
   static pfNode* oldnode( NULL );
   static pfMatrix oldmat;

   // Make a ray looking "down" at terrain
   segset.segs[0].pos = pos;
   segset.segs[0].dir = pfVec3(0.0, 0.0, -1.0);
   segset.segs[0].length = length;

   segset.mode = PFTRAV_IS_PRIM|PFTRAV_IS_NORM|PFTRAV_IS_CULL_BACK;
   segset.userData = NULL;
   segset.isectMask = mask;
   segset.activeMask = 0x1;
   segset.bound = NULL;
   segset.discFunc = NULL;

   // Query for collision data
   if (objNode->isect( &segset, hit ) > 0)
   {
	   uint query[] = {PFQHIT_FLAGS, PFQHIT_SEGNUM, PFQHIT_POINT, NULL};
	   
      // get the intersection results.
      pfHitQueryResult isectResult;
      hit[0][0]->mQuery( query, &isectResult );
	   
      if (isectResult.flags & PFHIT_XFORM)
      {    
         // correct for DCS transformations
	      hit[0][0]->query( PFQHIT_XFORM, &collidemat );
	      hit[0][0]->query( PFQHIT_NODE, &newnode );
	      originalpoint = isectResult.point;
	      isectResult.point.xformPt( isectResult.point, collidemat );
	      if (newnode == oldnode)
         {
		      oldpoint.xformPt( originalpoint, oldmat );
		      ridexyz.sub( isectResult.point, oldpoint );
		      collidemat.getOrthoCoord( &coord );
		      ridehpr = coord.hpr;
		      oldmat.getOrthoCoord( &coord );
		      ridehpr.sub( ridehpr, coord.hpr );
         }

         else
         {
		      oldnode = newnode;
		      ridexyz = pfVec3( 0.0, 0.0, 0.0 );
		      ridehpr = pfVec3( 0.0, 0.0, 0.0 );
	      }

         oldmat = collidemat;
	   }
	   
      else
      {
	       oldnode = NULL;
	       ridexyz = pfVec3( 0.0, 0.0, 0.0 );
	       ridehpr = pfVec3( 0.0, 0.0, 0.0 );
	   }
	   pos.sub( isectResult.point, pos );
	   bounce.add( pos, pfVec3(0.0, 0.0, length) );
	   return true;
   }
   
   else
   {
	   oldnode = NULL;
	   ridexyz = pfVec3( 0.0, 0.0, 0.0 );
	   ridehpr = pfVec3( 0.0, 0.0, 0.0 );
   }
   return false;
}
