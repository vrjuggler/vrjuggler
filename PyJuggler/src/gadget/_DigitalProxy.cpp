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
#include <gadget/Type/DigitalProxy.h>

// Using =======================================================================
using namespace boost::python;

// Declarations ================================================================


namespace pyj {


struct gadget_DigitalProxy_Wrapper: gadget::DigitalProxy
{
    gadget_DigitalProxy_Wrapper(PyObject* self_, const gadget::DigitalProxy & p0):
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
        return gadget::DigitalProxy::config(p0);
    }

    void set(std::basic_string<char,std::char_traits<char>,std::allocator<char> > p0, gadget::Digital * p1) {
        try
        {
            call_method< void >(self, "set", p0, p1);
        }
        catch(error_already_set)
        {
            PyErr_Print();
        }
    }

    void default_set(std::basic_string<char,std::char_traits<char>,std::allocator<char> > p0, gadget::Digital * p1) {
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
        .def(init< const gadget::DigitalProxy & >())
        .def("updateData", &gadget::DigitalProxy::updateData, &pyj::gadget_DigitalProxy_Wrapper::default_updateData)
        .def("config", &gadget::DigitalProxy::config, &pyj::gadget_DigitalProxy_Wrapper::default_config)
        .def("set", &gadget::TypedProxy<gadget::Digital>::set, &pyj::gadget_DigitalProxy_Wrapper::default_set)
        .def("refresh", &gadget::TypedProxy<gadget::Digital>::refresh, &pyj::gadget_DigitalProxy_Wrapper::default_refresh)
        .def("getDeviceName", &gadget::TypedProxy<gadget::Digital>::getDeviceName, &pyj::gadget_DigitalProxy_Wrapper::default_getDeviceName)
        .def("isStupified", &gadget::Proxy::isStupified, &pyj::gadget_DigitalProxy_Wrapper::default_isStupified)
        .def("getTimeStamp", &gadget::DigitalProxy::getTimeStamp)
        .def("getData", &gadget::DigitalProxy::getData)
        .def("getDigitalData", &gadget::DigitalProxy::getDigitalData, return_internal_reference< 1 >())
        .def("getDigitalPtr", &gadget::DigitalProxy::getDigitalPtr, return_internal_reference< 1 >())
        .def("getUnit", &gadget::DigitalProxy::getUnit)
        .def("getElementType", (std::basic_string<char,std::char_traits<char>,std::allocator<char> > (*)())&gadget::DigitalProxy::getElementType)
        .staticmethod("getElementType")
        .def("getName", &gadget::Proxy::getName)
        .def("setName", &gadget::Proxy::setName)
        .def("stupify", &gadget::Proxy::stupify, pyj::gadget_Proxy_stupify_overloads_0_1())
    ;

}
