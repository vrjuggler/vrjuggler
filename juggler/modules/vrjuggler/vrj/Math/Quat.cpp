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


#include <assert.h>
#include <Math/vjQuat.h>
#include <Math/vjMath.h>

// set self to the rotation in the given rotation matrix
void vjQuat::makeRot( const vjMatrix& mat )
{
   float tr, s;
   //static int nxt[3] = {VJ_Y, VJ_Z, VJ_X};

   tr = mat.mat[0][0] + mat.mat[1][1] + mat.mat[2][2];

   // Check the diagonal
   if (tr > 0.0)
   {
      s = vjMath::sqrt( tr + 1.0 );
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

      s = vjMath::sqrt( (mat.mat[i][i] - (mat.mat[j][j] + mat.mat[k][k])) + 1.0 );

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
void vjQuat::makeRot( const float& rad, const float& x, const float& y, const float& z )
{
   float halfRad = rad * 0.5f;
	float sinHalfRad = vjMath::sin( halfRad );
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
	vec[VJ_W] = vjMath::cos( halfRad );
	vec[VJ_X] = sinHalfRad * vecNormalized[0];
	vec[VJ_Y] = sinHalfRad * vecNormalized[1];
	vec[VJ_Z] = sinHalfRad * vecNormalized[2];

   // rotation quats are by definition 1 unit in magnitude.
	this->normalizeFast();
}

//: get the quat's twist (radians) and vector
void vjQuat::getRot( float& rad, float& xx, float& yy, float& zz ) const
{
   // make sure we don't get a NaN result from acos...
   vjQuat quat( *this );
   if (vjMath::abs( quat.vec[VJ_W] ) > 1.0f)
   {
      quat.normalize();
   }
   /*
   if (vjMath::abs( quat.vec[VJ_W] ) > 1.0f)
   {
      std::cerr<<" bad: "<<quat.vec[VJ_W]<<"\n"<<std::flush;
   }
   */
   assert( vjMath::abs( quat.vec[VJ_W] ) <= 1.0f && "acos returns NaN when |arg| > 1" );
   
   float halfrad = vjMath::acos( quat.vec[VJ_W] );
   float sin_halfrad = vjMath::sin( halfrad );
   
   // compute one/sin_halfrad
   if (sin_halfrad >= VJ_QUAT_EPSILON || sin_halfrad <= -VJ_QUAT_EPSILON)
   {
      float oneOverSinHalfRad = 1.0f / sin_halfrad;
      rad = halfrad * 2.0f;
      xx = quat.vec[VJ_X] * ( oneOverSinHalfRad );
      yy = quat.vec[VJ_Y] * ( oneOverSinHalfRad );
	   zz = quat.vec[VJ_Z] * ( oneOverSinHalfRad );
      return;
   }

   // avoid NAN
   // if rad == 0, then the vector is undefined anyway, 
   // avoid 0,0,0,0 when deg is 0.  make it 0,1,0,0
   else 
   {
      rad = 0.0f;
      xx = 1.0f;
      yy = 0.0f;
      zz = 0.0f;
      return;
   }
}

// Multiply the two quaternions
// From gdmag
void vjQuat::mult( const vjQuat& q1, const vjQuat& q2 )
{
   // Here is the easy to understand equation:
   // vector_component = q2.v * q1.s + q1.v * q2.s + cross(q1.v, q2.v)
   // scalar_component = q1.s * q2.s - dot(q1.v, q2.v)
	
   // Here is the same, only expanded...
   vjQuat temporary; 
   temporary[VJ_X] = q1[VJ_W]*q2[VJ_X] + q1[VJ_X]*q2[VJ_W] + q1[VJ_Y]*q2[VJ_Z] - q1[VJ_Z]*q2[VJ_Y];
   temporary[VJ_Y] = q1[VJ_W]*q2[VJ_Y] + q1[VJ_Y]*q2[VJ_W] + q1[VJ_Z]*q2[VJ_X] - q1[VJ_X]*q2[VJ_Z];
   temporary[VJ_Z] = q1[VJ_W]*q2[VJ_Z] + q1[VJ_Z]*q2[VJ_W] + q1[VJ_X]*q2[VJ_Y] - q1[VJ_Y]*q2[VJ_X];
   temporary[VJ_W] = q1[VJ_W]*q2[VJ_W] - q1[VJ_X]*q2[VJ_X] - q1[VJ_Y]*q2[VJ_Y] - q1[VJ_Z]*q2[VJ_Z];

   // use a temporary, in case q1 or q2 is the same as self.
   vec[VJ_X] = temporary[VJ_X];
   vec[VJ_Y] = temporary[VJ_Y];
   vec[VJ_Z] = temporary[VJ_Z];
   vec[VJ_W] = temporary[VJ_W];
   
   // Make sure self is a unit quaternion 
   // (TODO: only need unit quat for rotations, 
   //  are we sure that we should assume this behaviour in mult???)
   this->normalizeFast();
}

// From Adv Anim and Rendering Tech. Pg 364
void vjQuat::slerp(float t, const vjQuat& p, const vjQuat& q)
{
   vjQuat to;
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
      omega = vjMath::acos(cosom);
      sinom = vjMath::sin(omega);
      sclp  = vjMath::sin((1.0 - t)*omega)/sinom;
      sclq  = vjMath::sin(t*omega)/sinom;
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

void vjQuat::squad(float _t, const vjQuat& _q1, const vjQuat& _q2, const vjQuat& _a, const vjQuat& _b)
{ 
   assert( false && "not implemented" ); 
}

//: complex exponentiation 
//!POST: sets self to the exponentiation of quat
// NOTE: safe to pass self as argument
void vjQuat::exp( const vjQuat& quat )
{
   float len1, len2;

	len1 = vjMath::sqrt( quat.vec[VJ_X] * quat.vec[VJ_X] + 
                          quat.vec[VJ_Y] * quat.vec[VJ_Y] + 
                          quat.vec[VJ_Z] * quat.vec[VJ_Z] );
	if (len1 > 0.0f)
		len2 = vjMath::sin( len1 ) / len1;
	else
		len2 = 1.0f;

	vec[VJ_X] = quat.vec[VJ_X] * len2;
	vec[VJ_Y] = quat.vec[VJ_Y] * len2;
	vec[VJ_Z] = quat.vec[VJ_Z] * len2;
	vec[VJ_W] = vjMath::cos( len1 );
}

//: complex logarithm
//!POST: sets self to the log of quat
void vjQuat::log( const vjQuat& quat ) 
{
   float length;

	length = vjMath::sqrt( quat.vec[VJ_X] * quat.vec[VJ_X] + 
                            quat.vec[VJ_Y] * quat.vec[VJ_Y] + 
                            quat.vec[VJ_Z] * quat.vec[VJ_Z] );

	// avoid divide by 0
	if (quat.vec[VJ_W] != 0.0)
		length = vjMath::atan( length / quat.vec[VJ_W] );
	else 
      length = VJ_PI / 2.0f; // or VJ_PI_2...

	vec[VJ_W] = 0.0f;
	vec[VJ_X] = quat.vec[VJ_X] * length;
	vec[VJ_Y] = quat.vec[VJ_Y] * length;
	vec[VJ_Z] = quat.vec[VJ_Z] * length;
}

//: output method
std::ostream& vjQuat::outStreamRaw( std::ostream& out ) const
{
	out << "w: " << vec[VJ_W] << "  x: " << vec[VJ_X] << "  y: " << vec[VJ_Y] << "  z: " << vec[VJ_Z];
	return out;
}

//: output method
std::ostream& vjQuat::outStreamReadable( std::ostream& out ) const
{
	float rad;
	float x, y, z;
	this->getRot( rad, x, y, z );

	out << vjMath::rad2deg( rad ) << " deg, " << x << ", " << y << ", " << z;
	return out;
}

//: output operator
std::ostream& operator<<( std::ostream& out, const vjQuat& q )
{
	q.outStreamReadable( out );
	return out;
}
