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
#include <gadget/Type/Proxy.h>
#include <gadget/InputManager.h>

// Using =======================================================================
using namespace boost::python;

// Declarations ================================================================
namespace pyj {

struct gadget_InputManager_Wrapper: gadget::InputManager
{
    bool configAdd(jccl::ConfigElementPtr p0) {
        return call_method< bool >(self, "configAdd", p0);
    }

    bool default_configAdd(jccl::ConfigElementPtr p0) {
        return gadget::InputManager::configAdd(p0);
    }

    bool configRemove(jccl::ConfigElementPtr p0) {
        return call_method< bool >(self, "configRemove", p0);
    }

    bool default_configRemove(jccl::ConfigElementPtr p0) {
        return gadget::InputManager::configRemove(p0);
    }

    bool configCanHandle(jccl::ConfigElementPtr p0) {
        return call_method< bool >(self, "configCanHandle", p0);
    }

    bool default_configCanHandle(jccl::ConfigElementPtr p0) {
        return gadget::InputManager::configCanHandle(p0);
    }

    int configProcessPending() {
        return call_method< int >(self, "configProcessPending");
    }

    int default_configProcessPending() {
        return jccl::ConfigElementHandler::configProcessPending();
    }

    PyObject* self;
};


}// namespace 


// Module ======================================================================
void _Export_InputManager()
{
    class_< gadget::InputManager, boost::noncopyable, pyj::gadget_InputManager_Wrapper >("InputManager", no_init)
        .def("configAdd", (bool (gadget::InputManager::*)(jccl::ConfigElementPtr) )&gadget::InputManager::configAdd, (bool (pyj::gadget_InputManager_Wrapper::*)(jccl::ConfigElementPtr))&pyj::gadget_InputManager_Wrapper::default_configAdd)
        .def("configRemove", (bool (gadget::InputManager::*)(jccl::ConfigElementPtr) )&gadget::InputManager::configRemove, (bool (pyj::gadget_InputManager_Wrapper::*)(jccl::ConfigElementPtr))&pyj::gadget_InputManager_Wrapper::default_configRemove)
        .def("configCanHandle", (bool (gadget::InputManager::*)(jccl::ConfigElementPtr) )&gadget::InputManager::configCanHandle, (bool (pyj::gadget_InputManager_Wrapper::*)(jccl::ConfigElementPtr))&pyj::gadget_InputManager_Wrapper::default_configCanHandle)
        .def("configProcessPending", (int (jccl::ConfigElementHandler::*)() )&jccl::ConfigElementHandler::configProcessPending, (int (pyj::gadget_InputManager_Wrapper::*)())&pyj::gadget_InputManager_Wrapper::default_configProcessPending)
        .def("instance", &gadget::InputManager::instance, return_value_policy< reference_existing_object >())
        .def("getDisplaySystemElement", &gadget::InputManager::getDisplaySystemElement)
        .def("configureDevice", &gadget::InputManager::configureDevice)
        .def("updateAllData", &gadget::InputManager::updateAllData)
        .def("getDevice", &gadget::InputManager::getDevice, return_internal_reference< 1 >())
        .def("addDevice", &gadget::InputManager::addDevice)
        .def("addRemoteDevice", &gadget::InputManager::addRemoteDevice)
        .def("removeDevice", (bool (gadget::InputManager::*)(const std::string&) )&gadget::InputManager::removeDevice)
        .def("removeDevice", (bool (gadget::InputManager::*)(const gadget::Input*) )&gadget::InputManager::removeDevice)
        .def("addProxy", &gadget::InputManager::addProxy)
        .def("getProxy", &gadget::InputManager::getProxy, return_internal_reference< 1 >())
        .def("refreshAllProxies", &gadget::InputManager::refreshAllProxies)
        .def("getInputLogger", &gadget::InputManager::getInputLogger)
        .staticmethod("instance")
//        .def( other< std::ostream >() << self )
    ;

}

