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
#include <gmtl/VecBase.h>
#include <gmtl-wrappers.h>

// Using =======================================================================
using namespace boost::python;

// Module ======================================================================
void _Export_VecBase_int_3()
{
    scope* gmtl_VecBase_int_3_scope = new scope(
    class_< gmtl::VecBase<int,3> >("VecBase3i", init<  >())
        .def(init< const gmtl::VecBase<int,3> & >())
        .def(init< const int &, const int & >())
        .def(init< const int &, const int &, const int & >())
        .def(init< const int &, const int &, const int &, const int & >())
        .def("set", (void (gmtl::VecBase<int,3>::*)(const int *) )&gmtl::VecBase<int,3>::set)
        .def("set", (void (gmtl::VecBase<int,3>::*)(const int &) )&gmtl::VecBase<int,3>::set)
        .def("set", (void (gmtl::VecBase<int,3>::*)(const int &, const int &) )&gmtl::VecBase<int,3>::set)
        .def("set", (void (gmtl::VecBase<int,3>::*)(const int &, const int &, const int &) )&gmtl::VecBase<int,3>::set)
        .def("set", (void (gmtl::VecBase<int,3>::*)(const int &, const int &, const int &, const int &) )&gmtl::VecBase<int,3>::set)
        .def("__getitem__", (int& (gmtl::VecBase<int,3>::*)(const unsigned) ) &gmtl::VecBase<int,3>::operator[], return_value_policy<copy_non_const_reference>())
        .def("__setitem__", (void (*)(gmtl::VecBase<int,3>*, const unsigned, int)) &gmtlWrapper::setArrayElement)
    );

    enum_< gmtl::VecBase<int,3>::Params >("Params")
        .value("Size", gmtl::VecBase<int,3>::Size)
    ;

    delete gmtl_VecBase_int_3_scope;

}
