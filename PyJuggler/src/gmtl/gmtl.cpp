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

// Include =====================================================================
#include <boost/python.hpp>

// Exports =====================================================================
void _Export_AABox_double();
void _Export_AABox_float();
void _Export_Coord_gmtl_Vec_double_3_gmtl_AxisAngle_double();
void _Export_Coord_gmtl_Vec_double_3_gmtl_EulerAngle_double_gmtl_XYZ();
void _Export_Coord_gmtl_Vec_double_3_gmtl_EulerAngle_double_gmtl_ZXY();
void _Export_Coord_gmtl_Vec_double_3_gmtl_EulerAngle_double_gmtl_ZYX();
void _Export_Coord_gmtl_Vec_double_3_gmtl_Quat_double();
void _Export_Coord_gmtl_Vec_double_4_gmtl_AxisAngle_double();
void _Export_Coord_gmtl_Vec_double_4_gmtl_EulerAngle_double_gmtl_XYZ();
void _Export_Coord_gmtl_Vec_double_4_gmtl_EulerAngle_double_gmtl_ZXY();
void _Export_Coord_gmtl_Vec_double_4_gmtl_EulerAngle_double_gmtl_ZYX();
void _Export_Coord_gmtl_Vec_double_4_gmtl_Quat_double();
void _Export_Coord_gmtl_Vec_float_3_gmtl_AxisAngle_float();
void _Export_Coord_gmtl_Vec_float_3_gmtl_EulerAngle_float_gmtl_XYZ();
void _Export_Coord_gmtl_Vec_float_3_gmtl_EulerAngle_float_gmtl_ZXY();
void _Export_Coord_gmtl_Vec_float_3_gmtl_EulerAngle_float_gmtl_ZYX();
void _Export_Coord_gmtl_Vec_float_3_gmtl_Quat_float();
void _Export_Coord_gmtl_Vec_float_4_gmtl_AxisAngle_float();
void _Export_Coord_gmtl_Vec_float_4_gmtl_EulerAngle_float_gmtl_XYZ();
void _Export_Coord_gmtl_Vec_float_4_gmtl_EulerAngle_float_gmtl_ZXY();
void _Export_Coord_gmtl_Vec_float_4_gmtl_EulerAngle_float_gmtl_ZYX();
void _Export_Coord_gmtl_Vec_float_4_gmtl_Quat_float();
void _Export_EulerAngle_float_gmtl_XYZ();
void _Export_EulerAngle_float_gmtl_ZXY();
void _Export_EulerAngle_float_gmtl_ZYX();
void _Export_Matrix_float_3_3();
void _Export_Matrix_float_4_4();
void _Export_Plane_double();
void _Export_Plane_float();
void _Export_Quat_double();
void _Export_Quat_float();
void _Export_Ray_double();
void _Export_Ray_float();
void _Export_Sphere_double();
void _Export_Sphere_float();
void _Export_Tri_double();
void _Export_Tri_float();
void _Export_Tri_int();
void _Export_VecBase_double_2();
void _Export_VecBase_double_3();
void _Export_VecBase_double_4();
void _Export_VecBase_float_2();
void _Export_VecBase_float_3();
void _Export_VecBase_float_4();
void _Export_VecBase_int_2();
void _Export_VecBase_int_3();
void _Export_VecBase_int_4();
void _Export_AxisAngle_double();
void _Export_AxisAngle_float();
void _Export_LineSeg_double();
void _Export_LineSeg_float();
void _Export_Point_float_2();
void _Export_Point_float_3();
void _Export_Point_int_2();
void _Export_Point_int_3();
void _Export_Vec_double_3();
void _Export_Vec_double_4();
void _Export_Vec_float_3();
void _Export_Vec_float_4();
void _Export_Vec_int_3();
void _Export_gmtl_Math_h();
void _Export_gmtl_Defines_h();
void _Export_gmtl_functions_h();
void _Export_gmtl_wrappers_h();

// Module ======================================================================
BOOST_PYTHON_MODULE(gmtl)
{
    _Export_AABox_double();
    _Export_AABox_float();
    _Export_Coord_gmtl_Vec_double_3_gmtl_AxisAngle_double();
    _Export_Coord_gmtl_Vec_double_3_gmtl_EulerAngle_double_gmtl_XYZ();
    _Export_Coord_gmtl_Vec_double_3_gmtl_EulerAngle_double_gmtl_ZXY();
    _Export_Coord_gmtl_Vec_double_3_gmtl_EulerAngle_double_gmtl_ZYX();
    _Export_Coord_gmtl_Vec_double_3_gmtl_Quat_double();
    _Export_Coord_gmtl_Vec_double_4_gmtl_AxisAngle_double();
    _Export_Coord_gmtl_Vec_double_4_gmtl_EulerAngle_double_gmtl_XYZ();
    _Export_Coord_gmtl_Vec_double_4_gmtl_EulerAngle_double_gmtl_ZXY();
    _Export_Coord_gmtl_Vec_double_4_gmtl_EulerAngle_double_gmtl_ZYX();
    _Export_Coord_gmtl_Vec_double_4_gmtl_Quat_double();
    _Export_Coord_gmtl_Vec_float_3_gmtl_AxisAngle_float();
    _Export_Coord_gmtl_Vec_float_3_gmtl_EulerAngle_float_gmtl_XYZ();
    _Export_Coord_gmtl_Vec_float_3_gmtl_EulerAngle_float_gmtl_ZXY();
    _Export_Coord_gmtl_Vec_float_3_gmtl_EulerAngle_float_gmtl_ZYX();
    _Export_Coord_gmtl_Vec_float_3_gmtl_Quat_float();
    _Export_Coord_gmtl_Vec_float_4_gmtl_AxisAngle_float();
    _Export_Coord_gmtl_Vec_float_4_gmtl_EulerAngle_float_gmtl_XYZ();
    _Export_Coord_gmtl_Vec_float_4_gmtl_EulerAngle_float_gmtl_ZXY();
    _Export_Coord_gmtl_Vec_float_4_gmtl_EulerAngle_float_gmtl_ZYX();
    _Export_Coord_gmtl_Vec_float_4_gmtl_Quat_float();
    _Export_EulerAngle_float_gmtl_XYZ();
    _Export_EulerAngle_float_gmtl_ZXY();
    _Export_EulerAngle_float_gmtl_ZYX();
    _Export_Matrix_float_3_3();
    _Export_Matrix_float_4_4();
    _Export_Plane_double();
    _Export_Plane_float();
    _Export_Quat_double();
    _Export_Quat_float();
    _Export_Ray_double();
    _Export_Ray_float();
    _Export_Sphere_double();
    _Export_Sphere_float();
    _Export_Tri_double();
    _Export_Tri_float();
    _Export_Tri_int();
    _Export_VecBase_double_2();
    _Export_VecBase_double_3();
    _Export_VecBase_double_4();
    _Export_VecBase_float_2();
    _Export_VecBase_float_3();
    _Export_VecBase_float_4();
    _Export_VecBase_int_2();
    _Export_VecBase_int_3();
    _Export_VecBase_int_4();
    _Export_AxisAngle_double();
    _Export_AxisAngle_float();
    _Export_LineSeg_double();
    _Export_LineSeg_float();
    _Export_Point_float_2();
    _Export_Point_float_3();
    _Export_Point_int_2();
    _Export_Point_int_3();
    _Export_Vec_double_3();
    _Export_Vec_double_4();
    _Export_Vec_float_3();
    _Export_Vec_float_4();
    _Export_Vec_int_3();
    _Export_gmtl_Math_h();
    _Export_gmtl_Defines_h();
    _Export_gmtl_functions_h();
    _Export_gmtl_wrappers_h();
}
