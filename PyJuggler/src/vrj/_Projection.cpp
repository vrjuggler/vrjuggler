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
#include <vrj/Display/Projection.h>

// Using =======================================================================
using namespace boost::python;

// Declarations ================================================================
namespace pyj {

struct vrj_Projection_Wrapper: vrj::Projection
{
    vrj_Projection_Wrapper(PyObject* self_):
        vrj::Projection(), self(self_) {}

    vrj_Projection_Wrapper(PyObject* self_, const vrj::Projection& p0):
        vrj::Projection(p0), self(self_) {}

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

    PyObject* self;
};

}// namespace 


// Module ======================================================================
void _Export_Projection()
{
    scope* vrj_Projection_scope = new scope(
    class_< vrj::Projection, boost::noncopyable, pyj::vrj_Projection_Wrapper >("Projection", init<  >())
        .def("calcViewMatrix", pure_virtual(&vrj::Projection::calcViewMatrix))
        .def("getEye", &vrj::Projection::getEye)
        .def("getViewport", &vrj::Projection::getViewport, return_internal_reference< 1 >())
        .def("getFrustumApexAndCorners", &vrj::Projection::getFrustumApexAndCorners)
        .def("getViewMatrix", &vrj::Projection::getViewMatrix, return_value_policy< copy_const_reference >())
        .def("getFrustum", &vrj::Projection::getFrustum)
//        .def(self_ns::str(self))
    );

    enum_< vrj::Projection::Eye >("Eye")
        .value("RIGHT", vrj::Projection::RIGHT)
        .value("LEFT", vrj::Projection::LEFT)
    ;

    delete vrj_Projection_scope;

}
