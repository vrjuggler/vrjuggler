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

    std::string getInputTypeName() {
        try
        {
            return call_method< std::string >(self, "getInputTypeName");
        }
        catch(error_already_set)
        {
            PyErr_Print();
        }

        return std::string("UNKNOWN");
    }

    std::string default_getInputTypeName() {
        return gadget::Position::getInputTypeName();
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

}// namespace 


// Module ======================================================================
void _Export_Position()
{
    class_< gadget::Position, boost::noncopyable, pyj::gadget_Position_Wrapper >("Position",
         "gadget.Position is the abstract base class from which devices\n"
         "returning positional (translation and/or rotation) data must\n"
         "derive.  This is in addition to gadget.Input.  gadget.Input\n"
         "provides pure virtual function constraints in the following\n"
         "functions: startSampling(), stopSampling(), sample(), and\n"
         "updateData()\.n\n"
         "gadget.Position adds the function getPositionDdta() for retrieving\n"
         "the received positional data.  This is similar to the additions\n"
         "made by gadget.Analog and gadget.Digital."
         ,
         init<  >()
        )
        .def("config", &gadget::Position::config,
             &pyj::gadget_Position_Wrapper::default_config,
             "config(element) -> Boolean\n"
             "Configures this positional device.\n"
             "Arguments:\n:"
             "element -- The config element for an positional device.  It\n"
             "           must derive from the base config element type\n"
             "           'positional_device'."
         )
        .def("getInputTypeName", &gadget::Position::getInputTypeName,
             &pyj::gadget_Position_Wrapper::default_getInputTypeName)
        .def("writeObject",
             (vpr::ReturnStatus (gadget::Position::*)(vpr::ObjectWriter*) )&gadget::Position::writeObject,
             (vpr::ReturnStatus (pyj::gadget_Position_Wrapper::*)(vpr::ObjectWriter*))&pyj::gadget_Position_Wrapper::default_writeObject,
             "writeObject(writer) -> vpr.ReturnStatus object\n"
             "Serializes this object."
         )
        .def("readObject",
             (vpr::ReturnStatus (gadget::Position::*)(vpr::ObjectReader*) )&gadget::Position::readObject,
             (vpr::ReturnStatus (pyj::gadget_Position_Wrapper::*)(vpr::ObjectReader*))&pyj::gadget_Position_Wrapper::default_readObject
             "readObject(reader) -> vpr.ReturnStatus object\n"
             "De-serializes this object."
         )
        .def("getPositionData", &gadget::Position::getPositionData,
             (args("devNum") = 0),
             "getPositionData(devNum = 0) -> gadget.PositionData object\n"
             "Returns positional data.\n"
             "Keyword arguments:\n"
             "devNum -- The device unit numbmer to access.  This parameter\n"
             "          optional.  It defaults to 0."
         )
        .def("addPositionSample", &gadget::Position::addPositionSample,
             "addDigitalSample(sampleList)\n"
             "Helper method to add a collection of positional samples to the\n"
             "sample buffers for this device.  This MUST be called by all\n"
             "positional devices to add a new sample.  The data samples\n"
             "passed in will then be modified by any local filters, so the.\n"
             "data does not need to transformed before being passed in.\n"
             "Post-condition:\n"
             "The given samples are added to the buffers, and the local\n"
             "filters are run on the new samples."
             "Arguments:\n"
             "sampleList -- The list of newly collected samples."
         )
        .def("swapPositionBuffers", &gadget::Position::swapPositionBuffers,
             "swapDigitalBuffers()\n"
             "Swaps the positional data buffers.\n"
             "Post-condition:\n"
             "If the ready queue has values, then those values are copied\n"
             "from the ready queue to the stable queue.  If not, the stable\n"
             "queue is not changed."
         )
        .def("getPositionDataBuffer", &gadget::Position::getPositionDataBuffer,
             return_internal_reference< 1 >(),
             "getPositionDataBuffer() -> list of lists of DigitalData objects\n"
             "Returns the current stable sample buffers for this device."
         )
    ;

}
