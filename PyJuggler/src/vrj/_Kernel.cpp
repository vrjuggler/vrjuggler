/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * PyJuggler is (C) Copyright 2002, 2003 by Patrick Hartling
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
#include <vrj/Kernel/Kernel.h>
#include <vrj/Kernel/App.h>
#include <vrj/Kernel/User.h>

// Using =======================================================================
using namespace boost::python;

// Declarations ================================================================


namespace  {


struct vrj_Kernel_Wrapper: vrj::Kernel
{
    bool configCanHandle(boost::shared_ptr<jccl::ConfigElement> p0) {
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

    bool default_configCanHandle(boost::shared_ptr<jccl::ConfigElement> p0) {
        return vrj::Kernel::configCanHandle(p0);
    }

    bool configAdd(boost::shared_ptr<jccl::ConfigElement> p0) {
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

    bool default_configAdd(boost::shared_ptr<jccl::ConfigElement> p0) {
        return vrj::Kernel::configAdd(p0);
    }

    bool configRemove(boost::shared_ptr<jccl::ConfigElement> p0) {
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

    bool default_configRemove(boost::shared_ptr<jccl::ConfigElement> p0) {
        return vrj::Kernel::configRemove(p0);
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
        return vrj::Kernel::configProcessPending();
    }

    PyObject* self;
};

void vrj_Kernel_waitForKernelStop(vrj::Kernel* kernel)
{
   // A VR Juggler application written entirely in Python will call
   // vrj.Kernel.waitForKernelStop() and block until the kernel shuts down.
   // This is definitely a blocking I/O kind of case, so we need to release
   // the global interpreter lock before we block on the kernel shutdown
   // condition.
   Py_BEGIN_ALLOW_THREADS;
      kernel->waitForKernelStop();
   Py_END_ALLOW_THREADS;
}

}// namespace 


// Module ======================================================================
void _Export_Kernel()
{
    class_< vrj::Kernel, boost::noncopyable, vrj_Kernel_Wrapper >("Kernel", no_init)
        .def("configProcessPending", &jccl::ConfigElementHandler::configProcessPending, &vrj_Kernel_Wrapper::default_configProcessPending)
        .def("start", &vrj::Kernel::start)
        .def("stop", &vrj::Kernel::stop)
        .def("isRunning", &vrj::Kernel::isRunning)
        .def("waitForKernelStop", vrj_Kernel_waitForKernelStop)
        .def("setApplication", &vrj::Kernel::setApplication)
        .def("loadConfigFile", (void (vrj::Kernel::*)(const char *) )&vrj::Kernel::loadConfigFile)
        .def("scanForConfigDefinitions", (void (vrj::Kernel::*)(const std::basic_string<char,std::char_traits<char>,std::allocator<char> >&) )&vrj::Kernel::scanForConfigDefinitions)
        .def("getUser", &vrj::Kernel::getUser, return_internal_reference< 1 >())
        .def("getUsers", &vrj::Kernel::getUsers)
        .def("instance", &vrj::Kernel::instance, return_value_policy< reference_existing_object >())
        .staticmethod("instance")
    ;

}
