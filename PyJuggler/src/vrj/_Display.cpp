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
#include <vrj/Display/Display.h>
#include <vrj/Display/Viewport.h>

// Using =======================================================================
using namespace boost::python;

// Declarations ================================================================
namespace pyj {

struct vrj_Display_Wrapper: vrj::Display
{
    vrj_Display_Wrapper(PyObject* self_, const vrj::Display& p0):
        vrj::Display(p0), self(self_) {}

    vrj_Display_Wrapper(PyObject* self_):
        vrj::Display(), self(self_) {}


    PyObject* self;
};

inline tuple vrj_Display_getOriginAndSize_wrapper(vrj::Display* disp)
{
   int xo, yo, xs, ys;
   disp->getOriginAndSize(xo, yo, xs, ys);
   return make_tuple(xo, yo, xs, ys);
}

}// namespace 


// Module ======================================================================
void _Export_Display()
{
    class_< vrj::Display, pyj::vrj_Display_Wrapper >("Display", init<  >())
        .def(init< const vrj::Display& >())
        .def("isActive", &vrj::Display::isActive)
        .def("getName", &vrj::Display::getName)
        .def("getOriginAndSize", pyj::vrj_Display_getOriginAndSize_wrapper)
        .def("getPipe", &vrj::Display::getPipe)
        .def("getConfigElement", &vrj::Display::getConfigElement)
        .def("getNumViewports", &vrj::Display::getNumViewports)
        .def("getViewport", &vrj::Display::getViewport, return_internal_reference< 1 >())
//        .def(self_ns::str(self))
    ;

}
