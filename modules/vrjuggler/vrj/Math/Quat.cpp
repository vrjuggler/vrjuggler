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

#include <vrj/vrjConfig.h>
#include <vpr/Util/Assert.h>
#include <vrj/Math/Quat.h>
#include <vrj/Math/Math.h>

namespace vrj
{

// set self to the rotation in the given rotation matrix
void Quat::makeRot( const Matrix& mat )
{
   float tr, s;
   //static int nxt[3] = {VJ_Y, VJ_Z, VJ_X};

   tr = mat.mat[0][0] + mat.mat[1][1] + mat.mat[2][2];

   // Check the diagonal
   if (tr > 0.0)
   {
      s = Math::sqrt( tr + 1.0 );
      vec[VJ_W] = s/2.0;
      s = 0.5/s;

      vec[VJ_X] = (mat.mat[1][2] - mat.mat[2][1])*s;
      vec[VJ_Y] = (mat.mat[2][0] - mat.mat[0][2])*s;
      vec[VJ_Z] = (mat.mat[0][1] - mat.mat[1][0])*s;
   }
   else     // Diagonal is negative
   {
      int   i, j, k;
      int   nxt[3] = {VJ_Y, VJ_Z, VJ_X};

      i = VJ_X;

      if (mat.mat[VJ_Y][VJ_Y] > mat.mat[VJ_X][VJ_X])
      {
         i = VJ_Y;
      }
      if (mat.mat[VJ_Z][VJ_Z] > mat.mat[i][i])
      {
         i = VJ_Z;
      }

      j = nxt[i];
      k = nxt[j];

      s = Math::sqrt( (mat.mat[i][i] - (mat.mat[j][j] + mat.mat[k][k])) + 1.0 );

      vec[i] = s * 0.5;

      if (s != 0.0)
      {
         s = (0.5 / s);
      }

      vec[VJ_W] = (mat.mat[j][k] - mat.mat[k][j]) * s;
      vec[j] = (mat.mat[i][j] + mat.mat[j][i]) * s;
      vec[k] = (mat.mat[i][k] + mat.mat[k][i]) * s;
   }

}

//: make a quat from a twist (radians) about a vector (normalized)
void Quat::makeRot( const float& rad, const float& x, const float& y, const float& z )
{
   // q = [ cos(rad/2), sin(rad/2) * [x,y,z] ]
   Vec3 vec_normalized( x, y, z );
   if (vec_normalized.length() > VJ_QUAT_EPSILON)
   {
      vec_normalized.normalize();
   }

   float half_angle = rad * 0.5f;
   float sin_half_angle = Math::sin( half_angle );

   vec[VJ_W] = Math::cos( half_angle );
   vec[VJ_X] = sin_half_angle * vec_normalized[0];
   vec[VJ_Y] = sin_half_angle * vec_normalized[1];
   vec[VJ_Z] = sin_half_angle * vec_normalized[2];

   // should automagically be normalized (unit magnitude) now...
}

//: get the quat's twist (radians) and vector
void Quat::getRot( float& rad, float& xx, float& yy, float& zz ) const
{
   // make sure we don't get a NaN result from acos...
   Quat quat( *this );
   if (Math::abs( quat.vec[VJ_W] ) > 1.0f)
   {
      quat.normalize();
   }
   vprASSERT( Math::abs( quat.vec[VJ_W] ) <= 1.0f && "acos returns NaN when |arg| > 1" );


   // [acos( w ) * 2.0, v / (asin( w ) * 2.0)]

   // get the rotation:
   rad = Math::acos( quat.vec[VJ_W] ) * 2.0f;

   // get the axis: (use sin(rad) instead of asin(w))
   float sin_half_angle = Math::sin( rad * 0.5f );
   if (sin_half_angle >= VJ_QUAT_EPSILON)
   {
      float sin_half_angle_inv = 1.0f / sin_half_angle;
      xx = quat.vec[VJ_X] * sin_half_angle_inv;
      yy = quat.vec[VJ_Y] * sin_half_angle_inv;
      zz = quat.vec[VJ_Z] * sin_half_angle_inv;
   }

   // avoid NAN
   else
   {
      xx = 1.0f - quat.vec[VJ_W]; // one of the terms should be a 1,
      yy = 0.0f;                  // so we can maintain unit-ness
      zz = 0.0f;                  // in case w is 0 (which here w is 0)
   }
}

// Multiply the two quaternions
// From gdmag
void Quat::mult( const Quat& q1, const Quat& q2 )
{
   // Here is the easy to understand equation: (grassman product)
   // scalar_component = q1.s * q2.s - dot(q1.v, q2.v)
   // vector_component = q2.v * q1.s + q1.v * q2.s + cross(q1.v, q2.v)

   // Here is another version (euclidean product, just FYI)...
   // scalar_component = q1.s * q2.s + dot(q1.v, q2.v)
   // vector_component = q2.v * q1.s - q1.v * q2.s - cross(q1.v, q2.v)

   // Here it is, implemented using vector algebra (grassman product)
   /*
   const float& w1( q1[VJ_W] ), w2( q2[VJ_W] );
   Vec3 v1( q1[VJ_X], q1[VJ_Y], q1[VJ_Z] ), v2( q2[VJ_X], q2[VJ_Y], q2[VJ_Z] );

   float w = w1 * w2 - v1.dot( v2 );
   Vec3 v = (w1 * v2) + (w2 * v1) + v1.cross( v2 );

   vec[VJ_W] = w;
   vec[VJ_X] = v[0];
   vec[VJ_Y] = v[1];
   vec[VJ_Z] = v[2];
   */

   // Here is the same, only expanded... (grassman product)
   Quat temporary;
   temporary[VJ_X] = q1[VJ_W]*q2[VJ_X] + q1[VJ_X]*q2[VJ_W] + q1[VJ_Y]*q2[VJ_Z] - q1[VJ_Z]*q2[VJ_Y];
   temporary[VJ_Y] = q1[VJ_W]*q2[VJ_Y] + q1[VJ_Y]*q2[VJ_W] + q1[VJ_Z]*q2[VJ_X] - q1[VJ_X]*q2[VJ_Z];
   temporary[VJ_Z] = q1[VJ_W]*q2[VJ_Z] + q1[VJ_Z]*q2[VJ_W] + q1[VJ_X]*q2[VJ_Y] - q1[VJ_Y]*q2[VJ_X];
   temporary[VJ_W] = q1[VJ_W]*q2[VJ_W] - q1[VJ_X]*q2[VJ_X] - q1[VJ_Y]*q2[VJ_Y] - q1[VJ_Z]*q2[VJ_Z];

   // use a temporary, in case q1 or q2 is the same as self.
   vec[VJ_X] = temporary[VJ_X];
   vec[VJ_Y] = temporary[VJ_Y];
   vec[VJ_Z] = temporary[VJ_Z];
   vec[VJ_W] = temporary[VJ_W];

   // don't normalize, because it might not be rotation arithmetic we're doing
   // (only rotation quats have unit length)
}

// From Adv Anim and Rendering Tech. Pg 364
void Quat::slerp(float t, const Quat& p, const Quat& q)
{
   Quat to;
   double omega, cosom, sinom, sclp, sclq;
   //int i;

   // calc cosine
   cosom = p[VJ_X]*q[VJ_X] + p[VJ_Y]*q[VJ_Y] + p[VJ_Z]*q[VJ_Z]
           + p[VJ_W]*q[VJ_W];


   // adjust signs (if necessary)
   if ( cosom < 0.0 )
   {
      cosom = -cosom;
      to.vec[0] = -q.vec[0];   // Reverse all signs
      to.vec[1] = -q.vec[1];
      to.vec[2] = -q.vec[2];
      to.vec[3] = -q.vec[3];
   }
   else
   {
      to = q;
   }

   // Calculate coefficients
   if ((1.0 - cosom) > VJ_QUAT_EPSILON)
   {
      // Standard case (slerp)
      omega = Math::acos(cosom);
      sinom = Math::sin(omega);
      sclp  = Math::sin((1.0 - t)*omega)/sinom;
      sclq  = Math::sin(t*omega)/sinom;
   }
   else
   {
      // Very close, do linear interp
      sclp = 1.0 - t;
      sclq = t;
   }

   vec[VJ_X] = sclp*p[VJ_X] + sclq*to[VJ_X];
   vec[VJ_Y] = sclp*p[VJ_Y] + sclq*to[VJ_Y];
   vec[VJ_Z] = sclp*p[VJ_Z] + sclq*to[VJ_Z];
   vec[VJ_W] = sclp*p[VJ_W] + sclq*to[VJ_W];

}

void Quat::squad(float _t, const Quat& _q1, const Quat& _q2, const Quat& _a, const Quat& _b)
{
   vprASSERT( false && "not implemented" );
}

//: complex exponentiation
//!POST: sets self to the exponentiation of quat
// NOTE: safe to pass self as argument
void Quat::exp( const Quat& quat )
{
   float len1, len2;

   len1 = Math::sqrt( quat.vec[VJ_X] * quat.vec[VJ_X] +
                          quat.vec[VJ_Y] * quat.vec[VJ_Y] +
                          quat.vec[VJ_Z] * quat.vec[VJ_Z] );
   if (len1 > 0.0f)
      len2 = Math::sin( len1 ) / len1;
   else
      len2 = 1.0f;

   vec[VJ_X] = quat.vec[VJ_X] * len2;
   vec[VJ_Y] = quat.vec[VJ_Y] * len2;
   vec[VJ_Z] = quat.vec[VJ_Z] * len2;
   vec[VJ_W] = Math::cos( len1 );
}

//: complex logarithm
//!POST: sets self to the log of quat
void Quat::log( const Quat& quat )
{
   float length;

   length = Math::sqrt( quat.vec[VJ_X] * quat.vec[VJ_X] +
                            quat.vec[VJ_Y] * quat.vec[VJ_Y] +
                            quat.vec[VJ_Z] * quat.vec[VJ_Z] );

   // avoid divide by 0
   if (quat.vec[VJ_W] != 0.0)
      length = Math::atan( length / quat.vec[VJ_W] );
   else
      length = VJ_PI / 2.0f; // or VJ_PI_2...

   vec[VJ_W] = 0.0f;
   vec[VJ_X] = quat.vec[VJ_X] * length;
   vec[VJ_Y] = quat.vec[VJ_Y] * length;
   vec[VJ_Z] = quat.vec[VJ_Z] * length;
}

//: output method
std::ostream& Quat::outStreamRaw( std::ostream& out ) const
{
   //out << "w: " << vec[VJ_W] << "  x: " << vec[VJ_X] << "  y: " << vec[VJ_Y] << "  z: " << vec[VJ_Z];
   out << vec[VJ_W] << ", " << vec[VJ_X] << ", " << vec[VJ_Y] << ", " << vec[VJ_Z];
   return out;
}

//: output method
std::ostream& Quat::outStreamReadable( std::ostream& out ) const
{
   float rad;
   float x, y, z;
   this->getRot( rad, x, y, z );

   out << Math::rad2deg( rad ) << " deg, " << x << ", " << y << ", " << z;
   return out;
}

//: output operator
VJ_IMPLEMENT(std::ostream&) operator<<( std::ostream& out, const Quat& q )
{
   q.outStreamReadable( out );
   //q.outStreamRaw( out );
   return out;
}

};
