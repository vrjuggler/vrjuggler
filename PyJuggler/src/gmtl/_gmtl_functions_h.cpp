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


// Includes ====================================================================
#include <boost/python.hpp>
#include <gmtl-functions.h>

// Using =======================================================================
using namespace boost::python;

// Declarations ================================================================


namespace  {


BOOST_PYTHON_FUNCTION_OVERLOADS(isEqual_overloads_2_3, gmtl::isEqual, 2, 3)
BOOST_PYTHON_FUNCTION_OVERLOADS(isEquiv_overloads_2_3, gmtl::isEquiv, 2, 3)
BOOST_PYTHON_FUNCTION_OVERLOADS(isNormalized_overloads_1_2, gmtl::isNormalized, 1, 2)
BOOST_PYTHON_FUNCTION_OVERLOADS(setDirCos_overloads_4_7, gmtl::setDirCos, 4, 7)
BOOST_PYTHON_FUNCTION_OVERLOADS(slerp_overloads_4_5, gmtl::slerp, 4, 5)


}// namespace 


// Module ======================================================================
void _Export_gmtl_functions_h()
{
    def("add", (gmtl::Matrix<float,3,3> & (*)(gmtl::Matrix<float,3,3> &, const gmtl::Matrix<float,3,3> &, const gmtl::Matrix<float,3,3> &))&gmtl::add, return_internal_reference< 1 >());
    def("add", (gmtl::Matrix<float,4,4> & (*)(gmtl::Matrix<float,4,4> &, const gmtl::Matrix<float,4,4> &, const gmtl::Matrix<float,4,4> &))&gmtl::add, return_internal_reference< 1 >());
    def("add", (gmtl::Quat<double> & (*)(gmtl::Quat<double> &, const gmtl::Quat<double> &, const gmtl::Quat<double> &))&gmtl::add, return_internal_reference< 1 >());
    def("add", (gmtl::Quat<float> & (*)(gmtl::Quat<float> &, const gmtl::Quat<float> &, const gmtl::Quat<float> &))&gmtl::add, return_internal_reference< 1 >());
    def("center", (gmtl::Point<float,3> (*)(const gmtl::Tri<float> &))&gmtl::center);
    def("center", (gmtl::Point<double,3> (*)(const gmtl::Tri<double> &))&gmtl::center);
    def("center", (gmtl::Point<int,3> (*)(const gmtl::Tri<int> &))&gmtl::center);
    def("conj", (gmtl::Quat<float> & (*)(gmtl::Quat<float> &))&gmtl::conj, return_internal_reference< 1 >());
    def("conj", (gmtl::Quat<double> & (*)(gmtl::Quat<double> &))&gmtl::conj, return_internal_reference< 1 >());
    def("cross", (gmtl::Vec<float,3> & (*)(gmtl::Vec<float,3> &, const gmtl::Vec<float,3> &, const gmtl::Vec<float,3> &))&gmtl::cross, return_internal_reference< 1 >());
    def("cross", (gmtl::Vec<double,3> & (*)(gmtl::Vec<double,3> &, const gmtl::Vec<double,3> &, const gmtl::Vec<double,3> &))&gmtl::cross, return_internal_reference< 1 >());
    def("cross", (gmtl::Vec<int,3> & (*)(gmtl::Vec<int,3> &, const gmtl::Vec<int,3> &, const gmtl::Vec<int,3> &))&gmtl::cross, return_internal_reference< 1 >());
    def("distance", (double (*)(const gmtl::Plane<double> &, const gmtl::Point<double,3> &))&gmtl::distance);
    def("distance", (float (*)(const gmtl::Plane<float> &, const gmtl::Point<float,3> &))&gmtl::distance);
    def("distance", (float (*)(const gmtl::LineSeg<float> &, const gmtl::Point<float,3> &))&gmtl::distance);
    def("distance", (double (*)(const gmtl::LineSeg<double> &, const gmtl::Point<double,3> &))&gmtl::distance);
    def("div", (gmtl::Quat<double> & (*)(gmtl::Quat<double> &, const gmtl::Quat<double> &, double))&gmtl::div, return_internal_reference< 1 >());
    def("div", (gmtl::Quat<double> & (*)(gmtl::Quat<double> &, const gmtl::Quat<double> &, gmtl::Quat<double>))&gmtl::div, return_internal_reference< 1 >());
    def("div", (gmtl::Quat<float> & (*)(gmtl::Quat<float> &, const gmtl::Quat<float> &, float))&gmtl::div, return_internal_reference< 1 >());
    def("div", (gmtl::Quat<float> & (*)(gmtl::Quat<float> &, const gmtl::Quat<float> &, gmtl::Quat<float>))&gmtl::div, return_internal_reference< 1 >());
    def("dot", (double (*)(const gmtl::Vec<double,4> &, const gmtl::Vec<double,4> &))&gmtl::dot);
    def("dot", (double (*)(const gmtl::Quat<double> &, const gmtl::Quat<double> &))&gmtl::dot);
    def("dot", (float (*)(const gmtl::Quat<float> &, const gmtl::Quat<float> &))&gmtl::dot);
    def("dot", (float (*)(const gmtl::Vec<float,4> &, const gmtl::Vec<float,4> &))&gmtl::dot);
    def("dot", (float (*)(const gmtl::Vec<float,3> &, const gmtl::Vec<float,3> &))&gmtl::dot);
    def("dot", (double (*)(const gmtl::Vec<double,3> &, const gmtl::Vec<double,3> &))&gmtl::dot);
    def("exp", (gmtl::Quat<double> & (*)(gmtl::Quat<double> &))&gmtl::exp, return_internal_reference< 1 >());
    def("exp", (gmtl::Quat<float> & (*)(gmtl::Quat<float> &))&gmtl::exp, return_internal_reference< 1 >());
    def("extendVolume", (void (*)(gmtl::Sphere<double> &, const gmtl::Sphere<double> &))&gmtl::extendVolume, return_internal_reference< 1 >());
    def("extendVolume", (void (*)(gmtl::AABox<float> &, const gmtl::Point<float,3> &))&gmtl::extendVolume, return_internal_reference< 1 >());
    def("extendVolume", (void (*)(gmtl::AABox<double> &, const gmtl::AABox<double> &))&gmtl::extendVolume, return_internal_reference< 1 >());
    def("extendVolume", (void (*)(gmtl::AABox<double> &, const gmtl::Point<double,3> &))&gmtl::extendVolume, return_internal_reference< 1 >());
    def("extendVolume", (void (*)(gmtl::AABox<float> &, const gmtl::AABox<float> &))&gmtl::extendVolume, return_internal_reference< 1 >());
    def("extendVolume", (void (*)(gmtl::Sphere<double> &, const gmtl::Point<double,3> &))&gmtl::extendVolume, return_internal_reference< 1 >());
    def("extendVolume", (void (*)(gmtl::Sphere<float> &, const gmtl::Point<float,3> &))&gmtl::extendVolume, return_internal_reference< 1 >());
    def("extendVolume", (void (*)(gmtl::Sphere<float> &, const gmtl::Sphere<float> &))&gmtl::extendVolume, return_internal_reference< 1 >());
    def("findNearestPt", (gmtl::Point<float,3> (*)(const gmtl::LineSeg<float> &, const gmtl::Point<float,3> &))&gmtl::findNearestPt);
    def("findNearestPt", (gmtl::Point<double,3> (*)(const gmtl::LineSeg<double> &, const gmtl::Point<double,3> &))&gmtl::findNearestPt);
    def("findNearestPt", (double (*)(const gmtl::Plane<double> &, const gmtl::Point<double,3> &, gmtl::Point<double,3> &))&gmtl::findNearestPt);
    def("findNearestPt", (float (*)(const gmtl::Plane<float> &, const gmtl::Point<float,3> &, gmtl::Point<float,3> &))&gmtl::findNearestPt);
    def("identity", (gmtl::Matrix<float,3,3> & (*)(gmtl::Matrix<float,3,3> &))&gmtl::identity, return_internal_reference< 1 >());
    def("identity", (gmtl::Matrix<float,4,4> & (*)(gmtl::Matrix<float,4,4> &))&gmtl::identity, return_internal_reference< 1 >());
    def("identity", (gmtl::Matrix<double,4,4> & (*)(gmtl::Matrix<double,4,4> &))&gmtl::identity, return_internal_reference< 1 >());
    def("intersect", (bool (*)(const gmtl::AABox<double> &, const gmtl::Sphere<double> &))&gmtl::intersect);
    def("intersect", (bool (*)(const gmtl::Sphere<float> &, const gmtl::AABox<float> &))&gmtl::intersect);
    def("intersect", (bool (*)(const gmtl::Sphere<float> &, const gmtl::LineSeg<float> &, int &, float &, float &))&gmtl::intersect);
    def("intersect", (bool (*)(const gmtl::Sphere<double> &, const gmtl::LineSeg<double> &, int &, float &, float &))&gmtl::intersect);
    def("intersect", (bool (*)(const gmtl::Plane<float> &, const gmtl::Ray<float> &, float &))&gmtl::intersect);
    def("intersect", (bool (*)(const gmtl::Sphere<double> &, const gmtl::Ray<double> &, int &, float &, float &))&gmtl::intersect);
    def("intersect", (bool (*)(const gmtl::Sphere<double> &, const gmtl::AABox<double> &))&gmtl::intersect);
    def("intersect", (bool (*)(const gmtl::Sphere<float> &, const gmtl::Point<float,3> &))&gmtl::intersect);
    def("intersect", (bool (*)(const gmtl::Sphere<double> &, const gmtl::Point<double,3> &))&gmtl::intersect);
    def("intersect", (bool (*)(const gmtl::Sphere<float> &, const gmtl::Ray<float> &, int &, float &, float &))&gmtl::intersect);
    def("intersect", (bool (*)(const gmtl::AABox<float> &, const gmtl::Vec<float,3> &, const gmtl::AABox<float> &, const gmtl::Vec<float,3> &, float &, float &))&gmtl::intersect);
    def("intersect", (bool (*)(const gmtl::AABox<double> &, const gmtl::Point<double,3> &))&gmtl::intersect);
    def("intersect", (bool (*)(const gmtl::AABox<float> &, const gmtl::Point<float,3> &))&gmtl::intersect);
    def("intersect", (bool (*)(const gmtl::AABox<double> &, const gmtl::AABox<double> &))&gmtl::intersect);
    def("intersect", (bool (*)(const gmtl::AABox<float> &, const gmtl::Sphere<float> &))&gmtl::intersect);
    def("intersect", (bool (*)(const gmtl::Sphere<double> &, const gmtl::Vec<double,3> &, const gmtl::Sphere<double> &, const gmtl::Vec<double,3> &, double &, double &))&gmtl::intersect);
    def("intersect", (bool (*)(const gmtl::Sphere<float> &, const gmtl::Vec<float,3> &, const gmtl::Sphere<float> &, const gmtl::Vec<float,3> &, float &, float &))&gmtl::intersect);
    def("intersect", (bool (*)(const gmtl::AABox<double> &, const gmtl::Vec<double,3> &, const gmtl::AABox<double> &, const gmtl::Vec<double,3> &, double &, double &))&gmtl::intersect);
    def("intersect", (bool (*)(const gmtl::AABox<float> &, const gmtl::AABox<float> &))&gmtl::intersect);
    def("intersect", (bool (*)(const gmtl::Tri<float> &, const gmtl::LineSeg<float> &, float &, float &, float &))&gmtl::intersect);
    def("intersect", (bool (*)(const gmtl::Tri<double> &, const gmtl::Ray<double> &, float &, float &, float &))&gmtl::intersect);
    def("intersect", (bool (*)(const gmtl::Tri<double> &, const gmtl::LineSeg<double> &, float &, float &, float &))&gmtl::intersect);
    def("intersect", (bool (*)(const gmtl::Tri<float> &, const gmtl::Ray<float> &, float &, float &, float &))&gmtl::intersect);
    def("intersect", (bool (*)(const gmtl::Plane<double> &, const gmtl::Ray<double> &, double &))&gmtl::intersect);
    def("invert", (gmtl::Quat<double> & (*)(gmtl::Quat<double> &))&gmtl::invert, return_internal_reference< 1 >());
    def("invert", (gmtl::Quat<float> & (*)(gmtl::Quat<float> &))&gmtl::invert, return_internal_reference< 1 >());
    def("invert", (gmtl::Matrix<float,4,4> & (*)(gmtl::Matrix<float,4,4> &, const gmtl::Matrix<float,4,4> &))&gmtl::invert, return_internal_reference< 1 >());
    def("invert", (gmtl::Matrix<float,3,3> & (*)(gmtl::Matrix<float,3,3> &))&gmtl::invert, return_internal_reference< 1 >());
    def("invert", (gmtl::Matrix<double,3,3> & (*)(gmtl::Matrix<double,3,3> &, const gmtl::Matrix<double,3,3> &))&gmtl::invert, return_internal_reference< 1 >());
    def("invert", (gmtl::Matrix<float,3,3> & (*)(gmtl::Matrix<float,3,3> &, const gmtl::Matrix<float,3,3> &))&gmtl::invert, return_internal_reference< 1 >());
    def("invert", (gmtl::Matrix<float,4,4> & (*)(gmtl::Matrix<float,4,4> &))&gmtl::invert, return_internal_reference< 1 >());
    def("invert", (gmtl::Matrix<double,4,4> & (*)(gmtl::Matrix<double,4,4> &, const gmtl::Matrix<double,4,4> &))&gmtl::invert, return_internal_reference< 1 >());
    def("invert", (gmtl::Quat<double> & (*)(gmtl::Quat<double> &))&gmtl::invert, return_internal_reference< 1 >());
    def("invert", (gmtl::Quat<float> & (*)(gmtl::Quat<float> &))&gmtl::invert, return_internal_reference< 1 >());
    def("invert", (gmtl::Matrix<float,4,4> & (*)(gmtl::Matrix<float,4,4> &, const gmtl::Matrix<float,4,4> &))&gmtl::invert, return_internal_reference< 1 >());
    def("invert", (gmtl::Matrix<float,3,3> & (*)(gmtl::Matrix<float,3,3> &))&gmtl::invert, return_internal_reference< 1 >());
    def("invert", (gmtl::Matrix<double,3,3> & (*)(gmtl::Matrix<double,3,3> &, const gmtl::Matrix<double,3,3> &))&gmtl::invert, return_internal_reference< 1 >());
    def("invert", (gmtl::Matrix<float,3,3> & (*)(gmtl::Matrix<float,3,3> &, const gmtl::Matrix<float,3,3> &))&gmtl::invert, return_internal_reference< 1 >());
    def("invert", (gmtl::Matrix<float,4,4> & (*)(gmtl::Matrix<float,4,4> &))&gmtl::invert, return_internal_reference< 1 >());
    def("invert", (gmtl::Matrix<double,4,4> & (*)(gmtl::Matrix<double,4,4> &, const gmtl::Matrix<double,4,4> &))&gmtl::invert, return_internal_reference< 1 >());
    def("invertFull", (gmtl::Matrix<float,3,3> & (*)(gmtl::Matrix<float,3,3> &, const gmtl::Matrix<float,3,3> &))&gmtl::invertFull, return_internal_reference< 1 >());
    def("invertFull", (gmtl::Matrix<double,3,3> & (*)(gmtl::Matrix<double,3,3> &, const gmtl::Matrix<double,3,3> &))&gmtl::invertFull, return_internal_reference< 1 >());
    def("invertFull", (gmtl::Matrix<float,4,4> & (*)(gmtl::Matrix<float,4,4> &, const gmtl::Matrix<float,4,4> &))&gmtl::invertFull, return_internal_reference< 1 >());
    def("invertFull", (gmtl::Matrix<double,4,4> & (*)(gmtl::Matrix<double,4,4> &, const gmtl::Matrix<double,4,4> &))&gmtl::invertFull, return_internal_reference< 1 >());
    def("isEqual", (bool (*)(const gmtl::Tri<double> &, const gmtl::Tri<double> &, const double &))&gmtl::isEqual);
    def("isEqual", (bool (*)(const gmtl::VecBase<float,4> &, const gmtl::VecBase<float,4> &, const float &))&gmtl::isEqual);
    def("isEqual", (bool (*)(const gmtl::VecBase<double,4> &, const gmtl::VecBase<double,4> &, const double &))&gmtl::isEqual);
    def("isEqual", (bool (*)(const gmtl::VecBase<float,3> &, const gmtl::VecBase<float,3> &, const float &))&gmtl::isEqual);
    def("isEqual", (bool (*)(const gmtl::VecBase<double,3> &, const gmtl::VecBase<double,3> &, const double &))&gmtl::isEqual);
    def("isEqual", (bool (*)(const gmtl::Tri<float> &, const gmtl::Tri<float> &, const float &))&gmtl::isEqual);
    def("isEqual", (bool (*)(const gmtl::Quat<double> &, const gmtl::Quat<double> &, double))&gmtl::isEqual, isEqual_overloads_2_3());
    def("isEqual", (bool (*)(const gmtl::Quat<float> &, const gmtl::Quat<float> &, float))&gmtl::isEqual, isEqual_overloads_2_3());
    def("isEqual", (bool (*)(const gmtl::EulerAngle<float,gmtl::XYZ> &, const gmtl::EulerAngle<float,gmtl::XYZ> &, const float &))&gmtl::isEqual, isEqual_overloads_2_3());
    def("isEqual", (bool (*)(const gmtl::EulerAngle<float,gmtl::ZXY> &, const gmtl::EulerAngle<float,gmtl::ZXY> &, const float &))&gmtl::isEqual, isEqual_overloads_2_3());
    def("isEqual", (bool (*)(const gmtl::EulerAngle<float,gmtl::ZYX> &, const gmtl::EulerAngle<float,gmtl::ZYX> &, const float &))&gmtl::isEqual, isEqual_overloads_2_3());
    def("isEqual", (bool (*)(const gmtl::Plane<double> &, const gmtl::Plane<double> &, const double &))&gmtl::isEqual);
    def("isEqual", (bool (*)(const gmtl::Plane<float> &, const gmtl::Plane<float> &, const float &))&gmtl::isEqual);
    def("isEqual", (bool (*)(const gmtl::AxisAngle<double> &, const gmtl::AxisAngle<double> &, const double &))&gmtl::isEqual, isEqual_overloads_2_3());
    def("isEqual", (bool (*)(const gmtl::AxisAngle<float> &, const gmtl::AxisAngle<float> &, const float &))&gmtl::isEqual, isEqual_overloads_2_3());
    def("isEqual", (bool (*)(const gmtl::LineSeg<double> &, const gmtl::LineSeg<double> &, const double &))&gmtl::isEqual);
    def("isEqual", (bool (*)(const gmtl::LineSeg<float> &, const gmtl::LineSeg<float> &, const float &))&gmtl::isEqual);
    def("isEqual", (bool (*)(const gmtl::Sphere<double> &, const gmtl::Sphere<double> &, const double &))&gmtl::isEqual);
    def("isEqual", (bool (*)(const gmtl::Sphere<float> &, const gmtl::Sphere<float> &, const float &))&gmtl::isEqual);
    def("isEqual", (bool (*)(const gmtl::AABox<double> &, const gmtl::AABox<double> &, const double &))&gmtl::isEqual);
    def("isEqual", (bool (*)(const gmtl::AABox<float> &, const gmtl::AABox<float> &, const float &))&gmtl::isEqual);
    def("isEqual", (bool (*)(const gmtl::Matrix<float,4,4> &, const gmtl::Matrix<float,4,4> &, const float &))&gmtl::isEqual, isEqual_overloads_2_3());
    def("isEqual", (bool (*)(const gmtl::Matrix<float,3,3> &, const gmtl::Matrix<float,3,3> &, const float &))&gmtl::isEqual, isEqual_overloads_2_3());
    def("isEquiv", (bool (*)(const gmtl::Quat<float> &, const gmtl::Quat<float> &, float))&gmtl::isEquiv, isEquiv_overloads_2_3());
    def("isEquiv", (bool (*)(const gmtl::Quat<double> &, const gmtl::Quat<double> &, double))&gmtl::isEquiv, isEquiv_overloads_2_3());
    def("isInVolume", (bool (*)(const gmtl::Sphere<float> &, const gmtl::Point<float,3> &))&gmtl::isInVolume);
    def("isInVolume", (bool (*)(const gmtl::Sphere<float> &, const gmtl::Sphere<float> &))&gmtl::isInVolume);
    def("isInVolume", (bool (*)(const gmtl::Sphere<double> &, const gmtl::Point<double,3> &))&gmtl::isInVolume);
    def("isInVolume", (bool (*)(const gmtl::Sphere<double> &, const gmtl::Sphere<double> &))&gmtl::isInVolume);
    def("isNormalized", (bool (*)(const gmtl::Vec<float,3> &, const float))&gmtl::isNormalized, isNormalized_overloads_1_2());
    def("isNormalized", (bool (*)(const gmtl::Quat<float> &, const float))&gmtl::isNormalized, isNormalized_overloads_1_2());
    def("isNormalized", (bool (*)(const gmtl::Quat<double> &, const double))&gmtl::isNormalized, isNormalized_overloads_1_2());
    def("isNormalized", (bool (*)(const gmtl::Vec<float,4> &, const float))&gmtl::isNormalized, isNormalized_overloads_1_2());
    def("isOnVolume", (bool (*)(const gmtl::Sphere<double> &, const gmtl::Point<double,3> &, const double &))&gmtl::isOnVolume);
    def("isOnVolume", (bool (*)(const gmtl::Sphere<float> &, const gmtl::Point<float,3> &, const float &))&gmtl::isOnVolume);
    def("length", (double (*)(const gmtl::Vec<double,3> &))&gmtl::length);
    def("length", (float (*)(const gmtl::Vec<float,4> &))&gmtl::length);
    def("length", (float (*)(const gmtl::Vec<float,3> &))&gmtl::length);
    def("length", (double (*)(const gmtl::Quat<double> &))&gmtl::length);
    def("length", (float (*)(const gmtl::Quat<float> &))&gmtl::length);
    def("lengthSquared", (float (*)(const gmtl::Vec<float,3> &))&gmtl::lengthSquared);
    def("lengthSquared", (float (*)(const gmtl::Quat<float> &))&gmtl::lengthSquared);
    def("lengthSquared", (double (*)(const gmtl::Vec<double,3> &))&gmtl::lengthSquared);
    def("lengthSquared", (float (*)(const gmtl::Vec<float,4> &))&gmtl::lengthSquared);
    def("lengthSquared", (double (*)(const gmtl::Quat<double> &))&gmtl::lengthSquared);
    def("lerp", (gmtl::VecBase<float,4> & (*)(gmtl::VecBase<float,4> &, const float &, const gmtl::VecBase<float,4> &, const gmtl::VecBase<float,4> &))&gmtl::lerp, return_internal_reference< 1 >());
    def("lerp", (gmtl::VecBase<double,4> & (*)(gmtl::VecBase<double,4> &, const double &, const gmtl::VecBase<double,4> &, const gmtl::VecBase<double,4> &))&gmtl::lerp, return_internal_reference< 1 >());
    def("lerp", (gmtl::VecBase<float,3> & (*)(gmtl::VecBase<float,3> &, const float &, const gmtl::VecBase<float,3> &, const gmtl::VecBase<float,3> &))&gmtl::lerp, return_internal_reference< 1 >());
    def("lerp", (gmtl::VecBase<double,3> & (*)(gmtl::VecBase<double,3> &, const double &, const gmtl::VecBase<double,3> &, const gmtl::VecBase<double,3> &))&gmtl::lerp, return_internal_reference< 1 >());
    def("lerp", (gmtl::Quat<double> & (*)(gmtl::Quat<double> &, const double, const gmtl::Quat<double> &, const gmtl::Quat<double> &))&gmtl::lerp, return_internal_reference< 1 >());
    def("lerp", (gmtl::Quat<float> & (*)(gmtl::Quat<float> &, const float, const gmtl::Quat<float> &, const gmtl::Quat<float> &))&gmtl::lerp, return_internal_reference< 1 >());
    def("log", (gmtl::Quat<double> & (*)(gmtl::Quat<double> &))&gmtl::log, return_internal_reference< 1 >());
    def("log", (gmtl::Quat<float> & (*)(gmtl::Quat<float> &))&gmtl::log, return_internal_reference< 1 >());
    def("makeConj", (gmtl::Quat<float> (*)(const gmtl::Quat<float> &))&gmtl::makeConj);
    def("makeConj", (gmtl::Quat<double> (*)(const gmtl::Quat<double> &))&gmtl::makeConj);
    def("makeCross", (gmtl::Vec<float,3> (*)(const gmtl::Vec<float,3> &, const gmtl::Vec<float,3> &))&gmtl::makeCross);
    def("makeCross", (gmtl::Vec<double,3> (*)(const gmtl::Vec<double,3> &, const gmtl::Vec<double,3> &))&gmtl::makeCross);
    def("makeCross", (gmtl::Vec<int,3> (*)(const gmtl::Vec<int,3> &, const gmtl::Vec<int,3> &))&gmtl::makeCross);
    def("makeInvert", (gmtl::Matrix<double,4,4> (*)(const gmtl::Matrix<double,4,4> &))&gmtl::makeInvert);
    def("makeInvert", (gmtl::Matrix<double,3,3> (*)(const gmtl::Matrix<double,3,3> &))&gmtl::makeInvert);
    def("makeInvert", (gmtl::Matrix<float,4,4> (*)(const gmtl::Matrix<float,4,4> &))&gmtl::makeInvert);
    def("makeInvert", (gmtl::Matrix<float,3,3> (*)(const gmtl::Matrix<float,3,3> &))&gmtl::makeInvert);
    def("makeInvert", (gmtl::Quat<float> (*)(const gmtl::Quat<float> &))&gmtl::makeInvert);
    def("makeInvert", (gmtl::Quat<double> (*)(const gmtl::Quat<double> &))&gmtl::makeInvert);
    def("makeNormal", (gmtl::AxisAngle<double> (*)(const gmtl::AxisAngle<double> &))&gmtl::makeNormal);
    def("makeNormal", (gmtl::AxisAngle<float> (*)(const gmtl::AxisAngle<float> &))&gmtl::makeNormal);
    def("makeNormal", (gmtl::Vec<float,3> (*)(gmtl::Vec<float,3>))&gmtl::makeNormal);
    def("makeNormal", (gmtl::Vec<float,4> (*)(gmtl::Vec<float,4>))&gmtl::makeNormal);
    def("makeNormal", (gmtl::Vec<double,3> (*)(gmtl::Vec<double,3>))&gmtl::makeNormal);
    def("makeNormal", (gmtl::Quat<float> (*)(const gmtl::Quat<float> &))&gmtl::makeNormal);
    def("makeNormal", (gmtl::Quat<double> (*)(const gmtl::Quat<double> &))&gmtl::makeNormal);
    def("makePure", (gmtl::Quat<float> (*)(const gmtl::Vec<float,3> &))&gmtl::makePure);
    def("makePure", (gmtl::Quat<double> (*)(const gmtl::Vec<double,3> &))&gmtl::makePure);
    def("makeTranspose", (gmtl::Matrix<float,4,4> (*)(const gmtl::Matrix<float,4,4> &))&gmtl::makeTranspose);
    def("makeTranspose", (gmtl::Matrix<float,3,3> (*)(const gmtl::Matrix<float,3,3> &))&gmtl::makeTranspose);
    def("makeTranspose", (gmtl::Matrix<double,4,4> (*)(const gmtl::Matrix<double,4,4> &))&gmtl::makeTranspose);
    def("makeTranspose", (gmtl::Matrix<double,3,3> (*)(const gmtl::Matrix<double,3,3> &))&gmtl::makeTranspose);
    def("makeVec", (gmtl::Vec<double,3> (*)(const gmtl::Quat<double> &))&gmtl::makeVec);
    def("makeVec", (gmtl::Vec<float,3> (*)(const gmtl::Quat<float> &))&gmtl::makeVec);
    def("makeVolume", (void (*)(gmtl::Sphere<float> &, const std::vector<gmtl::Point<float, 3>,std::allocator<gmtl::Point<float, 3> > > &))&gmtl::makeVolume, return_internal_reference< 1 >());
    def("makeVolume", (void (*)(gmtl::AABox<double> &, const gmtl::Sphere<double> &))&gmtl::makeVolume, return_internal_reference< 1 >());
    def("makeVolume", (void (*)(gmtl::Sphere<double> &, const std::vector<gmtl::Point<double, 3>,std::allocator<gmtl::Point<double, 3> > > &))&gmtl::makeVolume, return_internal_reference< 1 >());
    def("makeVolume", (void (*)(gmtl::AABox<float> &, const gmtl::Sphere<float> &))&gmtl::makeVolume, return_internal_reference< 1 >());
    def("makeXRot", (float (*)(const gmtl::Matrix<float,4,4> &))&gmtl::makeXRot);
    def("makeXRot", (double (*)(const gmtl::Matrix<double,4,4> &))&gmtl::makeXRot);
    def("makeXRot", (double (*)(const gmtl::Matrix<double,3,3> &))&gmtl::makeXRot);
    def("makeXRot", (float (*)(const gmtl::Matrix<float,3,3> &))&gmtl::makeXRot);
    def("makeYRot", (float (*)(const gmtl::Matrix<float,3,3> &))&gmtl::makeYRot);
    def("makeYRot", (double (*)(const gmtl::Matrix<double,4,4> &))&gmtl::makeYRot);
    def("makeYRot", (float (*)(const gmtl::Matrix<float,4,4> &))&gmtl::makeYRot);
    def("makeYRot", (double (*)(const gmtl::Matrix<double,3,3> &))&gmtl::makeYRot);
    def("makeZRot", (double (*)(const gmtl::Matrix<double,4,4> &))&gmtl::makeZRot);
    def("makeZRot", (double (*)(const gmtl::Matrix<double,3,3> &))&gmtl::makeZRot);
    def("makeZRot", (float (*)(const gmtl::Matrix<float,4,4> &))&gmtl::makeZRot);
    def("makeZRot", (float (*)(const gmtl::Matrix<float,3,3> &))&gmtl::makeZRot);
    def("mult", (gmtl::Matrix<float,4,4> & (*)(gmtl::Matrix<float,4,4> &, const gmtl::Matrix<float,4,4> &, const float &))&gmtl::mult, return_internal_reference< 1 >());
    def("mult", (gmtl::Matrix<float,3,3> & (*)(gmtl::Matrix<float,3,3> &, const gmtl::Matrix<float,3,3> &, const float &))&gmtl::mult, return_internal_reference< 1 >());
    def("mult", (gmtl::Matrix<float,4,4> & (*)(gmtl::Matrix<float,4,4> &, float))&gmtl::mult, return_internal_reference< 1 >());
    def("mult", (gmtl::Matrix<float,3,3> & (*)(gmtl::Matrix<float,3,3> &, float))&gmtl::mult, return_internal_reference< 1 >());
    def("mult", (gmtl::Matrix<float,4,4> & (*)(gmtl::Matrix<float,4,4> &, const gmtl::Matrix<float,4,4> &, const gmtl::Matrix<float,4,4> &))&gmtl::mult, return_internal_reference< 1 >());
    def("mult", (gmtl::Matrix<float,3,3> & (*)(gmtl::Matrix<float,3,3> &, const gmtl::Matrix<float,3,3> &, const gmtl::Matrix<float,3,3> &))&gmtl::mult, return_internal_reference< 1 >());
    def("mult", (gmtl::Quat<double> & (*)(gmtl::Quat<double> &, const gmtl::Quat<double> &, const gmtl::Quat<double> &))&gmtl::mult, return_internal_reference< 1 >());
    def("mult", (gmtl::Quat<float> & (*)(gmtl::Quat<float> &, const gmtl::Quat<float> &, const gmtl::Quat<float> &))&gmtl::mult, return_internal_reference< 1 >());
    def("mult", (gmtl::Quat<double> & (*)(gmtl::Quat<double> &, const gmtl::Quat<double> &, double))&gmtl::mult, return_internal_reference< 1 >());
    def("mult", (gmtl::Quat<float> & (*)(gmtl::Quat<float> &, const gmtl::Quat<float> &, float))&gmtl::mult, return_internal_reference< 1 >());
    def("negate", (gmtl::Quat<double> & (*)(gmtl::Quat<double> &))&gmtl::negate, return_internal_reference< 1 >());
    def("negate", (gmtl::Quat<float> & (*)(gmtl::Quat<float> &))&gmtl::negate, return_internal_reference< 1 >());
    def("normalize", (float (*)(gmtl::Vec<float,3> &))&gmtl::normalize);
    def("normalize", (float (*)(gmtl::Vec<float,4> &))&gmtl::normalize);
    def("normalize", (double (*)(gmtl::Vec<double,3> &))&gmtl::normalize);
    def("normalize", (gmtl::Quat<float> & (*)(gmtl::Quat<float> &))&gmtl::normalize, return_internal_reference< 1 >());
    def("normalize", (gmtl::Quat<double> & (*)(gmtl::Quat<double> &))&gmtl::normalize, return_internal_reference< 1 >());
    def("postMult", (gmtl::Matrix<float,3,3> & (*)(gmtl::Matrix<float,3,3> &, const gmtl::Matrix<float,3,3> &))&gmtl::postMult, return_internal_reference< 1 >());
    def("postMult", (gmtl::Matrix<float,4,4> & (*)(gmtl::Matrix<float,4,4> &, const gmtl::Matrix<float,4,4> &))&gmtl::postMult, return_internal_reference< 1 >());
    def("preMult", (gmtl::Matrix<float,4,4> & (*)(gmtl::Matrix<float,4,4> &, const gmtl::Matrix<float,4,4> &))&gmtl::preMult, return_internal_reference< 1 >());
    def("preMult", (gmtl::Matrix<float,3,3> & (*)(gmtl::Matrix<float,3,3> &, const gmtl::Matrix<float,3,3> &))&gmtl::preMult, return_internal_reference< 1 >());
    def("reflect", (gmtl::VecBase<float,4> & (*)(gmtl::VecBase<float,4> &, const gmtl::VecBase<float,4> &, const gmtl::Vec<float,4> &))&gmtl::reflect, return_internal_reference< 1 >());
    def("reflect", (gmtl::VecBase<double,4> & (*)(gmtl::VecBase<double,4> &, const gmtl::VecBase<double,4> &, const gmtl::Vec<double,4> &))&gmtl::reflect, return_internal_reference< 1 >());
    def("reflect", (gmtl::VecBase<float,3> & (*)(gmtl::VecBase<float,3> &, const gmtl::VecBase<float,3> &, const gmtl::Vec<float,3> &))&gmtl::reflect, return_internal_reference< 1 >());
    def("reflect", (gmtl::VecBase<double,3> & (*)(gmtl::VecBase<double,3> &, const gmtl::VecBase<double,3> &, const gmtl::Vec<double,3> &))&gmtl::reflect, return_internal_reference< 1 >());
    def("set", (gmtl::EulerAngle<float,gmtl::ZXY> & (*)(gmtl::EulerAngle<float,gmtl::ZXY> &, const gmtl::Matrix<float,4,4> &))&gmtl::set, return_internal_reference< 1 >());
    def("set", (gmtl::EulerAngle<float,gmtl::ZYX> & (*)(gmtl::EulerAngle<float,gmtl::ZYX> &, const gmtl::Matrix<float,4,4> &))&gmtl::set, return_internal_reference< 1 >());
    def("set", (gmtl::EulerAngle<float,gmtl::XYZ> & (*)(gmtl::EulerAngle<float,gmtl::XYZ> &, const gmtl::Matrix<float,4,4> &))&gmtl::set, return_internal_reference< 1 >());
    def("set", (gmtl::AxisAngle<double> & (*)(gmtl::AxisAngle<double> &, gmtl::Quat<double>))&gmtl::set, return_internal_reference< 1 >());
    def("set", (gmtl::Matrix<float,3,3> & (*)(gmtl::Matrix<float,3,3> &, const gmtl::Quat<float> &))&gmtl::set, return_internal_reference< 1 >());
    def("set", (gmtl::Matrix<float,4,4> & (*)(gmtl::Matrix<float,4,4> &, const gmtl::Quat<float> &))&gmtl::set, return_internal_reference< 1 >());
    def("set", (gmtl::Matrix<double,3,3> & (*)(gmtl::Matrix<double,3,3> &, const gmtl::Quat<double> &))&gmtl::set, return_internal_reference< 1 >());
    def("set", (gmtl::Matrix<double,4,4> & (*)(gmtl::Matrix<double,4,4> &, const gmtl::Quat<double> &))&gmtl::set, return_internal_reference< 1 >());
    def("set", (gmtl::Quat<double> & (*)(gmtl::Quat<double> &, const gmtl::AxisAngle<double> &))&gmtl::set, return_internal_reference< 1 >());
    def("set", (gmtl::Matrix<double,4,4> & (*)(gmtl::Matrix<double,4,4> &, const gmtl::AxisAngle<double> &))&gmtl::set, return_internal_reference< 1 >());
    def("set", (gmtl::Matrix<float,4,4> & (*)(gmtl::Matrix<float,4,4> &, const gmtl::AxisAngle<float> &))&gmtl::set, return_internal_reference< 1 >());
    def("set", (gmtl::Matrix<float,4,4> & (*)(gmtl::Matrix<float,4,4> &, const gmtl::EulerAngle<float,gmtl::ZYX> &))&gmtl::set, return_internal_reference< 1 >());
    def("set", (gmtl::Matrix<float,4,4> & (*)(gmtl::Matrix<float,4,4> &, const gmtl::EulerAngle<float,gmtl::XYZ> &))&gmtl::set, return_internal_reference< 1 >());
    def("set", (gmtl::Matrix<float,4,4> & (*)(gmtl::Matrix<float,4,4> &, const gmtl::EulerAngle<float,gmtl::ZXY> &))&gmtl::set, return_internal_reference< 1 >());
    def("set", (gmtl::AxisAngle<float> & (*)(gmtl::AxisAngle<float> &, gmtl::Quat<float>))&gmtl::set, return_internal_reference< 1 >());
    def("set", (gmtl::Matrix<double,4,4> & (*)(gmtl::Matrix<double,4,4> &, const gmtl::EulerAngle<double,gmtl::XYZ> &))&gmtl::set, return_internal_reference< 1 >());
    def("set", (gmtl::Matrix<double,4,4> & (*)(gmtl::Matrix<double,4,4> &, const gmtl::EulerAngle<double,gmtl::ZYX> &))&gmtl::set, return_internal_reference< 1 >());
    def("set", (gmtl::Matrix<double,4,4> & (*)(gmtl::Matrix<double,4,4> &, const gmtl::EulerAngle<double,gmtl::ZXY> &))&gmtl::set, return_internal_reference< 1 >());
    def("set", (gmtl::Quat<float> & (*)(gmtl::Quat<float> &, const gmtl::AxisAngle<float> &))&gmtl::set, return_internal_reference< 1 >());
    def("set", (gmtl::Quat<float> & (*)(gmtl::Quat<float> &, const gmtl::EulerAngle<float,gmtl::XYZ> &))&gmtl::set, return_internal_reference< 1 >());
    def("set", (gmtl::Quat<float> & (*)(gmtl::Quat<float> &, const gmtl::EulerAngle<float,gmtl::ZYX> &))&gmtl::set, return_internal_reference< 1 >());
    def("set", (gmtl::Quat<float> & (*)(gmtl::Quat<float> &, const gmtl::Matrix<float,4,4> &))&gmtl::set, return_internal_reference< 1 >());
    def("set", (gmtl::Quat<double> & (*)(gmtl::Quat<double> &, const gmtl::Matrix<double,4,4> &))&gmtl::set, return_internal_reference< 1 >());
    def("set", (gmtl::Quat<float> & (*)(gmtl::Quat<float> &, const gmtl::EulerAngle<float,gmtl::ZXY> &))&gmtl::set, return_internal_reference< 1 >());
    def("setAxes", (gmtl::Matrix<double,4,4> & (*)(gmtl::Matrix<double,4,4> &, const gmtl::Vec<double,3> &, const gmtl::Vec<double,3> &, const gmtl::Vec<double,3> &))&gmtl::setAxes, return_internal_reference< 1 >());
    def("setAxes", (gmtl::Matrix<float,4,4> & (*)(gmtl::Matrix<float,4,4> &, const gmtl::Vec<float,3> &, const gmtl::Vec<float,3> &, const gmtl::Vec<float,3> &))&gmtl::setAxes, return_internal_reference< 1 >());
    def("setDirCos", (gmtl::Matrix<double,4,4> & (*)(gmtl::Matrix<double,4,4> &, const gmtl::Vec<double,3> &, const gmtl::Vec<double,3> &, const gmtl::Vec<double,3> &, const gmtl::Vec<double,3> &, const gmtl::Vec<double,3> &, const gmtl::Vec<double,3> &))&gmtl::setDirCos, return_internal_reference< 1 >(), setDirCos_overloads_4_7());
    def("setDirCos", (gmtl::Matrix<float,4,4> & (*)(gmtl::Matrix<float,4,4> &, const gmtl::Vec<float,3> &, const gmtl::Vec<float,3> &, const gmtl::Vec<float,3> &, const gmtl::Vec<float,3> &, const gmtl::Vec<float,3> &, const gmtl::Vec<float,3> &))&gmtl::setDirCos, return_internal_reference< 1 >(), setDirCos_overloads_4_7());
    def("setPure", (gmtl::Quat<float> & (*)(gmtl::Quat<float> &, const gmtl::Vec<float,3> &))&gmtl::setPure, return_internal_reference< 1 >());
    def("setPure", (gmtl::Quat<double> & (*)(gmtl::Quat<double> &, const gmtl::Vec<double,3> &))&gmtl::setPure, return_internal_reference< 1 >());
    def("setRot", (gmtl::AxisAngle<double> & (*)(gmtl::AxisAngle<double> &, gmtl::Quat<double>))&gmtl::setRot, return_internal_reference< 1 >());
    def("setRot", (gmtl::AxisAngle<float> & (*)(gmtl::AxisAngle<float> &, gmtl::Quat<float>))&gmtl::setRot, return_internal_reference< 1 >());
    def("setRot", (gmtl::EulerAngle<float,gmtl::XYZ> & (*)(gmtl::EulerAngle<float,gmtl::XYZ> &, const gmtl::Matrix<float,4,4> &))&gmtl::setRot, return_internal_reference< 1 >());
    def("setRot", (gmtl::EulerAngle<float,gmtl::ZYX> & (*)(gmtl::EulerAngle<float,gmtl::ZYX> &, const gmtl::Matrix<float,4,4> &))&gmtl::setRot, return_internal_reference< 1 >());
    def("setRot", (gmtl::EulerAngle<float,gmtl::ZXY> & (*)(gmtl::EulerAngle<float,gmtl::ZXY> &, const gmtl::Matrix<float,4,4> &))&gmtl::setRot, return_internal_reference< 1 >());
    def("setRot", (gmtl::Matrix<double,3,3> & (*)(gmtl::Matrix<double,3,3> &, const gmtl::Quat<double> &))&gmtl::setRot, return_internal_reference< 1 >());
    def("setRot", (gmtl::Matrix<double,4,4> & (*)(gmtl::Matrix<double,4,4> &, const gmtl::Quat<double> &))&gmtl::setRot, return_internal_reference< 1 >());
    def("setRot", (gmtl::Matrix<double,4,4> & (*)(gmtl::Matrix<double,4,4> &, const gmtl::EulerAngle<double,gmtl::ZXY> &))&gmtl::setRot, return_internal_reference< 1 >());
    def("setRot", (gmtl::Matrix<double,4,4> & (*)(gmtl::Matrix<double,4,4> &, const gmtl::EulerAngle<double,gmtl::ZYX> &))&gmtl::setRot, return_internal_reference< 1 >());
    def("setRot", (gmtl::Matrix<double,4,4> & (*)(gmtl::Matrix<double,4,4> &, const gmtl::EulerAngle<double,gmtl::XYZ> &))&gmtl::setRot, return_internal_reference< 1 >());
    def("setRot", (gmtl::Matrix<float,4,4> & (*)(gmtl::Matrix<float,4,4> &, const gmtl::EulerAngle<float,gmtl::ZXY> &))&gmtl::setRot, return_internal_reference< 1 >());
    def("setRot", (gmtl::Matrix<float,4,4> & (*)(gmtl::Matrix<float,4,4> &, const gmtl::EulerAngle<float,gmtl::XYZ> &))&gmtl::setRot, return_internal_reference< 1 >());
    def("setRot", (gmtl::Matrix<float,4,4> & (*)(gmtl::Matrix<float,4,4> &, const gmtl::AxisAngle<float> &))&gmtl::setRot, return_internal_reference< 1 >());
    def("setRot", (gmtl::Matrix<double,4,4> & (*)(gmtl::Matrix<double,4,4> &, const gmtl::AxisAngle<double> &))&gmtl::setRot, return_internal_reference< 1 >());
    def("setRot", (gmtl::Matrix<float,4,4> & (*)(gmtl::Matrix<float,4,4> &, const gmtl::Quat<float> &))&gmtl::setRot, return_internal_reference< 1 >());
    def("setRot", (gmtl::Matrix<float,3,3> & (*)(gmtl::Matrix<float,3,3> &, const gmtl::Quat<float> &))&gmtl::setRot, return_internal_reference< 1 >());
    def("setRot", (gmtl::Matrix<float,4,4> & (*)(gmtl::Matrix<float,4,4> &, const gmtl::EulerAngle<float,gmtl::ZYX> &))&gmtl::setRot, return_internal_reference< 1 >());
    def("setRot", (gmtl::Quat<float> & (*)(gmtl::Quat<float> &, const gmtl::Vec<float,3> &, const gmtl::Vec<float,3> &))&gmtl::setRot, return_internal_reference< 1 >());
    def("setRot", (gmtl::Matrix<float,4,4> & (*)(gmtl::Matrix<float,4,4> &, const gmtl::Vec<float,3> &, const gmtl::Vec<float,3> &))&gmtl::setRot, return_internal_reference< 1 >());
    def("setRot", (gmtl::Quat<float> & (*)(gmtl::Quat<float> &, const gmtl::AxisAngle<float> &))&gmtl::setRot, return_internal_reference< 1 >());
    def("setRot", (gmtl::Quat<float> & (*)(gmtl::Quat<float> &, const gmtl::Matrix<float,4,4> &))&gmtl::setRot, return_internal_reference< 1 >());
    def("setRot", (gmtl::Quat<double> & (*)(gmtl::Quat<double> &, const gmtl::Matrix<double,4,4> &))&gmtl::setRot, return_internal_reference< 1 >());
    def("setRot", (gmtl::Quat<float> & (*)(gmtl::Quat<float> &, const gmtl::EulerAngle<float,gmtl::XYZ> &))&gmtl::setRot, return_internal_reference< 1 >());
    def("setRot", (gmtl::Quat<float> & (*)(gmtl::Quat<float> &, const gmtl::EulerAngle<float,gmtl::ZYX> &))&gmtl::setRot, return_internal_reference< 1 >());
    def("setRot", (gmtl::Quat<float> & (*)(gmtl::Quat<float> &, const gmtl::EulerAngle<float,gmtl::ZXY> &))&gmtl::setRot, return_internal_reference< 1 >());
    def("setScale", (gmtl::Matrix<float,4,4> & (*)(gmtl::Matrix<float,4,4> &, const float))&gmtl::setScale, return_internal_reference< 1 >());
    def("setScale", (gmtl::Matrix<double,4,4> & (*)(gmtl::Matrix<double,4,4> &, const double))&gmtl::setScale, return_internal_reference< 1 >());
    def("setScale", (gmtl::Matrix<float,4,4> & (*)(gmtl::Matrix<float,4,4> &, const gmtl::Vec<float,3> &))&gmtl::setScale, return_internal_reference< 1 >());
    def("setScale", (gmtl::Matrix<double,4,4> & (*)(gmtl::Matrix<double,4,4> &, const gmtl::Vec<double,3> &))&gmtl::setScale, return_internal_reference< 1 >());
    def("setTrans", (gmtl::Matrix<float,4,4> & (*)(gmtl::Matrix<float,4,4> &, const gmtl::Vec<float,3> &))&gmtl::setTrans, return_internal_reference< 1 >());
    def("setTrans", (gmtl::Matrix<double,4,4> & (*)(gmtl::Matrix<double,4,4> &, const gmtl::Vec<double,3> &))&gmtl::setTrans, return_internal_reference< 1 >());
    def("setTrans", (gmtl::Vec<float,3> & (*)(gmtl::Vec<float,3> &, const gmtl::Matrix<float,4,4> &))&gmtl::setTrans, return_internal_reference< 1 >());
    def("setTrans", (gmtl::Vec<double,3> & (*)(gmtl::Vec<double,3> &, const gmtl::Matrix<double,4,4> &))&gmtl::setTrans, return_internal_reference< 1 >());
    def("slerp", (gmtl::Quat<double> & (*)(gmtl::Quat<double> &, const double, const gmtl::Quat<double> &, const gmtl::Quat<double> &, bool))&gmtl::slerp, return_internal_reference< 1 >(), slerp_overloads_4_5());
    def("slerp", (gmtl::Quat<float> & (*)(gmtl::Quat<float> &, const float, const gmtl::Quat<float> &, const gmtl::Quat<float> &, bool))&gmtl::slerp, return_internal_reference< 1 >(), slerp_overloads_4_5());
    def("squad", (void (*)(gmtl::Quat<double> &, double, const gmtl::Quat<double> &, const gmtl::Quat<double> &, const gmtl::Quat<double> &, const gmtl::Quat<double> &))&gmtl::squad, return_internal_reference< 1 >());
    def("squad", (void (*)(gmtl::Quat<float> &, float, const gmtl::Quat<float> &, const gmtl::Quat<float> &, const gmtl::Quat<float> &, const gmtl::Quat<float> &))&gmtl::squad, return_internal_reference< 1 >());
    def("sub", (gmtl::Matrix<float,4,4> & (*)(gmtl::Matrix<float,4,4> &, const gmtl::Matrix<float,4,4> &, const gmtl::Matrix<float,4,4> &))&gmtl::sub, return_internal_reference< 1 >());
    def("sub", (gmtl::Matrix<float,3,3> & (*)(gmtl::Matrix<float,3,3> &, const gmtl::Matrix<float,3,3> &, const gmtl::Matrix<float,3,3> &))&gmtl::sub, return_internal_reference< 1 >());
    def("sub", (gmtl::Quat<float> & (*)(gmtl::Quat<float> &, const gmtl::Quat<float> &, const gmtl::Quat<float> &))&gmtl::sub, return_internal_reference< 1 >());
    def("sub", (gmtl::Quat<double> & (*)(gmtl::Quat<double> &, const gmtl::Quat<double> &, const gmtl::Quat<double> &))&gmtl::sub, return_internal_reference< 1 >());
    def("transpose", (gmtl::Matrix<float,4,4> & (*)(gmtl::Matrix<float,4,4> &))&gmtl::transpose, return_internal_reference< 1 >());
    def("transpose", (gmtl::Matrix<float,3,3> & (*)(gmtl::Matrix<float,3,3> &, const gmtl::Matrix<float,3,3> &))&gmtl::transpose, return_internal_reference< 1 >());
    def("transpose", (gmtl::Matrix<float,4,4> & (*)(gmtl::Matrix<float,4,4> &, const gmtl::Matrix<float,4,4> &))&gmtl::transpose, return_internal_reference< 1 >());
    def("transpose", (gmtl::Matrix<double,4,4> & (*)(gmtl::Matrix<double,4,4> &))&gmtl::transpose, return_internal_reference< 1 >());
    def("transpose", (gmtl::Matrix<double,3,3> & (*)(gmtl::Matrix<double,3,3> &))&gmtl::transpose, return_internal_reference< 1 >());
    def("transpose", (gmtl::Matrix<float,3,3> & (*)(gmtl::Matrix<float,3,3> &))&gmtl::transpose, return_internal_reference< 1 >());
    def("whichSide", (gmtl::PlaneSide (*)(const gmtl::Plane<float> &, const gmtl::Point<float,3> &))&gmtl::whichSide);
    def("whichSide", (gmtl::PlaneSide (*)(const gmtl::Plane<double> &, const gmtl::Point<double,3> &, const double &))&gmtl::whichSide);
    def("whichSide", (gmtl::PlaneSide (*)(const gmtl::Plane<double> &, const gmtl::Point<double,3> &))&gmtl::whichSide);
    def("whichSide", (gmtl::PlaneSide (*)(const gmtl::Plane<float> &, const gmtl::Point<float,3> &, const float &))&gmtl::whichSide);
    def("xform", (gmtl::Vec<double,3> & (*)(gmtl::Vec<double,3> &, const gmtl::Matrix<double,3,3> &, const gmtl::Vec<double,3> &))&gmtl::xform, return_internal_reference< 1 >());
    def("xform", (gmtl::Vec<float,4> & (*)(gmtl::Vec<float,4> &, const gmtl::Matrix<float,4,4> &, const gmtl::Vec<float,4> &))&gmtl::xform, return_internal_reference< 1 >());
    def("xform", (gmtl::VecBase<double,3> & (*)(gmtl::VecBase<double,3> &, const gmtl::Quat<double> &, const gmtl::VecBase<double,3> &))&gmtl::xform, return_internal_reference< 1 >());
    def("xform", (gmtl::Vec<float,3> & (*)(gmtl::Vec<float,3> &, const gmtl::Matrix<float,3,3> &, const gmtl::Vec<float,3> &))&gmtl::xform, return_internal_reference< 1 >());
    def("xform", (gmtl::VecBase<float,3> & (*)(gmtl::VecBase<float,3> &, const gmtl::Quat<float> &, const gmtl::VecBase<float,3> &))&gmtl::xform, return_internal_reference< 1 >());
    def("xform", (gmtl::Vec<double,3> & (*)(gmtl::Vec<double,3> &, const gmtl::Matrix<double,3,3> &, const gmtl::Vec<double,3> &))&gmtl::xform, return_internal_reference< 1 >());
    def("xform", (gmtl::Vec<float,3> & (*)(gmtl::Vec<float,3> &, const gmtl::Matrix<float,4,4> &, const gmtl::Vec<float,3> &))&gmtl::xform, return_internal_reference< 1 >());
    def("xform", (gmtl::Vec<float,4> & (*)(gmtl::Vec<float,4> &, const gmtl::Matrix<float,4,4> &, const gmtl::Vec<float,4> &))&gmtl::xform, return_internal_reference< 1 >());
    def("xform", (gmtl::Vec<double,3> & (*)(gmtl::Vec<double,3> &, const gmtl::Matrix<double,4,4> &, const gmtl::Vec<double,3> &))&gmtl::xform, return_internal_reference< 1 >());
    def("xform", (gmtl::Vec<double,4> & (*)(gmtl::Vec<double,4> &, const gmtl::Matrix<double,4,4> &, const gmtl::Vec<double,4> &))&gmtl::xform, return_internal_reference< 1 >());
    def("xform", (gmtl::Point<float,3> & (*)(gmtl::Point<float,3> &, const gmtl::Matrix<float,3,3> &, const gmtl::Point<float,3> &))&gmtl::xform, return_internal_reference< 1 >());
    def("xform", (gmtl::Point<double,3> & (*)(gmtl::Point<double,3> &, const gmtl::Matrix<double,3,3> &, const gmtl::Point<double,3> &))&gmtl::xform, return_internal_reference< 1 >());
    def("xform", (gmtl::Point<float,4> & (*)(gmtl::Point<float,4> &, const gmtl::Matrix<float,4,4> &, const gmtl::Point<float,4> &))&gmtl::xform, return_internal_reference< 1 >());
    def("xform", (gmtl::Vec<double,4> & (*)(gmtl::Vec<double,4> &, const gmtl::Matrix<double,4,4> &, const gmtl::Vec<double,4> &))&gmtl::xform, return_internal_reference< 1 >());
    def("xform", (gmtl::Vec<float,3> & (*)(gmtl::Vec<float,3> &, const gmtl::Matrix<float,3,3> &, const gmtl::Vec<float,3> &))&gmtl::xform, return_internal_reference< 1 >());
    def("xform", (gmtl::Point<double,4> & (*)(gmtl::Point<double,4> &, const gmtl::Matrix<double,4,4> &, const gmtl::Point<double,4> &))&gmtl::xform, return_internal_reference< 1 >());
    def("xform", (gmtl::Point<double,3> & (*)(gmtl::Point<double,3> &, const gmtl::Matrix<double,4,4> &, const gmtl::Point<double,3> &))&gmtl::xform, return_internal_reference< 1 >());
    def("xform", (gmtl::Point<float,4> & (*)(gmtl::Point<float,4> &, const gmtl::Matrix<float,4,4> &, const gmtl::Point<float,4> &))&gmtl::xform, return_internal_reference< 1 >());
    def("xform", (gmtl::Point<float,3> & (*)(gmtl::Point<float,3> &, const gmtl::Matrix<float,4,4> &, const gmtl::Point<float,3> &))&gmtl::xform, return_internal_reference< 1 >());
    def("xform", (gmtl::Point<double,4> & (*)(gmtl::Point<double,4> &, const gmtl::Matrix<double,4,4> &, const gmtl::Point<double,4> &))&gmtl::xform, return_internal_reference< 1 >());
    def("zero", (gmtl::Matrix<float,4,4> & (*)(gmtl::Matrix<float,4,4> &))&gmtl::zero, return_internal_reference< 1 >());
    def("zero", (gmtl::Matrix<float,3,3> & (*)(gmtl::Matrix<float,3,3> &))&gmtl::zero, return_internal_reference< 1 >());
}
