/*
 *  File:          $RCSfile$
 *  Date modified: $Date$
 *  Version:       $Revision$
 *
 *
 *                                VR Juggler
 *                                    by
 *                              Allen Bierbaum
 *                             Christopher Just
 *                             Patrick Hartling
 *                            Carolina Cruz-Neira
 *                               Albert Baker
 *
 *                  Copyright (C) - 1997, 1998, 1999, 2000
 *              Iowa State University Research Foundation, Inc.
 *                            All Rights Reserved
 */


#ifndef _VJ_VEC3_
#define _VJ_VEC3_
//#pragma once

#include <vjConfig.h>
#include <math.h>

#include <Math/vjMatrix.h>
class vjMatrix;
#include <Math/vjVec4.h>

//----------------------------------------------------------------
//: Vector of length 3
// The class is a representation of a vector with 3 float values.
// It defines several operators that can be used on the vectors.
//-----------------------------------------------------------------
//!PUBLIC_API:
class vjVec3
{
public:
   // Constructor
   //! ARGS: x,y,z - the values of the vector
   vjVec3(float _x, float _y, float _z)
   { set(_x, _y, _z);}

   //: Default constructor
   vjVec3() {};

public:
   //: Set value of Vector
   void set(float _x, float _y, float _z) {
      vec[0] = _x;
      vec[1] = _y;
      vec[2] = _z;
   }

public:
   //: Are we equal
   //! RETURNS: true - Values in _v equal our values
   bool equal(const vjVec3&  _v) const {
      return (vec[0] == _v[0] &&
              vec[1] == _v[1] &&
              vec[2] == _v[2]);
   }

   //: Compute dot product
   //! RETURNS: Dot product of this and _v
   float dot(const vjVec3&  _v) const {
      return (vec[0] * _v[0] +
              vec[1] * _v[1] +
              vec[2] * _v[2]);
   }

   //: Normalize the vector
   void normalize()
   {
      float len = length();
      float len_inv = 1.0f/len;
      vec[0] *= len_inv;
      vec[1] *= len_inv;
      vec[2] *= len_inv;
   }

   //: Is the vector normalized
   bool isNormalized()
   {
      float len = lengthSquared();
      float diff = len-1.0f;
      return((diff < 0.0001f) && (diff > -0.0001f));
   }

   //: Return the length squared
   //! NOTE: Since this does not use sqrt it is faster
   //+ than length().  It can be used to optimize some
   //+ routines
   float lengthSquared() const
   {
      return ((vec[0]*vec[0])+
             (vec[1]*vec[1])+
             (vec[2]*vec[2]));
   }

   //: Return vector's length
   float length() const
   {
      return sqrtf(lengthSquared());
   }


   //: Calculate the cross product of me X _v
   const vjVec3 cross(const vjVec3&  _v)
   {
      return vjVec3( ((vec[1]*_v.vec[2]) - (vec[2]*_v.vec[1])),
                     ((vec[2]*_v.vec[0]) - (vec[0]*_v.vec[2])),
                     ((vec[0]*_v.vec[1]) - (vec[1]*_v.vec[0])));
   }

   //-----------------------------------------------------
   //: Transform the vec by the full 4x4 matrix.
   // vec = (Mat)(_v)
   // initial w = 1;  At end scale by W to convert back
   //-----------------------------------------------------
   void xformFull(const vjMatrix& _m, const vjVec3& _v);

   //-------------------------------------------------------
   //: Transform as vector.
   // w initial = 0.0  ==> Translation doesn't effect vector
   // No scale by 1/w at end.... why???
   //--------------------------------------------------------
   void xformVec(const vjMatrix& _m, const vjVec3& _v);

public:
   /// Operators
   float&  operator [](int i) { return vec[i];}
   const float&  operator [](int i) const { return vec[i];}

   int operator ==(const vjVec3& _v) const {
      return (vec[0] == _v[0] &&
              vec[1] == _v[1] &&
              vec[2] == _v[2]);
   }

   int operator !=(const vjVec3& _v) const {
      return !(*this == _v);
   }

public:
   // vjVec3 operators, return by value could be slow
   vjVec3 operator -() const {
      return vjVec3(-vec[0], -vec[1], -vec[2]);
   }

   vjVec3 operator +(const vjVec3& _v) const {
      return vjVec3(vec[0]+_v[0], vec[1]+_v[1], vec[2]+_v[2]);
   }

   vjVec3 operator -(const vjVec3& _v) const {
      return vjVec3(vec[0]-_v[0], vec[1]-_v[1], vec[2]-_v[2]);
   }

   friend vjVec3 operator *(float _s, const vjVec3&);
   friend vjVec3 operator *(const vjVec3& _v, float _s);
   friend vjVec3 operator /(const vjVec3& _v, float _s);
//    friend inline vjVec3 operator *(const vjVec3& _v, const vjMatrix& _m);

public:
   //: Assignment Operators
   vjVec3&  operator =(const vjVec3& _v) {
      vec[0] = _v[0];
      vec[1] = _v[1];
      vec[2] = _v[2];
      return *this;
   }

   //: Multiply all elements by a constant
   vjVec3& operator *=(float _s) {
      vec[0] *= _s;
      vec[1] *= _s;
      vec[2] *= _s;
      return *this;
   }

   //: Divide all elements by a constant
   vjVec3& operator /=(float _s) {
      _s = 1.0/_s;
      return *this *= _s;
   }

   vjVec3& operator +=(const vjVec3& _v) {
      vec[0] += _v[0];
      vec[1] += _v[1];
      vec[2] += _v[2];
      return *this;
   }

   vjVec3& operator -=(const vjVec3& _v) {
      vec[0] -= _v[0];
      vec[1] -= _v[1];
      vec[2] -= _v[2];
      return *this;
   }

public:
   float vec[3];
};


vjVec3 operator *(float _s, const vjVec3& _v);
vjVec3 operator *(const vjVec3& _v, float _s);
vjVec3 operator /(const vjVec3& _v, float _s);
ostream& operator<<(ostream& out, vjVec3& _v);

#endif
