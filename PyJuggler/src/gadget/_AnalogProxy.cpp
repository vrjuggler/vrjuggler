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
#include <gadget/Type/AnalogProxy.h>

// Using =======================================================================
using namespace boost::python;

// Declarations ================================================================


namespace pyj {


struct gadget_AnalogProxy_Wrapper: gadget::AnalogProxy
{
    gadget_AnalogProxy_Wrapper(PyObject* self_, const gadget::AnalogProxy & p0):
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

    bool config(boost::shared_ptr<jccl::ConfigElement> p0) {
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

    bool default_config(boost::shared_ptr<jccl::ConfigElement> p0) {
        return gadget::AnalogProxy::config(p0);
    }

    void set(std::basic_string<char,std::char_traits<char>,std::allocator<char> > p0, gadget::Analog * p1) {
        try
        {
            call_method< void >(self, "set", p0, p1);
        }
        catch(error_already_set)
        {
            PyErr_Print();
        }
    }

    void default_set(std::basic_string<char,std::char_traits<char>,std::allocator<char> > p0, gadget::Analog * p1) {
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

    std::basic_string<char,std::char_traits<char>,std::allocator<char> > getDeviceName() const {
        try
        {
            return call_method< std::basic_string<char,std::char_traits<char>,std::allocator<char> > >(self, "getDeviceName");
        }
        catch(error_already_set)
        {
            PyErr_Print();
        }

        return std::string("UNKNOWN");
    }

    std::basic_string<char,std::char_traits<char>,std::allocator<char> > default_getDeviceName() const {
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
        .def(init< const gadget::AnalogProxy & >())
        .def("updateData", &gadget::AnalogProxy::updateData, &pyj::gadget_AnalogProxy_Wrapper::default_updateData)
        .def("config", &gadget::AnalogProxy::config, &pyj::gadget_AnalogProxy_Wrapper::default_config)
        .def("set", &gadget::TypedProxy<gadget::Analog>::set, &pyj::gadget_AnalogProxy_Wrapper::default_set)
        .def("refresh", &gadget::TypedProxy<gadget::Analog>::refresh, &pyj::gadget_AnalogProxy_Wrapper::default_refresh)
        .def("getDeviceName", &gadget::TypedProxy<gadget::Analog>::getDeviceName, &pyj::gadget_AnalogProxy_Wrapper::default_getDeviceName)
        .def("isStupified", &gadget::Proxy::isStupified, &pyj::gadget_AnalogProxy_Wrapper::default_isStupified)
        .def("getTimeStamp", &gadget::AnalogProxy::getTimeStamp)
        .def("getData", &gadget::AnalogProxy::getData)
        .def("getAnalogPtr", &gadget::AnalogProxy::getAnalogPtr, return_internal_reference< 1 >())
        .def("getUnit", &gadget::AnalogProxy::getUnit)
        .def("getElementType", (std::basic_string<char,std::char_traits<char>,std::allocator<char> > (*)())&gadget::AnalogProxy::getElementType)
        .staticmethod("getElementType")
        .def("getName", &gadget::Proxy::getName)
        .def("setName", &gadget::Proxy::setName)
        .def("stupify", &gadget::Proxy::stupify, pyj::gadget_Proxy_stupify_overloads_0_1())
    ;

}
