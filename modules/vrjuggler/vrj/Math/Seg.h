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

#ifndef _VRJ_SEG_H_
#define _VRJ_SEG_H_
//#pragma once

#include <vrj/vrjConfig.h>
#include <vrj/Math/Vec3.h>

namespace vrj
{

//: Seg: A ray segment.
//
// Used for intersections.
//
// NOTE: Results are undefined if dir is not normalized at all times.
class VJ_CLASS_API Seg
{
public:
    Seg()
    {}

    //: Sets the segment to be one that starts at _p1 and ends at _p2
    void makePts(const Vec3& _p1, const Vec3& _p2);

    //: Finds the point on the seg nearest to pt.
    // Returns the nearest point in nearPt
    //
    // Makes assumptions that all pt dir is normalized
    void findNearestPt(const Vec3& pt, Vec3& nearPt);

   //: Does the segment hit the triangle
   //!NOTE: Triangle uses CCW vertex ordering
   bool isectTriangle(const Vec3 _v1, const Vec3 _v2, const Vec3 _v3, float* t);

    //: Is the tValue in the range of the seg
   //! RETURNS: true if tDist > 0 and tDist < length
    bool tValueOnSeg(float tDist) const
    { return ((tDist >= 0.0) && (tDist <= length));}

    //: Set equal to seg transformed by mat
   void xform(const Matrix& mat, Seg& seg);

    Vec3    startPt()
    { return pos; }

    // PRE: dir MUST be normalized
    Vec3    endPt()
    { return pos + (dir*length);}

   float getLength()
   { return length; }

   //: Get the pt at the given t_value
   //! NOTE: t_val is a multiple of the direction
   Vec3 getPt(const float& t_val)
   { return (pos + (dir*t_val)); }


public:
    Vec3  pos;
    Vec3  dir;      //: Direction: Assumed to be normalized
   float   length;   //: Length of the segment
};

inline
void Seg::makePts(const Vec3& _p1, const Vec3& _p2)
{
   dir = _p2 - _p1;
   pos = _p1;
   length = dir.length();
   //dir.normalize();
   if(length != 0.0f)
   {
      dir /= length;        // Normalize it
   }
}

inline
void Seg::xform(const Matrix& mat, Seg& seg)
{
    /*
    pos.xformFull(mat, seg.pos);        // c = Mo
    dir.xformVec(mat, seg.dir);
    dir.normalize();
    length = seg.length;
    */

    Vec3 pt1, pt2, diff;
    pt1.xformFull(mat, seg.startPt());
    pt2.xformFull(mat, seg.endPt());
    diff = pt2-pt1;

    pos = pt1;
    length = diff.length();
    dir = (diff / length);
}

};
#endif
