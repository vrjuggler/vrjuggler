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
#include <gmtl-wrappers.h>

// Using =======================================================================
using namespace boost::python;

// Declarations ================================================================


namespace  {


BOOST_PYTHON_FUNCTION_OVERLOADS(makeDirCosMatrix33_overloads_3_6, gmtlWrapper::makeDirCosMatrix33, 3, 6)
BOOST_PYTHON_FUNCTION_OVERLOADS(makeDirCosMatrix44_overloads_3_6, gmtlWrapper::makeDirCosMatrix44, 3, 6)


}// namespace 


// Module ======================================================================
void _Export_gmtl_wrappers_h()
{
    def("makeAxesMatrix44", (gmtl::Matrix<double,4,4> (*)(const gmtl::Vec<double,3> &, const gmtl::Vec<double,3> &, const gmtl::Vec<double,3> &))&gmtlWrapper::makeAxesMatrix44);
    def("makeAxesMatrix44", (gmtl::Matrix<float,4,4> (*)(const gmtl::Vec<float,3> &, const gmtl::Vec<float,3> &, const gmtl::Vec<float,3> &))&gmtlWrapper::makeAxesMatrix44);
    def("makeDirCosMatrix33", (gmtl::Matrix<double,3,3> (*)(const gmtl::Vec<double,3> &, const gmtl::Vec<double,3> &, const gmtl::Vec<double,3> &, const gmtl::Vec<double,3> &, const gmtl::Vec<double,3> &, const gmtl::Vec<double,3> &))&gmtlWrapper::makeDirCosMatrix33, makeDirCosMatrix33_overloads_3_6());
    def("makeDirCosMatrix33", (gmtl::Matrix<float,3,3> (*)(const gmtl::Vec<float,3> &, const gmtl::Vec<float,3> &, const gmtl::Vec<float,3> &, const gmtl::Vec<float,3> &, const gmtl::Vec<float,3> &, const gmtl::Vec<float,3> &))&gmtlWrapper::makeDirCosMatrix33, makeDirCosMatrix33_overloads_3_6());
    def("makeDirCosMatrix44", (gmtl::Matrix<double,4,4> (*)(const gmtl::Vec<double,3> &, const gmtl::Vec<double,3> &, const gmtl::Vec<double,3> &, const gmtl::Vec<double,3> &, const gmtl::Vec<double,3> &, const gmtl::Vec<double,3> &))&gmtlWrapper::makeDirCosMatrix44, makeDirCosMatrix44_overloads_3_6());
    def("makeDirCosMatrix44", (gmtl::Matrix<float,4,4> (*)(const gmtl::Vec<float,3> &, const gmtl::Vec<float,3> &, const gmtl::Vec<float,3> &, const gmtl::Vec<float,3> &, const gmtl::Vec<float,3> &, const gmtl::Vec<float,3> &))&gmtlWrapper::makeDirCosMatrix44, makeDirCosMatrix44_overloads_3_6());
    def("makeRotEulerAngleXYZ", (gmtl::EulerAngle<float,gmtl::XYZ> (*)(const gmtl::Matrix<float,4,4> &))&gmtlWrapper::makeRotEulerAngleXYZ);
    def("makeRotEulerAngleXYZ", (gmtl::EulerAngle<double,gmtl::XYZ> (*)(const gmtl::Matrix<double,4,4> &))&gmtlWrapper::makeRotEulerAngleXYZ);
    def("makeRotEulerAngleZXY", (gmtl::EulerAngle<float,gmtl::ZXY> (*)(const gmtl::Matrix<float,4,4> &))&gmtlWrapper::makeRotEulerAngleZXY);
    def("makeRotEulerAngleZXY", (gmtl::EulerAngle<double,gmtl::ZXY> (*)(const gmtl::Matrix<double,4,4> &))&gmtlWrapper::makeRotEulerAngleZXY);
    def("makeRotEulerAngleZYX", (gmtl::EulerAngle<float,gmtl::ZYX> (*)(const gmtl::Matrix<float,4,4> &))&gmtlWrapper::makeRotEulerAngleZYX);
    def("makeRotEulerAngleZYX", (gmtl::EulerAngle<double,gmtl::ZYX> (*)(const gmtl::Matrix<double,4,4> &))&gmtlWrapper::makeRotEulerAngleZYX);
    def("makeRotMatrix33", (gmtl::Matrix<double,3,3> (*)(const gmtl::Quat<double> &))&gmtlWrapper::makeRotMatrix33);
    def("makeRotMatrix33", (gmtl::Matrix<float,3,3> (*)(const gmtl::Quat<float> &))&gmtlWrapper::makeRotMatrix33);
    def("makeRotMatrix33", (gmtl::Matrix<float,3,3> (*)(const gmtl::EulerAngle<float,gmtl::ZYX> &))&gmtlWrapper::makeRotMatrix33);
    def("makeRotMatrix33", (gmtl::Matrix<float,3,3> (*)(const gmtl::EulerAngle<float,gmtl::XYZ> &))&gmtlWrapper::makeRotMatrix33);
    def("makeRotMatrix33", (gmtl::Matrix<float,3,3> (*)(const gmtl::EulerAngle<float,gmtl::ZXY> &))&gmtlWrapper::makeRotMatrix33);
    def("makeRotMatrix44", (gmtl::Matrix<float,4,4> (*)(const gmtl::Quat<float> &))&gmtlWrapper::makeRotMatrix44);
    def("makeRotMatrix44", (gmtl::Matrix<float,4,4> (*)(const gmtl::EulerAngle<float,gmtl::XYZ> &))&gmtlWrapper::makeRotMatrix44);
    def("makeRotMatrix44", (gmtl::Matrix<float,4,4> (*)(const gmtl::EulerAngle<float,gmtl::ZXY> &))&gmtlWrapper::makeRotMatrix44);
    def("makeRotMatrix44", (gmtl::Matrix<double,4,4> (*)(const gmtl::Quat<double> &))&gmtlWrapper::makeRotMatrix44);
    def("makeRotMatrix44", (gmtl::Matrix<float,4,4> (*)(const gmtl::EulerAngle<float,gmtl::ZYX> &))&gmtlWrapper::makeRotMatrix44);
    def("makeRotMatrix44", (gmtl::Matrix<float,4,4> (*)(const gmtl::AxisAngle<float> &))&gmtlWrapper::makeRotMatrix44);
    def("makeRotMatrix44", (gmtl::Matrix<double,4,4> (*)(const gmtl::AxisAngle<double> &))&gmtlWrapper::makeRotMatrix44);
    def("makeRotMatrix44", (gmtl::Matrix<float,4,4> (*)(const gmtl::Vec<float,3> &, const gmtl::Vec<float,3> &))&gmtlWrapper::makeRotMatrix44);
    def("makeRotQuat", (gmtl::Quat<float> (*)(const gmtl::Matrix<float,4,4> &))&gmtlWrapper::makeRotQuat);
    def("makeRotQuat", (gmtl::Quat<double> (*)(const gmtl::Matrix<double,4,4> &))&gmtlWrapper::makeRotQuat);
    def("makeRotQuat", (gmtl::Quat<float> (*)(const gmtl::Vec<float,3> &, const gmtl::Vec<float,3> &))&gmtlWrapper::makeRotQuat);
    def("makeRotQuat", (gmtl::Quat<double> (*)(const gmtl::Vec<double,3> &, const gmtl::Vec<double,3> &))&gmtlWrapper::makeRotQuat);
    def("makeScaleMatrix44", (gmtl::Matrix<double,4,4> (*)(const double))&gmtlWrapper::makeScaleMatrix44);
    def("makeScaleMatrix44", (gmtl::Matrix<double,4,4> (*)(const gmtl::Vec<double,4> &))&gmtlWrapper::makeScaleMatrix44);
    def("makeScaleMatrix44", (gmtl::Matrix<float,4,4> (*)(const float))&gmtlWrapper::makeScaleMatrix44);
    def("makeScaleMatrix44", (gmtl::Matrix<double,4,4> (*)(const gmtl::Vec<double,3> &))&gmtlWrapper::makeScaleMatrix44);
    def("makeScaleMatrix44", (gmtl::Matrix<float,4,4> (*)(const gmtl::Vec<float,4> &))&gmtlWrapper::makeScaleMatrix44);
    def("makeScaleMatrix44", (gmtl::Matrix<float,4,4> (*)(const gmtl::Vec<float,3> &))&gmtlWrapper::makeScaleMatrix44);
    def("makeTransMatrix44", (gmtl::Matrix<double,4,4> (*)(const gmtl::Vec<double,3> &))&gmtlWrapper::makeTransMatrix44);
    def("makeTransMatrix44", (gmtl::Matrix<float,4,4> (*)(const gmtl::Vec<float,3> &))&gmtlWrapper::makeTransMatrix44);
    def("makeTransVec3", (gmtl::Vec<double,3> (*)(const gmtl::Matrix<double,4,4> &))&gmtlWrapper::makeTransVec3);
    def("makeTransVec3", (gmtl::Vec<float,3> (*)(const gmtl::Matrix<float,4,4> &))&gmtlWrapper::makeTransVec3);
}
