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

#ifndef _VRJ_VEC4_H_
#define _VRJ_VEC4_H_
//#pragma once

#include <vrj/vjConfig.h>
#include <math.h>

#include <vrj/Math/Matrix.h>
#include <vrj/Math/Math.h>

// --- Define access reps --- //
#define VJ_X 0
#define VJ_Y 1
#define VJ_Z 2
#define VJ_W 3

namespace vrj
{
   class Matrix;

//: Vector of length 4
//!PUBLIC_API:
class VJ_CLASS_API Vec4
{
public:
   // Constructor
   Vec4(float _x, float _y, float _z, float _w)
   { set(_x, _y, _z, _w);}
   Vec4() { set(0.0f,0.0f,0.0f,0.0f);}

public:
   //: Set value of Vector
   void set(float _x, float _y, float _z, float _w) {
      vec[0] = _x;
      vec[1] = _y;
      vec[2] = _z;
      vec[3] = _w;
   }

public:
   //: Are we equal
   bool equal(const Vec4&  _v) const {
      return (vec[0] == _v[0] &&
               vec[1] == _v[1] &&
               vec[2] == _v[2] &&
               vec[3] == _v[3]);
   }

   float dot(const Vec4&  _v) const
   {
      return (vec[0] * _v[0] +
               vec[1] * _v[1] +
               vec[2] * _v[2] );
   }

   inline float length() const
   {
      return Math::sqrt((vec[0]*vec[0])+
                            (vec[1]*vec[1])+
                            (vec[2]*vec[2])+
                            (vec[3]*vec[3]));
   }

   inline void normalize()
   {
      float len = 1.0f / length();
      vec[0] *= len;
      vec[1] *= len;
      vec[2] *= len;
      vec[3] *= len;
   }

   //: Xform the vector
   // Set vec = (Mat)(Vec)
   void xform(const Matrix& _m, Vec4 _v);

public:
   /// Operators
   float&  operator [](int i) { return vec[i];}
   const float&  operator [](int i) const { return vec[i];}

   int operator ==(const Vec4& _v) const {
      return (vec[0] == _v[0] &&
              vec[1] == _v[1] &&
              vec[2] == _v[2] &&
              vec[3] == _v[3]);
   }

   int operator !=(const Vec4& _v) const {
      return !(*this == _v);
   }

public:
   // Vec4 operators, return by value could be slow
   Vec4 operator -() const {
      return Vec4(-vec[0], -vec[1], -vec[2], -vec[3]);
   }

   Vec4 operator +(const Vec4& _v) const {
      return Vec4(vec[0]+_v[0], vec[1]+_v[1], vec[2]+_v[2], vec[3]+_v[3]);
   }

   Vec4 operator -(const Vec4& _v) const {
      return Vec4(vec[0]-_v[0], vec[1]-_v[1], vec[2]-_v[2], vec[3]-_v[3]);
   }

   friend inline Vec4 operator *(float _s, const Vec4&);
   friend inline Vec4 operator *(const Vec4& _v, float _s);
   friend inline Vec4 operator /(const Vec4& _v, float _s);

public:
   //: Assignment Operators
   Vec4&  operator =(const Vec4& _v) {
      vec[0] = _v[0]; vec[1] = _v[1];
      vec[2] = _v[2]; vec[3] = _v[3];
      return *this;
   }

   Vec4& operator *=(float _s) {
      vec[0] *= _s; vec[1] *= _s;
      vec[2] *= _s; vec[3] *= _s;
      return *this;
   }

   Vec4& operator /=(float _s) {
      _s = 1.0/_s;
      return *this *= _s;
   }

   Vec4& operator +=(const Vec4& _v) {
      vec[0] += _v[0]; vec[1] += _v[1];
      vec[2] += _v[2]; vec[3] += _v[3];
      return *this;
   }

   Vec4& operator -=(const Vec4& _v) {
      vec[0] -= _v[0]; vec[1] -= _v[1];
      vec[2] -= _v[2]; vec[3] -= _v[3];
      return *this;
   }

public:
   float vec[4];        //: The vector data
};

inline Vec4 operator *(float _s, const Vec4& _v);
inline Vec4 operator *(const Vec4& _v, float _s);
inline Vec4 operator /(const Vec4& _v, float _s);
std::ostream& operator<<(std::ostream& out, Vec4& _v);

};
#endif
