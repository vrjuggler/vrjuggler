// PyJuggler is (C) Copyright 2002-2004 by Patrick Hartling
// Distributed under the GNU Lesser General Public License 2.1.  (See
// accompanying file COPYING.txt or http://www.gnu.org/copyleft/lesser.txt)

// File:          $RCSfile$
// Date modified: $Date$
// Version:       $Revision$

// Bosot Includes ==============================================================
#include <boost/python.hpp>
#include <boost/cstdint.hpp>

// Includes ====================================================================
#include <jccl/RTRC/ConfigElementHandler.h>

// Using =======================================================================
using namespace boost::python;

// Declarations ================================================================
namespace pyj {

struct jccl_ConfigElementHandler_Wrapper: jccl::ConfigElementHandler
{
    jccl_ConfigElementHandler_Wrapper(PyObject* self_, const jccl::ConfigElementHandler& p0):
        jccl::ConfigElementHandler(p0), self(self_) {}

    jccl_ConfigElementHandler_Wrapper(PyObject* self_):
        jccl::ConfigElementHandler(), self(self_) {}

    bool configCanHandle(jccl::ConfigElementPtr p0) {
        try
        {
            return call_method< bool >(self, "configCanHandle", p0);
        }
        catch(error_already_set)
        {
            PyErr_Print();
        }

        return false;
    }

    int configProcessPending() {
        try
        {
            return call_method< int >(self, "configProcessPending");
        }
        catch(error_already_set)
        {
            PyErr_Print();
        }

        return 0;
    }

    int default_configProcessPending() {
        return jccl::ConfigElementHandler::configProcessPending();
    }

    bool configAdd(jccl::ConfigElementPtr p0) {
        try
        {
            return call_method< bool >(self, "configAdd", p0);
        }
        catch(error_already_set)
        {
            PyErr_Print();
        }

        return false;
    }

    bool configRemove(jccl::ConfigElementPtr p0) {
        try
        {
            return call_method< bool >(self, "configRemove", p0);
        }
        catch(error_already_set)
        {
            PyErr_Print();
        }

        return false;
    }

    PyObject* self;
};

}// namespace 


// Module ======================================================================
void _Export_ConfigElementHandler()
{
    class_< jccl::ConfigElementHandler, boost::noncopyable, pyj::jccl_ConfigElementHandler_Wrapper >("ConfigElementHandler", init<  >())
        .def("configCanHandle", pure_virtual(&jccl::ConfigElementHandler::configCanHandle))
        .def("configProcessPending", &jccl::ConfigElementHandler::configProcessPending, &pyj::jccl_ConfigElementHandler_Wrapper::default_configProcessPending)
        .def("configAdd", pure_virtual(&jccl::ConfigElementHandler::configAdd))
        .def("configRemove", pure_virtual(&jccl::ConfigElementHandler::configRemove))
    ;
}
