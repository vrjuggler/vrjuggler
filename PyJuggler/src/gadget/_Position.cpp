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
#include <vpr/IO/ObjectReader.h>
#include <vpr/IO/ObjectWriter.h>
#include <gadget/Type/Position.h>

// Using =======================================================================
using namespace boost::python;

// Declarations ================================================================
namespace pyj {

struct gadget_Position_Wrapper: gadget::Position
{
    gadget_Position_Wrapper(PyObject* self_):
        gadget::Position(), self(self_) {}

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
        return gadget::Position::config(p0);
    }

    std::string getBaseType() {
        try
        {
            return call_method< std::string >(self, "getBaseType");
        }
        catch(error_already_set)
        {
            PyErr_Print();
        }

        return std::string("UNKNOWN");
    }

    std::string default_getBaseType() {
        return gadget::Position::getBaseType();
    }

    vpr::ReturnStatus writeObject(vpr::ObjectWriter* p0) {
        return call_method< vpr::ReturnStatus >(self, "writeObject", p0);
    }

    vpr::ReturnStatus default_writeObject(vpr::ObjectWriter* p0) {
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

    vpr::ReturnStatus readObject(vpr::ObjectReader* p0) {
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

    vpr::ReturnStatus default_readObject(vpr::ObjectReader* p0) {
        return gadget::Position::readObject(p0);
    }

    PyObject* self;
};

BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(gadget_Position_getPositionData_overloads_0_1, getPositionData, 0, 1)

}// namespace 


// Module ======================================================================
void _Export_Position()
{
    class_< gadget::Position, boost::noncopyable, pyj::gadget_Position_Wrapper >("Position", init<  >())
        .def("config", &gadget::Position::config, &pyj::gadget_Position_Wrapper::default_config)
        .def("getBaseType", &gadget::Position::getBaseType, &pyj::gadget_Position_Wrapper::default_getBaseType)
        .def("writeObject", (vpr::ReturnStatus (gadget::Position::*)(vpr::ObjectWriter*) )&gadget::Position::writeObject, (vpr::ReturnStatus (pyj::gadget_Position_Wrapper::*)(vpr::ObjectWriter*))&pyj::gadget_Position_Wrapper::default_writeObject)
        .def("readObject", (vpr::ReturnStatus (gadget::Position::*)(vpr::ObjectReader*) )&gadget::Position::readObject, (vpr::ReturnStatus (pyj::gadget_Position_Wrapper::*)(vpr::ObjectReader*))&pyj::gadget_Position_Wrapper::default_readObject)
        .def("getPositionData", &gadget::Position::getPositionData, pyj::gadget_Position_getPositionData_overloads_0_1())
        .def("addPositionSample", &gadget::Position::addPositionSample)
        .def("swapPositionBuffers", &gadget::Position::swapPositionBuffers)
        .def("getPositionDataBuffer", &gadget::Position::getPositionDataBuffer, return_internal_reference< 1 >())
    ;

}
