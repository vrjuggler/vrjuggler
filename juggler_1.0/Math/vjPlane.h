/*
 *  File:          $RCSfile$
 *  Date modified: $Date$
 *  Version:       $Revision$
 *
 *
 *                                VR Juggler
 *                                    by
 *                              Allen Bierbaum
 *                             Christopher Just
 *                             Patrick Hartling
 *                            Carolina Cruz-Neira
 *                               Albert Baker
 *
 *                  Copyright (C) - 1997, 1998, 1999, 2000
 *              Iowa State University Research Foundation, Inc.
 *                            All Rights Reserved
 */


#ifndef _VJ_PLANE_H_
#define _VJ_PLANE_H_
//#pragma once

#include <vjConfig.h>
#include <Kernel/vjDebug.h>
#include <Math/vjVec3.h>
#include <Math/vjSeg.h>


//: sgPlane: Defines a geometrical plane.
//
// All points on the plane satify the equation dot(Pt,Normal) = offset
// normal is assumed to be normalized
//
// pg. 309 Computer Graphics 2nd Edition Hearn Baker
// N dot P = -D
//
class vjPlane
{
public:
	vjPlane()
	{}
	
	//: Create a plane containing the given points.
	void makePts(const vjVec3& pt1, const vjVec3& pt2, const vjVec3& pt3);
		
	//: Create a plane given a normal and a point
   void makeNormPt(const vjVec3& _pt, const vjVec3& norm);
	
	
	//: Intersects the plane with a given segment.
	//! RETURNS: TRUE if there is a hit (within the seg).
   //+         *t = distance "down" the segment to the hit.
   //
	//! PRE: seg.dir must be normalized
	bool isect(vjSeg& seg, float* t);
		
	
	//: Intersects the plane with the line defined
	// by the given segment
	//
	// seg - seg that represents the line to isect
	// t   - the t value of the isect
	bool isectLine(const vjSeg& seg, float* t);
	
	//: Find nearest pt on the plane
	//! RETURN: distance to the point (d)
	//+ If d is positive, pt lies on same side as normal
	//+ If d is negative, pt lies on opposite side from normal
	//+ if d is "near" zero, pt is on the plane
	float findNearestPt(const vjVec3& pt, vjVec3& nearPt);
	
public:
	vjVec3  normal;     // N or Just Normal
	float   offset;     // D or offset
};


/******************************************************
               INLINES
 ******************************************************/

/// Create a plane containing the given points.
inline
void vjPlane::makePts(const vjVec3& pt1, const vjVec3& pt2, const vjVec3& pt3)
{
    vjVec3 vec12(pt2-pt1);
    vjVec3 vec13(pt3-pt1);

    normal = vec12.cross(vec13);

    normal.normalize();
    offset = -1.0f * (pt1.dot(normal));	// Graphics Gems I: Page 390
}

inline
void vjPlane::makeNormPt(const vjVec3& norm, const vjVec3& _pt)
{
    normal = norm;
    offset = -(_pt.dot(normal));
}



// Finds the point on the plane nearest to pt.
// Returns the nearest point in nearPt
inline
float vjPlane::findNearestPt(const vjVec3& pt, vjVec3& nearPt)
{
        // GGI: P 297
		// GGII: Pg 223
	vjASSERT(normal.isNormalized());		// Assert: Normalized
	float dist_to_plane(0.0);
	dist_to_plane = offset+normal.dot(pt);		// Distance to plane
	nearPt = pt - (normal*dist_to_plane);
	return dist_to_plane;
}

// Intersects the plane with the line defined
// by the given segment
//
// seg - seg that represents the line to isect
// t   - the t value of the isect
inline
bool vjPlane::isectLine(const vjSeg& seg, float* t)
{
	// GGI: Pg 299
	// Lu = seg.pos;
	// Lv = seg.dir;
	// Jn = normal;
	// Jd = offset;
	
	float denom = normal.dot(seg.dir);
	if(VJ_IS_ZERO(denom))
      return false;
	else
	{
      *t = - (offset+ normal.dot(seg.pos))/(denom);
      return true;
	}
}

// Intersects the plane with a given segment.
// Returns TRUE if there is a hit (within the seg).
// Also returns the distance "down" the segment of the hit in t.
//
// PRE: seg.dir must be normalized
inline
bool vjPlane::isect(vjSeg& seg, float* t)
{
	if(!isectLine(seg, t))
      return false;           // Plane and seg are parallel
	
   if(seg.tValueOnSeg(*t))
      return true;
   else
      return false;
}





#endif
