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
#include <vrj/Display/SimViewport.h>
#include <vrj/Display/Display.h>
#include <vrj/Kernel/User.h>

// Using =======================================================================
using namespace boost::python;

// Declarations ================================================================
namespace pyj {

struct vrj_SimViewport_Wrapper: vrj::SimViewport
{
    vrj_SimViewport_Wrapper(PyObject* self_):
        vrj::SimViewport(), self(self_) {}

    vrj_SimViewport_Wrapper(PyObject* self_, const vrj::SimViewport& p0):
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
    class_< vrj::SimViewport, bases< vrj::Viewport >, pyj::vrj_SimViewport_Wrapper >("SimViewport", init<  >())
        .def(init< const vrj::SimViewport& >())
        .def("updateProjections", (void (vrj::SimViewport::*)(const float) )&vrj::SimViewport::updateProjections, (void (pyj::vrj_SimViewport_Wrapper::*)(const float))&pyj::vrj_SimViewport_Wrapper::default_updateProjections)
        .def("getDrawSimInterface", &vrj::SimViewport::getDrawSimInterface, return_internal_reference< 1 >())
    ;
}

