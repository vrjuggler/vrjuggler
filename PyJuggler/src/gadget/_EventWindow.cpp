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
#include <gadget/Type/EventWindow.h>

// Using =======================================================================
using namespace boost::python;

// Declarations ================================================================
namespace pyj {

struct gadget_EventWindow_Wrapper: gadget::EventWindow
{
    gadget_EventWindow_Wrapper(PyObject* self_, const gadget::EventWindow& p0):
        gadget::EventWindow(p0), self(self_) {}

    gadget_EventWindow_Wrapper(PyObject* self_):
        gadget::EventWindow(), self(self_) {}

    std::string getInputTypeName() {
        return call_method< std::string >(self, "getInputTypeName");
    }

    std::string default_getInputTypeName() {
        return gadget::EventWindow::getInputTypeName();
    }

    vpr::ReturnStatus writeObject(vpr::ObjectWriter* p0) {
        return call_method< vpr::ReturnStatus >(self, "writeObject", p0);
    }

    vpr::ReturnStatus default_writeObject(vpr::ObjectWriter* p0) {
        return gadget::EventWindow::writeObject(p0);
    }

    vpr::ReturnStatus readObject(vpr::ObjectReader* p0) {
        return call_method< vpr::ReturnStatus >(self, "readObject", p0);
    }

    vpr::ReturnStatus default_readObject(vpr::ObjectReader* p0) {
        return gadget::EventWindow::readObject(p0);
    }

    bool config(jccl::ConfigElementPtr p0) {
        return call_method< bool >(self, "config", p0);
    }

    bool default_config(jccl::ConfigElementPtr p0) {
        return gadget::EventWindow::config(p0);
    }

    PyObject* self;
};


}// namespace 


// Module ======================================================================
void _Export_EventWindow()
{
    class_< gadget::EventWindow, boost::noncopyable, pyj::gadget_EventWindow_Wrapper >("EventWindow",
         "gadget.EventWindow is an abstract class for interfacing with\n"
         "keyboard and mouse devices.  Informally, an event window can be\n"
         "thought of as a map of keyboard and mouse events (presses,\n"
         "releases, and movements) to integers.  The integers indicate the\n"
         "number of times the event occurred since the last update.  That is\n"
         "to say, gadget.EventWindow counts the number of keyboard and mouse\n"
         "events between updates.  Updates in Juggler occur once per frame."
         ,
         init<  >()
        )
        .def(init< const gadget::EventWindow& >())
        .def("getInputTypeName", &gadget::EventWindow::getInputTypeName,
             &pyj::gadget_EventWindow_Wrapper::default_getInputTypeName)
        .def("writeObject",
             (vpr::ReturnStatus (gadget::EventWindow::*)(vpr::ObjectWriter*) )&gadget::EventWindow::writeObject,
             (vpr::ReturnStatus (pyj::gadget_EventWindow_Wrapper::*)(vpr::ObjectWriter*))&pyj::gadget_EventWindow_Wrapper::default_writeObject,
             "writeObject(writer) -> vpr.ReturnStatus object\n"
             "Writes both mCurKeys and mCurEventQueueLock to a stream using\n"
             "the given object writer."
         )
        .def("readObject",
             (vpr::ReturnStatus (gadget::EventWindow::*)(vpr::ObjectReader*) )&gadget::EventWindow::readObject,
             (vpr::ReturnStatus (pyj::gadget_EventWindow_Wrapper::*)(vpr::ObjectReader*))&pyj::gadget_EventWindow_Wrapper::default_readObject,
             "readObject(reader) -> vpr.ReturnStatus object\n"
             "Reads mCurKeys and mCurEventQueueLock from a stream using the\n"
             "given object reader."
         )
        .def("config", &gadget::EventWindow::config,
             &pyj::gadget_EventWindow_Wrapper::default_config,
             "config(element) -> Boolean\n"
             "Configures this event window device.\n"
             "Arguments:\n:"
             "element -- The config element for an event window device."
         )
        .def("getSyncTime", &gadget::EventWindow::getSyncTime,
             "getSyncTime -> vpr.Interval object\n"
             "Gets the interval that will be used for synchronization while\n"
             "only sharing keyboard data across the cluster."
         )
        .def("keyPressed", &gadget::EventWindow::keyPressed,
             "keyPressed(keyId) -> int\n"
             "Returns the number of times the given key was pressesd during\n"
             "the last frame.  This can be used in a conditional expression\n"
             "to determine if the key was pressed at all."
             "Arguments:\n"
             "keyId -- A value from gadget.Keys identifiing the key whose\n"
             "         state will be queried.\n"
             "Returns:\n"
             "The number of times the named key was pressed since the last\n"
             "update."
         )
        .def("modifierOnly", &gadget::EventWindow::modifierOnly,
             "modifierOnly(modKey) -> Boolean\n"
             "Determines if the given modifier key is the only modifier\n"
             "pressed.\n"
             "Pre-conditions:\n"
             "The given key identifier must be one of gadget.Keys.NONE,\n"
             "gadget.Keys.KEY_ALT, gadget.Keys.KEY_CTRL, or\n"
             "gadget.Keys.KET_SHIFT.\n"
             "Arguments:\n"
             "modKey -- A gadget.Keys value that must be one of\n"
             "          gadget.Keys.NONE, gadget.Keys.KEY_ALT,\n"
             "          gadget.Keys.KEY_CTRL, or gadget.Keys.KEY_SHIFT.\n"
             "Returns:\n"
             "True is returned if the given modifier is the only modifier\n"
             "key pressed."
         )
        .def("getKeyName", &gadget::EventWindow::getKeyName,
             "getKeyName(keyId) -> string object\n"
             "Returns the symbolic Gadgeteer name associated with the given\n"
             "key identifier.  For example, getKeyName(gadget.Keys.KEY_UP)\n"
             "returns \"KEY_UP\".\n"
             "Arguments:\n"
             "keyId -- A value from gadget.Keys identifiing the key whose\n"
             "         name will be returned.\n"
             "Returns:\n"
             "A string that is the symbolic name of the given key."
         )
        .def("getEventQueue", &gadget::EventWindow::getEventQueue,
             "getEventQueue() -> list of gadget.Event objects\n"
             "Returns a copy of the current queue of events for this window."
         )
        .def("addEvent", &gadget::EventWindow::addEvent,
             "addEvent(event)\n"
             "Adds the given event object to the in-progress queue."
             "Arguments:\n"
             "event -- The gadget.Event object that is the new event."
         )
    ;

}
