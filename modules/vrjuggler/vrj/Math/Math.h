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

#ifndef _VJ_MATH_H_
#define _VJ_MATH_H_

#include <vjConfig.h>
#include <math.h>

// some math.h do not have M_PI defined.
#ifndef M_PI
#define M_PI 3.14159265358979323846264338327950288419716939937510
#endif



/* Math constants */

//: PI
extern const float VJ_PI;

//: PI * 0.5
extern const float VJ_PI_2;

//: PI * 0.25
extern const float VJ_PI_4;

//: default epsilon used by some juggler math routines...
//extern const float VJ_EPSILON_DEFAULT;
#define VJ_EPSILON_DEFAULT 1e-6f

namespace vrj
{
   
//: x-platform standard math routines.
class VJ_CLASS_API Math 
{
public:
   //: sin returns the trigonometric sin of the radian argument x
   inline static double sin( const double x ) 
   {
     return ::sin( x );
   }

   //: sin returns the trigonometric sin of the radian argument x
   inline static float sin( const float x ) 
   {
#ifdef HAVE_SINF
      return ::sinf( x );
#else
      return (float) ::sin( x );
#endif
   }

   //: asin return the arc sine in the range -pi/2 to pi/2 
   inline static double asin( const double x ) 
   {
     return ::asin( x );
   }

   //: asin return the arc sine in the range -pi/2 to pi/2 
   inline static float asin( const float x ) 
   {
#ifdef HAVE_ASINF
      return ::asinf( x );
#else
      return (float) ::asin( x );
#endif
   }

   //: cos returns the trigonometric cosine of the radian argument x
   inline static double cos( const double x ) 
   {
      return ::cos( x );
   }

   //: cos returns the trigonometric cosine of the radian argument x
   inline static float cos( const float x ) 
   {
#ifdef HAVE_COSF
      return ::cosf( x );
#else
      return (float) ::cos( x );
#endif
   }

   //: arc cosine returns the arc cosine in the range 0 to pi
   inline static double acos( const double x )
   {
      return ::acos( x );
   }

   //: arc cosine returns the arc cosine in the range 0 to pi
   inline static float acos( const float x )
   {
#ifdef HAVE_ACOSF
      return ::acosf( x );
#else
      return (float) ::acos( x );
#endif
   }

   //: atan2 returns the arc tangent of y/x in the range of -PI to PI
   //  using the signs of both arguments to determine the quadrant of the
   //  return value
   inline static double atan2( const double x, const double y )
   {
     return ::atan2( x, y );
   }

   //: atan2 returns the arc tangent of y/x in the range of -PI to PI
   //  using the signs of both arguments to determine the quadrant of the
   //  return value
   inline static float atan2( const float x, const float y )
   {
#ifdef HAVE_ATAN2F
      return ::atan2f( x, y );
#else
      return (float) ::atan2( x, y );
#endif
   }

   //: atan returns the arc tangent of x in the range -pi/2 to pi/2
   inline static double atan( const double x ) 
   {
     return ::atan( x );
   }

   //: atan returns the arc tangent of x in the range -pi/2 to pi/2
   inline static float atan( const float x ) 
   {
#ifdef HAVE_ATAN2F
      return ::atanf( x );
#else
      return (float) ::atan( x );
#endif
   }

   //: tan returns the trigonometric tangent of the radian argument x
   inline static double tan( const double x ) 
   {
     return ::tan( x );
   }

   //: tan returns the trigonometric tangent of the radian argument x
   inline static float tan( const float x ) 
   {
#ifdef HAVE_ATAN2F
      return ::tanf( x );
#else
      return (float) ::tan( x );
#endif
   }

   //: sqrt returns the square root of the number x
   inline static double sqrt( const double x )
   {
      return ::sqrt( x );
   }

   //: sqrt returns the square root of the number x
   inline static float sqrt( const float x )
   {
#ifdef HAVE_SQRTF
      return ::sqrtf( x );
#else
      return (float) ::sqrt( x );
#endif
   }

   //: abs returns the absolute value of the number x
   inline static double abs( const double x )
   {
      return ::fabs( x );
   }

   //: abs returns the absolute value of the number x
   inline static float abs( const float x )
   {
#ifdef HAVE_FABSF
      return ::fabsf( x );
#else
      return (float) ::fabs( x );
#endif
   }

   //: convert a trigonometric value given in degrees to radians
   inline static float deg2rad( const float& deg )
   {
      // x * (VJ_PI / 180.0)
      return deg * 0.01745329252f; 
   }
   inline static double deg2rad( const double& deg )
   {
      // x * (VJ_PI / 180.0)
      return deg * 0.01745329252; 
   }
   
   //: convert a trigonometric value given in radians to degrees
   inline static float rad2deg( const float& rad )
   {
      // x * (180.0 / VJ_PI)
      return rad * 57.2957795131f;
   }
   inline static double rad2deg( const double& rad )
   {
      // x * (180.0 / VJ_PI)
      return rad * 57.2957795131;
   }
   
   //: min returns the minimum of 2 values
   template <class dataType>
   inline static dataType Min( const dataType& x, const dataType& y ) 
   {
      return ( x > y ) ? y : x;
   }
   
   //: min returns the minimum of 3 values
   template <class dataType>
   inline static dataType Min( const dataType& x, const dataType& y, const dataType& z ) 
   {
      return Math::Min( Math::Min( x, y ), z );
   }
   //: min returns the minimum of 4 values
   template <class dataType>
   inline static dataType Min( const dataType& w, const dataType& x, const dataType& y, const dataType& z ) 
   {
      return Math::Min( Math::Min( w, x ), Math::Min( y, z ) );
   }
   
   //: max returns the maximum of 2 values
   template <class dataType>
   inline static dataType Max( const dataType& x, const dataType& y ) 
   {
      return ( x > y ) ? x : y;
   }
   
   //: max returns the maximum of 3 values
   template <class dataType>
   inline static dataType Max( const dataType& x, const dataType& y, const dataType& z ) 
   {
      return Math::Max( Math::Max( x, y ), z );
   }
   //: max returns the maximum of 4 values
   template <class dataType>
   inline static dataType Max( const dataType& w, const dataType& x, const dataType& y, const dataType& z ) 
   {
      return Math::Max( Math::Max( w, x ), Math::Max( y, z ) );
   }

// non-standard but useful math funcs
public:
   //: Linear Interpolation between number [a] and [b]
   // use double or float only...
   template <class dataType>
   inline static void lerp( dataType& result, const dataType& lerp, const dataType& a, 
                     const dataType& b )
   {
       dataType size = b - a;
       result = a + (size * lerp);
   }
   
   // round to nearest integer (32bit)
   inline static float round( float p )
   {
      p += 0.5f;
      return floor( p );
   }
   // round to nearest integer (64bit)
   inline static double round( double p )
   {
      p += 0.5f;
      return floor( p );
   }
   
   //: Is almost equal?
   // test for equality with tolerance...
   template <class dataType>
   inline static bool isEqual( const dataType& a, const dataType& b, const dataType& tolerance )
   {
      return Math::abs( a - b ) <= tolerance;
   }
   
   //: return a random number between 0.0f and 1.0f
   //! RETURNS: random number between 0 and 1
   inline float random()
   {
      // ::rand() returns int from  0 - RAND_MAX
      const float rand_max = RAND_MAX;
      float r = static_cast<float>( ::rand() );
      r /= rand_max;
      return r;
   }

   //: return a random number between x1 and x2
   //! RETURNS: random number between x1 and x2
   inline float random( float x1, float x2 )
   {
      float r = Math::random();
      float size = x2 - x1;
      return r * size + x1;
   }
   
// clampy clampy
public:
   // clamp a number to zero if lower than epsilon
   // usage: zero_clamp( number )       /* use default VJ_EPSILON_DEFAULT for epsilon */
   //        zero_clamp( number, eps )  /* use eps for epsilon */
   template <class dataType>
   inline static dataType zero_clamp( const dataType& number, const dataType& epsilon = VJ_EPSILON_DEFAULT ) 
   {
      return ( Math::abs( number ) < epsilon ) ? 0.0f : number;
   }
   // is a number close to zero? (within specified epsilon)
   // usage: isZero( number )       /* use default VJ_EPSILON_DEFAULT for epsilon */
   //        isZero( number, eps )  /* use eps for epsilon */
   template <class dataType>
   inline static bool isZero( const dataType& number, const dataType& epsilon = VJ_EPSILON_DEFAULT ) 
   {
      return Math::abs( number ) < epsilon;
   }

   //: clamp a number to some value
   //  returns value when (number > value) is true
   template <class dataType>
   inline static dataType clamp( const dataType& number, const dataType& value ) 
   {
      return ( x > y ) ? y : x;
   }
};

};
#endif
