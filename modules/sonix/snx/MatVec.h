
/****************** <SNX heading BEGIN do not edit this line> *****************
 *
 * sonix
 *
 * Original Authors:
 *   Kevin Meinert, Carolina Cruz-Neira
 *
 * -----------------------------------------------------------------
 * File:          $RCSfile$
 * Date modified: $Date$
 * Version:       $Revision$
 * -----------------------------------------------------------------
 *
 ****************** <SNX heading END do not edit this line> ******************/
/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998, 1999, 2000, 2001 by Iowa State University
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
 *************** <auto-copyright.pl END do not edit this line> ***************/




#ifndef SNX_MAT_VEC_FUNCS
#define SNX_MAT_VEC_FUNCS

#include "snx/Math.h"
#include "snx/Matrix44.h"
#include "snx/Vec3.h"

namespace snx
{

// result[4] = mat * vec[4]
inline void xform( float* result, const snx::Matrix44& mat, const float* vec )
{
   for (int j = 0; j < 4; ++j)
	   for (int k = 0; k < 4; ++k)
		   result[j] += vec[k] * mat( k, j );
}

inline snx::Vec3 xformFull( const snx::Matrix44& mat, const snx::Vec3& vec )
{
   float vec4[4] = { vec[0], vec[1], vec[2], 1.0f };
   float result[4];
   xform( result, mat, vec4 );
   float w_inv = 1.0f / result[3];
   return snx::Vec3( result[0] * w_inv, result[1] * w_inv, result[2] * w_inv );
}

inline snx::Vec3 xformVec( const snx::Matrix44& mat, const snx::Vec3& vec )
{
   float vec4[4] = { vec[0], vec[1], vec[2], 0.0f };
   float result[4];
   xform( result, mat, vec4 );
   return snx::Vec3( result[0], result[1], result[2] );
}

inline snx::Vec3 operator+( const snx::Vec3& vec1, const snx::Vec3& vec2 )
{
   snx::Vec3 v( vec1[0] + vec2[0],
             vec1[1] + vec2[1],
             vec1[2] + vec2[2] );
   return v;
}

inline snx::Vec3 operator-( const snx::Vec3& vec1, const snx::Vec3& vec2 )
{
   snx::Vec3 v( vec1[0] - vec2[0],
             vec1[1] - vec2[1],
             vec1[2] - vec2[2] );
   return v;
}
inline snx::Vec3 operator*( const snx::Vec3& vec, const float& scalar )
{
   snx::Vec3 v( vec[0] * scalar,
             vec[1] * scalar,
             vec[2] * scalar );
   return v;
}

// Linear Interpolation between two vectors.
inline void Lerp(const snx::Vec3& from, 
      const snx::Vec3& to, 
      const float &lerp, 
      snx::Vec3& lerpedValue )
{
    snx::Vec3 offset = to - from;
    lerpedValue = from + offset*lerp;
}

};

#endif
