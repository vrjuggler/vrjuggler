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
#include <gadget/Type/PositionProxy.h>

// Using =======================================================================
using namespace boost::python;

// Declarations ================================================================


namespace pyj {


struct gadget_PositionProxy_Wrapper: gadget::PositionProxy
{
    gadget_PositionProxy_Wrapper(PyObject* self_, const gadget::PositionProxy & p0):
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
        return gadget::PositionProxy::config(p0);
    }

    void set(std::basic_string<char,std::char_traits<char>,std::allocator<char> > p0, gadget::Position * p1) {
        try
        {
            call_method< void >(self, "set", p0, p1);
        }
        catch(error_already_set)
        {
            PyErr_Print();
        }
    }

    void default_set(std::basic_string<char,std::char_traits<char>,std::allocator<char> > p0, gadget::Position * p1) {
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
        .def(init< const gadget::PositionProxy & >())
        .def("updateData", &gadget::PositionProxy::updateData, &pyj::gadget_PositionProxy_Wrapper::default_updateData)
        .def("config", &gadget::PositionProxy::config, &pyj::gadget_PositionProxy_Wrapper::default_config)
        .def("set", &gadget::TypedProxy<gadget::Position>::set, &pyj::gadget_PositionProxy_Wrapper::default_set)
        .def("refresh", &gadget::TypedProxy<gadget::Position>::refresh, &pyj::gadget_PositionProxy_Wrapper::default_refresh)
        .def("getDeviceName", &gadget::TypedProxy<gadget::Position>::getDeviceName, &pyj::gadget_PositionProxy_Wrapper::default_getDeviceName)
        .def("isStupified", &gadget::Proxy::isStupified, &pyj::gadget_PositionProxy_Wrapper::default_isStupified)
        .def("getTimeStamp", &gadget::PositionProxy::getTimeStamp)
        .def("getData", &gadget::PositionProxy::getData, pyj::gadget_PositionProxy_getData_overloads_0_1())
        .def("getPositionData", &gadget::PositionProxy::getPositionData, return_internal_reference< 1 >())
        .def("getUnit", &gadget::PositionProxy::getUnit)
        .def("getPositionPtr", &gadget::PositionProxy::getPositionPtr, return_internal_reference< 1 >())
        .def("getElementType", (std::basic_string<char,std::char_traits<char>,std::allocator<char> > (*)())&gadget::PositionProxy::getElementType)
        .staticmethod("getElementType")
        .def("getName", &gadget::Proxy::getName)
        .def("setName", &gadget::Proxy::setName)
        .def("stupify", &gadget::Proxy::stupify, pyj::gadget_Proxy_stupify_overloads_0_1())
    ;

}
