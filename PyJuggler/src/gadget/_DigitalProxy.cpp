// PyJuggler is (C) Copyright 2002-2004 by Patrick Hartling
// Distributed under the GNU Lesser General Public License 2.1.  (See
// accompanying file COPYING.txt or http://www.gnu.org/copyleft/lesser.txt)

// File:          $RCSfile$
// Date modified: $Date$
// Version:       $Revision$

// Includes ====================================================================
#include <boost/python.hpp>
#include <boost/cstdint.hpp>

// Includes ====================================================================
#include <gadget/Type/DigitalProxy.h>

// Using =======================================================================
using namespace boost::python;

// Declarations ================================================================
namespace pyj {

struct gadget_DigitalProxy_Wrapper: gadget::DigitalProxy
{
    gadget_DigitalProxy_Wrapper(PyObject* self_, const gadget::DigitalProxy& p0):
        gadget::DigitalProxy(p0), self(self_) {}

    gadget_DigitalProxy_Wrapper(PyObject* self_):
        gadget::DigitalProxy(), self(self_) {}

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
        gadget::DigitalProxy::updateData();
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
        return gadget::DigitalProxy::getTimeStamp();
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
        return gadget::DigitalProxy::config(p0);
    }

    void set(std::string p0, gadget::Digital* p1) {
        try
        {
            call_method< void >(self, "set", p0, p1);
        }
        catch(error_already_set)
        {
            PyErr_Print();
        }
    }

    void default_set(std::string p0, gadget::Digital* p1) {
        gadget::DigitalProxy::set(p0, p1);
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
        return gadget::DigitalProxy::refresh();
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
        return gadget::DigitalProxy::getDeviceName();
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
        return gadget::DigitalProxy::isStupified();
    }

    PyObject* self;
};

}// namespace 


// Module ======================================================================
void _Export_DigitalProxy()
{
    class_< gadget::DigitalProxy, pyj::gadget_DigitalProxy_Wrapper >("DigitalProxy",
         "A proxy class to digital devices used by the Input Manager.\n\n"
         "A digital proxy always points to a digital device and a unit\n"
         "number within that device.  The Input Manager can therefore keep\n"
         "an array of these around and treat them as digital devices that\n"
         "only return a single sub-device's amount of data (one int)."
         ,
         init<  >()
        )
        .def(init< const gadget::DigitalProxy& >())
        .def("updateData",
             (void (gadget::DigitalProxy::*)() )&gadget::DigitalProxy::updateData,
             (void (pyj::gadget_DigitalProxy_Wrapper::*)())&pyj::gadget_DigitalProxy_Wrapper::default_updateData,
             "updateData()\n"
             "Updates the cached data copy from the device."
         )
        .def("getTimeStamp",
             (vpr::Interval (gadget::DigitalProxy::*)() const)&gadget::DigitalProxy::getTimeStamp,
             (vpr::Interval (pyj::gadget_DigitalProxy_Wrapper::*)() const)&pyj::gadget_DigitalProxy_Wrapper::default_getTimeStamp,
             "getTimeStamp() -> vpr.Interval object\n"
             "Returns the time of the last update."
         )
        .def("config",
             (bool (gadget::DigitalProxy::*)(jccl::ConfigElementPtr) )&gadget::DigitalProxy::config,
             (bool (pyj::gadget_DigitalProxy_Wrapper::*)(jccl::ConfigElementPtr))&pyj::gadget_DigitalProxy_Wrapper::default_config,
             "config(element) -> Boolean\n"
             "Configures this proxy using the given jccl.ConfigElement\n"
             "instance."
         )
        .def("set",
             (void (gadget::TypedProxy<gadget::Digital>::*)(std::string,gadget::Digital*) )&gadget::TypedProxy<gadget::Digital>::set,
             (void (pyj::gadget_DigitalProxy_Wrapper::*)(std::string, gadget::Digital*))&pyj::gadget_DigitalProxy_Wrapper::default_set,
             "set(devName, devPtr)\n"
             "Sets the proxy to point to the given type-specific device.\n"
             "Pre-condition:\n"
             "devPtr must be a valid device of type gadget.Digital\n"
             "Post-condition:\n"
             "The proxy now references the given device.  The device name we\n"
             "are proxying is set to devPtr->getInstanceName()."
             "Arguments:\n"
             "devName -- The name of the device at which we are pointing.\n"
             "devPtr  -- Pointer to the device.  For gadget.Digital,\n"
             "           this points to an instance of gadget.Digital."
         )
        .def("refresh",
             (bool (gadget::TypedProxy<gadget::Digital>::*)() )&gadget::TypedProxy<gadget::Digital>::refresh,
             (bool (pyj::gadget_DigitalProxy_Wrapper::*)())&pyj::gadget_DigitalProxy_Wrapper::default_refresh,
             "refresh() -> Boolean\n"
             "Refreshes the proxy.  This attempts to lookup the device that\n"
             "we are proxying.  If the lookup fails, then we become\n"
             "stupified.  If not, then the proxy is pointed at this\n"
             "potentially new device."
         )
        .def("getDeviceName",
             (std::string (gadget::TypedProxy<gadget::Digital>::*)() const)&gadget::TypedProxy<gadget::Digital>::getDeviceName,
             (std::string (pyj::gadget_DigitalProxy_Wrapper::*)() const)&pyj::gadget_DigitalProxy_Wrapper::default_getDeviceName,
             "getDeviceName() -> string object\n"
             "Gets the name of the device that we are proxying."
         )
        .def("isStupified",
             (bool (gadget::Proxy::*)() const)&gadget::Proxy::isStupified,
             (bool (pyj::gadget_DigitalProxy_Wrapper::*)() const)&pyj::gadget_DigitalProxy_Wrapper::default_isStupified,
             "isStupified() -> Boolean\n"
             "Is the proxy currently stupified?\n"
             "If the device we are proxying does not exist, then this will\n"
             "return True."
         )
        .def("getData", &gadget::DigitalProxy::getData,
             "getData() -> float\n"
             "Gets the current digital data value."
         )
        .def("getDigitalData", &gadget::DigitalProxy::getDigitalData,
             return_internal_reference< 1 >()
         )
        .def("getDigitalPtr", &gadget::DigitalProxy::getDigitalPtr,
             return_internal_reference< 1 >(),
             "getDigitalPtr() -> gadget.Digital object\n"
         )
        .def("getUnit", &gadget::DigitalProxy::getUnit,
             "getUnit() -> int\n"
             "Returns the unit index into the analog device from which this\n"
             "proxy is reading data."
         )
        .def("getElementType", &gadget::DigitalProxy::getElementType,
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
