/*
 *  File:	    $RCSfile$
 *  Date modified:  $Date$
 *  Version:	    $Revision$
 *
 *
 *                                VR Juggler
 *                                    by
 *                              Allen Bierbaum
 *                             Christopher Just
 *                            Carolina Cruz-Neira
 *                               Albert Baker
 *
 *                         Copyright  - 1997,1998,1999
 *                Iowa State University Research Foundation, Inc.
 *                            All Rights Reserved
 */


#ifndef _VJ_SEG_H_
#define _VJ_SEG_H_
#pragma once

#include <vjConfig.h>
#include <Math/vjVec3.h>

//: vjSeg: A ray segment.
//
// Used for intersections.
//
// NOTE: Results are undefined if dir is not normalized at all times.
class vjSeg
{
public:
	vjSeg()
	{}
	
	//: Sets the segment to be one that starts at _p1 and ends at _p2
	void makePts(const vjVec3& _p1, const vjVec3& _p2);
	
	//: Finds the point on the seg nearest to pt.
	// Returns the nearest point in nearPt
	//
	// Makes assumptions that all pt dir is normalized
	void findNearestPt(const vjVec3& pt, vjVec3& nearPt);
	
   //: Does the segment hit the triangle
   //!NOTE: Triangle uses CCW vertex ordering
   bool isectTriangle(const vjVec3 _v1, const vjVec3 _v2, const vjVec3 _v3, float* t);

	//: Is the tValue in the range of the seg
   //! RETURNS: true if tDist > 0 and tDist < length
	bool tValueOnSeg(float tDist) const
	{ return ((tDist >= 0.0) && (tDist <= length));}
	
	//: Set equal to seg transformed by mat
   void xform(const vjMatrix& mat, vjSeg& seg);
	
	vjVec3	startPt()
	{ return pos; }
	
	// PRE: dir MUST be normalized
	vjVec3	endPt()
	{ return pos + (dir*length);}

   float getLength()
   { return length; }

   //: Get the pt at the given t_value
   //! NOTE: t_val is a multiple of the direction
   vjVec3 getPt(const float& t_val)
   { return (pos + (dir*t_val)); }
	
	
public:
	vjVec3  pos;
	vjVec3  dir;      //: Direction: Assumed to be normalized
   float   length;   //: Length of the segment
};

inline
void vjSeg::makePts(const vjVec3& _p1, const vjVec3& _p2)
{
   dir = _p2 - _p1;
   pos = _p1;
   length = dir.length();
   //dir.normalize();
   if(length != 0.0f)
   {
      dir /= length;		// Normalize it
   }
}

inline
void vjSeg::xform(const vjMatrix& mat, vjSeg& seg)
{
	/*
	pos.xformFull(mat, seg.pos);	    // c = Mo
	dir.xformVec(mat, seg.dir);
	dir.normalize();
	length = seg.length;
	*/

	vjVec3 pt1, pt2, diff;
	pt1.xformFull(mat, seg.startPt());
	pt2.xformFull(mat, seg.endPt());
	diff = pt2-pt1;

	pos = pt1;
	length = diff.length();
	dir = (diff / length);
}


#endif
