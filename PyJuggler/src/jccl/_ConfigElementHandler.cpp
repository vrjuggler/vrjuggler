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
    class_< jccl::ConfigElementHandler, boost::noncopyable, pyj::jccl_ConfigElementHandler_Wrapper >("ConfigElementHandler",
         "Abstract base class for all classes that can handle ConfigElement\n"
         "objects.  Any class supporting this interface can be dynamically\n"
         "reconfigured by the JCCL Configuration Manager.\n\n"
         "The idea is that a subclass overrides configCanHandle() to\n"
         "recognize those elements types that the derived class cares about.\n"
         "Then override configAdd() and configRemove() to process instances\n"
         "of those elements.\n\n"
         "The actual work of checking the list of pending add and remove\n"
         "requests and passing them to the methods of this interface is done\n"
         "by configProcessPending().  This can also be overriden to provide\n"
         "special behavior, but this is strongly discouraged.  The default\n"
         "implementation should be sufficient for almost any conceivable\n"
         "dynamic reconfiguration need."
         ,
         init<  >(
         )
        )
        .def("configCanHandle",
             pure_virtual(&jccl::ConfigElementHandler::configCanHandle),
             "configCanHandle(element) -> Boolean\n"
             "Checks if this handler can process the given config element.\n"
             "Typically, an implementation of this method will check the\n"
             "element's definition ID to decide if it knows how to deal with\n"
             "the element.\n"
             "Arguments:\n"
             "element -- The current config element that is ready to be\n"
             "           processed.\n"
             "Returns:\n"
             "True is returned if this handler can process the given element."
         )
        .def("configProcessPending",
             &jccl::ConfigElementHandler::configProcessPending,
             &pyj::jccl_ConfigElementHandler_Wrapper::default_configProcessPending,
             "configProcessPending() -> int\n"
             "Processes any pending reconfiguration requests that we know\n"
             "how to deal with.\n\n"
             "The default implementation does the following for each item in\n"
             "the pending list:\n\n"
             "   for each pending item p in the pending list do\n"
             "      if this->configCanHandle(p) AND p's dependencides are met\n"
             "         retval = configAdd or configRemove (p)\n"
             "         if retval == true\n"
             "            remove request from pending\n"
             "            add or remove p.element from active\n\n"
             "ConfigManager's pending list MUST be locked before this\n"
             "function is called.  Typically, configProcessPending() will be\n"
             "called by ConfigManager.attemptReconfiguration(), which takes\n"
             "care of this automatically."
         )
        .def("configAdd",
             pure_virtual(&jccl::ConfigElementHandler::configAdd),
             "configAdd(element) -> Boolean\n"
             "Adds the pending element to the configuration.\n"
             "Pre-condition:\n"
             "configCanHandle(element) == True\n"
             "Arguments:\n"
             "element -- A newly added ConfigElement object to be processed.\n"
             "Returns:\n"
             "True is returned if and only if the given element was\n"
             "processed successfully."
         )
        .def("configRemove",
             pure_virtual(&jccl::ConfigElementHandler::configRemove),
             "configRemove(element) -> Boolean\n"
             "Removes the pending element from the configuration.\n"
             "Pre-condition:\n"
             "configCanHandle(element) == True\n"
             "Arguments:\n"
             "element -- A newly removed ConfigElement object to be\n"
             "           processed.\n"
             "Returns:\n"
             "True is returned if and only if the given element was\n"
             "processed successfully."
         )
    ;
}
