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
#include <gadget/Type/SpeechRecogString.h>

// Using =======================================================================
using namespace boost::python;

// Declarations ================================================================
namespace pyj {

struct gadget_SpeechRecogString_Wrapper: gadget::SpeechRecogString
{
    gadget_SpeechRecogString_Wrapper(PyObject* self_):
        gadget::SpeechRecogString(), self(self_) {}

    bool config(jccl::ConfigElementPtr p0) {
        return call_method< bool >(self, "config", p0);
    }

    bool default_config(jccl::ConfigElementPtr p0) {
        return gadget::SpeechRecogString::config(p0);
    }

    std::string getInputTypeName() {
        return call_method< std::string >(self, "getInputTypeName");
    }

    std::string default_getInputTypeName() {
        return gadget::SpeechRecogString::getInputTypeName();
    }

    vpr::ReturnStatus writeObject(vpr::ObjectWriter* p0) {
        return call_method< vpr::ReturnStatus >(self, "writeObject", p0);
    }

    vpr::ReturnStatus default_writeObject(vpr::ObjectWriter* p0) {
        return gadget::SpeechRecogString::writeObject(p0);
    }

    vpr::ReturnStatus readObject(vpr::ObjectReader* p0) {
        return call_method< vpr::ReturnStatus >(self, "readObject", p0);
    }

    vpr::ReturnStatus default_readObject(vpr::ObjectReader* p0) {
        return gadget::SpeechRecogString::readObject(p0);
    }

    PyObject* self;
};

}// namespace 


// Module ======================================================================
void _Export_SpeechRecogString()
{
    class_< gadget::SpeechRecogString, boost::noncopyable, pyj::gadget_SpeechRecogString_Wrapper >("SpeechRecogString",
         "gadget.SpeechRecogString is the abstract base class for devices\n"
         "that return spoken commands.  Drivers for all such devices must\n"
         "derive from this class.  This is in addition to gadget.Input.\n"
         "gadget.Input provides pure virtual function constraints in the\n"
         "following functions: startSampling(), stopSampling(), sample(),\n"
         "and updateData().\n\n"
         "gadget.SpeechRecogString adds the function getStringDdta() for\n"
         "retrieving the received commands.  This is similar to the\n"
         "additions made by gadget.Position and gadget.Analog."
         ,
         init<  >()
        )
        .def("config", &gadget::SpeechRecogString::config,
             &pyj::gadget_SpeechRecogString_Wrapper::default_config,
             "config(element) -> Boolean\n"
             "Configures this speech recognition device.\n"
             "Arguments:\n"
             "element -- The config element for a digital speech recognition\n"
             "           device.  It must derive from the base config\n"
             "           element type 'speech_recog_string_device'."
         )
        .def("getInputTypeName", &gadget::SpeechRecogString::getInputTypeName,
             &pyj::gadget_SpeechRecogString_Wrapper::default_getInputTypeName
         )
        .def("writeObject",
             (vpr::ReturnStatus (gadget::SpeechRecogString::*)(vpr::ObjectWriter*) )&gadget::SpeechRecogString::writeObject,
             (vpr::ReturnStatus (pyj::gadget_SpeechRecogString_Wrapper::*)(vpr::ObjectWriter*))&pyj::gadget_SpeechRecogString_Wrapper::default_writeObject,
             "writeObject(writer) -> vpr.ReturnStatus object\n"
             "Serializes this object."
         )
        .def("readObject",
             (vpr::ReturnStatus (gadget::SpeechRecogString::*)(vpr::ObjectReader*) )&gadget::SpeechRecogString::readObject, (vpr::ReturnStatus (pyj::gadget_SpeechRecogString_Wrapper::*)(vpr::ObjectReader*))&pyj::gadget_SpeechRecogString_Wrapper::default_readObject,
             "readObject(reader) -> vpr.ReturnStatus object\n"
             "De-serializes this object."
         )
        .def("getStringData", &gadget::SpeechRecogString::getStringData,
             (args("devNum") = 0),
             "getStringData(devNum = 0) -> gadget.StringData object\n"
             "Returns string data.\n"
             "Keyword arguments:\n"
             "devNum -- The device unit numbmer to access.  This parameter\n"
             "          optional.  It defaults to 0."
         )
        .def("addStringSample", &gadget::SpeechRecogString::addStringSample,
             "addStringSample(sampleList)\n"
             "Helper method to add a collection of samples to the digital\n"
             "sample buffers for this device.  This MUST be called by all\n"
             "digital devices to add a new sample.\n"
             "Post-condition:\n"
             "The given samples are added to the buffers.\n"
             "Arguments:\n"
             "sampleList -- The list of newly collected samples."
         )
        .def("swapStringBuffers",
             &gadget::SpeechRecogString::swapStringBuffers,
             "swapStringBuffers()\n"
             "Swaps the digital data buffers.\n"
             "Post-condition:\n"
             "If the ready queue has values, then those values are copied\n"
             "from the ready queue to the stable queue.  If not, the stable\n"
             "queue is not changed."
         )
        .def("getStringDataBuffer",
             &gadget::SpeechRecogString::getStringDataBuffer,
             return_value_policy< copy_const_reference >(),
             "getStringDataBuffer() -> list of lists of StringData objects\n"
             "Returns the current stable sample buffers for this device."
         )
    ;

}
