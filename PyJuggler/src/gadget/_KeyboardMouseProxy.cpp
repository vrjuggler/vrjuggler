// PyJuggler is (C) Copyright 2002-2005 by Patrick Hartling
// Distributed under the GNU Lesser General Public License 2.1.  (See
// accompanying file COPYING.txt or http://www.gnu.org/copyleft/lesser.txt)

// File:          $RCSfile$
// Date modified: $Date$
// Version:       $Revision$

// Boost Includes ==============================================================
#include <boost/python.hpp>
#include <boost/cstdint.hpp>

// Includes ====================================================================
#include <gadget/Type/KeyboardMouseProxy.h>

// Using =======================================================================
using namespace boost::python;

// Declarations ================================================================
namespace pyj {

struct gadget_KeyboardMouseProxy_Wrapper: gadget::KeyboardMouseProxy
{
    gadget_KeyboardMouseProxy_Wrapper(PyObject* self_, const gadget::KeyboardMouseProxy& p0):
        gadget::KeyboardMouseProxy(p0), self(self_) {}

    gadget_KeyboardMouseProxy_Wrapper(PyObject* self_):
        gadget::KeyboardMouseProxy(), self(self_) {}

    vpr::Interval getTimeStamp() const {
        return call_method< vpr::Interval >(self, "getTimeStamp");
    }

    vpr::Interval default_getTimeStamp() const {
        return gadget::KeyboardMouseProxy::getTimeStamp();
    }

    bool config(jccl::ConfigElementPtr p0) {
        return call_method< bool >(self, "config", p0);
    }

    bool default_config(jccl::ConfigElementPtr p0) {
        return gadget::KeyboardMouseProxy::config(p0);
    }

    void set(const std::basic_string<char,std::char_traits<char>,std::allocator<char> >& p0, gadget::KeyboardMouse* p1) {
        call_method< void >(self, "set", p0, p1);
    }

    void default_set(const std::basic_string<char,std::char_traits<char>,std::allocator<char> >& p0, gadget::KeyboardMouse* p1) {
        gadget::TypedProxy<gadget::KeyboardMouse>::set(p0, p1);
    }

    bool refresh() {
        return call_method< bool >(self, "refresh");
    }

    bool default_refresh() {
        return gadget::TypedProxy<gadget::KeyboardMouse>::refresh();
    }

    std::basic_string<char,std::char_traits<char>,std::allocator<char> > getDeviceName() const {
        return call_method< std::basic_string<char,std::char_traits<char>,std::allocator<char> > >(self, "getDeviceName");
    }

    std::basic_string<char,std::char_traits<char>,std::allocator<char> > default_getDeviceName() const {
        return gadget::TypedProxy<gadget::KeyboardMouse>::getDeviceName();
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
void _Export_KeyboardMouseProxy()
{
    class_< gadget::KeyboardMouseProxy, pyj::gadget_KeyboardMouseProxy_Wrapper >("KeyboardMouseProxy",
         "A proxy class to keyboard/mouse devices used by the Input Manager.\n\n"
         "A keyboard/mouse proxy always points to a keyboard/mouse device\n"
         "and a unit number within that device.  The Input Manager can\n"
         "therefore keep an array of these around and treat them as\n"
         "keyboard/mouse devices that only return a single sub-device's\n"
         "amount of data (an event queue and individual keys)."
         ,
         init<  >()
        )
        .def(init< const gadget::KeyboardMouseProxy& >())
        .def("getTimeStamp",
             (vpr::Interval (gadget::KeyboardMouseProxy::*)() const)&gadget::KeyboardMouseProxy::getTimeStamp,
             (vpr::Interval (pyj::gadget_KeyboardMouseProxy_Wrapper::*)() const)&pyj::gadget_KeyboardMouseProxy_Wrapper::default_getTimeStamp,
             "getTimeStamp() -> vpr.Interval object\n"
             "Returns the time of the last update."
         )
        .def("config",
             (bool (gadget::KeyboardMouseProxy::*)(jccl::ConfigElementPtr) )&gadget::KeyboardMouseProxy::config,
             (bool (pyj::gadget_KeyboardMouseProxy_Wrapper::*)(jccl::ConfigElementPtr))&pyj::gadget_KeyboardMouseProxy_Wrapper::default_config,
             "config(element) -> Boolean\n"
             "Configures this proxy using the given jccl.ConfigElement\n"
             "instance."
         )
        .def("set",
             (void (gadget::TypedProxy<gadget::KeyboardMouse>::*)(const std::basic_string<char,std::char_traits<char>,std::allocator<char> >&, gadget::KeyboardMouse*) )&gadget::TypedProxy<gadget::KeyboardMouse>::set, (void (pyj::gadget_KeyboardMouseProxy_Wrapper::*)(const std::basic_string<char,std::char_traits<char>,std::allocator<char> >&,
             gadget::KeyboardMouse*))&pyj::gadget_KeyboardMouseProxy_Wrapper::default_set,
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
             (bool (gadget::TypedProxy<gadget::KeyboardMouse>::*)() )&gadget::TypedProxy<gadget::KeyboardMouse>::refresh,
             (bool (pyj::gadget_KeyboardMouseProxy_Wrapper::*)())&pyj::gadget_KeyboardMouseProxy_Wrapper::default_refresh,
             "refresh() -> Boolean\n"
             "Refreshes the proxy.  This attempts to lookup the device that\n"
             "we are proxying.  If the lookup fails, then we become\n"
             "stupified.  If not, then the proxy is pointed at this\n"
             "potentially new device."
         )
        .def("getDeviceName",
             (std::basic_string<char,std::char_traits<char>,std::allocator<char> > (gadget::TypedProxy<gadget::KeyboardMouse>::*)() const)&gadget::TypedProxy<gadget::KeyboardMouse>::getDeviceName,
             (std::basic_string<char,std::char_traits<char>,std::allocator<char> > (pyj::gadget_KeyboardMouseProxy_Wrapper::*)() const)&pyj::gadget_KeyboardMouseProxy_Wrapper::default_getDeviceName,
             "getDeviceName() -> string object\n"
             "Gets the name of the device that we are proxying."
         )
        .def("updateData",
             (void (gadget::Proxy::*)() )&gadget::Proxy::updateData,
             (void (pyj::gadget_KeyboardMouseProxy_Wrapper::*)())&pyj::gadget_KeyboardMouseProxy_Wrapper::default_updateData,
             "updateData()\n"
             "Updates the cached data copy from the device.  Copies the\n"
             "device data to local storage and transforms it if necessary."
         )
        .def("isStupified",
             (bool (gadget::Proxy::*)() const)&gadget::Proxy::isStupified,
             (bool (pyj::gadget_KeyboardMouseProxy_Wrapper::*)() const)&pyj::gadget_KeyboardMouseProxy_Wrapper::default_isStupified,
             "isStupified() -> Boolean\n"
             "Is the proxy currently stupified?\n"
             "If the device we are proxying does not exist, then this will\n"
             "return True."
         )
        .def("getKeyboardMousePtr",
             &gadget::KeyboardMouseProxy::getKeyboardMousePtr,
             return_internal_reference< 1 >(),
             "getKeyboardMousePtr() -> gadget.KeyboardMouse object\n"
             "Returns the gadget.KeyboardMouse object held by this proxy."
         )
        .def("modifierOnly", &gadget::KeyboardMouseProxy::modifierOnly,
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
        .def("keyPressed", &gadget::KeyboardMouseProxy::keyPressed,
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
        .def("getEventQueue", &gadget::KeyboardMouseProxy::getEventQueue,
             "getEventQueue() -> list of gadget.Event objects\n"
             "Returns a copy of the current queue of events for the proxied\n"
             "device."
         )
        .def("getElementType", &gadget::KeyboardMouseProxy::getElementType)
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
