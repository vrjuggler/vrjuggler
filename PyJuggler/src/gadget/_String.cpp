// PyJuggler is (C) Copyright 2002-2005 by Patrick Hartling
// Distributed under the GNU Lesser General Public License 2.1.  (See
// accompanying file COPYING.txt or http://www.gnu.org/copyleft/lesser.txt)

// File:          $RCSfile$
// Date modified: $Date$
// Version:       $Revision$

// Includes ====================================================================
#include <boost/python.hpp>
#include <boost/cstdint.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>

// Includes ====================================================================
#include <vpr/IO/ObjectReader.h>
#include <vpr/IO/ObjectWriter.h>
#include <gadget/Type/String.h>

// Using =======================================================================
using namespace boost::python;

// Declarations ================================================================
namespace pyj {

struct gadget_String_Wrapper: gadget::String
{
    gadget_String_Wrapper(PyObject* self_):
        gadget::String(), self(self_) {}

    bool config(jccl::ConfigElementPtr p0) {
        return call_method< bool >(self, "config", p0);
    }

    bool default_config(jccl::ConfigElementPtr p0) {
        return gadget::String::config(p0);
    }

    std::string getInputTypeName() {
        return call_method< std::string >(self, "getInputTypeName");
    }

    std::string default_getInputTypeName() {
        return gadget::String::getInputTypeName();
    }

    vpr::ReturnStatus writeObject(vpr::ObjectWriter* p0) {
        return call_method< vpr::ReturnStatus >(self, "writeObject", p0);
    }

    vpr::ReturnStatus default_writeObject(vpr::ObjectWriter* p0) {
        return gadget::String::writeObject(p0);
    }

    vpr::ReturnStatus readObject(vpr::ObjectReader* p0) {
        return call_method< vpr::ReturnStatus >(self, "readObject", p0);
    }

    vpr::ReturnStatus default_readObject(vpr::ObjectReader* p0) {
        return gadget::String::readObject(p0);
    }

    PyObject* self;
};

}// namespace 


// Module ======================================================================
void _Export_String()
{
    class_< gadget::String, boost::noncopyable, pyj::gadget_String_Wrapper >("String",
         "gadget.String is the abstract base class for devices that return\n"
         "strinsg.  Drivers for all such devices must derive from this class.\n"
         "This is in addition to gadget.Input.  gadget.Input provides pure\n"
         "virtual function constraints in the following functions:\n"
         "startSampling(), stopSampling(), sample(), and updateData().\n\n"
         "gadget.String adds the function getStringDdta() for\n"
         "retrieving the received commands.  This is similar to the\n"
         "additions made by gadget.Position and gadget.Analog."
         ,
         init<  >()
        )
        .def("config", &gadget::String::config,
             &pyj::gadget_String_Wrapper::default_config,
             "config(element) -> Boolean\n"
             "Configures this string device.\n"
             "Arguments:\n"
             "element -- The config element for a string device.  It must\n"
             "           derive from the base config element type\n"
             "           'string_device'."
         )
        .def("getInputTypeName", &gadget::String::getInputTypeName,
             &pyj::gadget_String_Wrapper::default_getInputTypeName
         )
        .def("writeObject",
             (vpr::ReturnStatus (gadget::String::*)(vpr::ObjectWriter*) )&gadget::String::writeObject,
             (vpr::ReturnStatus (pyj::gadget_String_Wrapper::*)(vpr::ObjectWriter*))&pyj::gadget_String_Wrapper::default_writeObject,
             "writeObject(writer) -> vpr.ReturnStatus object\n"
             "Serializes this object."
         )
        .def("readObject",
             (vpr::ReturnStatus (gadget::String::*)(vpr::ObjectReader*) )&gadget::String::readObject, (vpr::ReturnStatus (pyj::gadget_String_Wrapper::*)(vpr::ObjectReader*))&pyj::gadget_String_Wrapper::default_readObject,
             "readObject(reader) -> vpr.ReturnStatus object\n"
             "De-serializes this object."
         )
        .def("getStringData", &gadget::String::getStringData,
             (args("devNum") = 0),
             "getStringData(devNum = 0) -> gadget.StringData object\n"
             "Returns string data.\n"
             "Keyword arguments:\n"
             "devNum -- The device unit numbmer to access.  This parameter\n"
             "          optional.  It defaults to 0."
         )
        .def("addStringSample", &gadget::String::addStringSample,
             "addStringSample(sampleList)\n"
             "Helper method to add a collection of samples to the digital\n"
             "sample buffers for this device.  This MUST be called by all\n"
             "digital devices to add a new sample.\n"
             "Post-condition:\n"
             "The given samples are added to the buffers.\n"
             "Arguments:\n"
             "sampleList -- The list of newly collected samples as a\n"
             "              gadget.StringDataVec object."
         )
        .def("swapStringBuffers", &gadget::String::swapStringBuffers,
             "swapStringBuffers()\n"
             "Swaps the digital data buffers.\n"
             "Post-condition:\n"
             "If the ready queue has values, then those values are copied\n"
             "from the ready queue to the stable queue.  If not, the stable\n"
             "queue is not changed."
         )
        .def("getStringDataBuffer", &gadget::String::getStringDataBuffer,
             return_value_policy< copy_const_reference >(),
             "getStringDataBuffer() -> list of lists of StringData objects\n"
             "Returns the current stable sample buffers for this device."
         )
    ;

    class_< std::vector<gadget::StringData> >("StringDataVec",
        "An indexable container of gadget.StringData objects."
        )
        .def(vector_indexing_suite< std::vector<gadget::StringData> >())
    ;

}
