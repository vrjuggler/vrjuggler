/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * PyJuggler is (C) Copyright 2002, 2003 by Patrick Hartling
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

#ifndef _PYJUGGLER_GMTL_FUNCTIONS_H_
#define _PYJUGGLER_GMTL_FUNCTIONS_H_

#include <gmtl/Matrix.h>
#include <gmtl/MatrixOps.h>
#include <gmtl/Vec.h>
#include <gmtl/VecOps.h>

namespace gmtl
{
// gmtl::Matrix<> functions ===================================================
   template gmtl::Matrix44f& identity(gmtl::Matrix44f&);
   template gmtl::Matrix33f& identity(gmtl::Matrix33f&);

   template gmtl::Matrix44f& zero(gmtl::Matrix44f&);
   template gmtl::Matrix33f& zero(gmtl::Matrix33f&);

   template gmtl::Matrix44f& mult(gmtl::Matrix44f&, const gmtl::Matrix44f&,
                                  const gmtl::Matrix44f&);
   template gmtl::Matrix33f& mult(gmtl::Matrix33f&, const gmtl::Matrix33f&,
                                  const gmtl::Matrix33f&);

   template gmtl::Matrix44f& sub(gmtl::Matrix44f&, const gmtl::Matrix44f&,
                                 const gmtl::Matrix44f&);
   template gmtl::Matrix33f& sub(gmtl::Matrix33f&, const gmtl::Matrix33f&,
                                 const gmtl::Matrix33f&);

   template gmtl::Matrix44f& add(gmtl::Matrix44f&, const gmtl::Matrix44f&,
                                 const gmtl::Matrix44f&);
   template gmtl::Matrix33f& add(gmtl::Matrix33f&, const gmtl::Matrix33f&,
                                 const gmtl::Matrix33f&);

   template gmtl::Matrix44f& postMult(gmtl::Matrix44f&, const gmtl::Matrix44f&);
   template gmtl::Matrix33f& postMult(gmtl::Matrix33f&, const gmtl::Matrix33f&);

   template gmtl::Matrix44f& preMult(gmtl::Matrix44f&, const gmtl::Matrix44f&);
   template gmtl::Matrix33f& preMult(gmtl::Matrix33f&, const gmtl::Matrix33f&);

   template gmtl::Matrix44f& mult(gmtl::Matrix44f&, const gmtl::Matrix44f&,
                                  float);
   template gmtl::Matrix33f& mult(gmtl::Matrix33f&, const gmtl::Matrix33f&,
                                  float);

   template gmtl::Matrix44f& mult(gmtl::Matrix44f&, float);
   template gmtl::Matrix33f& mult(gmtl::Matrix33f&, float);

   template gmtl::Matrix44f& transpose(gmtl::Matrix44f&);
   template gmtl::Matrix33f& transpose(gmtl::Matrix33f&);

   template gmtl::Matrix44f& transpose(gmtl::Matrix44f&,
                                       const gmtl::Matrix44f&);
   template gmtl::Matrix33f& transpose(gmtl::Matrix33f&,
                                       const gmtl::Matrix33f&);

   template gmtl::Matrix44f& invertFull(gmtl::Matrix44f&,
                                        const gmtl::Matrix44f&);
   template gmtl::Matrix33f& invertFull(gmtl::Matrix33f&,
                                        const gmtl::Matrix33f&);

   template gmtl::Matrix44f& invert(gmtl::Matrix44f&, const gmtl::Matrix44f&);
   template gmtl::Matrix33f& invert(gmtl::Matrix33f&, const gmtl::Matrix33f&);

   template gmtl::Matrix44f& invert(gmtl::Matrix44f&);
   template gmtl::Matrix33f& invert(gmtl::Matrix33f&);

   template gmtl::Matrix44f operator*(const gmtl::Matrix44f&,
                                      const gmtl::Matrix44f&);
   template gmtl::Matrix33f operator*(const gmtl::Matrix33f&,
                                      const gmtl::Matrix33f&);

   template bool operator==(const gmtl::Matrix44f&, const gmtl::Matrix44f&);
   template bool operator==(const gmtl::Matrix33f&, const gmtl::Matrix33f&);

   template bool operator!=(const gmtl::Matrix44f&, const gmtl::Matrix44f&);
   template bool operator!=(const gmtl::Matrix33f&, const gmtl::Matrix33f&);

   template bool isEqual(const gmtl::Matrix44f&, const gmtl::Matrix44f&,
                         const float&);
   template bool isEqual(const gmtl::Matrix33f&, const gmtl::Matrix33f&,
                         const float&);
// =================================================== gmtl::Matrix<> functions

// gmtl::Vec<> functions ======================================================
   typedef gmtl::VecBase<float, 3> VecBase3f;
   typedef gmtl::VecBase<float, 4> VecBase4f;

   template Vec3f operator-(const VecBase3f&);
   template Vec4f operator-(const VecBase4f&);

   template VecBase3f& operator+=(VecBase3f&, const VecBase3f&);
   template VecBase4f& operator+=(VecBase4f&, const VecBase4f&);

   template VecBase3f operator+(const VecBase3f&, const VecBase3f&);
   template VecBase4f operator+(const VecBase4f&, const VecBase4f&);

   template VecBase3f& operator-=(VecBase3f&, const VecBase3f&);
   template VecBase4f& operator-=(VecBase4f&, const VecBase4f&);

   template Vec3f operator-(const VecBase3f&, const VecBase3f&);
   template Vec4f operator-(const VecBase4f&, const VecBase4f&);

   template VecBase3f& operator*=(VecBase3f&, const int&);
   template VecBase3f& operator*=(VecBase3f&, const float&);
   template VecBase4f& operator*=(VecBase4f&, const int&);
   template VecBase4f& operator*=(VecBase4f&, const float&);

   template VecBase3f operator*(const VecBase3f&, const int&);
   template VecBase3f operator*(const VecBase3f&, const float&);
   template VecBase4f operator*(const VecBase4f&, const int&);
   template VecBase4f operator*(const VecBase4f&, const float&);

   template VecBase3f operator*(const int&, const VecBase3f&);
   template VecBase3f operator*(const float&, const VecBase3f&);
   template VecBase4f operator*(const int&, const VecBase4f&);
   template VecBase4f operator*(const float&, const VecBase4f&);

   template VecBase3f& operator/=(VecBase3f&, const int&);
   template VecBase3f& operator/=(VecBase3f&, const float&);
   template VecBase4f& operator/=(VecBase4f&, const int&);
   template VecBase4f& operator/=(VecBase4f&, const float&);

   template VecBase3f operator/(const VecBase3f&, const int&);
   template VecBase3f operator/(const VecBase3f&, const float&);
   template VecBase4f operator/(const VecBase4f&, const int&);
   template VecBase4f operator/(const VecBase4f&, const float&);

   template float dot(const gmtl::Vec3f&, const gmtl::Vec3f&);
   template float dot(const gmtl::Vec4f&, const gmtl::Vec4f&);

   template float length(const gmtl::Vec3f&);
   template float length(const gmtl::Vec4f&);

   template float lengthSquared(const gmtl::Vec3f&);
   template float lengthSquared(const gmtl::Vec4f&);

   template float normalize(gmtl::Vec3f&);
   template float normalize(gmtl::Vec4f&);

   template bool isNormalized(const gmtl::Vec3f&, const float);
   template bool isNormalized(const gmtl::Vec4f&, const float);

   template gmtl::Vec3f cross(const gmtl::Vec3f&, const gmtl::Vec3f&);
   template gmtl::Vec3f& cross(gmtl::Vec3f&, const gmtl::Vec3f&,
                               const gmtl::Vec3f&);

   template VecBase3f& reflect(VecBase3f&, const VecBase3f&, const Vec3f&);
   template VecBase4f& reflect(VecBase4f&, const VecBase4f&, const Vec4f&);

   template VecBase3f& lerp(VecBase3f&, const float&, const VecBase3f&,
                            const VecBase3f&);
   template VecBase4f& lerp(VecBase4f&, const float&, const VecBase4f&,
                            const VecBase4f&);

   template bool operator==(const gmtl::VecBase3f&, const gmtl::VecBase3f&);
   template bool operator==(const gmtl::VecBase4f&, const gmtl::VecBase4f&);

   template bool operator!=(const gmtl::VecBase3f&, const gmtl::VecBase3f&);
   template bool operator!=(const gmtl::VecBase4f&, const gmtl::VecBase4f&);

   template bool isEqual(const gmtl::VecBase4f&, const gmtl::VecBase4f&,
                         const float&);
   template bool isEqual(const gmtl::VecBase3f&, const gmtl::VecBase3f&,
                         const float&);
// ====================================================== gmtl::Vec<> functions
}

#endif /* _PYJUGGLER_GMTL_FUNCTIONS_H_ */
