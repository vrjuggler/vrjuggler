// PyJuggler is (C) Copyright 2002-2004 by Patrick Hartling
// Distributed under the GNU Lesser General Public License 2.1.  (See
// accompanying file COPYING.txt or http://www.gnu.org/copyleft/lesser.txt)

// File:          $RCSfile$
// Date modified: $Date$
// Version:       $Revision$

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
