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


#ifndef _VJ_QUAT_H_
#define _VJ_QUAT_H_

#include <math.h>

#include <Math/vjVec4.h>
#include <Math/vjVec3.h>
#include <Math/vjMatrix.h>

#define VJ_QUAT_EPSILON     0.00001f

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
//       SIGGRAPH Proceedings Vol 19, Number 3, 1985           <br>
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
   void makeRot( const vjMatrix& mat );

   //: get the quat's twist (radians) and vector
   void getRot( float& rad, float& x, float& y, float& z ) const
   {
      vjQuat quat = *this;
      
      // make sure we don't get a NaN result from acos...
      if (fabsf( quat.vec[VJ_W] ) > 1.0f)
      {
         quat.normalize();
      }

      float halfrad = acosf(quat.vec[VJ_W]);
      float sin_halfrad = sinf( halfrad );
      float oneOverSinHalfRad;
      
      if (sin_halfrad != 0.0f)
      {
         oneOverSinHalfRad = 1.0f / sin_halfrad;
      }
      
      // avoid NAN
      // if rad == 0, then the vector is undefined anyway, 
      // since there is no twist, there is no need for a vector (set to 0)
      else 
      {
         oneOverSinHalfRad = 0.0f;
      }
      
	   rad = halfrad * 2.0f;
      vjVec3 t;
	   t[0] = quat.vec[VJ_X] * ( oneOverSinHalfRad );
      t[1] = quat.vec[VJ_Y] * ( oneOverSinHalfRad );
		t[2] = quat.vec[VJ_Z] * ( oneOverSinHalfRad );
      //t.normalize();
      x = t[0];
      y = t[1];
      z = t[2];
      
      // avoid 0,0,0,0 when deg is 0.  make it 0,1,0,0
      if (oneOverSinHalfRad == 0.0f)
      {
         x = 1.0f;
      }
   }

   //: make a quat from a twist (radians) about a vector (normalized)
   void makeRot( const float& rad, const float& x, const float& y, const float& z )
   {
      float halfRad = rad * 0.5f;
	   float sinHalfRad = sinf( halfRad );
	   vjVec3 vecNormalized;
         
      if (rad == 0.0f || (x == 0.0f && y == 0.0f && z == 0.0f))
      {
         vecNormalized.set( 1.0f, 0.0f, 0.0f );
      }
      else
      {
         vecNormalized.set( x, y, z );
	      vecNormalized.normalize();
      }
	   vec[VJ_W] = cosf( halfRad );
	   vec[VJ_X] = sinHalfRad * vecNormalized[0];
	   vec[VJ_Y] = sinHalfRad * vecNormalized[1];
	   vec[VJ_Z] = sinHalfRad * vecNormalized[2];
	   this->normalizeFast();
   }
 
   //: set to conj of quat
   void conj(const vjQuat& quat)
   {
      *this = quat;
      vec[VJ_X] = -quat.vec[VJ_X];
      vec[VJ_Y] = -quat.vec[VJ_Y];
      vec[VJ_Z] = -quat.vec[VJ_Z];
   }

   float norm() const
   {
      return ((vec[0]*vec[0])+(vec[1]*vec[1])+(vec[2]*vec[2])+(vec[3]*vec[3]));
   }

   // make a unit quat (normalize it)
   void normalizeFast()
   { 
      float n = 1 / this->norm(); 
      vec[VJ_W] = vec[VJ_W] * n; 
      vec[VJ_X] = vec[VJ_X] * n; 
      vec[VJ_Y] = vec[VJ_Y] * n; 
      vec[VJ_Z] = vec[VJ_Z] * n; 
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
   
   // not implemented
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
   
   inline std::ostream& outStream( std::ostream& out )
   {
	   float rad;
	   float x, y, z;
	   this->getRot( rad, x, y, z );

	   //out << vec[VJ_W] << ", " << vec[VJ_X] << ", " << vec[VJ_Y] << ", " << vec[VJ_Z];
	   out << VJ_RAD2DEG( rad ) << " deg, " << x << ", " << y << ", " << z;
	   return out;
   }
   
   inline friend std::ostream& operator<<( std::ostream& out, vjQuat& q )
   {
	   q.outStream( out );
	   return out;
   }
};

#endif
