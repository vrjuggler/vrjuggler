// PyJuggler is (C) Copyright 2002-2004 by Patrick Hartling
// Distributed under the GNU Lesser General Public License 2.1.  (See
// accompanying file COPYING.txt or http://www.gnu.org/copyleft/lesser.txt)

// File:          $RCSfile$
// Date modified: $Date$
// Version:       $Revision$

// Boost Includes ==============================================================
#include <boost/python.hpp>
#include <boost/cstdint.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>

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

}// namespace 


// Module ======================================================================
void _Export_Analog()
{
    class_< gadget::Analog, boost::noncopyable, pyj::gadget_Analog_Wrapper >("Analog",
         "gadget.Analog is the abstract base class from which devices\n"
         "returning analog data must derive.  This is in addition to\n"
         "gadget.Input.  gadget.Input provides pure virtual function\n"
         "constraints in the following functions: startSampling(),\n"
         "stopSampling(), sample(), and updateData().\n\n"
         "gadget.Analog adds the function getAnalogDdta() for retrieving the\n"
         "received analog data.  This is similar to the additions made by\n"
         "gadget.Position and gadget.Digital."
         ,
         init<  >()
        )
        .def("writeObject",
             (vpr::ReturnStatus (gadget::Analog::*)(vpr::ObjectWriter*) )&gadget::Analog::writeObject,
             (vpr::ReturnStatus (pyj::gadget_Analog_Wrapper::*)(vpr::ObjectWriter*))&pyj::gadget_Analog_Wrapper::default_writeObject,
             "writeObject(writer) -> vpr.ReturnStatus object\n"
             "Serializes this object."
         )
        .def("readObject",
             (vpr::ReturnStatus (gadget::Analog::*)(vpr::ObjectReader*) )&gadget::Analog::readObject,
             (vpr::ReturnStatus (pyj::gadget_Analog_Wrapper::*)(vpr::ObjectReader*))&pyj::gadget_Analog_Wrapper::default_readObject,
             "readObject(reader) -> vpr.ReturnStatus object\n"
             "De-serializes this object."
         )
        .def("config", &gadget::Analog::config,
             &pyj::gadget_Analog_Wrapper::default_config,
             "config(element) -> Boolean\n"
             "Reads the minimum and maximum value configuration information\n"
             "for this analog device.\n"
             "Arguments:\n"
             "element -- The config element for an analog device.  It must\n"
             "           derive from the base config element type\n"
             "           'analog_device'."
         )
        .def("getInputTypeName", &gadget::Analog::getInputTypeName,
             &pyj::gadget_Analog_Wrapper::default_getInputTypeName)
        .def("getAnalogData", &gadget::Analog::getAnalogData,
             (args("devNum") = 0),
             "getAnalogData(devNum = 0) -> gadget.AnalogData object\n"
             "Returns analog data.\n"
             "Post-condition:\n"
             "A value is returned in the range from 0.0 to 1.0\n"
             "Keyword arguments:\n"
             "devNum -- The device unit numbmer to access.  This parameter\n"
             "          optional.  It defaults to 0."
         )
        .def("addAnalogSample", &gadget::Analog::addAnalogSample,
             "addAnalogSample(sampleList)\n"
             "Helper method to add a collection of samples to the analog\n"
             "sample buffers for this device.  This MUST be called by all\n"
             "analog devices to add a new sample.  The data samples passed\n"
             "in will then be modified by any local filters.\n"
             "Post-condition:\n"
             "The given samples are added to the buffers and the local\n"
             "filters are run on the new samples."
             "Arguments:\n"
             "sampleList -- The list of newly collected samples as a\n"
             "              gadget.AnalogDataVec object."
         )
        .def("swapAnalogBuffers", &gadget::Analog::swapAnalogBuffers,
             "swapAnalogBuffers()\n"
             "Swaps the analog data buffers.\n"
             "Post-condition:\n"
             "If the ready queue has values, then those values are copied\n"
             "from the ready queue to the stable queue.  If not, the stable\n"
             "queue is not changed."
         )
        .def("getAnalogDataBuffer", &gadget::Analog::getAnalogDataBuffer,
             return_internal_reference< 1 >(),
             "getAnalogDataBuffer() -> list of lists of AnalogData objects\n"
             "Returns the current stable sample buffers for this device."
         )
    ;

    class_< std::vector<gadget::AnalogData> >("AnalogDataVec",
        "An indexable container of gadget.AnalogData objects."
        )
        .def(vector_indexing_suite< std::vector<gadget::AnalogData> >())
    ;

}
