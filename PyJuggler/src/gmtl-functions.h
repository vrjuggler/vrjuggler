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

// This file makes declarations of the templated function instantiations
// needed for Boost.Python to do its thing.

#include <gmtl/Math.h>
#include <gmtl/Matrix.h>
#include <gmtl/MatrixOps.h>
#include <gmtl/Vec.h>
#include <gmtl/VecOps.h>
#include <gmtl/Quat.h>
#include <gmtl/QuatOps.h>
#include <gmtl/EulerAngle.h>
#include <gmtl/EulerAngleOps.h>
#include <gmtl/AxisAngle.h>
#include <gmtl/AxisAngleOps.h>
#include <gmtl/Generate.h>

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

   template gmtl::Quatf& normalize(gmtl::Quatf&);
   template gmtl::Quatd& normalize(gmtl::Quatd&);

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

// gmtl::AxisAngle<> functions ================================================
   template bool operator==(const AxisAnglef&, const AxisAnglef&);
   template bool operator==(const AxisAngled&, const AxisAngled&);

   template bool operator!=(const AxisAnglef&, const AxisAnglef&);
   template bool operator!=(const AxisAngled&, const AxisAngled&);

   template bool isEqual(const AxisAnglef&, const AxisAnglef&, const float&);
   template bool isEqual(const AxisAngled&, const AxisAngled&, const double&);
// ================================================ gmtl::AxisAngle<> functions

// Generator functions ========================================================
   // See the gmtlWrapper namespace for GMTL functions that use the
   // Type2Type idiom.

   template gmtl::Matrix44f& setRot(gmtl::Matrix44f&, const Vec3f&,
                                    const Vec3f&);
   template gmtl::Quatf& setRot(gmtl::Quatf&, const Vec3f&, const Vec3f&);

   template gmtl::Vec3f makeVec(const gmtl::Quatf&);
   template gmtl::Vec3d makeVec(const gmtl::Quatd&);

   template gmtl::Vec3f makeNormal(gmtl::Vec3f);
   template gmtl::Vec4f makeNormal(gmtl::Vec4f);

   template gmtl::Vec3f& setTrans(gmtl::Vec3f&, const gmtl::Matrix44f&);
   template gmtl::Vec3d& setTrans(gmtl::Vec3d&, const gmtl::Matrix44d&);

   template gmtl::Quatf& setPure(gmtl::Quatf&, const gmtl::Vec3f&);
   template gmtl::Quatd& setPure(gmtl::Quatd&, const gmtl::Vec3d&);

   template gmtl::Quatf makePure(const gmtl::Vec3f&);
   template gmtl::Quatd makePure(const gmtl::Vec3d&);

   template gmtl::Quatf makeNormal(const gmtl::Quatf&);
   template gmtl::Quatd makeNormal(const gmtl::Quatd&);

   template gmtl::Quatf makeConj(const gmtl::Quatf&);
   template gmtl::Quatd makeConj(const gmtl::Quatd&);

   template gmtl::Quatf makeInvert(const gmtl::Quatf&);
   template gmtl::Quatd makeInvert(const gmtl::Quatd&);

   template gmtl::Quatf& set(gmtl::Quatf&, const gmtl::AxisAnglef&);
   template gmtl::Quatd& set(gmtl::Quatd&, const gmtl::AxisAngled&);

   template gmtl::Quatf& setRot(gmtl::Quatf&, const gmtl::EulerAngleXYZf&);
   template gmtl::Quatf& setRot(gmtl::Quatf&, const gmtl::EulerAngleZYXf&);
   template gmtl::Quatf& setRot(gmtl::Quatf&, const gmtl::EulerAngleZXYf&);

   template gmtl::Quatf& set(gmtl::Quatf&, const gmtl::Matrix44f&);
   template gmtl::Quatd& set(gmtl::Quatd&, const gmtl::Matrix44d&);

   template gmtl::Quatf& setRot(gmtl::Quatf&, const gmtl::Matrix44f&);
   template gmtl::Quatd& setRot(gmtl::Quatd&, const gmtl::Matrix44d&);

   template gmtl::AxisAnglef& set(gmtl::AxisAnglef&, gmtl::Quatf);
   template gmtl::AxisAngled& set(gmtl::AxisAngled&, gmtl::Quatd);

   template gmtl::AxisAnglef& setRot(gmtl::AxisAnglef&, gmtl::Quatf);
   template gmtl::AxisAngled& setRot(gmtl::AxisAngled&, gmtl::Quatd);

   template gmtl::AxisAnglef makeNormal(const gmtl::AxisAnglef&);
   template gmtl::AxisAngled makeNormal(const gmtl::AxisAngled&);

   template gmtl::EulerAngleXYZf& set(gmtl::EulerAngleXYZf&,
                                      const gmtl::Matrix44f&);
   template gmtl::EulerAngleZYXf& set(gmtl::EulerAngleZYXf&,
                                      const gmtl::Matrix44f&);
   template gmtl::EulerAngleZXYf& set(gmtl::EulerAngleZXYf&,
                                      const gmtl::Matrix44f&);

   template gmtl::EulerAngleXYZf& setRot(gmtl::EulerAngleXYZf&,
                                         const gmtl::Matrix44f&);
   template gmtl::EulerAngleZYXf& setRot(gmtl::EulerAngleZYXf&,
                                         const gmtl::Matrix44f&);
   template gmtl::EulerAngleZXYf& setRot(gmtl::EulerAngleZXYf&,
                                         const gmtl::Matrix44f&);

   template gmtl::Matrix44f& setTrans(gmtl::Matrix44f&, const gmtl::Vec3f&);
   template gmtl::Matrix44d& setTrans(gmtl::Matrix44d&, const gmtl::Vec3d&);

   template gmtl::Matrix44f& setScale(gmtl::Matrix44f&, const gmtl::Vec3f&);
   template gmtl::Matrix44d& setScale(gmtl::Matrix44d&, const gmtl::Vec3d&);

   template gmtl::Matrix44f& setScale(gmtl::Matrix44f&, const float);
   template gmtl::Matrix44d& setScale(gmtl::Matrix44d&, const double);

   template gmtl::Matrix44f& setRot(gmtl::Matrix44f&, const gmtl::AxisAnglef&);
   template gmtl::Matrix44d& setRot(gmtl::Matrix44d&, const gmtl::AxisAngled&);

   template gmtl::Matrix44f& set(gmtl::Matrix44f&, const gmtl::AxisAnglef&);
   template gmtl::Matrix44d& set(gmtl::Matrix44d&, const gmtl::AxisAngled&);

   template gmtl::Matrix44f& setRot(gmtl::Matrix44f&,
                                    const gmtl::EulerAngleXYZf&);
   template gmtl::Matrix44f& setRot(gmtl::Matrix44f&,
                                    const gmtl::EulerAngleZYXf&);
   template gmtl::Matrix44f& setRot(gmtl::Matrix44f&,
                                    const gmtl::EulerAngleZXYf&);
   template gmtl::Matrix44d& setRot(gmtl::Matrix44d&,
                                    const gmtl::EulerAngleXYZd&);
   template gmtl::Matrix44d& setRot(gmtl::Matrix44d&,
                                    const gmtl::EulerAngleZYXd&);
   template gmtl::Matrix44d& setRot(gmtl::Matrix44d&,
                                    const gmtl::EulerAngleZXYd&);

   template gmtl::Matrix44f& set(gmtl::Matrix44f&, const gmtl::EulerAngleXYZf&);
   template gmtl::Matrix44f& set(gmtl::Matrix44f&, const gmtl::EulerAngleZYXf&);
   template gmtl::Matrix44f& set(gmtl::Matrix44f&, const gmtl::EulerAngleZXYf&);
   template gmtl::Matrix44d& set(gmtl::Matrix44d&, const gmtl::EulerAngleXYZd&);
   template gmtl::Matrix44d& set(gmtl::Matrix44d&, const gmtl::EulerAngleZYXd&);
   template gmtl::Matrix44d& set(gmtl::Matrix44d&, const gmtl::EulerAngleZXYd&);

   template float makeYRot(const gmtl::Matrix33f&);
   template float makeYRot(const gmtl::Matrix44f&);
   template double makeYRot(const gmtl::Matrix33d&);
   template double makeYRot(const gmtl::Matrix44d&);

   template float makeXRot(const gmtl::Matrix33f&);
   template float makeXRot(const gmtl::Matrix44f&);
   template double makeXRot(const gmtl::Matrix33d&);
   template double makeXRot(const gmtl::Matrix44d&);

   template float makeZRot(const gmtl::Matrix33f&);
   template float makeZRot(const gmtl::Matrix44f&);
   template double makeZRot(const gmtl::Matrix33d&);
   template double makeZRot(const gmtl::Matrix44d&);

   template gmtl::Matrix44f& setDirCos(gmtl::Matrix44f&, const gmtl::Vec3f&,
                                       const gmtl::Vec3f&, const gmtl::Vec3f&,
                                       const gmtl::Vec3f&, const gmtl::Vec3f&,
                                       const gmtl::Vec3f&);
   template gmtl::Matrix44d& setDirCos(gmtl::Matrix44d&, const gmtl::Vec3d&,
                                       const gmtl::Vec3d&, const gmtl::Vec3d&,
                                       const gmtl::Vec3d&, const gmtl::Vec3d&,
                                       const gmtl::Vec3d&);

   template gmtl::Matrix44f& setAxes(gmtl::Matrix44f&, const gmtl::Vec3f&,
                                       const gmtl::Vec3f&, const gmtl::Vec3f&);
   template gmtl::Matrix44d& setAxes(gmtl::Matrix44d&, const gmtl::Vec3d&,
                                       const gmtl::Vec3d&, const gmtl::Vec3d&);

   template gmtl::Matrix33f makeTranspose(const gmtl::Matrix33f&);
   template gmtl::Matrix44f makeTranspose(const gmtl::Matrix44f&);
   template gmtl::Matrix33d makeTranspose(const gmtl::Matrix33d&);
   template gmtl::Matrix44d makeTranspose(const gmtl::Matrix44d&);

   template gmtl::Matrix33f makeInverse(const gmtl::Matrix33f&);
   template gmtl::Matrix44f makeInverse(const gmtl::Matrix44f&);
   template gmtl::Matrix33d makeInverse(const gmtl::Matrix33d&);
   template gmtl::Matrix44d makeInverse(const gmtl::Matrix44d&);

   // XXX: gmtl::set(Matrix, const Coord) missing...

   template gmtl::Matrix33f& setRot(gmtl::Matrix33f&, const gmtl::Quatf&);
   template gmtl::Matrix44f& setRot(gmtl::Matrix44f&, const gmtl::Quatf&);
   template gmtl::Matrix33d& setRot(gmtl::Matrix33d&, const gmtl::Quatd&);
   template gmtl::Matrix44d& setRot(gmtl::Matrix44d&, const gmtl::Quatd&);

   template gmtl::Matrix33f& set(gmtl::Matrix33f&, const gmtl::Quatf&);
   template gmtl::Matrix44f& set(gmtl::Matrix44f&, const gmtl::Quatf&);
   template gmtl::Matrix33d& set(gmtl::Matrix33d&, const gmtl::Quatd&);
   template gmtl::Matrix44d& set(gmtl::Matrix44d&, const gmtl::Quatd&);

   // XXX: gmtl::set(Coord, const Matrix) missing...
   // XXX: gmtl::setRot(Coord, const Matrix) missing...

// ======================================================== Generator functions
}


namespace gmtlWrapper
{

   template<typename DATA_TYPE>
   gmtl::Matrix<DATA_TYPE, 3, 3> makeRotMatrix33(const gmtl::Quat<DATA_TYPE>& q)
   {
      return gmtl::makeRot< gmtl::Matrix<DATA_TYPE, 3, 3> >(q);
   }

   template<typename DATA_TYPE>
   gmtl::Matrix<DATA_TYPE, 4, 4> makeRotMatrix44(const gmtl::Quat<DATA_TYPE>& q)
   {
      return gmtl::makeRot< gmtl::Matrix<DATA_TYPE, 4, 4> >(q);
   }

   template<typename DATA_TYPE, typename ROT_ORDER>
   gmtl::Matrix<DATA_TYPE, 3, 3> makeRotMatrix33(const gmtl::EulerAngle<DATA_TYPE, ROT_ORDER>& e)
   {
      return gmtl::makeRot< gmtl::Matrix<DATA_TYPE, 3, 3> >(e);
   }

   template<typename DATA_TYPE, typename ROT_ORDER>
   gmtl::Matrix<DATA_TYPE, 4, 4> makeRotMatrix44(const gmtl::EulerAngle<DATA_TYPE, ROT_ORDER>& e)
   {
      return gmtl::makeRot< gmtl::Matrix<DATA_TYPE, 4, 4> >(e);
   }

   template<typename DATA_TYPE, unsigned ROW, unsigned COL>
   gmtl::Quat<DATA_TYPE> makeRotQuat(const gmtl::Matrix<DATA_TYPE, ROW, COL>& m)
   {
      return gmtl::makeRot< gmtl::Quat<DATA_TYPE> >(m);
   }

   template<typename DATA_TYPE, unsigned ROW, unsigned COL>
   gmtl::EulerAngle<DATA_TYPE, gmtl::XYZ> makeRotEulerAngleXYZ(const gmtl::Matrix<DATA_TYPE, ROW, COL>& m)
   {
      return gmtl::makeRot< gmtl::EulerAngle<DATA_TYPE, gmtl::XYZ> >(m);
   }

   template<typename DATA_TYPE, unsigned ROW, unsigned COL>
   gmtl::EulerAngle<DATA_TYPE, gmtl::ZYX> makeRotEulerAngleZYX(const gmtl::Matrix<DATA_TYPE, ROW, COL>& m)
   {
      return gmtl::makeRot< gmtl::EulerAngle<DATA_TYPE, gmtl::ZYX> >(m);
   }

   template<typename DATA_TYPE, unsigned ROW, unsigned COL>
   gmtl::EulerAngle<DATA_TYPE, gmtl::ZXY> makeRotEulerAngleZXY(const gmtl::Matrix<DATA_TYPE, ROW, COL>& m)
   {
      return gmtl::makeRot< gmtl::EulerAngle<DATA_TYPE, gmtl::ZXY> >(m);
   }

   template gmtl::Matrix33f      makeRotMatrix33(const gmtl::Quatf&);
   template gmtl::Matrix33d      makeRotMatrix33(const gmtl::Quatd&);
   template gmtl::Matrix33f      makeRotMatrix33(const gmtl::EulerAngleXYZf&);
   template gmtl::Matrix33f      makeRotMatrix33(const gmtl::EulerAngleZYXf&);
   template gmtl::Matrix33f      makeRotMatrix33(const gmtl::EulerAngleZXYf&);
   template gmtl::Matrix44f      makeRotMatrix44(const gmtl::Quatf&);
   template gmtl::Matrix44d      makeRotMatrix44(const gmtl::Quatd&);
   template gmtl::Matrix44f      makeRotMatrix44(const gmtl::EulerAngleXYZf&);
   template gmtl::Matrix44f      makeRotMatrix44(const gmtl::EulerAngleZYXf&);
   template gmtl::Matrix44f      makeRotMatrix44(const gmtl::EulerAngleZXYf&);
   template gmtl::Quatf          makeRotQuat(const gmtl::Matrix44f&);
   template gmtl::Quatd          makeRotQuat(const gmtl::Matrix44d&);
   template gmtl::EulerAngleXYZf makeRotEulerAngleXYZ(const gmtl::Matrix44f&);
   template gmtl::EulerAngleZYXf makeRotEulerAngleZYX(const gmtl::Matrix44f&);
   template gmtl::EulerAngleZXYf makeRotEulerAngleZXY(const gmtl::Matrix44f&);
   template gmtl::EulerAngleXYZd makeRotEulerAngleXYZ(const gmtl::Matrix44d&);
   template gmtl::EulerAngleZYXd makeRotEulerAngleZYX(const gmtl::Matrix44d&);
   template gmtl::EulerAngleZXYd makeRotEulerAngleZXY(const gmtl::Matrix44d&);

   template<typename DATA_TYPE>
   gmtl::Matrix<DATA_TYPE, 4, 4> makeRotMatrix44(const gmtl::Vec<DATA_TYPE, 3>& from,
                                                 const gmtl::Vec<DATA_TYPE, 3>& to)
   {
      return gmtl::makeRot< gmtl::Matrix<DATA_TYPE, 4, 4> >(from, to);
   }

   template<typename DATA_TYPE>
   gmtl::Quat<DATA_TYPE> makeRotQuat(const gmtl::Vec<DATA_TYPE, 3>& from,
                                     const gmtl::Vec<DATA_TYPE, 3>& to)
   {
      return gmtl::makeRot< gmtl::Quat<DATA_TYPE> >(from, to);
   }

   template gmtl::Matrix44f makeRotMatrix44(const gmtl::Vec3f&,
                                            const gmtl::Vec3f&);
   template gmtl::Quatf     makeRotQuat(const gmtl::Vec3f&, const gmtl::Vec3f&);
   template gmtl::Quatd     makeRotQuat(const gmtl::Vec3d&, const gmtl::Vec3d&);

   template<typename DATA_TYPE>
   gmtl::Matrix<DATA_TYPE, 3, 3> makeDirCosMatrix33(const gmtl::Vec<DATA_TYPE, 3>& xDestAxis,
                                                    const gmtl::Vec<DATA_TYPE, 3>& yDestAxis,
                                                    const gmtl::Vec<DATA_TYPE, 3>& zDestAxis,
                                                    const gmtl::Vec<DATA_TYPE, 3>& xSrcAxis = gmtl::Vec<DATA_TYPE, 3>(1, 0, 0),
                                                    const gmtl::Vec<DATA_TYPE, 3>& ySrcAxis = gmtl::Vec<DATA_TYPE, 3>(1, 0, 0),
                                                    const gmtl::Vec<DATA_TYPE, 3>& zSrcAxis = gmtl::Vec<DATA_TYPE, 3>(1, 0, 0))
   {
      return gmtl::makeDirCos< gmtl::Matrix<DATA_TYPE, 3, 3> >(xDestAxis,
                                                               yDestAxis,
                                                               zDestAxis,
                                                               xSrcAxis,
                                                               ySrcAxis,
                                                               zSrcAxis);
   }

   template<typename DATA_TYPE>
   gmtl::Matrix<DATA_TYPE, 4, 4> makeDirCosMatrix44(const gmtl::Vec<DATA_TYPE, 3>& xDestAxis,
                                                    const gmtl::Vec<DATA_TYPE, 3>& yDestAxis,
                                                    const gmtl::Vec<DATA_TYPE, 3>& zDestAxis,
                                                    const gmtl::Vec<DATA_TYPE, 3>& xSrcAxis = gmtl::Vec<DATA_TYPE, 3>(1, 0, 0),
                                                    const gmtl::Vec<DATA_TYPE, 3>& ySrcAxis = gmtl::Vec<DATA_TYPE, 3>(1, 0, 0),
                                                    const gmtl::Vec<DATA_TYPE, 3>& zSrcAxis = gmtl::Vec<DATA_TYPE, 3>(1, 0, 0))
   {
      return gmtl::makeDirCos< gmtl::Matrix<DATA_TYPE, 4, 4> >(xDestAxis,
                                                               yDestAxis,
                                                               zDestAxis,
                                                               xSrcAxis,
                                                               ySrcAxis,
                                                               zSrcAxis);
   }

   template gmtl::Matrix33f makeDirCosMatrix33(const gmtl::Vec3f&,
                                               const gmtl::Vec3f&,
                                               const gmtl::Vec3f&,
                                               const gmtl::Vec3f&,
                                               const gmtl::Vec3f&,
                                               const gmtl::Vec3f&);
   template gmtl::Matrix33d makeDirCosMatrix33(const gmtl::Vec3d&,
                                               const gmtl::Vec3d&,
                                               const gmtl::Vec3d&,
                                               const gmtl::Vec3d&,
                                               const gmtl::Vec3d&,
                                               const gmtl::Vec3d&);
   template gmtl::Matrix44f makeDirCosMatrix44(const gmtl::Vec3f&,
                                               const gmtl::Vec3f&,
                                               const gmtl::Vec3f&,
                                               const gmtl::Vec3f&,
                                               const gmtl::Vec3f&,
                                               const gmtl::Vec3f&);
   template gmtl::Matrix44d makeDirCosMatrix44(const gmtl::Vec3d&,
                                               const gmtl::Vec3d&,
                                               const gmtl::Vec3d&,
                                               const gmtl::Vec3d&,
                                               const gmtl::Vec3d&,
                                               const gmtl::Vec3d&);


   template<typename DATA_TYPE>
   gmtl::Matrix<DATA_TYPE, 4, 4> makeTransMatrix44(const gmtl::Vec<DATA_TYPE, 3>& v)
   {
      return gmtl::makeTrans< gmtl::Matrix<DATA_TYPE, 4, 4> >(v);
   }

   template<typename DATA_TYPE, unsigned ROW, unsigned COL>
   gmtl::Vec<DATA_TYPE, 3> makeTransVec3(const gmtl::Matrix<DATA_TYPE, ROW, COL>& m)
   {
      return gmtl::makeTrans< gmtl::Vec<DATA_TYPE, 3> >(m);
   }

   template gmtl::Matrix44f makeTransMatrix44(const gmtl::Vec3f&);
   template gmtl::Matrix44d makeTransMatrix44(const gmtl::Vec3d&);
   template gmtl::Vec3f     makeTransVec3(const gmtl::Matrix44f&);
   template gmtl::Vec3d     makeTransVec3(const gmtl::Matrix44d&);

   template<typename DATA_TYPE, unsigned SIZE>
   gmtl::Matrix<DATA_TYPE, 4, 4> makeScaleMatrix44(const gmtl::Vec<DATA_TYPE, SIZE>& scale)
   {
      return gmtl::makeScale< gmtl::Matrix<DATA_TYPE, 4, 4> >(scale);
   }

   template<typename DATA_TYPE>
   gmtl::Matrix<DATA_TYPE, 4, 4> makeScaleMatrix44(const DATA_TYPE scale)
   {
      return gmtl::makeScale< gmtl::Matrix<DATA_TYPE, 4, 4> >(scale);
   }

   template gmtl::Matrix44f makeScaleMatrix44(const gmtl::Vec3f&);
   template gmtl::Matrix44d makeScaleMatrix44(const gmtl::Vec3d&);
   template gmtl::Matrix44f makeScaleMatrix44(const gmtl::Vec4f&);
   template gmtl::Matrix44d makeScaleMatrix44(const gmtl::Vec4d&);

   template gmtl::Matrix44f makeScaleMatrix44(const float);
   template gmtl::Matrix44d makeScaleMatrix44(const double);

   template<typename DATA_TYPE>
   gmtl::Matrix<DATA_TYPE, 4, 4> makeAxesMatrix44(const gmtl::Vec<DATA_TYPE, 3>& xAxis,
                                                  const gmtl::Vec<DATA_TYPE, 3>& yAxis,
                                                  const gmtl::Vec<DATA_TYPE, 3>& zAxis)
   {
      return gmtl::makeAxes< gmtl::Matrix<DATA_TYPE, 4, 4> >(xAxis, yAxis,
                                                             zAxis);
   }

   template gmtl::Matrix44f makeAxesMatrix44(const gmtl::Vec3f&,
                                             const gmtl::Vec3f&,
                                             const gmtl::Vec3f&);
   template gmtl::Matrix44d makeAxesMatrix44(const gmtl::Vec3d&,
                                             const gmtl::Vec3d&,
                                             const gmtl::Vec3d&);
}


#endif /* _PYJUGGLER_GMTL_FUNCTIONS_H_ */
