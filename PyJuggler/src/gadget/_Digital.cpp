// PyJuggler is (C) Copyright 2002-2004 by Patrick Hartling
// Distributed under the GNU Lesser General Public License 2.1.  (See
// accompanying file COPYING.txt or http://www.gnu.org/copyleft/lesser.txt)

// File:          $RCSfile$
// Date modified: $Date$
// Version:       $Revision$

// Includes ====================================================================
#include <boost/python.hpp>
#include <boost/cstdint.hpp>

// Includes ====================================================================
#include <vpr/IO/ObjectReader.h>
#include <vpr/IO/ObjectWriter.h>
#include <gadget/Type/Digital.h>

// Using =======================================================================
using namespace boost::python;

// Declarations ================================================================
namespace pyj {

struct gadget_Digital_Wrapper: gadget::Digital
{
    gadget_Digital_Wrapper(PyObject* self_):
        gadget::Digital(), self(self_) {}

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
        return gadget::Digital::config(p0);
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

        return std::string("UNKONWN");
    }

    std::string default_getBaseType() {
        return gadget::Digital::getBaseType();
    }

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
        return gadget::Digital::writeObject(p0);
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
        .def("writeObject", (vpr::ReturnStatus (gadget::Digital::*)(vpr::ObjectWriter*) )&gadget::Digital::writeObject, (vpr::ReturnStatus (pyj::gadget_Digital_Wrapper::*)(vpr::ObjectWriter*))&pyj::gadget_Digital_Wrapper::default_writeObject)
        .def("readObject", (vpr::ReturnStatus (gadget::Digital::*)(vpr::ObjectReader*) )&gadget::Digital::readObject, (vpr::ReturnStatus (pyj::gadget_Digital_Wrapper::*)(vpr::ObjectReader*))&pyj::gadget_Digital_Wrapper::default_readObject)
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
