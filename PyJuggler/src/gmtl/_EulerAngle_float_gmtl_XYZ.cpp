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
#include <gmtl/EulerAngle.h>
#include <gmtl/EulerAngleOps.h>
#include <gmtl/Output.h>
#include <gmtl-wrappers.h>

// Using =======================================================================
using namespace boost::python;

// Module ======================================================================
void _Export_EulerAngle_float_gmtl_XYZ()
{
    scope* gmtl_EulerAngle_float_gmtl_XYZ_scope = new scope(
    class_< gmtl::EulerAngle<float,gmtl::XYZ> >("EulerAngleXYZf", init<  >())
        .def(init< const gmtl::EulerAngle<float,gmtl::XYZ> & >())
        .def(init< float, float, float >())
        .def("set", &gmtl::EulerAngle<float,gmtl::XYZ>::set)
        .def("__getitem__", (float& (gmtl::EulerAngle<float,gmtl::XYZ>::*)(const unsigned) ) &gmtl::EulerAngle<float,gmtl::XYZ>::operator[], return_value_policy<copy_non_const_reference>())
        .def("__setitem__", (void (*)(gmtl::EulerAngle<float,gmtl::XYZ>*, const unsigned, float)) &gmtlWrapper::setArrayElement)
        .def(self == self)
        .def(self != self)
        .def(self_ns::str(self))
    );

    enum_< gmtl::EulerAngle<float,gmtl::XYZ>::Params >("Params")
        .value("Order", gmtl::EulerAngle<float,gmtl::XYZ>::Order)
        .value("Size", gmtl::EulerAngle<float,gmtl::XYZ>::Size)
    ;

    delete gmtl_EulerAngle_float_gmtl_XYZ_scope;

}
