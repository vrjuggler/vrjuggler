
// Includes ====================================================================
#include <boost/python.hpp>
#include <gmtl/Quat.h>
#include <gmtl/Defines.h>
#include <gmtl/Plane.h>
#include <gmtl/LineSeg.h>
#include <gmtl/Vec.h>
#include <gmtl-functions.h>
#include <gmtl/Matrix.h>
#include <gmtl/AABox.h>
#include <gmtl/Point.h>
#include <gmtl/EulerAngle.h>
#include <gmtl/Ray.h>
#include <gmtl/Tri.h>
#include <gmtl/Sphere.h>
#include <gmtl/AxisAngle.h>

// Using =======================================================================
using namespace boost::python;

// Declarations ================================================================
namespace  {


BOOST_PYTHON_FUNCTION_OVERLOADS(makeDirCosMatrix33_overloads_3_6, gmtlWrapper::makeDirCosMatrix33, 3, 6)
BOOST_PYTHON_FUNCTION_OVERLOADS(makeDirCosMatrix44_overloads_3_6, gmtlWrapper::makeDirCosMatrix44, 3, 6)
BOOST_PYTHON_FUNCTION_OVERLOADS(setDirCos_overloads_4_7, gmtl::setDirCos, 4, 7)
BOOST_PYTHON_FUNCTION_OVERLOADS(isEqual_overloads_2_3, gmtl::isEqual, 2, 3)
BOOST_PYTHON_FUNCTION_OVERLOADS(isNormalized_overloads_1_2, gmtl::isNormalized, 1, 2)
BOOST_PYTHON_FUNCTION_OVERLOADS(slerp_overloads_4_5, gmtl::slerp, 4, 5)
BOOST_PYTHON_FUNCTION_OVERLOADS(isEquiv_overloads_2_3, gmtl::isEquiv, 2, 3)


}// namespace 


// Module ======================================================================
BOOST_PYTHON_MODULE(gmtl)
{
    def("identity", (gmtl::Matrix<double,4,4> & (*)(gmtl::Matrix<double,4,4> &))&gmtl::identity, return_internal_reference< 1 >());
    def("identity", (gmtl::Matrix<float,3,3> & (*)(gmtl::Matrix<float,3,3> &))&gmtl::identity, return_internal_reference< 1 >());
    def("identity", (gmtl::Matrix<float,4,4> & (*)(gmtl::Matrix<float,4,4> &))&gmtl::identity, return_internal_reference< 1 >());
    def("distance", (float (*)(const gmtl::Plane<float> &, const gmtl::Point<float,3> &))&gmtl::distance);
    def("distance", (double (*)(const gmtl::Plane<double> &, const gmtl::Point<double,3> &))&gmtl::distance);
    def("distance", (float (*)(const gmtl::LineSeg<float> &, const gmtl::Point<float,3> &))&gmtl::distance);
    def("distance", (double (*)(const gmtl::LineSeg<double> &, const gmtl::Point<double,3> &))&gmtl::distance);
    def("whichSide", (gmtl::PlaneSide (*)(const gmtl::Plane<float> &, const gmtl::Point<float,3> &, const float &))&gmtl::whichSide);
    def("whichSide", (gmtl::PlaneSide (*)(const gmtl::Plane<double> &, const gmtl::Point<double,3> &, const double &))&gmtl::whichSide);
    def("whichSide", (gmtl::PlaneSide (*)(const gmtl::Plane<float> &, const gmtl::Point<float,3> &))&gmtl::whichSide);
    def("whichSide", (gmtl::PlaneSide (*)(const gmtl::Plane<double> &, const gmtl::Point<double,3> &))&gmtl::whichSide);
    def("findNearestPt", (float (*)(const gmtl::Plane<float> &, const gmtl::Point<float,3> &, gmtl::Point<float,3> &))&gmtl::findNearestPt);
    def("findNearestPt", (double (*)(const gmtl::Plane<double> &, const gmtl::Point<double,3> &, gmtl::Point<double,3> &))&gmtl::findNearestPt);
    def("findNearestPt", (gmtl::Point<float,3> (*)(const gmtl::LineSeg<float> &, const gmtl::Point<float,3> &))&gmtl::findNearestPt);
    def("findNearestPt", (gmtl::Point<double,3> (*)(const gmtl::LineSeg<double> &, const gmtl::Point<double,3> &))&gmtl::findNearestPt);
    def("center", (gmtl::Point<float,3> (*)(const gmtl::Tri<float> &))&gmtl::center);
    def("center", (gmtl::Point<int,3> (*)(const gmtl::Tri<int> &))&gmtl::center);
    def("center", (gmtl::Point<double,3> (*)(const gmtl::Tri<double> &))&gmtl::center);
    def("makeRotMatrix33", (gmtl::Matrix<float,3,3> (*)(const gmtl::Quat<float> &))&gmtlWrapper::makeRotMatrix33);
    def("makeRotMatrix33", (gmtl::Matrix<double,3,3> (*)(const gmtl::Quat<double> &))&gmtlWrapper::makeRotMatrix33);
    def("makeRotMatrix33", (gmtl::Matrix<float,3,3> (*)(const gmtl::EulerAngle<float,gmtl::XYZ> &))&gmtlWrapper::makeRotMatrix33);
    def("makeRotMatrix33", (gmtl::Matrix<float,3,3> (*)(const gmtl::EulerAngle<float,gmtl::ZYX> &))&gmtlWrapper::makeRotMatrix33);
    def("makeRotMatrix33", (gmtl::Matrix<float,3,3> (*)(const gmtl::EulerAngle<float,gmtl::ZXY> &))&gmtlWrapper::makeRotMatrix33);
    def("makeRotMatrix44", (gmtl::Matrix<float,4,4> (*)(const gmtl::Quat<float> &))&gmtlWrapper::makeRotMatrix44);
    def("makeRotMatrix44", (gmtl::Matrix<double,4,4> (*)(const gmtl::Quat<double> &))&gmtlWrapper::makeRotMatrix44);
    def("makeRotMatrix44", (gmtl::Matrix<float,4,4> (*)(const gmtl::EulerAngle<float,gmtl::XYZ> &))&gmtlWrapper::makeRotMatrix44);
    def("makeRotMatrix44", (gmtl::Matrix<float,4,4> (*)(const gmtl::EulerAngle<float,gmtl::ZYX> &))&gmtlWrapper::makeRotMatrix44);
    def("makeRotMatrix44", (gmtl::Matrix<float,4,4> (*)(const gmtl::EulerAngle<float,gmtl::ZXY> &))&gmtlWrapper::makeRotMatrix44);
    def("makeRotMatrix44", (gmtl::Matrix<float,4,4> (*)(const gmtl::Vec<float,3> &, const gmtl::Vec<float,3> &))&gmtlWrapper::makeRotMatrix44);
    def("makeRotQuat", (gmtl::Quat<double> (*)(const gmtl::Matrix<double,4,4> &))&gmtlWrapper::makeRotQuat);
    def("makeRotQuat", (gmtl::Quat<float> (*)(const gmtl::Matrix<float,4,4> &))&gmtlWrapper::makeRotQuat);
    def("makeRotQuat", (gmtl::Quat<double> (*)(const gmtl::Vec<double,3> &, const gmtl::Vec<double,3> &))&gmtlWrapper::makeRotQuat);
    def("makeRotQuat", (gmtl::Quat<float> (*)(const gmtl::Vec<float,3> &, const gmtl::Vec<float,3> &))&gmtlWrapper::makeRotQuat);
    def("makeRotEulerAngleXYZ", (gmtl::EulerAngle<float,gmtl::XYZ> (*)(const gmtl::Matrix<float,4,4> &))&gmtlWrapper::makeRotEulerAngleXYZ);
    def("makeRotEulerAngleXYZ", (gmtl::EulerAngle<double,gmtl::XYZ> (*)(const gmtl::Matrix<double,4,4> &))&gmtlWrapper::makeRotEulerAngleXYZ);
    def("makeRotEulerAngleZYX", (gmtl::EulerAngle<float,gmtl::ZYX> (*)(const gmtl::Matrix<float,4,4> &))&gmtlWrapper::makeRotEulerAngleZYX);
    def("makeRotEulerAngleZYX", (gmtl::EulerAngle<double,gmtl::ZYX> (*)(const gmtl::Matrix<double,4,4> &))&gmtlWrapper::makeRotEulerAngleZYX);
    def("makeRotEulerAngleZXY", (gmtl::EulerAngle<float,gmtl::ZXY> (*)(const gmtl::Matrix<float,4,4> &))&gmtlWrapper::makeRotEulerAngleZXY);
    def("makeRotEulerAngleZXY", (gmtl::EulerAngle<double,gmtl::ZXY> (*)(const gmtl::Matrix<double,4,4> &))&gmtlWrapper::makeRotEulerAngleZXY);
    def("makeDirCosMatrix33", (gmtl::Matrix<double,3,3> (*)(const gmtl::Vec<double,3> &, const gmtl::Vec<double,3> &, const gmtl::Vec<double,3> &, const gmtl::Vec<double,3> &, const gmtl::Vec<double,3> &, const gmtl::Vec<double,3> &))&gmtlWrapper::makeDirCosMatrix33, makeDirCosMatrix33_overloads_3_6());
    def("makeDirCosMatrix33", (gmtl::Matrix<float,3,3> (*)(const gmtl::Vec<float,3> &, const gmtl::Vec<float,3> &, const gmtl::Vec<float,3> &, const gmtl::Vec<float,3> &, const gmtl::Vec<float,3> &, const gmtl::Vec<float,3> &))&gmtlWrapper::makeDirCosMatrix33, makeDirCosMatrix33_overloads_3_6());
    def("makeDirCosMatrix44", (gmtl::Matrix<float,4,4> (*)(const gmtl::Vec<float,3> &, const gmtl::Vec<float,3> &, const gmtl::Vec<float,3> &, const gmtl::Vec<float,3> &, const gmtl::Vec<float,3> &, const gmtl::Vec<float,3> &))&gmtlWrapper::makeDirCosMatrix44, makeDirCosMatrix44_overloads_3_6());
    def("makeDirCosMatrix44", (gmtl::Matrix<double,4,4> (*)(const gmtl::Vec<double,3> &, const gmtl::Vec<double,3> &, const gmtl::Vec<double,3> &, const gmtl::Vec<double,3> &, const gmtl::Vec<double,3> &, const gmtl::Vec<double,3> &))&gmtlWrapper::makeDirCosMatrix44, makeDirCosMatrix44_overloads_3_6());
    def("makeTransMatrix44", (gmtl::Matrix<float,4,4> (*)(const gmtl::Vec<float,3> &))&gmtlWrapper::makeTransMatrix44);
    def("makeTransMatrix44", (gmtl::Matrix<double,4,4> (*)(const gmtl::Vec<double,3> &))&gmtlWrapper::makeTransMatrix44);
    def("makeTransVec3", (gmtl::Vec<float,3> (*)(const gmtl::Matrix<float,4,4> &))&gmtlWrapper::makeTransVec3);
    def("makeTransVec3", (gmtl::Vec<double,3> (*)(const gmtl::Matrix<double,4,4> &))&gmtlWrapper::makeTransVec3);
    def("makeScaleMatrix44", (gmtl::Matrix<float,4,4> (*)(const gmtl::Vec<float,3> &))&gmtlWrapper::makeScaleMatrix44);
    def("makeScaleMatrix44", (gmtl::Matrix<double,4,4> (*)(const gmtl::Vec<double,3> &))&gmtlWrapper::makeScaleMatrix44);
    def("makeScaleMatrix44", (gmtl::Matrix<float,4,4> (*)(const gmtl::Vec<float,4> &))&gmtlWrapper::makeScaleMatrix44);
    def("makeScaleMatrix44", (gmtl::Matrix<double,4,4> (*)(const gmtl::Vec<double,4> &))&gmtlWrapper::makeScaleMatrix44);
    def("makeScaleMatrix44", (gmtl::Matrix<double,4,4> (*)(const double))&gmtlWrapper::makeScaleMatrix44);
    def("makeScaleMatrix44", (gmtl::Matrix<float,4,4> (*)(const float))&gmtlWrapper::makeScaleMatrix44);
    def("makeAxesMatrix44", (gmtl::Matrix<double,4,4> (*)(const gmtl::Vec<double,3> &, const gmtl::Vec<double,3> &, const gmtl::Vec<double,3> &))&gmtlWrapper::makeAxesMatrix44);
    def("makeAxesMatrix44", (gmtl::Matrix<float,4,4> (*)(const gmtl::Vec<float,3> &, const gmtl::Vec<float,3> &, const gmtl::Vec<float,3> &))&gmtlWrapper::makeAxesMatrix44);
    def("setRot", (gmtl::Matrix<float,3,3> & (*)(gmtl::Matrix<float,3,3> &, const gmtl::EulerAngle<float,gmtl::ZXY> &))&gmtl::setRot, return_internal_reference< 1 >());
    def("setRot", (gmtl::Matrix<float,4,4> & (*)(gmtl::Matrix<float,4,4> &, const gmtl::Quat<float> &))&gmtl::setRot, return_internal_reference< 1 >());
    def("setRot", (gmtl::Matrix<float,3,3> & (*)(gmtl::Matrix<float,3,3> &, const gmtl::Quat<float> &))&gmtl::setRot, return_internal_reference< 1 >());
    def("setRot", (gmtl::Matrix<double,4,4> & (*)(gmtl::Matrix<double,4,4> &, const gmtl::AxisAngle<double> &))&gmtl::setRot, return_internal_reference< 1 >());
    def("setRot", (gmtl::Matrix<double,4,4> & (*)(gmtl::Matrix<double,4,4> &, const gmtl::Quat<double> &))&gmtl::setRot, return_internal_reference< 1 >());
    def("setRot", (gmtl::Matrix<double,3,3> & (*)(gmtl::Matrix<double,3,3> &, const gmtl::Quat<double> &))&gmtl::setRot, return_internal_reference< 1 >());
    def("setRot", (gmtl::Matrix<float,4,4> & (*)(gmtl::Matrix<float,4,4> &, const gmtl::AxisAngle<float> &))&gmtl::setRot, return_internal_reference< 1 >());
    def("setRot", (gmtl::AxisAngle<float> & (*)(gmtl::AxisAngle<float> &, gmtl::Quat<float>))&gmtl::setRot, return_internal_reference< 1 >());
    def("setRot", (gmtl::Quat<double> & (*)(gmtl::Quat<double> &, const gmtl::Matrix<double,4,4> &))&gmtl::setRot, return_internal_reference< 1 >());
    def("setRot", (gmtl::Quat<float> & (*)(gmtl::Quat<float> &, const gmtl::Matrix<float,4,4> &))&gmtl::setRot, return_internal_reference< 1 >());
    def("setRot", (gmtl::Quat<float> & (*)(gmtl::Quat<float> &, const gmtl::EulerAngle<float,gmtl::ZXY> &))&gmtl::setRot, return_internal_reference< 1 >());
    def("setRot", (gmtl::Quat<float> & (*)(gmtl::Quat<float> &, const gmtl::EulerAngle<float,gmtl::XYZ> &))&gmtl::setRot, return_internal_reference< 1 >());
    def("setRot", (gmtl::Quat<float> & (*)(gmtl::Quat<float> &, const gmtl::EulerAngle<float,gmtl::ZYX> &))&gmtl::setRot, return_internal_reference< 1 >());
    def("setRot", (gmtl::Quat<double> & (*)(gmtl::Quat<double> &, const gmtl::AxisAngle<double> &))&gmtl::setRot, return_internal_reference< 1 >());
    def("setRot", (gmtl::Quat<float> & (*)(gmtl::Quat<float> &, const gmtl::AxisAngle<float> &))&gmtl::setRot, return_internal_reference< 1 >());
    def("setRot", (gmtl::Quat<double> & (*)(gmtl::Quat<double> &, const gmtl::Vec<double,3> &, const gmtl::Vec<double,3> &))&gmtl::setRot, return_internal_reference< 1 >());
    def("setRot", (gmtl::Quat<float> & (*)(gmtl::Quat<float> &, const gmtl::Vec<float,3> &, const gmtl::Vec<float,3> &))&gmtl::setRot, return_internal_reference< 1 >());
    def("setRot", (gmtl::Matrix<float,4,4> & (*)(gmtl::Matrix<float,4,4> &, const gmtl::Vec<float,3> &, const gmtl::Vec<float,3> &))&gmtl::setRot, return_internal_reference< 1 >());
    def("setRot", (gmtl::EulerAngle<float,gmtl::XYZ> & (*)(gmtl::EulerAngle<float,gmtl::XYZ> &, const gmtl::Matrix<float,4,4> &))&gmtl::setRot, return_internal_reference< 1 >());
    def("setRot", (gmtl::AxisAngle<double> & (*)(gmtl::AxisAngle<double> &, gmtl::Quat<double>))&gmtl::setRot, return_internal_reference< 1 >());
    def("setRot", (gmtl::EulerAngle<float,gmtl::ZXY> & (*)(gmtl::EulerAngle<float,gmtl::ZXY> &, const gmtl::Matrix<float,4,4> &))&gmtl::setRot, return_internal_reference< 1 >());
    def("setRot", (gmtl::EulerAngle<float,gmtl::ZYX> & (*)(gmtl::EulerAngle<float,gmtl::ZYX> &, const gmtl::Matrix<float,4,4> &))&gmtl::setRot, return_internal_reference< 1 >());
    def("setRot", (gmtl::Matrix<float,4,4> & (*)(gmtl::Matrix<float,4,4> &, const gmtl::EulerAngle<float,gmtl::ZYX> &))&gmtl::setRot, return_internal_reference< 1 >());
    def("setRot", (gmtl::Matrix<float,4,4> & (*)(gmtl::Matrix<float,4,4> &, const gmtl::EulerAngle<float,gmtl::XYZ> &))&gmtl::setRot, return_internal_reference< 1 >());
    def("setRot", (gmtl::Matrix<double,4,4> & (*)(gmtl::Matrix<double,4,4> &, const gmtl::EulerAngle<double,gmtl::XYZ> &))&gmtl::setRot, return_internal_reference< 1 >());
    def("setRot", (gmtl::Matrix<float,4,4> & (*)(gmtl::Matrix<float,4,4> &, const gmtl::EulerAngle<float,gmtl::ZXY> &))&gmtl::setRot, return_internal_reference< 1 >());
    def("setRot", (gmtl::Matrix<double,4,4> & (*)(gmtl::Matrix<double,4,4> &, const gmtl::EulerAngle<double,gmtl::ZXY> &))&gmtl::setRot, return_internal_reference< 1 >());
    def("setRot", (gmtl::Matrix<double,4,4> & (*)(gmtl::Matrix<double,4,4> &, const gmtl::EulerAngle<double,gmtl::ZYX> &))&gmtl::setRot, return_internal_reference< 1 >());
    def("setRot", (gmtl::Matrix<float,3,3> & (*)(gmtl::Matrix<float,3,3> &, const gmtl::EulerAngle<float,gmtl::ZYX> &))&gmtl::setRot, return_internal_reference< 1 >());
    def("setRot", (gmtl::Matrix<float,3,3> & (*)(gmtl::Matrix<float,3,3> &, const gmtl::EulerAngle<float,gmtl::XYZ> &))&gmtl::setRot, return_internal_reference< 1 >());
    def("makeVec", (gmtl::Vec<double,3> (*)(const gmtl::Quat<double> &))&gmtl::makeVec);
    def("makeVec", (gmtl::Vec<float,3> (*)(const gmtl::Quat<float> &))&gmtl::makeVec);
    def("makeNormal", (gmtl::Vec<double,3> (*)(gmtl::Vec<double,3>))&gmtl::makeNormal);
    def("makeNormal", (gmtl::Quat<float> (*)(const gmtl::Quat<float> &))&gmtl::makeNormal);
    def("makeNormal", (gmtl::Quat<double> (*)(const gmtl::Quat<double> &))&gmtl::makeNormal);
    def("makeNormal", (gmtl::Vec<float,4> (*)(gmtl::Vec<float,4>))&gmtl::makeNormal);
    def("makeNormal", (gmtl::Vec<float,3> (*)(gmtl::Vec<float,3>))&gmtl::makeNormal);
    def("makeNormal", (gmtl::AxisAngle<double> (*)(const gmtl::AxisAngle<double> &))&gmtl::makeNormal);
    def("makeNormal", (gmtl::AxisAngle<float> (*)(const gmtl::AxisAngle<float> &))&gmtl::makeNormal);
    def("setTrans", (gmtl::Vec<float,3> & (*)(gmtl::Vec<float,3> &, const gmtl::Matrix<float,4,4> &))&gmtl::setTrans, return_internal_reference< 1 >());
    def("setTrans", (gmtl::Vec<double,3> & (*)(gmtl::Vec<double,3> &, const gmtl::Matrix<double,4,4> &))&gmtl::setTrans, return_internal_reference< 1 >());
    def("setTrans", (gmtl::Matrix<double,4,4> & (*)(gmtl::Matrix<double,4,4> &, const gmtl::Vec<double,3> &))&gmtl::setTrans, return_internal_reference< 1 >());
    def("setTrans", (gmtl::Matrix<float,4,4> & (*)(gmtl::Matrix<float,4,4> &, const gmtl::Vec<float,3> &))&gmtl::setTrans, return_internal_reference< 1 >());
    def("setPure", (gmtl::Quat<float> & (*)(gmtl::Quat<float> &, const gmtl::Vec<float,3> &))&gmtl::setPure, return_internal_reference< 1 >());
    def("setPure", (gmtl::Quat<double> & (*)(gmtl::Quat<double> &, const gmtl::Vec<double,3> &))&gmtl::setPure, return_internal_reference< 1 >());
    def("makePure", (gmtl::Quat<float> (*)(const gmtl::Vec<float,3> &))&gmtl::makePure);
    def("makePure", (gmtl::Quat<double> (*)(const gmtl::Vec<double,3> &))&gmtl::makePure);
    def("makeConj", (gmtl::Quat<float> (*)(const gmtl::Quat<float> &))&gmtl::makeConj);
    def("makeConj", (gmtl::Quat<double> (*)(const gmtl::Quat<double> &))&gmtl::makeConj);
    def("makeInvert", (gmtl::Quat<double> (*)(const gmtl::Quat<double> &))&gmtl::makeInvert);
    def("makeInvert", (gmtl::Quat<float> (*)(const gmtl::Quat<float> &))&gmtl::makeInvert);
    def("set", (gmtl::Matrix<float,3,3> & (*)(gmtl::Matrix<float,3,3> &, const gmtl::Quat<float> &))&gmtl::set, return_internal_reference< 1 >());
    def("set", (gmtl::Matrix<float,4,4> & (*)(gmtl::Matrix<float,4,4> &, const gmtl::AxisAngle<float> &))&gmtl::set, return_internal_reference< 1 >());
    def("set", (gmtl::Matrix<double,4,4> & (*)(gmtl::Matrix<double,4,4> &, const gmtl::AxisAngle<double> &))&gmtl::set, return_internal_reference< 1 >());
    def("set", (gmtl::AxisAngle<double> & (*)(gmtl::AxisAngle<double> &, gmtl::Quat<double>))&gmtl::set, return_internal_reference< 1 >());
    def("set", (gmtl::AxisAngle<float> & (*)(gmtl::AxisAngle<float> &, gmtl::Quat<float>))&gmtl::set, return_internal_reference< 1 >());
    def("set", (gmtl::Quat<double> & (*)(gmtl::Quat<double> &, const gmtl::Matrix<double,4,4> &))&gmtl::set, return_internal_reference< 1 >());
    def("set", (gmtl::Quat<float> & (*)(gmtl::Quat<float> &, const gmtl::Matrix<float,4,4> &))&gmtl::set, return_internal_reference< 1 >());
    def("set", (gmtl::Quat<float> & (*)(gmtl::Quat<float> &, const gmtl::EulerAngle<float,gmtl::XYZ> &))&gmtl::set, return_internal_reference< 1 >());
    def("set", (gmtl::Quat<float> & (*)(gmtl::Quat<float> &, const gmtl::EulerAngle<float,gmtl::ZXY> &))&gmtl::set, return_internal_reference< 1 >());
    def("set", (gmtl::Quat<float> & (*)(gmtl::Quat<float> &, const gmtl::EulerAngle<float,gmtl::ZYX> &))&gmtl::set, return_internal_reference< 1 >());
    def("set", (gmtl::Quat<float> & (*)(gmtl::Quat<float> &, const gmtl::AxisAngle<float> &))&gmtl::set, return_internal_reference< 1 >());
    def("set", (gmtl::Quat<double> & (*)(gmtl::Quat<double> &, const gmtl::AxisAngle<double> &))&gmtl::set, return_internal_reference< 1 >());
    def("set", (gmtl::Matrix<float,4,4> & (*)(gmtl::Matrix<float,4,4> &, const gmtl::Quat<float> &))&gmtl::set, return_internal_reference< 1 >());
    def("set", (gmtl::Matrix<double,3,3> & (*)(gmtl::Matrix<double,3,3> &, const gmtl::Quat<double> &))&gmtl::set, return_internal_reference< 1 >());
    def("set", (gmtl::Matrix<double,4,4> & (*)(gmtl::Matrix<double,4,4> &, const gmtl::Quat<double> &))&gmtl::set, return_internal_reference< 1 >());
    def("set", (gmtl::EulerAngle<float,gmtl::XYZ> & (*)(gmtl::EulerAngle<float,gmtl::XYZ> &, const gmtl::Matrix<float,4,4> &))&gmtl::set, return_internal_reference< 1 >());
    def("set", (gmtl::EulerAngle<float,gmtl::ZYX> & (*)(gmtl::EulerAngle<float,gmtl::ZYX> &, const gmtl::Matrix<float,4,4> &))&gmtl::set, return_internal_reference< 1 >());
    def("set", (gmtl::EulerAngle<float,gmtl::ZXY> & (*)(gmtl::EulerAngle<float,gmtl::ZXY> &, const gmtl::Matrix<float,4,4> &))&gmtl::set, return_internal_reference< 1 >());
    def("set", (gmtl::EulerAngle<double,gmtl::XYZ> & (*)(gmtl::EulerAngle<double,gmtl::XYZ> &, const gmtl::Matrix<double,4,4> &))&gmtl::set, return_internal_reference< 1 >());
    def("set", (gmtl::EulerAngle<double,gmtl::ZYX> & (*)(gmtl::EulerAngle<double,gmtl::ZYX> &, const gmtl::Matrix<double,4,4> &))&gmtl::set, return_internal_reference< 1 >());
    def("set", (gmtl::EulerAngle<double,gmtl::ZXY> & (*)(gmtl::EulerAngle<double,gmtl::ZXY> &, const gmtl::Matrix<double,4,4> &))&gmtl::set, return_internal_reference< 1 >());
    def("set", (gmtl::Matrix<float,4,4> & (*)(gmtl::Matrix<float,4,4> &, const gmtl::EulerAngle<float,gmtl::XYZ> &))&gmtl::set, return_internal_reference< 1 >());
    def("set", (gmtl::Matrix<float,4,4> & (*)(gmtl::Matrix<float,4,4> &, const gmtl::EulerAngle<float,gmtl::ZYX> &))&gmtl::set, return_internal_reference< 1 >());
    def("set", (gmtl::Matrix<float,4,4> & (*)(gmtl::Matrix<float,4,4> &, const gmtl::EulerAngle<float,gmtl::ZXY> &))&gmtl::set, return_internal_reference< 1 >());
    def("set", (gmtl::Matrix<float,3,3> & (*)(gmtl::Matrix<float,3,3> &, const gmtl::EulerAngle<float,gmtl::ZYX> &))&gmtl::set, return_internal_reference< 1 >());
    def("set", (gmtl::Matrix<float,3,3> & (*)(gmtl::Matrix<float,3,3> &, const gmtl::EulerAngle<float,gmtl::ZXY> &))&gmtl::set, return_internal_reference< 1 >());
    def("set", (gmtl::Matrix<double,4,4> & (*)(gmtl::Matrix<double,4,4> &, const gmtl::EulerAngle<double,gmtl::XYZ> &))&gmtl::set, return_internal_reference< 1 >());
    def("set", (gmtl::Matrix<double,4,4> & (*)(gmtl::Matrix<double,4,4> &, const gmtl::EulerAngle<double,gmtl::ZYX> &))&gmtl::set, return_internal_reference< 1 >());
    def("set", (gmtl::Matrix<double,4,4> & (*)(gmtl::Matrix<double,4,4> &, const gmtl::EulerAngle<double,gmtl::ZXY> &))&gmtl::set, return_internal_reference< 1 >());
    def("set", (gmtl::Matrix<float,3,3> & (*)(gmtl::Matrix<float,3,3> &, const gmtl::EulerAngle<float,gmtl::XYZ> &))&gmtl::set, return_internal_reference< 1 >());
    def("setScale", (gmtl::Matrix<double,4,4> & (*)(gmtl::Matrix<double,4,4> &, const double))&gmtl::setScale, return_internal_reference< 1 >());
    def("setScale", (gmtl::Matrix<double,4,4> & (*)(gmtl::Matrix<double,4,4> &, const gmtl::Vec<double,4> &))&gmtl::setScale, return_internal_reference< 1 >());
    def("setScale", (gmtl::Matrix<float,4,4> & (*)(gmtl::Matrix<float,4,4> &, const gmtl::Vec<float,4> &))&gmtl::setScale, return_internal_reference< 1 >());
    def("setScale", (gmtl::Matrix<double,4,4> & (*)(gmtl::Matrix<double,4,4> &, const gmtl::Vec<double,3> &))&gmtl::setScale, return_internal_reference< 1 >());
    def("setScale", (gmtl::Matrix<float,4,4> & (*)(gmtl::Matrix<float,4,4> &, const gmtl::Vec<float,3> &))&gmtl::setScale, return_internal_reference< 1 >());
    def("setScale", (gmtl::Matrix<float,4,4> & (*)(gmtl::Matrix<float,4,4> &, const float))&gmtl::setScale, return_internal_reference< 1 >());
    def("makeYRot", (double (*)(const gmtl::Matrix<double,4,4> &))&gmtl::makeYRot);
    def("makeYRot", (double (*)(const gmtl::Matrix<double,3,3> &))&gmtl::makeYRot);
    def("makeYRot", (float (*)(const gmtl::Matrix<float,3,3> &))&gmtl::makeYRot);
    def("makeYRot", (float (*)(const gmtl::Matrix<float,4,4> &))&gmtl::makeYRot);
    def("makeXRot", (double (*)(const gmtl::Matrix<double,4,4> &))&gmtl::makeXRot);
    def("makeXRot", (double (*)(const gmtl::Matrix<double,3,3> &))&gmtl::makeXRot);
    def("makeXRot", (float (*)(const gmtl::Matrix<float,4,4> &))&gmtl::makeXRot);
    def("makeXRot", (float (*)(const gmtl::Matrix<float,3,3> &))&gmtl::makeXRot);
    def("makeZRot", (double (*)(const gmtl::Matrix<double,4,4> &))&gmtl::makeZRot);
    def("makeZRot", (double (*)(const gmtl::Matrix<double,3,3> &))&gmtl::makeZRot);
    def("makeZRot", (float (*)(const gmtl::Matrix<float,4,4> &))&gmtl::makeZRot);
    def("makeZRot", (float (*)(const gmtl::Matrix<float,3,3> &))&gmtl::makeZRot);
    def("setDirCos", (gmtl::Matrix<float,4,4> & (*)(gmtl::Matrix<float,4,4> &, const gmtl::Vec<float,3> &, const gmtl::Vec<float,3> &, const gmtl::Vec<float,3> &, const gmtl::Vec<float,3> &, const gmtl::Vec<float,3> &, const gmtl::Vec<float,3> &))&gmtl::setDirCos, return_internal_reference< 1 >(), setDirCos_overloads_4_7());
    def("setDirCos", (gmtl::Matrix<double,4,4> & (*)(gmtl::Matrix<double,4,4> &, const gmtl::Vec<double,3> &, const gmtl::Vec<double,3> &, const gmtl::Vec<double,3> &, const gmtl::Vec<double,3> &, const gmtl::Vec<double,3> &, const gmtl::Vec<double,3> &))&gmtl::setDirCos, return_internal_reference< 1 >(), setDirCos_overloads_4_7());
    def("setDirCos", (gmtl::Matrix<float,3,3> & (*)(gmtl::Matrix<float,3,3> &, const gmtl::Vec<float,3> &, const gmtl::Vec<float,3> &, const gmtl::Vec<float,3> &, const gmtl::Vec<float,3> &, const gmtl::Vec<float,3> &, const gmtl::Vec<float,3> &))&gmtl::setDirCos, return_internal_reference< 1 >(), setDirCos_overloads_4_7());
    def("setDirCos", (gmtl::Matrix<double,3,3> & (*)(gmtl::Matrix<double,3,3> &, const gmtl::Vec<double,3> &, const gmtl::Vec<double,3> &, const gmtl::Vec<double,3> &, const gmtl::Vec<double,3> &, const gmtl::Vec<double,3> &, const gmtl::Vec<double,3> &))&gmtl::setDirCos, return_internal_reference< 1 >(), setDirCos_overloads_4_7());
    def("setAxes", (gmtl::Matrix<float,4,4> & (*)(gmtl::Matrix<float,4,4> &, const gmtl::Vec<float,3> &, const gmtl::Vec<float,3> &, const gmtl::Vec<float,3> &))&gmtl::setAxes, return_internal_reference< 1 >());
    def("setAxes", (gmtl::Matrix<double,4,4> & (*)(gmtl::Matrix<double,4,4> &, const gmtl::Vec<double,3> &, const gmtl::Vec<double,3> &, const gmtl::Vec<double,3> &))&gmtl::setAxes, return_internal_reference< 1 >());
    def("makeTranspose", (gmtl::Matrix<double,4,4> (*)(const gmtl::Matrix<double,4,4> &))&gmtl::makeTranspose);
    def("makeTranspose", (gmtl::Matrix<double,3,3> (*)(const gmtl::Matrix<double,3,3> &))&gmtl::makeTranspose);
    def("makeTranspose", (gmtl::Matrix<float,3,3> (*)(const gmtl::Matrix<float,3,3> &))&gmtl::makeTranspose);
    def("makeTranspose", (gmtl::Matrix<float,4,4> (*)(const gmtl::Matrix<float,4,4> &))&gmtl::makeTranspose);
    def("makeInverse", (gmtl::Matrix<double,4,4> (*)(const gmtl::Matrix<double,4,4> &))&gmtl::makeInverse);
    def("makeInverse", (gmtl::Matrix<double,3,3> (*)(const gmtl::Matrix<double,3,3> &))&gmtl::makeInverse);
    def("makeInverse", (gmtl::Matrix<float,3,3> (*)(const gmtl::Matrix<float,3,3> &))&gmtl::makeInverse);
    def("makeInverse", (gmtl::Matrix<float,4,4> (*)(const gmtl::Matrix<float,4,4> &))&gmtl::makeInverse);
    def("isInVolume", (bool (*)(const gmtl::Sphere<double> &, const gmtl::Sphere<double> &))&gmtl::isInVolume);
    def("isInVolume", (bool (*)(const gmtl::Sphere<float> &, const gmtl::Sphere<float> &))&gmtl::isInVolume);
    def("isInVolume", (bool (*)(const gmtl::Sphere<double> &, const gmtl::Point<double,3> &))&gmtl::isInVolume);
    def("isInVolume", (bool (*)(const gmtl::Sphere<float> &, const gmtl::Point<float,3> &))&gmtl::isInVolume);
    def("extendVolume", (void (*)(gmtl::AABox<float> &, const gmtl::Point<float,3> &))&gmtl::extendVolume, return_internal_reference< 1 >());
    def("extendVolume", (void (*)(gmtl::AABox<double> &, const gmtl::Point<double,3> &))&gmtl::extendVolume, return_internal_reference< 1 >());
    def("extendVolume", (void (*)(gmtl::AABox<float> &, const gmtl::AABox<float> &))&gmtl::extendVolume, return_internal_reference< 1 >());
    def("extendVolume", (void (*)(gmtl::AABox<double> &, const gmtl::AABox<double> &))&gmtl::extendVolume, return_internal_reference< 1 >());
    def("extendVolume", (void (*)(gmtl::Sphere<float> &, const gmtl::Sphere<float> &))&gmtl::extendVolume, return_internal_reference< 1 >());
    def("extendVolume", (void (*)(gmtl::Sphere<double> &, const gmtl::Sphere<double> &))&gmtl::extendVolume, return_internal_reference< 1 >());
    def("extendVolume", (void (*)(gmtl::Sphere<double> &, const gmtl::Point<double,3> &))&gmtl::extendVolume, return_internal_reference< 1 >());
    def("extendVolume", (void (*)(gmtl::Sphere<float> &, const gmtl::Point<float,3> &))&gmtl::extendVolume, return_internal_reference< 1 >());
    def("isOnVolume", (bool (*)(const gmtl::Sphere<float> &, const gmtl::Point<float,3> &, const float &))&gmtl::isOnVolume);
    def("isOnVolume", (bool (*)(const gmtl::Sphere<double> &, const gmtl::Point<double,3> &, const double &))&gmtl::isOnVolume);
    def("makeVolume", (void (*)(gmtl::Sphere<float> &, const std::vector<gmtl::Point<float, 3>,std::allocator<gmtl::Point<float, 3> > > &))&gmtl::makeVolume, return_internal_reference< 1 >());
    def("makeVolume", (void (*)(gmtl::Sphere<double> &, const std::vector<gmtl::Point<double, 3>,std::allocator<gmtl::Point<double, 3> > > &))&gmtl::makeVolume, return_internal_reference< 1 >());
    def("makeVolume", (void (*)(gmtl::AABox<double> &, const gmtl::Sphere<double> &))&gmtl::makeVolume, return_internal_reference< 1 >());
    def("makeVolume", (void (*)(gmtl::AABox<float> &, const gmtl::Sphere<float> &))&gmtl::makeVolume, return_internal_reference< 1 >());
    def("intersect", (bool (*)(const gmtl::AABox<double> &, const gmtl::Vec<double,3> &, const gmtl::AABox<double> &, const gmtl::Vec<double,3> &, double &, double &))&gmtl::intersect);
    def("intersect", (bool (*)(const gmtl::Tri<double> &, const gmtl::LineSeg<double> &, float &, float &, float &))&gmtl::intersect);
    def("intersect", (bool (*)(const gmtl::Tri<float> &, const gmtl::LineSeg<float> &, float &, float &, float &))&gmtl::intersect);
    def("intersect", (bool (*)(const gmtl::Tri<double> &, const gmtl::Ray<double> &, float &, float &, float &))&gmtl::intersect);
    def("intersect", (bool (*)(const gmtl::Tri<float> &, const gmtl::Ray<float> &, float &, float &, float &))&gmtl::intersect);
    def("intersect", (bool (*)(const gmtl::Sphere<double> &, const gmtl::LineSeg<double> &, int &, float &, float &))&gmtl::intersect);
    def("intersect", (bool (*)(const gmtl::Sphere<float> &, const gmtl::LineSeg<float> &, int &, float &, float &))&gmtl::intersect);
    def("intersect", (bool (*)(const gmtl::AABox<float> &, const gmtl::AABox<float> &))&gmtl::intersect);
    def("intersect", (bool (*)(const gmtl::AABox<double> &, const gmtl::Point<double,3> &))&gmtl::intersect);
    def("intersect", (bool (*)(const gmtl::Sphere<double> &, const gmtl::Vec<double,3> &, const gmtl::Sphere<double> &, const gmtl::Vec<double,3> &, double &, double &))&gmtl::intersect);
    def("intersect", (bool (*)(const gmtl::Sphere<float> &, const gmtl::Vec<float,3> &, const gmtl::Sphere<float> &, const gmtl::Vec<float,3> &, float &, float &))&gmtl::intersect);
    def("intersect", (bool (*)(const gmtl::Sphere<double> &, const gmtl::Ray<double> &, int &, float &, float &))&gmtl::intersect);
    def("intersect", (bool (*)(const gmtl::Sphere<float> &, const gmtl::Ray<float> &, int &, float &, float &))&gmtl::intersect);
    def("intersect", (bool (*)(const gmtl::Sphere<double> &, const gmtl::Point<double,3> &))&gmtl::intersect);
    def("intersect", (bool (*)(const gmtl::Sphere<float> &, const gmtl::Point<float,3> &))&gmtl::intersect);
    def("intersect", (bool (*)(const gmtl::Sphere<double> &, const gmtl::AABox<double> &))&gmtl::intersect);
    def("intersect", (bool (*)(const gmtl::Sphere<float> &, const gmtl::AABox<float> &))&gmtl::intersect);
    def("intersect", (bool (*)(const gmtl::AABox<double> &, const gmtl::Sphere<double> &))&gmtl::intersect);
    def("intersect", (bool (*)(const gmtl::Plane<double> &, const gmtl::Ray<double> &, double &))&gmtl::intersect);
    def("intersect", (bool (*)(const gmtl::Plane<float> &, const gmtl::Ray<float> &, float &))&gmtl::intersect);
    def("intersect", (bool (*)(const gmtl::AABox<double> &, const gmtl::AABox<double> &))&gmtl::intersect);
    def("intersect", (bool (*)(const gmtl::AABox<float> &, const gmtl::Point<float,3> &))&gmtl::intersect);
    def("intersect", (bool (*)(const gmtl::AABox<float> &, const gmtl::Vec<float,3> &, const gmtl::AABox<float> &, const gmtl::Vec<float,3> &, float &, float &))&gmtl::intersect);
    def("intersect", (bool (*)(const gmtl::AABox<float> &, const gmtl::Sphere<float> &))&gmtl::intersect);
    def("xform", (gmtl::VecBase<float,3> & (*)(gmtl::VecBase<float,3> &, const gmtl::Quat<float> &, const gmtl::VecBase<float,3> &))&gmtl::xform, return_internal_reference< 1 >());
    def("xform", (gmtl::Vec<double,3> & (*)(gmtl::Vec<double,3> &, const gmtl::Matrix<double,3,3> &, const gmtl::Vec<double,3> &))&gmtl::xform, return_internal_reference< 1 >());
    def("xform", (gmtl::Vec<float,3> & (*)(gmtl::Vec<float,3> &, const gmtl::Matrix<float,4,4> &, const gmtl::Vec<float,3> &))&gmtl::xform, return_internal_reference< 1 >());
    def("xform", (gmtl::Vec<double,4> & (*)(gmtl::Vec<double,4> &, const gmtl::Matrix<double,4,4> &, const gmtl::Vec<double,4> &))&gmtl::xform, return_internal_reference< 1 >());
    def("xform", (gmtl::Vec<float,3> & (*)(gmtl::Vec<float,3> &, const gmtl::Matrix<float,3,3> &, const gmtl::Vec<float,3> &))&gmtl::xform, return_internal_reference< 1 >());
    def("xform", (gmtl::Vec<double,3> & (*)(gmtl::Vec<double,3> &, const gmtl::Matrix<double,3,3> &, const gmtl::Vec<double,3> &))&gmtl::xform, return_internal_reference< 1 >());
    def("xform", (gmtl::VecBase<double,3> & (*)(gmtl::VecBase<double,3> &, const gmtl::Quat<double> &, const gmtl::VecBase<double,3> &))&gmtl::xform, return_internal_reference< 1 >());
    def("xform", (gmtl::Vec<float,3> & (*)(gmtl::Vec<float,3> &, const gmtl::Matrix<float,3,3> &, const gmtl::Vec<float,3> &))&gmtl::xform, return_internal_reference< 1 >());
    def("xform", (gmtl::Vec<float,4> & (*)(gmtl::Vec<float,4> &, const gmtl::Matrix<float,4,4> &, const gmtl::Vec<float,4> &))&gmtl::xform, return_internal_reference< 1 >());
    def("xform", (gmtl::Point<double,4> & (*)(gmtl::Point<double,4> &, const gmtl::Matrix<double,4,4> &, const gmtl::Point<double,4> &))&gmtl::xform, return_internal_reference< 1 >());
    def("xform", (gmtl::Point<float,4> & (*)(gmtl::Point<float,4> &, const gmtl::Matrix<float,4,4> &, const gmtl::Point<float,4> &))&gmtl::xform, return_internal_reference< 1 >());
    def("xform", (gmtl::Point<double,3> & (*)(gmtl::Point<double,3> &, const gmtl::Matrix<double,3,3> &, const gmtl::Point<double,3> &))&gmtl::xform, return_internal_reference< 1 >());
    def("xform", (gmtl::Point<float,3> & (*)(gmtl::Point<float,3> &, const gmtl::Matrix<float,3,3> &, const gmtl::Point<float,3> &))&gmtl::xform, return_internal_reference< 1 >());
    def("xform", (gmtl::Point<double,3> & (*)(gmtl::Point<double,3> &, const gmtl::Matrix<double,4,4> &, const gmtl::Point<double,3> &))&gmtl::xform, return_internal_reference< 1 >());
    def("xform", (gmtl::Point<float,4> & (*)(gmtl::Point<float,4> &, const gmtl::Matrix<float,4,4> &, const gmtl::Point<float,4> &))&gmtl::xform, return_internal_reference< 1 >());
    def("xform", (gmtl::Point<float,3> & (*)(gmtl::Point<float,3> &, const gmtl::Matrix<float,4,4> &, const gmtl::Point<float,3> &))&gmtl::xform, return_internal_reference< 1 >());
    def("xform", (gmtl::Point<double,4> & (*)(gmtl::Point<double,4> &, const gmtl::Matrix<double,4,4> &, const gmtl::Point<double,4> &))&gmtl::xform, return_internal_reference< 1 >());
    def("xform", (gmtl::Vec<double,3> & (*)(gmtl::Vec<double,3> &, const gmtl::Matrix<double,4,4> &, const gmtl::Vec<double,3> &))&gmtl::xform, return_internal_reference< 1 >());
    def("xform", (gmtl::Vec<float,4> & (*)(gmtl::Vec<float,4> &, const gmtl::Matrix<float,4,4> &, const gmtl::Vec<float,4> &))&gmtl::xform, return_internal_reference< 1 >());
    def("xform", (gmtl::Vec<double,4> & (*)(gmtl::Vec<double,4> &, const gmtl::Matrix<double,4,4> &, const gmtl::Vec<double,4> &))&gmtl::xform, return_internal_reference< 1 >());
    def("isEqual", (bool (*)(const gmtl::Quat<float> &, const gmtl::Quat<float> &, float))&gmtl::isEqual, isEqual_overloads_2_3());
    def("isEqual", (bool (*)(const gmtl::EulerAngle<float,gmtl::ZYX> &, const gmtl::EulerAngle<float,gmtl::ZYX> &, const float &))&gmtl::isEqual, isEqual_overloads_2_3());
    def("isEqual", (bool (*)(const gmtl::EulerAngle<float,gmtl::XYZ> &, const gmtl::EulerAngle<float,gmtl::XYZ> &, const float &))&gmtl::isEqual, isEqual_overloads_2_3());
    def("isEqual", (bool (*)(const gmtl::EulerAngle<float,gmtl::ZXY> &, const gmtl::EulerAngle<float,gmtl::ZXY> &, const float &))&gmtl::isEqual, isEqual_overloads_2_3());
    def("isEqual", (bool (*)(const gmtl::AxisAngle<float> &, const gmtl::AxisAngle<float> &, const float &))&gmtl::isEqual, isEqual_overloads_2_3());
    def("isEqual", (bool (*)(const gmtl::AxisAngle<double> &, const gmtl::AxisAngle<double> &, const double &))&gmtl::isEqual, isEqual_overloads_2_3());
    def("isEqual", (bool (*)(const gmtl::Tri<double> &, const gmtl::Tri<double> &, const double &))&gmtl::isEqual);
    def("isEqual", (bool (*)(const gmtl::Tri<float> &, const gmtl::Tri<float> &, const float &))&gmtl::isEqual);
    def("isEqual", (bool (*)(const gmtl::Matrix<float,4,4> &, const gmtl::Matrix<float,4,4> &, const float &))&gmtl::isEqual, isEqual_overloads_2_3());
    def("isEqual", (bool (*)(const gmtl::Plane<float> &, const gmtl::Plane<float> &, const float &))&gmtl::isEqual);
    def("isEqual", (bool (*)(const gmtl::Plane<double> &, const gmtl::Plane<double> &, const double &))&gmtl::isEqual);
    def("isEqual", (bool (*)(const gmtl::Quat<double> &, const gmtl::Quat<double> &, double))&gmtl::isEqual, isEqual_overloads_2_3());
    def("isEqual", (bool (*)(const gmtl::VecBase<double,3> &, const gmtl::VecBase<double,3> &, const double &))&gmtl::isEqual);
    def("isEqual", (bool (*)(const gmtl::VecBase<float,4> &, const gmtl::VecBase<float,4> &, const float &))&gmtl::isEqual);
    def("isEqual", (bool (*)(const gmtl::VecBase<float,3> &, const gmtl::VecBase<float,3> &, const float &))&gmtl::isEqual);
    def("isEqual", (bool (*)(const gmtl::LineSeg<double> &, const gmtl::LineSeg<double> &, const double &))&gmtl::isEqual);
    def("isEqual", (bool (*)(const gmtl::LineSeg<float> &, const gmtl::LineSeg<float> &, const float &))&gmtl::isEqual);
    def("isEqual", (bool (*)(const gmtl::Sphere<float> &, const gmtl::Sphere<float> &, const float &))&gmtl::isEqual);
    def("isEqual", (bool (*)(const gmtl::Sphere<double> &, const gmtl::Sphere<double> &, const double &))&gmtl::isEqual);
    def("isEqual", (bool (*)(const gmtl::Matrix<float,3,3> &, const gmtl::Matrix<float,3,3> &, const float &))&gmtl::isEqual, isEqual_overloads_2_3());
    def("isEqual", (bool (*)(const gmtl::AABox<double> &, const gmtl::AABox<double> &, const double &))&gmtl::isEqual);
    def("isEqual", (bool (*)(const gmtl::AABox<float> &, const gmtl::AABox<float> &, const float &))&gmtl::isEqual);
    enum_< gmtl::VectorIndex >("VectorIndex")
        .value("Xelt", gmtl::Xelt)
        .value("Yelt", gmtl::Yelt)
        .value("Welt", gmtl::Welt)
        .value("Zelt", gmtl::Zelt)
    ;

    enum_< gmtl::PlaneSide >("PlaneSide")
        .value("NEG_SIDE", gmtl::NEG_SIDE)
        .value("POS_SIDE", gmtl::POS_SIDE)
        .value("ON_PLANE", gmtl::ON_PLANE)
    ;

    def("zero", (gmtl::Matrix<float,4,4> & (*)(gmtl::Matrix<float,4,4> &))&gmtl::zero, return_internal_reference< 1 >());
    def("zero", (gmtl::Matrix<float,3,3> & (*)(gmtl::Matrix<float,3,3> &))&gmtl::zero, return_internal_reference< 1 >());
    def("mult", (gmtl::Matrix<float,4,4> & (*)(gmtl::Matrix<float,4,4> &, const gmtl::Matrix<float,4,4> &, const gmtl::Matrix<float,4,4> &))&gmtl::mult, return_internal_reference< 1 >());
    def("mult", (gmtl::Matrix<float,3,3> & (*)(gmtl::Matrix<float,3,3> &, const gmtl::Matrix<float,3,3> &, const gmtl::Matrix<float,3,3> &))&gmtl::mult, return_internal_reference< 1 >());
    def("mult", (gmtl::Matrix<float,4,4> & (*)(gmtl::Matrix<float,4,4> &, const gmtl::Matrix<float,4,4> &, float))&gmtl::mult, return_internal_reference< 1 >());
    def("mult", (gmtl::Matrix<float,4,4> & (*)(gmtl::Matrix<float,4,4> &, float))&gmtl::mult, return_internal_reference< 1 >());
    def("mult", (gmtl::Matrix<float,3,3> & (*)(gmtl::Matrix<float,3,3> &, float))&gmtl::mult, return_internal_reference< 1 >());
    def("mult", (gmtl::Matrix<float,3,3> & (*)(gmtl::Matrix<float,3,3> &, const gmtl::Matrix<float,3,3> &, float))&gmtl::mult, return_internal_reference< 1 >());
    def("mult", (gmtl::Quat<double> & (*)(gmtl::Quat<double> &, const gmtl::Quat<double> &, double))&gmtl::mult, return_internal_reference< 1 >());
    def("mult", (gmtl::Quat<float> & (*)(gmtl::Quat<float> &, const gmtl::Quat<float> &, float))&gmtl::mult, return_internal_reference< 1 >());
    def("mult", (gmtl::Quat<float> & (*)(gmtl::Quat<float> &, const gmtl::Quat<float> &, const gmtl::Quat<float> &))&gmtl::mult, return_internal_reference< 1 >());
    def("mult", (gmtl::Quat<double> & (*)(gmtl::Quat<double> &, const gmtl::Quat<double> &, const gmtl::Quat<double> &))&gmtl::mult, return_internal_reference< 1 >());
    def("transpose", (gmtl::Matrix<float,3,3> & (*)(gmtl::Matrix<float,3,3> &, const gmtl::Matrix<float,3,3> &))&gmtl::transpose, return_internal_reference< 1 >());
    def("transpose", (gmtl::Matrix<float,4,4> & (*)(gmtl::Matrix<float,4,4> &, const gmtl::Matrix<float,4,4> &))&gmtl::transpose, return_internal_reference< 1 >());
    def("transpose", (gmtl::Matrix<double,4,4> & (*)(gmtl::Matrix<double,4,4> &))&gmtl::transpose, return_internal_reference< 1 >());
    def("transpose", (gmtl::Matrix<float,3,3> & (*)(gmtl::Matrix<float,3,3> &))&gmtl::transpose, return_internal_reference< 1 >());
    def("transpose", (gmtl::Matrix<double,3,3> & (*)(gmtl::Matrix<double,3,3> &))&gmtl::transpose, return_internal_reference< 1 >());
    def("transpose", (gmtl::Matrix<float,4,4> & (*)(gmtl::Matrix<float,4,4> &))&gmtl::transpose, return_internal_reference< 1 >());
    def("invertFull", (gmtl::Matrix<float,4,4> & (*)(gmtl::Matrix<float,4,4> &, const gmtl::Matrix<float,4,4> &))&gmtl::invertFull, return_internal_reference< 1 >());
    def("invertFull", (gmtl::Matrix<double,3,3> & (*)(gmtl::Matrix<double,3,3> &, const gmtl::Matrix<double,3,3> &))&gmtl::invertFull, return_internal_reference< 1 >());
    def("invertFull", (gmtl::Matrix<float,3,3> & (*)(gmtl::Matrix<float,3,3> &, const gmtl::Matrix<float,3,3> &))&gmtl::invertFull, return_internal_reference< 1 >());
    def("invertFull", (gmtl::Matrix<double,4,4> & (*)(gmtl::Matrix<double,4,4> &, const gmtl::Matrix<double,4,4> &))&gmtl::invertFull, return_internal_reference< 1 >());
    def("invert", (gmtl::Quat<double> & (*)(gmtl::Quat<double> &))&gmtl::invert, return_internal_reference< 1 >());
    def("invert", (gmtl::Quat<float> & (*)(gmtl::Quat<float> &))&gmtl::invert, return_internal_reference< 1 >());
    def("invert", (gmtl::Matrix<float,3,3> & (*)(gmtl::Matrix<float,3,3> &))&gmtl::invert, return_internal_reference< 1 >());
    def("invert", (gmtl::Matrix<double,4,4> & (*)(gmtl::Matrix<double,4,4> &, const gmtl::Matrix<double,4,4> &))&gmtl::invert, return_internal_reference< 1 >());
    def("invert", (gmtl::Matrix<float,4,4> & (*)(gmtl::Matrix<float,4,4> &))&gmtl::invert, return_internal_reference< 1 >());
    def("invert", (gmtl::Matrix<float,3,3> & (*)(gmtl::Matrix<float,3,3> &, const gmtl::Matrix<float,3,3> &))&gmtl::invert, return_internal_reference< 1 >());
    def("invert", (gmtl::Matrix<double,3,3> & (*)(gmtl::Matrix<double,3,3> &, const gmtl::Matrix<double,3,3> &))&gmtl::invert, return_internal_reference< 1 >());
    def("invert", (gmtl::Matrix<float,4,4> & (*)(gmtl::Matrix<float,4,4> &, const gmtl::Matrix<float,4,4> &))&gmtl::invert, return_internal_reference< 1 >());
    def("dot", (float (*)(const gmtl::Vec<float,4> &, const gmtl::Vec<float,4> &))&gmtl::dot);
    def("dot", (float (*)(const gmtl::Vec<float,3> &, const gmtl::Vec<float,3> &))&gmtl::dot);
    def("dot", (double (*)(const gmtl::Vec<double,3> &, const gmtl::Vec<double,3> &))&gmtl::dot);
    def("dot", (float (*)(const gmtl::Quat<float> &, const gmtl::Quat<float> &))&gmtl::dot);
    def("dot", (double (*)(const gmtl::Quat<double> &, const gmtl::Quat<double> &))&gmtl::dot);
    def("length", (double (*)(const gmtl::Vec<double,3> &))&gmtl::length);
    def("length", (float (*)(const gmtl::Vec<float,4> &))&gmtl::length);
    def("length", (float (*)(const gmtl::Vec<float,3> &))&gmtl::length);
    def("length", (double (*)(const gmtl::Quat<double> &))&gmtl::length);
    def("length", (float (*)(const gmtl::Quat<float> &))&gmtl::length);
    def("lengthSquared", (float (*)(const gmtl::Vec<float,3> &))&gmtl::lengthSquared);
    def("lengthSquared", (double (*)(const gmtl::Vec<double,3> &))&gmtl::lengthSquared);
    def("lengthSquared", (float (*)(const gmtl::Quat<float> &))&gmtl::lengthSquared);
    def("lengthSquared", (double (*)(const gmtl::Quat<double> &))&gmtl::lengthSquared);
    def("lengthSquared", (float (*)(const gmtl::Vec<float,4> &))&gmtl::lengthSquared);
    def("normalize", (gmtl::Quat<double> & (*)(gmtl::Quat<double> &))&gmtl::normalize, return_internal_reference< 1 >());
    def("normalize", (float (*)(gmtl::Vec<float,4> &))&gmtl::normalize);
    def("normalize", (float (*)(gmtl::Vec<float,3> &))&gmtl::normalize);
    def("normalize", (gmtl::Quat<float> & (*)(gmtl::Quat<float> &))&gmtl::normalize, return_internal_reference< 1 >());
    def("normalize", (double (*)(gmtl::Vec<double,3> &))&gmtl::normalize);
    def("isNormalized", (bool (*)(const gmtl::Quat<double> &, const double))&gmtl::isNormalized, isNormalized_overloads_1_2());
    def("isNormalized", (bool (*)(const gmtl::Quat<float> &, const float))&gmtl::isNormalized, isNormalized_overloads_1_2());
    def("isNormalized", (bool (*)(const gmtl::Vec<float,3> &, const float))&gmtl::isNormalized, isNormalized_overloads_1_2());
    def("isNormalized", (bool (*)(const gmtl::Vec<float,4> &, const float))&gmtl::isNormalized, isNormalized_overloads_1_2());
    def("cross", (gmtl::Vec<double,3> & (*)(gmtl::Vec<double,3> &, const gmtl::Vec<double,3> &, const gmtl::Vec<double,3> &))&gmtl::cross, return_internal_reference< 1 >());
    def("cross", (gmtl::Vec<float,3> (*)(const gmtl::Vec<float,3> &, const gmtl::Vec<float,3> &))&gmtl::cross);
    def("cross", (gmtl::Vec<double,3> (*)(const gmtl::Vec<double,3> &, const gmtl::Vec<double,3> &))&gmtl::cross);
    def("cross", (gmtl::Vec<float,3> & (*)(gmtl::Vec<float,3> &, const gmtl::Vec<float,3> &, const gmtl::Vec<float,3> &))&gmtl::cross, return_internal_reference< 1 >());
    def("reflect", (gmtl::VecBase<float,4> & (*)(gmtl::VecBase<float,4> &, const gmtl::VecBase<float,4> &, const gmtl::Vec<float,4> &))&gmtl::reflect, return_internal_reference< 1 >());
    def("reflect", (gmtl::VecBase<float,3> & (*)(gmtl::VecBase<float,3> &, const gmtl::VecBase<float,3> &, const gmtl::Vec<float,3> &))&gmtl::reflect, return_internal_reference< 1 >());
    def("lerp", (gmtl::Quat<float> & (*)(gmtl::Quat<float> &, const float, const gmtl::Quat<float> &, const gmtl::Quat<float> &))&gmtl::lerp, return_internal_reference< 1 >());
    def("lerp", (gmtl::Quat<double> & (*)(gmtl::Quat<double> &, const double, const gmtl::Quat<double> &, const gmtl::Quat<double> &))&gmtl::lerp, return_internal_reference< 1 >());
    def("lerp", (gmtl::VecBase<float,3> & (*)(gmtl::VecBase<float,3> &, const float &, const gmtl::VecBase<float,3> &, const gmtl::VecBase<float,3> &))&gmtl::lerp, return_internal_reference< 1 >());
    def("lerp", (gmtl::VecBase<float,4> & (*)(gmtl::VecBase<float,4> &, const float &, const gmtl::VecBase<float,4> &, const gmtl::VecBase<float,4> &))&gmtl::lerp, return_internal_reference< 1 >());
    def("negate", (gmtl::Quat<double> & (*)(gmtl::Quat<double> &))&gmtl::negate, return_internal_reference< 1 >());
    def("negate", (gmtl::Quat<float> & (*)(gmtl::Quat<float> &))&gmtl::negate, return_internal_reference< 1 >());
    def("div", (gmtl::Quat<float> & (*)(gmtl::Quat<float> &, const gmtl::Quat<float> &, gmtl::Quat<float>))&gmtl::div, return_internal_reference< 1 >());
    def("div", (gmtl::Quat<float> & (*)(gmtl::Quat<float> &, const gmtl::Quat<float> &, float))&gmtl::div, return_internal_reference< 1 >());
    def("div", (gmtl::Quat<double> & (*)(gmtl::Quat<double> &, const gmtl::Quat<double> &, gmtl::Quat<double>))&gmtl::div, return_internal_reference< 1 >());
    def("div", (gmtl::Quat<double> & (*)(gmtl::Quat<double> &, const gmtl::Quat<double> &, double))&gmtl::div, return_internal_reference< 1 >());
    def("conj", (gmtl::Quat<float> & (*)(gmtl::Quat<float> &))&gmtl::conj, return_internal_reference< 1 >());
    def("conj", (gmtl::Quat<double> & (*)(gmtl::Quat<double> &))&gmtl::conj, return_internal_reference< 1 >());
    def("invert", (gmtl::Quat<double> & (*)(gmtl::Quat<double> &))&gmtl::invert, return_internal_reference< 1 >());
    def("invert", (gmtl::Quat<float> & (*)(gmtl::Quat<float> &))&gmtl::invert, return_internal_reference< 1 >());
    def("invert", (gmtl::Matrix<float,3,3> & (*)(gmtl::Matrix<float,3,3> &))&gmtl::invert, return_internal_reference< 1 >());
    def("invert", (gmtl::Matrix<double,4,4> & (*)(gmtl::Matrix<double,4,4> &, const gmtl::Matrix<double,4,4> &))&gmtl::invert, return_internal_reference< 1 >());
    def("invert", (gmtl::Matrix<float,4,4> & (*)(gmtl::Matrix<float,4,4> &))&gmtl::invert, return_internal_reference< 1 >());
    def("invert", (gmtl::Matrix<float,3,3> & (*)(gmtl::Matrix<float,3,3> &, const gmtl::Matrix<float,3,3> &))&gmtl::invert, return_internal_reference< 1 >());
    def("invert", (gmtl::Matrix<double,3,3> & (*)(gmtl::Matrix<double,3,3> &, const gmtl::Matrix<double,3,3> &))&gmtl::invert, return_internal_reference< 1 >());
    def("invert", (gmtl::Matrix<float,4,4> & (*)(gmtl::Matrix<float,4,4> &, const gmtl::Matrix<float,4,4> &))&gmtl::invert, return_internal_reference< 1 >());
    def("exp", (gmtl::Quat<double> & (*)(gmtl::Quat<double> &))&gmtl::exp, return_internal_reference< 1 >());
    def("exp", (gmtl::Quat<float> & (*)(gmtl::Quat<float> &))&gmtl::exp, return_internal_reference< 1 >());
    def("log", (gmtl::Quat<double> & (*)(gmtl::Quat<double> &))&gmtl::log, return_internal_reference< 1 >());
    def("log", (gmtl::Quat<float> & (*)(gmtl::Quat<float> &))&gmtl::log, return_internal_reference< 1 >());
    def("squad", (void (*)(gmtl::Quat<float> &, float, const gmtl::Quat<float> &, const gmtl::Quat<float> &, const gmtl::Quat<float> &, const gmtl::Quat<float> &))&gmtl::squad, return_internal_reference< 1 >());
    def("squad", (void (*)(gmtl::Quat<double> &, double, const gmtl::Quat<double> &, const gmtl::Quat<double> &, const gmtl::Quat<double> &, const gmtl::Quat<double> &))&gmtl::squad, return_internal_reference< 1 >());
    def("slerp", (gmtl::Quat<double> & (*)(gmtl::Quat<double> &, const double, const gmtl::Quat<double> &, const gmtl::Quat<double> &, bool))&gmtl::slerp, return_internal_reference< 1 >(), slerp_overloads_4_5());
    def("slerp", (gmtl::Quat<float> & (*)(gmtl::Quat<float> &, const float, const gmtl::Quat<float> &, const gmtl::Quat<float> &, bool))&gmtl::slerp, return_internal_reference< 1 >(), slerp_overloads_4_5());
    def("isEquiv", (bool (*)(const gmtl::Quat<float> &, const gmtl::Quat<float> &, float))&gmtl::isEquiv, isEquiv_overloads_2_3());
    def("isEquiv", (bool (*)(const gmtl::Quat<double> &, const gmtl::Quat<double> &, double))&gmtl::isEquiv, isEquiv_overloads_2_3());
    def("preMult", (gmtl::Matrix<float,4,4> & (*)(gmtl::Matrix<float,4,4> &, const gmtl::Matrix<float,4,4> &))&gmtl::preMult, return_internal_reference< 1 >());
    def("preMult", (gmtl::Matrix<float,3,3> & (*)(gmtl::Matrix<float,3,3> &, const gmtl::Matrix<float,3,3> &))&gmtl::preMult, return_internal_reference< 1 >());
    def("sub", (gmtl::Matrix<float,4,4> & (*)(gmtl::Matrix<float,4,4> &, const gmtl::Matrix<float,4,4> &, const gmtl::Matrix<float,4,4> &))&gmtl::sub, return_internal_reference< 1 >());
    def("sub", (gmtl::Matrix<float,3,3> & (*)(gmtl::Matrix<float,3,3> &, const gmtl::Matrix<float,3,3> &, const gmtl::Matrix<float,3,3> &))&gmtl::sub, return_internal_reference< 1 >());
    def("sub", (gmtl::Quat<double> & (*)(gmtl::Quat<double> &, const gmtl::Quat<double> &, const gmtl::Quat<double> &))&gmtl::sub, return_internal_reference< 1 >());
    def("sub", (gmtl::Quat<float> & (*)(gmtl::Quat<float> &, const gmtl::Quat<float> &, const gmtl::Quat<float> &))&gmtl::sub, return_internal_reference< 1 >());
    def("add", (gmtl::Matrix<float,4,4> & (*)(gmtl::Matrix<float,4,4> &, const gmtl::Matrix<float,4,4> &, const gmtl::Matrix<float,4,4> &))&gmtl::add, return_internal_reference< 1 >());
    def("add", (gmtl::Matrix<float,3,3> & (*)(gmtl::Matrix<float,3,3> &, const gmtl::Matrix<float,3,3> &, const gmtl::Matrix<float,3,3> &))&gmtl::add, return_internal_reference< 1 >());
    def("add", (gmtl::Quat<double> & (*)(gmtl::Quat<double> &, const gmtl::Quat<double> &, const gmtl::Quat<double> &))&gmtl::add, return_internal_reference< 1 >());
    def("add", (gmtl::Quat<float> & (*)(gmtl::Quat<float> &, const gmtl::Quat<float> &, const gmtl::Quat<float> &))&gmtl::add, return_internal_reference< 1 >());
    def("postMult", (gmtl::Matrix<float,4,4> & (*)(gmtl::Matrix<float,4,4> &, const gmtl::Matrix<float,4,4> &))&gmtl::postMult, return_internal_reference< 1 >());
    def("postMult", (gmtl::Matrix<float,3,3> & (*)(gmtl::Matrix<float,3,3> &, const gmtl::Matrix<float,3,3> &))&gmtl::postMult, return_internal_reference< 1 >());
/*
    def("+=", (gmtl::VecBase<float,4> & (*)(gmtl::VecBase<float,4> &, const gmtl::VecBase<float,4> &))&gmtl::operator+=, return_internal_reference< 1 >());
    def("+=", (gmtl::VecBase<float,3> & (*)(gmtl::VecBase<float,3> &, const gmtl::VecBase<float,3> &))&gmtl::operator+=, return_internal_reference< 1 >());
    def("+=", (gmtl::Quat<float> & (*)(gmtl::Quat<float> &, const gmtl::Quat<float> &))&gmtl::operator+=, return_internal_reference< 1 >());
    def("+=", (gmtl::Quat<double> & (*)(gmtl::Quat<double> &, const gmtl::Quat<double> &))&gmtl::operator+=, return_internal_reference< 1 >());
    def("+", (gmtl::VecBase<float,4> (*)(const gmtl::VecBase<float,4> &, const gmtl::VecBase<float,4> &))&gmtl::operator+);
    def("+", (gmtl::VecBase<float,3> (*)(const gmtl::VecBase<float,3> &, const gmtl::VecBase<float,3> &))&gmtl::operator+);
    def("+", (gmtl::Quat<float> (*)(const gmtl::Quat<float> &, const gmtl::Quat<float> &))&gmtl::operator+);
    def("+", (gmtl::Quat<double> (*)(const gmtl::Quat<double> &, const gmtl::Quat<double> &))&gmtl::operator+);
    def("-=", (gmtl::VecBase<double,3> & (*)(gmtl::VecBase<double,3> &, const gmtl::VecBase<double,3> &))&gmtl::operator-=, return_internal_reference< 1 >());
    def("-=", (gmtl::VecBase<float,4> & (*)(gmtl::VecBase<float,4> &, const gmtl::VecBase<float,4> &))&gmtl::operator-=, return_internal_reference< 1 >());
    def("-=", (gmtl::VecBase<float,3> & (*)(gmtl::VecBase<float,3> &, const gmtl::VecBase<float,3> &))&gmtl::operator-=, return_internal_reference< 1 >());
    def("-=", (gmtl::Quat<float> & (*)(gmtl::Quat<float> &, const gmtl::Quat<float> &))&gmtl::operator-=, return_internal_reference< 1 >());
    def("-=", (gmtl::Quat<double> & (*)(gmtl::Quat<double> &, const gmtl::Quat<double> &))&gmtl::operator-=, return_internal_reference< 1 >());
    def("-", (gmtl::Vec<float,3> (*)(const gmtl::VecBase<float,3> &, const gmtl::VecBase<float,3> &))&gmtl::operator-);
    def("-", (gmtl::Vec<float,4> (*)(const gmtl::VecBase<float,4> &))&gmtl::operator-);
    def("-", (gmtl::Vec<float,3> (*)(const gmtl::VecBase<float,3> &))&gmtl::operator-);
    def("-", (gmtl::Quat<float> (*)(const gmtl::Quat<float> &))&gmtl::operator-);
    def("-", (gmtl::Quat<double> (*)(const gmtl::Quat<double> &))&gmtl::operator-);
    def("-", (gmtl::Quat<float> (*)(const gmtl::Quat<float> &, const gmtl::Quat<float> &))&gmtl::operator-);
    def("-", (gmtl::Quat<double> (*)(const gmtl::Quat<double> &, const gmtl::Quat<double> &))&gmtl::operator-);
    def("-", (gmtl::Vec<float,4> (*)(const gmtl::VecBase<float,4> &, const gmtl::VecBase<float,4> &))&gmtl::operator-);
    def("-", (gmtl::Vec<double,3> (*)(const gmtl::VecBase<double,3> &, const gmtl::VecBase<double,3> &))&gmtl::operator-);
    def("*=", (gmtl::Quat<float> & (*)(gmtl::Quat<float> &, float))&gmtl::operator*=, return_internal_reference< 1 >());
    def("*=", (gmtl::Quat<double> & (*)(gmtl::Quat<double> &, double))&gmtl::operator*=, return_internal_reference< 1 >());
    def("*=", (gmtl::Quat<double> & (*)(gmtl::Quat<double> &, const gmtl::Quat<double> &))&gmtl::operator*=, return_internal_reference< 1 >());
    def("*=", (gmtl::Quat<float> & (*)(gmtl::Quat<float> &, const gmtl::Quat<float> &))&gmtl::operator*=, return_internal_reference< 1 >());
    def("*=", (gmtl::VecBase<float,4> & (*)(gmtl::VecBase<float,4> &, const int &))&gmtl::operator*=, return_internal_reference< 1 >());
    def("*=", (gmtl::VecBase<float,4> & (*)(gmtl::VecBase<float,4> &, const float &))&gmtl::operator*=, return_internal_reference< 1 >());
    def("*=", (gmtl::VecBase<float,3> & (*)(gmtl::VecBase<float,3> &, const int &))&gmtl::operator*=, return_internal_reference< 1 >());
    def("*=", (gmtl::VecBase<float,3> & (*)(gmtl::VecBase<float,3> &, const float &))&gmtl::operator*=, return_internal_reference< 1 >());
    def("*", (gmtl::Quat<float> (*)(const gmtl::Quat<float> &, float))&gmtl::operator*);
    def("*", (gmtl::Quat<double> (*)(const gmtl::Quat<double> &, double))&gmtl::operator*);
    def("*", (gmtl::Quat<double> (*)(const gmtl::Quat<double> &, const gmtl::Quat<double> &))&gmtl::operator*);
    def("*", (gmtl::Quat<float> (*)(const gmtl::Quat<float> &, const gmtl::Quat<float> &))&gmtl::operator*);
    def("*", (gmtl::Matrix<float,4,4> (*)(const gmtl::Matrix<float,4,4> &, const gmtl::Matrix<float,4,4> &))&gmtl::operator*);
    def("*", (gmtl::Matrix<float,3,3> (*)(const gmtl::Matrix<float,3,3> &, const gmtl::Matrix<float,3,3> &))&gmtl::operator*);
    def("*", (gmtl::VecBase<float,4> (*)(const gmtl::VecBase<float,4> &, const int &))&gmtl::operator*);
    def("*", (gmtl::VecBase<float,4> (*)(const gmtl::VecBase<float,4> &, const float &))&gmtl::operator*);
    def("*", (gmtl::VecBase<float,3> (*)(const gmtl::VecBase<float,3> &, const int &))&gmtl::operator*);
    def("*", (gmtl::VecBase<float,3> (*)(const gmtl::VecBase<float,3> &, const float &))&gmtl::operator*);
    def("*", (gmtl::VecBase<float,3> (*)(const float &, const gmtl::VecBase<float,3> &))&gmtl::operator*);
    def("*", (gmtl::VecBase<float,3> (*)(const int &, const gmtl::VecBase<float,3> &))&gmtl::operator*);
    def("*", (gmtl::VecBase<float,4> (*)(const float &, const gmtl::VecBase<float,4> &))&gmtl::operator*);
    def("*", (gmtl::VecBase<float,4> (*)(const int &, const gmtl::VecBase<float,4> &))&gmtl::operator*);
    def("/=", (gmtl::Quat<double> & (*)(gmtl::Quat<double> &, const gmtl::Quat<double> &))&gmtl::operator/=, return_internal_reference< 1 >());
    def("/=", (gmtl::Quat<float> & (*)(gmtl::Quat<float> &, const gmtl::Quat<float> &))&gmtl::operator/=, return_internal_reference< 1 >());
    def("/=", (gmtl::VecBase<float,4> & (*)(gmtl::VecBase<float,4> &, const float &))&gmtl::operator/=, return_internal_reference< 1 >());
    def("/=", (gmtl::VecBase<float,4> & (*)(gmtl::VecBase<float,4> &, const int &))&gmtl::operator/=, return_internal_reference< 1 >());
    def("/=", (gmtl::VecBase<float,3> & (*)(gmtl::VecBase<float,3> &, const float &))&gmtl::operator/=, return_internal_reference< 1 >());
    def("/=", (gmtl::VecBase<float,3> & (*)(gmtl::VecBase<float,3> &, const int &))&gmtl::operator/=, return_internal_reference< 1 >());
    def("/", (gmtl::Quat<double> (*)(const gmtl::Quat<double> &, gmtl::Quat<double>))&gmtl::operator/);
    def("/", (gmtl::Quat<float> (*)(const gmtl::Quat<float> &, gmtl::Quat<float>))&gmtl::operator/);
    def("/", (gmtl::Quat<double> (*)(const gmtl::Quat<double> &, double))&gmtl::operator/);
    def("/", (gmtl::Quat<float> (*)(const gmtl::Quat<float> &, float))&gmtl::operator/);
    def("/", (gmtl::VecBase<float,4> (*)(const gmtl::VecBase<float,4> &, const int &))&gmtl::operator/);
    def("/", (gmtl::VecBase<float,4> (*)(const gmtl::VecBase<float,4> &, const float &))&gmtl::operator/);
    def("/", (gmtl::VecBase<float,3> (*)(const gmtl::VecBase<float,3> &, const float &))&gmtl::operator/);
    def("/", (gmtl::VecBase<float,3> (*)(const gmtl::VecBase<float,3> &, const int &))&gmtl::operator/);
    def("==", (bool (*)(const gmtl::Matrix<float,4,4> &, const gmtl::Matrix<float,4,4> &))&gmtl::operator==);
    def("==", (bool (*)(const gmtl::Matrix<float,3,3> &, const gmtl::Matrix<float,3,3> &))&gmtl::operator==);
    def("==", (bool (*)(const gmtl::EulerAngle<float,gmtl::ZXY> &, const gmtl::EulerAngle<float,gmtl::ZXY> &))&gmtl::operator==);
    def("==", (bool (*)(const gmtl::EulerAngle<float,gmtl::ZYX> &, const gmtl::EulerAngle<float,gmtl::ZYX> &))&gmtl::operator==);
    def("==", (bool (*)(const gmtl::AxisAngle<float> &, const gmtl::AxisAngle<float> &))&gmtl::operator==);
    def("==", (bool (*)(const gmtl::AxisAngle<double> &, const gmtl::AxisAngle<double> &))&gmtl::operator==);
    def("==", (bool (*)(const gmtl::Quat<double> &, const gmtl::Quat<double> &))&gmtl::operator==);
    def("==", (bool (*)(const gmtl::EulerAngle<float,gmtl::XYZ> &, const gmtl::EulerAngle<float,gmtl::XYZ> &))&gmtl::operator==);
    def("==", (bool (*)(const gmtl::VecBase<float,3> &, const gmtl::VecBase<float,3> &))&gmtl::operator==);
    def("==", (bool (*)(const gmtl::VecBase<float,4> &, const gmtl::VecBase<float,4> &))&gmtl::operator==);
    def("==", (bool (*)(const gmtl::VecBase<double,4> &, const gmtl::VecBase<double,4> &))&gmtl::operator==);
    def("==", (bool (*)(const gmtl::Quat<float> &, const gmtl::Quat<float> &))&gmtl::operator==);
    def("!=", (bool (*)(const gmtl::Matrix<float,4,4> &, const gmtl::Matrix<float,4,4> &))&gmtl::operator!=);
    def("!=", (bool (*)(const gmtl::Matrix<float,3,3> &, const gmtl::Matrix<float,3,3> &))&gmtl::operator!=);
    def("!=", (bool (*)(const gmtl::EulerAngle<float,gmtl::ZYX> &, const gmtl::EulerAngle<float,gmtl::ZYX> &))&gmtl::operator!=);
    def("!=", (bool (*)(const gmtl::AxisAngle<float> &, const gmtl::AxisAngle<float> &))&gmtl::operator!=);
    def("!=", (bool (*)(const gmtl::Quat<double> &, const gmtl::Quat<double> &))&gmtl::operator!=);
    def("!=", (bool (*)(const gmtl::AxisAngle<double> &, const gmtl::AxisAngle<double> &))&gmtl::operator!=);
    def("!=", (bool (*)(const gmtl::EulerAngle<float,gmtl::XYZ> &, const gmtl::EulerAngle<float,gmtl::XYZ> &))&gmtl::operator!=);
    def("!=", (bool (*)(const gmtl::EulerAngle<float,gmtl::ZXY> &, const gmtl::EulerAngle<float,gmtl::ZXY> &))&gmtl::operator!=);
    def("!=", (bool (*)(const gmtl::VecBase<float,4> &, const gmtl::VecBase<float,4> &))&gmtl::operator!=);
    def("!=", (bool (*)(const gmtl::VecBase<float,3> &, const gmtl::VecBase<float,3> &))&gmtl::operator!=);
    def("!=", (bool (*)(const gmtl::Quat<float> &, const gmtl::Quat<float> &))&gmtl::operator!=);
*/
    scope* gmtl_Quat_float_scope = new scope(
    class_< gmtl::Quat<float> >("Quatf", init<  >())
        .def(init< const float, const float, const float, const float >())
        .def(init< const gmtl::Quat<float> & >())
        .def_readwrite("mData", &gmtl::Quat<float>::mData)
        .def("set", &gmtl::Quat<float>::set)
        .def("get", &gmtl::Quat<float>::get)
        .def("__getitem__", (float& (gmtl::Quat<float>::*)(const int)) &gmtl::Quat<float>::operator[], return_value_policy<copy_non_const_reference>())
        .def("__setitem__", (void (*)(gmtl::Quatf*, const unsigned, float)) &gmtlWrapper::setArrayElement)
    );

    enum_< gmtl::Quat<float>::Params >("Params")
        .value("Size", gmtl::Quat<float>::Size)
    ;

    delete gmtl_Quat_float_scope;

    scope* gmtl_Matrix_float_4_4_scope = new scope(
    class_< gmtl::Matrix<float,4,4> >("Matrix44f", init<  >())
        .def(init< const gmtl::Matrix<float,4,4> & >())
        .def_readwrite("mState", &gmtl::Matrix<float,4,4>::mState)
        .def("set", (void (gmtl::Matrix<float,4,4>::*)(float, float, float, float) )&gmtl::Matrix<float,4,4>::set)
        .def("set", (void (gmtl::Matrix<float,4,4>::*)(float, float, float, float, float, float) )&gmtl::Matrix<float,4,4>::set)
        .def("set", (void (gmtl::Matrix<float,4,4>::*)(float, float, float, float, float, float, float, float, float) )&gmtl::Matrix<float,4,4>::set)
        .def("set", (void (gmtl::Matrix<float,4,4>::*)(float, float, float, float, float, float, float, float, float, float, float, float) )&gmtl::Matrix<float,4,4>::set)
        .def("set", (void (gmtl::Matrix<float,4,4>::*)(const float *) )&gmtl::Matrix<float,4,4>::set)
        .def("setTranspose", &gmtl::Matrix<float,4,4>::setTranspose)
        .def("isError", &gmtl::Matrix<float,4,4>::isError)
        .def("setError", &gmtl::Matrix<float,4,4>::setError)
        .def("__getitem__", &gmtl::Matrix<float,4,4>::operator[])
    );

    class_< gmtl::Matrix<float,4,4>::RowAccessor >("RowAccessor", no_init)
       .def("__getitem__", &gmtl::Matrix<float,4,4>::RowAccessor::operator[], return_value_policy<copy_non_const_reference>())
       .def("__setitem__", (void (*)(gmtl::Matrix44f::RowAccessor*, const unsigned, float)) &gmtlWrapper::setArrayElement)
    ;


    enum_< gmtl::Matrix<float,4,4>::Params >("Params")
        .value("Rows", gmtl::Matrix<float,4,4>::Rows)
        .value("Cols", gmtl::Matrix<float,4,4>::Cols)
    ;


    enum_< gmtl::Matrix<float,4,4>::XformState >("XformState")
        .value("ORTHONORMAL", gmtl::Matrix<float,4,4>::ORTHONORMAL)
        .value("XFORM_ERROR", gmtl::Matrix<float,4,4>::XFORM_ERROR)
        .value("FULL", gmtl::Matrix<float,4,4>::FULL)
        .value("ORTHOGONAL", gmtl::Matrix<float,4,4>::ORTHOGONAL)
        .value("AFFINE", gmtl::Matrix<float,4,4>::AFFINE)
        .value("IDENTITY", gmtl::Matrix<float,4,4>::IDENTITY)
    ;

    delete gmtl_Matrix_float_4_4_scope;

    scope* gmtl_Quat_double_scope = new scope(
    class_< gmtl::Quat<double> >("Quatd", init<  >())
        .def(init< const double, const double, const double, const double >())
        .def(init< const gmtl::Quat<double> & >())
        .def_readwrite("mData", &gmtl::Quat<double>::mData)
        .def("set", &gmtl::Quat<double>::set)
        .def("get", &gmtl::Quat<double>::get)
        .def("__getitem__", (double& (gmtl::Quat<double>::*)(const int)) &gmtl::Quat<double>::operator[], return_value_policy<copy_non_const_reference>())
        .def("__setitem__", (void (*)(gmtl::Quatd*, const unsigned, double)) &gmtlWrapper::setArrayElement)
    );

    enum_< gmtl::Quat<double>::Params >("Params")
        .value("Size", gmtl::Quat<double>::Size)
    ;

    delete gmtl_Quat_double_scope;

    scope* gmtl_VecBase_float_3_scope = new scope(
    class_< gmtl::VecBase<float,3> >("VecBase3f", init<  >())
        .def(init< const gmtl::VecBase<float,3> & >())
        .def(init< const float &, const float & >())
        .def(init< const float &, const float &, const float & >())
        .def(init< const float &, const float &, const float &, const float & >())
        .def("set", (void (gmtl::VecBase<float,3>::*)(const float *) )&gmtl::VecBase<float,3>::set)
        .def("set", (void (gmtl::VecBase<float,3>::*)(const float &) )&gmtl::VecBase<float,3>::set)
        .def("set", (void (gmtl::VecBase<float,3>::*)(const float &, const float &) )&gmtl::VecBase<float,3>::set)
        .def("set", (void (gmtl::VecBase<float,3>::*)(const float &, const float &, const float &) )&gmtl::VecBase<float,3>::set)
        .def("set", (void (gmtl::VecBase<float,3>::*)(const float &, const float &, const float &, const float &) )&gmtl::VecBase<float,3>::set)
        .def("__getitem__", (float& (gmtl::VecBase<float,3>::*)(const unsigned) ) &gmtl::VecBase<float,3>::operator[], return_value_policy<copy_non_const_reference>())
        .def("__setitem__", (void (*)(gmtl::VecBase<float,3>*, const unsigned, float)) &gmtlWrapper::setArrayElement)
    );

    enum_< gmtl::VecBase<float,3>::Params >("Params")
        .value("Size", gmtl::VecBase<float,3>::Size)
    ;

    delete gmtl_VecBase_float_3_scope;

    scope* gmtl_VecBase_float_4_scope = new scope(
    class_< gmtl::VecBase<float,4> >("VecBase4f", init<  >())
        .def(init< const gmtl::VecBase<float,4> & >())
        .def(init< const float &, const float & >())
        .def(init< const float &, const float &, const float & >())
        .def(init< const float &, const float &, const float &, const float & >())
        .def("set", (void (gmtl::VecBase<float,4>::*)(const float *) )&gmtl::VecBase<float,4>::set)
        .def("set", (void (gmtl::VecBase<float,4>::*)(const float &) )&gmtl::VecBase<float,4>::set)
        .def("set", (void (gmtl::VecBase<float,4>::*)(const float &, const float &) )&gmtl::VecBase<float,4>::set)
        .def("set", (void (gmtl::VecBase<float,4>::*)(const float &, const float &, const float &) )&gmtl::VecBase<float,4>::set)
        .def("set", (void (gmtl::VecBase<float,4>::*)(const float &, const float &, const float &, const float &) )&gmtl::VecBase<float,4>::set)
        .def("__getitem__", (float& (gmtl::VecBase<float,4>::*)(const unsigned) ) &gmtl::VecBase<float,4>::operator[], return_value_policy<copy_non_const_reference>())
        .def("__setitem__", (void (*)(gmtl::VecBase<float,4>*, const unsigned, float)) &gmtlWrapper::setArrayElement)
    );

    enum_< gmtl::VecBase<float,4>::Params >("Params")
        .value("Size", gmtl::VecBase<float,4>::Size)
    ;

    delete gmtl_VecBase_float_4_scope;

    scope* gmtl_VecBase_int_3_scope = new scope(
    class_< gmtl::VecBase<int,3> >("VecBase3i", init<  >())
        .def(init< const gmtl::VecBase<int,3> & >())
        .def(init< const int &, const int & >())
        .def(init< const int &, const int &, const int & >())
        .def(init< const int &, const int &, const int &, const int & >())
        .def("set", (void (gmtl::VecBase<int,3>::*)(const int *) )&gmtl::VecBase<int,3>::set)
        .def("set", (void (gmtl::VecBase<int,3>::*)(const int &) )&gmtl::VecBase<int,3>::set)
        .def("set", (void (gmtl::VecBase<int,3>::*)(const int &, const int &) )&gmtl::VecBase<int,3>::set)
        .def("set", (void (gmtl::VecBase<int,3>::*)(const int &, const int &, const int &) )&gmtl::VecBase<int,3>::set)
        .def("set", (void (gmtl::VecBase<int,3>::*)(const int &, const int &, const int &, const int &) )&gmtl::VecBase<int,3>::set)
        .def("__getitem__", (int& (gmtl::VecBase<int,3>::*)(const unsigned) ) &gmtl::VecBase<int,3>::operator[], return_value_policy<copy_non_const_reference>())
        .def("__setitem__", (void (*)(gmtl::VecBase<int,3>*, const unsigned, int)) &gmtlWrapper::setArrayElement)
    );

    enum_< gmtl::VecBase<int,3>::Params >("Params")
        .value("Size", gmtl::VecBase<int,3>::Size)
    ;

    delete gmtl_VecBase_int_3_scope;

    scope* gmtl_VecBase_float_2_scope = new scope(
    class_< gmtl::VecBase<float,2> >("VecBase2f", init<  >())
        .def(init< const gmtl::VecBase<float,2> & >())
        .def(init< const float &, const float & >())
        .def(init< const float &, const float &, const float & >())
        .def(init< const float &, const float &, const float &, const float & >())
        .def("set", (void (gmtl::VecBase<float,2>::*)(const float *) )&gmtl::VecBase<float,2>::set)
        .def("set", (void (gmtl::VecBase<float,2>::*)(const float &) )&gmtl::VecBase<float,2>::set)
        .def("set", (void (gmtl::VecBase<float,2>::*)(const float &, const float &) )&gmtl::VecBase<float,2>::set)
        .def("set", (void (gmtl::VecBase<float,2>::*)(const float &, const float &, const float &) )&gmtl::VecBase<float,2>::set)
        .def("set", (void (gmtl::VecBase<float,2>::*)(const float &, const float &, const float &, const float &) )&gmtl::VecBase<float,2>::set)
        .def("__getitem__", (float& (gmtl::VecBase<float,2>::*)(const unsigned) ) &gmtl::VecBase<float,2>::operator[], return_value_policy<copy_non_const_reference>())
        .def("__setitem__", (void (*)(gmtl::VecBase<float,2>*, const unsigned, float)) &gmtlWrapper::setArrayElement)
    );

    enum_< gmtl::VecBase<float,2>::Params >("Params")
        .value("Size", gmtl::VecBase<float,2>::Size)
    ;

    delete gmtl_VecBase_float_2_scope;

    scope* gmtl_VecBase_int_2_scope = new scope(
    class_< gmtl::VecBase<int,2> >("VecBase2i", init<  >())
        .def(init< const gmtl::VecBase<int,2> & >())
        .def(init< const int &, const int & >())
        .def(init< const int &, const int &, const int & >())
        .def(init< const int &, const int &, const int &, const int & >())
        .def("set", (void (gmtl::VecBase<int,2>::*)(const int *) )&gmtl::VecBase<int,2>::set)
        .def("set", (void (gmtl::VecBase<int,2>::*)(const int &) )&gmtl::VecBase<int,2>::set)
        .def("set", (void (gmtl::VecBase<int,2>::*)(const int &, const int &) )&gmtl::VecBase<int,2>::set)
        .def("set", (void (gmtl::VecBase<int,2>::*)(const int &, const int &, const int &) )&gmtl::VecBase<int,2>::set)
        .def("set", (void (gmtl::VecBase<int,2>::*)(const int &, const int &, const int &, const int &) )&gmtl::VecBase<int,2>::set)
        .def("__getitem__", (int& (gmtl::VecBase<int,2>::*)(const unsigned) ) &gmtl::VecBase<int,2>::operator[], return_value_policy<copy_non_const_reference>())
        .def("__setitem__", (void (*)(gmtl::VecBase<int,2>*, const unsigned, int)) &gmtlWrapper::setArrayElement)
    );

    enum_< gmtl::VecBase<int,2>::Params >("Params")
        .value("Size", gmtl::VecBase<int,2>::Size)
    ;

    delete gmtl_VecBase_int_2_scope;

    scope* gmtl_EulerAngle_float_gmtl_XYZ_scope = new scope(
    class_< gmtl::EulerAngle<float,gmtl::XYZ> >("EulerAngleXYZf", init<  >())
        .def(init< const gmtl::EulerAngle<float,gmtl::XYZ> & >())
        .def(init< float, float, float >())
        .def("set", &gmtl::EulerAngle<float,gmtl::XYZ>::set)
        .def("__getitem__", (float& (gmtl::EulerAngle<float,gmtl::XYZ>::*)(const unsigned) ) &gmtl::EulerAngle<float,gmtl::XYZ>::operator[], return_value_policy<copy_non_const_reference>())
        .def("__setitem__", (void (*)(gmtl::EulerAngle<float,gmtl::XYZ>*, const unsigned, float)) &gmtlWrapper::setArrayElement)
    );

    enum_< gmtl::EulerAngle<float,gmtl::XYZ>::Params >("Params")
        .value("Order", gmtl::EulerAngle<float,gmtl::XYZ>::Order)
        .value("Size", gmtl::EulerAngle<float,gmtl::XYZ>::Size)
    ;

    delete gmtl_EulerAngle_float_gmtl_XYZ_scope;

    scope* gmtl_EulerAngle_float_gmtl_ZYX_scope = new scope(
    class_< gmtl::EulerAngle<float,gmtl::ZYX> >("EulerAngleZYXf", init<  >())
        .def(init< const gmtl::EulerAngle<float,gmtl::ZYX> & >())
        .def(init< float, float, float >())
        .def("set", &gmtl::EulerAngle<float,gmtl::ZYX>::set)
        .def("__getitem__", (float& (gmtl::EulerAngle<float,gmtl::ZYX>::*)(const unsigned) ) &gmtl::EulerAngle<float,gmtl::ZYX>::operator[], return_value_policy<copy_non_const_reference>())
        .def("__setitem__", (void (*)(gmtl::EulerAngle<float,gmtl::ZYX>*, const unsigned, float)) &gmtlWrapper::setArrayElement)
    );

    enum_< gmtl::EulerAngle<float,gmtl::ZYX>::Params >("Params")
        .value("Order", gmtl::EulerAngle<float,gmtl::ZYX>::Order)
        .value("Size", gmtl::EulerAngle<float,gmtl::ZYX>::Size)
    ;

    delete gmtl_EulerAngle_float_gmtl_ZYX_scope;

    scope* gmtl_EulerAngle_float_gmtl_ZXY_scope = new scope(
    class_< gmtl::EulerAngle<float,gmtl::ZXY> >("EulerAngleZXYf", init<  >())
        .def(init< const gmtl::EulerAngle<float,gmtl::ZXY> & >())
        .def(init< float, float, float >())
        .def("set", &gmtl::EulerAngle<float,gmtl::ZXY>::set)
        .def("__getitem__", (float& (gmtl::EulerAngle<float,gmtl::ZXY>::*)(const unsigned) ) &gmtl::EulerAngle<float,gmtl::ZXY>::operator[], return_value_policy<copy_non_const_reference>())
        .def("__setitem__", (void (*)(gmtl::EulerAngle<float,gmtl::ZXY>*, const unsigned, float)) &gmtlWrapper::setArrayElement)
    );

    enum_< gmtl::EulerAngle<float,gmtl::ZXY>::Params >("Params")
        .value("Order", gmtl::EulerAngle<float,gmtl::ZXY>::Order)
        .value("Size", gmtl::EulerAngle<float,gmtl::ZXY>::Size)
    ;

    delete gmtl_EulerAngle_float_gmtl_ZXY_scope;

    class_< gmtl::Sphere<float> >("Spheref", init<  >())
        .def(init< const gmtl::Point<float,3> &, const float & >())
        .def(init< const gmtl::Sphere<float> & >())
        .def_readwrite("mCenter", &gmtl::Sphere<float>::mCenter)
        .def_readwrite("mRadius", &gmtl::Sphere<float>::mRadius)
        .def("getCenter", &gmtl::Sphere<float>::getCenter, return_value_policy< copy_const_reference >())
        .def("getRadius", &gmtl::Sphere<float>::getRadius, return_value_policy< copy_const_reference >())
        .def("setCenter", &gmtl::Sphere<float>::setCenter)
        .def("setRadius", &gmtl::Sphere<float>::setRadius)
    ;

    class_< gmtl::Sphere<double> >("Sphered", init<  >())
        .def(init< const gmtl::Point<double,3> &, const double & >())
        .def(init< const gmtl::Sphere<double> & >())
        .def_readwrite("mCenter", &gmtl::Sphere<double>::mCenter)
        .def_readwrite("mRadius", &gmtl::Sphere<double>::mRadius)
        .def("getCenter", &gmtl::Sphere<double>::getCenter, return_value_policy< copy_const_reference >())
        .def("getRadius", &gmtl::Sphere<double>::getRadius, return_value_policy< copy_const_reference >())
        .def("setCenter", &gmtl::Sphere<double>::setCenter)
        .def("setRadius", &gmtl::Sphere<double>::setRadius)
    ;

    class_< gmtl::AABox<float> >("AABoxf", init<  >())
        .def(init< const gmtl::Point<float,3> &, const gmtl::Point<float,3> & >())
        .def(init< const gmtl::AABox<float> & >())
        .def_readwrite("mMin", &gmtl::AABox<float>::mMin)
        .def_readwrite("mMax", &gmtl::AABox<float>::mMax)
        .def_readwrite("mEmpty", &gmtl::AABox<float>::mEmpty)
        .def("getMin", &gmtl::AABox<float>::getMin, return_value_policy< copy_const_reference >())
        .def("getMax", &gmtl::AABox<float>::getMax, return_value_policy< copy_const_reference >())
        .def("isEmpty", &gmtl::AABox<float>::isEmpty)
        .def("setMin", &gmtl::AABox<float>::setMin)
        .def("setMax", &gmtl::AABox<float>::setMax)
        .def("setEmpty", &gmtl::AABox<float>::setEmpty)
    ;

    class_< gmtl::AABox<double> >("AABoxd", init<  >())
        .def(init< const gmtl::Point<double,3> &, const gmtl::Point<double,3> & >())
        .def(init< const gmtl::AABox<double> & >())
        .def_readwrite("mMin", &gmtl::AABox<double>::mMin)
        .def_readwrite("mMax", &gmtl::AABox<double>::mMax)
        .def_readwrite("mEmpty", &gmtl::AABox<double>::mEmpty)
        .def("getMin", &gmtl::AABox<double>::getMin, return_value_policy< copy_const_reference >())
        .def("getMax", &gmtl::AABox<double>::getMax, return_value_policy< copy_const_reference >())
        .def("isEmpty", &gmtl::AABox<double>::isEmpty)
        .def("setMin", &gmtl::AABox<double>::setMin)
        .def("setMax", &gmtl::AABox<double>::setMax)
        .def("setEmpty", &gmtl::AABox<double>::setEmpty)
    ;

    class_< gmtl::Ray<float> >("Rayf", init<  >())
        .def(init< const gmtl::Point<float,3> &, const gmtl::Vec<float,3> & >())
        .def(init< const gmtl::Ray<float> & >())
        .def_readwrite("mOrigin", &gmtl::Ray<float>::mOrigin)
        .def_readwrite("mDir", &gmtl::Ray<float>::mDir)
        .def("getOrigin", &gmtl::Ray<float>::getOrigin, return_value_policy< copy_const_reference >())
        .def("setOrigin", &gmtl::Ray<float>::setOrigin)
        .def("getDir", &gmtl::Ray<float>::getDir, return_value_policy< copy_const_reference >())
        .def("setDir", &gmtl::Ray<float>::setDir)
    ;

    class_< gmtl::Ray<double> >("Rayd", init<  >())
        .def(init< const gmtl::Point<double,3> &, const gmtl::Vec<double,3> & >())
        .def(init< const gmtl::Ray<double> & >())
        .def_readwrite("mOrigin", &gmtl::Ray<double>::mOrigin)
        .def_readwrite("mDir", &gmtl::Ray<double>::mDir)
        .def("getOrigin", &gmtl::Ray<double>::getOrigin, return_value_policy< copy_const_reference >())
        .def("setOrigin", &gmtl::Ray<double>::setOrigin)
        .def("getDir", &gmtl::Ray<double>::getDir, return_value_policy< copy_const_reference >())
        .def("setDir", &gmtl::Ray<double>::setDir)
    ;

    class_< gmtl::Plane<float> >("Planef", init<  >())
        .def(init< const gmtl::Point<float,3> &, const gmtl::Point<float,3> &, const gmtl::Point<float,3> & >())
        .def(init< const gmtl::Vec<float,3> &, const gmtl::Point<float,3> & >())
        .def(init< const gmtl::Vec<float,3> &, const float & >())
        .def(init< const gmtl::Plane<float> & >())
        .def_readwrite("mNorm", &gmtl::Plane<float>::mNorm)
        .def_readwrite("mOffset", &gmtl::Plane<float>::mOffset)
        .def("getNormal", &gmtl::Plane<float>::getNormal, return_value_policy< copy_const_reference >())
        .def("setNormal", &gmtl::Plane<float>::setNormal)
        .def("getOffset", &gmtl::Plane<float>::getOffset, return_value_policy< copy_const_reference >())
        .def("setOffset", &gmtl::Plane<float>::setOffset)
    ;

    class_< gmtl::Plane<double> >("Planed", init<  >())
        .def(init< const gmtl::Point<double,3> &, const gmtl::Point<double,3> &, const gmtl::Point<double,3> & >())
        .def(init< const gmtl::Vec<double,3> &, const gmtl::Point<double,3> & >())
        .def(init< const gmtl::Vec<double,3> &, const double & >())
        .def(init< const gmtl::Plane<double> & >())
        .def_readwrite("mNorm", &gmtl::Plane<double>::mNorm)
        .def_readwrite("mOffset", &gmtl::Plane<double>::mOffset)
        .def("getNormal", &gmtl::Plane<double>::getNormal, return_value_policy< copy_const_reference >())
        .def("setNormal", &gmtl::Plane<double>::setNormal)
        .def("getOffset", &gmtl::Plane<double>::getOffset, return_value_policy< copy_const_reference >())
        .def("setOffset", &gmtl::Plane<double>::setOffset)
    ;

    class_< gmtl::Tri<float> >("Trif", init<  >())
        .def(init< const gmtl::Point<float,3> &, const gmtl::Point<float,3> &, const gmtl::Point<float,3> & >())
        .def(init< const gmtl::Tri<float> & >())
        .def("edge", &gmtl::Tri<float>::edge)
        .def("set", &gmtl::Tri<float>::set)
    ;

    class_< gmtl::Tri<double> >("Trid", init<  >())
        .def(init< const gmtl::Point<double,3> &, const gmtl::Point<double,3> &, const gmtl::Point<double,3> & >())
        .def(init< const gmtl::Tri<double> & >())
        .def("edge", &gmtl::Tri<double>::edge)
        .def("set", &gmtl::Tri<double>::set)
    ;

    class_< gmtl::Tri<int> >("Trii", init<  >())
        .def(init< const gmtl::Point<int,3> &, const gmtl::Point<int,3> &, const gmtl::Point<int,3> & >())
        .def(init< const gmtl::Tri<int> & >())
        .def("edge", &gmtl::Tri<int>::edge)
        .def("set", &gmtl::Tri<int>::set)
    ;

    scope* gmtl_Matrix_float_3_3_scope = new scope(
    class_< gmtl::Matrix<float,3,3> >("Matrix33f", init<  >())
        .def(init< const gmtl::Matrix<float,3,3> & >())
        .def_readwrite("mState", &gmtl::Matrix<float,3,3>::mState)
        .def("set", (void (gmtl::Matrix<float,3,3>::*)(float, float, float, float) )&gmtl::Matrix<float,3,3>::set)
        .def("set", (void (gmtl::Matrix<float,3,3>::*)(float, float, float, float, float, float) )&gmtl::Matrix<float,3,3>::set)
        .def("set", (void (gmtl::Matrix<float,3,3>::*)(float, float, float, float, float, float, float, float, float) )&gmtl::Matrix<float,3,3>::set)
        .def("set", (void (gmtl::Matrix<float,3,3>::*)(float, float, float, float, float, float, float, float, float, float, float, float) )&gmtl::Matrix<float,3,3>::set)
        .def("set", (void (gmtl::Matrix<float,3,3>::*)(const float *) )&gmtl::Matrix<float,3,3>::set)
        .def("setTranspose", &gmtl::Matrix<float,3,3>::setTranspose)
        .def("isError", &gmtl::Matrix<float,3,3>::isError)
        .def("setError", &gmtl::Matrix<float,3,3>::setError)
        .def("__getitem__", &gmtl::Matrix<float,3,3>::operator[])
    );

    class_< gmtl::Matrix<float,3,3>::RowAccessor >("RowAccessor", no_init)
       .def("__getitem__", &gmtl::Matrix<float,3,3>::RowAccessor::operator[], return_value_policy<copy_non_const_reference>())
       .def("__setitem__", (void (*)(gmtl::Matrix33f::RowAccessor*, const unsigned, float)) &gmtlWrapper::setArrayElement)
    ;


    enum_< gmtl::Matrix<float,3,3>::Params >("Params")
        .value("Rows", gmtl::Matrix<float,3,3>::Rows)
        .value("Cols", gmtl::Matrix<float,3,3>::Cols)
    ;


    enum_< gmtl::Matrix<float,3,3>::XformState >("XformState")
        .value("ORTHONORMAL", gmtl::Matrix<float,3,3>::ORTHONORMAL)
        .value("XFORM_ERROR", gmtl::Matrix<float,3,3>::XFORM_ERROR)
        .value("FULL", gmtl::Matrix<float,3,3>::FULL)
        .value("ORTHOGONAL", gmtl::Matrix<float,3,3>::ORTHOGONAL)
        .value("AFFINE", gmtl::Matrix<float,3,3>::AFFINE)
        .value("IDENTITY", gmtl::Matrix<float,3,3>::IDENTITY)
    ;

    delete gmtl_Matrix_float_3_3_scope;

    scope* gmtl_Point_int_2_scope = new scope(
    class_< gmtl::Point<int,2>, bases< gmtl::VecBase<int,2> >  >("Point2i", init<  >())
        .def(init< const gmtl::Point<int,2> & >())
        .def(init< const gmtl::VecBase<int,2> & >())
        .def(init< const int &, const int & >())
        .def(init< const int &, const int &, const int & >())
        .def(init< const int &, const int &, const int &, const int & >())
    );

    enum_< gmtl::Point<int,2>::Params >("Params")
        .value("Size", gmtl::Point<int,2>::Size)
    ;

    delete gmtl_Point_int_2_scope;

    scope* gmtl_Vec_float_3_scope = new scope(
    class_< gmtl::Vec<float,3>, bases< gmtl::VecBase<float,3> >  >("Vec3f", init<  >())
        .def(init< const gmtl::Vec<float,3> & >())
        .def(init< const gmtl::VecBase<float,3> & >())
        .def(init< const float &, const float & >())
        .def(init< const float &, const float &, const float & >())
        .def(init< const float &, const float &, const float &, const float & >())
    );

    enum_< gmtl::Vec<float,3>::Params >("Params")
        .value("Size", gmtl::Vec<float,3>::Size)
    ;

    delete gmtl_Vec_float_3_scope;

    scope* gmtl_Vec_float_4_scope = new scope(
    class_< gmtl::Vec<float,4>, bases< gmtl::VecBase<float,4> >  >("Vec4f", init<  >())
        .def(init< const gmtl::Vec<float,4> & >())
        .def(init< const gmtl::VecBase<float,4> & >())
        .def(init< const float &, const float & >())
        .def(init< const float &, const float &, const float & >())
        .def(init< const float &, const float &, const float &, const float & >())
    );

    enum_< gmtl::Vec<float,4>::Params >("Params")
        .value("Size", gmtl::Vec<float,4>::Size)
    ;

    delete gmtl_Vec_float_4_scope;

    scope* gmtl_Point_int_3_scope = new scope(
    class_< gmtl::Point<int,3>, bases< gmtl::VecBase<int,3> >  >("Point3i", init<  >())
        .def(init< const gmtl::Point<int,3> & >())
        .def(init< const gmtl::VecBase<int,3> & >())
        .def(init< const int &, const int & >())
        .def(init< const int &, const int &, const int & >())
        .def(init< const int &, const int &, const int &, const int & >())
    );

    enum_< gmtl::Point<int,3>::Params >("Params")
        .value("Size", gmtl::Point<int,3>::Size)
    ;

    delete gmtl_Point_int_3_scope;

    scope* gmtl_Point_float_3_scope = new scope(
    class_< gmtl::Point<float,3>, bases< gmtl::VecBase<float,3> >  >("Point3f", init<  >())
        .def(init< const gmtl::Point<float,3> & >())
        .def(init< const gmtl::VecBase<float,3> & >())
        .def(init< const float &, const float & >())
        .def(init< const float &, const float &, const float & >())
        .def(init< const float &, const float &, const float &, const float & >())
    );

    enum_< gmtl::Point<float,3>::Params >("Params")
        .value("Size", gmtl::Point<float,3>::Size)
    ;

    delete gmtl_Point_float_3_scope;

    scope* gmtl_Point_float_2_scope = new scope(
    class_< gmtl::Point<float,2>, bases< gmtl::VecBase<float,2> >  >("Point2f", init<  >())
        .def(init< const gmtl::Point<float,2> & >())
        .def(init< const gmtl::VecBase<float,2> & >())
        .def(init< const float &, const float & >())
        .def(init< const float &, const float &, const float & >())
        .def(init< const float &, const float &, const float &, const float & >())
    );

    enum_< gmtl::Point<float,2>::Params >("Params")
        .value("Size", gmtl::Point<float,2>::Size)
    ;

    delete gmtl_Point_float_2_scope;

    scope* gmtl_AxisAngle_float_scope = new scope(
    class_< gmtl::AxisAngle<float>, bases< gmtl::VecBase<float,4> >  >("AxisAnglef", init<  >())
        .def(init< const gmtl::AxisAngle<float> & >())
        .def(init< const float &, const float &, const float &, const float & >())
        .def(init< const float &, const gmtl::Vec<float,3> & >())
        .def("set", (void (gmtl::AxisAngle<float>::*)(const float &, const float &, const float &, const float &) )&gmtl::AxisAngle<float>::set)
        .def("set", (void (gmtl::AxisAngle<float>::*)(const float &, const gmtl::Vec<float,3> &) )&gmtl::AxisAngle<float>::set)
        .def("setAxis", &gmtl::AxisAngle<float>::setAxis)
        .def("setAngle", &gmtl::AxisAngle<float>::setAngle)
        .def("getAxis", &gmtl::AxisAngle<float>::getAxis)
        .def("getAngle", &gmtl::AxisAngle<float>::getAngle, return_value_policy< copy_const_reference >())
    );

    enum_< gmtl::AxisAngle<float>::Params >("Params")
        .value("Size", gmtl::AxisAngle<float>::Size)
    ;

    delete gmtl_AxisAngle_float_scope;

    scope* gmtl_AxisAngle_double_scope = new scope(
    class_< gmtl::AxisAngle<double> >("AxisAngled", init<  >())
        .def(init< const gmtl::AxisAngle<double> & >())
        .def(init< const double &, const double &, const double &, const double & >())
        .def(init< const double &, const gmtl::Vec<double,3> & >())
        .def("set", (void (gmtl::AxisAngle<double>::*)(const double &, const double &, const double &, const double &) )&gmtl::AxisAngle<double>::set)
        .def("set", (void (gmtl::AxisAngle<double>::*)(const double &, const gmtl::Vec<double,3> &) )&gmtl::AxisAngle<double>::set)
        .def("setAxis", &gmtl::AxisAngle<double>::setAxis)
        .def("setAngle", &gmtl::AxisAngle<double>::setAngle)
        .def("getAxis", &gmtl::AxisAngle<double>::getAxis)
        .def("getAngle", &gmtl::AxisAngle<double>::getAngle, return_value_policy< copy_const_reference >())
    );

    enum_< gmtl::AxisAngle<double>::Params >("Params")
        .value("Size", gmtl::AxisAngle<double>::Size)
    ;

    delete gmtl_AxisAngle_double_scope;

    class_< gmtl::LineSeg<float>, bases< gmtl::Ray<float> >  >("LineSegf", init<  >())
        .def(init< const gmtl::Point<float,3> &, const gmtl::Vec<float,3> & >())
        .def(init< const gmtl::LineSeg<float> & >())
        .def(init< const gmtl::Point<float,3> &, const gmtl::Point<float,3> & >())
        .def("getLength", &gmtl::LineSeg<float>::getLength)
    ;

    class_< gmtl::LineSeg<double>, bases< gmtl::Ray<double> >  >("LineSegd", init<  >())
        .def(init< const gmtl::Point<double,3> &, const gmtl::Vec<double,3> & >())
        .def(init< const gmtl::LineSeg<double> & >())
        .def(init< const gmtl::Point<double,3> &, const gmtl::Point<double,3> & >())
        .def("getLength", &gmtl::LineSeg<double>::getLength)
    ;

}

