/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998, 1999, 2000 by Iowa State University
 *
 * Original Authors:
 *   Allen Bierbaum, Christopher Just,
 *   Patrick Hartling, Kevin Meinert,
 *   Carolina Cruz-Neira, Albert Baker
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 *
 * -----------------------------------------------------------------
 * File:          $RCSfile$
 * Date modified: $Date$
 * Version:       $Revision$
 * -----------------------------------------------------------------
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

#ifndef _VRJ_PLANE_H_
#define _VRJ_PLANE_H_
//#pragma once

#include <vrj/vrjConfig.h>
#include <vrj/Util/Debug.h>
#include <vrj/Math/Vec3.h>
#include <vrj/Math/Seg.h>
#include <vrj/Math/Math.h>


namespace vrj
{

//: Plane: Defines a geometrical plane.
//
// All points on the plane satify the equation dot(Pt,Normal) = offset
// normal is assumed to be normalized
//
// pg. 309 Computer Graphics 2nd Edition Hearn Baker
// N dot P = -D
//
class Plane
{
public:
    Plane()
    {}

    //: Create a plane containing the given points.
    void makePts(const Vec3& pt1, const Vec3& pt2, const Vec3& pt3);

    //: Create a plane given a normal and a point
         void makeNormPt( const Vec3& norm, const Vec3& pt );


    //: Intersects the plane with a given segment.
    //! RETURNS: TRUE if there is a hit (within the seg).
   //+         *t = distance "down" the segment to the hit.
   //
    //! PRE: seg.dir must be normalized
    bool isect(Seg& seg, float* t);


    //: Intersects the plane with the line defined
    // by the given segment
    //
    // seg - seg that represents the line to isect
    // t   - the t value of the isect
    bool isectLine(const Seg& seg, float* t);

    //: Find nearest pt on the plane
    //! RETURN: distance to the point (d), and point on the plane
    //+ If d is positive, pt lies on same side as normal
    //+ If d is negative, pt lies on opposite side from normal
    //+ if d is "near" zero, pt is on the plane
    float findNearestPt(const Vec3& pt, Vec3& nearPt);

   // Finds the distance between pt and the point on the plane nearest pt
   //! RETURN: distance to the point (d)
    //+ If d is positive, pt lies on same side as normal
    //+ If d is negative, pt lies on opposite side from normal
    //+ if d is "near" zero, pt is on the plane
    float getDistance( const Vec3& pt );

public:
    Vec3  normal;     // N or Just Normal
    float   offset;     // D or offset
};


/******************************************************
               INLINES
 ******************************************************/

/// Create a plane containing the given points.
inline
void Plane::makePts(const Vec3& pt1, const Vec3& pt2, const Vec3& pt3)
{
    Vec3 vec12(pt2-pt1);
    Vec3 vec13(pt3-pt1);

    normal = vec12.cross(vec13);

    normal.normalize();
    offset = -1.0f * (pt1.dot(normal)); // Graphics Gems I: Page 390
}

inline
void Plane::makeNormPt(const Vec3& norm, const Vec3& _pt)
{
    normal = norm;
    offset = -(_pt.dot(normal));
}


// Finds the distance between pt and the point on the plane nearest pt
inline
float Plane::getDistance( const Vec3& pt )
{
   // GGI: P 297
    // GGII: Pg 223
    vprASSERT( normal.isNormalized() );     // Assert: Normalized
    return offset+normal.dot( pt );       // Distance to plane
}

// Finds the point on the plane nearest to pt.
// Returns the nearest point in nearPt
inline
float Plane::findNearestPt(const Vec3& pt, Vec3& nearPt)
{
        // GGI: P 297
        // GGII: Pg 223
    vprASSERT(normal.isNormalized());                   // Assert: Normalized
    float dist_to_plane = offset+normal.dot(pt);        // Distance to plane
    nearPt = pt - (normal*dist_to_plane);
    return dist_to_plane;
}

// Intersects the plane with the line defined
// by the given segment
//
// seg - seg that represents the line to isect
// t   - the t value of the isect
inline
bool Plane::isectLine(const Seg& seg, float* t)
{
    // GGI: Pg 299
    // Lu = seg.pos;
    // Lv = seg.dir;
    // Jn = normal;
    // Jd = offset;

    float denom = normal.dot(seg.dir);
    if(Math::isZero(denom))
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
bool Plane::isect(Seg& seg, float* t)
{
    if(!isectLine(seg, t))
      return false;           // Plane and seg are parallel

   if(seg.tValueOnSeg(*t))
      return true;
   else
      return false;
}



};

#endif
