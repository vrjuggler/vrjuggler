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
#include <gadget/Type/Command.h>

// Using =======================================================================
using namespace boost::python;

// Declarations ================================================================
namespace pyj {

struct gadget_Command_Wrapper: gadget::Command
{
    gadget_Command_Wrapper(PyObject* self_):
        gadget::Command(), self(self_) {}

    bool config(jccl::ConfigElementPtr p0) {
        return call_method< bool >(self, "config", p0);
    }

    bool default_config(jccl::ConfigElementPtr p0) {
        return gadget::Command::config(p0);
    }

    std::string getInputTypeName() {
        return call_method< std::string >(self, "getInputTypeName");
    }

    std::string default_getInputTypeName() {
        return gadget::Command::getInputTypeName();
    }

    vpr::ReturnStatus writeObject(vpr::ObjectWriter* p0) {
        return call_method< vpr::ReturnStatus >(self, "writeObject", p0);
    }

    vpr::ReturnStatus default_writeObject(vpr::ObjectWriter* p0) {
        return gadget::Command::writeObject(p0);
    }

    vpr::ReturnStatus readObject(vpr::ObjectReader* p0) {
        return call_method< vpr::ReturnStatus >(self, "readObject", p0);
    }

    vpr::ReturnStatus default_readObject(vpr::ObjectReader* p0) {
        return gadget::Command::readObject(p0);
    }

    PyObject* self;
};

}// namespace 


// Module ======================================================================
void _Export_Command()
{
    class_< gadget::Command, boost::noncopyable, pyj::gadget_Command_Wrapper >("Command",
         "gadget.Command is the abstract base class for devices that\n"
         "translate spoken commands into integer-identified commands.\n"
         "Drivers for all such devices must derive from this class.  This is\n"
         "in addition to gadget.Input.  gadget.Input provides pure virtual\n"
         "function constraints in the following functions: startSampling(),\n"
         "stopSampling(), sample(), and updateData().\n\n"
         "gadget.Command adds the function getCommandDdta() for retrieving\n"
         "the received commands.  This is similar to the additions made by\n"
         "gadget.Position and gadget.Analog."
         ,
         init<  >()
        )
        .def("config", &gadget::Command::config,
             &pyj::gadget_Command_Wrapper::default_config,
             "config(element) -> Boolean\n"
             "Configures this command-oriented device.\n"
             "Arguments:\n"
             "element -- The config element for a command-oriented device.\n"
             "           It must derive from the base config element type\n"
             "           'command_device'."
         )
        .def("getInputTypeName", &gadget::Command::getInputTypeName,
             &pyj::gadget_Command_Wrapper::default_getInputTypeName
         )
        .def("writeObject",
             (vpr::ReturnStatus (gadget::Command::*)(vpr::ObjectWriter*) )&gadget::Command::writeObject,
             (vpr::ReturnStatus (pyj::gadget_Command_Wrapper::*)(vpr::ObjectWriter*))&pyj::gadget_Command_Wrapper::default_writeObject,
             "writeObject(writer) -> vpr.ReturnStatus object\n"
             "Serializes this object."
         )
        .def("readObject",
             (vpr::ReturnStatus (gadget::Command::*)(vpr::ObjectReader*) )&gadget::Command::readObject, (vpr::ReturnStatus (pyj::gadget_Command_Wrapper::*)(vpr::ObjectReader*))&pyj::gadget_Command_Wrapper::default_readObject,
             "readObject(reader) -> vpr.ReturnStatus object\n"
             "De-serializes this object."
         )
        .def("getCommandData", &gadget::Command::getCommandData,
             (args("devNum") = 0),
             "getCommandData(devNum = 0) -> gadget.CommandData object\n"
             "Returns command data.\n"
             "Keyword arguments:\n"
             "devNum -- The device unit numbmer to access.  This parameter\n"
             "          optional.  It defaults to 0."
         )
        .def("addCommandSample", &gadget::Command::addCommandSample,
             "addCommandSample(sampleList)\n"
             "Helper method to add a collection of samples to the command\n"
             "sample buffers for this device.  This MUST be called by all\n"
             "command-oriented devices to add a new sample.\n"
             "Post-condition:\n"
             "The given samples are added to the buffers.\n"
             "Arguments:\n"
             "sampleList -- The list of newly collected samples as a\n"
             "              gadget.DigitalDataVec object."
         )
        .def("swapCommandBuffers",
             &gadget::Command::swapCommandBuffers,
             "swapCommandBuffers()\n"
             "Swaps the command data buffers.\n"
             "Post-condition:\n"
             "If the ready queue has values, then those values are copied\n"
             "from the ready queue to the stable queue.  If not, the stable\n"
             "queue is not changed."
         )
        .def("getCommandDataBuffer",
             &gadget::Command::getCommandDataBuffer,
             return_value_policy< copy_const_reference >(),
             "getCommandDataBuffer() -> list of lists of CommandData objects\n"
             "Returns the current stable sample buffers for this device."
         )
    ;

}
