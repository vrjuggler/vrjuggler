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

