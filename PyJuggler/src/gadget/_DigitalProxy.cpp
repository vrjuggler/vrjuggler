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

BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(gadget_Proxy_stupify_overloads_0_1, stupify, 0, 1)

}// namespace 


// Module ======================================================================
void _Export_DigitalProxy()
{
    class_< gadget::DigitalProxy, pyj::gadget_DigitalProxy_Wrapper >("DigitalProxy", init<  >())
        .def(init< const gadget::DigitalProxy& >())
        .def("updateData", (void (gadget::DigitalProxy::*)() )&gadget::DigitalProxy::updateData, (void (pyj::gadget_DigitalProxy_Wrapper::*)())&pyj::gadget_DigitalProxy_Wrapper::default_updateData)
        .def("getTimeStamp", (vpr::Interval (gadget::DigitalProxy::*)() const)&gadget::DigitalProxy::getTimeStamp, (vpr::Interval (pyj::gadget_DigitalProxy_Wrapper::*)() const)&pyj::gadget_DigitalProxy_Wrapper::default_getTimeStamp)
        .def("config", (bool (gadget::DigitalProxy::*)(jccl::ConfigElementPtr) )&gadget::DigitalProxy::config, (bool (pyj::gadget_DigitalProxy_Wrapper::*)(jccl::ConfigElementPtr))&pyj::gadget_DigitalProxy_Wrapper::default_config)
        .def("set", (void (gadget::TypedProxy<gadget::Digital>::*)(std::string, gadget::Digital*) )&gadget::TypedProxy<gadget::Digital>::set, (void (pyj::gadget_DigitalProxy_Wrapper::*)(std::string, gadget::Digital*))&pyj::gadget_DigitalProxy_Wrapper::default_set)
        .def("refresh", (bool (gadget::TypedProxy<gadget::Digital>::*)() )&gadget::TypedProxy<gadget::Digital>::refresh, (bool (pyj::gadget_DigitalProxy_Wrapper::*)())&pyj::gadget_DigitalProxy_Wrapper::default_refresh)
        .def("getDeviceName", (std::string (gadget::TypedProxy<gadget::Digital>::*)() const)&gadget::TypedProxy<gadget::Digital>::getDeviceName, (std::string (pyj::gadget_DigitalProxy_Wrapper::*)() const)&pyj::gadget_DigitalProxy_Wrapper::default_getDeviceName)
        .def("isStupified", (bool (gadget::Proxy::*)() const)&gadget::Proxy::isStupified, (bool (pyj::gadget_DigitalProxy_Wrapper::*)() const)&pyj::gadget_DigitalProxy_Wrapper::default_isStupified)
        .def("getData", &gadget::DigitalProxy::getData)
        .def("getDigitalData", &gadget::DigitalProxy::getDigitalData, return_internal_reference< 1 >())
        .def("getDigitalPtr", &gadget::DigitalProxy::getDigitalPtr, return_internal_reference< 1 >())
        .def("getUnit", &gadget::DigitalProxy::getUnit)
        .def("getElementType", &gadget::DigitalProxy::getElementType)
        .def("getName", &gadget::Proxy::getName)
        .def("setName", &gadget::Proxy::setName)
        .def("stupify", &gadget::Proxy::stupify, pyj::gadget_Proxy_stupify_overloads_0_1())
        .staticmethod("getElementType")
    ;

}
