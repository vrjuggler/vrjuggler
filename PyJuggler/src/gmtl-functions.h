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
#include <gmtl/Sphere.h>
#include <gmtl/SphereOps.h>
#include <gmtl/AABox.h>
#include <gmtl/AABoxOps.h>
#include <gmtl/LineSeg.h>
#include <gmtl/LineSegOps.h>
#include <gmtl/Plane.h>
#include <gmtl/PlaneOps.h>
#include <gmtl/Tri.h>
#include <gmtl/TriOps.h>
#include <gmtl/Coord.h>
#include <gmtl/CoordOps.h>
#include <gmtl/Containment.h>
#include <gmtl/Generate.h>
#include <gmtl/Xforms.h>

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
                                  const float&);
   template gmtl::Matrix33f& mult(gmtl::Matrix33f&, const gmtl::Matrix33f&,
                                  const float&);

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

   template bool isEqual(const gmtl::Matrix44f&, const gmtl::Matrix44f&,
                         const float&);
   template bool isEqual(const gmtl::Matrix33f&, const gmtl::Matrix33f&,
                         const float&);
// =================================================== gmtl::Matrix<> functions

// gmtl::Vec<> functions ======================================================
   typedef gmtl::VecBase<float, 3> VecBase3f;
   typedef gmtl::VecBase<double, 3> VecBase3d;
   typedef gmtl::VecBase<float, 4> VecBase4f;
   typedef gmtl::VecBase<double, 4> VecBase4d;

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

   template gmtl::Vec3i& cross(gmtl::Vec3i&, const gmtl::Vec3i&,
                               const gmtl::Vec3i&);
   template gmtl::Vec3d& cross(gmtl::Vec3d&, const gmtl::Vec3d&,
                               const gmtl::Vec3d&);
   template gmtl::Vec3f& cross(gmtl::Vec3f&, const gmtl::Vec3f&,
                               const gmtl::Vec3f&);

   template gmtl::Vec3i makeCross(const gmtl::Vec3i&, const gmtl::Vec3i&);
   template gmtl::Vec3d makeCross(const gmtl::Vec3d&, const gmtl::Vec3d&);
   template gmtl::Vec3f makeCross(const gmtl::Vec3f&, const gmtl::Vec3f&);

   template VecBase3f& reflect(VecBase3f&, const VecBase3f&, const Vec3f&);
   template VecBase3d& reflect(VecBase3d&, const VecBase3d&, const Vec3d&);
   template VecBase4f& reflect(VecBase4f&, const VecBase4f&, const Vec4f&);
   template VecBase4d& reflect(VecBase4d&, const VecBase4d&, const Vec4d&);

   template VecBase3f& lerp(VecBase3f&, const float&, const VecBase3f&,
                            const VecBase3f&);
   template VecBase3d& lerp(VecBase3d&, const double&, const VecBase3d&,
                            const VecBase3d&);
   template VecBase4f& lerp(VecBase4f&, const float&, const VecBase4f&,
                            const VecBase4f&);
   template VecBase4d& lerp(VecBase4d&, const double&, const VecBase4d&,
                            const VecBase4d&);

   template bool isEqual(const gmtl::VecBase3f&, const gmtl::VecBase3f&,
                         const float&);
   template bool isEqual(const gmtl::VecBase3d&, const gmtl::VecBase3d&,
                         const double&);
   template bool isEqual(const gmtl::VecBase4f&, const gmtl::VecBase4f&,
                         const float&);
   template bool isEqual(const gmtl::VecBase4d&, const gmtl::VecBase4d&,
                         const double&);
// ====================================================== gmtl::Vec<> functions

// gmtl::Quat<> functions =====================================================
   template gmtl::Quatf& mult(gmtl::Quatf&, const gmtl::Quatf&,
                              const gmtl::Quatf&);
   template gmtl::Quatd& mult(gmtl::Quatd&, const gmtl::Quatd&,
                              const gmtl::Quatd&);

   template gmtl::Quatf& mult(gmtl::Quatf&, const gmtl::Quatf&, float);
   template gmtl::Quatd& mult(gmtl::Quatd&, const gmtl::Quatd&, double);

   template gmtl::Quatf& negate(gmtl::Quatf&);
   template gmtl::Quatd& negate(gmtl::Quatd&);

   template gmtl::Quatf& div(gmtl::Quatf&, const gmtl::Quatf&, gmtl::Quatf);
   template gmtl::Quatd& div(gmtl::Quatd&, const gmtl::Quatd&, gmtl::Quatd);

   template gmtl::Quatf& div(gmtl::Quatf&, const gmtl::Quatf&, float);
   template gmtl::Quatd& div(gmtl::Quatd&, const gmtl::Quatd&, double);

   template gmtl::Quatf& add(gmtl::Quatf&, const gmtl::Quatf&,
                             const gmtl::Quatf&);
   template gmtl::Quatd& add(gmtl::Quatd&, const gmtl::Quatd&,
                             const gmtl::Quatd&);

   template gmtl::Quatf& sub(gmtl::Quatf&, const gmtl::Quatf&,
                             const gmtl::Quatf&);
   template gmtl::Quatd& sub(gmtl::Quatd&, const gmtl::Quatd&,
                             const gmtl::Quatd&);

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

   template void squad(gmtl::Quatf&, float, const gmtl::Quatf&,
                       const gmtl::Quatf&, const gmtl::Quatf&,
                       const gmtl::Quatf&);
   template void squad(gmtl::Quatd&, double, const gmtl::Quatd&,
                       const gmtl::Quatd&, const gmtl::Quatd&,
                       const gmtl::Quatd&);

   template gmtl::Quatf& slerp(gmtl::Quatf&, const float, const gmtl::Quatf&,
                               const gmtl::Quatf&, bool);
   template gmtl::Quatd& slerp(gmtl::Quatd&, const double, const gmtl::Quatd&,
                               const gmtl::Quatd&, bool);

   template Quatf& lerp(Quatf&, const float, const Quatf&, const Quatf&);
   template Quatd& lerp(Quatd&, const double, const Quatd&, const Quatd&);

   template bool isEqual(const gmtl::Quatf&, const gmtl::Quatf&, float);
   template bool isEqual(const gmtl::Quatd&, const gmtl::Quatd&, double);

   template bool isEquiv(const gmtl::Quatf&, const gmtl::Quatf&, float);
   template bool isEquiv(const gmtl::Quatd&, const gmtl::Quatd&, double);
// ===================================================== gmtl::Quat<> functions

// gmtl::EulerAngle<> functions ===============================================
   template bool isEqual(const EulerAngleXYZf&, const EulerAngleXYZf&,
                         const float&);
   template bool isEqual(const EulerAngleZXYf&, const EulerAngleZXYf&,
                         const float&);
   template bool isEqual(const EulerAngleZYXf&, const EulerAngleZYXf&,
                         const float&);
// =============================================== gmtl::EulerAngle<> functions

// gmtl::AxisAngle<> functions ================================================
   template bool isEqual(const AxisAnglef&, const AxisAnglef&, const float&);
   template bool isEqual(const AxisAngled&, const AxisAngled&, const double&);
// ================================================ gmtl::AxisAngle<> functions

// gmtl::Sphere<> functions ===================================================
   template bool isEqual(const gmtl::Spheref&, const gmtl::Spheref&,
                         const float&);
   template bool isEqual(const gmtl::Sphered&, const gmtl::Sphered&,
                         const double&);
// =================================================== gmtl::Sphere<> functions

// gmtl::AABox<> functions ====================================================
   template bool isEqual(const gmtl::AABoxf&, const gmtl::AABoxf&,
                         const float&);
   template bool isEqual(const gmtl::AABoxd&, const gmtl::AABoxd&,
                         const double&);
// ==================================================== gmtl::AABox<> functions

// gmtl::LineSeg<> functions ==================================================
   template gmtl::Point3f findNearestPt(const gmtl::LineSegf&,
                                        const gmtl::Point3f&);
   template gmtl::Point3d findNearestPt(const gmtl::LineSegd&,
                                        const gmtl::Point3d&);

   template float distance(const gmtl::LineSegf&, const gmtl::Point3f&);
   template double distance(const gmtl::LineSegd&, const gmtl::Point3d&);

   template float distanceSquared(const gmtl::LineSegf&, const gmtl::Point3f&);
   template double distanceSquared(const gmtl::LineSegd&, const gmtl::Point3d&);

   template bool isEqual(const gmtl::LineSegf&, const gmtl::LineSegf&,
                         const float&);
   template bool isEqual(const gmtl::LineSegd&, const gmtl::LineSegd&,
                         const double&);
// ================================================== gmtl::LineSeg<> functions

// gmtl::Plane<> functions ====================================================
   template float distance(const gmtl::Planef&, const gmtl::Point3f&);
   template double distance(const gmtl::Planed&, const gmtl::Point3d&);

   template gmtl::PlaneSide whichSide(const gmtl::Planef&,
                                      const gmtl::Point3f&);
   template gmtl::PlaneSide whichSide(const gmtl::Planed&,
                                      const gmtl::Point3d&);

   template gmtl::PlaneSide whichSide(const gmtl::Planef&,
                                      const gmtl::Point3f&, const float&);
   template gmtl::PlaneSide whichSide(const gmtl::Planed&,
                                      const gmtl::Point3d&, const double&);

   template float findNearestPt(const gmtl::Planef&, const gmtl::Point3f&,
                                gmtl::Point3f&);
   template double findNearestPt(const gmtl::Planed&, const gmtl::Point3d&,
                                 gmtl::Point3d&);

   template bool isEqual(const gmtl::Planef&, const gmtl::Planef&,
                         const float&);
   template bool isEqual(const gmtl::Planed&, const gmtl::Planed&,
                         const double&);
// ==================================================== gmtl::Plane<> functions

// gmtl::Tri<> functions ======================================================
   template gmtl::Point3f center(const gmtl::Trif&);
   template gmtl::Point3d center(const gmtl::Trid&);
   template gmtl::Point3i center(const gmtl::Trii&);

   template gmtl::Vec3f normal(const gmtl::Trif&);
   template gmtl::Vec3d normal(const gmtl::Trid&);
//   template gmtl::Vec3i normal(const gmtl::Trii&);

   template bool isEqual(const gmtl::Trif&, const gmtl::Trif&, const float&);
   template bool isEqual(const gmtl::Trid&, const gmtl::Trid&, const double&);
// ====================================================== gmtl::Tri<> functions

// gmtl::Plane<> functions ====================================================
   template bool isEqual(const gmtl::Coord3fXYZ&, const gmtl::Coord3fXYZ&,
                         float);
   template bool isEqual(const gmtl::Coord3fZYX&, const gmtl::Coord3fZYX&,
                         float);
   template bool isEqual(const gmtl::Coord3fZXY&, const gmtl::Coord3fZXY&,
                         float);

   template bool isEqual(const gmtl::Coord3dXYZ&, const gmtl::Coord3dXYZ&,
                         double);
   template bool isEqual(const gmtl::Coord3dZYX&, const gmtl::Coord3dZYX&,
                         double);
   template bool isEqual(const gmtl::Coord3dZXY&, const gmtl::Coord3dZXY&,
                         double);

   template bool isEqual(const gmtl::Coord4fXYZ&, const gmtl::Coord4fXYZ&,
                         float);
   template bool isEqual(const gmtl::Coord4fZYX&, const gmtl::Coord4fZYX&,
                         float);
   template bool isEqual(const gmtl::Coord4fZXY&, const gmtl::Coord4fZXY&,
                         float);

   template bool isEqual(const gmtl::Coord4dXYZ&, const gmtl::Coord4dXYZ&,
                         double);
   template bool isEqual(const gmtl::Coord4dZYX&, const gmtl::Coord4dZYX&,
                         double);
   template bool isEqual(const gmtl::Coord4dZXY&, const gmtl::Coord4dZXY&,
                         double);

   template bool isEqual(const gmtl::Coord3fQuat&, const gmtl::Coord3fQuat&,
                         float);
   template bool isEqual(const gmtl::Coord3dQuat&, const gmtl::Coord3dQuat&,
                         double);

   template bool isEqual(const gmtl::Coord4fQuat&, const gmtl::Coord4fQuat&,
                         float);
   template bool isEqual(const gmtl::Coord4dQuat&, const gmtl::Coord4dQuat&,
                         double);

   template bool isEqual(const gmtl::Coord3fAxisAngle&,
                         const gmtl::Coord3fAxisAngle&, float);
   template bool isEqual(const gmtl::Coord3dAxisAngle&,
                         const gmtl::Coord3dAxisAngle&, double);

   template bool isEqual(const gmtl::Coord4fAxisAngle&,
                         const gmtl::Coord4fAxisAngle&, float);
   template bool isEqual(const gmtl::Coord4dAxisAngle&,
                         const gmtl::Coord4dAxisAngle&, double);
// ==================================================== gmtl::Plane<> functions

// Generator functions ========================================================
   // See gmtl-wrappers.h for GMTL functions that use the Type2Type idiom.

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

   template gmtl::Matrix44f& setFrustum(gmtl::Matrix44f&, float, float, float,
                                        float, float, float);
   template gmtl::Matrix44d& setFrustum(gmtl::Matrix44d&, double, double,
                                        double, double, double, double);

   template gmtl::Matrix44f& setPerspective(gmtl::Matrix44f&, float,
                                            float, float, float);
   template gmtl::Matrix44d& setPerspective(gmtl::Matrix44d&, double,
                                            double, double, double);

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

   template gmtl::Matrix33f makeInvert(const gmtl::Matrix33f&);
   template gmtl::Matrix44f makeInvert(const gmtl::Matrix44f&);
   template gmtl::Matrix33d makeInvert(const gmtl::Matrix33d&);
   template gmtl::Matrix44d makeInvert(const gmtl::Matrix44d&);

   template gmtl::Matrix33f& set(gmtl::Matrix33f&, const gmtl::Coord3fXYZ&);
   template gmtl::Matrix33f& set(gmtl::Matrix33f&, const gmtl::Coord3fZYX&);
   template gmtl::Matrix33f& set(gmtl::Matrix33f&, const gmtl::Coord3fZXY&);
   template gmtl::Matrix33f& set(gmtl::Matrix33f&, const gmtl::Coord4fXYZ&);
   template gmtl::Matrix33f& set(gmtl::Matrix33f&, const gmtl::Coord4fZYX&);
   template gmtl::Matrix33f& set(gmtl::Matrix33f&, const gmtl::Coord4fZXY&);

   template gmtl::Matrix44f& set(gmtl::Matrix44f&, const gmtl::Coord3fXYZ&);
   template gmtl::Matrix44f& set(gmtl::Matrix44f&, const gmtl::Coord3fZYX&);
   template gmtl::Matrix44f& set(gmtl::Matrix44f&, const gmtl::Coord3fZXY&);
   template gmtl::Matrix44f& set(gmtl::Matrix44f&, const gmtl::Coord4fXYZ&);
   template gmtl::Matrix44f& set(gmtl::Matrix44f&, const gmtl::Coord4fZYX&);
   template gmtl::Matrix44f& set(gmtl::Matrix44f&, const gmtl::Coord4fZXY&);

   template gmtl::Matrix33f& set(gmtl::Matrix33f&, const gmtl::Coord3fQuat&);
   template gmtl::Matrix33f& set(gmtl::Matrix33f&, const gmtl::Coord4fQuat&);

   template gmtl::Matrix44f& set(gmtl::Matrix44f&, const gmtl::Coord3fQuat&);
   template gmtl::Matrix44f& set(gmtl::Matrix44f&, const gmtl::Coord4fQuat&);

   template gmtl::Matrix33f& set(gmtl::Matrix33f&,
                                 const gmtl::Coord3fAxisAngle&);
   template gmtl::Matrix33f& set(gmtl::Matrix33f&,
                                 const gmtl::Coord4fAxisAngle&);

   template gmtl::Matrix44f& set(gmtl::Matrix44f&,
                                 const gmtl::Coord3fAxisAngle&);
   template gmtl::Matrix44f& set(gmtl::Matrix44f&,
                                 const gmtl::Coord4fAxisAngle&);

   template gmtl::Matrix33f& setRot(gmtl::Matrix33f&, const gmtl::Quatf&);
   template gmtl::Matrix44f& setRot(gmtl::Matrix44f&, const gmtl::Quatf&);
   template gmtl::Matrix33d& setRot(gmtl::Matrix33d&, const gmtl::Quatd&);
   template gmtl::Matrix44d& setRot(gmtl::Matrix44d&, const gmtl::Quatd&);

   template gmtl::Matrix33f& set(gmtl::Matrix33f&, const gmtl::Quatf&);
   template gmtl::Matrix44f& set(gmtl::Matrix44f&, const gmtl::Quatf&);
   template gmtl::Matrix33d& set(gmtl::Matrix33d&, const gmtl::Quatd&);
   template gmtl::Matrix44d& set(gmtl::Matrix44d&, const gmtl::Quatd&);

   template gmtl::Coord3fXYZ& set(gmtl::Coord3fXYZ&, const gmtl::Matrix33f&);
   template gmtl::Coord3fZYX& set(gmtl::Coord3fZYX&, const gmtl::Matrix33f&);
   template gmtl::Coord3fZXY& set(gmtl::Coord3fZXY&, const gmtl::Matrix33f&);
   template gmtl::Coord4fXYZ& set(gmtl::Coord4fXYZ&, const gmtl::Matrix33f&);
   template gmtl::Coord4fZYX& set(gmtl::Coord4fZYX&, const gmtl::Matrix33f&);
   template gmtl::Coord4fZXY& set(gmtl::Coord4fZXY&, const gmtl::Matrix33f&);

   template gmtl::Coord3fXYZ& set(gmtl::Coord3fXYZ&, const gmtl::Matrix44f&);
   template gmtl::Coord3fZYX& set(gmtl::Coord3fZYX&, const gmtl::Matrix44f&);
   template gmtl::Coord3fZXY& set(gmtl::Coord3fZXY&, const gmtl::Matrix44f&);
   template gmtl::Coord4fXYZ& set(gmtl::Coord4fXYZ&, const gmtl::Matrix44f&);
   template gmtl::Coord4fZYX& set(gmtl::Coord4fZYX&, const gmtl::Matrix44f&);
   template gmtl::Coord4fZXY& set(gmtl::Coord4fZXY&, const gmtl::Matrix44f&);

   template gmtl::Coord3fQuat& set(gmtl::Coord3fQuat&, const gmtl::Matrix33f&);
   template gmtl::Coord4fQuat& set(gmtl::Coord4fQuat&, const gmtl::Matrix33f&);

   template gmtl::Coord3fQuat& set(gmtl::Coord3fQuat&, const gmtl::Matrix44f&);
   template gmtl::Coord4fQuat& set(gmtl::Coord4fQuat&, const gmtl::Matrix44f&);

   template gmtl::Coord3fXYZ& setRot(gmtl::Coord3fXYZ&,
                                     const gmtl::Matrix33f&);
   template gmtl::Coord3fZYX& setRot(gmtl::Coord3fZYX&,
                                     const gmtl::Matrix33f&);
   template gmtl::Coord3fZXY& setRot(gmtl::Coord3fZXY&,
                                     const gmtl::Matrix33f&);
   template gmtl::Coord4fXYZ& setRot(gmtl::Coord4fXYZ&,
                                     const gmtl::Matrix33f&);
   template gmtl::Coord4fZYX& setRot(gmtl::Coord4fZYX&,
                                     const gmtl::Matrix33f&);
   template gmtl::Coord4fZXY& setRot(gmtl::Coord4fZXY&,
                                     const gmtl::Matrix33f&);

   template gmtl::Coord3fXYZ& setRot(gmtl::Coord3fXYZ&,
                                     const gmtl::Matrix44f&);
   template gmtl::Coord3fZYX& setRot(gmtl::Coord3fZYX&,
                                     const gmtl::Matrix44f&);
   template gmtl::Coord3fZXY& setRot(gmtl::Coord3fZXY&,
                                     const gmtl::Matrix44f&);
   template gmtl::Coord4fXYZ& setRot(gmtl::Coord4fXYZ&,
                                     const gmtl::Matrix44f&);
   template gmtl::Coord4fZYX& setRot(gmtl::Coord4fZYX&,
                                     const gmtl::Matrix44f&);
   template gmtl::Coord4fZXY& setRot(gmtl::Coord4fZXY&,
                                     const gmtl::Matrix44f&);

   template gmtl::Coord3fQuat& setRot(gmtl::Coord3fQuat&,
                                      const gmtl::Matrix33f&);
   template gmtl::Coord4fQuat& setRot(gmtl::Coord4fQuat&,
                                      const gmtl::Matrix33f&);

   template gmtl::Coord3fQuat& setRot(gmtl::Coord3fQuat&,
                                      const gmtl::Matrix44f&);
   template gmtl::Coord4fQuat& setRot(gmtl::Coord4fQuat&,
                                      const gmtl::Matrix44f&);

// ======================================================== Generator functions

// Containment functions ======================================================
   template bool isInVolume(const gmtl::Spheref&, const gmtl::Point3f&);
   template bool isInVolume(const gmtl::Sphered&, const gmtl::Point3d&);

   template bool isInVolume(const gmtl::Spheref&, const gmtl::Spheref&);
   template bool isInVolume(const gmtl::Sphered&, const gmtl::Sphered&);

   template void extendVolume(gmtl::Spheref&, const gmtl::Point3f&);
   template void extendVolume(gmtl::Sphered&, const gmtl::Point3d&);

   template void extendVolume(gmtl::Spheref&, const gmtl::Spheref&);
   template void extendVolume(gmtl::Sphered&, const gmtl::Sphered&);

   template void makeVolume(gmtl::Spheref&, const std::vector<gmtl::Point3f>&);
   template void makeVolume(gmtl::Sphered&, const std::vector<gmtl::Point3d>&);

   template bool isOnVolume(const gmtl::Spheref&, const gmtl::Point3f&,
                            const float&);
   template bool isOnVolume(const gmtl::Sphered&, const gmtl::Point3d&,
                            const double&);

   template void extendVolume(gmtl::AABoxf&, const gmtl::Point3f&);
   template void extendVolume(gmtl::AABoxd&, const gmtl::Point3d&);

   template void extendVolume(gmtl::AABoxf&, const gmtl::AABoxf&);
   template void extendVolume(gmtl::AABoxd&, const gmtl::AABoxd&);

   template void makeVolume(gmtl::AABoxf&, const gmtl::Spheref&);
   template void makeVolume(gmtl::AABoxd&, const gmtl::Sphered&);
// ====================================================== Containment functions

// Transform functions ========================================================
   template gmtl::VecBase<float, 3>& xform(gmtl::VecBase<float, 3>&,
                                           const gmtl::Quat<float>&,
                                           const gmtl::VecBase<float, 3>&);
   template gmtl::VecBase<double, 3>& xform(gmtl::VecBase<double, 3>&,
                                            const gmtl::Quat<double>&,
                                            const gmtl::VecBase<double, 3>&);

   // XXX: Could probably add a gmtl::Matrix43[fd] here if that type were
   // exposed to Python as well.
   template gmtl::Vec<float, 3>& xform(gmtl::Vec<float, 3>&,
                                       const gmtl::Matrix<float, 3, 3>&,
                                       const gmtl::Vec<float, 3>&);
   template gmtl::Vec<double, 3>& xform(gmtl::Vec<double, 3>&,
                                        const gmtl::Matrix<double, 3, 3>&,
                                        const gmtl::Vec<double, 3>&);
   template gmtl::Vec<float, 4>& xform(gmtl::Vec<float, 4>&,
                                       const gmtl::Matrix<float, 4, 4>&,
                                       const gmtl::Vec<float, 4>&);
   template gmtl::Vec<double, 4>& xform(gmtl::Vec<double, 4>&,
                                        const gmtl::Matrix<double, 4, 4>&,
                                        const gmtl::Vec<double, 4>&);

   template gmtl::Vec<float, 3>& xform(gmtl::Vec<float, 3>&,
                                       const gmtl::Matrix<float, 4, 4>&,
                                       const gmtl::Vec<float, 3>&);
   template gmtl::Vec<double, 3>& xform(gmtl::Vec<double, 3>&,
                                        const gmtl::Matrix<double, 4, 4>&,
                                        const gmtl::Vec<double, 3>&);

   // XXX: Could probably add a gmtl::Matrix43[fd] here if that type were
   // exposed to Python as well.
   template gmtl::Point<float, 3>& xform(gmtl::Point<float, 3>&,
                                         const gmtl::Matrix<float, 3, 3>&,
                                         const gmtl::Point<float, 3>&);
   template gmtl::Point<double, 3>& xform(gmtl::Point<double, 3>&,
                                          const gmtl::Matrix<double, 3, 3>&,
                                          const gmtl::Point<double, 3>&);
   template gmtl::Point<float, 4>& xform(gmtl::Point<float, 4>&,
                                         const gmtl::Matrix<float, 4, 4>&,
                                         const gmtl::Point<float, 4>&);
   template gmtl::Point<double, 4>& xform(gmtl::Point<double, 4>&,
                                          const gmtl::Matrix<double, 4, 4>&,
                                          const gmtl::Point<double, 4>&);

   template gmtl::Point<float, 3>& xform(gmtl::Point<float, 3>&,
                                         const gmtl::Matrix<float, 4, 4>&,
                                         const gmtl::Point<float, 3>&);
   template gmtl::Point<double, 3>& xform(gmtl::Point<double, 3>&,
                                          const gmtl::Matrix<double, 4, 4>&,
                                          const gmtl::Point<double, 3>&);
// ======================================================== Transform functions
}


#endif /* _PYJUGGLER_GMTL_FUNCTIONS_H_ */
