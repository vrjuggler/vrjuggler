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
#include <gadget/Type/EventWindowProxy.h>

// Using =======================================================================
using namespace boost::python;

// Declarations ================================================================
namespace pyj {

struct gadget_EventWindowProxy_Wrapper: gadget::EventWindowProxy
{
    gadget_EventWindowProxy_Wrapper(PyObject* self_, const gadget::EventWindowProxy& p0):
        gadget::EventWindowProxy(p0), self(self_) {}

    gadget_EventWindowProxy_Wrapper(PyObject* self_):
        gadget::EventWindowProxy(), self(self_) {}

    vpr::Interval getTimeStamp() const {
        return call_method< vpr::Interval >(self, "getTimeStamp");
    }

    vpr::Interval default_getTimeStamp() const {
        return gadget::EventWindowProxy::getTimeStamp();
    }

    bool config(jccl::ConfigElementPtr p0) {
        return call_method< bool >(self, "config", p0);
    }

    bool default_config(jccl::ConfigElementPtr p0) {
        return gadget::EventWindowProxy::config(p0);
    }

    void set(const std::basic_string<char,std::char_traits<char>,std::allocator<char> >& p0, gadget::EventWindow* p1) {
        call_method< void >(self, "set", p0, p1);
    }

    void default_set(const std::basic_string<char,std::char_traits<char>,std::allocator<char> >& p0, gadget::EventWindow* p1) {
        gadget::TypedProxy<gadget::EventWindow>::set(p0, p1);
    }

    bool refresh() {
        return call_method< bool >(self, "refresh");
    }

    bool default_refresh() {
        return gadget::TypedProxy<gadget::EventWindow>::refresh();
    }

    std::basic_string<char,std::char_traits<char>,std::allocator<char> > getDeviceName() const {
        return call_method< std::basic_string<char,std::char_traits<char>,std::allocator<char> > >(self, "getDeviceName");
    }

    std::basic_string<char,std::char_traits<char>,std::allocator<char> > default_getDeviceName() const {
        return gadget::TypedProxy<gadget::EventWindow>::getDeviceName();
    }

    void updateData() {
        call_method< void >(self, "updateData");
    }

    void default_updateData() {
        gadget::Proxy::updateData();
    }

    bool isStupified() const {
        return call_method< bool >(self, "isStupified");
    }

    bool default_isStupified() const {
        return gadget::Proxy::isStupified();
    }

    PyObject* self;
};

}// namespace 


// Module ======================================================================
void _Export_EventWindowProxy()
{
    class_< gadget::EventWindowProxy, pyj::gadget_EventWindowProxy_Wrapper >("EventWindowProxy",
         "A proxy class to event window devices used by the Input Manager.\n\n"
         "An event window proxy always points to an event window device and\n"
         "a unit number within that device.  The Input Manager can therefore\n"
         "keep an array of these around and treat them as event window\n"
         "devices that only return a single sub-device's amount of data\n"
         "(an event queue and individual keys)."
         ,
         init<  >()
        )
        .def(init< const gadget::EventWindowProxy& >())
        .def("getTimeStamp",
             (vpr::Interval (gadget::EventWindowProxy::*)() const)&gadget::EventWindowProxy::getTimeStamp,
             (vpr::Interval (pyj::gadget_EventWindowProxy_Wrapper::*)() const)&pyj::gadget_EventWindowProxy_Wrapper::default_getTimeStamp,
             "getTimeStamp() -> vpr.Interval object\n"
             "Returns the time of the last update."
         )
        .def("config",
             (bool (gadget::EventWindowProxy::*)(jccl::ConfigElementPtr) )&gadget::EventWindowProxy::config,
             (bool (pyj::gadget_EventWindowProxy_Wrapper::*)(jccl::ConfigElementPtr))&pyj::gadget_EventWindowProxy_Wrapper::default_config,
             "config(element) -> Boolean\n"
             "Configures this proxy using the given jccl.ConfigElement\n"
             "instance."
         )
        .def("set",
             (void (gadget::TypedProxy<gadget::EventWindow>::*)(const std::basic_string<char,std::char_traits<char>,std::allocator<char> >&, gadget::EventWindow*) )&gadget::TypedProxy<gadget::EventWindow>::set, (void (pyj::gadget_EventWindowProxy_Wrapper::*)(const std::basic_string<char,std::char_traits<char>,std::allocator<char> >&,
             gadget::EventWindow*))&pyj::gadget_EventWindowProxy_Wrapper::default_set,
             "set(devName, devPtr)\n"
             "Sets the proxy to point to the given type-specific device.\n"
             "Pre-condition:\n"
             "devPtr must be a valid device of type gadget.Digital\n"
             "Post-condition:\n"
             "The proxy now references the given device.  The device name we\n"
             "are proxying is set to devPtr.getInstanceName()."
             "Arguments:\n"
             "devName -- The name of the device at which we are pointing.\n"
             "devPtr  -- Pointer to the device.  For gadget.Digital,\n"
             "           this points to an instance of gadget.Digital."
         )
        .def("refresh",
             (bool (gadget::TypedProxy<gadget::EventWindow>::*)() )&gadget::TypedProxy<gadget::EventWindow>::refresh,
             (bool (pyj::gadget_EventWindowProxy_Wrapper::*)())&pyj::gadget_EventWindowProxy_Wrapper::default_refresh,
             "refresh() -> Boolean\n"
             "Refreshes the proxy.  This attempts to lookup the device that\n"
             "we are proxying.  If the lookup fails, then we become\n"
             "stupified.  If not, then the proxy is pointed at this\n"
             "potentially new device."
         )
        .def("getDeviceName",
             (std::basic_string<char,std::char_traits<char>,std::allocator<char> > (gadget::TypedProxy<gadget::EventWindow>::*)() const)&gadget::TypedProxy<gadget::EventWindow>::getDeviceName,
             (std::basic_string<char,std::char_traits<char>,std::allocator<char> > (pyj::gadget_EventWindowProxy_Wrapper::*)() const)&pyj::gadget_EventWindowProxy_Wrapper::default_getDeviceName,
             "getDeviceName() -> string object\n"
             "Gets the name of the device that we are proxying."
         )
        .def("updateData",
             (void (gadget::Proxy::*)() )&gadget::Proxy::updateData,
             (void (pyj::gadget_EventWindowProxy_Wrapper::*)())&pyj::gadget_EventWindowProxy_Wrapper::default_updateData,
             "updateData()\n"
             "Updates the cached data copy from the device.  Copies the\n"
             "device data to local storage and transforms it if necessary."
         )
        .def("isStupified",
             (bool (gadget::Proxy::*)() const)&gadget::Proxy::isStupified,
             (bool (pyj::gadget_EventWindowProxy_Wrapper::*)() const)&pyj::gadget_EventWindowProxy_Wrapper::default_isStupified,
             "isStupified() -> Boolean\n"
             "Is the proxy currently stupified?\n"
             "If the device we are proxying does not exist, then this will\n"
             "return True."
         )
        .def("getEventWindowPtr",
             &gadget::EventWindowProxy::getEventWindowPtr,
             return_internal_reference< 1 >(),
             "getEventWindowPtr() -> gadget.EventWindow object\n"
             "Returns the gadget.EventWindow object held by this proxy."
         )
        .def("modifierOnly", &gadget::EventWindowProxy::modifierOnly,
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
        .def("keyPressed", &gadget::EventWindowProxy::keyPressed,
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
        .def("getEventQueue", &gadget::EventWindowProxy::getEventQueue,
             "getEventQueue() -> list of gadget.Event objects\n"
             "Returns a copy of the current queue of events for the proxied\n"
             "window."
         )
        .def("getElementType", &gadget::EventWindowProxy::getElementType)
        .def("getName", &gadget::Proxy::getName,
             "getName() -> string object\n"
             "Gets the name of the proxy."
         )
        .def("setName", &gadget::Proxy::setName,
             "setName(name)\n"
             "Sets the name of the proxy.\n"
             "Arguments:\n"
             "name -- The name for this proxy as a string object."
         )
        .def("stupify", &gadget::Proxy::stupify,
             "stupify(newState = True)\n"
             "Sets the stupification state of this proxy.\n"
             "Keyword arguments:\n"
             "newState -- The new state of stupification."
         )
        .staticmethod("getElementType")
    ;

}
