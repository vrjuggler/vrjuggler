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
#include <gmtl/Matrix.h>
#include <gmtl-wrappers.h>

// Using =======================================================================
using namespace boost::python;

// Module ======================================================================
void _Export_Matrix_float_3_3()
{
    scope* gmtl_Matrix_float_3_3_scope = new scope(
    class_< gmtl::Matrix<float,3,3> >("Matrix33f", init<  >())
        .def(init< const gmtl::Matrix<float,3,3> & >())
        .def_readwrite("mState", &gmtl::Matrix<float,3,3>::mState)
        .def("set", (void (gmtl::Matrix<float,3,3>::*)(float, float, float, float) )&gmtl::Matrix<float,3,3>::set)
        .def("set", (void (gmtl::Matrix<float,3,3>::*)(float, float, float, float, float, float) )&gmtl::Matrix<float,3,3>::set)
        .def("set", (void (gmtl::Matrix<float,3,3>::*)(float, float, float, float, float, float, float, float, float) )&gmtl::Matrix<float,3,3>::set)
        .def("set", (void (gmtl::Matrix<float,3,3>::*)(float, float, float, float, float, float, float, float, float, float, float, float) )&gmtl::Matrix<float,3,3>::set)
        .def("set", (void (gmtl::Matrix<float,3,3>::*)(const float *) )&gmtl::Matrix<float,3,3>::set)
        .def("setTranspose", &gmtl::Matrix<float,3,3>::setTranspose)
        .def("isError", &gmtl::Matrix<float,3,3>::isError)
        .def("setError", &gmtl::Matrix<float,3,3>::setError)
        .def("__getitem__", (gmtl::Matrix<float,3,3>::RowAccessor (gmtl::Matrix<float,3,3>::*)(const unsigned) )&gmtl::Matrix<float,3,3>::operator[])
        .def(self * self)
        .def(self *= self)
//        .def(self * float())
        .def(self *= float())
        .def(self == self)
        .def(self != self)
//        .def(str(self))
    );

    class_< gmtl::Matrix<float,3,3>::RowAccessor >("RowAccessor", no_init)
       .def("__getitem__", &gmtl::Matrix<float,3,3>::RowAccessor::operator[], return_value_policy<copy_non_const_reference>())
       .def("__setitem__", (void (*)(gmtl::Matrix<float,3,3>::RowAccessor*, const unsigned, float)) &gmtlWrapper::setArrayElement)
    ;


    enum_< gmtl::Matrix<float,3,3>::Params >("Params")
        .value("Rows", gmtl::Matrix<float,3,3>::Rows)
        .value("Cols", gmtl::Matrix<float,3,3>::Cols)
    ;


    enum_< gmtl::Matrix<float,3,3>::XformState >("XformState")
        .value("TRANS", gmtl::Matrix<float,3,3>::TRANS)
        .value("FULL", gmtl::Matrix<float,3,3>::FULL)
        .value("ORTHOGONAL", gmtl::Matrix<float,3,3>::ORTHOGONAL)
        .value("AFFINE", gmtl::Matrix<float,3,3>::AFFINE)
        .value("NON_UNISCALE", gmtl::Matrix<float,3,3>::NON_UNISCALE)
        .value("XFORM_ERROR", gmtl::Matrix<float,3,3>::XFORM_ERROR)
        .value("IDENTITY", gmtl::Matrix<float,3,3>::IDENTITY)
    ;

    delete gmtl_Matrix_float_3_3_scope;

}
