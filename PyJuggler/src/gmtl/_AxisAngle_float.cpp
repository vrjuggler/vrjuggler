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
#include <gmtl/AxisAngle.h>
#include <gmtl/Output.h>

// Using =======================================================================
using namespace boost::python;

// Module ======================================================================
void _Export_AxisAngle_float()
{
    scope* gmtl_AxisAngle_float_scope = new scope(
    class_< gmtl::AxisAngle<float>, bases< gmtl::VecBase<float,4> > >("AxisAnglef", init<  >())
        .def(init< const gmtl::AxisAngle<float> & >())
        .def(init< const float &, const float &, const float &, const float & >())
        .def(init< const float &, const gmtl::Vec<float,3> & >())
        .def("set", (void (gmtl::AxisAngle<float>::*)(const float &, const float &, const float &, const float &) )&gmtl::AxisAngle<float>::set)
        .def("set", (void (gmtl::AxisAngle<float>::*)(const float &, const gmtl::Vec<float,3> &) )&gmtl::AxisAngle<float>::set)
        .def("setAxis", &gmtl::AxisAngle<float>::setAxis)
        .def("setAngle", &gmtl::AxisAngle<float>::setAngle)
        .def("getAxis", &gmtl::AxisAngle<float>::getAxis)
        .def("getAngle", &gmtl::AxisAngle<float>::getAngle, return_value_policy< copy_const_reference >())
        .def(self == self)
        .def(self != self)
        .def(self_ns::str(self))
    );

    enum_< gmtl::AxisAngle<float>::Params >("Params")
        .value("Size", gmtl::AxisAngle<float>::Size)
    ;

    delete gmtl_AxisAngle_float_scope;

}
