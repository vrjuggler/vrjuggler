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

// Includes ====================================================================
#include <boost/python.hpp>
#include <vrj/Display/Display.h>
#include <vrj/Display/Viewport.h>

// Using =======================================================================
using namespace boost::python;

// Declarations ================================================================


namespace  {


struct vrj_Display_Wrapper: vrj::Display
{
    vrj_Display_Wrapper(PyObject* self_, const vrj::Display & p0):
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
    class_< vrj::Display, vrj_Display_Wrapper >("Display", init<  >())
        .def(init< const vrj::Display & >())
        .def("isActive", &vrj::Display::isActive)
        .def("getName", &vrj::Display::getName)
        .def("getOriginAndSize", vrj_Display_getOriginAndSize_wrapper)
        .def("getPipe", &vrj::Display::getPipe)
        .def("getNumViewports", &vrj::Display::getNumViewports)
        .def("getViewport", &vrj::Display::getViewport, return_internal_reference< 1 >())
//        .def(self_ns::str(self))
    ;

}
