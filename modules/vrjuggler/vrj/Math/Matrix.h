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

#ifndef _VRJ_MATRIX_H_
#define _VRJ_MATRIX_H_
//#pragma once

#include <vrj/vjConfig.h>
#include <math.h>
#include <string.h>
#include <vrj/Math/Math.h>


namespace vrj
{
   
class Quat;
class Vec3;
class Coord;


//-----------------------------------------------------------------------------
//: Matrix: 4x4 Matrix class (OpenGL ordering)
//
// C/C++ uses matrices in row major order.  In other words the access
// indices look like: <br>
// (0,0) (0,1) (0,2) (0,3)   <=== Array      <br>
// (1,0) (1,1) (1,2) (1,3)   <=== Array      <br>
// (2,0) (2,1) (2,2) (2,3)   <=== Array      <br>
// (3,0) (3,1) (3,2) (3,3)   <=== Array      <br>
// <br>
// OpenGL ordering specifies that the matrix has to be column major in memory,
// so we need to access it more like: <br>
//    NOTE: The given indexes are what the cells have to be called in C/C++
//          notation.  Since we are putting the columns into memory
//          back-to-back.        <br>
// (0,0) (1,0) (2,0) (3,0)       <br>
// (0,1) (1,1) (2,1) (3,1)       <br>
// (0,2) (1,2) (2,2) (3,2)       <br>
// (0,3) (1,3) (2,3) (3,3)       <br>
//   ^     ^     ^     ^         <br>
//   ====================== Arrays     <br>
//
//-----------------------------------------------------------------------------
//!PUBLIC_API:
class VJ_CLASS_API Matrix
{
public:
   // Column major.  In other words {Column1, Column2, Column3, Column4} in memory
   // Access element mat[column][row]
   float mat[4][4];

public:
   //: Default Constructor (Identity constructor)
   Matrix() : mat() { makeIdent(); };

   //: Constructor: Specify each element
   Matrix(float a00, float a01, float a02, float a03,
            float a10, float a11, float a12, float a13,
            float a20, float a21, float a22, float a23,
            float a30, float a31, float a32, float a33)
   {
      mat[0][0] = a00; mat[1][0] = a01; mat[2][0] = a02; mat[3][0] = a03;
      mat[0][1] = a10; mat[1][1] = a11; mat[2][1] = a12; mat[3][1] = a13;
      mat[0][2] = a10; mat[1][2] = a21; mat[2][2] = a22; mat[3][2] = a23;
      mat[0][3] = a30; mat[1][3] = a31; mat[2][3] = a32; mat[3][3] = a33;
   }

   //: Build matrix from a Coord object
   //! POST: mat = coord represented as a matrix
   Matrix(Coord coord);

   //: copy constructor
   //! POST: *this = matrix
   Matrix( const Matrix& matrix )
   {
      this->copy( matrix );
   }

public:
   //: Set from a memory region
   //! PRE: m refers to an array of 16 floats
   //! POST: this' = *m
   void    set( const float* const m ) {
      memcpy(mat, m, sizeof(float) * 16);
   }

   //: Set matrix elementwise
   //!POST: this' is set to pass args
   //!ARGS: a00 - Upper left
   //!ARGS: a33 - Lower left
   void set(float a00, float a01, float a02, float a03,
            float a10, float a11, float a12, float a13,
            float a20, float a21, float a22, float a23,
            float a30, float a31, float a32, float a33)
   {
      mat[0][0] = a00; mat[1][0] = a01; mat[2][0] = a02; mat[3][0] = a03;
      mat[0][1] = a10; mat[1][1] = a11; mat[2][1] = a12; mat[3][1] = a13;
      mat[0][2] = a20; mat[1][2] = a21; mat[2][2] = a22; mat[3][2] = a23;
      mat[0][3] = a30; mat[1][3] = a31; mat[2][3] = a32; mat[3][3] = a33;
   }

public:
   //: Create Identity Matrix
   //! POST: this' is identity
   void makeIdent()
   { zero(); mat[0][0] = mat[1][1] = mat[2][2] = mat[3][3] = 1.0f;}

   //: Zero out the matrix
   //! POST: this' is all zeros
   void zero()
   {
      mat[0][0]=0.0f; mat[1][0]=0.0f; mat[2][0]=0.0f; mat[3][0]=0.0f;
      mat[0][1]=0.0f; mat[1][1]=0.0f; mat[2][1]=0.0f; mat[3][1]=0.0f;
      mat[0][2]=0.0f; mat[1][2]=0.0f; mat[2][2]=0.0f; mat[3][2]=0.0f;
      mat[0][3]=0.0f; mat[1][3]=0.0f; mat[2][3]=0.0f; mat[3][3]=0.0f;
   }

   //: Make XYZEuler matrix
   // args are in degrees
   //!ARGS: rotX - Rotation about the x-axis
   //!ARGS: rotY - Rotation about the y-axis
   //!ARGS: rotZ - Rotation about the z-axis
   //
   //!POST: mat = rX*rY*rZ
   void makeXYZEuler(float xRot, float yRot, float zRot);
   void getXYZEuler(float& xRot, float& yRot, float& zRot) const;

   //: Make ZYX Euler matrix
   // args are in degrees
   //!POST: mat = rZ*rY*rX
   //!ARGS: rotZ - Rotation about the z-axis
   //!ARGS: rotY - Rotation about the y-axis
   //!ARGS: rotX - Rotation about the x-axis
   void makeZYXEuler(float zRot, float yRot, float xRot);
   void getZYXEuler(float& zRot, float& yRot, float& xRot) const;

   //!POST: mat = rZ*rX*rY
   // args are in degrees
   void makeZXYEuler( float zRot, float xRot, float yRot );
   void getZXYEuler( float& zRot, float& xRot, float& yRot ) const;

   //: extract the yaw information from the matrix
   //!POST:  returned value is from -180 to 180, where 0 is none
   float    getYRot() const;
   float    getYaw() const
   { return getYRot(); }

   //: extract the pitch information from the matrix
   //!POST:  returned value is from -180 to 180, where 0 none
   float    getXRot() const;
   float    getPitch() const
   {return getXRot();}

   //: extract the roll information from the matrix
   //!POST:  returned value is from -180 to 180, where 0 is no roll
   float    getZRot() const;
   float    getRoll() const
   { return getZRot();}

   //: constrain the matrix rotation to a certain axis or axes
   //  Uses getTrans, getYaw, getPitch, getRoll to recreate the matrix.<BR><BR>
   //
   //!ARGS:  allowXRot = true to keep the x rotation info<BR>
   //+       allowYRot = true to keep the y rotation info<BR>
   //+       allowZRot = true to keep the z rotation info<BR>
   //!POST:  result returned is a constrained matrix.
   void     constrainRotAxis( const bool& allowXRot, const bool& allowYRot, const bool& allowZRot, Matrix& result );
   void     _kevn_constrainRotAxis( const bool& pitch, const bool& yaw, const bool& roll, Matrix& result );



   //: Make a matrix using direction cosines
   //
   //!ARGS: secXAxis - The x-axis of the secondary coord system in terms of the first
   //+      secYAxis - The y-axis of the secondary coord system in terms of the first
   //+      secZAxis - The z-axis of the secondary coord system in terms of the first
   //
   //!PRE: The axis must be normalized
   //!POST: mat = direction cosine matrix
   //!NOTE: Based on "Virtual Reality Sytems" pg. 26
   //+      The matrix created is able to transform a pt in the first coord
   //+      system to the second coord system. Ps = sMf*Pf
   void     makeDirCos( Vec3 secXAxis, Vec3 secYAxis, Vec3 secZAxis );

   //: Make matrix from given quaternion
   //!POST: mat = Matrix specified by Quaternion _quat
   //!POST: sets every cell, no need to call makeIdent before this.
   void     makeQuaternion( const float* const q );
   void     makeQuaternion( const Quat& q );


   //: Make rotation matrix around _axis
   //!ARGS: _degrees - Number of degrees to rotate
   //+      _axis - The axis to rotate around
   void makeRot(float _degrees, Vec3 _axis);

   //: Make translation matrix
   //!POST: mat = matrix with only the new translation
   void makeTrans(float _x, float _y, float _z);
   void makeTrans(const Vec3& trans);

   //: Sets given translation to current matrix
   //!POST: mat = old(mat) with the tranlation portion set to the parameters
   void setTrans(float _x, float _y, float _z);
   void setTrans(const Vec3& trans);

   //: Get the translation portion of the matrix
   //!POST: _x, _y, and _z contain the translation portion of the matrix
   void getTrans(float& _x, float& _y, float& _z) const;
   Vec3 getTrans() const;

   //: Make scale matrix
   void makeScale(float _x, float _y, float _z);

//    void  makeVecRotVec(const Vec3&  _v1, const Vec3&  _v2)
// {;}
//    void  makeCoord(const sgCoord* _c)
// {;}

   //: Copy matrix
   //!POST: *this' = _m
   void copy( const Matrix& _m )
   {
      ((mat)[0][0] = (_m)[0][0]); ((mat)[0][1] = (_m)[0][1]); ((mat)[0][2] = (_m)[0][2]); ((mat)[0][3] = (_m)[0][3]);
      ((mat)[1][0] = (_m)[1][0]); ((mat)[1][1] = (_m)[1][1]); ((mat)[1][2] = (_m)[1][2]); ((mat)[1][3] = (_m)[1][3]);
      ((mat)[2][0] = (_m)[2][0]); ((mat)[2][1] = (_m)[2][1]); ((mat)[2][2] = (_m)[2][2]); ((mat)[2][3] = (_m)[2][3]);
      ((mat)[3][0] = (_m)[3][0]); ((mat)[3][1] = (_m)[3][1]); ((mat)[3][2] = (_m)[3][2]); ((mat)[3][3] = (_m)[3][3]);
   }

   //: Compare matrix for equality
   //!RETVAL: true - this == _m
   //!RETVAL: false - this != _m
   bool     equal(const Matrix&  _m) const  {
      return (((mat)[0][0] == (_m)[0][0]) &&
              ((mat)[0][1] == (_m)[0][1]) &&
              ((mat)[0][2] == (_m)[0][2]) &&
              ((mat)[0][3] == (_m)[0][3]) &&
              ((mat)[1][0] == (_m)[1][0]) &&
              ((mat)[1][1] == (_m)[1][1]) &&
              ((mat)[1][2] == (_m)[1][2]) &&
              ((mat)[1][3] == (_m)[1][3]) &&
              ((mat)[2][0] == (_m)[2][0]) &&
              ((mat)[2][1] == (_m)[2][1]) &&
              ((mat)[2][2] == (_m)[2][2]) &&
              ((mat)[2][3] == (_m)[2][3]) &&
              ((mat)[3][0] == (_m)[3][0]) &&
              ((mat)[3][1] == (_m)[3][1]) &&
              ((mat)[3][2] == (_m)[3][2]) &&
              ((mat)[3][3] == (_m)[3][3]));
   }

   //: Clamps the values of the matrix to zero
   //!POST: Any values in matrix < (some epsilon) are set to 0
   void zeroClamp()
   {
      // Clamp values near zero.  Just for looks.
      // Could be removed
       for(int i=0;i<4;i++)
          for(int j=0;j<4;j++)
             mat[i][j] = Math::zero_clamp(mat[i][j]);
   }

public:
   //: Set to the transpose of the matrix
   //!POST: mat = transpose(_m)
   void transpose(const Matrix&  _m)
   {
      for (int i=0; i<4; i++)
         for (int j=0; j<4; j++)
            mat[i][j] = _m.mat[j][i];
   }

   //: Multiple 2 matrices
   //!POST: mat = m1 * m2
   void mult(const Matrix& _m1, const Matrix &_m2)
   {
      *this = _m1;       //  Set equal to param 1
      postMult(_m2);     // Now post mult be the second param
   }

   //: Add 2 matrices
   //!POST: mat = m1 + m2
   void add(const Matrix& _m1, const Matrix &_m2)
   {
      for (int n=0;n<4;n++)
         for (int m=0;m<4;m++)
            mat[n][m] = (_m1.mat[n][m] + _m2.mat[n][m]);
   }

   //: Subtract a matrix
   //!POST: mat' = m1 - m2
   void sub(const Matrix& _m1, const Matrix &_m2)
   {
      for (int n=0;n<4;n++)
         for (int m=0;m<4;m++)
            mat[n][m] = (_m1.mat[n][m] - _m2.mat[n][m]);
   }

   //: Scale a matrix by a scalar
   //!POST: Each element of mat' = mat * _s
   //!NOTE: Not a 3D scale
   void scale(float _s, const Matrix &_m)
   {
      for (int n=0;n<4;n++)
         for (int m=0;m<4;m++)
            mat[n][m] = (_m.mat[n][m] * _s);
   }

   //: Post multiply by a matrix
   //!POST: mat' = mat * m
   void postMult( const Matrix& _m );

   //: Pre multiply by a matrix
   //!POST: mat' = m * mat
   void preMult( const Matrix& _m );

   //: Find inverse of a matrix
   //!POST: mat = inverse(_m)
   //! RETURNS: 1 - Success
   int invert( const Matrix& _m );

public:
   // --- Transformation functions --- //

   //: Pre translate a matrix
   //!POST: mat' = trans(_x,_y,_z) * _m
   void preTrans( float _x, float _y, float _z, const Matrix&  _m );

   //: Pre translate a matrix
   //!POST: mat' = trans(_x,_y,_z) * _m
   void preTrans( const Vec3& _trans, const Matrix&  _m );

   //: Post translate a matrix
   //!POST: mat' = _m * trans(_x,_y,_z)
   void postTrans( const Matrix&  _m, float _x, float _y, float _z );

   //: Post translate a matrix
   //!POST: mat' = _m * trans(_x,_y,_z)
   void postTrans( const Matrix&  _m, const Vec3& _trans );

   //: Pre rotate a matrix
   //!POST: mat' = rot(_degrees, axis) * _m
   void preRot( const float& _degrees, const Vec3& axis, const Matrix&  _m );

   //: Post rotate a matrix
   //!POST: mat' = _m * rot(_degrees, axis)
   void postRot( const Matrix&  _m, const float& _degrees, const Vec3& axis );

   void preXYZEuler( float x, float y, float z, const Matrix& _m );
   void postXYZEuler( const Matrix& _m, float x, float y, float z );

   //!POST: mat' = scale(_xs,_ys,_zs) * _m;
   void preScale( float _xs, float _ys, float _zs, const Matrix&  _m );

   //!POST: mat' = _m * scale(_xs,_ys,_zs)
   void postScale( const Matrix&  _m, float _xs, float _ys, float _zs );

public:
   //: Get a float pointer to the matrix data
   //!RETVAL: Returns a ptr to the head of the matrix data
   inline float*        getFloatPtr() { return (float*)mat;}
   inline const float*  getFloatPtr() const { return (float*)mat;}

   // Operators
   inline float*        operator[]( int i )        { return &mat[i][0];}
   inline const float*  operator[]( int i ) const  { return &mat[i][0];}
   inline float&        operator()( const int& row, const int& column ) { return mat[column][row];}
   inline const float&  operator()( const int& row, const int& column ) const { return mat[column][row];}

   int operator==( const Matrix&  _m ) const
   {
      return this->equal(_m);
   }

   int operator!=( const Matrix&  _m ) const
   {
      return !this->equal(_m);
   }

public:
   // Matrix operators (N.B. return by value can be quite slow)
   Matrix operator*( const Matrix&  _m ) const
   {
      Matrix dst; dst.mult(*this, _m); return dst;
   }

   //: addition
   Matrix operator+( const Matrix&  _m ) const
   {
      Matrix dst; dst.add(*this, _m); return dst;
   }

   //: subtraction
   Matrix operator-( const Matrix&  _m ) const
   {
      Matrix dst;
      dst.sub(*this, _m);
      return dst;
   }

   friend VJ_API(Matrix) operator*( float _s, const Matrix& );
   friend VJ_API(Matrix) operator*( const Matrix& _v, float _s );
   friend VJ_API(Matrix) operator/( const Matrix& _v, float _s );
   friend VJ_API(std::ostream&) operator<<( std::ostream& out,
                                            const Matrix& _mat );

public:
   // Assignment operators
   //!POST: *this' = _m
   inline Matrix&  operator=( const Matrix&  _m )
   {
      this->copy( _m );
      return *this;
   }

   Matrix&  operator*=( const Matrix&  _m )
   {
      this->postMult(_m); return *this;
   }

   Matrix&  operator*=( float _s )
   {
      for (int i=0; i<4; i++)
         for (int j=0; j<4; j++)
            mat[i][j] = mat[i][j] * _s;

      return *this;
   }

   //vjMatrix&  operator/=( float _s );
   //vjMatrix&  operator+=( const Matrix&  _m );
   //vjMatrix&  operator-=( const Matrix&  _m );
};

VJ_API(Matrix) operator*( float _s, const Matrix& );
VJ_API(Matrix) operator*( const Matrix& _v, float _s );
VJ_API(Matrix) operator/( const Matrix& _v, float _s );
VJ_API(std::ostream&) operator<<( std::ostream& out, const Matrix& _mat );

};

#endif
