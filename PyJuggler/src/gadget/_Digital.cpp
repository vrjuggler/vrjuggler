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
#include <gadget/Type/Digital.h>

// Using =======================================================================
using namespace boost::python;

// Declarations ================================================================


namespace pyj {


struct gadget_Digital_Wrapper: gadget::Digital
{
    gadget_Digital_Wrapper(PyObject* self_):
        gadget::Digital(), self(self_) {}

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
        return gadget::Digital::config(p0);
    }

    std::basic_string<char,std::char_traits<char>,std::allocator<char> > getBaseType() {
        try
        {
            return call_method< std::basic_string<char,std::char_traits<char>,std::allocator<char> > >(self, "getBaseType");
        }
        catch(error_already_set)
        {
            PyErr_Print();
        }

        return std::string("UNKONWN");
    }

    std::basic_string<char,std::char_traits<char>,std::allocator<char> > default_getBaseType() {
        return gadget::Digital::getBaseType();
    }

    vpr::ReturnStatus writeObject(vpr::ObjectWriter * p0) {
        try
        {
            return call_method< vpr::ReturnStatus >(self, "writeObject", p0);
        }
        catch(error_already_set)
        {
            PyErr_Print();
        }

        return vpr::ReturnStatus::Fail;
    }

    vpr::ReturnStatus default_writeObject(vpr::ObjectWriter * p0) {
        return gadget::Digital::writeObject(p0);
    }

    vpr::ReturnStatus readObject(vpr::ObjectReader * p0) {
        try
        {
            return call_method< vpr::ReturnStatus >(self, "readObject", p0);
        }
        catch(error_already_set)
        {
            PyErr_Print();
        }

        return vpr::ReturnStatus::Fail;
    }

    vpr::ReturnStatus default_readObject(vpr::ObjectReader * p0) {
        return gadget::Digital::readObject(p0);
    }

    PyObject* self;
};

BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(gadget_Digital_getDigitalData_overloads_0_1, getDigitalData, 0, 1)



}// namespace 


// Module ======================================================================
void _Export_Digital()
{
    scope* gadget_Digital_scope = new scope(
    class_< gadget::Digital, boost::noncopyable, pyj::gadget_Digital_Wrapper >("Digital", init<  >())
        .def("config", &gadget::Digital::config, &pyj::gadget_Digital_Wrapper::default_config)
        .def("getBaseType", &gadget::Digital::getBaseType, &pyj::gadget_Digital_Wrapper::default_getBaseType)
        .def("writeObject", &gadget::Digital::writeObject, &pyj::gadget_Digital_Wrapper::default_writeObject)
        .def("readObject", &gadget::Digital::readObject, &pyj::gadget_Digital_Wrapper::default_readObject)
        .def("getDigitalData", &gadget::Digital::getDigitalData, pyj::gadget_Digital_getDigitalData_overloads_0_1())
        .def("addDigitalSample", &gadget::Digital::addDigitalSample)
        .def("swapDigitalBuffers", &gadget::Digital::swapDigitalBuffers)
        .def("getDigitalDataBuffer", &gadget::Digital::getDigitalDataBuffer, return_internal_reference< 1 >())
    );

    enum_< gadget::Digital::State >("State")
        .value("TOGGLE_OFF", gadget::Digital::TOGGLE_OFF)
        .value("ON", gadget::Digital::ON)
        .value("OFF", gadget::Digital::OFF)
        .value("TOGGLE_ON", gadget::Digital::TOGGLE_ON)
    ;

    delete gadget_Digital_scope;

}
