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
void _Export_Quat_float()
{
    scope* gmtl_Quat_float_scope = new scope(
    class_< gmtl::Quat<float> >("Quatf", init<  >())
        .def(init< const float, const float, const float, const float >())
        .def(init< const gmtl::Quat<float> & >())
        .def_readwrite("mData", &gmtl::Quat<float>::mData)
        .def("set", &gmtl::Quat<float>::set)
        .def("get", &gmtl::Quat<float>::get)
        .def("getData", (tuple (gmtlWrappers::*)(gmtl::Quat<float>*)) &gmtlWrappers::Quat_getData)
        .add_property("data", (tuple (gmtlWrappers::*)(gmtl::Quat<float>*)) &gmtlWrappers::Quat_getData)
        .def("__getitem__", (float& (gmtl::Quat<float>::*)(const int)) &gmtl::Quat<float>::operator[], return_value_policy<copy_non_const_reference>())
        .def("__setitem__", (void (*)(gmtl::Quatf*, const unsigned, float)) &gmtlWrapper::setArrayElement)
        .def(self * self)
        .def(self *= self)
        .def(-self)
        .def(self * float())
        .def(self *= float())
        .def(self / self)
        .def(self /= self)
        .def(self / float())
//        .def(self /= float())
        .def(self + self)
        .def(self += self)
        .def(self - self)
        .def(self -= self)
        .def(self == self)
        .def(self != self)
        .def(self_ns::str(self))
    );

    enum_< gmtl::Quat<float>::Params >("Params")
        .value("Size", gmtl::Quat<float>::Size)
    ;

    delete gmtl_Quat_float_scope;

}
