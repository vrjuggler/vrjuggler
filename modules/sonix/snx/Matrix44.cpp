
/****************** <AJ heading BEGIN do not edit this line> *****************
 *
 * Audio Juggler
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
 ****************** <AJ heading END do not edit this line> ******************/
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





#include "aj/Math.h"
#include "aj/Matrix44.h"

namespace aj
{

//: Default Constructor
//  NOTE: does no initialization, call identity() to init to identity matrix
Matrix44::Matrix44()
{
   this->makeIdent();
}

//: set the matrix with 16 floats
void Matrix44::set( float a0, float a4, float a8,  float a12,
       float a1, float a5, float a9,  float a13,
       float a2, float a6, float a10, float a14,
       float a3, float a7, float a11, float a15 )
{
   mData[0] = a0; mData[4] = a4; mData[8]  = a8;  mData[12] = a12;
   mData[1] = a1; mData[5] = a5; mData[9]  = a9;  mData[13] = a13;
   mData[2] = a2; mData[6] = a6; mData[10] = a10; mData[14] = a14;
   mData[3] = a3; mData[7] = a7; mData[11] = a11; mData[15] = a15;
}

//: set the matrix with a pointer to 16 floats
void Matrix44::set( const float* m )
{
   this->set( m[0], m[4], m[8],  m[12],
              m[1], m[5], m[9],  m[13],
              m[2], m[6], m[10], m[14],
              m[3], m[7], m[11], m[15] );
}

void Matrix44::copy( const Matrix44& mat )
{
   this->set( mat.mData );
}

//: this = M
Matrix44& Matrix44::operator=( const Matrix44& mat )
{
   this->copy( mat );
   return *this;
}

//: Copy constructor
Matrix44::Matrix44( const Matrix44& mat )
{
   this->copy( mat );
}

//: Construct 4x4 matrix from 16 floats
Matrix44::Matrix44( float a0, float a4, float a8,  float a12,
       float a1, float a5, float a9,  float a13,
       float a2, float a6, float a10, float a14,
       float a3, float a7, float a11, float a15 )
{
   this->set( a0, a4, a8,  a12,
              a1, a5, a9,  a13,
              a2, a6, a10, a14,
              a3, a7, a11, a15 );
}

//: get the inverse of this matrix
bool Matrix44::invertFull( const Matrix44& mat )
{
   Matrix44  a = mat;   // source
   Matrix44&  b = *this; // dest
   
   int   n = 4;
   int    i, j, k;
      int    r[ 4], c[ 4], row[ 4], col[ 4];
      float  m[ 4][ 4*2], pivot, maxmData, tmpmData, fac;

   
      // Initialization
      for (i = 0; i < n; i ++ )
      {
            r[ i] = c[ i] = 0;
            row[ i] = col[ i] = 0;
      }

      // Set working matrix
      for (i = 0; i < n; i++ )
      {
            for (j = 0; j < n; j++ )
            {
                  m[ i][ j] = a[ i * n + j];
                  m[ i][ j + n] =( i == j ) ? 1.0f : 0.0f ;
            }
      }

      // Begin of loop
      for (k = 0; k < n; k++ )
      {
            // Choosing the pivot
            for (i = 0, maxmData = 0; i < n; i++ )
            {
                  if (row[ i]  )     continue;
                  for (j = 0; j < n; j++ )
                  {
                        if (col[ j] )        continue;
                        tmpmData = ajMath::abs( m[ i][ j]);
                        if (tmpmData > maxmData)
                        {
                              maxmData = tmpmData;
                              r[ k] = i;
                              c[ k] = j;
                        }
                  }
            }
            row[ r[k] ] = col[ c[k] ] = 1;
            pivot = m[ r[ k] ][ c[ k] ];


            if (ajMath::abs( pivot) <= 1e-20)
            {
                  //cerr << "*** pivot = %f in mat_inv. ***\n";
                  //exit( 0);
                  return false;
            }

            // Normalization
            for (j = 0; j < 2*n; j++ )
            {
                  if (j == c[ k] )
                        m[ r[ k]][ j] = 1.0;
                  else
                        m[ r[ k]][ j] /= pivot;
            }

            // Reduction
            for (i = 0; i < n; i++ )
            {
                  if (i == r[ k] )
                        continue;

                  for (j=0, fac = m[ i][ c[k]]; j < 2*n; j++ )
                  {
                        if (j == c[ k] )
                              m[ i][ j] = 0.0;
                        else
                              m[ i][ j] -= fac * m[ r[k]][ j];
                  }
            }
      }

      // Assign inverse to a matrix
      for (i = 0; i < n; i++ )
            for (j = 0; j < n; j++ )
                  row[ i] =( c[ j] == i ) ? r[ j] : row[ i];

      for (i = 0; i < n; i++ )
            for (j = 0; j < n; j++ )
                  b[ i * n +  j] = m[ row[ i]][ j + n];
       
   return true;   // It worked   
}

// takes angles in radians..
void Matrix44::getEulerXYZ( float& xRot, float& yRot, float& zRot ) const
{
   float cz;

   zRot = ajMath::atan2(-mData[4], mData[0]);     // -(-cy*sz)/(cy*cz) - cy falls out
   xRot = ajMath::atan2(-mData[9], mData[10]);     // -(sx*cy)/(cx*cy) - cy falls out
   cz   = ajMath::cos( zRot );
   yRot = ajMath::atan2(mData[8], mData[0]/cz);   // (sy)/((cy*cz)/cz)
}   

// takes angles in radians..
void Matrix44::getEulerZYX(  float& zRot, float& yRot, float& xRot ) const
{
   float sx;

   zRot = ajMath::atan2( mData[1], mData[0]);      // (cy*sz)/(cy*cz) - cy falls out
   xRot = ajMath::atan2( mData[6], mData[10]);      // (sx*cy)/(cx*cy) - cy falls out
   sx   = ajMath::sin( xRot );
   yRot = ajMath::atan2( -mData[2],(mData[6]/sx) );   // -(-sy)/((sx*cy)/sx)
}  

// takes angles in radians..
void Matrix44::makeEulerXYZ( const float& xRot, const float& yRot, const float& zRot )
{
   float sx = ajMath::sin( xRot );  float cx = ajMath::cos( xRot );
   float sy = ajMath::sin( yRot );  float cy = ajMath::cos( yRot );
   float sz = ajMath::sin( zRot );  float cz = ajMath::cos( zRot );

   // Derived by multiplying the matrices by hand
   // X*Y*Z
   mData[0] = cy*cz;             mData[4] = -cy*sz;            mData[8]  = sy;     mData[12] = 0.0f;
   mData[1] = sx*sy*cz + cx*sz;  mData[5] = -sx*sy*sz + cx*cz; mData[9]  = -sx*cy; mData[13] = 0.0f;
   mData[2] = -cx*sy*cz + sx*sz; mData[6] = cx*sy*sz + sx*cz;  mData[10] = cx*cy;  mData[14] = 0.0f;
   mData[3] = 0.0f;              mData[7] = 0.0f;              mData[11] = 0.0f;   mData[15] = 1.0f;
}   

// takes angles in radians..
void Matrix44::makeEulerZYX( const float& zRot, const float& yRot, const float& xRot )
{
   float sz = ajMath::sin( zRot);  float cz = ajMath::cos( zRot );
   float sy = ajMath::sin( yRot);  float cy = ajMath::cos( yRot );
   float sx = ajMath::sin( xRot);  float cx = ajMath::cos( xRot );

   // Derived by multiplying the matrices by hand
   // Z*Y*Z
   mData[0] = cy*cz; mData[4] = -cx*sz + sx*sy*cz; mData[8]  = sx*sz + cx*sy*cz;  mData[12] = 0.0f;
   mData[1] = cy*sz; mData[5] = cx*cz + sx*sy*sz;  mData[9]  = -sx*cz + cx*sy*sz; mData[13] = 0.0f;
   mData[2] = -sy;   mData[6] = sx*cy;             mData[10] = cx*cy;             mData[14] = 0.0f;
   mData[3] = 0.0f;  mData[7] = 0.0f;              mData[11] = 0.0f;              mData[15] = 1.0f;
}   

//: get the translation component of this matrix
void Matrix44::getTrans( float& tx, float& ty, float& tz ) const
{
   tx = mData[12];
   ty = mData[13];
   tz = mData[14];
}

//: make matrix an identity matrix
void Matrix44::makeIdent()
{
   mData[0] = 1.0f; mData[4] = 0.0f; mData[8]  = 0.0f; mData[12] = 0.0f;
   mData[1] = 0.0f; mData[5] = 1.0f; mData[9]  = 0.0f; mData[13] = 0.0f;
   mData[2] = 0.0f; mData[6] = 0.0f; mData[10] = 1.0f; mData[14] = 0.0f;
   mData[3] = 0.0f; mData[7] = 0.0f; mData[11] = 0.0f; mData[15] = 1.0f;
}

//: c = a * b
// required: c, a, and b must each point to 16 floats
// WARNING: This function is dangerous since you could pass a bad pointer, use at your own risk
void Matrix44::mult( const float* a, const float* b )
{
   float* c = this->mData;
   c[0]  = a[0] * b[0]  + a[4] * b[1]  + a[8]  * b[2]  + a[12] * b[3];
   c[1]  = a[1] * b[0]  + a[5] * b[1]  + a[9]  * b[2]  + a[13] * b[3];
   c[2]  = a[2] * b[0]  + a[6] * b[1]  + a[10] * b[2]  + a[14] * b[3];
   c[3]  = a[3] * b[0]  + a[7] * b[1]  + a[11] * b[2]  + a[15] * b[3];

   c[4]  = a[0] * b[4]  + a[4] * b[5]  + a[8]  * b[6]  + a[12] * b[7];
   c[5]  = a[1] * b[4]  + a[5] * b[5]  + a[9]  * b[6]  + a[13] * b[7];
   c[6]  = a[2] * b[4]  + a[6] * b[5]  + a[10] * b[6]  + a[14] * b[7];
   c[7]  = a[3] * b[4]  + a[7] * b[5]  + a[11] * b[6]  + a[15] * b[7];

   c[8]  = a[0] * b[8]  + a[4] * b[9]  + a[8]  * b[10] + a[12] * b[11];
   c[9]  = a[1] * b[8]  + a[5] * b[9]  + a[9]  * b[10] + a[13] * b[11];
   c[10] = a[2] * b[8]  + a[6] * b[9]  + a[10] * b[10] + a[14] * b[11];
   c[11] = a[3] * b[8]  + a[7] * b[9]  + a[11] * b[10] + a[15] * b[11];

   c[12] = a[0] * b[12] + a[4] * b[13] + a[8]  * b[14] + a[12] * b[15];
   c[13] = a[1] * b[12] + a[5] * b[13] + a[9]  * b[14] + a[13] * b[15];
   c[14] = a[2] * b[12] + a[6] * b[13] + a[10] * b[14] + a[14] * b[15];
   c[15] = a[3] * b[12] + a[7] * b[13] + a[11] * b[14] + a[15] * b[15];
}

//: c = a * b
void Matrix44::mult( const Matrix44& a, const Matrix44& b )
{
   this->mult( a.data(), b.data() );
}

//: this = M * this
void Matrix44::preMult( const Matrix44& mat )
{
   Matrix44 result;
   result.mult( mat, *this );
   this->copy( result );
}

//: this = this * M
void Matrix44::postMult( const Matrix44& mat )
{
   Matrix44 result;
   result.mult( *this, mat );
   this->copy( result );
}


//: set the twist about an arbitrary axis.
// NOTE: this erases any translation in this matrix
void Matrix44::makeRot( const float& rad, const float& x, const float& y, const float& z )
{
   float cosine = ajMath::cos( rad );
   float sine = ajMath::sin( rad );
   float one_minus_cosine = 1 - cosine;

   // rotation part   
   mData[0] = x * x +( 1.0f - x*x ) * cosine;
   mData[1] = x * y * one_minus_cosine + z * sine;
   mData[2] = x * z * one_minus_cosine - y * sine;
   
   mData[4] = y * x * one_minus_cosine - z * sine;
   mData[5] = y * y +( 1.0f - y*y ) * cosine;
   mData[6] = y * z * one_minus_cosine + x * sine;
   
   mData[8] = z * x * one_minus_cosine + y * sine;
   mData[9] = z * y * one_minus_cosine - x * sine;
   mData[10] = z * z +( 1.0f - z*z ) * cosine;
   
   // translation part
   mData[12] = 0;
   mData[13] = 0;
   mData[14] = 0;

   // bottom row
   mData[3] = 0;
   mData[7] = 0;
   mData[11] = 0;
   mData[15] = 1;
}

void Matrix44::setTrans( float tx, float ty, float tz )
{
   mData[12] = tx;
   mData[13] = ty;
   mData[14] = tz;
}

void Matrix44::makeTrans( float tx, float ty, float tz )
{
   this->makeIdent();
   this->setTrans( tx, ty, tz );
}



/*
//: make this matrix equal to it's transpose
void Matrix44::transpose()
{
   // Make a transposed copy(don't do the diagonals).

   float a[16];
                        a[4]  = mData[1];   a[8]  = mData[2];   a[12] = mData[3];
   a[1]  = mData[4];                         a[9]  = mData[6];   a[13] = mData[7];
   a[2]  = mData[8];    a[6]  = mData[9];                         a[14] = mData[11];
   a[3]  = mData[12];   a[7]  = mData[13];   a[11] = mData[14];

   // Copy back into this matrix.

                      mData[4] = a[4];   mData[8] = a[8];   mData[12] = a[12];
   mData[1] = a[1];                      mData[9] = a[9];   mData[13] = a[13];
   mData[2] = a[2];   mData[6] = a[6];                      mData[14] = a[14];
   mData[3] = a[3];   mData[7] = a[7];   mData[11] = a[11];
}

*/

      
}; // end namespace

