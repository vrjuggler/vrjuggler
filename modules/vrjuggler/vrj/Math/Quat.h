/*
 *  File:	    $Name$
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


#ifndef _VJ_QUAT_H_
#define _VJ_QUAT_H_

#include <Math/vjVec4.h>
#include <Math/vjVec3.h>
#include <math.h>
#include <Math/vjMatrix.h>

#define VJ_QUAT_EPSILON	    0.00001f

// ----------------------------------------------------------------------------
//: Class to encapsulate quaternion behaviors.
//
// Note: The code for most of these routines was borrowed from the the folling
// sources.
//
//    Game Developer Magazine: Feb 98, pg.34-42                   <br>
//    Advanced Animation and Rendering Techniques: pp363-365      <br>
//    Quaternion Calculus for Animation,  Ken Shoemake SIGGRAPH course notes 1989  <br>
//    Animating Rotation with Quaternion Curves,  Ken Shoemake <br>
//		   SIGGRAPH Proceedings Vol 19, Number 3, 1985           <br>
//-----------------------------------------------------------------------------
//!PUBLIC_API:
class vjQuat : public vjVec4
{
public:
   vjQuat()
   {}

   //: Construct Quat from 4 floats
   vjQuat( const float x,
           const float y,
           const float z,
           const float w)
   {
      vec[VJ_X] = x;
      vec[VJ_Y] = y;
      vec[VJ_Z] = z;
      vec[VJ_W] = w;
   }

   //: Construct quat from matrix
   void makeQuat(vjMatrix& mat);

   //: Not implemented
   void getRot(float* angle, float* x, float* y, float* z)
   {;}

   //: Not implemented
   void makeRot(float angle, float x, float y, float z)
   {;}

   //: set to conj of quat
   void conj(const vjQuat& quat)
   {
      *this = quat;
      normalize();
      vec[VJ_X] = -quat.vec[VJ_X];
      vec[VJ_Y] = -quat.vec[VJ_Y];
      vec[VJ_Z] = -quat.vec[VJ_Z];
   }

   float norm() const
   {
      return ((vec[0]*vec[0])+(vec[1]*vec[1])+(vec[2]*vec[2])+(vec[3]*vec[3]));
   }


public:
   //: Multiply two quaternions
   //!POST: this' = q1 * q2
   void mult(const vjQuat& q1, const vjQuat& q2);

   // quat = q1/q2
   void div(const vjQuat& q1, const vjQuat& q2)
   {
      vjQuat r, s;
      vjQuat inv_denom;
      inv_denom.invert(q2);

      r.mult(q1, inv_denom);
      s.mult(inv_denom, inv_denom);

      vec[VJ_X] = r.vec[VJ_X] / s.vec[VJ_W];
      vec[VJ_Y] = r.vec[VJ_Y] / s.vec[VJ_W];
      vec[VJ_Z] = r.vec[VJ_Z] / s.vec[VJ_W];
      vec[VJ_W] = r.vec[VJ_W] / s.vec[VJ_W];
   }

   // quat = inv(q1)
   void invert(const vjQuat& q1);

   //void exp(const vjQuat& _q);
   //void log(const vjQuat& _q);

   void slerp(float _t, const vjQuat& _q1, const vjQuat& _q2);
   void squad(float _t, const vjQuat& _q1, const vjQuat& _q2, const vjQuat& _a, const vjQuat& _b);

   //void meanTangent(const vjQuat& _q1, const vjQuat& _q2, const vjQuat& _q3);

public:
   // vjQuat operators
   vjQuat operator *(const vjQuat&  _m) const {
      vjQuat dst; dst.mult(*this, _m); return dst;
   }

   vjQuat operator /(const vjQuat&  _m) const {
      vjQuat dst; dst.div(*this, _m); return dst;
   }

public:
   // Assignment operators
   vjQuat&  operator *=(const vjQuat& _q) {
      this->mult(*this, _q); return *this;
   }
   vjQuat&  operator /=(const vjQuat& _q) {
      this->div(*this, _q); return *this;
   }

};

#endif
