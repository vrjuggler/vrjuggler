#include "collide.h"

const int pfTerryVCollide::COLLIDE_SEGMENTS( 8 );

pfTerryVCollide::pfTerryVCollide( int isectMask )
{
    mCollideVolume.mode = PFTRAV_IS_PRIM|PFTRAV_IS_NORM|PFTRAV_IS_CULL_BACK;
    mCollideVolume.userData = NULL;
    mCollideVolume.isectMask = isectMask;
    mCollideVolume.activeMask = 0xff;		    // test all segments
    mCollideVolume.bound = NULL;
    mCollideVolume.discFunc = NULL;
    mCollideVolume.segs[0].dir = pfVec3(0.57735, 0.57735, 0.57735);
    mCollideVolume.segs[1].dir = pfVec3(0.57735, 0.57735, -0.57735);
    mCollideVolume.segs[2].dir = pfVec3(0.57735, -0.57735, 0.57735);
    mCollideVolume.segs[3].dir = pfVec3(0.57735, -0.57735, -0.57735);
    mCollideVolume.segs[4].dir = pfVec3(-0.57735, 0.57735, 0.57735);
    mCollideVolume.segs[5].dir = pfVec3(-0.57735, 0.57735, -0.57735);
    mCollideVolume.segs[6].dir = pfVec3(-0.57735, -0.57735, 0.57735);
    mCollideVolume.segs[7].dir = pfVec3(-0.57735, -0.57735, -0.57735);
    mCollideVolume.segs[0].length = 1.0f;
    mCollideVolume.segs[1].length = 1.0f;
    mCollideVolume.segs[2].length = 1.0f;
    mCollideVolume.segs[3].length = 1.0f;
    mCollideVolume.segs[4].length = 1.0f;
    mCollideVolume.segs[5].length = 1.0f;
    mCollideVolume.segs[6].length = 1.0f;
    mCollideVolume.segs[7].length = 1.0f;
}   

// set the intersect mask of this volume
// needs to match every node tht you want to intersect with
// read the performer docs for more info (see pfNode, or pfGeoSet::isect function)
void pfTerryVCollide::setup( const int& isectMask )
{
   mCollideVolume.isectMask = isectMask;
}   

//  Effectively sets the radius of your bounding volume
void pfTerryVCollide::setVolumeRadius( float radius )
{
   int i;

   for (i = 0; i < COLLIDE_SEGMENTS; i++)
   {
      mCollideVolume.segs[i].length = radius;
   }
}

// Set up intersection masks for collision detection using 'mask' as the
// intersection mask. Cache normals and such inside geosets if the geometry
// is static, but not if it is dynamic, i.e., its vertices change.
void pfTerryCollide::nodeSetup( pfNode *node, int mode, int mask )
{
    int fullmode;

    if (mode == COLLIDE_DYNAMIC)
	fullmode = PFTRAV_SELF|PFTRAV_DESCEND|PFTRAV_IS_UNCACHE;
    else
	fullmode = PFTRAV_SELF|PFTRAV_DESCEND|PFTRAV_IS_CACHE;
    node->setTravMask(PFTRAV_ISECT, mask, fullmode, PF_SET);
}

bool pfTerryRCollide::collideGround( pfVec3 &bounce, pfNode *objNode, int mask, pfVec3 pos, float length )
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



bool pfTerryRCollide::collideGroundAndRide( pfVec3 &bounce, pfVec3 &ridexyz, pfVec3 &ridehpr, pfNode *objNode, int mask, pfVec3 pos, float length )
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

int pfTerryVCollide::collideVolume(pfVec3 &bounce, pfNode *objNode, pfVec3 pos)
{   
   int i, j, returnval = 0;
   float distance;
   pfVec3 tempvec;
   pfHit **hits[COLLIDE_SEGMENTS];
   //pfMatrix xform;
   pfMatrix collidemat;
   
   // this is const, but pf doesn't take a const...
   static unsigned int query[] = {PFQHIT_FLAGS, PFQHIT_SEGNUM, PFQHIT_POINT, PFQHIT_NORM, NULL};
   
   // Set collision positions
   for (i = 0; i < COLLIDE_SEGMENTS; i++)
   {
      mCollideVolume.segs[i].pos = pos;
   }

   // Query for collision data
   bounce = pfVec3(0, 0, 0);
   if (objNode->isect(&mCollideVolume, hits) > 0)
   {
      for (i = 0; i < COLLIDE_SEGMENTS; i++)
      {
         hits[i][0]->mQuery( query, &isectResult );
         if ((isectResult.flags & PFHIT_POINT) && (isectResult.flags & PFHIT_NORM))
         {
            if (isectResult.flags & PFHIT_XFORM)
            {   // correct for DCS transformations
               hits[i][0]->query(PFQHIT_XFORM, &collidemat);
               isectResult.point.xformPt(isectResult.point, collidemat);
               isectResult.normal.xformVec(isectResult.normal, collidemat);
               isectResult.normal.normalize();
            }
            tempvec.sub(isectResult.point, pos);
            distance = tempvec.length();
            distance = mCollideVolume.segs[isectResult.whichseg].length - distance;
            tempvec.negate(mCollideVolume.segs[isectResult.whichseg].dir);
            tempvec.scale(distance, tempvec);
            distance = tempvec.dot(isectResult.normal);
            tempvec.scale(distance, isectResult.normal);
            for (j = 0; j < 3; j++)
            {     // Add on only necessary vector components
               if (tempvec[j] > 0)
               {
                  if (bounce[j] <= 0)
                     bounce[j] += tempvec[j];
                  else
                  {
                     if (tempvec[j] > bounce[j])
                        bounce[j] = tempvec[j];
                  }
               }
               else
               {
                  if (bounce[j] >= 0)
                     bounce[j] += tempvec[j];
                  else
                  {
                     if (tempvec[j] < bounce[j])
                        bounce[j] = tempvec[j];
                  }
               }
            }
            returnval++;
         }
      }
   }
   return returnval;
}



