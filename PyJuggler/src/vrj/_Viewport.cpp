// PyJuggler is (C) Copyright 2002-2005 by Patrick Hartling
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
    class_< vrj::Viewport, boost::noncopyable, pyj::vrj_Viewport_Wrapper >("Viewport",
         "Base class for window viewports and all viewport data.  This\n"
         "stores projection data, viewport info, and relevant user."
         ,
         init<  >()
        )
        .def("updateProjections",
             pure_virtual(&vrj::Viewport::updateProjections),
             "updateProjections(positionScale)\n"
             "Updates the projection data for this viewport.  This uses the\n"
             "data from the head position for this viewport."
         )
        .def("getType", &vrj::Viewport::getType,
             "getType() -> vrj.Viewport.Type value\n"
             "Gets the type of this viewport."
         )
        .def("isSimulator", &vrj::Viewport::isSimulator,
             "isSimulator() -> Boolean\n"
             "Determines if this is a simulator viewport."
         )
        .def("isSurface", &vrj::Viewport::isSurface,
             "isSurface() -> Boolean\n"
             "Determines if this is a surface viewport."
         )
        .def("isActive", &vrj::Viewport::isActive,
             "isActive() -> Boolean\n"
             "Determines if this viewport is active."
         )
        .def("getName", &vrj::Viewport::getName,
             return_internal_reference< 1 >(),
             "getName() -> string object\n"
             "Gets the name of this viewport."
         )
        .def("inStereo", &vrj::Viewport::inStereo,
             "inStereo() -> Boolean\n"
             "Determines if this viewport is rendering in stereo."
         )
        .def("getView", &vrj::Viewport::getView,
             "getView() -> vrj.Viewport.View value\n"
             "Which view are we supposed to render?"
         )
        .def("getOriginAndSize", pyj::vrj_Viewport_getOriginAndSize_wrapper,
             "getOriginAndSize() -> (float, float, float, float)\n"
             "Returns the origin and size of this viewport as a tuple of\n"
             "floating-point values (xOrigin, yOrigin, width, height)."
         )
        .def("getConfigElement", &vrj::Viewport::getConfigElement,
             "getConfigElement() -> jccl.ConfigElement object\n"
             "gets the config element that configured this viewport."
         )
        .def("getUser", &vrj::Viewport::getUser,
             return_internal_reference< 1 >(),
             "getUser() -> vrj.User object\n"
             "Gets the user associated with this viewport."
         )
        .def("getDisplay", &vrj::Viewport::getDisplay,
             return_internal_reference< 1 >(),
             "getDisplay() -> vrj.Display object\n"
             "Gets the display contaning this viewprot."
         )
        .def("getLeftProj", &vrj::Viewport::getLeftProj,
             return_internal_reference< 1 >(),
             "getLeftProj() -> vrj.Projection object\n"
             "Gets the left projection."
         )
        .def("getRightProj", &vrj::Viewport::getRightProj,
             return_internal_reference< 1 >(),
             "getRightProj() -> vrj.Projection object\n"
             "Gets the right projection."
         )
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
