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
#include <gadget/Type/PositionProxy.h>

// Using =======================================================================
using namespace boost::python;

// Declarations ================================================================
namespace pyj {

struct gadget_PositionProxy_Wrapper: gadget::PositionProxy
{
    gadget_PositionProxy_Wrapper(PyObject* self_, const gadget::PositionProxy& p0):
        gadget::PositionProxy(p0), self(self_) {}

    gadget_PositionProxy_Wrapper(PyObject* self_):
        gadget::PositionProxy(), self(self_) {}

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
        gadget::PositionProxy::updateData();
    }

    vpr::Interval getTimeStamp() const {
        return call_method< vpr::Interval >(self, "getTimeStamp");
    }

    vpr::Interval default_getTimeStamp() const {
        return gadget::PositionProxy::getTimeStamp();
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
        return gadget::PositionProxy::config(p0);
    }

    void set(std::string p0, gadget::Position* p1) {
        try
        {
            call_method< void >(self, "set", p0, p1);
        }
        catch(error_already_set)
        {
            PyErr_Print();
        }
    }

    void default_set(std::string p0, gadget::Position* p1) {
        gadget::PositionProxy::set(p0, p1);
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
        return gadget::PositionProxy::refresh();
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
        return gadget::PositionProxy::getDeviceName();
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
        return gadget::PositionProxy::isStupified();
    }

    PyObject* self;
};

}// namespace 


// Module ======================================================================
void _Export_PositionProxy()
{
    class_< gadget::PositionProxy, pyj::gadget_PositionProxy_Wrapper >("PositionProxy"
         "A proxy class to positional devices used by the Input Manager.\n\n"
         "A position proxy always points to a positional device and a unit\n"
         "number within that device.  The Input Manager can therefore keep\n"
         "an array of these around and treat them as positional devices that\n"
         "only return a single sub-device's amount of data (one int)."
         ,
         init<  >()
        )
        .def(init< const gadget::PositionProxy& >())
        .def("updateData",
             (void (gadget::PositionProxy::*)() )&gadget::PositionProxy::updateData,
             (void (pyj::gadget_PositionProxy_Wrapper::*)())&pyj::gadget_PositionProxy_Wrapper::default_updateData,
             "updateData()\n"
             "Updates the cached data copy from the device.  Copies the\n"
             "device data to local storage and transforms it if necessary."
         )
        .def("getTimeStamp",
             (vpr::Interval (gadget::PositionProxy::*)() const)&gadget::PositionProxy::getTimeStamp,
             (vpr::Interval (pyj::gadget_PositionProxy_Wrapper::*)() const)&pyj::gadget_PositionProxy_Wrapper::default_getTimeStamp,
             "getTimeStamp() -> vpr.Interval object\n"
             "Returns the time of the last update."
         )
        .def("config",
             (bool (gadget::PositionProxy::*)(jccl::ConfigElementPtr) )&gadget::PositionProxy::config,
             (bool (pyj::gadget_PositionProxy_Wrapper::*)(jccl::ConfigElementPtr))&pyj::gadget_PositionProxy_Wrapper::default_config,
             "config(element) -> Boolean\n"
             "Configures this proxy using the given jccl.ConfigElement\n"
             "instance."
         )
        .def("set",
             (void (gadget::TypedProxy<gadget::Position>::*)(std::string, gadget::Position*) )&gadget::TypedProxy<gadget::Position>::set,
             (void (pyj::gadget_PositionProxy_Wrapper::*)(std::string, gadget::Position*))&pyj::gadget_PositionProxy_Wrapper::default_set,
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
             (bool (gadget::TypedProxy<gadget::Position>::*)() )&gadget::TypedProxy<gadget::Position>::refresh,
             (bool (pyj::gadget_PositionProxy_Wrapper::*)())&pyj::gadget_PositionProxy_Wrapper::default_refresh,
             "refresh() -> Boolean\n"
             "Refreshes the proxy.  This attempts to lookup the device that\n"
             "we are proxying.  If the lookup fails, then we become\n"
             "stupified.  If not, then the proxy is pointed at this\n"
             "potentially new device."
         )
        .def("getDeviceName",
             (std::string (gadget::TypedProxy<gadget::Position>::*)() const)&gadget::TypedProxy<gadget::Position>::getDeviceName,
             (std::string (pyj::gadget_PositionProxy_Wrapper::*)() const)&pyj::gadget_PositionProxy_Wrapper::default_getDeviceName,
             "getDeviceName() -> string object\n"
             "Gets the name of the device that we are proxying."
         )
        .def("isStupified",
             (bool (gadget::Proxy::*)() const)&gadget::Proxy::isStupified,
             (bool (pyj::gadget_PositionProxy_Wrapper::*)() const)&pyj::gadget_PositionProxy_Wrapper::default_isStupified,
             "isStupified() -> Boolean\n"
             "Is the proxy currently stupified?\n"
             "If the device we are proxying does not exist, then this will\n"
             "return True."
         )
        .def("getData", &gadget::PositionProxy::getData,
             (args("scaleFactor") = gadget::PositionUnitConversion::ConvertToFeet),
             "getData(scaleFactor = 3.28) -> gmtl.Matrix44f object\n"
             "Gets the current positional data value from the device as a.\n"
             "matrix.  For example, getData(3.28) will return a matrix in\n"
             "feet.\n"
             "Keyword Arguments:\n"
             "scaleFactor -- Factor to convert from meters to the desired\n"
             "               units.  This paramter is optional.  If not\n"
             "               specified, it defaults to the conversion factor\n"
             "               from meters to feet."
         )
        .def("getPositionData", &gadget::PositionProxy::getPositionData,
             return_internal_reference< 1 >(),
             "getPositionData() -> gadget.PositionData object\n"
             "Gets the actual gadget.PositionData object."
         )
        .def("getUnit", &gadget::PositionProxy::getUnit,
             "getUnit() -> int\n"
             "Returns the unit index into the analog device from which this\n"
             "proxy is reading data."
         )
        .def("getPositionPtr", &gadget::PositionProxy::getPositionPtr,
             return_internal_reference< 1 >(),
             "getPositionPtr() -> gadget.Position object\n"
             "Returns the gadget.Position object held by this proxy."
         )
        .def("getElementType", &gadget::PositionProxy::getElementType,
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
