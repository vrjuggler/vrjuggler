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
#include <gmtl/Sphere.h>
#include <gmtl/SphereOps.h>
#include <gmtl/Output.h>

// Using =======================================================================
using namespace boost::python;

// Module ======================================================================
void _Export_Sphere_float()
{
    class_< gmtl::Sphere<float> >("Spheref", init<  >())
        .def(init< const gmtl::Point<float,3> &, const float & >())
        .def(init< const gmtl::Sphere<float> & >())
        .def_readwrite("mCenter", &gmtl::Sphere<float>::mCenter)
        .def_readwrite("mRadius", &gmtl::Sphere<float>::mRadius)
        .def("getCenter", &gmtl::Sphere<float>::getCenter, return_value_policy< copy_const_reference >())
        .def("getRadius", &gmtl::Sphere<float>::getRadius, return_value_policy< copy_const_reference >())
        .def("setCenter", &gmtl::Sphere<float>::setCenter)
        .def("setRadius", &gmtl::Sphere<float>::setRadius)
        .def(self == self)
        .def(self != self)
        .def(self_ns::str(self))
    ;

}
