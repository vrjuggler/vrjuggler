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
#include <gmtl/Plane.h>
#include <gmtl/PlaneOps.h>
#include <gmtl/Output.h>

// Using =======================================================================
using namespace boost::python;

// Module ======================================================================
void _Export_Plane_float()
{
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
        .def(self == self)
        .def(self != self)
        .def(self_ns::str(self))
    ;

}
