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
#include <vrj/Display/SimViewport.h>
#include <vrj/Display/Display.h>
#include <vrj/Kernel/User.h>

// Using =======================================================================
using namespace boost::python;

// Declarations ================================================================


namespace  {


struct vrj_SimViewport_Wrapper: vrj::SimViewport
{
    vrj_SimViewport_Wrapper(PyObject* self_):
        vrj::SimViewport(), self(self_) {}

    vrj_SimViewport_Wrapper(PyObject* self_, const vrj::SimViewport & p0):
        vrj::SimViewport(p0), self(self_) {}

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

    void default_updateProjections(const float p0) {
        vrj::SimViewport::updateProjections(p0);
    }

    PyObject* self;
};

inline tuple vrj_SimViewport_getOriginAndSize_wrapper(vrj::SimViewport* vp)
{
   float xo, yo, xs, ys;
   vp->getOriginAndSize(xo, yo, xs, ys);
   return make_tuple(xo, yo, xs, ys);
}

}// namespace 


// Module ======================================================================
void _Export_SimViewport()
{
    class_< vrj::SimViewport, bases< vrj::Viewport >, vrj_SimViewport_Wrapper >("SimViewport", init<  >())
        .def(init< const vrj::SimViewport & >())
        .def("updateProjections", &vrj::SimViewport::updateProjections, &vrj_SimViewport_Wrapper::default_updateProjections)
        .def("getDrawSimInterface", &vrj::SimViewport::getDrawSimInterface, return_internal_reference< 1 >())
        .def("getType", &vrj::Viewport::getType)
        .def("isSimulator", &vrj::Viewport::isSimulator)
        .def("isSurface", &vrj::Viewport::isSurface)
        .def("isActive", &vrj::Viewport::isActive)
        .def("getName", &vrj::Viewport::getName, return_internal_reference< 1 >())
        .def("inStereo", &vrj::Viewport::inStereo)
        .def("getView", &vrj::Viewport::getView)
        .def("getOriginAndSize", vrj_SimViewport_getOriginAndSize_wrapper)
        .def("getConfigElement", &vrj::Viewport::getConfigElement)
        .def("getUser", &vrj::Viewport::getUser, return_internal_reference< 1 >())
        .def("getDisplay", &vrj::Viewport::getDisplay, return_internal_reference< 1 >())
        .def("getLeftProj", &vrj::Viewport::getLeftProj, return_internal_reference< 1 >())
        .def("getRightProj", &vrj::Viewport::getRightProj, return_internal_reference< 1 >());
}

