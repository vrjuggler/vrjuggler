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
#include <gmtl/Quat.h>
#include <gmtl/Output.h>
#include <gmtl-wrappers.h>
#include <gmtl-getData-wrappers.h>

// Using =======================================================================
using namespace boost::python;

// Module ======================================================================
void _Export_Quat_double()
{
    scope* gmtl_Quat_double_scope = new scope(
    class_< gmtl::Quat<double> >("Quatd", init<  >())
        .def(init< const double, const double, const double, const double >())
        .def(init< const gmtl::Quat<double> & >())
        .def_readwrite("mData", &gmtl::Quat<double>::mData)
        .def("set", &gmtl::Quat<double>::set)
        .def("get", &gmtl::Quat<double>::get)
        .def("getData", (tuple (gmtlWrappers::*)(gmtl::Quat<double>*)) &gmtlWrappers::Quat_getData)
        .def("__getitem__", (double& (gmtl::Quat<double>::*)(const int)) &gmtl::Quat<double>::operator[], return_value_policy<copy_non_const_reference>())
        .def("__setitem__", (void (*)(gmtl::Quatd*, const unsigned, double)) &gmtlWrapper::setArrayElement)
        .def(self * self)
        .def(self *= self)
        .def(-self)
        .def(self * double())
        .def(self *= double())
        .def(self / self)
        .def(self /= self)
        .def(self / double())
//        .def(self /= double())
        .def(self + self)
        .def(self += self)
        .def(self - self)
        .def(self -= self)
        .def(self == self)
        .def(self != self)
        .def(self_ns::str(self))
    );

    enum_< gmtl::Quat<double>::Params >("Params")
        .value("Size", gmtl::Quat<double>::Size)
    ;

    delete gmtl_Quat_double_scope;

}
