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
#include <vpr/IO/ObjectReader.h>
#include <vpr/IO/ObjectWriter.h>
#include <gadget/Type/Analog.h>

// Using =======================================================================
using namespace boost::python;

// Declarations ================================================================
namespace pyj {

struct gadget_Analog_Wrapper: gadget::Analog
{
    gadget_Analog_Wrapper(PyObject* self_):
        gadget::Analog(), self(self_) {}

    vpr::ReturnStatus writeObject(vpr::ObjectWriter* p0) {
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

    vpr::ReturnStatus default_writeObject(vpr::ObjectWriter* p0) {
        return gadget::Analog::writeObject(p0);
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
        return gadget::Analog::readObject(p0);
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
        return gadget::Analog::config(p0);
    }

    std::string getInputTypeName() {
        try
        {
            return call_method< std::string >(self, "getInputTypeName");
        }
        catch(error_already_set)
        {
            PyErr_Print();
        }

        return std::string("UNKONWN");
    }

    std::string default_getInputTypeName() {
        return gadget::Analog::getInputTypeName();
    }

    PyObject* self;
};

BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(gadget_Analog_getAnalogData_overloads_0_1, getAnalogData, 0, 1)

}// namespace 


// Module ======================================================================
void _Export_Analog()
{
    class_< gadget::Analog, boost::noncopyable, pyj::gadget_Analog_Wrapper >("Analog", init<  >())
        .def("writeObject", (vpr::ReturnStatus (gadget::Analog::*)(vpr::ObjectWriter*) )&gadget::Analog::writeObject, (vpr::ReturnStatus (pyj::gadget_Analog_Wrapper::*)(vpr::ObjectWriter*))&pyj::gadget_Analog_Wrapper::default_writeObject)
        .def("readObject", (vpr::ReturnStatus (gadget::Analog::*)(vpr::ObjectReader*) )&gadget::Analog::readObject, (vpr::ReturnStatus (pyj::gadget_Analog_Wrapper::*)(vpr::ObjectReader*))&pyj::gadget_Analog_Wrapper::default_readObject)
        .def("config", &gadget::Analog::config, &pyj::gadget_Analog_Wrapper::default_config)
        .def("getInputTypeName", &gadget::Analog::getInputTypeName, &pyj::gadget_Analog_Wrapper::default_getInputTypeName)
        .def("getAnalogData", &gadget::Analog::getAnalogData, pyj::gadget_Analog_getAnalogData_overloads_0_1())
        .def("addAnalogSample", &gadget::Analog::addAnalogSample)
        .def("swapAnalogBuffers", &gadget::Analog::swapAnalogBuffers)
        .def("getAnalogDataBuffer", &gadget::Analog::getAnalogDataBuffer, return_internal_reference< 1 >())
    ;

}
