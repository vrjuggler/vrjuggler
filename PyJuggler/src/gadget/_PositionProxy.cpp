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

BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(gadget_PositionProxy_getData_overloads_0_1, getData, 0, 1)

BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(gadget_Proxy_stupify_overloads_0_1, stupify, 0, 1)

}// namespace 


// Module ======================================================================
void _Export_PositionProxy()
{
    class_< gadget::PositionProxy, pyj::gadget_PositionProxy_Wrapper >("PositionProxy", init<  >())
        .def(init< const gadget::PositionProxy& >())
        .def("updateData", (void (gadget::PositionProxy::*)() )&gadget::PositionProxy::updateData, (void (pyj::gadget_PositionProxy_Wrapper::*)())&pyj::gadget_PositionProxy_Wrapper::default_updateData)
        .def("getTimeStamp", (vpr::Interval (gadget::PositionProxy::*)() const)&gadget::PositionProxy::getTimeStamp, (vpr::Interval (pyj::gadget_PositionProxy_Wrapper::*)() const)&pyj::gadget_PositionProxy_Wrapper::default_getTimeStamp)
        .def("config", (bool (gadget::PositionProxy::*)(jccl::ConfigElementPtr) )&gadget::PositionProxy::config, (bool (pyj::gadget_PositionProxy_Wrapper::*)(jccl::ConfigElementPtr))&pyj::gadget_PositionProxy_Wrapper::default_config)
        .def("set", (void (gadget::TypedProxy<gadget::Position>::*)(std::string, gadget::Position*) )&gadget::TypedProxy<gadget::Position>::set, (void (pyj::gadget_PositionProxy_Wrapper::*)(std::string, gadget::Position*))&pyj::gadget_PositionProxy_Wrapper::default_set)
        .def("refresh", (bool (gadget::TypedProxy<gadget::Position>::*)() )&gadget::TypedProxy<gadget::Position>::refresh, (bool (pyj::gadget_PositionProxy_Wrapper::*)())&pyj::gadget_PositionProxy_Wrapper::default_refresh)
        .def("getDeviceName", (std::string (gadget::TypedProxy<gadget::Position>::*)() const)&gadget::TypedProxy<gadget::Position>::getDeviceName, (std::string (pyj::gadget_PositionProxy_Wrapper::*)() const)&pyj::gadget_PositionProxy_Wrapper::default_getDeviceName)
        .def("isStupified", (bool (gadget::Proxy::*)() const)&gadget::Proxy::isStupified, (bool (pyj::gadget_PositionProxy_Wrapper::*)() const)&pyj::gadget_PositionProxy_Wrapper::default_isStupified)
        .def("getData", &gadget::PositionProxy::getData, pyj::gadget_PositionProxy_getData_overloads_0_1())
        .def("getPositionData", &gadget::PositionProxy::getPositionData, return_internal_reference< 1 >())
        .def("getUnit", &gadget::PositionProxy::getUnit)
        .def("getPositionPtr", &gadget::PositionProxy::getPositionPtr, return_internal_reference< 1 >())
        .def("getElementType", &gadget::PositionProxy::getElementType)
        .def("getName", &gadget::Proxy::getName)
        .def("setName", &gadget::Proxy::setName)
        .def("stupify", &gadget::Proxy::stupify, pyj::gadget_Proxy_stupify_overloads_0_1())
        .staticmethod("getElementType")
    ;

}
