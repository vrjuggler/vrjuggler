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
