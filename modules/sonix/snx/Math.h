
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





#ifndef _AUDIOJUGGLER_VJ_MATH_H_
#define _AUDIOJUGGLER_VJ_MATH_H_

#include <math.h>

//: x-platform standard math routines.
namespace snxMath
{
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
   template <class dataType>
   inline static dataType deg2rad( const dataType& deg )
   {
      // x * (VJ_PI / 180.0)
      return deg * 0.01745329252f; 
   }
   
   //: convert a trigonometric value given in radians to degrees
   template <class dataType>
   inline static dataType rad2deg( const dataType& rad )
   {
      // x * (180.0 / VJ_PI)
      return rad * 57.2957795131f;
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
      return snxMath::Min( snxMath::Min( x, y ), z );
   }
   //: min returns the minimum of 4 values
   template <class dataType>
   inline static dataType Min( const dataType& w, const dataType& x, const dataType& y, const dataType& z ) 
   {
      return snxMath::Min( snxMath::Min( w, x ), snxMath::Min( y, z ) );
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
      return snxMath::Max( snxMath::Max( x, y ), z );
   }
   //: max returns the maximum of 4 values
   template <class dataType>
   inline static dataType Max( const dataType& w, const dataType& x, const dataType& y, const dataType& z ) 
   {
      return snxMath::Max( snxMath::Max( w, x ), snxMath::Max( y, z ) );
   }

   //: Linear Interpolation between number [a] and [b]
   // use double or float only...
   template <class dataType>
   inline static void lerp( dataType& result, const dataType& lerp, const dataType& a, 
                     const dataType& b )
   {
       dataType size = b - a;
       result = a + (size * lerp);
   }
   
   //: Is almost equal?
   // test for equality with tolerance...
   template <class dataType>
   inline static bool isEqual( const dataType& a, const dataType& b, const dataType& tolerance )
   {
      return snxMath::abs( a - b ) <= tolerance;
   }
};

#endif
