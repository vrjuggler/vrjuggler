# VRJ.NET is (C) Copyright 2004 by Patrick Hartling
# Distributed under the GNU Lesser General Public License 2.1.  (See
# accompanying file COPYING.txt or http://www.gnu.org/copyleft/lesser.txt)

# File:          $RCSfile$
# Date modified: $Date$
# Version:       $Revision$

mod = 'gmtl'

# -----------------------------------------------------------------------------
# Types.
# -----------------------------------------------------------------------------

# gmtl::Matrix<> ==============================================================
# XXX: Need to add overloads for operator* and operator== 
Matrix = ReferenceTemplate(mod, 'gmtl::Matrix', 'gmtl/Matrix.h')

Matrix44f = Matrix('float 4 4', rename = 'gmtl::Matrix44f')
sealed(Matrix44f)
return_array(Matrix44f.getData, 16)
property(Matrix44f.mData)
readonly(Matrix44f.mData)

Matrix44d = Matrix('double 4 4', rename = 'gmtl::Matrix44d')
sealed(Matrix44d)
return_array(Matrix44d.getData, 16)
property(Matrix44d.mData)
readonly(Matrix44d.mData)

Matrix33f = Matrix('float 3 3', rename = 'gmtl::Matrix33f')
sealed(Matrix33f)
return_array(Matrix33f.getData, 9)
property(Matrix33f.mData)
readonly(Matrix33f.mData)

Matrix33d = Matrix('double 3 3', rename = 'gmtl::Matrix33d')
sealed(Matrix33d)
return_array(Matrix33d.getData, 9)
property(Matrix33d.mData)
readonly(Matrix33d.mData)
# ============================================================== gmtl::Matrix<>

# gmtl::Vec<> =================================================================
# XXX: The gmtl::VecBase<> instantiations cannot be renamed until Sharppy
# supports renaming without requiring existing typedefs.
VecBase = ReferenceTemplate(mod, 'gmtl::VecBase', 'gmtl/VecBase.h')

VecBase2f = VecBase('float 2')
return_array(VecBase2f.getData, 2)

VecBase2d = VecBase('double 2')
return_array(VecBase2d.getData, 2)

VecBase2i = VecBase('int 2')
return_array(VecBase2i.getData, 2)

VecBase3f = VecBase('float 3')
return_array(VecBase3f.getData, 3)

VecBase3d = VecBase('double 3')
return_array(VecBase3d.getData, 3)

VecBase3i = VecBase('int 3')
return_array(VecBase3i.getData, 3)

VecBase4f = VecBase('float 4')
return_array(VecBase4f.getData, 4)

VecBase4d = VecBase('double 4')
return_array(VecBase4d.getData, 4)

VecBase4i = VecBase('int 4')
return_array(VecBase4i.getData, 4)

Vec = ReferenceTemplate(mod, 'gmtl::Vec', 'gmtl/Vec.h')

Vec3f = Vec('float 3', rename = 'gmtl::Vec3f')
Vec3d = Vec('double 3', rename = 'gmtl::Vec3d')
Vec3i = Vec('int 3', rename = 'gmtl::Vec3i')
sealed(Vec3f)
sealed(Vec3d)
sealed(Vec3i)

Vec4f = Vec('float 4', rename = 'gmtl::Vec4f')
Vec4d = Vec('double 4', rename = 'gmtl::Vec4d')
Vec4i = Vec('int 4', rename = 'gmtl::Vec4i')
sealed(Vec4f)
sealed(Vec4d)
sealed(Vec4i)
# ================================================================= gmtl::Vec<>

# gmtl::Quat<> ================================================================
Quat = ReferenceTemplate(mod, 'gmtl::Quat', 'gmtl/Quat.h')

Quatf = Quat('float', rename = 'gmtl::Quatf')
Quatd = Quat('double', rename = 'gmtl::Quatd')
sealed(Quatf)
sealed(Quatd)
# ================================================================ gmtl::Quat<>

# gmtl::Point<> ===============================================================
Point = ReferenceTemplate(mod, 'gmtl::Point', 'gmtl/Point.h')

Point4f = Point('float 4', rename = 'gmtl::Point4f')
Point4d = Point('double 4', rename = 'gmtl::Point4d')
Point4i = Point('int 4', rename = 'gmtl::Point4i')
sealed(Point4f)
sealed(Point4d)
sealed(Point4i)

Point3f = Point('float 3', rename = 'gmtl::Point3f')
Point3d = Point('double 3', rename = 'gmtl::Point3d')
Point3i = Point('int 3', rename = 'gmtl::Point3i')
sealed(Point3f)
sealed(Point3d)
sealed(Point3i)

Point2f = Point('float 2', rename = 'gmtl::Point2f')
Point2d = Point('double 2', rename = 'gmtl::Point2d')
Point2i = Point('int 2', rename = 'gmtl::Point2i')
sealed(Point2f)
sealed(Point2d)
sealed(Point2i)
# =============================================================== gmtl::Point<>

# gmtl::EulerAngle<> ==========================================================
EulerAngle = ReferenceTemplate(mod, 'gmtl::EulerAngle', 'gmtl/EulerAngle.h')

EulerAngleXYZf = EulerAngle('float gmtl::XYZ', rename = 'gmtl::EulerAngleXYZf')
sealed(EulerAngleXYZf)
return_array(EulerAngleXYZf.getData, 3)

EulerAngleZYXf = EulerAngle('float gmtl::ZYX', rename = 'gmtl::EulerAngleZYXf')
sealed(EulerAngleZYXf)
return_array(EulerAngleZYXf.getData, 3)

EulerAngleZXYf = EulerAngle('float gmtl::ZXY', rename = 'gmtl::EulerAngleZXYf')
sealed(EulerAngleZXYf)
return_array(EulerAngleZXYf.getData, 3)

EulerAngleXYZd = EulerAngle('double gmtl::XYZ', rename = 'gmtl::EulerAngleXYZd')
sealed(EulerAngleXYZd)
return_array(EulerAngleXYZd.getData, 3)

EulerAngleZYXd = EulerAngle('double gmtl::ZYX', rename = 'gmtl::EulerAngleZYXd')
sealed(EulerAngleZYXd)
return_array(EulerAngleZYXd.getData, 3)

EulerAngleZXYd = EulerAngle('double gmtl::ZXY', rename = 'gmtl::EulerAngleZXYd')
sealed(EulerAngleZXYd)
return_array(EulerAngleZXYd.getData, 3)
# ========================================================== gmtl::EulerAngle<>

# gmtl::AxisAngle<> ===========================================================
AxisAngle = ReferenceTemplate(mod, 'gmtl::AxisAngle', 'gmtl/AxisAngle.h')

AxisAnglef = AxisAngle('float', rename = 'gmtl::AxisAnglef')
AxisAngled = AxisAngle('double', rename = 'gmtl::AxisAngled')
sealed(AxisAnglef)
sealed(AxisAngled)
# =========================================================== gmtl::AxisAngle<>

# gmtl::Sphere<> ==============================================================
Sphere = ReferenceTemplate(mod, 'gmtl::Sphere', 'gmtl/Sphere.h')

Spheref = Sphere('float', rename = 'gmtl::Spheref')
Sphered = Sphere('double', rename = 'gmtl::Sphered')
sealed(Spheref)
sealed(Sphered)
# ============================================================== gmtl::Sphere<>

# gmtl::AABox<> ===============================================================
AABox = ReferenceTemplate(mod, 'gmtl::AABox', 'gmtl/AABox.h')

AABoxf = AABox('float', rename = 'gmtl::AABoxf')
AABoxd = AABox('double', rename = 'gmtl::AABoxd')
sealed(AABoxf)
sealed(AABoxd)
# =============================================================== gmtl::AABox<>

# gmtl::Ray<> =================================================================
Ray = ReferenceTemplate(mod, 'gmtl::Ray', 'gmtl/Ray.h')

Rayf = Ray('float', rename = 'gmtl::Rayf')
Rayd = Ray('double', rename = 'gmtl::Rayd')
# ================================================================= gmtl::Ray<>

# gmtl::LineSeg<> =============================================================
LineSeg = ReferenceTemplate(mod, 'gmtl::LineSeg', 'gmtl/LineSeg.h')

LineSegf = LineSeg('float', rename = 'gmtl::LineSegf')
LineSegd = LineSeg('double', rename = 'gmtl::LineSegd')
sealed(LineSegf)
sealed(LineSegd)
# ============================================================= gmtl::LineSeg<>

# gmtl::Plane<> ===============================================================
Plane = ReferenceTemplate(mod, 'gmtl::Plane', 'gmtl/Plane.h')

Planef = Plane('float', rename = 'gmtl::Planef')
Planed = Plane('double', rename = 'gmtl::Planed')
sealed(Planef)
sealed(Planed)
# =============================================================== gmtl::Plane<>

# gmtl::Tri<> =================================================================
Tri = ReferenceTemplate(mod, 'gmtl::Tri', 'gmtl/Tri.h')

Trif = Tri('float', rename = 'gmtl::Trif')
Trid = Tri('double', rename = 'gmtl::Trid')
Trii = Tri('int', rename = 'gmtl::Trii')
sealed(Trif)
sealed(Trid)
sealed(Trii)
# ================================================================= gmtl::Tri<>

# gmtl::Coord<> ===============================================================
Coord = ReferenceTemplate(mod, 'gmtl::Coord', 'gmtl/Coord.h')

Coord3fXYZ = Coord('gmtl::Vec<float,3> gmtl::EulerAngle<float,gmtl::XYZ>',
                   ['gmtl/Vec.h', 'gmtl/EulerAngle.h'], 'gmtl::Coord3fXYZ')
Coord3fZYX = Coord('gmtl::Vec<float,3> gmtl::EulerAngle<float,gmtl::ZYX>',
                   ['gmtl/Vec.h', 'gmtl/EulerAngle.h'], 'gmtl::Coord3fZYX')
Coord3fZXY = Coord('gmtl::Vec<float,3> gmtl::EulerAngle<float,gmtl::ZXY>',
                   ['gmtl/Vec.h', 'gmtl/EulerAngle.h'], 'gmtl::Coord3fZXY')
sealed(Coord3fXYZ)
sealed(Coord3fZYX)
sealed(Coord3fZXY)

Coord3dXYZ = Coord('gmtl::Vec<double,3> gmtl::EulerAngle<double,gmtl::XYZ>',
                   ['gmtl/Vec.h', 'gmtl/EulerAngle.h'], 'gmtl::Coord3dXYZ')
Coord3dZYX = Coord('gmtl::Vec<double,3> gmtl::EulerAngle<double,gmtl::ZYX>',
                   ['gmtl/Vec.h', 'gmtl/EulerAngle.h'], 'gmtl::Coord3dZYX')
Coord3dZXY = Coord('gmtl::Vec<double,3> gmtl::EulerAngle<double,gmtl::ZXY>',
                   ['gmtl/Vec.h', 'gmtl/EulerAngle.h'], 'gmtl::Coord3dZXY')
sealed(Coord3dXYZ)
sealed(Coord3dZYX)
sealed(Coord3dZXY)

Coord4fXYZ = Coord('gmtl::Vec<float,4> gmtl::EulerAngle<float,gmtl::XYZ>',
                   ['gmtl/Vec.h', 'gmtl/EulerAngle.h'], 'gmtl::Coord4fXYZ')
Coord4fZYX = Coord('gmtl::Vec<float,4> gmtl::EulerAngle<float,gmtl::ZYX>',
                   ['gmtl/Vec.h', 'gmtl/EulerAngle.h'], 'gmtl::Coord4fZYX')
Coord4fZXY = Coord('gmtl::Vec<float,4> gmtl::EulerAngle<float,gmtl::ZXY>',
                   ['gmtl/Vec.h', 'gmtl/EulerAngle.h'], 'gmtl::Coord4fZXY')
sealed(Coord4fXYZ)
sealed(Coord4fZYX)
sealed(Coord4fZXY)

Coord4dXYZ = Coord('gmtl::Vec<double,4> gmtl::EulerAngle<double,gmtl::XYZ>',
                   ['gmtl/Vec.h', 'gmtl/EulerAngle.h'], 'gmtl::Coord4dXYZ')
Coord4dZYX = Coord('gmtl::Vec<double,4> gmtl::EulerAngle<double,gmtl::ZYX>',
                   ['gmtl/Vec.h', 'gmtl/EulerAngle.h'], 'gmtl::Coord4dZYX')
Coord4dZXY = Coord('gmtl::Vec<double,4> gmtl::EulerAngle<double,gmtl::ZXY>',
                   ['gmtl/Vec.h', 'gmtl/EulerAngle.h'], 'gmtl::Coord4dZXY')
sealed(Coord4dXYZ)
sealed(Coord4dZYX)
sealed(Coord4dZXY)

Coord3fQuat = Coord('gmtl::Vec<float,3> gmtl::Quat<float>',
                    ['gmtl/Vec.h', 'gmtl/Quat.h'], 'gmtl::Coord3fQuat')
Coord3dQuat = Coord('gmtl::Vec<double,3> gmtl::Quat<double>',
                    ['gmtl/Vec.h', 'gmtl/Quat.h'], 'gmtl::Coord3dQuat')
sealed(Coord3fQuat)
sealed(Coord3dQuat)

Coord4fQuat = Coord('gmtl::Vec<float,4> gmtl::Quat<float>',
                    ['gmtl/Vec.h', 'gmtl/Quat.h'], 'gmtl::Coord4fQuat')
Coord4dQuat = Coord('gmtl::Vec<double,4> gmtl::Quat<double>',
                    ['gmtl/Vec.h', 'gmtl/Quat.h'], 'gmtl::Coord4dQuat')
sealed(Coord4fQuat)
sealed(Coord4dQuat)

Coord3fAxisAngle = Coord('gmtl::Vec<float,3> gmtl::AxisAngle<float>',
                         ['gmtl/Vec.h', 'gmtl/AxisAngle.h'],
                         'gmtl::Coord3fAxisAngle')
Coord3dAxisAngle = Coord('gmtl::Vec<double,3> gmtl::AxisAngle<double>',
                         ['gmtl/Vec.h', 'gmtl/AxisAngle.h'],
                         'gmtl::Coord3dAxisAngle')
sealed(Coord3fAxisAngle)
sealed(Coord3dAxisAngle)

Coord4fAxisAngle = Coord('gmtl::Vec<float,4> gmtl::AxisAngle<float>',
                         ['gmtl/Vec.h', 'gmtl/AxisAngle.h'],
                         'gmtl::Coord4fAxisAngle')
Coord4dAxisAngle = Coord('gmtl::Vec<double,4> gmtl::AxisAngle<double>',
                         ['gmtl/Vec.h', 'gmtl/AxisAngle.h'],
                         'gmtl::Coord4dAxisAngle')
sealed(Coord4fAxisAngle)
sealed(Coord4dAxisAngle)
# =============================================================== gmtl::Coord<>

# -----------------------------------------------------------------------------
# Free functions.
# -----------------------------------------------------------------------------
gmtl_free_types = FreeTypesHolder(mod, 'Gmtl', 'gmtl-free-types.h')
gmtl_free_types.addEnum('gmtl::VectorIndex')
gmtl_free_types.addEnum('gmtl::PlaneSide')
gmtl_free_types.addConstant('gmtl::GMTL_EPSILON')
gmtl_free_types.addConstant('gmtl::GMTL_MAT_EQUAL_EPSILON')
gmtl_free_types.addConstant('gmtl::GMTL_VEC_EQUAL_EPSILON')

# General operations ----------------------------------------------------------
gmtl_free_types.addFunction('gmtl::add')
gmtl_free_types.addFunction('gmtl::center')
gmtl_free_types.addFunction('gmtl::conj')
gmtl_free_types.addFunction('gmtl::cross')
gmtl_free_types.addFunction('gmtl::distance')
gmtl_free_types.addFunction('gmtl::distanceSquared')
gmtl_free_types.addFunction('gmtl::div')
gmtl_free_types.addFunction('gmtl::dot')
gmtl_free_types.addFunction('gmtl::exp')
gmtl_free_types.addFunction('gmtl::findNearestPt')
gmtl_free_types.addFunction('gmtl::identity')
gmtl_free_types.addFunction('gmtl::invert')
gmtl_free_types.addFunction('gmtl::invertAffine')
gmtl_free_types.addFunction('gmtl::invertFull')
gmtl_free_types.addFunction('gmtl::invertOrthogonal')
gmtl_free_types.addFunction('gmtl::invertTrans')
gmtl_free_types.addFunction('gmtl::isEqual')
gmtl_free_types.addFunction('gmtl::isEquiv')
gmtl_free_types.addFunction('gmtl::isNormalized')
gmtl_free_types.addFunction('gmtl::length')
gmtl_free_types.addFunction('gmtl::lengthSquared')
gmtl_free_types.addFunction('gmtl::lerp')
gmtl_free_types.addFunction('gmtl::log')
gmtl_free_types.addFunction('gmtl::mult')
gmtl_free_types.addFunction('gmtl::negate')
gmtl_free_types.addFunction('gmtl::postMult')
gmtl_free_types.addFunction('gmtl::preMult')
gmtl_free_types.addFunction('gmtl::reflect')
gmtl_free_types.addFunction('gmtl::slerp')
gmtl_free_types.addFunction('gmtl::squad')
gmtl_free_types.addFunction('gmtl::sub')
gmtl_free_types.addFunction('gmtl::transpose')
gmtl_free_types.addFunction('gmtl::whichSide')
gmtl_free_types.addFunction('gmtl::zero')
# ---------------------------------------------------------- General operations

# Generator operations --------------------------------------------------------
gmtl_free_types.addFunction('gmtlWrappers::makeRotMatrix33')
gmtl_free_types.addFunction('gmtlWrappers::makeRotMatrix44')
gmtl_free_types.addFunction('gmtlWrappers::makeRotQuat')
gmtl_free_types.addFunction('gmtlWrappers::makeRotEulerAngleXYZ')
gmtl_free_types.addFunction('gmtlWrappers::makeRotEulerAngleZYX')
gmtl_free_types.addFunction('gmtlWrappers::makeRotEulerAngleZXY')

gmtl_free_types.addFunction('gmtlWrappers::makeDirCosMatrix33')
gmtl_free_types.addFunction('gmtlWrappers::makeDirCosMatrix44')

gmtl_free_types.addFunction('gmtlWrappers::makeTransMatrix44')
gmtl_free_types.addFunction('gmtlWrappers::makeTransVec3')

gmtl_free_types.addFunction('gmtlWrappers::makeScaleMatrix44')

gmtl_free_types.addFunction('gmtlWrappers::makeAxesMatrix44')

gmtl_free_types.addFunction('gmtl::setRot')
gmtl_free_types.addFunction('gmtl::makeVec')
gmtl_free_types.addFunction('gmtl::makeNormal')
gmtl_free_types.addFunction('gmtl::makeCross')
gmtl_free_types.addFunction('gmtl::setTrans')
gmtl_free_types.addFunction('gmtl::setPure')
gmtl_free_types.addFunction('gmtl::makePure')
gmtl_free_types.addFunction('gmtl::makeConj')
gmtl_free_types.addFunction('gmtl::makeInvert')
gmtl_free_types.addFunction('gmtl::set')
gmtl_free_types.addFunction('gmtl::setScale')

gmtl_free_types.addFunction('gmtl::makeYRot')
gmtl_free_types.addFunction('gmtl::makeXRot')
gmtl_free_types.addFunction('gmtl::makeZRot')
gmtl_free_types.addFunction('gmtl::setDirCos')
gmtl_free_types.addFunction('gmtl::setAxes')
gmtl_free_types.addFunction('gmtl::makeTranspose')

gmtl_free_types.addFunction('gmtl::setFrustum')
gmtl_free_types.addFunction('gmtl::setPerspective')
# -------------------------------------------------------- Generator operations

# Containment operations ------------------------------------------------------
gmtl_free_types.addFunction('gmtl::isInVolume')
gmtl_free_types.addFunction('gmtl::extendVolume')
gmtl_free_types.addFunction('gmtl::isOnVolume')
#gmtl_free_types.addFunction('gmtl::makeVolume')   # std::vector<T> param
# ------------------------------------------------------ Containment operations

# Intersection operations -----------------------------------------------------
gmtl_free_types.addFunction('gmtl::intersect')
gmtl_free_types.addFunction('gmtl::intersectVolume')
# ----------------------------------------------------- Intersection operations

# Transform operations --------------------------------------------------------
gmtl_free_types.addFunction('gmtl::xform')
# -------------------------------------------------------- Transform operations

# NOTE: The following probably duplicate existing .NET math code, so they could
# be removed without causing trouble.
gmtl_Math_funcs = FreeTypesHolder(mod, 'Math', 'gmtl-Math-functions.h')
gmtl_Math_funcs.addFunction('gmtl::Math::deg2Rad')
gmtl_Math_funcs.addFunction('gmtl::Math::rad2Deg')
gmtl_Math_funcs.addFunction('gmtl::Math::ceil')
gmtl_Math_funcs.addFunction('gmtl::Math::floor')
gmtl_Math_funcs.addFunction('gmtl::Math::seedRandom')
gmtl_Math_funcs.addFunction('gmtl::Math::unitRandom')
gmtl_Math_funcs.addFunction('gmtl::Math::rangeRandom')
