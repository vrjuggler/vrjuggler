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
#include <boost/cstdint.hpp>

// Includes ====================================================================
#include <gadget/Type/AnalogProxy.h>

// Using =======================================================================
using namespace boost::python;

// Declarations ================================================================
namespace pyj {

struct gadget_AnalogProxy_Wrapper: gadget::AnalogProxy
{
    gadget_AnalogProxy_Wrapper(PyObject* self_, const gadget::AnalogProxy& p0):
        gadget::AnalogProxy(p0), self(self_) {}

    gadget_AnalogProxy_Wrapper(PyObject* self_):
        gadget::AnalogProxy(), self(self_) {}

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
        gadget::AnalogProxy::updateData();
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
        return gadget::AnalogProxy::getTimeStamp();
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
        return gadget::AnalogProxy::config(p0);
    }

    void set(std::string p0, gadget::Analog* p1) {
        try
        {
            call_method< void >(self, "set", p0, p1);
        }
        catch(error_already_set)
        {
            PyErr_Print();
        }
    }

    void default_set(std::string p0, gadget::Analog* p1) {
        gadget::AnalogProxy::set(p0, p1);
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
        return gadget::AnalogProxy::refresh();
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
        return gadget::AnalogProxy::getDeviceName();
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
        return gadget::AnalogProxy::isStupified();
    }

    PyObject* self;
};

BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(gadget_Proxy_stupify_overloads_0_1, stupify, 0, 1)

}// namespace 


// Module ======================================================================
void _Export_AnalogProxy()
{
    class_< gadget::AnalogProxy, pyj::gadget_AnalogProxy_Wrapper >("AnalogProxy", init<  >())
        .def(init< const gadget::AnalogProxy& >())
        .def("updateData", (void (gadget::AnalogProxy::*)() )&gadget::AnalogProxy::updateData, (void (pyj::gadget_AnalogProxy_Wrapper::*)())&pyj::gadget_AnalogProxy_Wrapper::default_updateData)
        .def("getTimeStamp", (vpr::Interval (gadget::AnalogProxy::*)() const)&gadget::AnalogProxy::getTimeStamp, (vpr::Interval (pyj::gadget_AnalogProxy_Wrapper::*)() const)&pyj::gadget_AnalogProxy_Wrapper::default_getTimeStamp)
        .def("config", (bool (gadget::AnalogProxy::*)(jccl::ConfigElementPtr) )&gadget::AnalogProxy::config, (bool (pyj::gadget_AnalogProxy_Wrapper::*)(jccl::ConfigElementPtr))&pyj::gadget_AnalogProxy_Wrapper::default_config)
        .def("set", (void (gadget::TypedProxy<gadget::Analog>::*)(std::string, gadget::Analog*) )&gadget::TypedProxy<gadget::Analog>::set, (void (pyj::gadget_AnalogProxy_Wrapper::*)(std::string, gadget::Analog*))&pyj::gadget_AnalogProxy_Wrapper::default_set)
        .def("refresh", (bool (gadget::TypedProxy<gadget::Analog>::*)() )&gadget::TypedProxy<gadget::Analog>::refresh, (bool (pyj::gadget_AnalogProxy_Wrapper::*)())&pyj::gadget_AnalogProxy_Wrapper::default_refresh)
        .def("getDeviceName", (std::string (gadget::TypedProxy<gadget::Analog>::*)() const)&gadget::TypedProxy<gadget::Analog>::getDeviceName, (std::string (pyj::gadget_AnalogProxy_Wrapper::*)() const)&pyj::gadget_AnalogProxy_Wrapper::default_getDeviceName)
        .def("isStupified", (bool (gadget::Proxy::*)() const)&gadget::Proxy::isStupified, (bool (pyj::gadget_AnalogProxy_Wrapper::*)() const)&pyj::gadget_AnalogProxy_Wrapper::default_isStupified)
        .def("getData", &gadget::AnalogProxy::getData)
        .def("getAnalogPtr", &gadget::AnalogProxy::getAnalogPtr, return_internal_reference< 1 >())
        .def("getUnit", &gadget::AnalogProxy::getUnit)
        .def("getElementType", &gadget::AnalogProxy::getElementType)
        .def("getName", &gadget::Proxy::getName)
        .def("setName", &gadget::Proxy::setName)
        .def("stupify", &gadget::Proxy::stupify, pyj::gadget_Proxy_stupify_overloads_0_1())
        .staticmethod("getElementType")
    ;

}
