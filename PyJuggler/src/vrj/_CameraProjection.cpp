/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * PyJuggler is (C) Copyright 2002-2004 by Patrick Hartling
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

// Boost Includes ==============================================================
#include <boost/python.hpp>
#include <boost/cstdint.hpp>

// Includes ====================================================================
#include <vrj/Display/CameraProjection.h>

// Using =======================================================================
using namespace boost::python;

// Declarations ================================================================
namespace pyj {

struct vrj_CameraProjection_Wrapper: vrj::CameraProjection
{
    vrj_CameraProjection_Wrapper(PyObject* self_, const vrj::CameraProjection& p0):
        vrj::CameraProjection(p0), self(self_) {}

    vrj_CameraProjection_Wrapper(PyObject* self_):
        vrj::CameraProjection(), self(self_) {}

    void calcViewMatrix(gmtl::Matrix44f& p0, const float p1) {
        try
        {
            call_method< void >(self, "calcViewMatrix", p0, p1);
        }
        catch(error_already_set)
        {
            PyErr_Print();
        }
    }

    void default_calcViewMatrix(gmtl::Matrix44f& p0, const float p1) {
        vrj::CameraProjection::calcViewMatrix(p0, p1);
    }

    PyObject* self;
};

}// namespace 


// Module ======================================================================
void _Export_CameraProjection()
{
    class_< vrj::CameraProjection, bases< vrj::Projection >, pyj::vrj_CameraProjection_Wrapper >("CameraProjection", init<  >())
        .def(init< const vrj::CameraProjection& >())
        .def_readwrite("mVertFOV", &vrj::CameraProjection::mVertFOV)
        .def("calcViewMatrix", (void (vrj::CameraProjection::*)(gmtl::Matrix44f&, const float) )&vrj::CameraProjection::calcViewMatrix, (void (pyj::vrj_CameraProjection_Wrapper::*)(gmtl::Matrix44f&, const float))&pyj::vrj_CameraProjection_Wrapper::default_calcViewMatrix)
    ;
}
