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

#include <gmtl/Math.h>
#include <gmtl/Matrix.h>
#include <gmtl/MatrixOps.h>
#include <gmtl/Vec.h>
#include <gmtl/VecOps.h>
#include <gmtl/Quat.h>
#include <gmtl/QuatOps.h>
#include <gmtl/EulerAngle.h>
#include <gmtl/EulerAngleOps.h>

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

   template bool isEqual(const gmtl::VecBase3f&, const gmtl::VecBase3f&,
                         const float&);
   template bool isEqual(const gmtl::VecBase4f&, const gmtl::VecBase4f&,
                         const float&);
// ====================================================== gmtl::Vec<> functions

// gmtl::Quat<> functions =====================================================
   template gmtl::Quatf& mult(gmtl::Quatf&, const gmtl::Quatf&,
                              const gmtl::Quatf&);
   template gmtl::Quatd& mult(gmtl::Quatd&, const gmtl::Quatd&,
                              const gmtl::Quatd&);

   template gmtl::Quatf operator*(const gmtl::Quatf&, const gmtl::Quatf&);
   template gmtl::Quatd operator*(const gmtl::Quatd&, const gmtl::Quatd&);

   template gmtl::Quatf& operator*=(gmtl::Quatf&, const gmtl::Quatf&);
   template gmtl::Quatd& operator*=(gmtl::Quatd&, const gmtl::Quatd&);

   template gmtl::Quatf& mult(gmtl::Quatf&, const gmtl::Quatf&, float);
   template gmtl::Quatd& mult(gmtl::Quatd&, const gmtl::Quatd&, double);

   template gmtl::Quatf operator*(const gmtl::Quatf&, float);
   template gmtl::Quatd operator*(const gmtl::Quatd&, double);

   template gmtl::Quatf& operator*=(gmtl::Quatf&, float);
   template gmtl::Quatd& operator*=(gmtl::Quatd&, double);

   template gmtl::Quatf& negate(gmtl::Quatf&);
   template gmtl::Quatd& negate(gmtl::Quatd&);

   template gmtl::Quatf operator-(const gmtl::Quatf&);
   template gmtl::Quatd operator-(const gmtl::Quatd&);

   template gmtl::Quatf& div(gmtl::Quatf&, const gmtl::Quatf&, gmtl::Quatf);
   template gmtl::Quatd& div(gmtl::Quatd&, const gmtl::Quatd&, gmtl::Quatd);

   template gmtl::Quatf operator/(const gmtl::Quatf&, gmtl::Quatf);
   template gmtl::Quatd operator/(const gmtl::Quatd&, gmtl::Quatd);

   template gmtl::Quatf& operator/=(gmtl::Quatf&, const gmtl::Quatf&);
   template gmtl::Quatd& operator/=(gmtl::Quatd&, const gmtl::Quatd&);

   template gmtl::Quatf& div(gmtl::Quatf&, const gmtl::Quatf&, float);
   template gmtl::Quatd& div(gmtl::Quatd&, const gmtl::Quatd&, double);

   template gmtl::Quatf operator/(const gmtl::Quatf&, float);
   template gmtl::Quatd operator/(const gmtl::Quatd&, double);

//   template gmtl::Quatf& operator/=(const gmtl::Quatf&, float);
//   template gmtl::Quatd& operator/=(const gmtl::Quatd&, double);

   template gmtl::Quatf& add(gmtl::Quatf&, const gmtl::Quatf&,
                             const gmtl::Quatf&);
   template gmtl::Quatd& add(gmtl::Quatd&, const gmtl::Quatd&,
                             const gmtl::Quatd&);

   template gmtl::Quatf operator+(const gmtl::Quatf&, const gmtl::Quatf&);
   template gmtl::Quatd operator+(const gmtl::Quatd&, const gmtl::Quatd&);

   template gmtl::Quatf& operator+=(gmtl::Quatf&, const gmtl::Quatf&);
   template gmtl::Quatd& operator+=(gmtl::Quatd&, const gmtl::Quatd&);

   template gmtl::Quatf& sub(gmtl::Quatf&, const gmtl::Quatf&,
                             const gmtl::Quatf&);
   template gmtl::Quatd& sub(gmtl::Quatd&, const gmtl::Quatd&,
                             const gmtl::Quatd&);

   template gmtl::Quatf operator-(const gmtl::Quatf&, const gmtl::Quatf&);
   template gmtl::Quatd operator-(const gmtl::Quatd&, const gmtl::Quatd&);

   template gmtl::Quatf& operator-=(gmtl::Quatf&, const gmtl::Quatf&);
   template gmtl::Quatd& operator-=(gmtl::Quatd&, const gmtl::Quatd&);

   template float dot(const gmtl::Quatf&, const gmtl::Quatf&);
   template double dot(const gmtl::Quatd&, const gmtl::Quatd&);

   template float lengthSquared(const gmtl::Quatf&);
   template double lengthSquared(const gmtl::Quatd&);

   template float length(const gmtl::Quatf&);
   template double length(const gmtl::Quatd&);

   // XXX: These are commented out because the gmtl::normalize() for
   // quaternions has different return semantics than that of the version used
   // for vectors.
//   template gmtl::Quatf& normalize(gmtl::Quatf&);
//   template gmtl::Quatd& normalize(gmtl::Quatd&);

   template bool isNormalized(const gmtl::Quatf&, const float);
   template bool isNormalized(const gmtl::Quatd&, const double);

   template gmtl::Quatf& conj(gmtl::Quatf&);
   template gmtl::Quatd& conj(gmtl::Quatd&);

   template gmtl::Quatf& invert(gmtl::Quatf&);
   template gmtl::Quatd& invert(gmtl::Quatd&);

   template gmtl::Quatf& exp(gmtl::Quatf&);
   template gmtl::Quatd& exp(gmtl::Quatd&);

   template gmtl::Quatf& log(gmtl::Quatf&);
   template gmtl::Quatd& log(gmtl::Quatd&);

   template void squad(gmtl::Quatf&, float, const Quatf&, const Quatf&,
                       const Quatf&, const Quatf&);
   template void squad(gmtl::Quatd&, double, const Quatd&, const Quatd&,
                       const Quatd&, const Quatd&);

   template Quatf& slerp(Quatf&, const float, const Quatf&, const Quatf&,
                         bool);
   template Quatd& slerp(Quatd&, const double, const Quatd&, const Quatd&,
                         bool);

   template Quatf& lerp(Quatf&, const float, const Quatf&, const Quatf&);
   template Quatd& lerp(Quatd&, const double, const Quatd&, const Quatd&);

   template bool operator==(const gmtl::Quatf&, const gmtl::Quatf&);
   template bool operator==(const gmtl::Quatd&, const gmtl::Quatd&);

   template bool operator!=(const gmtl::Quatf&, const gmtl::Quatf&);
   template bool operator!=(const gmtl::Quatd&, const gmtl::Quatd&);

   template bool isEqual(const gmtl::Quatf&, const gmtl::Quatf&, float);
   template bool isEqual(const gmtl::Quatd&, const gmtl::Quatd&, double);

   template bool isEquiv(const gmtl::Quatf&, const gmtl::Quatf&, float);
   template bool isEquiv(const gmtl::Quatd&, const gmtl::Quatd&, double);
// ===================================================== gmtl::Quat<> functions

// gmtl::EulerAngle<> functions ===============================================
   template bool operator==(const EulerAngleXYZf&, const EulerAngleXYZf&);
   template bool operator==(const EulerAngleZXYf&, const EulerAngleZXYf&);
   template bool operator==(const EulerAngleZYXf&, const EulerAngleZYXf&);

   template bool operator!=(const EulerAngleXYZf&, const EulerAngleXYZf&);
   template bool operator!=(const EulerAngleZXYf&, const EulerAngleZXYf&);
   template bool operator!=(const EulerAngleZYXf&, const EulerAngleZYXf&);

   template bool isEqual(const EulerAngleXYZf&, const EulerAngleXYZf&,
                         const float&);
   template bool isEqual(const EulerAngleZXYf&, const EulerAngleZXYf&,
                         const float&);
   template bool isEqual(const EulerAngleZYXf&, const EulerAngleZYXf&,
                         const float&);
// =============================================== gmtl::EulerAngle<> functions
}

#endif /* _PYJUGGLER_GMTL_FUNCTIONS_H_ */
