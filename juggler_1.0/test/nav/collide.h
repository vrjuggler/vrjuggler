// Awesome Performer collision routines by Terry Welsh
// February 8, 1998
//
// These functions provide a simulated volume collision
// detection routine and a ground collision detection
// routine.  The volume is simulated using eight vectors
// and provides suitable collision for most architectural
// walkthroughs and similar applications.  Any corner
// that is >= 90 degrees should be impossible to penetrate.
//
// Instructions:
//
// 1.  Setup the node you want to collide with using
// collideNodeSetup(node to collide with, COLLIDE_STATIC or
// COLLIDE_DYNAMIC, mask)
// Usually you'll pass a node representing your entire
// world and a simple mask such as 0x01.  The two different
// collide modes control caching.  Objects that will always
// remain in the same place should be COLLIDE_STATIC.
// This function is not necessary if you don't want to
// distinguish different geometry using different masks.
//
// 2.  Provide collideVolumeSetup with a mask for what you
// want to collide with (probably the same mask you used in
// collideNodeSetup.
//
// 3.  To collide with objects using the simulated volume
// routine, call collideVolumeRadius(radius of volume) and
// then call collideVolume(displacement vector, the whole
// world, your position).  You'll probably want to give
// one radius at the beginning of your application, but
// collideVolumeRadius can be called over and over if you
// want your radius to change.  Make sure the radius is
// larger than the fastest speed you can travel, otherwise
// you may penetrate some walls.  The world node that you
// provide represents your whole scene and is probably the
// same node you passed to collideNodeSetup if you only
// called it once.  This function returns a 1 if there was
// a collision and a 0 otherwise.  If there is a collision,
// the displacement vector will say what to offset your
// position by to eliminate the collision.
//
// 4.  For collisions with the ground, use collideGround(
// displacement vector, the whole world, mask, your position
// length of collision vector).  The world and mask will
// probably be the same ones you used in collideNodeSetup.
// Make sure that the length of the vector is longer than
// the rate at which you are falling or you might fall right
// through the ground.
//     collideGroundAndRide(displacement vector, position
// offset, rotation offset, the whole world, mask, your
// position) is just like collideGround but with two extra
// parameters.  If you have animated geometry that you can
// stand on, these parameters will tell you how much to
// change your position and rotation by each frame to stay
// on top of things.  If you want to stand up straight all
// the time, you'll probably just want to use the heading
// from the rotation offset.  The vector stores an hpr, but
// the pitch and roll probably won't be useful.

#ifndef _COLLIDE_H_
#define _COLLIDE_H_

#include <Performer/pf.h>
#include <Performer/pf/pfNode.h>
#include <Performer/pr.h>
#include <Performer/pr/pfGeoSet.h>
#include <Performer/pfutil.h>
#include <iostream.h>
#include <math.h>

#define COLLIDE_STATIC 0
#define COLLIDE_DYNAMIC 1
#define COLLIDE_SEGMENTS 8

struct {
    int flags;
    int whichseg;
    pfVec3 point;
    pfVec3 normal;
} isectResult;
pfMatrix collidemat;
static pfSegSet mySegset;


void collideVolumeSetup(int mask);
void collideVolumeRadius(float radius);
void collideNodeSetup(pfNode* node, int mode, int mask);
int collideGround(pfVec3 &bounce, pfNode *objNode, int mask, pfVec3 pos, float length);
int collideGroundAndRide(pfVec3 &bounce, pfVec3 &ridexyz, pfVec3 &ridehpr, pfNode *objNode, int mask, pfVec3 pos, float length);
int collideVolume(pfVec3 &bounce, pfNode *objNode, pfVec3 pos);





void collideVolumeSetup(int mask){
    mySegset.mode = PFTRAV_IS_PRIM|PFTRAV_IS_NORM|PFTRAV_IS_CULL_BACK;
    mySegset.userData = NULL;
    mySegset.isectMask = mask;
    mySegset.activeMask = 0xff;		    // test all segments
    mySegset.bound = NULL;
    mySegset.discFunc = NULL;
    mySegset.segs[0].dir = pfVec3(0.57735, 0.57735, 0.57735);
    mySegset.segs[1].dir = pfVec3(0.57735, 0.57735, -0.57735);
    mySegset.segs[2].dir = pfVec3(0.57735, -0.57735, 0.57735);
    mySegset.segs[3].dir = pfVec3(0.57735, -0.57735, -0.57735);
    mySegset.segs[4].dir = pfVec3(-0.57735, 0.57735, 0.57735);
    mySegset.segs[5].dir = pfVec3(-0.57735, 0.57735, -0.57735);
    mySegset.segs[6].dir = pfVec3(-0.57735, -0.57735, 0.57735);
    mySegset.segs[7].dir = pfVec3(-0.57735, -0.57735, -0.57735);
}


//  Effectively sets the radius of your bounding volume
void collideVolumeRadius(float radius){
    int i;

    for(i = 0; i < COLLIDE_SEGMENTS; i++){
	mySegset.segs[i].length = radius;
    }
}


/*
 * Set up intersection masks for collision detection using 'mask' as the
 * intersection mask. Cache normals and such inside geosets if the geometry
 * is static, but not if it is dynamic, i.e., its vertices change.
 */
void collideNodeSetup(pfNode *node, int mode, int mask){
    int fullmode;

    if (mode == COLLIDE_DYNAMIC)
	fullmode = PFTRAV_SELF|PFTRAV_DESCEND|PFTRAV_IS_UNCACHE;
    else
	fullmode = PFTRAV_SELF|PFTRAV_DESCEND|PFTRAV_IS_CACHE;
    node->setTravMask(PFTRAV_ISECT, mask, fullmode, PF_SET);
}


int collideGround(pfVec3 &bounce, pfNode *objNode, int mask, pfVec3 pos, float length){
    static pfHit **hit[1];
    static pfSegSet segset;

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
    if(objNode->isect(&segset, hit) > 0){
	uint query[] = {PFQHIT_FLAGS, PFQHIT_SEGNUM, PFQHIT_POINT, NULL};
	hit[0][0]->mQuery(query, &isectResult);
	if(isectResult.flags & PFHIT_XFORM){    // correct for DCS transformations
	    hit[0][0]->query(PFQHIT_XFORM, &collidemat);
	    isectResult.point.xformPt(isectResult.point, collidemat);
	}
	pos.sub(isectResult.point, pos);
	bounce.add(pos, pfVec3(0.0, 0.0, length));
	return(1);
    }
    return(0);
}


int collideGroundAndRide(pfVec3 &bounce, pfVec3 &ridexyz, pfVec3 &ridehpr, pfNode *objNode, int mask, pfVec3 pos, float length){
    static pfHit **hit[1];
    static pfSegSet segset;
    static pfNode *newnode, *oldnode;
    static pfMatrix oldmat;
    static pfVec3 originalpoint, oldpoint;
    static pfCoord coord;

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
    if(objNode->isect(&segset, hit) > 0){
	uint query[] = {PFQHIT_FLAGS, PFQHIT_SEGNUM, PFQHIT_POINT, NULL};
	hit[0][0]->mQuery(query, &isectResult);
	if(isectResult.flags & PFHIT_XFORM){    // correct for DCS transformations
	    hit[0][0]->query(PFQHIT_XFORM, &collidemat);
	    hit[0][0]->query(PFQHIT_NODE, &newnode);
	    originalpoint = isectResult.point;
	    isectResult.point.xformPt(isectResult.point, collidemat);
	    if(newnode == oldnode){
		oldpoint.xformPt(originalpoint, oldmat);
		ridexyz.sub(isectResult.point, oldpoint);
		collidemat.getOrthoCoord(&coord);
		ridehpr = coord.hpr;
		oldmat.getOrthoCoord(&coord);
		ridehpr.sub(ridehpr, coord.hpr);
	    }
	    else{
		oldnode = newnode;
		ridexyz = pfVec3(0.0, 0.0, 0.0);
		ridehpr = pfVec3(0.0, 0.0, 0.0);
	    }
	    oldmat = collidemat;
	}
	else{
	    oldnode = NULL;
	    ridexyz = pfVec3(0.0, 0.0, 0.0);
	    ridehpr = pfVec3(0.0, 0.0, 0.0);
	}
	pos.sub(isectResult.point, pos);
	bounce.add(pos, pfVec3(0.0, 0.0, length));
	return(1);
    }
    else{
	oldnode = NULL;
	ridexyz = pfVec3(0.0, 0.0, 0.0);
	ridehpr = pfVec3(0.0, 0.0, 0.0);
    }
    return(0);
}


int collideVolume(pfVec3 &bounce, pfNode *objNode, pfVec3 pos){
   static int i, j, returnval;
   static float distance;
   static pfVec3 tempvec;
   static pfHit **hits[COLLIDE_SEGMENTS];
   static uint query[] = {PFQHIT_FLAGS, PFQHIT_SEGNUM, PFQHIT_POINT, PFQHIT_NORM, NULL};
   static pfMatrix xform;

   // Set collision positions
   for (i = 0; i < COLLIDE_SEGMENTS; i++)
   {
      mySegset.segs[i].pos = pos;
   }

   // Query for collision data
   returnval = 0;
   bounce = pfVec3(0, 0, 0);
   if (objNode->isect(&mySegset, hits) > 0)
   {
      for (i = 0; i < COLLIDE_SEGMENTS; i++)
      {
         hits[i][0]->mQuery(query, &isectResult);
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
            distance = mySegset.segs[isectResult.whichseg].length - distance;
            tempvec.negate(mySegset.segs[isectResult.whichseg].dir);
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


#endif
