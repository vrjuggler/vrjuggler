// PyJuggler is (C) Copyright 2002-2005 by Patrick Hartling
// Distributed under the GNU Lesser General Public License 2.1.  (See
// accompanying file COPYING.txt or http://www.gnu.org/copyleft/lesser.txt)

// File:          $RCSfile$
// Date modified: $Date$
// Version:       $Revision$

// Includes ====================================================================
#include <boost/python.hpp>
#include <boost/cstdint.hpp>

// Includes ====================================================================
#include <gadget/Type/CommandProxy.h>

// Using =======================================================================
using namespace boost::python;

// Declarations ================================================================
namespace pyj {

struct gadget_CommandProxy_Wrapper: gadget::CommandProxy
{
    gadget_CommandProxy_Wrapper(PyObject* self_, const gadget::CommandProxy& p0):
        gadget::CommandProxy(p0), self(self_) {}

    gadget_CommandProxy_Wrapper(PyObject* self_):
        gadget::CommandProxy(), self(self_) {}

    void updateData() {
        try
        {
            call_method< void >(self, "updateData");
        }
        catch(error_already_set)
        {
            PyErr_Print();
        }
    }

    void default_updateData() {
        gadget::CommandProxy::updateData();
    }

    vpr::Interval getTimeStamp() const {
        try
        {
           return call_method< vpr::Interval >(self, "getTimeStamp");
        }
        catch(error_already_set)
        {
            PyErr_Print();
        }

        return vpr::Interval();
    }

    vpr::Interval default_getTimeStamp() const {
        return gadget::CommandProxy::getTimeStamp();
    }

    bool config(jccl::ConfigElementPtr p0) {
        try
        {
            return call_method< bool >(self, "config", p0);
        }
        catch(error_already_set)
        {
            PyErr_Print();
        }

        return false;
    }

    bool default_config(jccl::ConfigElementPtr p0) {
        return gadget::CommandProxy::config(p0);
    }

    void set(std::string p0, gadget::Command* p1) {
        try
        {
            call_method< void >(self, "set", p0, p1);
        }
        catch(error_already_set)
        {
            PyErr_Print();
        }
    }

    void default_set(std::string p0, gadget::Command* p1) {
        gadget::CommandProxy::set(p0, p1);
    }

    bool refresh() {
        try
        {
            return call_method< bool >(self, "refresh");
        }
        catch(error_already_set)
        {
            PyErr_Print();
        }

        return false;
    }

    bool default_refresh() {
        return gadget::CommandProxy::refresh();
    }

    std::string getDeviceName() const {
        try
        {
            return call_method< std::string >(self, "getDeviceName");
        }
        catch(error_already_set)
        {
            PyErr_Print();
        }

        return std::string("UNKNOWN");
    }

    std::string default_getDeviceName() const {
        return gadget::CommandProxy::getDeviceName();
    }

    bool isStupified() const {
        try
        {
            return call_method< bool >(self, "isStupified");
        }
        catch(error_already_set)
        {
            PyErr_Print();
        }

        return false;
    }

    bool default_isStupified() const {
        return gadget::CommandProxy::isStupified();
    }

    PyObject* self;
};

}// namespace 


// Module ======================================================================
void _Export_CommandProxy()
{
    class_< gadget::CommandProxy, pyj::gadget_CommandProxy_Wrapper >("CommandProxy",
         "A proxy class to command-oriented devices used by the Input\n"
         "Manager.\n\n"
         "A command proxy always points to a command-oriented device and a\n"
         "unit number within that device.  The Input Manager can therefore\n"
         "keep an array of these around and treat them as command-oriented\n"
         "devices that only return a single sub-device's amount of data (one\n"
         "int)."
         ,
         init<  >()
        )
        .def(init< const gadget::CommandProxy& >())
        .def("updateData",
             (void (gadget::CommandProxy::*)() )&gadget::CommandProxy::updateData,
             (void (pyj::gadget_CommandProxy_Wrapper::*)())&pyj::gadget_CommandProxy_Wrapper::default_updateData,
             "updateData()\n"
             "Updates the cached data copy from the device."
         )
        .def("getTimeStamp",
             (vpr::Interval (gadget::CommandProxy::*)() const)&gadget::CommandProxy::getTimeStamp,
             (vpr::Interval (pyj::gadget_CommandProxy_Wrapper::*)() const)&pyj::gadget_CommandProxy_Wrapper::default_getTimeStamp,
             "getTimeStamp() -> vpr.Interval object\n"
             "Returns the time of the last update."
         )
        .def("config",
             (bool (gadget::CommandProxy::*)(jccl::ConfigElementPtr) )&gadget::CommandProxy::config,
             (bool (pyj::gadget_CommandProxy_Wrapper::*)(jccl::ConfigElementPtr))&pyj::gadget_CommandProxy_Wrapper::default_config,
             "config(element) -> Boolean\n"
             "Configures this proxy using the given jccl.ConfigElement\n"
             "instance."
         )
        .def("set",
             (void (gadget::TypedProxy<gadget::Command>::*)(std::string,gadget::Command*) )&gadget::TypedProxy<gadget::Command>::set,
             (void (pyj::gadget_CommandProxy_Wrapper::*)(std::string, gadget::Command*))&pyj::gadget_CommandProxy_Wrapper::default_set,
             "set(devName, devPtr)\n"
             "Sets the proxy to point to the given type-specific device.\n"
             "Pre-condition:\n"
             "devPtr must be a valid device of type gadget.Command\n"
             "Post-condition:\n"
             "The proxy now references the given device.  The device name we\n"
             "are proxying is set to devPtr.getInstanceName().\n"
             "Arguments:\n"
             "devName -- The name of the device at which we are pointing.\n"
             "devPtr  -- Pointer to the device.  For gadget.Command,\n"
             "           this points to an instance of gadget.Command."
         )
        .def("refresh",
             (bool (gadget::TypedProxy<gadget::Command>::*)() )&gadget::TypedProxy<gadget::Command>::refresh,
             (bool (pyj::gadget_CommandProxy_Wrapper::*)())&pyj::gadget_CommandProxy_Wrapper::default_refresh,
             "refresh() -> Boolean\n"
             "Refreshes the proxy.  This attempts to lookup the device that\n"
             "we are proxying.  If the lookup fails, then we become\n"
             "stupified.  If not, then the proxy is pointed at this\n"
             "potentially new device."
         )
        .def("getDeviceName",
             (std::string (gadget::TypedProxy<gadget::Command>::*)() const)&gadget::TypedProxy<gadget::Command>::getDeviceName,
             (std::string (pyj::gadget_CommandProxy_Wrapper::*)() const)&pyj::gadget_CommandProxy_Wrapper::default_getDeviceName,
             "getDeviceName() -> string object\n"
             "Gets the name of the device that we are proxying."
         )
        .def("isStupified",
             (bool (gadget::Proxy::*)() const)&gadget::Proxy::isStupified,
             (bool (pyj::gadget_CommandProxy_Wrapper::*)() const)&pyj::gadget_CommandProxy_Wrapper::default_isStupified,
             "isStupified() -> Boolean\n"
             "Is the proxy currently stupified?\n"
             "If the device we are proxying does not exist, then this will\n"
             "return True."
         )
        .def("getData", &gadget::CommandProxy::getData,
             "getData() -> float\n"
             "Gets the current command data value."
         )
        .def("getCommandData", &gadget::CommandProxy::getCommandData,
             return_internal_reference< 1 >()
         )
        .def("getCommandPtr",
             &gadget::CommandProxy::getCommandPtr,
             return_internal_reference< 1 >(),
             "getCommandPtr() -> gadget.Command object\n"
         )
        .def("getUnit", &gadget::CommandProxy::getUnit,
             "getUnit() -> int\n"
             "Returns the unit index into the command device from which this\n"
             "proxy is reading data."
         )
        .def("getElementType",
             &gadget::CommandProxy::getElementType,
             "getElementType() -> string object"
         )
        .def("getName", &gadget::Proxy::getName,
             "getName() -> string object\n"
             "Gets the name of the proxy."
         )
        .def("setName", &gadget::Proxy::setName,
             "setName(name)\n"
             "Sets the name of the proxy.\n"
             "Arguments:\n"
             "name -- The name for this proxy as a string object."
         )
        .def("stupify", &gadget::Proxy::stupify,
             "stupify(newState = True)\n"
             "Sets the stupification state of this proxy.\n"
             "Keyword arguments:\n"
             "newState -- The new state of stupification."
         )
        .staticmethod("getElementType")
    ;

}
