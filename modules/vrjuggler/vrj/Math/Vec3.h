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


#ifndef _VJ_VEC3_
#define _VJ_VEC3_
//#pragma once

#include <vjConfig.h>
#include <math.h>
#include <Math/vjMath.h>

class vjMatrix;

//----------------------------------------------------------------
//: Vector of length 3
// The class is a representation of a vector with 3 float values.
// It defines several operators that can be used on the vectors.
//-----------------------------------------------------------------
//!PUBLIC_API:
class VJ_CLASS_API vjVec3
{
public:
   // Constructor
   //! ARGS: x,y,z - the values of the vector
   vjVec3(float _x, float _y, float _z)
   { set(_x, _y, _z);}

   //: Default constructor
   vjVec3() { set(0.0f,0.0f,0.0f); }

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
   //! RETURNS: Dot product of this and _v (returns 
   //! RETURNS:      cos of the angle [this] and [v] make)
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
   bool isNormalized() const
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
      return vjMath::sqrt(lengthSquared());
   }


   //: Calculate the cross product of me X _v
   vjVec3 cross(const vjVec3&  _v) const
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

   friend VJ_API(vjVec3) operator *(float _s, const vjVec3&);
   friend VJ_API(vjVec3) operator *(const vjVec3& _v, float _s);
   friend VJ_API(vjVec3) operator /(const vjVec3& _v, float _s);
//    friend VJ_API(vjVec3) operator *(const vjVec3& _v, const vjMatrix& _m);

   //: linear interpolate from vector a to vector b
   //!PRE:  n is a number between [0..1]
   //!POST: "this" is set to the interpolated vector
   void lerp( float n, const vjVec3& a, const vjVec3& b )
   {
      vjVec3 offset = b - a;
      (*this) = a + (offset * n);
   }   
public:
   //: Assignment Operators
   vjVec3&  operator =(const vjVec3& _v)
   {
      if(&_v == this)
         return *this;
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

   friend VJ_API(std::ostream&) operator<<( std::ostream& out,
                                            const vjVec3& _v );

public:
   float vec[3];
};


vjVec3 operator *(float _s, const vjVec3& _v);
vjVec3 operator *(const vjVec3& _v, float _s);
vjVec3 operator /(const vjVec3& _v, float _s);

#endif
