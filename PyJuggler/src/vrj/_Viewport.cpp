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
#include <vrj/Display/Viewport.h>
#include <vrj/Display/Display.h>
#include <vrj/Display/Projection.h>
#include <vrj/Kernel/User.h>

// Using =======================================================================
using namespace boost::python;

// Declarations ================================================================
namespace pyj {

struct vrj_Viewport_Wrapper: vrj::Viewport
{
    vrj_Viewport_Wrapper(PyObject* self_):
        vrj::Viewport(), self(self_) {}

    vrj_Viewport_Wrapper(PyObject* self_, const vrj::Viewport& p0):
        vrj::Viewport(p0), self(self_) {}

    void updateProjections(const float p0) {
        try
        {
            call_method< void >(self, "updateProjections", p0);
        }
        catch(error_already_set)
        {
            PyErr_Print();
        }
    }

    PyObject* self;
};

inline tuple vrj_Viewport_getOriginAndSize_wrapper(vrj::Viewport* vp)
{
   float xo, yo, xs, ys;
   vp->getOriginAndSize(xo, yo, xs, ys);
   return make_tuple(xo, yo, xs, ys);
}

}// namespace 


// Module ======================================================================
void _Export_Viewport()
{
    scope* vrj_Viewport_scope = new scope(
    class_< vrj::Viewport, boost::noncopyable, pyj::vrj_Viewport_Wrapper >("Viewport", init<  >())
        .def("updateProjections", pure_virtual(&vrj::Viewport::updateProjections))
        .def("getType", &vrj::Viewport::getType)
        .def("isSimulator", &vrj::Viewport::isSimulator)
        .def("isSurface", &vrj::Viewport::isSurface)
        .def("isActive", &vrj::Viewport::isActive)
        .def("getName", &vrj::Viewport::getName, return_internal_reference< 1 >())
        .def("inStereo", &vrj::Viewport::inStereo)
        .def("getView", &vrj::Viewport::getView)
        .def("getOriginAndSize", pyj::vrj_Viewport_getOriginAndSize_wrapper)
        .def("getConfigElement", &vrj::Viewport::getConfigElement)
        .def("getUser", &vrj::Viewport::getUser, return_internal_reference< 1 >())
        .def("getDisplay", &vrj::Viewport::getDisplay, return_internal_reference< 1 >())
        .def("getLeftProj", &vrj::Viewport::getLeftProj, return_internal_reference< 1 >())
        .def("getRightProj", &vrj::Viewport::getRightProj, return_internal_reference< 1 >())
//        .def(self_ns::str(self))
    );

    enum_< vrj::Viewport::Type >("Type")
        .value("SIM", vrj::Viewport::SIM)
        .value("UNDEFINED", vrj::Viewport::UNDEFINED)
        .value("SURFACE", vrj::Viewport::SURFACE)
    ;


    enum_< vrj::Viewport::View >("View")
        .value("LEFT_EYE", vrj::Viewport::LEFT_EYE)
        .value("NONE", vrj::Viewport::NONE)
        .value("STEREO", vrj::Viewport::STEREO)
        .value("RIGHT_EYE", vrj::Viewport::RIGHT_EYE)
    ;

    delete vrj_Viewport_scope;

}
