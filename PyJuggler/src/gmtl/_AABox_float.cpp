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
#include <gmtl/AABox.h>

// Using =======================================================================
using namespace boost::python;

// Module ======================================================================
void _Export_AABox_float()
{
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

}
