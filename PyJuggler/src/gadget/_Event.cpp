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
#include <gadget/Type/EventWindow/Event.h>

// Using =======================================================================
using namespace boost::python;

// Declarations ================================================================
namespace pyj {

struct gadget_Event_Wrapper: gadget::Event
{
    gadget_Event_Wrapper(PyObject* self_, const gadget::Event& p0):
        gadget::Event(p0), self(self_) {}

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
        return gadget::Event::writeObject(p0);
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
        return gadget::Event::readObject(p0);
    }

    PyObject* self;
};

}// namespace 


// Module ======================================================================
void _Export_Event()
{
    scope* gadget_Event_scope = new scope(
    class_< gadget::Event, pyj::gadget_Event_Wrapper >("Event",
         "Base event type that an event source may generate.  This class\n"
         "cannot be used directly.  Subclasses must be used instead."
         ,
         init< const gadget::Event& >()
        )
        .def("writeObject",
            (vpr::ReturnStatus (gadget::Event::*)(vpr::ObjectWriter*) )&gadget::Event::writeObject,
            (vpr::ReturnStatus (pyj::gadget_Event_Wrapper::*)(vpr::ObjectWriter*))&pyj::gadget_Event_Wrapper::default_writeObject,
            "writeObject(writer) -> vpr.ReturnStatus object\n"
            "Serializes this object."
         )
        .def("readObject",
             (vpr::ReturnStatus (gadget::Event::*)(vpr::ObjectReader*) )&gadget::Event::readObject,
             (vpr::ReturnStatus (pyj::gadget_Event_Wrapper::*)(vpr::ObjectReader*))&pyj::gadget_Event_Wrapper::default_readObject,
            "readObject(reader) -> vpr.ReturnStatus object\n"
            "De-serializes this object."
         )
        .def("type", &gadget::Event::type,
             return_value_policy< copy_const_reference >(),
             "type() -> gadget.EventType object\n"
             "Returns the type of this event.  This can be used for dynamic\n"
             "casting to more specific event types."
         )
        .def("setType", &gadget::Event::setType,
             "setType(type)\n"
             "Sets the type of this event.  This is needed because, while\n"
             "using an object reader to de-serialize a gadget.Event object,\n"
             "we cannot set the type during construction.  We must set the\n"
             "event type after creating this event using the EventFactory."
             "Arguments:\n"
             "type -- A gadget.EventType object or a subclass thereof."
         )
        .def("time", &gadget::Event::time,
             return_value_policy< copy_const_reference >(),
             "time() -> int\n"
             "Returns the time at which the event occurred."
         )
    );
    register_ptr_to_python< boost::shared_ptr< gadget::Event > >();
    delete gadget_Event_scope;

}
