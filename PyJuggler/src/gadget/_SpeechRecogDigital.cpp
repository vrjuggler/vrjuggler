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
#include <gadget/Type/SpeechRecogDigital.h>

// Using =======================================================================
using namespace boost::python;

// Declarations ================================================================
namespace pyj {

struct gadget_SpeechRecogDigital_Wrapper: gadget::SpeechRecogDigital
{
    gadget_SpeechRecogDigital_Wrapper(PyObject* self_):
        gadget::SpeechRecogDigital(), self(self_) {}

    bool config(jccl::ConfigElementPtr p0) {
        return call_method< bool >(self, "config", p0);
    }

    bool default_config(jccl::ConfigElementPtr p0) {
        return gadget::SpeechRecogDigital::config(p0);
    }

    std::string getInputTypeName() {
        return call_method< std::string >(self, "getInputTypeName");
    }

    std::string default_getInputTypeName() {
        return gadget::SpeechRecogDigital::getInputTypeName();
    }

    vpr::ReturnStatus writeObject(vpr::ObjectWriter* p0) {
        return call_method< vpr::ReturnStatus >(self, "writeObject", p0);
    }

    vpr::ReturnStatus default_writeObject(vpr::ObjectWriter* p0) {
        return gadget::SpeechRecogDigital::writeObject(p0);
    }

    vpr::ReturnStatus readObject(vpr::ObjectReader* p0) {
        return call_method< vpr::ReturnStatus >(self, "readObject", p0);
    }

    vpr::ReturnStatus default_readObject(vpr::ObjectReader* p0) {
        return gadget::SpeechRecogDigital::readObject(p0);
    }

    PyObject* self;
};

}// namespace 


// Module ======================================================================
void _Export_SpeechRecogDigital()
{
    class_< gadget::SpeechRecogDigital, boost::noncopyable, pyj::gadget_SpeechRecogDigital_Wrapper >("SpeechRecogDigital",
         "gadget.SpeechRecogDigital is the abstract base class for devices\n"
         "that translate spoken commands into integer-identified commands.\n"
         "Drivers for all such devices must derive from this class.  This is\n"
         "in addition to gadget.Input.  gadget.Input provides pure virtual\n"
         "function constraints in the following functions: startSampling(),\n"
         "stopSampling(), sample(), and updateData().\n\n"
         "gadget.SpeechRecogDigital adds the function getDigitalDdta() for\n"
         "retrieving the received commands.  This is similar to the\n"
         "additions made by gadget.Position and gadget.Analog."
         ,
         init<  >()
        )
        .def("config", &gadget::SpeechRecogDigital::config,
             &pyj::gadget_SpeechRecogDigital_Wrapper::default_config,
             "config(element) -> Boolean\n"
             "Configures this digital speech recognition device.\n"
             "Arguments:\n:"
             "element -- The config element for a digital speech recognition\n"
             "           device.  It must derive from the base config\n"
             "           element type 'speech_recog_digital_device'."
         )
        .def("getInputTypeName", &gadget::SpeechRecogDigital::getInputTypeName,
             &pyj::gadget_SpeechRecogDigital_Wrapper::default_getInputTypeName
         )
        .def("writeObject",
             (vpr::ReturnStatus (gadget::SpeechRecogDigital::*)(vpr::ObjectWriter*) )&gadget::SpeechRecogDigital::writeObject,
             (vpr::ReturnStatus (pyj::gadget_SpeechRecogDigital_Wrapper::*)(vpr::ObjectWriter*))&pyj::gadget_SpeechRecogDigital_Wrapper::default_writeObject,
             "writeObject(writer) -> vpr.ReturnStatus object\n"
             "Serializes this object."
         )
        .def("readObject",
             (vpr::ReturnStatus (gadget::SpeechRecogDigital::*)(vpr::ObjectReader*) )&gadget::SpeechRecogDigital::readObject, (vpr::ReturnStatus (pyj::gadget_SpeechRecogDigital_Wrapper::*)(vpr::ObjectReader*))&pyj::gadget_SpeechRecogDigital_Wrapper::default_readObject,
             "readObject(reader) -> vpr.ReturnStatus object\n"
             "De-serializes this object."
         )
        .def("getDigitalData", &gadget::SpeechRecogDigital::getDigitalData,
             (args("devNum") = 0),
             "getDigitalData(devNum = 0) -> gadget.DigitalData object\n"
             "Returns digital data.\n"
             "Keyword arguments:\n"
             "devNum -- The device unit numbmer to access.  This parameter\n"
             "          optional.  It defaults to 0."
         )
        .def("addDigitalSample", &gadget::SpeechRecogDigital::addDigitalSample,
             "addDigitalSample(sampleList)\n"
             "Helper method to add a collection of samples to the digital\n"
             "sample buffers for this device.  This MUST be called by all\n"
             "digital devices to add a new sample.\n"
             "Post-condition:\n"
             "The given samples are added to the buffers.\n"
             "Arguments:\n"
             "sampleList -- The list of newly collected samples."
         )
        .def("swapDigitalBuffers",
             &gadget::SpeechRecogDigital::swapDigitalBuffers,
             "swapDigitalBuffers()\n"
             "Swaps the digital data buffers.\n"
             "Post-condition:\n"
             "If the ready queue has values, then those values are copied\n"
             "from the ready queue to the stable queue.  If not, the stable\n"
             "queue is not changed."
         )
        .def("getDigitalDataBuffer",
             &gadget::SpeechRecogDigital::getDigitalDataBuffer,
             return_value_policy< copy_const_reference >(),
             "getDigitalDataBuffer() -> list of lists of DigitalData objects\n"
             "Returns the current stable sample buffers for this device."
         )
    ;

}
